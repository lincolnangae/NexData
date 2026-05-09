#pragma once
#include "Dependencias.h"

class User
{
public:
	User(string user, string contra) {
		usuario = user; contrasena = contra;
	}
	~User(){}
	string getUser() { return usuario; }
	string getPass() { return contrasena; }

private:
	string usuario, contrasena;
};
