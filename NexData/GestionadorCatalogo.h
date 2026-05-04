#pragma once
#include "Dependencias.h"
#include "ListaDobleCircular.h"
#include "ListaSimple.h"
#include "Pelicula.h"

//Clase principal que controla el flujo de datos
enum class TipoTop { RECIENTES, MASVISTAS };
class Gestionador {
private:

    ListaCircularDoble<Pelicula<double>> catalogo;
    vector<Pelicula<double>> datosModificables; //Lista para actualizar "DirectorioDatos.txt"
    ListaSimple<Pelicula<double>> topMasVistos; //Los Mas Vistos
    ListaSimple<Pelicula<double>> topRecientes; //Los ultimos de la lista

    //Vistas Referenciales
    Pelicula<double> Top1;
    Pelicula<double> Top2;
    Pelicula<double> Top3;


    int TopVistas1;
    int TopVistas2;
    int TopVistas3;
 
public:

    Gestionador() {
        TopVistas1 = 0;
        TopVistas2 = 0;
        TopVistas3 = 0;
    }

    ~Gestionador() {
        datosModificables.clear();
    }

    void cargarDesdeArchivos() {
        //Nombre de Ruta
        String^ rutaPelis = "DirectorioPeliculas.txt";
        String^ rutaDatos = "DirectorioDatos.txt";

        //Verificacion de existencia
        if (File::Exists(rutaPelis) && File::Exists(rutaDatos)) {
            cout << "dentro";
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

                    ActualizarTopVistas(nuevaPeli);

                    catalogo.InsertarAlFinal(nuevaPeli);
                    datosModificables.push_back(nuevaPeli);
                    topRecientes.InsertarAlInicio(nuevaPeli);
                }
            }
            catch (Exception^ ex) {
                cout << "Error critico al procesar los datos: " << marshal_as<string>(ex->Message) << endl;
            }
            finally {

                if (TopVistas1 > 0) {
                    topMasVistos.InsertarAlInicio(Top3);
                    topMasVistos.InsertarAlInicio(Top2);
                    topMasVistos.InsertarAlInicio(Top1);
                }

                lectorPelis->Close();
                lectorDatos->Close();
            }
        }
    }

    void GuardarDatos() {
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

    void ActualizarTopVistas(Pelicula<double> p) {
        if (p.VistasTotales > TopVistas1) {
            Top3 = Top2; TopVistas3 = TopVistas2;
            Top2 = Top1; TopVistas2 = TopVistas1;
            Top1 = p; TopVistas1 = p.VistasTotales;
        }
        else if (p.VistasTotales > TopVistas2) {
            Top3 = Top2; TopVistas3 = TopVistas2;
            Top2 = p; TopVistas2 = p.VistasTotales;
        }
        else if (p.VistasTotales > TopVistas3) {
            Top3 = p; TopVistas3 = p.VistasTotales;
        }
    }

    void MostrarTops(TipoTop tipo) {
        Nodo<Pelicula<double>>* actual = nullptr;
        int indice = 0;

        if (tipo == TipoTop::MASVISTAS) {
            actual = topMasVistos.getCabeza();
                indice = 1;
        }
        else {
            actual = topRecientes.getCabeza();
                indice = 4;
        }

        while (actual != nullptr) {
            cout << indice << ". ";
            actual->dato.MostrarEnLista();
                actual = actual->siguiente;
            indice++;
            cout << endl;
        }
    }

};
