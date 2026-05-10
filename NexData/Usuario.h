#pragma once
#include "Dependencias.h"

class Usuario
{
public:
	Usuario(string u = "", string p = "") : usuario(u), contrasena(p) {}

	~Usuario(){}
	string GetUsuario() { return usuario; }
	string GetContra() { return contrasena; }

	bool operator==(const Usuario& otro) const {
		return this->usuario == otro.usuario;
	}

private:
	string usuario, contrasena;
};
