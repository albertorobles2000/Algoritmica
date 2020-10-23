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
#include <set>

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


int distanciaMemoria(map<pair<int,set<int>>,int> & memoria, int num, set<int> & conjunto){
  map<pair<int,set<int>>,int>::iterator it;
  pair<int,set<int>> pareja(num,conjunto);
  it = memoria.find(pareja);
  if(it == memoria.end())
    return -1;
  else
    return it->second;
}

void anideAMemoria(map<pair<int,set<int>>,int> & memoria, int num, set<int> conjunto, int distancia){
  pair<int,set<int>> pareja(num,conjunto);
  pair<pair<int,set<int>>,int> nueva (pareja, distancia);
  memoria.insert(nueva);
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


//caminoResultado es el camino a devolver
int caminoOptimo(int nodo, set<int> & S, map<pair<int,set<int>>,int> & memoria){

  if(S.size() <= 1){
    int ciudadRestante = *(S.begin());
    return costes[nodo][ciudadRestante]+costes[ciudadRestante][0];
  }

	int minCoste;
	int minCiudad = -1;
	int costeActual = -1;

  minCoste = distanciaMemoria(memoria,nodo,S);
  if(minCoste == -1){
    minCoste = numeric_limits<int>::max();
  	// Estudiamos todo el conjunto de ciudades candidatas restantes (S)
    set<int> SAux = S;
    set<int>::iterator it;;
  	for(it = S.begin(); it != S.end(); ++it){

  			set<int>::iterator it2 = SAux.find(*it);
  			SAux.erase(it2);
  			// llamamos a la funcion recursivamente

          costeActual = costes[nodo][*it] + caminoOptimo(*it, SAux,memoria);

  			if(costeActual < minCoste){
  				minCoste = costeActual;
  				minCiudad = *it;
  			}
        SAux.insert(*it);
  	}

    anideAMemoria(memoria,nodo,S,minCoste);
  }

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

  map<pair<int,set<int>>,int> memoria;
  /**********************************************************/
	inicializaMatrizCostes(ciudades);

	cout << "\n---------------- COSTES ----------------\n\n";

	for(int i = 0; i < ciudades.size(); ++i){
		for(int j = 0; j < ciudades.size(); ++j){
			cout << costes[i][j] << "\t";
		}
		cout << "\n";
	}

	cout << "\n--------------------------------\n\n";
  /**********************************************************/
	// S contiene índices de ciudades candidatas tendremos todas las ciudades excepto la inicial
	// por ello el -1
	set<int> S;
	// asignamos los indices de todas excepto la primera
	for(int i=0;i<ciudades.size()-1;i++)
		S.insert(i+1); //i es la ciudad inicial, i+1 es la ciudad que estamos tratanto y no la cogemos,


	clock_t tantes, tdespues;
	//Ahora calculamos y mostramos las soluciones y su coste total.
	tantes = clock();

	int coste_min = caminoOptimo(0,S,memoria); //parte de la ciudad 0
	tdespues = clock();


	cout << ciudades.size() << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;
  cout << "Coste Minimo: " << coste_min;

}
