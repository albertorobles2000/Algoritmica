#include <iostream>
#include <ctime>
#include "funciones.cpp"

using namespace std;

int main(int argc, char ** argv){

  int num_ciudades = -1;
  Ciudad * datos = 0;
  if(argc < 2){
    cerr << "Numero de argumentos invalido. Falta fichero de datos.\n";
    exit(-1);
  }
  if(argc==2){
    string fichero = argv[1];
    datos = leerFichero(fichero, num_ciudades); 
  }
  else{
      num_ciudades = atoi(argv[2]);
      datos = generaCiudades(num_ciudades);
  }
  
  Ciudad * camino = new Ciudad[num_ciudades];

/*
  cout << "Ciudades" << endl;
  for(int i=0; i<num_ciudades; i++){
    cout << datos[i].numero  << " :: " << datos[i].x << " :: " << datos[i].y << endl;
  }
*/

  clock_t tantes, tdespues;
  tantes = clock();
    calculaCaminoCercania(datos, camino, num_ciudades);
  tdespues = clock();

  cout << num_ciudades << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;

  /*
	cout << "\nCamino" << endl;
   for (int i = 0; i < num_ciudades; i++)
   {
	   cout << camino[i].numero << " :: " << camino[i].x << " :: " << camino[i].y << endl;
   } 
  */
    
//cout << "\n\nDistancia: " << distanciaCamino(camino, num_ciudades) << endl;  


//Liberamos memoria
delete []datos;
delete []camino;
}
