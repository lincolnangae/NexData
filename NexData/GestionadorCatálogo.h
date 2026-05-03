#pragma once
#include "Dependencias.h"
#include "ListaDobleCircular.h"
#include "ListaSimple.h"
#include "Pelicula.h"

//Clase principal que controla el flujo de datos
class Gestionador {
private:

    ListaCircularDoble<Pelicula<double>> catalogo;
    vector<Pelicula<double>> datosModificables; //Lista para actualizar "DirectorioDatos.txt"

public:

    void cargarDesdeArchivos() {
        //Nombre de Ruta
        String^ rutaPelis = "DirectorioPeliculas.txt";
        String^ rutaDatos = "DirectorioDatos.txt";

        //Verificacion de existencia
        if (File::Exists(rutaPelis) && File::Exists(rutaDatos)) {
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
                    double rating = Convert::ToDouble(datosD[1]);
                    int volumen = Convert::ToInt32(datosD[2]);
                    int vistas = Convert::ToInt32(datosD[3]);

                    //Crear objeto pelicula
                    Pelicula<double> nuevaPeli(orden, titulo, lanzamiento, categorias, rating, volumen, vistas);

                    catalogo.InsertarAlFinal(nuevaPeli);
                    datosModificables.push_back(nuevaPeli);
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
    }

    void guardarDatos() {
        String^ rutaDatos = "DirectorioDatos.txt";

        StreamWriter^ escritor = gcnew StreamWriter(rutaDatos);

        try {
            for (const auto& peli : datosModificables) {
                String^ linea = peli.Orden + "," +
                    peli.Puntuacion + "," +
                    peli.Volumen + "," +
                    peli.VistasTotales;
                escritor->WriteLine(linea);
            }
        }
        catch (Exception^ ex) {
            cout << "Error al guardar datos: " << marshal_as<string>(ex->Message) << endl;
        }
        finally {
            escritor->Close();
        }
    }
};
