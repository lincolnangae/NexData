#pragma once
#include "Nodo.h"
#include <iostream>

template<typename T>
class Pila {
public:
    Pila() : top(nullptr), tamanio(0) {}
    ~Pila() { while (!IsEmpty()) Pop(); }

    void Push(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        nuevo->siguiente = top;
        top = nuevo;
        tamanio++;
    }

    void Pop() {
        if (!IsEmpty()) {
            Nodo<T>* aux = top;
            top = top->siguiente;
            delete aux;
            tamanio--;
        }
    }

    bool IsEmpty() { return top == nullptr; }

    int ContarElementosRecursivo(Nodo<T>* actual) {
        if (actual == nullptr) return 0;
        return 1 + ContarElementosRecursivo(actual->siguiente);
    }

    int GetTamanio() { return tamanio; }

    Nodo<T>* GetCima() { return top; }

private:
    Nodo<T>* top;
    int tamanio;
};