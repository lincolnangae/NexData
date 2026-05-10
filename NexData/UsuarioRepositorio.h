#pragma once
#include "ListaSimple.h"
#include "Dependencias.h"
#include "Usuario.h"

class UsuarioRepositorio {
private:
    ListaSimple<Usuario> usuarios;
public:
    Usuario* Buscar(string nombre) {
        Nodo<Usuario>* temp = usuarios.GetCabeza();
        while (temp != nullptr) {
            if (temp->Dato.GetUsuario() == nombre) {
                return &(temp->Dato);
            }
            temp = temp->siguiente;
        }
        return nullptr;
    }

    void AgregarUsuario(Usuario u) {
        usuarios.InsertarAlInicio(u);
    }

    void Guardar(Usuario u) {
        FileStream^ fs = gcnew FileStream("Datos.bin", FileMode::OpenOrCreate);
        BinaryWriter^ bw = gcnew BinaryWriter(fs);
        bw->Write(marshal_as<String^>(u.GetUsuario()));
        bw->Write(marshal_as<String^>(u.GetContra()));
        bw->Close();
        fs->Close();
    }

    Usuario Cargar() {
        if (!File::Exists("Datos.bin")) return Usuario("", "");
        FileStream^ fs = gcnew FileStream("Datos.bin", FileMode::Open, FileAccess::Read);
        BinaryReader^ br = gcnew BinaryReader(fs);
        string u = marshal_as<string>(br->ReadString());
        string p = marshal_as<string>(br->ReadString());
        br->Close();
        fs->Close();

        Usuario cargado(u, p);
        this->AgregarUsuario(cargado);
        return cargado;
    }
};