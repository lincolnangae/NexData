#pragma once
#include <iostream>
#include <vector>
using namespace std;
#include "PeliculaEstadisticas.h"
#include "PeliculaMetadata.h"
#include "Complementos.h"

//Lista de datos a guardar de los TXT
template <typename T_Format>
class Pelicula {
private:
    PeliculaMetadata* metadata;
    PeliculaStats<T_Format>* stats;

public:
    Pelicula(int ord = 0, string title = "Sin Titulo", int year = 0,
        vector<string> cat = {}, T_Format punt = 0, int vol = 0, int visits = 0) {
        metadata = new PeliculaMetadata(ord, title, year, cat);
        stats = new PeliculaStats<T_Format>(punt, vol, visits);
    }

    // Getters de Metadata
    int GetOrden() const { return metadata->Orden; }
    string GetTitulo() const { return metadata->Titulo; }
    int GetLanzamiento() const { return metadata->Lanzamiento; }
    vector<string> GetCategorias() const { return metadata->Categorias; }

    // Getters de Estadísticas
    T_Format GetPuntuacion() const { return stats->Puntuacion; }
    int GetVolumen() const { return stats->Volumen; }
    int GetVistasTotales() const { return stats->VistasTotales; }

    // Métodos de acceso a objetos completos si son necesarios
    PeliculaMetadata* GetMetadata() { return metadata; }
    PeliculaStats<T_Format>* GetEstadisticas() { return stats; }

    // settes para actualizar las estadísticas
    void SetPuntuacion(T_Format p) { stats->Puntuacion = p; }
    void SetVolumen(int v) { stats->Volumen = v; }
    void SetVistasTotales(int vt) { stats->VistasTotales = vt; }

    // Métodos para simplificar la lógica de "ver película" o "calificar"
    void AumentarVistas() { stats->VistasTotales++; }

    void ActualizarCalificacion(T_Format calif) {
        double califActual = stats->Puntuacion;
        int volumenActual = stats->Volumen;
        double nuevaSuma = (califActual * volumenActual) + calif;
        stats->Volumen = volumenActual + 1;
        stats->Puntuacion = nuevaSuma / stats->Volumen;
    }

    bool operator==(const Pelicula& otra) const {
        return this->metadata->Titulo == otra.metadata->Titulo;
    }

    void MostrarEnLista() {
        // Lambda para reemplazar el signo - por espacios
        auto limpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
            };
        string tituloLimpio = limpiarTexto(GetTitulo());

		Console::ForegroundColor = ConsoleColor::DarkYellow;
        cout << "[" << GetOrden() << "] " << tituloLimpio << " (" << GetLanzamiento() << ")" << endl;
		Console::ForegroundColor = ConsoleColor::DarkGreen;
		cout << "Rating: ";
		Console::ForegroundColor = ConsoleColor::White;
		cout<< GetPuntuacion() << " | Vistas: " << GetVistasTotales() << endl;
        cout << "Generos: ";
        for (const auto& c : GetCategorias()) cout << c << " ";
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
        string tituloLimpio = limpiarTexto(GetTitulo());

        GotoxSimplified(0, 0);
        setColor(8); cout << "[ "; setColor(14); cout << "C"; setColor(8); cout << " ] Calificar  ";
        setColor(8); cout << "[ "; setColor(11); cout << "V"; setColor(8); cout << " ] Reproducir  ";
        setColor(8); cout << "[ "; setColor(10); cout << "G"; setColor(8); cout << " ] Guardar  ";
        setColor(8); cout << "[ "; setColor(12); cout << "0"; setColor(8); cout << " ] Volver";
        setColor(15);

        int posX_Titulo = ((int)anchoV / 2) - ((int)tituloLimpio.length() / 2) - 5;
        GotoxSimplified(posX_Titulo, y + 2);
        setColor(11); cout << ">>> ";
        setColor(14); cout << tituloLimpio;
        setColor(11); cout << " <<<";

        GotoxSimplified(5, y + 3);
        DibujarLinea(anchoV - 10, 3);

        setColor(11); GotoxSimplified(5, y + 5); cout << "DETALLES TECNICOS:";
        setColor(15);
        GotoxSimplified(7, y + 6); cout << char(175) << " LANZAMIENTO: "; setColor(14); cout << GetLanzamiento();
        setColor(15); GotoxSimplified(7, y + 7); cout << char(175) << " CALIFICACION: ";
        if (GetPuntuacion() > 8) setColor(10); else if (GetPuntuacion() > 5) setColor(14); else setColor(12);
        cout << GetPuntuacion() << "/10";

        setColor(15); GotoxSimplified(7, y + 8); cout << char(175) << " CATEGORIAS: ";
        setColor(3);
        vector<string> cats = GetCategorias();
        for (size_t i = 0; i < cats.size(); i++) {
            cout << cats[i] << (i < cats.size() - 1 ? " | " : "");
        }

        int colDerecha = anchoV / 2 + 10;
        setColor(11); GotoxSimplified(colDerecha, y + 5); cout << "ESTADISTICAS DE COMUNIDAD:";
        setColor(15);
        GotoxSimplified(colDerecha + 2, y + 6); cout << char(250) << " VISTAS: "; setColor(11); cout << GetVistasTotales();
        setColor(15); GotoxSimplified(colDerecha + 2, y + 7); cout << char(250) << " VOTANTES: "; setColor(11); cout << GetVolumen();

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