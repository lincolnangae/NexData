#pragma once
#include <iostream>
using namespace std;
#include "Nodo.h"

template <typename T>
class ListaCircularDoble {
private:
    Nodo<T>* cabeza;
    int cantidad;

public:
    ListaCircularDoble() : cabeza(nullptr), cantidad(0) {}

    void InsertarAlFinal(T dato) {

        if (cabeza != nullptr) {
            if (dato.Titulo == cabeza->Dato.Titulo) {
                return;
            }
        }

        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (!cabeza) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        }
        else {
            Nodo<T>* ultimo = cabeza->anterior;
            ultimo->siguiente = nuevo;
            nuevo->anterior = ultimo;
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
        }
        cantidad++;
    }

};