#pragma once
#include <string>
#include <vector>
using namespace std;

class PeliculaMetadata {
public:
    int Orden;
    string Titulo;
    int Lanzamiento;
    vector<string> Categorias;

    PeliculaMetadata(int ord = 0, string title = "Sin Titulo", int year = 0, vector<string> cat = {})
        : Orden(ord), Titulo(title), Lanzamiento(year), Categorias(cat) {
    }
};