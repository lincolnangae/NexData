#pragma once
#include "UsuarioRepositorio.h"
#include "Pila.h"

class ServicioAutenticacion {
private:
    UsuarioRepositorio* repo;
    Usuario* usuarioLogueado;
    Pila<string> logIntentos;

public:
    ServicioAutenticacion(UsuarioRepositorio* r) : repo(r), usuarioLogueado(nullptr) {}

    bool Autenticar(string user, string pass) {
        Usuario* u = repo->Buscar(user);
        if (u != nullptr && u->GetContra() == pass) {
            usuarioLogueado = u;
            return true;
        }
        logIntentos.Push("Fallo: " + user);
        return false;
    }

    bool ExisteCuenta() {
        return File::Exists("Datos.bin");
    }

    void Registrar() {
        string u, p, pc;
        cout << "\n--- REGISTRO DE USUARIO ---" << endl;
        cout << "Ingrese nombre de usuario: "; cin >> u;
        cout << "Ingrese contrasenia: "; cin >> p;
        do {
            cout << "Confirme su contrasenia: "; cin >> pc;
            if (p != pc) cout << "No coinciden. Intente de nuevo." << endl;
        } while (p != pc);

        Usuario nuevo(u, p);
        repo->Guardar(nuevo);
        repo->AgregarUsuario(nuevo);
        cout << "\n¡Cuenta creada correctamente!" << endl;
        system("pause");
    }

    int GetNumeroDeFallos() {
        return logIntentos.ContarElementosRecursivo(logIntentos.GetCima());
    }

    Usuario* GetLoggedUser() { return usuarioLogueado; }
};