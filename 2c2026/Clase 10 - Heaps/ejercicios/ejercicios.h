#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <cstddef>
#include <vector>

// ============================================================================
// Tut 10 - Heaps
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
//
// En las Partes 1 y 2 implementás el heap "a mano" sobre un std::vector<int>
// (sin punteros: el árbol está implícito en las posiciones del vector). En la
// Parte 3 lo usás como cola de prioridad para resolver problemas: ahí podés
// usar tu MinHeap o std::priority_queue (ver el README).
// ============================================================================

// ============================================================================
// ESTRUCTURA DEL HEAP
// ============================================================================

// Un min-heap de enteros guardado en un vector, nivel por nivel y de
// izquierda a derecha, indexando desde 0:
//   - datos[0] es la raíz (el mínimo).
//   - los hijos de datos[i] son datos[2i + 1] y datos[2i + 2] (si existen).
//
// Propiedad de min-heap: cada elemento es MENOR O IGUAL que sus hijos.
// Un vector vacío o de un solo elemento es un heap válido.
//
// Los índices son size_t porque son posiciones de un vector: así no hace falta
// convertir cada vez que se indexa `datos`, y -Wconversion queda contento.
struct MinHeap {
    std::vector<int> datos;
};

// ============================================================================
// PARTE 1 — EL HEAP SOBRE UN ARREGLO
// ============================================================================

// Ejercicio 1 — Navegar el árbol implícito
// Devuelven la posición del padre, del hijo izquierdo y del hijo derecho del
// elemento en la posición i. No chequean que esas posiciones existan: eso lo
// hace quien las usa, comparando contra el tamaño del heap.
// Precondición de padre: i > 0 (la raíz no tiene padre).
// Ejemplos: padre(1) = padre(2) = 0, padre(5) = 2,
//           hijoIzq(0) = 1, hijoDer(0) = 2, hijoIzq(3) = 7.
// Complejidad: O(1).
size_t padre(size_t i);
size_t hijoIzq(size_t i);
size_t hijoDer(size_t i);

// Ejercicio 2 — Sift-up
// El elemento en la posición i puede ser más chico que su padre (por ejemplo,
// porque se acaba de agregar al final). Lo hace SUBIR intercambiándolo con su
// padre mientras sea estrictamente menor que él.
// Precondición: i < v.size() y v es un min-heap salvo, quizás, por v[i].
// Complejidad: O(log n).
void siftUp(std::vector<int>& v, size_t i);

// Ejercicio 3 — Sift-down
// El elemento en la posición i puede ser más grande que alguno de sus hijos
// (por ejemplo, porque se acaba de poner en la raíz). Lo hace BAJAR
// intercambiándolo con el MENOR de sus hijos, mientras sea mayor que él.
// Solo se consideran las primeras n posiciones del vector: lo que está de n
// en adelante no es parte del heap (así buildHeap y heapsort lo pueden
// reusar).
// Precondición: i < n <= v.size(), y los subárboles de los hijos de i ya son
// min-heaps.
// Complejidad: O(log n).
void siftDown(std::vector<int>& v, size_t i, size_t n);

// Ejercicio 4 — Insertar
// Agrega el valor al heap: lo pone al final del vector y lo hace subir.
// Complejidad: O(log n).
void insertar(MinHeap& heap, int valor);

// Ejercicio 5 — Mínimo
// Devuelve el mínimo del heap, sin sacarlo.
// Precondición: el heap no está vacío.
// Complejidad: O(1).
int minimo(const MinHeap& heap);

// Ejercicio 6 — Extraer mínimo
// Saca el mínimo del heap y lo devuelve: pone el ÚLTIMO elemento en la raíz,
// achica el vector en uno y hace bajar a la nueva raíz.
// Precondición: el heap no está vacío.
// Complejidad: O(log n).
int extraerMinimo(MinHeap& heap);

// Ejercicio 7 — Build heap (heapify)
// Reordena el vector (in-place) para que cumpla la propiedad de min-heap.
// Tiene que ser la versión O(n): siftDown desde el ÚLTIMO NODO CON HIJOS
// hasta la raíz (las hojas ya son heaps de un elemento).
// Ojo: i es size_t, así que un `for (size_t i = ...; i >= 0; i--)` no
// termina nunca (un size_t siempre es >= 0).
// Complejidad: O(n).
void buildHeap(std::vector<int>& v);

// Ejercicio 8 — ¿Es min-heap?
// Devuelve true si el vector cumple la propiedad de min-heap: cada elemento
// es menor o igual que sus hijos (los que existan).
// Ejemplos: {1, 3, 2, 7, 4} -> true    {1, 3, 2, 0} -> false    {} -> true
// Complejidad: O(n).
bool esMinHeap(const std::vector<int>& v);

// Ejercicio 9 — Máximo de un min-heap
// Devuelve el MÁXIMO de un min-heap (es la "pregunta para pensar" de los
// apuntes). El máximo no puede tener hijos (si los tuviera, serían mayores o
// iguales), así que es una HOJA: alcanza con mirar solo las hojas, que en un
// árbol casi completo son las posiciones de n/2 a n-1.
// Precondición: el vector no es vacío y es un min-heap.
// Complejidad: O(n) (se miran ~n/2 elementos; no se puede hacer en O(1)).
int maximoDeMinHeap(const std::vector<int>& v);

// ============================================================================
// PARTE 2 — HEAPSORT Y SELECCIÓN
// ============================================================================

// Ejercicio 10 — Heapsort
// Ordena el vector de MENOR a MAYOR, in-place (sin vectores auxiliares).
// Como el máximo tiene que terminar al final, se usa un MAX-heap:
//   1. buildHeap con un max-heap, en O(n).
//   2. n - 1 veces: intercambiar la raíz (el máximo) con el último del heap
//      "activo", achicar el heap activo en uno y hacer siftDown desde la raíz.
// Vas a necesitar un siftDown para max-heap (escribilo como función auxiliar).
// Complejidad: O(n log n) en tiempo, O(1) de memoria extra.
void heapsort(std::vector<int>& v);

// Ejercicio 11 — k-ésimo mayor
// Devuelve el k-ésimo elemento más grande del vector (k = 1 es el máximo).
// Los repetidos cuentan: en {3, 3, 1}, el 1º mayor es 3 y el 2º también.
// Ejemplo: {3, 2, 1, 5, 6, 4} con k = 2 -> 5.
// Precondición: 1 <= k <= v.size().
// Pista: mantené un MIN-heap con los k más grandes vistos hasta ahora. Si
// tiene más de k elementos, sacás el mínimo. Al final, la raíz es la
// respuesta. (Ordenar todo es O(n log n); así sale O(n log k).)
// Complejidad: O(n log k).
int kEsimoMayor(const std::vector<int>& v, int k);

// ============================================================================
// PARTE 3 — EL HEAP COMO COLA DE PRIORIDAD
// ============================================================================

// Ejercicio 12 — La piedra más pesada
// Hay piedras con ciertos pesos. En cada turno se toman las DOS más pesadas,
// x <= y, y se chocan: si x == y se destruyen las dos; si no, x se destruye
// y la otra queda con peso y - x. Se repite hasta que quede a lo sumo una.
// Devuelve el peso de la piedra que queda, o 0 si no queda ninguna.
// Ejemplo: {2, 7, 4, 1, 8, 1} -> 1
//   (8,7 -> 1)  {2,4,1,1,1}   (4,2 -> 2)  {2,1,1,1}
//   (2,1 -> 1)  {1,1,1}       (1,1 -> 0)  {1}
// Pista: un MAX-heap con los pesos.
// Complejidad: O(n log n).
int piedraMasPesada(const std::vector<int>& pesos);

// Ejercicio 13 — Costo mínimo para unir sogas
// Unir dos sogas de largos a y b cuesta a + b, y da una soga de largo a + b.
// Devuelve el costo total MÍNIMO de unir todas las sogas en una sola.
// Con 0 o 1 sogas, el costo es 0.
// Ejemplo: {4, 3, 2, 6} -> 29
//   unir 2 y 3 (cuesta 5)  -> {4, 5, 6}
//   unir 4 y 5 (cuesta 9)  -> {9, 6}
//   unir 6 y 9 (cuesta 15) -> {15}         total: 5 + 9 + 15 = 29
// Pista: siempre conviene unir las DOS más cortas (es la misma idea que el
// código de Huffman). Un min-heap te las da en O(log n).
// Complejidad: O(n log n).
long long costoUnirSogas(const std::vector<int>& largos);

// Ejercicio 14 — Mezclar k vectores ordenados
// Recibe k vectores, cada uno ordenado de menor a mayor, y devuelve un único
// vector ordenado con todos los elementos (con repetidos).
// Ejemplo: {{1, 4, 5}, {1, 3, 4}, {2, 6}} -> {1, 1, 2, 3, 4, 4, 5, 6}
// Puede haber vectores vacíos (o ninguno).
// Pista: un min-heap con el PRIMER elemento pendiente de cada vector, junto
// con de qué vector viene y en qué posición está (podés guardar tuplas o
// pares en un std::priority_queue). Sacás el mínimo, lo agregás al resultado
// y metés en el heap el siguiente de ese mismo vector.
// Complejidad: O(N log k), con N el total de elementos.
std::vector<int> mezclarOrdenados(const std::vector<std::vector<int>>& listas);

// Ejercicio 15 — Medianas de un flujo
// Los números llegan de a uno. Después de cada uno, hay que informar la
// mediana de todos los que llegaron hasta ese momento. Si hay una cantidad
// par, la mediana es el promedio de los dos del medio.
// Devuelve un vector con la mediana después de cada número.
// Ejemplo: {5, 15, 1, 3} -> {5, 10, 5, 4}
//   {5} -> 5    {5,15} -> 10    {1,5,15} -> 5    {1,3,5,15} -> (3+5)/2 = 4
// Pista: dos heaps. Un MAX-heap con la mitad más chica y un MIN-heap con la
// mitad más grande, balanceados para que el max-heap tenga la misma
// cantidad o uno más. La mediana sale de mirar las raíces.
// (Ordenar todo de nuevo con cada número sería O(n² log n).)
// Complejidad: O(n log n) en total.
std::vector<double> medianasEnFlujo(const std::vector<int>& numeros);

#endif
