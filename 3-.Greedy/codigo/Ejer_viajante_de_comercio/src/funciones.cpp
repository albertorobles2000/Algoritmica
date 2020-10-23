#include <cmath>
#include <fstream>
#include <string>
#include <limits>
#include <iostream>

using namespace std;

// Definimos una ciudad, su nunmero y coordenadas
struct Ciudad{
  int numero;
  double x;
  double y;
};

// Calcula y devuelve la distancia entre dos ciudades
int distanciaEntreCiudades(Ciudad a, Ciudad b){
  return sqrt(pow((b.x-a.x),2)+pow((b.y-a.y),2));
}

Ciudad * generaCiudades(int num_ciudades){
  Ciudad * datos = new Ciudad[num_ciudades];
  Ciudad nueva;
  srandom(time(0));
  for(int i=0; i<num_ciudades; i++){
    nueva.numero = i+1;
    nueva.x = rand() % 300;
    nueva.y = rand() % 300;
    datos[i] = nueva;
  }
  return datos;
}

// Lectura de fichero de datos .tsp que genera una ciudad
Ciudad * leerFichero(string fichero, int & num_ciudades){

  ifstream fi(fichero);
  if (!fi){
     cout << "No puedo abrir " << fichero << endl;
     exit(1);
  }
  //cout << "fichero: " << fichero << endl;
  string linea;
  getline(fi,linea);  //name
  getline(fi,linea);  //comment
  getline(fi,linea);  // type

  getline(fi, linea, ' '); // dimension hasta el espacio
  fi >> num_ciudades; //cogemos las ciudades

  getline(fi,linea);  //edge
  getline(fi,linea);  // node coord

  getline(fi,linea);  // next line

  Ciudad * ciudades = new Ciudad[num_ciudades];

  Ciudad nueva;
  for(int i=0; i<num_ciudades; i++){
    //recogemos datos
    fi >> nueva.numero;
    fi >> nueva.x;
    fi >> nueva.y;

    // la insertamos
    ciudades[i] = nueva;
  }
  return ciudades;

}

// Insertar la ciudad (a_insertar) en la posicion que se le pasa como parametro (indice)
void insertaCiudad(Ciudad * camino, int &elementos, Ciudad a_insertar, int indice){
  for(int i = elementos; i>indice; i--){
    camino[i] = camino[i-1];
  }
  camino[indice] = a_insertar;
  elementos++;
}

// Elimina la ciudad en el indice del vector datos, de tamanio num_ciudades
void eliminaCiudad(Ciudad * datos, int & num_ciudades, int indice){
  for(int i=indice; i<num_ciudades-1; i++){
    datos[i] = datos[i+1];
  }
  num_ciudades--;
}

// Calcula y devuelve la distancia del camino
int distanciaCamino(Ciudad * camino, int ciudades){
  int distancia = 0;
  for(int i=0; i<ciudades; i++){
    distancia += distanciaEntreCiudades(camino[i], camino[(i+1)%ciudades]);
  }
  
  return distancia;
}


//CERCANIA.CPP
//CALCULA CAMINO CERCANIA
// Calcula el camino con búsqueda voraz basada en cercanía
void calculaCaminoCercania(Ciudad * datos, Ciudad * camino , int num_ciudades){
  //Punto inicial
  int tam_camino = 0;
  int ciudadInicio = 0;
  camino[0] = datos[ciudadInicio];
  tam_camino++;

  //Se elimina la primera ciudad de datos, ya esta añadida en las no visitadas
  eliminaCiudad(datos,num_ciudades,ciudadInicio);

  //Se busca la siguiente ciudad mas cercana, se añade al camino y se elimina de las no visitadas
  while(num_ciudades > 0){
    int distancia_menor = distanciaEntreCiudades(camino[tam_camino-1], datos[0]);
    int ciudad = 0;

    // Calculamos la ciudad más cercana (distancia mínima)
    for(int i=1; i<num_ciudades; i++){
      if(distanciaEntreCiudades(camino[tam_camino-1], datos[i]) < distancia_menor){
        distancia_menor = distanciaEntreCiudades(camino[tam_camino-1], datos[i]);
        ciudad = i;
      }
    }

	  //Insertamos la ciudad más cercana encontrada en el camino
    camino[tam_camino] = datos[ciudad];
    tam_camino++; //actualizamos tamanio camino
	  //La eliminamos de las ciudades no visitadas.
    eliminaCiudad(datos,num_ciudades,ciudad);
  }
}