/*
g++ -O2 -o traspuesta traspuesta.cpp
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <cmath>

using namespace std;


void traspuesta (int ** M,int filas, int columnas) {
	int aux;
 	for(int i=0; i<filas; i++){
 		for(int j=i; j<columnas; j++){
			aux=M[i][j];
			M[i][j] = M[j][i];
			M[j][i] = aux;
		}
 	}
}

void muestra_matriz(int ** M,int filas, int columnas){
	for (int i = 0; i < filas; i++)
	{
		cout << endl;
		for(int j=0; j < columnas; j++)
			cout << setw(10) << M[i][j] << " ";
	}
}

int main(int argc, char ** argv){

	if (argc != 2)
    {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
    }

	clock_t tantes, tdespues;
	int n = atoi(argv[1]);
	long elementos=pow(n,2);
	int ** M = new int*[n];
	for (int i = 0; i < n; i++) {
   	M[i] = new int[n];
	}
	assert(M);

		srandom(time(0));

		int z=0;
		for (int i=0; i < n; i++)
		{
		 	for(int j=0; j < n; j++){
		   	//M[i][j] = random();
				M[i][j] = z;
				z++;
			}
		}

		muestra_matriz(M,n,n);

		tantes = clock();
		traspuesta (M, n, n);
		tdespues = clock();

		cout << "\n\nTraspuesta: " ;
		muestra_matriz(M,n,n);

		cout << endl;
		cout << elementos << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;


		for (int i = 0; i < n; i++) {
	   	delete[] M[i];
		}
		delete[] M;
}
