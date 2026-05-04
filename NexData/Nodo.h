#pragma once

//Comunidaor directo con las listas enlazadas
template <typename T_Generico>
class Nodo {
public:
    T_Generico dato;
    Nodo<T_Generico>* siguiente;
    Nodo<T_Generico>* anterior;

    Nodo(T_Generico valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
};