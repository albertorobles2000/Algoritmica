#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
using namespace std;

// Leemos el fichero de datos y devolvemos un array con los contenedores.
// Las variables de pesoMax y numContenedores se inicializan correctamente
// segun la lectura.
int * leerFichero(string fichero, int & pesoMaxBuque, int & numContenedores){

  ifstream fi(fichero);
  if (!fi){
     cout << "No puedo abrir " << fichero << endl;
     exit(1);
  }

  //cout << "fichero: " << fichero << endl;
  string linea;
  getline(fi,linea); //linea info
  fi >> pesoMaxBuque; //Peso maximo    
  fi >> numContenedores; //Numero de contenedores

  cout << "Peso maximo del buque: " << pesoMaxBuque << endl;
  cout << "Numero contenedores: " << numContenedores << endl;

  getline(fi,linea);  // linea vacia

  int * contenedores = new int[numContenedores];

  int pesoCont;
  for(int i=0; i<numContenedores; ++i){
    //recogemos los pesos
    fi >> pesoCont;
    
    // los insertamos
    contenedores[i] = pesoCont;    
  }
  return contenedores;
}

// El parametro criterio indica aquel que se va a seguir para calcular la solucion:
// Si su valor es 1, se intentara maximizar el NUMERO de contenedores
// Si su valor es 2, se intentara maximizar el PESO que se carga en el barco
// Supondremos que el valor introducido es correcto
void SolucionCaminoContenedores(int * datos, int * camino , int peso_max_buque, int &num_contenedores, int criterio){
  int contenedor_elegido = 0;  // Almacena el indice del contenedor menos pesado en cada momento
  int carga_actual = 0;
  bool almacenados[num_contenedores] = {0}; // Todos tienen valor false inicialmente.
  bool pesoSuperado = false; 

  // Vamos añadiendo contenedores mientras no se supere el numero maximo ni la carga soportada
  int i, lim_peso;

  // Si el criterio es 1, se busca Maximizar el NUMERO de contenedores
  if(criterio == 1){
    // Recorremos todos los contenedores y mientras no se supere el Peso Maximo
    // Loop del camino buque
    for(i = 0; i < num_contenedores && !pesoSuperado; ++i){  
      lim_peso = numeric_limits<int>::max();
      // Este loop es el que encuentra el contenedor de menor peso
      for(int j = 0; j < num_contenedores; ++j){
        // Chequeamos que sea de menor peso y este disponible
        if(datos[j] < lim_peso && !almacenados[j]){
          // Actualizamos contenedor y el peso 
          contenedor_elegido = j;
          lim_peso = datos[j];
        }
      }
        // Volvemos a comprobar que el peso sigue siendo menor:
        // Si el contenedor que se va a añadir hace que se supere la carga maxima, no se mete y termina el proceso
      if(carga_actual + datos[contenedor_elegido] > peso_max_buque){
        pesoSuperado = true;
      }
      else{
        // Almacenamos el contenedor menos pesado en la solucion
        camino[i] = contenedor_elegido;
        // Indicamos que ese contenedor ya se ha almacenado
        almacenados[contenedor_elegido] = true;
        // Actualizamos la carga del buque
        carga_actual += datos[contenedor_elegido];
      }
    }
  }

  // Si el criterio es 2, se busca Maximizar el PESO de la carga
  else{
    for(i = 0; i < num_contenedores && !pesoSuperado; ++i){
      lim_peso = 0;
      contenedor_elegido = -1;  // Para comprobar si se ha encontrado un minimo admisible
                                // ya que en caso de encontrarlo, ese indice cambiara.

      //buscamos el contenedor de maximo peso admisible (el mayor que quepa)
      for(int j = 0; j < num_contenedores; ++j){
        if(datos[j] > lim_peso && !almacenados[j]){
          // Si el peso efectivamente cabe dentro
          if(carga_actual + datos[j] <= peso_max_buque){
            contenedor_elegido = j;
            lim_peso = datos[j];
          }
        }
      }
      // Si el contenedor que se va a añadir hace que se supere la carga maxima, no se mete y termina el proceso
      if(contenedor_elegido == -1){ // Significa que no se ha encontrado ningun contenedor adecuado: ninguno cabe
        pesoSuperado = true;
      }
      else{
        // Almacenamos el contenedor menos pesado en la solucion
        camino[i] = contenedor_elegido;
        // Indicamos que ese contenedor ya se ha almacenado
        almacenados[contenedor_elegido] = true;
        // Actualizamos la carga del buque
        carga_actual += datos[contenedor_elegido];
      }
    }
  }

  // Actualizamos el numero final de contenedores almacenados
  num_contenedores = i - 1;
}

// Generador de contenedores aleatorios
int * GeneradorDatos(int & pesoMax, int & numCont){
  
  int * datos = new int[numCont];
  int peso_cont;
  srandom(time(0));
  for(int i=0; i<numCont; ++i){
    peso_cont = (rand() % (pesoMax /3) )+1;
    datos[i] = peso_cont;
  }
  return datos;
}

int main(int argc, char ** argv){

  int pesoMaxBuque = 0, numContenedores = 0;  
  int * contenedores;

  if(argc == 1){
    cerr << "Numero de argumentos invalido. \nFormato: <fichero>\n"; // o <num_contenedores>\n";
    exit(-1);
  }
  /*
  //Ejecucion con generador aleatorios
  if(argc == 2){
    pesoMaxBuque = 186000;
    numContenedores = atoi(argv[1]);
    contenedores = GeneradorDatos(pesoMaxBuque, numContenedores);
  }
  */
  //Para la ejecucion con fichero
  if(argc == 2){
    string fichero = argv[1];
    contenedores = leerFichero(fichero, pesoMaxBuque, numContenedores);
  }


  // Mostramos los pesos de cada contenedor
  
  cout << "Contenedores leidos: " << endl;
  for(int i=0; i<numContenedores; ++i){
    cout << "Peso del Contenedor " << "[ " << i << " ]" << " = " << contenedores[i] << endl;
  }
  

	// El camino lo almacenaremos en la variable camino_buque
  // segun los numero de contenedores que tengamos
  // aunque luego no se usen todos
  int * camino_buque = new int[numContenedores];

  // Calculamos el camino y lo ponemos en la variable camino_buque
  // Calculamos el maximo numero de contenedores y ajustamos la variable numContenedores
  // para ver cuantos realmente consiguen entrar.

  // esta variable sera la que almacene el total de contenedores que finalmente se
  // almacenen en el buque.
  int total_contenedores = numContenedores;

  clock_t tantes, tdespues;
  tantes = clock();
  // CRITERIO 1
  SolucionCaminoContenedores(contenedores, camino_buque, pesoMaxBuque, total_contenedores, 1);
  tdespues = clock();

  //MOSTRAMOS SALIDA
  cout << endl<< "Secuencia de contenedores que maximizan el numero total cargados:" << endl << endl;
  for(int i=0; i<total_contenedores; i++){
    int indice_cont = camino_buque[i];
    cout << "Contenedor [ " << indice_cont << " ]" << " = " << contenedores[indice_cont] << endl;
  }
  cout << "-Total contenedores cargados: " << total_contenedores << endl;
  cout << "-Tiempo: " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;

  

  // Calculamos el maximo numero de contenedores y ajustamos la variable total_contenedores
  // para ver cuantos realmente consiguen entrar.

  // Reseteamos el valor del numero total de contenedores inicial
  total_contenedores = numContenedores;

  tantes = clock();
  //CRITERIO 2
  SolucionCaminoContenedores(contenedores, camino_buque, pesoMaxBuque, total_contenedores, 2);
  tdespues = clock();


  //MOSTRAMOS SALIDA
  cout << endl<< "Secuencia de contenedores que maximizan el tonelaje total cargado:" << endl << endl;
  for(int i=0; i<total_contenedores; i++){
    int indice_cont = camino_buque[i];
    cout << "Contenedor [ " << indice_cont << " ]" << " = " << contenedores[indice_cont] << endl;
  }
  cout << "-Total contenedores cargados: " << total_contenedores << endl;
  cout << "-Tiempo: " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;


  // Liberamos la memoria asignada a cada vector.
  delete []contenedores;
  delete []camino_buque;
}