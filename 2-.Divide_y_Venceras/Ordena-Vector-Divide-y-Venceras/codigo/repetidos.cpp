/*
g++ -O2 -o repetidos repetidos.cpp
*/

#include <iostream>
using namespace std;
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>

//Creación de numeros aleatorios
double uniforme()
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

void eliminaRepetidos(int * T, int & elementos){

  for(int i=0; i<elementos; i++){
    int j=i+1;
    //Recorremos todo el vector comparando el elemento T[i] con T[j]
    while(j<elementos){
      if(T[i] == T[j]){ //Si son iguales
        for(int k=j+1; k<elementos; k++){ //Eliminamos T[j]
          T[k-1] = T[k];
        }
        --elementos; //Decrementamos el numero de elementos de T
      }
      else //Si no son iguales incrementamos j
        j++;
    }
  }
}

int main(int argc, char * argv[])
{

  if (argc != 2)
    {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
    }

  int n = atoi(argv[1]);
  int elementos = n;

  int * T = new int[n];
  assert(T);

  srand(time(0));
  //para generar un vector que pueda tener elementos repetidos
  for (int j=0; j<n; j++) {
    double u=uniforme();
    int k=(int)(n*u);
    T[j]=k;
  }
/*
  for (int j=0; j<n; j++)
    cout << T[j] << " ";
*/
  clock_t tantes, tdespues;

  tantes = clock();//Tomamos el tiempo de inicio
  eliminaRepetidos(T,n);
  tdespues = clock();//Tomamos el tiempo de finalización


  cout << elementos << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;
/*
  cout << endl;
  for (int j=0; j<n; j++)
    cout << T[j] << " ";
  cout << endl;
*/
}
