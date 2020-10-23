/*
g++ -O2 -o repetidos_DyV repetidos_DyV.cpp
*/

#include <iostream>
using namespace std;

//Mezclamos los dos vectores y eliminamos los elementos repetidos
void Mezcla (int * T1, int & elementosT1, int * T2, int & elementosT2, int & elementos_final)
{
  int posT1 = 0;
  int posT2 = 0;

  int elementosT = 0;
  int * T = new int[elementosT1+elementosT2]; //Vector auxiliar
  // Mientras los dos vectores tengan elementos que mezclar continuamos
  while ( posT1 < elementosT1 && posT2 <  elementosT2)
  {
    //Si el elemento esta repetido en ambos vectores copiamos uno de ellos en T
    //y avanzamos los indices de ambos vectores
    if ( T1[posT1] == T2[posT2] )
    {
      T[elementosT] = T1[posT1];
      ++posT1;
      ++posT2;
    }
    // Si el elemento de T1 es menor que el de T2 copiamos al vector T el elemento
    // de T1
    else if ( T1[posT1] < T2[posT2] ){
      T[elementosT] = T1[posT1];
      ++posT1;
    }
    // Si el elemento de T2 es menor que el de T1 copiamos al vector T el elemento
    // de T2
    else{
      T[elementosT] = T2[posT2];
      ++posT2;
    }
    ++elementosT;//Aumentamos el indice del vector T
  }

  // Si T1 tiene elementos aun los copiamos en T
  while ( posT1 < elementosT1 ){
    T[elementosT] = T1[posT1];
    ++posT1;
    ++elementosT;
  }

  // Si T2 tiene elementos aun los copiamos en T
  while ( posT2 < elementosT2 ){
    T[elementosT] = T2[posT2];
    ++posT2;
    ++elementosT;
  }
  // Copiamos T de vuelta en el vector originar apartir del puntero T1
  for ( int i = 0; i < elementosT; i++)
    T1[i]= T[i];

  //Actualizamos el numero de elementos de la parte del vector compuesta por el
  //vector T1 y T2
  elementos_final = elementosT;
  delete [] T;
}


void eliminaRepetidos ( int * T, int & elementos )
{
  if ( elementos > 1 ) //Si el subvector es de tamaño 1 no seguimos dividiendo
  {
    //Separamos el vector en dos partes
    int intermedio = elementos/2;
    int * T1 = T;
    int elementosT1 = intermedio;
    int * T2 = &T[intermedio];
    int elementosT2 = elementos - intermedio;

    //Resolvemos los subproblemas por separado
    eliminaRepetidos(T1, elementosT1);
    eliminaRepetidos(T2, elementosT2);

    //Juntamos la solución
    Mezcla( T1, elementosT1, T2, elementosT2, elementos);

  }
}
//Creación de numeros aleatorios
double uniforme()
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

int main (int argc, char ** argv)
{

  if (argc != 2)
    {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
    }
    int elementos = atoi(argv[1]);
    int n = elementos;
    int * T = new int[n];

  srand(time(0));
  //para generar un vector que pueda tener elementos repetidos
  for (int j=0; j<n; j++) {
    double u=uniforme();
    int k=(int)(n*u);
    T[j] = k;
  }
  cout << "Vector inicial: ";
  for (int j=0; j<n; j++)
    cout << T[j] << " ";

  clock_t tantes, tdespues;

  tantes = clock(); //Tomamos el tiempo de inicio
  eliminaRepetidos ( T,n );
  tdespues = clock();//Tomamos el tiempo de finalización

  cout << "\nVector final: ";
  for (int j=0; j<n; j++)
    cout << T[j] << " ";
  cout << endl;



  cout << elementos << "  " << (double)(tdespues - tantes) / CLOCKS_PER_SEC << endl;
}
