#pragma once
#include"Dependencias.h" //.h donde guardar dependencias

template<typename S, typename I, typename D> //String, INT y  DOUBLE
class Pelicula
{
public:
	//Constructor y destructor de los Nodos
	Pelicula(S titulo, S generos, I ano, D puntuacions, I views){
		title = titulo; genero = generos; year = ano; puntuacion = puntuacions; view = views;
		next = previous = nullptr;
	}
	~Pelicula(){}

	S title, genero;
	I year, view;
	D puntuacion;
	Pelicula<S, I, D>* next,* previous;

};

template<typename S, typename I, typename D>
class ListaCircular
{
public:
	ListaCircular(){
		head = tail = nullptr;
	}
	~ListaCircular(){}
	void Agregar_final(S title, S genero, I year, D punt, I views) {
		Pelicula<S, I, D>* nuevo = new Pelicula<S, I, D>(title, genero, year, punt, views);
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
	vector<Pelicula<S, I, D>*> OrdenarCalificacion() {
		vector<Pelicula<S, I, D>*> vectorAux;
		Pelicula<S, I, D>* curr = head;
		do {
			vectorAux.push_back(curr);
			curr = curr->next;
		} while (curr != head);

		for (int i = 0; i < vectorAux.size(); i++) {
			for (int j = 0; j < vectorAux.size() - 1; j++) {
				if (vectorAux[j]->puntuacion <= vectorAux[j + 1]->puntuacion) {
					swap(vectorAux[j], vectorAux[j + 1]);
				}
			}
		}

		return vectorAux;
	}
	vector<Pelicula<S, I, D>*> OrdenarAlfabeticamente() {
		vector<Pelicula<S, I, D>*> vectorAux;
		Pelicula<S, I, D>* curr = head;
		do {
			vectorAux.push_back(curr);
			curr = curr->next;
		} while (curr != head);
		for (int i = 0; i < vectorAux.size(); i++) {
			for (int j = 0; j < vectorAux.size() - 1; j++) {
				char letraJ = vectorAux[j]->title[0], letraJ1 = vectorAux[j+1]->title[0];
				if (int(letraJ) - 65 >= int(letraJ1) - 65) {
					swap(vectorAux[j], vectorAux[j+1]);
				}
			}
		}


		return vectorAux;
	}
	vector<Pelicula<S, I, D>*> OrdenarPorAnio() {
		vector<Pelicula<S, I, D>*> vectorAux;
		Pelicula<S, I, D>* curr = head;
		do {
			vectorAux.push_back(curr);
			curr = curr->next;
		} while (curr != head);

		for (int i = 0; i < vectorAux.size(); i++) {
			for (int j = 0; j < vectorAux.size() - 1; j++) {
				if (vectorAux[j]->year <= vectorAux[j + 1]->year) {
					swap(vectorAux[j], vectorAux[j + 1]);
				}
			}
		}

		return vectorAux;
	}

private:
	Pelicula<S, I, D>* head,* tail;
};

template<typename S, typename I, typename D>
class Queue
{
public:
	Queue(){
		front = end = nullptr;
	}
	~Queue(){}



private:
	Pelicula<S, I, D>* front, * end;
};
