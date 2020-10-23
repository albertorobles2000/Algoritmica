#include <iostream>
#include "funciones.cpp"

using namespace std;

// Funcion para insertar una ciudad (se pasara la ciudad mas alejada del resto) en la mejor posicion,
// es decir, donde menor aumento de la distancia del camino suponga 
void insertaPeorCiudad(Ciudad * camino, int num_ciudades, int peor_ciudad){
    Ciudad aInsertar = camino[peor_ciudad];
    eliminaCiudad(camino, num_ciudades, peor_ciudad);
    int mejor_distancia = numeric_limits<int>::max();
    int mejor_indice_insert = -1;
    int dist_actual = -1;

    // Buscamos la mejor posicion de insercion
    for(int i=0; i<num_ciudades; i++){
        dist_actual = distanciaEntreCiudades(camino[i],aInsertar);
        dist_actual += distanciaEntreCiudades(aInsertar,camino[(i+1)%num_ciudades]);

        if(dist_actual < mejor_distancia){
            mejor_distancia = dist_actual;
            mejor_indice_insert = i+1;
        }
    }
    // Insertamos la ciudad en cuestion
    insertaCiudad(camino, num_ciudades, aInsertar, mejor_indice_insert);
}

// Función para buscar el nodo mas alejado de sus dos ciudades vecinas
int buscaCiudadMasAislada(Ciudad * camino, int num_ciudades, bool * usados){
    int indiceNodoBuscado = -1;
    int distanciaMaxima = -1; 
    int distanciaEstudiada; // Contendra la distancia a las ciudades vecinas del nodo estudiado en cada momento

    // No empieza en i=0 ya que la primera ciudad no se coge.
    for(int i = 1; i < num_ciudades; ++i){
        distanciaEstudiada = distanciaEntreCiudades(camino[i], camino[(i+1) % num_ciudades]);
        if(distanciaEstudiada > distanciaMaxima && !usados[camino[i].numero-1]){ // Si la distancia a sus vecinos es la menor, almacenamos la informacion
            distanciaMaxima = distanciaEstudiada;
            indiceNodoBuscado = i;
        }
    }
    //Ponemos a true el indice de la ciudad que ya ha sido seleccionada para inserción
    usados[camino[indiceNodoBuscado].numero-1] = true;
    
    return indiceNodoBuscado;
}

// 
void calculaCaminoPropio(Ciudad * datos, Ciudad * camino , int num_ciudades){
    calculaCaminoCercania(datos, camino , num_ciudades);

    bool usados[num_ciudades]={0};
    
    int indice_peor;
    // como buscamos en todas las ciudades excepto la primera,
    // el bucle es hasta ciudades-1
    // el valor de "i" es irrelevante en este 
    for(int i=0; i<num_ciudades-1;++i){
      indice_peor = buscaCiudadMasAislada(camino, num_ciudades, usados);
      insertaPeorCiudad(camino, num_ciudades, indice_peor);
    }
} 



int main(int argc, char ** argv){

  //Inicializacion del vector de ciudades
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
    // los datos son las ciudades que generemos nosotros
    // para realizar mejor las grafica
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
  calculaCaminoPropio(datos, camino, num_ciudades);
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