#include "pch.h"
#include <iostream>
#include <conio.h>
using namespace std;
#include "GestionadorCatalogo.h"

void EjecutarMenu(Gestionador& sistema) {
    char tecla = ' ';
    while (tecla != '0') {
        system("cls");
        TipoTop TmpSeleccion = TipoTop::MASVISTAS;

        Console::ForegroundColor = ConsoleColor::Green;

        Gotoxy(GetAnchoVentana() / 2 - 20, 0); cout << "========================================" << endl;
        Gotoxy(GetAnchoVentana() / 2 - 20, 1); cout << "             NEXT --- DATA              " << endl;
        Gotoxy(GetAnchoVentana() / 2 - 20, 2); cout << "========================================" << endl;

        Console::ForegroundColor = ConsoleColor::Cyan;
        cout << "\n------------------------------------------------------------------" << endl;
        cout << "ORDENAR POR: [C]alificacion | A[N]io de Lanzamiento | [A]lfabetico" << endl;
        cout << "GUARDAR PELICULA DEL CATALOGO PARA ' VER MAS TARDE ' : [G]" << endl;
        cout << "VER LISTA DE ' VER MAS TARDE ' : [V]" << endl;

        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ LO MAS VISTO ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTops(TipoTop::MASVISTAS);
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ RECIENTEMENTE AGREGADOS ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTops(TipoTop::RECIENTES);
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ RECIENTEMENTE VISTOS POR USUARIO ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTops(TipoTop::VISTOSRECIENTES);


        Console::ForegroundColor = ConsoleColor::DarkBlue;
        cout << "Presione (1-9) para ver detalle || 0 para Salir: ";


        Console::ForegroundColor = ConsoleColor::White;
        //Capturador de tecla
        tecla = _getch();
        if (tecla == '0') break;

        //Que lista elegir
        if (tecla >= '1' && tecla <= '3') {
            TmpSeleccion = TipoTop::MASVISTAS;
        }
        else if (tecla >= '4' && tecla <= '6') {
            TmpSeleccion = TipoTop::RECIENTES;
        }
        else if (tecla >= '7' && tecla <= '9') {
            TmpSeleccion = TipoTop::VISTOSRECIENTES;
        }

        // Imprimir lista elegida
        if (tecla >= '1' && tecla <= '9') {
            system("cls");
            int opcionNum = tecla - '0';
            cout << ">>> DETALLE DE PELICULA SELECCIONADA [" << opcionNum << "] <<<" << endl;
            sistema.ImprimirInformacion(TmpSeleccion, opcionNum);
        }

        else if (tecla == 'c' || tecla == 'C') {
            system("cls");
            cout << "[ FILTRANDO POR CALIFICACION... ]" << endl;
            system("pause");
        }
        else if (tecla == 'n' || tecla == 'N') {
            system("cls");
            cout << "[ FILTRANDO POR ANIO DE LANZAMIENTO... ]" << endl;
            system("pause");
        }
        else if (tecla == 'a' || tecla == 'A') {
            system("cls");
            cout << "[ FILTRANDO ALFABETICAMENTE... ]" << endl;
            system("pause");
        }
        else if (tecla == 'g' || tecla == 'G') {
            sistema.GuardarParaVerMasTarde();
        }
        else if (tecla == 'v' || tecla == 'V') {
            sistema.VerListaVerMasTarde();
        }
    }
}

int main() {
    Gestionador miSistema;
    miSistema.cargarDesdeArchivos();

    EjecutarMenu(miSistema);

    miSistema.GuardarDatos();

    return 0;
}