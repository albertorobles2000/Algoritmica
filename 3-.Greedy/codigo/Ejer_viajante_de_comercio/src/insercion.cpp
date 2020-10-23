#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <ctime>

#include "funciones.cpp"

using namespace std;

// Calcula las 3 ciudades que estan mas al norte, al oeste y al este
// y las inserta en camino
void calculaCircuitoInicial(Ciudad * datos, Ciudad * camino , int &num_ciudades){
    Ciudad norte; norte.y=-1;
    Ciudad este; este.x=-1;
    Ciudad oeste; oeste.x = numeric_limits<double>::max();
    int i_norte=-1, i_este=-1, i_oeste=-1;
    
    camino[0] = datos[0]; //Empezamos en la ciudad 1
    eliminaCiudad(datos,num_ciudades,0);
    //Buscamos la ciudad que esta mas al norte
    for(int i=0; i<num_ciudades; i++){
        if(datos[i].y > norte.y){
          norte = datos[i]; i_norte = i;
        }
    }
    camino[1] = datos[i_norte];
    eliminaCiudad(datos,num_ciudades,i_norte);
    
    //Buscamos la ciudad que esta mas al oeste
    for(int i=0; i<num_ciudades; i++){
        if(datos[i].x < oeste.x)
          oeste = datos[i]; i_oeste = i;
    }
    camino[2] = datos[i_oeste];
    eliminaCiudad(datos,num_ciudades,i_oeste);

    //Buscamos la ciudad que esta mas al este
    for(int i=0; i<num_ciudades; i++){
        if(datos[i].x > este.x)
          este = datos[i]; i_este = i;
    }
    camino[3] = datos[i_este];
    eliminaCiudad(datos,num_ciudades,i_este);  
}


// Función que busca entre qué par de ciudades supone menor aumento de la distancia total la inserción de una ciudad
// En el parámetro distancia_final devolveremos la distancia del camino resultante con la mejor posición de inserción
int encontrarMejorCamino(Ciudad * camino, Ciudad una_ciudad, int tam_camino, int &distancia_final){
  int distancia_actual = -1;
  int mejor_posicion = 1;
  Ciudad posible_camino[tam_camino + 1]; 
  posible_camino[0] = camino[0]; //La primera ciudad es la misma
  posible_camino[1] = una_ciudad; 

  for(int j = 1; j < tam_camino; ++j)
		posible_camino[j + 1] = camino[j];

  int distancia_minima = distanciaCamino(posible_camino, tam_camino+1);
  Ciudad auxiliar;

	for(int i = 1; i < tam_camino+1; ++i){ // Vamos estudiando las distintas posiciones de inserción
    auxiliar = posible_camino[i%(tam_camino+1)];
    posible_camino[i%(tam_camino+1)] = posible_camino[(i+1)%(tam_camino+1)];
    posible_camino[(i+1)%(tam_camino+1)] = auxiliar;

		//Estudiamos si la distancia es mínima
		distancia_actual = distanciaCamino(posible_camino, tam_camino +1);
		if(distancia_actual < distancia_minima){  // Si la distancia total del camino es mínima la guardamos, además de la posición de inserción
			distancia_minima = distancia_actual;
			mejor_posicion = i;
		}
	}

	// Devolvemos la distancia final por parámetro (pasado por referencia) y la posición de inserción mediante el return
	distancia_final = distancia_minima;
  
	return mejor_posicion;
}

//Devuelve la ciudad que menos perjudica a la distancia del camino, la que posicionada en (mejor_insercion)
//aumenta menos la distancia del camino, se devuelve el incice de la ciudad mas prometedora, y la posicion
// donde insertarla mediante paso por referencia por (mejor_insercion)
int siguienteCiudad(Ciudad * datos,Ciudad * camino,int &num_ciudades, int &tam_camino, int & mejor_insertar){
  
  int distancia_menor = numeric_limits<int>::max();
  int distancia_actual = 0;
  int mejor_ciudad = -1;
  mejor_insertar = -1;
  int insertar_actual = -1;

  for(int i=0; i<num_ciudades; i++){
    insertar_actual = encontrarMejorCamino(camino, datos[i], tam_camino, distancia_actual);
    if(distancia_actual < distancia_menor){
      distancia_menor = distancia_actual;
      mejor_ciudad = i;
      mejor_insertar = insertar_actual;
    }
  }

 return mejor_ciudad;
}

//Función que calcula el camino por inserción aplicando greedy por el puntero a ciudades
//camino
void calculaCaminoInsercion(Ciudad * datos, Ciudad * camino , int num_ciudades){
  calculaCircuitoInicial(datos,camino,num_ciudades); //Construcción del recorrido parcial.
  
  int tam_camino = 4;
  int indice_ciudad = -1;
  int indice_insertar = -1;
  while(num_ciudades > 0){ //Mientras no hayamos seleccionado todas las ciudades
    //Buscamos la siguiente ciudad a insertar
    indice_ciudad = siguienteCiudad(datos,camino,num_ciudades, tam_camino, indice_insertar);
    //La insertamos en camino en (indice_insertar)
    insertaCiudad(camino, tam_camino, datos[indice_ciudad], indice_insertar);
    //Eliminamos la ciudad en la posicion (indice_ciudad) de datos
    eliminaCiudad(datos, num_ciudades, indice_ciudad);
  }
}

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
  
  //Reservamos espacio para el camino
  Ciudad * camino = new Ciudad[num_ciudades];

  /*
  cout << "Ciudades" << endl;
  for(int i=0; i<num_ciudades; i++){
    cout << datos[i].numero  << " :: " << datos[i].x << " :: " << datos[i].y << endl;
  }
  */

  clock_t tantes, tdespues;
  tantes = clock();
    calculaCaminoInsercion(datos, camino, num_ciudades);  
  tdespues = clock();

  cout << num_ciudades << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;

  /*
  cout << "\nCamino" << endl;
  for(int i=0; i<num_ciudades; i++){
    cout << camino[i].numero  << " :: " << camino[i].x << " :: " << camino[i].y << endl;
  }
  */
  //cout << "\n\nDistancia: " << distanciaCamino(camino, num_ciudades) << endl;

  //Liberamos memoria
  delete []datos;
  delete []camino;

}
