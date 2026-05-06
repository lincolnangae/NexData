#include "pch.h"
#include <iostream>
#include <conio.h>
using namespace std;
#include "GestionadorCatalogo.h"

void EjecutarMenu(Gestionador& sistema) {
    char tecla = ' ';
    while (tecla != '0') {
        system("cls");

        Console::ForegroundColor = ConsoleColor::Green;
        cout << "========================================" << endl;
        cout << "             NEXT --- DATA              " << endl;
        cout << "========================================" << endl;

        Console::ForegroundColor = ConsoleColor::Cyan;
        cout << "\n----------------------------------------" << endl;
        cout << "ORDENAR POR: [C]alificacion | A[N]io de Lanzamiento | [A]lfabetico" << endl;

        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ LO MAS VISTO ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTops(TipoTop::MASVISTAS);

        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ RECIENTEMENTE AGREGADOS ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTops(TipoTop::RECIENTES);

        Console::ForegroundColor = ConsoleColor::White;
        cout << "Presione (1-6) para ver detalle o 0 para Salir: ";

        //Capturador de tecla
        tecla = _getch();

        if (tecla == '0') break;
        TipoTop TmpSeleccion;
		if (tecla >= '1' && tecla <= '3') TmpSeleccion = TipoTop::MASVISTAS;
		else  TmpSeleccion = TipoTop::RECIENTES;
        if (tecla >= '1' && tecla <= '6') {
            system("cls");
            int opcionNum = tecla - '0';
            cout << ">>> DETALLE DE PELICULA SELECCIONADA [" << opcionNum << "] <<<" << endl;

            sistema.ImprimirInformacion(TmpSeleccion, opcionNum);
            
            gotoxy(0, 11); cout << "\nPresione cualquier tecla para volver al menu...";

            tecla = _getch();
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
    }
}

int main() {
    Gestionador miSistema;
    miSistema.cargarDesdeArchivos();

    EjecutarMenu(miSistema);

    miSistema.GuardarDatos();

    return 0;
}