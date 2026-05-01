#pragma once
#include"Dependencias.h" //.h donde guardar dependencias


template<typename S, typename I, typename D> //String, INT y  DOUBLE
class Pelicula
{
public:
	//Constructor y destructor de los Nodos
	Pelicula(S titulo, S generos, I ano, D puntuacions){
		title = titulo; genero = generos; year = ano; puntuacion = puntuacions;
		next = previous = nullptr;
	}
	~Pelicula(){}

	S title, genero;
	I year;
	D puntuacion;
	Pelicula* next, prepvious;

};

template<typename S, typename I, typename D>
class ListaCircular
{
public:
	ListaCircular(){
		head = tail = nullptr;
	}
	~ListaCircular(){}
	void Agregar_final(S title, S genero, I year, D punt) {
		Pelicula<S, I, D>* nuevo = new Pelicula<S, I, D>(title, genero, year, punt);
		if (head == nullptr) {
			head = tail = nuevo;
			nuevo->next = head;
		}
		else {
			tail->next = nuevo;
			tail = nuevo;
			tail->next = head;
		}
	}

private:
	Pelicula<S, I, D>* head, tail;
};
