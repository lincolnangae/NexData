#include"pch.h"
#include"MediaItem.h"

ListaCircular<string, int, double>* ListaPeliculas;
//EJEMPLO
void Consola(int x, int y) {
    Console::SetCursorPosition(x, y);
}
void BuscadorDePelicular() {

}

void menu() {
    Consola(34, 0); cout << "Bienvenido a Netflix BAMBA";
}
void Iniciador() {
    Console::SetWindowSize(100, 400);
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
				//Convertir clase administrativa String a string de c++ para agregar a la lista circular
				string titulo = marshal_as<string>(title), generoS = marshal_as<string>(genero);
				ListaPeliculas->Agregar_final(titulo, generoS, year, puntuacion);
			}
		}
		catch(Exception^mi_ex){
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
    while (1) {
        menu();
    }
    return 0;
}
