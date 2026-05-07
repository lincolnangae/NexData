#pragma once
#include <iostream>
#include <vector>
using namespace std;
#include "Complementos.h"

//Lista de datos a guardar de los TXT
template <typename T_Format>
class Pelicula {
public:
    int Orden;
    string Titulo;
    int Lanzamiento;
    vector<string> Categorias;
    T_Format Puntuacion;
    int Volumen;
    int VistasTotales;

    Pelicula(int ord = 0, const string& title = "Sin Titulo", int year = 0,
        const vector<string>& cat = vector<string>(), T_Format punt = 0,
        int vol = 0, int visits = 0)
        : Orden(ord), Titulo(title), Lanzamiento(year), Categorias(cat),
        Puntuacion(punt), Volumen(vol), VistasTotales(visits) {
    }

    void MostrarEnLista() {
        // Lambda para reemplazar el signo - por espacios
        auto limpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
            };

        string tituloLimpio = limpiarTexto(Titulo);

        cout << "[" << Orden << "] " << tituloLimpio << " (" << Lanzamiento << ")" << endl;
        cout << "Rating: " << Puntuacion << " | Vistas: " << VistasTotales << endl;
        cout << "Generos: ";
        for (const auto& c : Categorias) cout << c << " ";
        cout << endl;
    }
    void ImprimirInfoExtendida(int y) {
        auto limpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
        };
        auto GotoxSimplified = [](int x, int y) {
            gotoxy(x, y);
            };

        cout << endl;
        GotoxSimplified((int)((getAnchoVentana() / 2) - (Titulo.length() / 2) - 3), y + 3);
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "<<<" << limpiarTexto(Titulo) << ">>>";
        Console::ForegroundColor = ConsoleColor::White;
        GotoxSimplified(5, y + 5); cout << "ANIO DE LANZAMIENTO: " << Lanzamiento;
        GotoxSimplified(5, y+6);
        cout<< "RATING: " << Puntuacion << endl;
        GotoxSimplified(5, y + 7);
        cout << "CATEGORIAS: ";
        int indice = 0;
        for (auto g : Categorias) {
            cout << g;
            if (indice < Categorias.size() - 1) cout << " | ";
            indice++;
        }
        cout << endl;
        GotoxSimplified(5, y + 8);
        cout << "VISTAS TOTALES: " << VistasTotales;
        GotoxSimplified(5, y + 9);
		cout << "VOLUMEN DE REPRODUCCION: " << Volumen << endl;
        GotoxSimplified(0, 0);
        cout << "CALIFICAR PELICULA [C] | VER PELICULA [V] | SALIR [S]" << endl;

    }
};