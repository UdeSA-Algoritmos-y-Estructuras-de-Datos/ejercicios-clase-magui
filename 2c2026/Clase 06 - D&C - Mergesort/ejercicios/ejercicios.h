#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <vector>

// ============================================================================
// Tut 06 - Divide & Conquer - Mergesort
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
// ============================================================================

// ============================================================================
// MERGE SORT
// ============================================================================

// Ejercicio 1 — Merge
// Combina dos sub-rangos YA ORDENADOS de v en uno solo ordenado, usando un
// vector auxiliar: [izq, medio] y [medio+1, der].
// Precondición: v[izq..medio] y v[medio+1..der] están cada uno ordenados.
// Complejidad: O(n), con n = der - izq + 1.
void merge(std::vector<int>& v, int izq, int medio, int der);

// Ejercicio 2 — Merge Sort
// Ordena v[izq..der] de menor a mayor usando Divide & Conquer: divide el
// rango al medio, ordena recursivamente cada mitad y las combina con merge.
// Para ordenar todo el vector, llamar con izq=0 y der=v.size()-1.
// Caso base: si izq >= der, el rango tiene 0 o 1 elementos (ya ordenado).
// Complejidad: T(n) = 2T(n/2) + O(n) = O(n log n), siempre (mejor, promedio
// y peor caso).
void mergeSort(std::vector<int>& v, int izq, int der);

// Ejercicio 3 — Combinar dos vectores ordenados
// Dados dos vectores YA ORDENADOS (de menor a mayor), devuelve un nuevo
// vector con todos sus elementos combinados y ordenados.
// A diferencia de "merge", acá los dos vectores son independientes (no
// sub-rangos de un mismo vector) y se devuelve un vector nuevo.
// Complejidad: O(n + m), con n y m los tamaños de a y b.
std::vector<int> mergeVectoresOrdenados(const std::vector<int>& a, const std::vector<int>& b);

// ============================================================================
// QUICK SORT
// ============================================================================

// Ejercicio 4 — Partición (esquema de Lomuto)
// Elige v[der] como pivot y reordena v[izq..der] de forma que todos los
// elementos menores al pivot queden a su izquierda y todos los mayores (o
// iguales) a su derecha. Devuelve la posición final del pivot.
// Complejidad: O(n), con n = der - izq + 1.
int partition(std::vector<int>& v, int izq, int der);

// Ejercicio 5 — Quick Sort
// Ordena v[izq..der] de menor a mayor usando Divide & Conquer: particiona el
// rango con partition() y ordena recursivamente lo que quedó a cada lado
// del pivot (el pivot ya queda en su posición final, no hace falta combinar).
// Caso base: si izq >= der, el rango tiene 0 o 1 elementos (ya ordenado).
// Complejidad: O(n log n) en el mejor/promedio caso, O(n²) en el peor caso
// (por ejemplo, si el vector ya está ordenado).
void quickSort(std::vector<int>& v, int izq, int der);

// ============================================================================
// UNA APLICACIÓN DE MERGE SORT
// ============================================================================

// Ejercicio 6 — Contar inversiones
// Una "inversión" es un par de índices (i, j) con i < j pero v[i] > v[j]
// (es decir, están "desordenados" entre sí). Devuelve la cantidad total de
// inversiones del vector, SIN modificar el vector original.
// Pista: adaptá merge sort. Al hacer merge, cada vez que tomás un elemento
// de la mitad derecha "antes de tiempo" (todavía quedan elementos en la
// mitad izquierda), esos elementos que quedan forman inversiones con él.
// Complejidad: O(n log n) (vs. O(n²) con fuerza bruta).
int contarInversiones(const std::vector<int>& v);

// ============================================================================
// EJERCICIOS PROPUESTOS
// ============================================================================

// Propuesto 1 — Quick Sort con mediana de tres
// Igual que quickSort, pero eligiendo como pivot la MEDIANA entre v[izq],
// v[izq + (der-izq)/2] y v[der] (en vez de usar siempre v[der]). Esto evita
// el peor caso O(n²) en vectores que ya están ordenados o casi ordenados.
// Pista: antes de particionar, intercambiá la mediana de los tres con v[der]
// y después particioná igual que en el Ejercicio 4/5.
// Complejidad: O(n log n) en el caso promedio; sigue siendo O(n²) en el
// peor caso teórico, pero mucho más difícil de provocar en la práctica.
void quickSortMediana3(std::vector<int>& v, int izq, int der);

// Propuesto 2 — k-ésimo menor (quickselect)
// Devuelve el valor del k-ésimo elemento más chico del vector (k=0 es el
// mínimo, k=1 el segundo más chico, etc.), SIN ordenar todo el vector.
// Pista: es una variante de quickSort que, después de particionar, sólo
// sigue recursivamente por el lado donde sabe que está el k-ésimo elemento
// (no hace falta bajar por los dos lados como en quickSort).
// Precondición: 0 <= k < v.size().
// Complejidad: O(n) en el caso promedio, O(n²) en el peor caso.
int kEsimoMenor(std::vector<int>& v, int k);

// Propuesto 3 — Suma máxima de subarreglo (Divide & Conquer)
// Devuelve la máxima suma posible de un subarreglo CONTIGUO no vacío de v.
// Resolverlo con Divide & Conquer: dividir el vector al medio y considerar
// tres casos: la respuesta está completamente en la mitad izquierda,
// completamente en la mitad derecha, o "cruza" el medio (en ese caso hay
// que extender la suma hacia ambos lados a partir del centro).
// Precondición: v no está vacío.
// Complejidad: T(n) = 2T(n/2) + O(n) = O(n log n).
long long sumaMaximaSubarreglo(const std::vector<int>& v);

// Propuesto 4 — Potencia rápida (Divide & Conquer)
// Calcula base^exponente usando Divide & Conquer en vez de multiplicar
// "exponente" veces: base^n = (base^(n/2))², y si n es impar hay que
// multiplicar una vez más por base.
// Precondición: exponente >= 0.
// Complejidad: T(n) = T(n/2) + O(1) = O(log n) (comparar con el ejemplo de
// búsqueda binaria del teorema maestro en los apuntes).
long long potencia(long long base, int exponente);

#endif
