#pragma once
#include "Pila.h"
#include "Usuario.h"
#include "ListaDobleCircular.h"
#include "ListaSimple.h"
#include "Pelicula.h"
#include "GestionadorArchivos.h"
#include "AlgoritmoServicio.h"
//Clase principal que controla el flujo de datos
enum class TipoTop { RECIENTES, MASVISTAS , VISTOSRECIENTES };
class Gestionador {
private:
    ListaCircularDoble<Pelicula<double>> Catalogo;
    ListaCircularDoble<Pelicula<double>> VerMasTarde; //Los recientes vistos
    ListaSimple<Pelicula<double>> TopMasVistos; //Los Mas Vistos
    ListaSimple<Pelicula<double>> TopRecientes; //Los ultimos de la lista
    ListaSimple<Pelicula<double>> TopRecienVistos; //Los ultimos que vemos de la lista
    Pila<Pelicula<double>> Pila;

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

    void Iniciar() {
        GestionadorArchivos repo;
        repo.CargarTodo(Catalogo, TopRecientes);

        Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
        if (aux != nullptr) {
            do {
                ActualizarTopVistas(aux->Dato);
                aux = aux->siguiente;
            } while (aux != Catalogo.GetCabeza());
        }
    }

    void Guardar() {
        GestionadorArchivos repo;
        repo.GuardarTodo(Catalogo);
    }

    void CargarHistorial() {
        GestionadorArchivos repo;
        repo.CargarHistorial(Catalogo, Pila);
    }

    void GuardarPila() {
        GestionadorArchivos repo;
        repo.GuardarHistorial(Pila);
    }

    void ActualizarTopVistas(Pelicula<double> p) {
        //verificar si ya existe para no cambiar
        if (p.GetTitulo() == Top1.GetTitulo()) {
            Top1 = p; TopVistas1 = p.GetVistasTotales();
        }
        else if (p.GetTitulo() == Top2.GetTitulo()) {
            Top2 = p; TopVistas2 = p.GetVistasTotales();
        }
        else if (p.GetTitulo() == Top3.GetTitulo()) {
            Top3 = p; TopVistas3 = p.GetVistasTotales();
        }
        else {
            //si es nueva, la logica normal
            if (p.GetVistasTotales() > TopVistas1) {
                Top3 = Top2; TopVistas3 = TopVistas2;
                Top2 = Top1; TopVistas2 = TopVistas1;
                Top1 = p; TopVistas1 = p.GetVistasTotales();
            }
            else if (p.GetVistasTotales() > TopVistas2) {
                Top3 = Top2; TopVistas3 = TopVistas2;
                Top2 = p; TopVistas2 = p.GetVistasTotales();
            }
            else if (p.GetVistasTotales() > TopVistas3) {
                Top3 = p; TopVistas3 = p.GetVistasTotales();
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
                    cout << "\n>> '" << LimpiarTexto(nodoElegido->Dato.GetTitulo()) << "' agregada exitosamente." << endl;
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
            cout << "--------------------------------------------------------------------------------------" << endl;
            cout << "[Q] Anterior | [E] Siguiente | [S] Reordenar | [V] Ver todas las peliculas | [0] Salir" << endl;
            cout << "--------------------------------------------------------------------------------------" << endl;
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
            else if (tecla >= '1' && tecla <= '9') {
                int seleccion = tecla - '0';
                if (seleccion <= limite) {
                    Nodo<Pelicula<double>>* nodoElegido = inicioVentana;
                    for (int k = 1; k < seleccion; k++) nodoElegido = nodoElegido->siguiente;

                    ImprimirInformacionDesdePelicula(nodoElegido->Dato);
                }
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
                    Pelicula<double> peli(0, curr->Dato.GetTitulo(), curr->Dato.GetLanzamiento(), curr->Dato.GetCategorias(), curr->Dato.GetPuntuacion(), curr->Dato.GetVolumen(), curr->Dato.GetVistasTotales());
                    peli.GetVistasTotales();
                    Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
                    if (aux != nullptr) {
                        do {
                            if (aux->Dato.GetTitulo() == peli.GetTitulo()) {
                                aux->Dato.SetPuntuacion(peli.GetPuntuacion());
                                aux->Dato.SetVolumen(peli.GetVolumen());
                                aux->Dato.SetVistasTotales(peli.GetVistasTotales());
                                Pila.Push(peli);
                                break;
                            }
                            aux = aux->siguiente;
                        } while (aux != Catalogo.GetCabeza());
                    }
                    CargarHistorial();
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
            return a.GetPuntuacion() > b.GetPuntuacion();
            };
        auto ordenAnioDesc = [](const Pelicula<double>& a, const Pelicula<double>& b) {
            return a.GetLanzamiento() > b.GetLanzamiento();
            };
        auto ordenAlfabeticoAsc = [](const Pelicula<double>& a, const Pelicula<double>& b) {
            return a.GetTitulo() < b.GetTitulo();
            };


        Nodo<Pelicula<double>>* bajo = listaTemporal.GetCabeza();
        Nodo<Pelicula<double>>* alto = bajo->anterior;

        AlgoritmoServicio ObjAlgoritmo;
        // PASO 3: Ordenar la copia, no el original
        switch (toupper(criterio)) {
        case 'C': ObjAlgoritmo.QuickSort(bajo, alto, ordenCalificacionDesc); break;
        case 'N': ObjAlgoritmo.QuickSort(bajo, alto, ordenAnioDesc); break;
        case 'A': ObjAlgoritmo.QuickSort(bajo, alto, ordenAlfabeticoAsc); break;
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

        while (tecla != '0') {
            CleanScreen();

            Nodo<Pelicula<double>>* tempCheck = VerMasTarde.GetCabeza();
            bool estaGuardada = false;
            if (tempCheck != nullptr) {
                do {
                    if (tempCheck->Dato.GetTitulo() == peli.GetTitulo()) {
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
                    int ancho = GetAnchoVentana();
                    int xCentro = (ancho / 2) - 20;

                    Console::ForegroundColor = ConsoleColor::Yellow;
                    Gotoxy(xCentro, 0); cout << "========================================" << endl;
                    Gotoxy(xCentro, 1); cout << "       CALIFICAR PELICULA               " << endl;
                    Gotoxy(xCentro, 2); cout << "========================================" << endl;

                    Gotoxy(xCentro, 4);
                    Console::ForegroundColor = ConsoleColor::White;
                    cout << "Pelicula: ";
                    Console::ForegroundColor = ConsoleColor::Cyan;
                    cout << LimpiarTexto(peli.GetTitulo());

                    while (true) {
                        Gotoxy(xCentro, 6); cout << "                                        ";
                        Gotoxy(xCentro, 7); cout << "                                        ";

                        Gotoxy(xCentro, 6);
                        Console::ForegroundColor = ConsoleColor::White;
                        cout << "Ingrese nota [0.0 - 10.0]: ";

                        if (cin >> calif && calif >= 0 && calif <= 10) {
                            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                            break;
                        }

                        Console::ForegroundColor = ConsoleColor::Red;
                        Gotoxy(xCentro, 7);
                        cout << "Error: Valor invalido. Pulse para continuar.";

                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        _getch();
                    }

                    peli.ActualizarCalificacion(calif);

                    Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
                    if (aux != nullptr) {
                        do {
                            if (aux->Dato.GetTitulo() == peli.GetTitulo()) {
                                aux->Dato.SetPuntuacion(peli.GetPuntuacion());
                                aux->Dato.SetVolumen(peli.GetVolumen());
                                aux->Dato.SetVistasTotales(peli.GetVistasTotales());
                                break;
                            }
                            aux = aux->siguiente;
                        } while (aux != Catalogo.GetCabeza());
                    }

                    Nodo<Pelicula<double>>* curr = VerMasTarde.GetCabeza();
                    if (curr != nullptr) {
                        do {
                            if (curr->Dato.GetTitulo() == peli.GetTitulo()) {
                                curr->Dato.SetPuntuacion(peli.GetPuntuacion());
                                break;
                            }
                            curr = curr->siguiente;
                        } while (curr != VerMasTarde.GetCabeza());
                    }

                    CargarHistorial();
                    ActualizarTopVistas(peli);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

                    Gotoxy(xCentro, 9);
                    Console::ForegroundColor = ConsoleColor::Green;
                    cout << "[!] CALIFICACION REGISTRADA CON EXITO";
                    Gotoxy(xCentro, 10);
                    cout << "Nuevo Rating: " << peli.GetPuntuacion();

                    Console::ForegroundColor = ConsoleColor::White;
                    Gotoxy(xCentro, 12);
                    cout << "Presione cualquier tecla para continuar...";
                    tecla = _getch();
                }

            else if (tecla == 'V') { // REPRODUCIR
                peli.AumentarVistas();
                TopRecienVistos.InsertarAlInicio(peli);
                Pila.Push(peli);
                CleanScreen();
                Console::ForegroundColor = ConsoleColor::Yellow;
                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(peli.GetTitulo().length()) / 2), 1);
                cout << "<<<" << LimpiarTexto(peli.GetTitulo()) << ">>>";

                Gotoxy((int)(GetAnchoVentana() / 2) - ((int)(peli.GetTitulo().length()) / 2) + ((int)peli.GetTitulo().length() + 6 - 30) / 2, 2);
                Console::ForegroundColor = ConsoleColor::Cyan;
                cout << "<<<PELICULA REPRODUCIENDOSE>>>" << endl;

                Dibujo();
                Console::ForegroundColor = ConsoleColor::White;
                cout << endl << "En reproduccion... Presione cualquier tecla para finalizar";
                system("pause>0");

                //actualizar catalogo principal
                Nodo<Pelicula<double>>* aux = Catalogo.GetCabeza();
                if (aux != nullptr) {
                    do {
                        if (aux->Dato.GetTitulo() == peli.GetTitulo()) {
                            aux->Dato.SetPuntuacion(peli.GetPuntuacion());
                            aux->Dato.SetVolumen(peli.GetVolumen());
                            aux->Dato.SetVistasTotales(peli.GetVistasTotales());
                            break;
                        }
                        aux = aux->siguiente;
                    } while (aux != Catalogo.GetCabeza());
                }

                // actualiar ver mas tarde
                Nodo<Pelicula<double>>* curr = VerMasTarde.GetCabeza();
                if (curr != nullptr) {
                    do {
                        if (curr->Dato.GetTitulo() == peli.GetTitulo()) {
                            curr->Dato.SetVistasTotales(peli.GetVistasTotales());
                            curr->Dato.SetPuntuacion(peli.GetPuntuacion());
                            break;
                        }
                        curr = curr->siguiente;
                    } while (curr != VerMasTarde.GetCabeza());
                }

                CargarHistorial();
                ActualizarTopVistas(peli);
            }

            else if (tecla == 'G') { // GUARDAR
                // Lógica de guardado simplificada sin switch
                CleanScreen();
                bool yaGuardada = false;
                Nodo<Pelicula<double>>* checkG = VerMasTarde.GetCabeza();
                if (checkG != nullptr) {
                    do {
                        if (checkG->Dato.GetTitulo() == peli.GetTitulo()) { yaGuardada = true; break; }
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
    
    void MostrarListaHistorico() {
        char tecla = ' ';
        while (tecla != '0') {
            system("cls");

            // Título con formato para cumplir con criterios de Usabilidad (Rúbrica TB1)
            Console::ForegroundColor = ConsoleColor::Yellow;
            Gotoxy(GetAnchoVentana() / 2 - 20, 0); cout << "========================================" << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 1); cout << "    VISTA - HISTORIAL - PELICULAS       " << endl;
            Gotoxy(GetAnchoVentana() / 2 - 20, 2); cout << "========================================" << endl;
            Console::ForegroundColor = ConsoleColor::White;
            cout << "\n[] Este historia les un reflejo de lo visto, no interactivo." << endl;
            // Acceso a la cima de la Pila
            Nodo<Pelicula<double>>* actual = Pila.GetCima();

            // Validación de estructura vacía
            if (actual == nullptr) {
                Console::ForegroundColor = ConsoleColor::Red;
                cout << "\n[!] El historial de visualizacion esta vacio." << endl;
                Console::ForegroundColor = ConsoleColor::White;
                cout << "\nPresione cualquier tecla para regresar...";
                tecla = _getch();
                return;
            }

            int i = 1;
            // Recorrido de la Pila utilizando punteros
            // Complejidad Temporal: O(n)
            while (actual != nullptr) {
                Console::ForegroundColor = ConsoleColor::DarkYellow;
                cout << "[" << i << "] ";
                Console::ForegroundColor = ConsoleColor::White;

                // Llamado al método de la entidad Pelicula (Abstracción)
                actual->Dato.MostrarEnLista();

                Console::ForegroundColor = ConsoleColor::DarkGray;
                cout << "----------------------------------------------------" << endl;
                Console::ForegroundColor = ConsoleColor::White;

                // Avance al siguiente nodo (hacia el fondo de la pila)
                actual = actual->siguiente;
                i++;
            }

            Console::ForegroundColor = ConsoleColor::Yellow;
            cout << "\n[0] Salir al menu principal" << endl;
            Console::ForegroundColor = ConsoleColor::White;

            tecla = _getch();
        }
    }

};