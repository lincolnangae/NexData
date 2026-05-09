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
    ListaSimple<Pelicula<double>> VerMasTarde;  // Nueva lista para 'ver mas tarde'

    //Vistas Referenciales
    Pelicula<double> Top1;
    Pelicula<double> Top2;
    Pelicula<double> Top3;

    int TopVistas1;
    int TopVistas2;
    int TopVistas3;

public:

    Gestionador() : VerMasTarde(1000) {  // Capacidad de 1000 películas en "Ver más tarde"
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
            // Verificar si la película está guardada en "Ver más tarde"
            Nodo<Pelicula<double>>* tempCheck = VerMasTarde.GetCabeza();
            bool estaGuardada = false;

            while (tempCheck != nullptr) {
                if (tempCheck->Dato.Titulo == curr->Dato.Titulo) {
                    estaGuardada = true;
                    break;
                }
                tempCheck = tempCheck->siguiente;
            }

            // Mostrar la información con el indicador de guardado
            curr->Dato.ImprimirInfoExtendida(1, estaGuardada);
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
            case 'G':
                {
                    // Verificar si ya está en la lista antes de guardar
                    Nodo<Pelicula<double>>* tempCheck = VerMasTarde.GetCabeza();
                    bool yaGuardada = false;

                    while (tempCheck != nullptr) {
                        if (tempCheck->Dato.Titulo == curr->Dato.Titulo) {
                            yaGuardada = true;
                            break;
                        }
                        tempCheck = tempCheck->siguiente;
                    }

                    CleanScreen();

                    if (yaGuardada) {
                        Console::ForegroundColor = ConsoleColor::Yellow;
                        Gotoxy((int)(GetAnchoVentana() / 2) - 20, 1);
                        cout << "PELICULA YA GUARDADA";
                        Console::ForegroundColor = ConsoleColor::White;
                        cout << endl << endl;
                        cout << "La pelicula '" << LimpiarTexto(curr->Dato.Titulo) << "' ya estaba en tu lista 'Ver mas tarde'." << endl;
                    } else {
                        VerMasTarde.InsertarAlInicio(curr->Dato);
                        Console::ForegroundColor = ConsoleColor::Green;
                        Gotoxy((int)(GetAnchoVentana() / 2) - 25, 1);
                        cout << "PELICULA GUARDADA PARA VER MAS TARDE";
                        Console::ForegroundColor = ConsoleColor::White;
                        cout << endl << endl;
                        cout << "La pelicula '" << LimpiarTexto(curr->Dato.Titulo) << "' ha sido guardada exitosamente." << endl;
                    }

                    system("pause");
                    CleanScreen();  // ← LIMPIAR PANTALLA DESPUÉS DE PAUSE
                }
                break; //Guardar para ver más tarde
            case 'S': break;
            }
        }



    }

    // Método auxiliar para obtener película por índice desde DatosModificables
    Pelicula<double> ObtenerPeliculaPorIndice(int indice) {
        // Usar DatosModificables que es un vector con todas las películas cargadas desde el TXT
        if (indice > 0 && indice <= (int)DatosModificables.size()) {
            return DatosModificables[indice - 1];  // Los vectores empiezan en 0, usuario cuenta desde 1
        }
        return Pelicula<double>();  // Retorna película vacía si no existe
    }

    // Método auxiliar para contar películas en el catálogo
    int ContarPeliculasEnCatalogo() {
        return (int)DatosModificables.size();  // Retorna el tamaño del vector
    }

    void GuardarParaVerMasTarde() {
        system("cls");
        auto CleanScreen = []() {
            system("cls");
        };
        auto LimpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
        };

        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n======== AGREGAR A 'VER MAS TARDE' ========\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;

        int totalPeliculas = ContarPeliculasEnCatalogo();
        if (totalPeliculas == 0) {
            Console::ForegroundColor = ConsoleColor::Red;
            cout << "No hay peliculas en el catalogo." << endl;
            Console::ForegroundColor = ConsoleColor::White;
            system("pause");
            return;
        }

        // Mostrar todas las películas del catálogo desde DatosModificables
        cout << "\n[ PELICULAS DISPONIBLES ]\n" << endl;
        for (int i = 0; i < totalPeliculas; i++) {
            cout << (i + 1) << ". ";
            DatosModificables[i].MostrarEnLista();
            cout << endl;
        }

        char continuar = 'S';
        while (continuar == 'S' || continuar == 's') {
            int numPeli;
            cout << "\nIngresa el numero de pelicula a guardar (0 para terminar): ";
            cin >> numPeli;

            if (cin.fail()) {
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                Console::ForegroundColor = ConsoleColor::Red;
                cout << "Error: Debes ingresar un numero." << endl;
                Console::ForegroundColor = ConsoleColor::White;
                continue;
            }

            if (numPeli == 0) break;

            if (numPeli < 1 || numPeli > totalPeliculas) {
                Console::ForegroundColor = ConsoleColor::Red;
                cout << "Numero invalido. Intenta de nuevo." << endl;
                Console::ForegroundColor = ConsoleColor::White;
                continue;
            }

            // Obtener la película del catálogo y agregarla a VerMasTarde
            Pelicula<double> peliSeleccionada = ObtenerPeliculaPorIndice(numPeli);
            VerMasTarde.InsertarAlInicio(peliSeleccionada);

            Console::ForegroundColor = ConsoleColor::Green;
            cout << "Pelicula '" << LimpiarTexto(peliSeleccionada.Titulo) << "' guardada para ver mas tarde." << endl;
            Console::ForegroundColor = ConsoleColor::White;

            cout << "Agregar otra? (S/N): ";
            continuar = _getch();
        }

        CleanScreen();
        Console::ForegroundColor = ConsoleColor::Green;
        cout << "Peliculas guardadas exitosamente." << endl;
        Console::ForegroundColor = ConsoleColor::White;
        system("pause");
    }

    void VerListaVerMasTarde() {
        system("cls");
        auto CleanScreen = []() {
            system("cls");
        };
        auto LimpiarTexto = [](string texto) {
            for (char& c : texto) {
                if (c == '-') c = ' ';
            }
            return texto;
        };

        mostrar_lista:
        Console::ForegroundColor = ConsoleColor::Yellow;
        cout << "\n======== LISTA 'VER MAS TARDE' ========\n" << endl;
        Console::ForegroundColor = ConsoleColor::White;

        // Mostrar la lista actual
        Nodo<Pelicula<double>>* actual = VerMasTarde.GetCabeza();
        int indice = 1;

        if (actual == nullptr) {
            Console::ForegroundColor = ConsoleColor::Red;
            cout << "La lista esta vacia." << endl;
            Console::ForegroundColor = ConsoleColor::White;
            system("pause");
            return;
        }

        while (actual != nullptr) {
            cout << indice << ". ";
            actual->Dato.MostrarEnLista();
            actual = actual->siguiente;
            indice++;
            cout << endl;
        }

        // Contar películas en la lista
        int totalPelisVerMasTarde = 0;
        Nodo<Pelicula<double>>* temp = VerMasTarde.GetCabeza();
        while (temp != nullptr) {
            totalPelisVerMasTarde++;
            temp = temp->siguiente;
        }

        char opcion = ' ';
        while (true) {
            cout << "\n¿Deseas reordenar? (S/N): ";
            opcion = _getch();

            if (opcion == 'N' || opcion == 'n') {
                break;
            }
            else if (opcion == 'S' || opcion == 's') {
                CleanScreen();

                cout << "\n======== REORDENAR LISTA 'VER MAS TARDE' ========\n" << endl;

                // Mostrar la lista nuevamente
                actual = VerMasTarde.GetCabeza();
                indice = 1;
                while (actual != nullptr) {
                    cout << indice << ". ";
                    actual->Dato.MostrarEnLista();
                    actual = actual->siguiente;
                    indice++;
                    cout << endl;
                }

                int peliculaActual, posicionNueva;

                cout << "\n¿Que pelicula deseas mover? (numero): ";
                cin >> peliculaActual;

                if (cin.fail() || peliculaActual < 1 || peliculaActual > totalPelisVerMasTarde) {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    Console::ForegroundColor = ConsoleColor::Red;
                    cout << "Numero invalido." << endl;
                    Console::ForegroundColor = ConsoleColor::White;
                    system("pause");
                    CleanScreen();
                    goto mostrar_lista;
                }

                cout << "¿A que posicion? (numero): ";
                cin >> posicionNueva;

                if (cin.fail() || posicionNueva < 1 || posicionNueva > totalPelisVerMasTarde) {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    Console::ForegroundColor = ConsoleColor::Red;
                    cout << "Numero invalido." << endl;
                    Console::ForegroundColor = ConsoleColor::White;
                    system("pause");
                    CleanScreen();
                    goto mostrar_lista;
                }

                // PASO 1: Extraer todas las películas de VerMasTarde en un vector
                vector<Pelicula<double>> peliculasEnOrden;
                actual = VerMasTarde.GetCabeza();
                int contador = 1;
                Pelicula<double> peliculaAMover;

                while (actual != nullptr) {
                    if (contador == peliculaActual) {
                        peliculaAMover = actual->Dato;  // Guardamos la película a mover
                    } else {
                        peliculasEnOrden.push_back(actual->Dato);  // Guardamos las demás
                    }
                    actual = actual->siguiente;
                    contador++;
                }

                // PASO 2: Insertar la película a mover en la posición deseada del vector
                if (posicionNueva > (int)peliculasEnOrden.size()) {
                    peliculasEnOrden.push_back(peliculaAMover);  // Al final
                } else {
                    peliculasEnOrden.insert(peliculasEnOrden.begin() + (posicionNueva - 1), peliculaAMover);
                }

                // PASO 3: Limpiar VerMasTarde
                VerMasTarde = ListaSimple<Pelicula<double>>(1000);

                // PASO 4: Reconstruir VerMasTarde usando InsertarEnPosicion()
                // Primero insertamos la primera película al inicio
                if (peliculasEnOrden.size() > 0) {
                    VerMasTarde.InsertarAlInicio(peliculasEnOrden[0]);

                    // Luego insertamos el resto en sus posiciones correctas usando InsertarEnPosicion()
                    for (int i = 1; i < (int)peliculasEnOrden.size(); i++) {
                        VerMasTarde.InsertarEnPosicion(i + 1, peliculasEnOrden[i]);
                    }
                }

                Console::ForegroundColor = ConsoleColor::Green;
                cout << "\nPelicula '" << LimpiarTexto(peliculaAMover.Titulo) << "' movida a la posicion " << posicionNueva << endl;
                Console::ForegroundColor = ConsoleColor::White;
                system("pause");

                CleanScreen();
                goto mostrar_lista;
            }
        }

        CleanScreen();
    }
};