#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <chrono>
#include <limits>
#include <cstdlib>
#include <vector>
#include <queue>
#include <map>


using namespace std;

const int TAM = 100;
int costes[TAM][TAM];


struct Ciudad{
  int numero;
  double x;
  double y;
};

vector<Ciudad> leerFichero(string fichero){
  int num_ciudades;
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

  vector<Ciudad> ciudades(num_ciudades);

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

// Calcula y devuelve la distancia entre dos ciudades
int distanciaEntreCiudades(Ciudad a, Ciudad b){
  return sqrt(pow((b.x-a.x),2)+pow((b.y-a.y),2));
}

vector<Ciudad> generaCiudades(int num_ciudades){
  vector<Ciudad> datos;
  Ciudad nueva;
  srandom(time(0));
  for(int i=0; i<num_ciudades; i++){
    nueva.numero = i;
    nueva.x = rand() % 100;
    nueva.y = rand() % 100;
    datos.push_back(nueva);
  }
  return datos;
}

void inicializaMatrizCostes(vector<Ciudad> &datos){
  for(int i=0; i<datos.size(); i++){
    for(int j=0; j<datos.size(); j++){
		costes[i][j] = distanciaEntreCiudades(datos[i], datos[j]);
    }
  }
}

int buscaMinimo(vector<int> v){
	int posMinimo = 0;

	for(int i = 0; i < v.size(); ++i){
		if(v[i] < v[posMinimo])
			posMinimo = i;
	}

	return posMinimo;
}

//caminoResultado es el camino a devolver
int caminoOptimo(int nodo, vector<int> S, vector<int> &caminoResultado){

	// Nodo inicial y el final seran los del nodo de partida, cerrando el camino
	// Si S está vacío, significa que es el último nodo del camino, así que el coste será
	// desde ese camino hasta el nodo inicial, que será el 0
	if(S.size() == 0){
		caminoResultado.push_back(nodo);
		return costes[nodo][0];
	}

	int minCoste = numeric_limits<int>::max();
	int minCiudad = -1;
	vector<int> minCamino;
	int costeActual = -1;

	// Estudiamos todo el conjunto de ciudades candidatas restantes (S)
	for(int j = 0; j < S.size(); ++j){

		vector<int> SAux(S.size());

		// Copiamos los elementos en SAux y se llama a la función recursivamente pero con distinto conjunto de ciudades candidatas
		for(int k = 0; k < SAux.size(); ++k){
			SAux[k] = S[k];
		}

		// se elimina el nodo en cuestión del nuevo subconjunto
		SAux.erase(SAux.begin() + j);

		costeActual = costes[nodo][S[j]] + caminoOptimo(S[j], SAux, caminoResultado);
		if(costeActual < minCoste){
			minCoste = costeActual;
			minCiudad = S[j];
			minCamino = caminoResultado;
		}
		caminoResultado.clear();
	}

	caminoResultado = minCamino;
	caminoResultado.push_back(nodo);

	//for(int i=0; i<caminoResultado.size(); i++){
	//	cout << caminoResultado[i] << endl;
	//}
	return minCoste;
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
	vector<int> camino;

	inicializaMatrizCostes(ciudades);

	cout << "\n---------------- COSTES ----------------\n\n";

	for(int i = 0; i < ciudades.size(); ++i){
		for(int j = 0; j < ciudades.size(); ++j){
			cout << costes[i][j] << "\t";
		}
		cout << "\n";
	}

	cout << "\n--------------------------------\n\n";

	// S contiene índices de ciudades candidatas tendremos todas las ciudades excepto la inicial
	// por ello el -1
	vector<int> S(ciudades.size()-1);
	// asignamos los indices de todas excepto la primera
	for(int i=0;i<ciudades.size()-1;i++)
		S[i] = i+1; //i es la ciudad inicial, i+1 es la ciudad que estamos tratanto y no la cogemos,


	clock_t tantes, tdespues;
	//Ahora calculamos y mostramos las soluciones y su coste total.
	tantes = clock();

	int coste_min = caminoOptimo(0,S,camino); //parte de la ciudad 0
	tdespues = clock();


	cout << ciudades.size() << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;
	cout << "Camino" << endl << "0";
	for(int i=0; i<camino.size(); i++){
		cout << " --> " << camino[i];
	}
  cout << "Coste Minimo: " << coste_min;
	cout << "\nFIN" << endl;

}
