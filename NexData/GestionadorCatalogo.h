#pragma once
#include "Dependencias.h"
#include "ListaDobleCircular.h"
#include "ListaSimple.h"
#include "Pelicula.h"

//Clase principal que controla el flujo de datos
enum class TipoTop { RECIENTES, MASVISTAS , VISTOSRECIENTES };
class Gestionador {
private:

    ListaCircularDoble<Pelicula<double>> Catalogo;
    vector<Pelicula<double>> DatosModificables; //Lista para actualizar "DirectorioDatos.txt"
    ListaSimple<Pelicula<double>> TopMasVistos; //Los Mas Vistos
    ListaSimple<Pelicula<double>> TopRecientes; //Los ultimos de la lista
    ListaSimple<Pelicula<double>> TopRecienVistos; //Los ultimos de la lista

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
        DatosModificables.clear();
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

                    Catalogo.InsertarAlFinal(nuevaPeli);
                    DatosModificables.push_back(nuevaPeli);
                    TopRecientes.InsertarAlInicio(nuevaPeli);
                }
            }
            catch (Exception^ ex) {
                cout << "Error critico al procesar los datos: " << marshal_as<string>(ex->Message) << endl;
            }
            finally {
                if (TopVistas1 > 0) {
                    //al final de el while y despues de comparar, recien ubicamos las peliculas
                    TopMasVistos.InsertarAlInicio(Top3);
                    TopMasVistos.InsertarAlInicio(Top2);
                    TopMasVistos.InsertarAlInicio(Top1);
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
            for (const auto& peli : DatosModificables) {
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
        bool huboCambio = 0;
        if (p.VistasTotales > TopVistas1) {
            Top3 = Top2; TopVistas3 = TopVistas2;
            Top2 = Top1; TopVistas2 = TopVistas1;
            Top1 = p; TopVistas1 = p.VistasTotales;
            huboCambio = 1;
        }
        else if (p.VistasTotales > TopVistas2) {
            Top3 = Top2; TopVistas3 = TopVistas2;
            Top2 = p; TopVistas2 = p.VistasTotales;
            huboCambio = 1;


        }
        else if (p.VistasTotales > TopVistas3) {
            Top3 = p; TopVistas3 = p.VistasTotales;
            huboCambio = 1;
        }

        if (huboCambio = 1) {
            TopMasVistos = ListaSimple<Pelicula<double>>(3);
            TopMasVistos.InsertarAlInicio(Top3);
            TopMasVistos.InsertarAlInicio(Top2);
            TopMasVistos.InsertarAlInicio(Top1);
        }
    }

    void MostrarTops(TipoTop tipo) {
        Nodo<Pelicula<double>>* actual = nullptr;
        int indice = 0;

        if (tipo == TipoTop::MASVISTAS) {
            actual = TopMasVistos.GetCabeza();
            indice = 1;
        }
        else if (tipo == TipoTop::RECIENTES) {
            actual = TopRecientes.GetCabeza();
            indice = 4;
        }
        else {
            actual = TopRecienVistos.GetCabeza();
            indice = 7;
        }

        while (actual != nullptr) {
            cout << indice << ". ";
            actual->Dato.MostrarEnLista();
            actual = actual->siguiente;
            indice++;
            cout << endl;
        }

    }
    //Mostrar Informacion extendida 
    //Proximo a agregar : Logica para calificar pelicula 
    void ImprimirInformacion(TipoTop type, int indice) {
        auto CambiarMayuscula = [](char c) {
            return toupper(c);
            };
        auto LimpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
            };
        auto CleanScreen = []() {
            system("cls");
            };

        CleanScreen();
        Nodo<Pelicula<double>>* curr = nullptr;

        int indx;
        if (type == TipoTop::MASVISTAS) {
            curr = TopMasVistos.GetCabeza();
            indx = 1;
        }
        else if (type == TipoTop::RECIENTES) {
            curr = TopRecientes.GetCabeza();
            indx = 4;
        }
        else {
            curr = TopRecienVistos.GetCabeza();
            indx = 7;
        }
        while (indx != indice && curr != nullptr) {
            indx++;
            curr = curr->siguiente;
        }
        if (curr == nullptr) {
            //El programa detectaba fuga de memoria, para evitar advertencias agrege esto
            return;
        }
        char tecla = ' ';
        while (tecla != 'S') {
            curr->Dato.ImprimirInfoExtendida(1);
            tecla = CambiarMayuscula(_getch());
            switch (tecla) {
            case 'C':
                CleanScreen();
                Console::ForegroundColor = ConsoleColor::Yellow;
                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(curr->Dato.Titulo.length()) / 2), 1);
                cout << "<<CALIFICAR PELICULA>>";
                Console::ForegroundColor = ConsoleColor::White;
                cout << endl;
                //Usamos limits como buena practica para evitar errores al usar cin
                double ValueRatingTemp;
                do {
                    cout << "Introduce una calificacion [0-10]: ";
                    cin >> ValueRatingTemp;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        cout << "Error: No se permiten letras o simbolos." << endl;
                        ValueRatingTemp = -1;
                        continue;
                    }

                    if (ValueRatingTemp < 0 || ValueRatingTemp > 10) {
                        cout << "Rango invalido. Intenta de nuevo." << endl;
                        cout << endl;
                    }

                } while (ValueRatingTemp < 0 || ValueRatingTemp > 10);
                //esta solo para validar ya, limits esta en dependencias
                CleanScreen(); 
                break; //Calificar pelicula
            case 'V':
                curr->Dato.VistasTotales++;
                TopRecienVistos.InsertarAlInicio(curr->Dato);
                CleanScreen();
                Console::ForegroundColor = ConsoleColor::Yellow;
                //El copilador toma de advertencia para size_t a int, asi que decimos que el dato size_t se convierta en int antes de copilar
                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(curr->Dato.Titulo.length()) / 2), 1);
                cout << "<<<" << LimpiarTexto(curr->Dato.Titulo) << ">>>";
                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(curr->Dato.Titulo.length()) / 2) + ((int)curr->Dato.Titulo.length() + 6 - 30) / 2, 2);
                Console::ForegroundColor = ConsoleColor::Cyan;
                cout << "<<<PELICULA REPRODUCIENDOSE>>>" << endl;
                cout << endl;
                Dibujo();
                Console::ForegroundColor = ConsoleColor::White;
                cout << endl << "En reproduccion... Presione cualquier tecla para finalizar";
                system("pause>0");
                CleanScreen();
                for (auto& temp : DatosModificables) {
                    if (temp.Titulo == curr->Dato.Titulo) {
                        temp.VistasTotales = curr->Dato.VistasTotales;
                        break;
                    }
                }
                ActualizarTopVistas(curr->Dato);
                break; //Ver Pelicula (Aumentar vistas totales)
            case 'S': break;
            }
        }


    }
};