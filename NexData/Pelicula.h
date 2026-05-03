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

    Pelicula(int ord, const string& title, int year, const vector<string>& cat, T_Format punt, int vol, int visits)
        : Orden(ord), Titulo(title), Lanzamiento(year), Categorias(cat), Puntuacion(punt), Volumen(vol), VistasTotales(visits) {
    }

    void MostrarEnLista() {
        cout << "[" << Orden << "] " << Titulo << " (" << Lanzamiento << ")" << endl;
        cout << "Rating: " << Puntuacion << " | Vistas: " << VistasTotales << endl;
        cout << "Generos: ";
        for (const auto& c : Categorias) cout << c << " ";
        cout << endl;
    }

    void MostrarEnCatalogo(int x, int y) {
        // Lambda para reemplazar '-' por espacios
        auto limpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
            };

        string tituloLimpio = limpiarTexto(Titulo);

        int offsetTitulo = (40 - Titulo.length()) / 2;
        gotoxy(x + offsetTitulo, y);
        cout << ">>> " << tituloLimpio << " <<<" << endl;

        gotoxy(x, y + 2);
        cout << "ANIO : " << Lanzamiento << " | RATING: " << Puntuacion;

        gotoxy(x, y + 4);
        cout << "CATEGORIAS: ";
        for (const auto& c : Categorias) cout << c << " ";

        gotoxy(x, y + 6);
        cout << "VISTAS TOTALES: " << VistasTotales;

        gotoxy(x, y + 8);
        cout << "C - CALIFICAR | S - SALIR";
    }

};