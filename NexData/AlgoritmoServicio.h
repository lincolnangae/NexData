#pragma once
#include "Pelicula.h"
#include "Nodo.h"

class AlgoritmoServicio {
public:
    template <typename Comparador>
    static void QuickSort(Nodo<Pelicula<double>>* bajo, Nodo<Pelicula<double>>* alto, Comparador comp) {
        if (bajo == nullptr || alto == nullptr || bajo == alto) return;

        Nodo<Pelicula<double>>* p = Particion(bajo, alto, comp);
        if (p != bajo) QuickSort(bajo, p->anterior, comp);
        if (p != alto) QuickSort(p->siguiente, alto, comp);
    }

private:
    template <typename Comparador>
    static Nodo<Pelicula<double>>* Particion(Nodo<Pelicula<double>>* bajo, Nodo<Pelicula<double>>* alto, Comparador comp) {
        Pelicula<double> pivote = alto->Dato;
        Nodo<Pelicula<double>>* i = bajo->anterior;

        for (Nodo<Pelicula<double>>* j = bajo; j != alto; j = j->siguiente) {
            if (comp(j->Dato, pivote)) {
                i = (i == bajo->anterior) ? bajo : i->siguiente;
                swap(i->Dato, j->Dato);
            }
        }
        i = (i == bajo->anterior) ? bajo : i->siguiente;
        swap(i->Dato, alto->Dato);
        return i;
    }
};