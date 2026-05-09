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

    Nodo<T>* GetCabeza() { return cabeza; }
    int GetCantidad() { return cantidad; }

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

    void InsertarAlInicio(T dato) {

        if (cabeza != nullptr) {
            if (dato.Titulo == cabeza->Dato.Titulo) {
                return;
            }
        }

        InsertarAlFinal(dato);

        cabeza = cabeza->anterior;
    }

    void InsertarEnPosicion(int indx, T dato) {
        if (indx <= 1) {
            // Por ahora, usaremos tu lógica existente si está vacía
            InsertarAlFinal(dato);
            return;
        }

        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (!cabeza) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        }
        else {
            Nodo<T>* curr = cabeza;
            // Avanzar hasta la posición anterior a la deseada
            for (int i = 1; i < indx - 1; i++) {
                curr = curr->siguiente;
                if (curr == cabeza) break; 
            }

            Nodo<T>* sig = curr->siguiente;
            nuevo->siguiente = sig;
            nuevo->anterior = curr;
            curr->siguiente = nuevo;
            sig->anterior = nuevo;
        }
        cantidad++;
    }
    void Clear() {

        if (cabeza == nullptr) return;

        Nodo<T>* curr = cabeza->siguiente;
        while (curr != cabeza) {
            Nodo<T>* temp = curr;
            curr = curr->siguiente;
            delete temp;
        }

        delete cabeza;
        cabeza = nullptr;
        cantidad = 0;
    }

};