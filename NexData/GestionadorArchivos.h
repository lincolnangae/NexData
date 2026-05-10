#pragma once
#include "Dependencias.h"
#include "Pelicula.h"
#include "ListaDobleCircular.h"
#include "Pila.h"

class GestionadorArchivos {
public:
    // Carga inicial del catálogo y la lista de recientes
    void CargarTodo(ListaCircularDoble<Pelicula<double>>& catalogo, ListaSimple<Pelicula<double>>& recientes) {
        String^ rutaPelis = "DirectorioPeliculas.txt";
        String^ rutaDatos = "DirectorioDatos.txt";

        if (!File::Exists(rutaPelis) || !File::Exists(rutaDatos)) return;

        StreamReader^ lectorPelis = gcnew StreamReader(rutaPelis);
        StreamReader^ lectorDatos = gcnew StreamReader(rutaDatos);

        try {
            while (!lectorPelis->EndOfStream && !lectorDatos->EndOfStream) {
                String^ lineaP = lectorPelis->ReadLine();
                String^ lineaD = lectorDatos->ReadLine();

                cli::array<String^>^ datosP = lineaP->Split(',');
                cli::array<String^>^ datosD = lineaD->Split(',');

                int orden = Convert::ToInt32(datosP[0]);
                String^ temp = datosP[1];
                string titulo = marshal_as<string>(temp);
                int lanzamiento = Convert::ToInt32(datosP[2]);

                vector<string> categorias;
                for (int i = 3; i < (int)datosP->Length; i++) {
                    String^ temp = datosP[i];
                    categorias.push_back(marshal_as<string>(temp));
                }

                double calificacion = Convert::ToDouble(datosD[1]);
                int volumen = Convert::ToInt32(datosD[2]);
                int vistas = Convert::ToInt32(datosD[3]);

                Pelicula<double> nuevaPeli(orden, titulo, lanzamiento, categorias, calificacion, volumen, vistas);
                catalogo.InsertarAlFinal(nuevaPeli);
                recientes.InsertarAlInicio(nuevaPeli);
            }
        }
        finally {
            lectorPelis->Close();
            lectorDatos->Close();
        }
    }

    // GUARDA EL CATÁLOGO: Puntuaciones y vistas actualizadas
    void GuardarTodo(ListaCircularDoble<Pelicula<double>>& catalogo) {
        String^ rutaDatos = "DirectorioDatos.txt";
        System::Globalization::CultureInfo^ cultura = System::Globalization::CultureInfo::InvariantCulture;
        StreamWriter^ escritor = gcnew StreamWriter(rutaDatos);

        Nodo<Pelicula<double>>* actual = catalogo.GetCabeza();
        if (actual == nullptr) { escritor->Close(); return; }

        try {
            do {
                Pelicula<double>& p = actual->Dato;
                String^ linea = String::Format(cultura, "{0},{1:F2},{2},{3}",
                    p.GetOrden(), p.GetPuntuacion(), p.GetVolumen(), p.GetVistasTotales());
                escritor->WriteLine(linea);
                actual = actual->siguiente;
            } while (actual != catalogo.GetCabeza());
        }
        finally {
            escritor->Close();
        }
    }

    // CARGA EL HISTORIAL: Reconstruye la pila desde el archivo
    void CargarHistorial(ListaCircularDoble<Pelicula<double>>& catalogo, Pila<Pelicula<double>>& historial) {
        String^ ruta = "HistorialVisitado.txt";
        if (!File::Exists(ruta)) return;

        StreamReader^ reader = gcnew StreamReader(ruta);
        vector<Pelicula<double>> tmp;

        try {
            while (!reader->EndOfStream) {
                String^ lineaCompleta = reader->ReadLine();
                if (String::IsNullOrWhiteSpace(lineaCompleta)) continue;
                cli::array<String^>^ partes = lineaCompleta->Split(',');

                if (partes->Length >= 2) {
                    String^ temp = partes[1];
                    string tituloBuscado = marshal_as<string>(temp);
                    Nodo<Pelicula<double>>* curr = catalogo.GetCabeza();
                    if (curr != nullptr) {
                        do {
                            if (curr->Dato.GetTitulo() == tituloBuscado) {
                                tmp.push_back(curr->Dato);
                                break;
                            }
                            curr = curr->siguiente;
                        } while (curr != catalogo.GetCabeza());
                    }
                }
            }
        }
        finally {
            reader->Close();
        }

        reverse(tmp.begin(), tmp.end());
        for (const auto& peli : tmp) historial.Push(peli);
    }

    // GUARDA EL HISTORIAL: Escribe la pila actual al archivo
    void GuardarHistorial(Pila<Pelicula<double>>& historial) {
        String^ ruta = "HistorialVisitado.txt";
        System::Globalization::CultureInfo^ cultura = System::Globalization::CultureInfo::InvariantCulture;
        StreamWriter^ writer = gcnew StreamWriter(ruta);

        try {
            Nodo<Pelicula<double>>* tmp = historial.GetCima();
            while (tmp != nullptr) {
                Pelicula<double>& p = tmp->Dato;
                String^ linea = String::Format(cultura, "{0},{1},{2:F2},{3},{4}",
                    p.GetOrden(), marshal_as<String^>(p.GetTitulo()), p.GetPuntuacion(),
                    p.GetVolumen(), p.GetVistasTotales());
                writer->WriteLine(linea);
                tmp = tmp->siguiente;
            }
        }
        finally {
            writer->Close();
        }
    }
};