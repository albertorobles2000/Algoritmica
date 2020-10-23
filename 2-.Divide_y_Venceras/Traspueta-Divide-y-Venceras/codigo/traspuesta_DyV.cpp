/*
g++ -O2 -o traspuesta_DyV traspuesta_DyV.cpp
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <cmath>

using namespace std;

void muestra_matriz(int ** M,int filas, int columnas){
	for (int i = 0; i < filas; i++)
	{
		cout << endl;
		for(int j=0; j < columnas; j++)
			cout << setw(10) << M[i][j] << " ";
	}
}

void traspuesta (int ** M,int primera_fil , int ultima_fil, int primera_col, int ultima_col) {

 	if (primera_fil < ultima_fil) { //Si las submatrices no son de 1x1
    //Dividimos el problema en 4 submatrices
		int fila_intermedia = (primera_fil+ultima_fil)/2;
		int columna_intermedia = (primera_col+ultima_col)/2;

    //Resolvemos cada una de las partes por separado
		traspuesta (M, primera_fil, fila_intermedia, primera_col, columna_intermedia);
		traspuesta (M, primera_fil, fila_intermedia, columna_intermedia+1, ultima_col);
		traspuesta (M, fila_intermedia+1, ultima_fil, primera_col, columna_intermedia);
		traspuesta (M, fila_intermedia+1, ultima_fil, columna_intermedia+1, ultima_col);


    //juntamos soluciones
		int aux;
		int dim_intercambiados = ultima_fil-fila_intermedia;
		for (int i=0; i<dim_intercambiados; i++)
 			for (int j=0; j<dim_intercambiados; j++) {
 				aux = M[fila_intermedia+1+i][primera_col+j];
				M[fila_intermedia+1+i][primera_col+j] = M[primera_fil+i][columna_intermedia+1+j];
 				M[primera_fil+i][columna_intermedia+1+j] = aux;
 			}

	};
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
		int z=1;
		for (int i=0; i < n; i++)
		{
		 	for(int j=0; j < n; j++){
		   	//M[i][j] = random();
				M[i][j] = z;
				z++;
			}
		}

		cout << "\n\nOriginal: " ;
    muestra_matriz(M,n,n);

    tantes = clock();
		traspuesta (M, 0, n-1, 0, n-1);
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
