#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <limits>
#include <list>
#include <queue>
#include <utility>
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include <limits>
#include <cstdlib>

using namespace std;

//variables globales para analizar el nivel de complejidad
int contador_poda = 0, nodos_extraidos = 0, tam_max_cola = 1;

// Definimos una ciudad, su numero y coordenadas
struct Ciudad{
  int numero;
  double x;
  double y;
};

void ImprimeMatriz(const vector<vector<int>> & original){
    // Como son cuadradas, tendremos el tamanio fijo con size
    int tamanio = original.size();
    for(int i=0; i<tamanio; i++){
        for(int j=0; j<tamanio; j++){
           if( original[i][j] == numeric_limits<int>::max() )
               cout << "inf";
           else
               cout << original[i][j];
               
           cout << " \t";
        }
        cout << endl;
    }
}

int reduceMatriz(const vector<vector<int>> & original, vector<vector<int>> & reducida){
  int minimo;
  int coste = 0;
  
  reducida.resize(original.size());
  for(int i = 0; i < original.size(); ++i){
    reducida[i].resize(original.size());  // La matriz es cuadrada
  }

  for(int i=0; i<original.size(); ++i){
    minimo = numeric_limits<int>::max();

    for(int j=0; j<original.size(); ++j){
      if(original[i][j]<minimo && i!=j){
        minimo = original[i][j];
      }
    }

    for(int j=0; j<original.size();++j){
      if(j!=i)
        reducida[i][j] = original[i][j]-minimo;
      else 
        reducida[i][j] = original[i][j];
    }

    coste += minimo;
  }
  
  for(int i=0; i<original.size(); ++i){
    minimo = numeric_limits<int>::max();

    for(int j=0; j<original.size(); ++j){
      if(reducida[j][i]<minimo && i!=j){
        minimo = reducida[j][i];
      }
    }

    for(int j=0; j<original.size();++j){
      if(j!=i)
        reducida[j][i] = reducida[j][i]-minimo;
      else 
        reducida[j][i] = reducida[j][i];
    }

    coste += minimo;
  }
  return coste;
}


int reduceMatriz(const vector<vector<int>> & original, vector<vector<int>> & reducida, int orig, int dest){
  int minimo;
  int coste = 0;

  reducida.resize(original.size());
  for(int i = 0; i < original.size(); ++i){
    reducida[i].resize(original.size());  // La matriz es cuadrada
  }
  
  for(int i=0; i<reducida.size(); ++i){
   for(int j=0; j<reducida.size(); ++j){
    if(i==orig || j==dest)
      reducida[i][j] =  numeric_limits<int>::max();
    else
     reducida[i][j] = original[i][j]; 
   } 
  }
  reducida[dest][orig] =  numeric_limits<int>::max();


  for(int i=0; i<reducida.size(); ++i){
    minimo = numeric_limits<int>::max();

    for(int j=0; j<reducida.size(); ++j){
      if(reducida[i][j]<minimo && i!=j){
        minimo = reducida[i][j];
      }
    }

    for(int j=0; j<reducida.size();++j){
      if(j!=i)
        reducida[i][j] = reducida[i][j] - minimo;
      else 
        reducida[i][j] = numeric_limits<int>::max();
    }

    coste += minimo;
  }
  
  for(int i=0; i<reducida.size(); ++i){
    minimo = numeric_limits<int>::max();

    for(int j=0; j<reducida.size(); ++j){
      if(reducida[j][i]<minimo && i!=j){
        minimo = reducida[j][i];
      }
    }

    for(int j=0; j<reducida.size();++j){
      if(j!=i)
        reducida[j][i] = reducida[j][i]-minimo;
      else 
        reducida[j][i] = reducida[j][i];
    }

    coste += minimo;
  }
  return coste;
}


struct Nodo{
  vector<vector<int>> matriz;
  int coste;
  int ciudad; // Almacenará la última ciudad del camino
  list<int> camino;
  int costeReduccion;
};

// Elige los nodos de menor coste
struct ComparaCostes{
	bool operator()(const Nodo &a, const Nodo &b) const{
		return a.coste > b.coste;
	}
};

// Calcula y devuelve la distancia entre dos ciudades
int distanciaEntreCiudades(Ciudad a, Ciudad b){
  return sqrt(pow((b.x-a.x),2)+pow((b.y-a.y),2));
}

// Generador de ciudades aleatorias de coordenadas aleatorias
// del 0 al 300
vector<Ciudad> generaCiudades(int num_ciudades){
  vector<Ciudad> datos;
  Ciudad nueva;
  srandom(time(0));
  for(int i=0; i<num_ciudades; i++){
    nueva.numero = i;
    nueva.x = rand() % 300;
    nueva.y = rand() % 300;
    datos.push_back(nueva);
  }
  return datos;
}

// Matriz de Adyacencia con los costes de viajar de una ciudad a otra

vector<vector<int>> CreaMatrizAdyacencia(vector<Ciudad> &datos){
  vector<vector<int>> matrizAdyacencia(datos.size(), std::vector<int>(datos.size()));
  for(int i=0; i<datos.size(); i++){
    for(int j=0; j<datos.size(); j++){
      if(i!=j)
        matrizAdyacencia[i][j] = distanciaEntreCiudades(datos[i],datos[j]);
      else
        matrizAdyacencia[i][j] = numeric_limits<int>::max();
      
    }
  }
  return matrizAdyacencia;
}

vector<Ciudad> leerFichero(string fichero){
  int num_ciudades;
  ifstream fi(fichero);
  if (!fi){
     cout << "No puedo abrir " << fichero << endl;
     exit(1);
  }
  //cout << "fichero: " << fichero << endl;
  string linea;
  fi >> linea; // leo lo de NAME:
  getline(fi,linea);  //leo el nombre
  cout << endl << "Nombre de fichero: " << linea << endl;

  getline(fi,linea);  //comment
  getline(fi,linea);  // type

  getline(fi, linea, ' '); // dimension hasta el espacio
  fi >> num_ciudades; //cogemos las ciudades

  getline(fi,linea);  //edge
  getline(fi,linea);  // node coord

  getline(fi,linea);  // next line

  vector<Ciudad> ciudades(num_ciudades);

  Ciudad nueva;

  cout << endl;
  cout << "Indice ciudad " << "\t";
  cout << "Coordenadas (x,y)" << endl;
  for(int i=0; i<num_ciudades; i++){
    //recogemos datos
    fi >> nueva.numero;
    fi >> nueva.x;
    fi >> nueva.y;

    //imprimimos los datos
    // le restamos 1 porque empezamos en la ciudad 0
    cout << "\t" << nueva.numero -1 << "\t";
    cout << nueva.x << "\t";
    cout << nueva.y << endl;

    // la insertamos
    ciudades[i] = nueva;
  }
  cout << endl << "Numero de ciudades: " << num_ciudades << endl;
  return ciudades;

}


// Calculador del camino Optimo

list<int> caminoOptimo(int ciudadInicial, vector<vector<int>> & distancias){
  // cola con prioridad de las ciudades, las de menor coste seran prioritarias
  priority_queue<Nodo, vector<Nodo>, ComparaCostes> colaPrio;
  
  // limite superior 
  int upperbound = numeric_limits<int>::max();
  Nodo actual;

  actual.ciudad=ciudadInicial;
  actual.coste= reduceMatriz(distancias, actual.matriz);
  actual.costeReduccion = actual.coste;
  actual.camino = list<int>(); 
  actual.camino.push_back(ciudadInicial); 
  
  colaPrio.push(actual);
  Nodo menorCoste;
  
  while(!colaPrio.empty()){
    
    colaPrio.pop();
    if(upperbound > actual.coste){  // Sólo estudiaremos los nodos que estén por debajo de la cota superior, inicialmente infinito
      // acutalizamos el valor de la variable que controla el numero de nodos extraidos
      nodos_extraidos++;
      if(actual.camino.size() != distancias.size()){
        
        Nodo hijo;
        
        // imprimir la reducida? conviene
        for(int i = 0; i < distancias.size(); ++i){
          // Solo exploramos las ciudades no visitadas
          if( find(actual.camino.begin(), actual.camino.end(), i) == actual.camino.end() && i != actual.ciudad){ 
            vector<vector<int>> reducida(actual.matriz.size(), std::vector<int>(actual.matriz.size()));
            int CosteMin = reduceMatriz(actual.matriz,reducida,actual.ciudad,i);
            hijo.ciudad = i; 
            hijo.costeReduccion = CosteMin;
                        //^r          //r                    //C(x,y)
            hijo.coste = CosteMin + actual.coste + actual.matriz[actual.ciudad][hijo.ciudad] ;
            hijo.matriz = reducida;
            hijo.camino = actual.camino;
            hijo.camino.push_back(hijo.ciudad);
            colaPrio.push(hijo);
            // actualizamos el tamanio maximo de la cola con prioridad de nodos vivos 
            if(colaPrio.size() > tam_max_cola){
                tam_max_cola = colaPrio.size();
            }
            
          }
        }
      }
      else
      {
        upperbound = actual.coste;
        menorCoste.coste = actual.coste;
        menorCoste.ciudad = actual.ciudad;
        menorCoste.camino = actual.camino;
      }
    }
    else{
        // actualizamos el valor de la variable que extrae el numero de podas realizadas
        ++contador_poda;
        //cout << "Podo" << endl;
    }
    if(!colaPrio.empty())    
      actual = colaPrio.top();
  }
    // mostramos los valores de las variables globales
    // nivel de complejidad
    cout << "Num Podas: " << contador_poda << endl;
    cout << "Num nodos extraidos: " << nodos_extraidos << endl;
    cout << "Tam maximo de cola con prioridad de nodos vivos: " << nodos_extraidos << endl;
  
  return menorCoste.camino;
}



int main(int argc, char ** argv){


  vector<Ciudad> ciudades;
  if(argc < 2){
    cerr << "Numero de argumentos invalido. Falta fichero de datos.\n";
    exit(-1);
  }
  if(argc==2){
    string fichero = argv[1];
    ciudades = leerFichero(fichero);
  }
  else{
      int elementos = atoi(argv[2]);
      ciudades = generaCiudades(elementos);
  }
	
	vector<vector<int>> mAdyacencia = CreaMatrizAdyacencia(ciudades);
  cout << endl << "\t*Matriz de Costes*" << endl << endl;
  ImprimeMatriz(mAdyacencia);
  cout << endl << endl;

	clock_t tantes, tdespues;
	//Ahora calculamos y mostramos las soluciones y su coste total.
	tantes = clock();
  
	list<int> camino = caminoOptimo(0,mAdyacencia); //parte de la ciudad 0

	tdespues = clock();

	cout << "\n\n";
  int coste = 0;

  // recorrer la lista del camino
  // para calcular el coste final
  list<int>::iterator it, it2;
  it = camino.begin();
  it2 = it;
  for(++it2; it2 != camino.end(); ++it, ++it2){
    coste += mAdyacencia[*it][*it2];
  }

  coste += mAdyacencia[*it][*(camino.begin())];
  cout << "Camino FINAL" << endl;
  for(it = camino.begin(); it != camino.end(); ++it){
    cout << *it << " --> ";
  }
  
  cout << *(camino.begin()) << endl;
  cout << endl << endl << "Ciudades en total = " << ciudades.size() << endl;
  cout << "Coste FINAL = " << coste << endl;
  cout << "Tiempo de ejecucion: " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl << endl;

}
