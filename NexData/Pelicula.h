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

		Console::ForegroundColor = ConsoleColor::DarkYellow;
        cout << "[" << Orden << "] " << tituloLimpio << " (" << Lanzamiento << ")" << endl;
		Console::ForegroundColor = ConsoleColor::DarkGreen;
		cout << "Rating: ";
		Console::ForegroundColor = ConsoleColor::White;
		cout<< Puntuacion << " | Vistas: " << VistasTotales << endl;
        cout << "Generos: ";
        for (const auto& c : Categorias) cout << c << " ";
        cout << endl;
    }

    void ImprimirInfoExtendida(int y, bool estaGuardada = false) {

        auto limpiarTexto = [](string texto) {
            for (char& c : texto) if (c == '-') c = ' ';
            return texto;
            };
        auto GotoxSimplified = [](int x, int y) { Gotoxy(x, y); };

        auto DibujarLinea = [](int ancho, int color) {
            setColor(color);
            for (int i = 0; i < ancho; i++) cout << char(205);
            setColor(15);
            cout << endl;
            };

        int anchoV = GetAnchoVentana();
        string tituloLimpio = limpiarTexto(Titulo);

        GotoxSimplified(0, 0);
        setColor(8); cout << "[ "; setColor(14); cout << "C"; setColor(8); cout << " ] Calificar  ";
        setColor(8); cout << "[ "; setColor(11); cout << "V"; setColor(8); cout << " ] Reproducir  ";
        setColor(8); cout << "[ "; setColor(10); cout << "G"; setColor(8); cout << " ] Guardar  ";
        setColor(8); cout << "[ "; setColor(12); cout << "S"; setColor(8); cout << " ] Volver";
        setColor(15);

        int posX_Titulo = (anchoV / 2) - (tituloLimpio.length() / 2) - 5;
        GotoxSimplified(posX_Titulo, y + 2);
        setColor(11); cout << ">>> ";
        setColor(14); cout << tituloLimpio;
        setColor(11); cout << " <<<";

        GotoxSimplified(5, y + 3);
        DibujarLinea(anchoV - 10, 3);

        setColor(11); GotoxSimplified(5, y + 5); cout << "DETALLES TECNICOS:";
        setColor(15);
        GotoxSimplified(7, y + 6); cout << char(175) << " LANZAMIENTO: "; setColor(14); cout << Lanzamiento;
        setColor(15); GotoxSimplified(7, y + 7); cout << char(175) << " CALIFICACION: ";
        if (Puntuacion > 8) setColor(10); else if (Puntuacion > 5) setColor(14); else setColor(12);
        cout << Puntuacion << "/10";

        setColor(15); GotoxSimplified(7, y + 8); cout << char(175) << " CATEGORIAS: ";
        setColor(3);
        for (size_t i = 0; i < Categorias.size(); i++) {
            cout << Categorias[i] << (i < Categorias.size() - 1 ? " | " : "");
        }

        int colDerecha = anchoV / 2 + 10;
        setColor(11); GotoxSimplified(colDerecha, y + 5); cout << "ESTADISTICAS DE COMUNIDAD:";
        setColor(15);
        GotoxSimplified(colDerecha + 2, y + 6); cout << char(250) << " VISTAS: "; setColor(11); cout << VistasTotales;
        setColor(15); GotoxSimplified(colDerecha + 2, y + 7); cout << char(250) << " VOTANTES: "; setColor(11); cout << Volumen;

        if (estaGuardada) {
            GotoxSimplified(colDerecha + 2, y + 9);
            setColor(10);
            cout << char(21) << " ITEM EN TU LISTA DE DESEOS";
        }

        GotoxSimplified(5, y + 11);
        DibujarLinea(anchoV - 10, 3);
        setColor(15);
    }
};