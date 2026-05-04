#pragma once
#include "Nodo.h"

template <typename T_Format>
class ListaSimple {
private:
    Nodo<T_Format>* cabeza;
    int capacidad;
    int actual;

public:
    ListaSimple(int _capacidad = 3) : cabeza(nullptr), capacidad(_capacidad), actual(0) {}

    void InsertarAlInicio(T_Format dato) {
        Nodo<T_Format>* nuevo = new Nodo<T_Format>(dato);

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
            Nodo<T_Format>* temp = cabeza;
            while (temp->siguiente->siguiente) {
                temp = temp->siguiente;
            }
            delete temp->siguiente;
            temp->siguiente = nullptr;
        }
        actual--;
    }

    Nodo<T_Format>* getCabeza() { return cabeza; }
};