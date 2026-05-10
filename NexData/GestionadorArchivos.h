#pragma once
#include "Dependencias.h"
#include "Pelicula.h"
#include "ListaDobleCircular.h"
#include "Pila.h"

class GestionadorArchivos {
public:
    // Mover aquí CargarDesdeArchivos
    void CargarTodo(ListaCircularDoble<Pelicula<double>>& catalogo, ListaSimple<Pelicula<double>>& recientes) {
        //Nombre de Ruta
        String^ rutaPelis = "DirectorioPeliculas.txt";
        String^ rutaDatos = "DirectorioDatos.txt";
        StreamReader^ lectorPelis = gcnew StreamReader(rutaPelis);
        StreamReader^ lectorDatos = gcnew StreamReader(rutaDatos);

        try {
            while (!lectorPelis->EndOfStream && !lectorDatos->EndOfStream) {

                String^ lineaP = lectorPelis->ReadLine();
                String^ lineaD = lectorDatos->ReadLine();

                cli::array<String^>^ datosP = lineaP->Split(',');
                cli::array<String^>^ datosD = lineaD->Split(',');

                //Primer txt
                int orden = Convert::ToInt32(datosP[0]);
                String^ title = datosP[1];
                string titulo = marshal_as<string>(title);

                int lanzamiento = Convert::ToInt32(datosP[2]);

                vector<string> categorias;
                for (int i = 3; i < (int)datosP->Length; i++) {
                    String^ subcat = datosP[i];
                    categorias.push_back(marshal_as<string>(subcat));
                }

                //Segundo txt
                double calificacion = Convert::ToDouble(datosD[1]);
                int volumen = Convert::ToInt32(datosD[2]);
                int vistas = Convert::ToInt32(datosD[3]);

                //Crear objeto pelicula
                Pelicula<double> nuevaPeli(orden, titulo, lanzamiento, categorias, calificacion, volumen, vistas);

                catalogo.InsertarAlFinal(nuevaPeli);
                recientes.InsertarAlInicio(nuevaPeli);
            }
        }
        catch (Exception^ ex) {
            cout << "Error critico al procesar los datos: " << marshal_as<string>(ex->Message) << endl;
        }
        finally {
            lectorPelis->Close();
            lectorDatos->Close();
        }
    }

    void CargarHistorial(ListaCircularDoble<Pelicula<double>>& catalogo, Pila<Pelicula<double>>& historial) {
        String^ ruta = "HistorialVisitado.txt";
        if (File::Exists(ruta)) {
            StreamReader^ reader = gcnew StreamReader(ruta);
            vector<Pelicula<double>> tmp;

            try {
                while (!reader->EndOfStream) {
                    String^ lineaCompleta = reader->ReadLine();
                    if (String::IsNullOrWhiteSpace(lineaCompleta)) continue;

                    cli::array<String^>^ partes = lineaCompleta->Split(',');

                    if (partes->Length >= 2) {

                        String^ subcat = partes[1];
                        string tituloBuscado = marshal_as<string>(subcat);

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
            catch (Exception^ ex) {
                cout << "Error al procesar historial: " << marshal_as<string>(ex->Message) << endl;
            }
            finally {
                reader->Close();
            }

            reverse(tmp.begin(), tmp.end());
            for (const auto& peli : tmp) {
                historial.Push(peli);
            }
        }
    }

    void GuardarHistorial(ListaCircularDoble<Pelicula<double>>& catalogo, Pila<Pelicula<double>>& historial) {
        String^ ruta = "HistorialVisitado.txt";
        int orden = 1;
        if (File::Exists(ruta)) {
            StreamReader^ reader = gcnew StreamReader(ruta);
            vector<Pelicula<double>> tmp;
            try {
                while (!reader->EndOfStream) {
                    String^ line = reader->ReadLine();
                    string titulo = marshal_as<string>(line);
                    Nodo<Pelicula<double>>* curr = catalogo.GetCabeza();
                    do {
                        if (curr->Dato.GetTitulo() == titulo) {
                            Pelicula<double> nuevaPeli(orden, titulo, curr->Dato.GetLanzamiento(), curr->Dato.GetCategorias(), curr->Dato.GetPuntuacion(), curr->Dato.GetVolumen(), curr->Dato.GetVistasTotales());
                            tmp.push_back(nuevaPeli);
                            break;
                        }
                        curr = curr->siguiente;
                    } while (curr != catalogo.GetCabeza());

                }
            }
            catch (Exception^ ex) {
                cout << "Error critico al procesar los datos: " << marshal_as<string>(ex->Message) << endl;
            }
            finally {
                reader->Close();
            }
            reverse(tmp.begin(), tmp.end());
            for (auto g : tmp) {
                historial.Push(g);
            }
        }
    }
};