#include"pch.h"
#include"MediaItem.h"

ListaCircular<string, int, double>* ListaPeliculas;
//EJEMPLO
void Consola(int x, int y) {
    Console::SetCursorPosition(x, y);
}
void ImprimirInfoPelicula(Pelicula<string, int, double>* pelicula) {
	//Aca personalmente se imprimira la pelicula seleccionada, con toda su informacion, y se le dara la opcion al usuario de calificarla o agregarla a su cola
}

void BuscadorDePelicular(char type) { //Opcion 1
	vector<Pelicula<string, int, double>*> peliculasOrdenadas;
	ListaCircular<string, int, double>* listaAux = ListaPeliculas;
	if (type == 'C') { //Calificacion
		peliculasOrdenadas = ListaPeliculas->OrdenarCalificacion();
		int PrimerIndx = 0;
		do {
			Consola(34, 0); cout << "Bienvenido a Netflix BAMBA";
			Consola(0, 1); cout << "Peliculas ordenadas por calificacion: ";
			int posiY = 3;
			for (int i = PrimerIndx; i < PrimerIndx + 8; i++) {
				Consola(5, posiY); cout << i + 1 << ". ";
				if (i < peliculasOrdenadas.size()) {
					cout << peliculasOrdenadas[i]->title << " | " << peliculasOrdenadas[i]->genero << " | " << peliculasOrdenadas[i]->year << " | " << peliculasOrdenadas[i]->puntuacion << " | " << peliculasOrdenadas[i]->view << endl;
				}
				posiY++;
			}
			char Opcion;
			do {
				Consola(0, posiY + 1); cout << "Elegir una opcion (S = Siguiente pagina, A = Pagina anterior, V = Ver Informacion de pelicula, E = Salir al menu principal): "; cin >> Opcion;
			} while (Opcion != 'S' && Opcion != 'A' && Opcion != 'V' && Opcion != 'E');
			switch (Opcion) {
			case 'S': 
				PrimerIndx += 9;
				if (PrimerIndx >= peliculasOrdenadas.size()) {
					PrimerIndx = 0;
				}
				break;
			case 'A': 
				PrimerIndx -= 9;
				if (PrimerIndx < 0) {
					if (peliculasOrdenadas.size() % 9 == 0) {
						PrimerIndx = peliculasOrdenadas.size() - 9;
					}
					else {
						PrimerIndx = peliculasOrdenadas.size() - (peliculasOrdenadas.size() % 9);
					}
				}
				break;
			case 'V': 
				int opcionIndx;
				do {
					Consola(0, posiY + 2); cout << "Elegir una pelicula: "; cin >> opcionIndx;
				} while ((opcionIndx<PrimerIndx + 1 || opcionIndx>PrimerIndx + 9) && opcionIndx<peliculasOrdenadas.size());
				//ENVIAR Y CORREGIR TODO XFAAA, tdv no pruebo codigo solo he tirado lineas
				break;
			case 'E': return;

			}

		} while (1);
		system("pause>0");
	}
	else if(type == 'A'){
		

	}
	else if(type == 'N') {
		peliculasOrdenadas = ListaPeliculas->OrdenarPorAnio();

	}
	else {

	}


	system("cls");
}
void PeliculasVistasRecientemente() { //Opcion 3

}
void ReproducirColaDePeliculas() { //Opcion 4

}

void menu() {
    Consola(34, 0); cout << "Bienvenido a Netflix BAMBA";
	Consola(0, 2); cout << "1. Buscar Pelicula por categoria";
	Consola(0, 3); cout<<"2. Ver Peliculas vistas recientemente";
	Consola(0, 4); cout << "3. Reproducir cola de peliculas";
	Consola(0, 5); cout << "4. Salir";
}
void Iniciador() {
    Console::SetWindowSize(100, 200);
    Console::CursorVisible = 0;
    ListaPeliculas = new ListaCircular<string, int, double>();
    String^ ruta = "ARCHIVOS_DE_TEXTO.txt";

	//Lectura Archivos TXT
	if (File::Exists(ruta)) {
		StreamReader^ txt = gcnew StreamReader(ruta);
		try {
			while (!txt->EndOfStream) {
				String^ Line = txt->ReadLine();
				cli::array<String^>^ datos = Line->Split(',');
				String^ title = datos[0];
				String^ genero = datos[1];
				int year = Convert::ToInt32(datos[2]);
				double puntuacion = Convert::ToDouble(datos[3]);
				int views = Convert::ToInt32(datos[4]);
				//Convertir clase administrativa String a string de c++ para agregar a la lista circular
				string titulo = marshal_as<string>(title), generoS = marshal_as<string>(genero);
				// pelicula -> Pelicula
				titulo[0] = toupper(titulo[0]);
				generoS[0] = toupper(generoS[0]);
				ListaPeliculas->Agregar_final(titulo, generoS, year, puntuacion, views);
			}
		}
		catch (Exception^ mi_ex) {
			system("cls");
			cout << "El archivo de texto no se puedo leer correctamente" << endl;
			system("pause>0");
		}
		finally {
			txt->Close();
		}
	}
}
int main()
{
    Iniciador();
	bool ok = 1;
    while (ok) {
        menu();
		int opcion, indiceY = 7;
		do {
			Consola(0, indiceY);cout << "Ingrese una opcion: ";
			cin >> opcion;
			indiceY++;
		} while ((opcion) < 1 || (opcion) > 5);
		system("cls");
		switch (opcion) {
		case 1:
			
			char opcionBuscador;
			do {
				Consola(34, 0); cout << "Bienvenido a Netflix BAMBA";
				Consola(0, 3); cout << "Buscador de pelicula: ";
				Consola(0, 5); cout << "Buscar Peliculas por: ";
				Consola(0, 6); cout << "CALIFICACION (C)";
				Consola(0, 7); cout << "ALFABETICO (A)";
				Consola(0, 8); cout << "ANIO (N)";
				Consola(0, 9); cout << "GENERO (G)";
				Consola(0, 10); cout << "Ingrese una opcion(C = CALIFICACION, A = ALFABETICO, N = ANIO, G = GENERO): "; cin >> opcionBuscador;
				opcionBuscador = toupper(opcionBuscador);
				system("cls");
			} while (opcionBuscador != 'C' && opcionBuscador != 'A' && opcionBuscador != 'N' && opcionBuscador != 'G');
			BuscadorDePelicular(opcionBuscador);
			break;
		case 2: break;
		case 3: break;
		case 4: break;
		case 5:
			char opcionSalir;
			do {
				Consola(34, 0); cout << "Bienvenido a Netflix BAMBA";
				Consola(0, 3);cout << "Estas seguro que deseas salir? (S/N): ";
				cin >> opcionSalir;
				system("cls");
			} while (opcionSalir != 'S' && opcionSalir != 'N');
			if (opcionSalir == 'S') {
				ok = 0;
				system("cls");
				Consola(0, 0); cout << "Gracias por usar Netflix BAMBA";
				Consola(0, 1); cout << "Vuelve pronto!";
				system("pause>0");
			}
			break;
		}


    }
    return 0;
}
