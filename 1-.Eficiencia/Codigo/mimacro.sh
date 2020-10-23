#!/bin/bash
I=1
N=5
F=100

for ((P=I;P<=F;P=P+N))
do
./burbuja $P >> salida/salida_burbuja.dat
done

echo "*******************************************************"
echo "*BURBUJA HECHO**"
echo "*******************************************************"

#for ((P=I;P<=F;P=P+N))
#do
#./floyd $P >> salida/salida_floyd.dat
#done

echo "*******************************************************"
echo "*FLOYD HECHO**"
echo "*******************************************************"

#for ((P=I;P<=F;P=P+N))
#do
#./hanoi $P >> salida/salida_hanoi.dat
#done

echo "*******************************************************"
echo "*HANOI HECHO**"
echo "*******************************************************"

for ((P=I;P<=F;P=P+N))
do
./heapsort $P >> salida/salida_heapsort.dat
done

echo "*******************************************************"
echo "*HEAPSORT HECHO**"
echo "*******************************************************"

for ((P=I;P<=F;P=P+N))
do
./insercion $P >> salida/salida_insercion.dat
done

echo "*******************************************************"
echo "*INSERCION HECHO**"
echo "*******************************************************"

for ((P=I;P<=F;P=P+N))
do
./mergesort $P >> salida/salida_mergesort.dat
done

echo "*******************************************************"
echo "*MERGESORT HECHO**"
echo "*******************************************************"

for ((P=I;P<=F;P=P+N))
do
./quicksort $P >> salida/salida_quicksort.dat
done

echo "*******************************************************"
echo "*QUICKSORT HECHO**"
echo "*******************************************************"

for ((P=I;P<=F;P=P+N))
do
./seleccion $P >> salida/salida_seleccion.dat
done

echo "*******************************************************"
echo "*SELECCION HECHO**"
echo "*******************************************************"
