#include"pch.h"
#include <iostream>
using namespace std;

#include "GestionadorCatalogo.h"

void EjecutarMenu(Gestionador& sistema) {
    int opcion = -1;

    while (opcion != 0) {
        system("cls");
        Console::ForegroundColor = ConsoleColor::Green;
        cout << "========================================" << endl;
        cout << "             NEXT --- DATA              " << endl;
        cout << "========================================" << endl;

        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ LO MAS VISTO ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.mostrarTopVistas();
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n[ RECIENTES ]\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        sistema.mostrarRecientes();

        cout << "\n----------------------------------------" << endl;
        cout << "0. Salir | Seleccione (1-6) para ver detalle: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (opcion >= 1 && opcion <= 6) {
            system("cls");
            cout << "Cargando detalle de la pelicula " << opcion << "..." << endl;
            system("pause");
        }
    }
}

int main()
{
    Gestionador miSistema;
    miSistema.cargarDesdeArchivos();

    EjecutarMenu(miSistema);

    miSistema.guardarDatos();

    return 0;
}
