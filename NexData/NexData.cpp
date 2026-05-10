#include "pch.h"
#include <iostream>
#include <conio.h>
using namespace std;
#include "GestionadorCatalogo.h"
#include "ServicioAutenticacion.h"

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
        cout << "\n------------------------------------------------------------------" << endl;
        cout << "GUARDAR PELICULA DEL CATALOGO PARA VER MAS TARDE : [G]" << endl;
        cout << "\n------------------------------------------------------------------" << endl;
        cout << "VER LISTA DE VER MAS TARDE : [V]" << endl;
        cout << endl << "------------------------------------------------------------------" << endl<<"Ver Historia historico [H]"<<endl<<endl;

        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ LO MAS VISTO ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTopMenus(TipoTop::MASVISTAS);
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ RECIENTEMENTE AGREGADOS ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTopMenus(TipoTop::RECIENTES);
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ RECIENTEMENTE VISTOS POR USUARIO ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.MostrarTopMenus(TipoTop::VISTOSRECIENTES);


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
            sistema.OrdenarYMostrar('C');
        }
        else if (tecla == 'n' || tecla == 'N') {
            sistema.OrdenarYMostrar('N');
        }
        else if (tecla == 'a' || tecla == 'A') {
            sistema.OrdenarYMostrar('A');
        }
        else if (tecla == 'g' || tecla == 'G') {
            sistema.GuardarParaVerMasTarde();
        }
        else if (tecla == 'v' || tecla == 'V') {
            sistema.VerListaVerMasTarde();
        }
        else if (tecla == 'h' || tecla == 'H') {
            sistema.MostrarListaHistorico();
        }
    }
}

bool FaseAutenticacion(ServicioAutenticacion& auth) {
    char opcion;
    bool autenticado = false;

    while (!autenticado) {
        system("cls");
        cout << "========================================" << endl;
        cout << "       SISTEMA DE CATALOGO - NEXT       " << endl;
        cout << "========================================" << endl;

        if (!auth.ExisteCuenta()) {
            auth.Registrar();
        }
        else {
            cout << "1. Iniciar Sesion" << endl;
            cout << "2. Crear Nueva Cuenta (Sobrescribir)" << endl;
            cout << "0. Salir" << endl;
            cout << "\nOpcion: ";
            opcion = _getch();

            if (opcion == '1') {
                string user, pass;
                system("cls");
                cout << "--- LOGIN ---" << endl;
                cout << "Usuario: "; cin >> user;
                cout << "Contraseña: "; cin >> pass;

                if (auth.Autenticar(user, pass)) {
                    autenticado = true;
                    cout << "\nBienvenido al sistema." << endl;
                    system("pause");
                }
                else {
                    cout << "\nError. Fallos en esta sesion: " << auth.GetNumeroDeFallos() << endl;
                    system("pause");
                }
            }
            else if (opcion == '2') {
                auth.Registrar();
            }
            else if (opcion == '0') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    UsuarioRepositorio repoUsuarios;
    ServicioAutenticacion auth(&repoUsuarios);
    Gestionador miSistema;

    if (auth.ExisteCuenta()) {
        repoUsuarios.Cargar();
    }

    if (FaseAutenticacion(auth)) {
        miSistema.Iniciar();
        miSistema.CargarHistorial();

        EjecutarMenu(miSistema);

        miSistema.Guardar();
    }

    return 0;
}