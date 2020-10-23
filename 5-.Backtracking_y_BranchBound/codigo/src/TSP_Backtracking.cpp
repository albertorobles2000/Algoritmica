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
#include <stack>



using namespace std;

//variables globales para analizar el nivel de complejidad
int contador_poda = 0, nodos_extraidos = 0;


// Definimos una ciudad, su numero y coordenadas
struct Ciudad{
  int numero;
  double x;
  double y;
};

void ImprimeMatriz(const vector<vector<int>> & original){
    // como son cuadradas, tendremos el tamanio fijo con size
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

struct Nodo{
  int coste;
  int ciudad; // Almacenará la última ciudad del camino
  list<int> camino;
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
  string linea;
  fi >> linea; // leo lo de NAME:
  getline(fi,linea);  // leo el nombre
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
void caminoOptimo(Nodo & actual, vector<vector<int>> & distancias, int & upperbound, Nodo & solucion){

  if(distancias.size() == actual.camino.size()){
    int costeGlobal = actual.coste+distancias[actual.ciudad][0];
    if(upperbound >= costeGlobal){
      upperbound = actual.coste;
      solucion.camino = actual.camino;
      solucion.ciudad = actual.ciudad;
      solucion.coste = costeGlobal;
      
    }
    return;
  }

  Nodo hijo;
  for(int i=0; i<distancias.size(); i++){
    if(find(actual.camino.begin(), actual.camino.end(), i) == actual.camino.end() && i != actual.ciudad){
      hijo.ciudad = i;
      hijo.camino = actual.camino;
      hijo.camino.push_back(i);
      hijo.coste = actual.coste + distancias[actual.ciudad][hijo.ciudad];
  
      if(hijo.coste < upperbound){
        // acutalizamos el valor de la variable que controla el numero de nodos extraidos
        nodos_extraidos++;
        caminoOptimo(hijo, distancias, upperbound, solucion);
      }
      else{
        // actualizamos el valor de la variable que extrae el numero de podas realizadas
        ++contador_poda;
      }
    }
  }
}

list<int> caminoOptimo(int ciudadInicial, vector<vector<int>> & distancias){
  Nodo nuevo; 
  nuevo.ciudad=ciudadInicial; 
  nuevo.coste=0; 
  nuevo.camino.push_back(ciudadInicial);
  int upperbound = numeric_limits<int>::max();
  Nodo solucion;
  // Calculamos el caminoOptimo en si
  caminoOptimo(nuevo, distancias, upperbound, solucion);
  
  // mostramos los valores de las variables globales
  // nivel de complejidad
  cout << "Num Podas: " << contador_poda << endl;
  cout << "Num nodos extraidos: " << nodos_extraidos << endl;
  
  return solucion.camino;
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
  cout << endl << "Ciudades en total = " << ciudades.size() << endl;
  cout << "Coste FINAL = " << coste << endl;
  cout << "Tiempo de ejecucion: " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl << endl;

}