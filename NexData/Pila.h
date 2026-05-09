#pragma once
#include"Nodo.h"
template<typename T>
class Pila
{
public:
	Pila() {
		top = nullptr;
	}
	~Pila(){}
	void push(T dato) {
		Nodo<T>* nuevo = new Nodo<T>(dato);
		nuevo->siguiente = top;
		top = nuevo;
	}

	Nodo<T>* getCima() {
		return top;
	}
private:
	Nodo<T>* top;
};
