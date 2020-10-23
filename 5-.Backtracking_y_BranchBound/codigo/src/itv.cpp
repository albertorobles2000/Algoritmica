#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

struct Coche{
	int numero;
	int tiempoInspeccion;
};

struct Cola{
	int numCoches;
	int tiempoAcumulado;
};

int getTiempoMaximo(vector<Cola> colas){
	int maximo = 0;
	for(int i = 0; i < colas.size(); ++i){
		if(colas[i].tiempoAcumulado > maximo)
			maximo = colas[i].tiempoAcumulado;
	}

	return maximo;
};


struct Nodo{
	int indice;
	vector<vector<int>> colas;
	vector<Cola> IndicesColas;
	int tiempoGlobal;

	void AniadeCocheACola(int coche, int cola, int tiempoInspeccion){
		colas[IndicesColas[cola].numCoches][cola] = coche;
		IndicesColas[cola].numCoches=IndicesColas[cola].numCoches+1;
		IndicesColas[cola].tiempoAcumulado += tiempoInspeccion;
		tiempoGlobal = getTiempoMaximo(IndicesColas);
		
	}
};


void ImprimeSolucion(Nodo solucion){
    for(int i=0; i<solucion.IndicesColas.size() ; i++){
        cout << "\nCola numero [" << i << "]" << endl;
        cout << "Hay *" << solucion.IndicesColas[i].numCoches << " coches en esa cola" << "\n";
        cout << "Coche: ";
		for(int j=0; j<solucion.IndicesColas[i].numCoches; ++j){
            cout << solucion.colas[j][i] << " ";
        }
        cout << endl;
    }	
};


void vueltaAtras(vector<Coche> &coches, Nodo &padre, int & cotaSuperior, Nodo & solucion){
	
	if(padre.indice >= (int) coches.size()-1){
		if(cotaSuperior > padre.tiempoGlobal){
			cotaSuperior = padre.tiempoGlobal;
			//Guardamos en nodo solucion
			solucion = padre;
		}
		return;
	}

	if(padre.indice == 0){	// Si es el primer coche, lo metemos en la primera cola (eso siempre es correcto)
		padre.AniadeCocheACola(padre.indice,0, coches[padre.indice].tiempoInspeccion);
	}
	

	Nodo hijo;
	hijo.indice = padre.indice + 1;
	for(int i=0; i<padre.IndicesColas.size(); ++i){ //Recorremos las colas
		hijo.IndicesColas = padre.IndicesColas;
		hijo.colas = padre.colas;
		//Añadimos al hijo al final de la cola i 

		hijo.AniadeCocheACola(hijo.indice,i, coches[hijo.indice].tiempoInspeccion);
			
		//Comprobamos que sea menor que la cota superior
		if(cotaSuperior > hijo.tiempoGlobal){
			vueltaAtras(coches, hijo, cotaSuperior, solucion);	
		}
		/*else{
			Poda
		}*/

	}
};

vector<Coche> leerFichero(string fichero, int & num_lineas_itv, int & num_coches){
  vector<Coche> vector_coches;

  ifstream fi(fichero);
  if (!fi){
     cout << "No puedo abrir " << fichero << endl;
     exit(1);
  }

	// linea cabezera del fichero
	string linea;
  	getline(fi,linea);
	//fi >> linea; // leemos cabezera
	cout << "Imprimo cabezera: " << linea << endl;
	
	fi >> num_lineas_itv; // num lineas de itv
	fi >> num_coches; // num de coches

  // Inicializamos los coches y los metemos en el vector de coches
	Coche nuevo;
	for(int i = 0; i < num_coches; i++){
		nuevo.numero = i;
		// leemos los tiempos de inspeccion de cada coche
		fi >> nuevo.tiempoInspeccion;
		// insertamos ese coche en el vector de coches
		vector_coches.push_back(nuevo);
	}
	cout << "Num lineas inspeccion: " << num_lineas_itv << endl;
	cout << "Num coches: " << num_coches << endl;

  return vector_coches;
};


int main(int argc, char **argv){
	vector<Coche> VecCoches;
	int num_Estaciones_Inspeccion;
	int num_coches;

	if(argc < 2){
		cerr << "Numero de argumentos invalido. Falta fichero de datos.\n";
		exit(-1);
	}
	if(argc==2){
		string fichero = argv[1];
		VecCoches = leerFichero(fichero, num_Estaciones_Inspeccion, num_coches);
		cout << endl << "Valores de tiempo: " << endl;
		for(int i=0; i<VecCoches.size(); i++){
			cout << VecCoches[i].numero << "-->" << VecCoches[i].tiempoInspeccion << endl;
		}		
	}
	if(argc==3){
		num_Estaciones_Inspeccion = atoi(argv[1]);
		num_coches = atoi(argv[2]);

		// Inicializamos los coches y los metemos en el vector de coches
		Coche nuevo;
		for(int i = 0; i < num_coches; i++){
			nuevo.numero = i;
			nuevo.tiempoInspeccion = rand() % 100;
			VecCoches.push_back(nuevo);
		}
		
		cout << "Valores de tiempo: " << endl;
		for(int i=0; i<VecCoches.size(); i++){
			cout << "Coche [" << i << "] tarda: ";
			cout << VecCoches[i].tiempoInspeccion << endl;
		}	
	}
	
	// creamos la matriz de las colas, con el numero de coches como filas
	// y el numero de estaciones como columnas
	vector<vector<int>> colas(num_coches, vector<int>(num_Estaciones_Inspeccion));
	for(int i=0; i<num_coches; i++){
		for(int j=0; j<num_Estaciones_Inspeccion; j++){
			colas[i][j] = -1;
		}
	}
	vector<Cola> IndicesColas;

	// creamos las colasITV y las metemos en el vector de colas (IndicesColas)
	Cola nuevaCola;
	for(int i=0; i<num_Estaciones_Inspeccion; ++i){
		nuevaCola.numCoches = 0;
		nuevaCola.tiempoAcumulado = 0;
		IndicesColas.push_back(nuevaCola);
	}

	Nodo primero;
	primero.indice = 0;
	primero.IndicesColas = IndicesColas;
	primero.colas = colas;
	primero.tiempoGlobal = 0;

	Nodo solucion;

	int cotaSuperior = numeric_limits<int>::max();
	
	clock_t tantes, tdespues;

	tantes = clock();
  	vueltaAtras(VecCoches,primero,cotaSuperior,solucion);
	tdespues = clock();

	cout << endl << "***Reparto *FINAL* de coches en las colas: ***" << endl;

	ImprimeSolucion(solucion);	
	
	cout << endl << "*Tiempo total de trabajo de la estacion: " << solucion.tiempoGlobal << endl;

	cout <<"\n*Tiempo de ejecucion del programa: " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;

	return (0);
}
