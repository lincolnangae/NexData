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
        if (cabeza != nullptr) {
            if (dato.Titulo == cabeza->Dato.Titulo) {
                return;
            }
        }

        if (actual >= capacidad) {
            EliminarUltimo();
        }

        Nodo<T_Format>* nuevo = new Nodo<T_Format>(dato);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        actual++;
    }

    void InsertarEnPosicion(int indx, T_Format dato) {
        if (cabeza != nullptr) {
            if (dato.Titulo == cabeza->Dato.Titulo) {
                return;
            }
        }

		Nodo<T_Format>* nuevo = new Nodo<T_Format>(dato);
        Nodo<T_Format>* curr = cabeza;
        if (indx == 1) {
			nuevo->siguiente = cabeza;
			cabeza = nuevo;
		}
		else {
			for (int i = 1; i < indx - 1 && curr != nullptr; i++) {
				curr = curr->siguiente;
			}
			if (curr != nullptr) {
				nuevo->siguiente = curr->siguiente;
				curr->siguiente = nuevo;
			}
        }
    }
    //Como dijimos, en el menu principal solo se mostraran 3 peliculas
    //Esta logica evita que cualquiera de estas tenga mas de  3 elementos
    void EliminarUltimo() {
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

    Nodo<T_Format> GetCabeza2() { cabeza; }

    Nodo<T_Format>* GetCabeza() { return cabeza; }
};