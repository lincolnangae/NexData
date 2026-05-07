#pragma once

//Comunidaor directo con las listas enlazadas
template <typename T_Generico>
class Nodo {
public:
    T_Generico Dato;
    Nodo<T_Generico>* siguiente;
    Nodo<T_Generico>* anterior;

    Nodo(T_Generico valor) : Dato(valor), siguiente(nullptr), anterior(nullptr) {}
};