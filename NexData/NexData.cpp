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
    char tecla;
    bool autenticado = false;
    char b = (char)219;
    int xCentro = GetAnchoVentana() / 2 - 20;

    while (!autenticado) {
        system("cls");

        // Cabecera con Bloques (Estilo Sólido)
        Console::ForegroundColor = ConsoleColor::Yellow;
        Gotoxy(xCentro, 0); for (int i = 0; i < 40; i++) cout << b;
        Gotoxy(xCentro, 1); cout << b << "       SISTEMA DE CATALOGO - NEXT       " << b;
        Gotoxy(xCentro, 2); for (int i = 0; i < 40; i++) cout << b;

        if (!auth.ExisteCuenta()) {
            Console::ForegroundColor = ConsoleColor::Cyan;
            Gotoxy(xCentro, 4); cout << "[!] No se detecto ninguna cuenta activa.";
            Gotoxy(xCentro, 5); cout << "Redirigiendo al registro...";
            Console::ForegroundColor = ConsoleColor::White;
            _getch();
            auth.Registrar();
        }
        else {
            // Cuerpo del Menú
            Console::ForegroundColor = ConsoleColor::Cyan;
            Gotoxy(xCentro, 4); cout << "----------------------------------------";
            Gotoxy(xCentro, 5); cout << "[1] Iniciar Sesion  |  [2] Nueva Cuenta ";
            Gotoxy(xCentro, 6); cout << "[0] Salir del Sistema";
            Gotoxy(xCentro, 7); cout << "----------------------------------------";

            Console::ForegroundColor = ConsoleColor::White;
            Gotoxy(xCentro, 9); cout << "Seleccione una opcion: ";
            tecla = _getch();

            if (tecla == '1') {
                string user, pass;
                system("cls");

                // Banner de Login
                Console::ForegroundColor = ConsoleColor::Yellow;
                Gotoxy(xCentro, 0); for (int i = 0; i < 20; i++) cout << b;
                Gotoxy(xCentro, 1); cout << b << "      LOGIN       " << b;
                Gotoxy(xCentro, 2); for (int i = 0; i < 20; i++) cout << b;

                Console::ForegroundColor = ConsoleColor::White;
                Gotoxy(xCentro, 4); cout << "Usuario: "; cin >> user;
                Gotoxy(xCentro, 5); cout << "Password: "; cin >> pass;

                if (auth.Autenticar(user, pass)) {
                    autenticado = true;
                    Console::ForegroundColor = ConsoleColor::Green;
                    Gotoxy(xCentro, 7); cout << ">> Acceso concedido. Bienvenido.";
                    Console::ForegroundColor = ConsoleColor::White;
                    _getch();
                }
                else {
                    Console::ForegroundColor = ConsoleColor::Red;
                    Gotoxy(xCentro, 7); cout << ">> Error de credenciales.";
                    Gotoxy(xCentro, 8); cout << ">> Fallos: " << auth.GetNumeroDeFallos();
                    Console::ForegroundColor = ConsoleColor::White;
                    _getch();
                }
            }
            else if (tecla == '2') {
                auth.Registrar();
            }
            else if (tecla == '0') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // 1. Inicialización de Repositorios y Servicios
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

        miSistema.GuardarPila();
    }

    return 0;
}