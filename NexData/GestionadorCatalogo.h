#pragma once
#include"Pila.h"
#include"Usuario.h"
#include "ListaDobleCircular.h"
#include "ListaSimple.h"
#include "Pelicula.h"

//Clase principal que controla el flujo de datos
enum class TipoTop { RECIENTES, MASVISTAS , VISTOSRECIENTES };
class Gestionador {
private:
    User* Usuario;
    ListaCircularDoble<Pelicula<double>> Catalogo;
    ListaCircularDoble<Pelicula<double>> VerMasTarde; //Los recientes vistos
    ListaSimple<Pelicula<double>> TopMasVistos; //Los Mas Vistos
    ListaSimple<Pelicula<double>> TopRecientes; //Los ultimos de la lista
    ListaSimple<Pelicula<double>> TopRecienVistos; //Los ultimos de la lista
    Pila<Pelicula<double>> pila;

    //Vistas Referenciales
    Pelicula<double> Top1;
    Pelicula<double> Top2;
    Pelicula<double> Top3;

    int TopVistas1;
    int TopVistas2;
    int TopVistas3;

public:

    Gestionador() {  // Capacidad de 1000 películas en "Ver más tarde"
        TopVistas1 = 0;
        TopVistas2 = 0;
        TopVistas3 = 0;
    }
    ~Gestionador() {
    }
    void CargarDesdeArchivos() {
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
                    TopRecientes.InsertarAlInicio(nuevaPeli);
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

    void GuardarDatos() {
        String^ rutaDatos = "DirectorioDatos.txt";
        // Forzamos el punto decimal para evitar conflictos
        System::Globalization::CultureInfo^ cultura = System::Globalization::CultureInfo::InvariantCulture;
        StreamWriter^ escritor = gcnew StreamWriter(rutaDatos);
        Nodo<Pelicula<double>>* actual = Catalogo.GetCabeza();

        if (actual != nullptr) {
            try {
                do {
                    Pelicula<double>& p = actual->Dato;
                    String^ linea = String::Format(cultura, "{0},{1:F2},{2},{3}",
                        p.Orden,
                        p.Puntuacion,
                        p.Volumen,
                        p.VistasTotales);

                    escritor->WriteLine(linea);
                    actual = actual->siguiente;
                } while (actual != Catalogo.GetCabeza());
            }
            catch (Exception^ ex) {
                cout << "Error al guardar: " << marshal_as<string>(ex->Message) << endl;
            }
            finally {
                escritor->Close();
            }
        }
    }

    void ActualizarTopVistas(Pelicula<double> p) {
        //verificar si ya existe para no cambiar
        if (p.Titulo == Top1.Titulo) {
            Top1 = p; TopVistas1 = p.VistasTotales;
        }
        else if (p.Titulo == Top2.Titulo) {
            Top2 = p; TopVistas2 = p.VistasTotales;
        }
        else if (p.Titulo == Top3.Titulo) {
            Top3 = p; TopVistas3 = p.VistasTotales;
        }
        else {
            //si es nueva, la logica normal
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

        // reordenar si hubo subimiento
        if (TopVistas2 > TopVistas1) {
            swap(Top1, Top2); swap(TopVistas1, TopVistas2);
        }
        if (TopVistas3 > TopVistas2) {
            swap(Top2, Top3); swap(TopVistas2, TopVistas3);
        }
        if (TopVistas2 > TopVistas1) {
            swap(Top1, Top2); swap(TopVistas1, TopVistas2);
        }

        TopMasVistos = ListaSimple<Pelicula<double>>(3);
        if (TopVistas3 > 0) TopMasVistos.InsertarAlInicio(Top3);
        if (TopVistas2 > 0) TopMasVistos.InsertarAlInicio(Top2);
        if (TopVistas1 > 0) TopMasVistos.InsertarAlInicio(Top1);
    }

    void MostrarTopMenus(TipoTop tipo) {
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
    void ImprimirInformacion(TipoTop type, int indice) {
        auto CleanScreen = []() { system("cls"); };
        CleanScreen();
        Nodo<Pelicula<double>>* curr = nullptr;
        int indx = (type == TipoTop::MASVISTAS) ? 1 : (type == TipoTop::RECIENTES ? 4 : 7);

        if (type == TipoTop::MASVISTAS) curr = TopMasVistos.GetCabeza();
        else if (type == TipoTop::RECIENTES) curr = TopRecientes.GetCabeza();
        else curr = TopRecienVistos.GetCabeza();

        while (indx != indice && curr != nullptr) {
            indx++;
            curr = curr->siguiente;
        }

        if (curr != nullptr) {
            ImprimirInformacionDesdePelicula(curr->Dato);
        }
    }

    int ContarPeliculasEnCatalogo() {
        return (int)Catalogo.GetCantidad(); 
    }

    void GuardarParaVerMasTarde() {
        if (Catalogo.GetCantidad() == 0) {
            return;
        }

        auto DibujarSeparador = [this]() {
            for (int j = 0; j < 40; j++) {
                setColor(j % 4);
                cout << char(178);
            }
            setColor(15);
            cout << endl;
            };

        ListaCircularDoble<Pelicula<double>> listaNavegacion;
        Nodo<Pelicula<double>>* actualCat = Catalogo.GetCabeza();

        if (actualCat != nullptr) {
            do {
                listaNavegacion.InsertarAlFinal(actualCat->Dato);
                actualCat = actualCat->siguiente;
            } while (actualCat != Catalogo.GetCabeza());
        }

        auto CleanScreen = []() { system("cls"); };
        auto LimpiarTexto = [](string texto) {
            for (char& c : texto) if (c == '-') c = ' ';
            return texto;
            };

        Nodo<Pelicula<double>>* inicioVentana = listaNavegacion.GetCabeza();
        char tecla = ' ';

        while (tecla != '0') {
            CleanScreen();
            Console::ForegroundColor = ConsoleColor::Yellow;
            Gotoxy(GetAnchoVentana() / 2 - 20, 0); cout << "========================================" << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 1); cout << "        LISTADO - DE - PELICULAS        " << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 2); cout << "========================================" << endl;
            Console::ForegroundColor = ConsoleColor::Cyan;
            cout << "------------------------------------------------------------------" << endl;
            cout << "[Q] Anterior  |  [E] Siguiente  |  [1-9] Seleccionar  |  [0] Salir" << endl;
            cout << "------------------------------------------------------------------" << endl;
            cout << "Seleccionar una pelicula segun indice [1 - 9]" << endl;
            cout << endl;
            Console::ForegroundColor = ConsoleColor::White;

            //mostrar 9 peliculas
            Nodo<Pelicula<double>>* temp = inicioVentana;
            int limite = (listaNavegacion.GetCantidad() < 9) ? listaNavegacion.GetCantidad() : 9;

            for (int i = 1; i <= limite; i++) {
                Console::ForegroundColor = ConsoleColor::Gray;
                cout << "[" << i << "] ";
                Console::ForegroundColor = ConsoleColor::White;
                // Llamada al método de la clase Pelicula
                temp->Dato.MostrarEnLista();
                Console::ForegroundColor = ConsoleColor::DarkGray;
                cout << "----------------------------------------------------------" << endl;
                Console::ForegroundColor = ConsoleColor::White;
                DibujarSeparador();
                temp = temp->siguiente;
            }

            tecla = toupper(_getch());

            // navegacion de toda la lista
            if (tecla == 'E') {
                inicioVentana = inicioVentana->siguiente;
            }
            else if (tecla == 'Q') {
                inicioVentana = inicioVentana->anterior;
            }
            //seleccion de pelicula
            else if (tecla >= '1' && tecla <= '9') {
                int seleccion = tecla - '0';
                if (seleccion <= limite) {
                    Nodo<Pelicula<double>>* nodoElegido = inicioVentana;
                    for (int k = 1; k < seleccion; k++) {
                        nodoElegido = nodoElegido->siguiente;
                    }
                    // logica de guardado en ListaSimple
                    VerMasTarde.InsertarAlInicio(nodoElegido->Dato);

                    Console::ForegroundColor = ConsoleColor::Green;
                    cout << "\n>> '" << LimpiarTexto(nodoElegido->Dato.Titulo) << "' agregada exitosamente." << endl;
                    Console::ForegroundColor = ConsoleColor::White;
                    system("pause");
                }
            }
        }
    }

    void VerListaVerMasTarde() {
        if (VerMasTarde.GetCantidad() == 0) {
            system("cls");
            Console::ForegroundColor = ConsoleColor::Red;
            cout << "\nTu lista 'Ver mas tarde' esta vacia." << endl;
            Console::ForegroundColor = ConsoleColor::White;
            system("pause");
            return;
        }

        bool enMenu = true;
        Nodo<Pelicula<double>>* inicioVentana = VerMasTarde.GetCabeza();

        while (enMenu) {
            system("cls");
            Console::ForegroundColor = ConsoleColor::Yellow;
            Gotoxy(GetAnchoVentana() / 2 - 20, 0); cout << "========================================" << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 1); cout << "      LISTADO - DE - VER - DESPUES      " << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 2); cout << "========================================" << endl;

            Console::ForegroundColor = ConsoleColor::Cyan;
            cout << "------------------------------------------------------------------" << endl;
            cout << "[Q] Anterior | [E] Siguiente | [S] Reordenar | [V] Ver todas las peliculas | [0] Salir" << endl;
            cout << "------------------------------------------------------------------" << endl;
            Console::ForegroundColor = ConsoleColor::White;

            Nodo<Pelicula<double>>* temp = inicioVentana;
            int total = VerMasTarde.GetCantidad();
            int limite = (total < 9) ? total : 9;

            for (int i = 1; i <= limite; i++) {
                Console::ForegroundColor = ConsoleColor::Gray;
                cout << "[" << i << "] ";
                Console::ForegroundColor = ConsoleColor::White;
                temp->Dato.MostrarEnLista();
                Console::ForegroundColor = ConsoleColor::DarkGray;
                cout << "----------------------------------------------------------" << endl;
                temp = temp->siguiente;
            }

            char tecla = toupper(_getch());

            if (tecla == 'E') {
                inicioVentana = inicioVentana->siguiente;
            }
            else if (tecla == 'Q') {
                inicioVentana = inicioVentana->anterior;
            }
            else if (tecla == '0') {
                enMenu = false;
            }
            else if (tecla == 'S') {
                int peliIndex, posNueva;
                cout << "\nIndice de pelicula a mover (1-" << total << "): "; cin >> peliIndex;
                cout << "Nueva posicion (1-" << total << "): "; cin >> posNueva;

                if (cin.fail() || peliIndex < 1 || peliIndex > total || posNueva < 1 || posNueva > total) {
                    cin.clear(); cin.ignore(1000, '\n');
                    cout << "Entrada invalida." << endl; system("pause");
                    continue;
                }

                //extraer el dato
                Nodo<Pelicula<double>>* aux = VerMasTarde.GetCabeza();
                for (int i = 1; i < peliIndex; i++) aux = aux->siguiente;
                Pelicula<double> peliExtraida = aux->Dato;

                //temporal para evitar corromper la lista principal
                ListaCircularDoble<Pelicula<double>> nuevaLista;
                Nodo<Pelicula<double>>* it = VerMasTarde.GetCabeza();
                for (int i = 1; i <= total; i++) {
                    if (i != peliIndex) nuevaLista.InsertarAlFinal(it->Dato);
                    it = it->siguiente;
                }

                //Insertar en la nueva posición
                nuevaLista.InsertarEnPosicion(posNueva, peliExtraida);

                VerMasTarde = nuevaLista;

                Console::ForegroundColor = ConsoleColor::Green;
                cout << "\nReordenado correctamente." << endl;
                Console::ForegroundColor = ConsoleColor::White;
                system("pause");
                inicioVentana = VerMasTarde.GetCabeza();
            }
            else if (tecla == 'V') {
                
                Nodo<Pelicula<double>>* curr = VerMasTarde.GetCabeza();
                
                do {
                    Pelicula<double> peli(0, curr->Dato.Titulo, curr->Dato.Lanzamiento, curr->Dato.Categorias, curr->Dato.Puntuacion, curr->Dato.Volumen, curr->Dato.VistasTotales);
                    peli.VistasTotales++;
                    Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
                    if (aux != nullptr) {
                        do {
                            if (aux->Dato.Titulo == peli.Titulo) {
                                aux->Dato.Puntuacion = peli.Puntuacion;
                                aux->Dato.Volumen = peli.Volumen;
                                aux->Dato.VistasTotales = peli.VistasTotales;
                                pila.push(peli);
                                break;
                            }
                            aux = aux->siguiente;
                        } while (aux != Catalogo.GetCabeza());
                    }
                    GuardarDatos();
                    ActualizarTopVistas(peli);
                    curr = curr->siguiente;
                } while (curr != VerMasTarde.GetCabeza());
                
                VerMasTarde.Clear();

                system("cls");
                cout << "Usted vio todas las peliculas correctamente" << endl << "Presione una tecla para continuar..."; system("pause>0");
                enMenu = 0;
            }
        }
    }
    
    //==============LOGICA PURA PARA OPERARR SOBRE LAS LISTAS DOBLES
    // Ahora recibe una referencia a la lista que queremos ver
    void MostrarCatalogoPaginado(ListaCircularDoble<Pelicula<double>>& listaAMostrar) {

        auto CleanScreen = []() { system("cls"); };

        auto LimpiarTexto = [](string texto) {
            for (char& c : texto) if (c == '-') c = ' ';
            return texto;
            };

        auto DibujarSeparador = [this]() {
            for (int j = 0; j < 40; j++) {
                setColor(j % 4);
                cout << char(178);
            }
            setColor(15);
            cout << endl;
            };

        // Verificación inicial
        if (listaAMostrar.GetCantidad() == 0) {
            cout << "El catalogo esta vacio." << endl;
            system("pause");
            return;
        }

        // Puntero que controla el inicio de nuestra "ventana" de visualización
        Nodo<Pelicula<double>>* inicioVentana = listaAMostrar.GetCabeza();
        char tecla = ' ';

        while (tecla != '0') {
            CleanScreen();

            CleanScreen();
            Console::ForegroundColor = ConsoleColor::Yellow;
            Gotoxy(GetAnchoVentana() / 2 - 20, 0); cout << "========================================" << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 1); cout << "             LISTA - CATALOGADO         " << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 2); cout << "========================================" << endl;

            Console::ForegroundColor = ConsoleColor::Cyan;
            cout << "\n------------------------------------------------------------------" << endl;
            cout << "[Q] Anterior  |  [E] Siguiente  |  [1-9] Detalle  |  [0] Salir" << endl;
            cout << "------------------------------------------------------------------" << endl;
            cout << "Seleccionar una pelicula segun indice [1 - 9]" << endl;
            Console::ForegroundColor = ConsoleColor::White;
            cout << endl;

            // Renderizado de la ventana: Máximo 9 películas para coincidir con teclas 1-9
            Nodo<Pelicula<double>>* temp = inicioVentana;
            int limite = (listaAMostrar.GetCantidad() < 9) ? listaAMostrar.GetCantidad() : 9;

            for (int i = 1; i <= limite; i++) {
                Console::ForegroundColor = ConsoleColor::Gray;
                cout << "[" << i << "] ";
                Console::ForegroundColor = ConsoleColor::White;
                // Llamada al método de la clase Pelicula
                temp->Dato.MostrarEnLista();
                Console::ForegroundColor = ConsoleColor::DarkGray;
                cout << "----------------------------------------------------------" << endl;
                Console::ForegroundColor = ConsoleColor::White;
                DibujarSeparador();
                temp = temp->siguiente;
            }

            tecla = toupper(_getch());

            //ir hacia atras
            if (tecla == 'E') {
                inicioVentana = inicioVentana->siguiente;
            }
            //ir hacia atras
            else if (tecla == 'Q') {
                inicioVentana = inicioVentana->anterior;
            }

            else if (tecla >= '1' && tecla <= '9') {
                int seleccion = tecla - '0';
                if (seleccion <= limite) {
                    Nodo<Pelicula<double>>* nodoElegido = inicioVentana;
                    for (int k = 1; k < seleccion; k++) nodoElegido = nodoElegido->siguiente;

                    ImprimirInformacionDesdePelicula(nodoElegido->Dato);
                }
            }
        }
    }
    // Método auxiliar para intercambiar los datos de dos nodos
    void Intercambiar(Nodo<Pelicula<double>>* a, Nodo<Pelicula<double>>* b) {
        Pelicula<double> temp = a->Dato;
        a->Dato = b->Dato;
        b->Dato = temp;
    }
    // Partición para la Lista Doble Circular
    template <typename Comparador>
    Nodo<Pelicula<double>>* ParticionLista(Nodo<Pelicula<double>>* bajo, Nodo<Pelicula<double>>* alto, Comparador comp) {
        Pelicula<double> pivote = alto->Dato;
        Nodo<Pelicula<double>>* i = bajo->anterior;

        for (Nodo<Pelicula<double>>* j = bajo; j != alto; j = j->siguiente) {
            if (comp(j->Dato, pivote)) {

                i = (i == bajo->anterior) ? bajo : i->siguiente;
                Intercambiar(i, j);
            }
        }
        i = (i == bajo->anterior) ? bajo : i->siguiente;
        Intercambiar(i, alto);
        return i;
    }

    // QuickSort Recursivo para Listas
    template <typename Comparador>
    void QuickSortRecursivoLista(Nodo<Pelicula<double>>* bajo, Nodo<Pelicula<double>>* alto, Comparador comp) {
        if (bajo == nullptr || alto == nullptr || bajo == alto) {
            return;
        }
        Nodo<Pelicula<double>>* p = ParticionLista(bajo, alto, comp);
        if (p != bajo) {
            QuickSortRecursivoLista(bajo, p->anterior, comp);
        }
        if (p != alto) {
            QuickSortRecursivoLista(p->siguiente, alto, comp);
        }
    }


    //ordena la lista segun un parametro en especifico
    void OrdenarYMostrar(char criterio) {
        if (Catalogo.GetCantidad() == 0) return;

        ListaCircularDoble<Pelicula<double>> listaTemporal;
        Nodo<Pelicula<double>>* actual = Catalogo.GetCabeza();

        // Recorremos la circular y copiamos los datos a la nueva lista
        if (actual != nullptr) {
            do {
                listaTemporal.InsertarAlFinal(actual->Dato);
                actual = actual->siguiente;
            } while (actual != Catalogo.GetCabeza());
        }

        //Definir comparadores y rangos para la lista temporal
        auto ordenCalificacionDesc = [](const Pelicula<double>& a, const Pelicula<double>& b) {
            return a.Puntuacion > b.Puntuacion;
            };
        auto ordenAnioDesc = [](const Pelicula<double>& a, const Pelicula<double>& b) {
            return a.Lanzamiento > b.Lanzamiento;
            };
        auto ordenAlfabeticoAsc = [](const Pelicula<double>& a, const Pelicula<double>& b) {
            return a.Titulo < b.Titulo;
            };


        Nodo<Pelicula<double>>* bajo = listaTemporal.GetCabeza();
        Nodo<Pelicula<double>>* alto = bajo->anterior;

        // PASO 3: Ordenar la copia, no el original
        switch (toupper(criterio)) {
        case 'C': QuickSortRecursivoLista(bajo, alto, ordenCalificacionDesc); break;
        case 'N': QuickSortRecursivoLista(bajo, alto, ordenAnioDesc); break;
        case 'A': QuickSortRecursivoLista(bajo, alto, ordenAlfabeticoAsc); break;
        default: return;
        }

        //Mostrar la lista temporal
        MostrarCatalogoPaginado(listaTemporal);
    }

    //menu mostraro al elegir una pelicula
    void ImprimirInformacionDesdePelicula(Pelicula<double>& peli) {
        auto CambiarMayuscula = [](char c) { return toupper(c); };
        auto LimpiarTexto = [](string texto) {
            for (char& c : texto) if (c == '-') c = ' ';
            return texto;
            };
        auto CleanScreen = []() { system("cls"); };

        char tecla = ' ';

        while (tecla != 'S') {
            CleanScreen();

            Nodo<Pelicula<double>>* tempCheck = VerMasTarde.GetCabeza();
            bool estaGuardada = false;
            if (tempCheck != nullptr) {
                do {
                    if (tempCheck->Dato.Titulo == peli.Titulo) {
                        estaGuardada = true;
                        break;
                    }
                    tempCheck = tempCheck->siguiente;
                } while (tempCheck != VerMasTarde.GetCabeza());
            }

            peli.ImprimirInfoExtendida(1, estaGuardada);
                tecla = CambiarMayuscula(_getch());

                if (tecla == 'C') { // CALIFICAR
                    system("cls");
                    double calif = -1;

                    // Validación de entrada
                    while (true) {
                        cout << "Calificar '" << peli.Titulo << "' [0.0 - 10.0]: ";
                        if (cin >> calif && calif >= 0 && calif <= 10) {
                            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                            break;
                        }
                        cout << "Error: Ingresa un valor valido entre 0 y 10." << endl;
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    }

                    // Cálculo puro (sin redondeos intermedios que limiten el rango)
                    double califActual = peli.Puntuacion;
                    int volumenActual = peli.Volumen;

                    double nuevaSuma = (califActual * volumenActual) + calif;
                    int nuevoVolumen = volumenActual + 1;

                    // El double guardará la precisión total en RAM
                    peli.Puntuacion = nuevaSuma / nuevoVolumen;
                    peli.Volumen = nuevoVolumen;

                    // Sincronización con el Catálogo Principal (Fuente de verdad)
                    Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
                    if (aux != nullptr) {
                        do {
                            if (aux->Dato.Titulo == peli.Titulo) {
                                aux->Dato.Puntuacion = peli.Puntuacion;
                                aux->Dato.Volumen = peli.Volumen;
                                aux->Dato.VistasTotales = peli.VistasTotales;
                                break;
                            }
                            aux = aux->siguiente;
                        } while (aux != Catalogo.GetCabeza());
                    }

                    // Persistencia y actualización de vistas
                    GuardarDatos();
                    ActualizarTopVistas(peli);

                    // Limpieza de buffer de consola para asegurar la pausa
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

                    Console::ForegroundColor = ConsoleColor::Green;
                    cout << "\n[!] PELICULA CORRECTAMENTE CALIFICADA" << endl;
                    cout << "Nuevo Rating: " << peli.Puntuacion << " (Guardado con 2 decimales)" << endl;
                    Console::ForegroundColor = ConsoleColor::White;

                    tecla = _getch();
                    Nodo<Pelicula<double>>* curr = VerMasTarde.GetCabeza();
                    if (curr != nullptr) {
                        do {
                            if (curr->Dato.Titulo == peli.Titulo) {
                                curr->Dato.Puntuacion = peli.Puntuacion;
                                break;
                            }
                            curr = curr->siguiente;
                        } while (curr != VerMasTarde.GetCabeza());
                    }
                }

            else if (tecla == 'V') { // REPRODUCIR
                peli.VistasTotales++;
                TopRecienVistos.InsertarAlInicio(peli);
                pila.push(peli);
                CleanScreen();
                Console::ForegroundColor = ConsoleColor::Yellow;
                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(peli.Titulo.length()) / 2), 1);
                cout << "<<<" << LimpiarTexto(peli.Titulo) << ">>>";

                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(peli.Titulo.length()) / 2) + ((int)peli.Titulo.length() + 6 - 30) / 2, 2);
                Console::ForegroundColor = ConsoleColor::Cyan;
                cout << "<<<PELICULA REPRODUCIENDOSE>>>" << endl;

                Dibujo();
                Console::ForegroundColor = ConsoleColor::White;
                cout << endl << "En reproduccion... Presione cualquier tecla para finalizar";
                system("pause>0");

                //actualizar catalogo
                Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
                if (aux != nullptr) {
                    do {
                        if (aux->Dato.Titulo == peli.Titulo) {
                            aux->Dato.Puntuacion = peli.Puntuacion;
                            aux->Dato.Volumen = peli.Volumen;
                            aux->Dato.VistasTotales = peli.VistasTotales;
                            break;
                        }
                        aux = aux->siguiente;
                    } while (aux != Catalogo.GetCabeza());
                }
                //Actualizar datos de VerMasTarde dps de ver una peli por este modo
                Nodo<Pelicula<double>>* curr = VerMasTarde.GetCabeza();
                if (curr != nullptr) {
                    do {
                        if (curr->Dato.Titulo == peli.Titulo) {
                            curr->Dato.VistasTotales = peli.VistasTotales;break;
                        }
                        curr = curr->siguiente;
                    } while (curr != VerMasTarde.GetCabeza());
                }
                GuardarDatos();
                ActualizarTopVistas(peli);
            }

            else if (tecla == 'G') { // GUARDAR
                // Lógica de guardado simplificada sin switch
                CleanScreen();
                bool yaGuardada = false;
                Nodo<Pelicula<double>>* checkG = VerMasTarde.GetCabeza();
                if (checkG != nullptr) {
                    do {
                        if (checkG->Dato.Titulo == peli.Titulo) { yaGuardada = true; break; }
                        checkG = checkG->siguiente;
                    } while (checkG != VerMasTarde.GetCabeza());
                }

                if (yaGuardada) {
                    Console::ForegroundColor = ConsoleColor::Yellow;
                    cout << "LA PELICULA YA ESTA EN TU LISTA." << endl;
                }
                else {
                    VerMasTarde.InsertarAlInicio(peli);
                    Console::ForegroundColor = ConsoleColor::Green;
                    cout << "PELICULA GUARDADA EXITOSAMENTE." << endl;
                }
                Console::ForegroundColor = ConsoleColor::White;
                system("pause");
            }
        }
    }
    void IniciarSesion() {
        auto mayuscula = [](string& s) {
            for (auto& g : s) {
                g = toupper(g);
            }
            };


        String^ ruta = "Datos.bin";
        FileStream^ fs = gcnew FileStream(ruta, FileMode::Open, FileAccess::Read);
        BinaryReader^ leer = gcnew BinaryReader(fs);

        String^ usuario1 = leer->ReadString();
        String^ password1 = leer->ReadString();
        string usuarioO = marshal_as<string>(usuario1), passwordO = marshal_as<string>(password1);
        mayuscula(usuarioO);
        mayuscula(passwordO);
        Usuario = new User(usuarioO, passwordO);
        leer->Close();
        fs->Close();
        string usuario, password;
        do {
            Console::ForegroundColor = ConsoleColor::Yellow;
            Gotoxy((GetAnchoVentana() / 2) - 20, 0);
            cout << "<<<Inicie sesion para poder continuar>>>" << endl;
            Console::ForegroundColor = ConsoleColor::White;
            cout << "Ingrese su usuario: "; cin >> usuario;
            cout << "Ingrese su contrasena: "; cin >> password;
            mayuscula(usuario);
            mayuscula(password);
            system("cls");
        } while (usuario != Usuario->getUser() || password != Usuario->getPass());
        cout << "Bienvenido "<<Usuario->getUser()<<", usted inicio sesion correctamente" << endl << "Presione una tecla para continuar...";
        
        system("pause>0");
    }
    void CrearCuenta() {
        Console::ForegroundColor = ConsoleColor::Yellow;
        Gotoxy((GetAnchoVentana()/2) - 20, 0);
        cout << "<<<Registre su usuario para continuar>>>" << endl;
        Console::ForegroundColor = ConsoleColor::White;
        string usuario, password, passwordC;
        cout << "Ingrese su usuario: "; cin >> usuario;
        cout << "Ingrese su contrasena: "; cin >> password;
        do {
            cout << "Confirme su contrasena: "; cin >> passwordC;
        } while (passwordC != password);
        String^ ruta = "Datos.bin";
        FileStream^ fs = gcnew FileStream(ruta, FileMode::OpenOrCreate);
        BinaryWriter^ pen = gcnew BinaryWriter(fs);
        //Transformar 
        pen->Write(marshal_as<String^>(usuario));
        pen->Write(marshal_as<String^>(password));
        cout << "Se creo su cuenta correctamente" << endl << "Presione una tecla para continuar..."; system("pause>0");
        pen->Close();
        fs->Close();
        system("cls");
    }


    void CargarHistorial() {
        String^ ruta = "HistorialVisitado.txt";
        int orden = 1;
        if (File::Exists(ruta)) {
            StreamReader^ reader = gcnew StreamReader(ruta);
            try {
                while (!reader->EndOfStream) {
                    String^ line = reader->ReadLine();
                    string titulo = marshal_as<string>(line);
                    Nodo<Pelicula<double>>* curr = Catalogo.GetCabeza();
                    do {
                        if (curr->Dato.Titulo == titulo) {
                            Pelicula<double> nuevaPeli(orden, titulo, curr->Dato.Lanzamiento, curr->Dato.Categorias, curr->Dato.Puntuacion, curr->Dato.Volumen, curr->Dato.VistasTotales);
                            pila.push(nuevaPeli);
                            break;
                        }

                        curr = curr->siguiente;
                    } while (curr != Catalogo.GetCabeza());
                    
                }
            }
            catch (Exception^ ex) {
                cout << "Error critico al procesar los datos: " << marshal_as<string>(ex->Message) << endl;
            }
            finally {
                reader->Close();
            }
        }
    }
};