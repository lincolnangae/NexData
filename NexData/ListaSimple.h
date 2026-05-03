#pragma once
#include "Nodo.h"

template <typename T>
class ListaSimple {
private:
    Nodo<T>* cabeza;
    int capacidad;
    int actual;

public:
    ListaSimple(int _capacidad = 3) : cabeza(nullptr), capacidad(_capacidad), actual(0) {}

    void insertarAlInicio(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);

        if (actual == capacidad) {
            eliminarUltimo();
        }

        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        actual++;
    }

    void eliminarUltimo() {
        if (!cabeza) return;
        if (!cabeza->siguiente) {
            delete cabeza;
            cabeza = nullptr;
        }
        else {
            Nodo<T>* temp = cabeza;
            while (temp->siguiente->siguiente) {
                temp = temp->siguiente;
            }
            delete temp->siguiente;
            temp->siguiente = nullptr;
        }
        actual--;
    }

    Nodo<T>* getCabeza() { return cabeza; }
};