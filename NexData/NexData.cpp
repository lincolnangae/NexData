#include"pch.h"
#include"MediaItem.h"

ListaCircular<string, int, double>* ListaPeliculas;
//EJEMPLO
void Consola(int x, int y) {
    Console::SetCursorPosition(x, y);
}
void BuscadorDePelicular(char type) { //Opcion 1
	if(type == 'C'){
	
	}
	else if(type == 'A'){
	
	}
	else {

	}
}
void CalificarPelicula() { //Opcion 2

}

void PeliculasVistasRecientemente() { //Opcion 3


}

void ReproducirColaDePeliculas() { //Opcion 4

}

void menu() {
    Consola(34, 0); cout << "Bienvenido a Netflix BAMBA";
	Consola(0, 2); cout << "1. Buscar Pelicula por categoria";
	Consola(0, 3); cout << "2. Calificar Pelicula";
	Consola(0, 4); cout<<"3. Ver Peliculas vistas recientemente";
	Consola(0, 5); cout << "4. Reproducir cola de peliculas";
	Consola(0, 6); cout << "5. Salir";
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
				Consola(0, 9); cout << "Ingrese una opcion(C = CALIFICACION, A = ALFABETICO, N = ANIO): "; cin >> opcionBuscador;
				opcionBuscador = toupper(opcionBuscador);
				system("cls");
			} while (opcionBuscador != 'C' && opcionBuscador != 'A' && opcionBuscador != 'N');
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
