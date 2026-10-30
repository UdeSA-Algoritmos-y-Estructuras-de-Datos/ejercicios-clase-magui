#include "ejercicios.h"

// ============================================================================
// Acá va TU código. Cada función tiene una cáscara con un "TODO": borrá el
// contenido de ejemplo y escribí la implementación.
//
// Los `(void)parametro;` que ves abajo están solo para que la cáscara
// compile sin advertencias antes de que la implementes. Cuando uses el
// parámetro en tu código, borrá la línea `(void)...` correspondiente.
//
// Podés agregar funciones auxiliares arriba de la función que las usa. Si
// querés que sean privadas del archivo, declaralas `static`.
//
// Los índices del heap son size_t. Ojo con -Wconversion: para comparar un
// size_t con un int (por ejemplo, con k) usá static_cast. Y ojo con recorrer
// para atrás: con size_t, `i >= 0` es SIEMPRE verdadero.
//
// Para la Parte 3 podés usar tu MinHeap o std::priority_queue (agregá
// `#include <queue>` y, para el min-heap, `#include <functional>`).
// ============================================================================

// ============================================================================
// PARTE 1 — EL HEAP SOBRE UN ARREGLO
// ============================================================================

// Ejercicio 1 — Navegar el árbol implícito
size_t padre(size_t i)
{
    // TODO: implementar padre.
    // Se espera la posición del padre del elemento i (i > 0).
    (void)i;
    return 0;
}

size_t hijoIzq(size_t i)
{
    // TODO: implementar hijoIzq.
    // Se espera la posición del hijo izquierdo del elemento i.
    (void)i;
    return 0;
}

size_t hijoDer(size_t i)
{
    // TODO: implementar hijoDer.
    // Se espera la posición del hijo derecho del elemento i.
    (void)i;
    return 0;
}

// Ejercicio 2 — Sift-up
void siftUp(std::vector<int>& v, size_t i)
{
    // TODO: implementar siftUp.
    // Se espera que, después de la llamada, el vector entero cumpla la
    // propiedad de min-heap. Solo se mueven elementos del camino entre i y
    // la raíz.
    (void)v;
    (void)i;
}

// Ejercicio 3 — Sift-down
void siftDown(std::vector<int>& v, size_t i, size_t n)
{
    // TODO: implementar siftDown.
    // Se espera que, después de la llamada, las primeras n posiciones cumplan
    // la propiedad de min-heap. Las posiciones de n en adelante no se tocan.
    // Pensá qué pasa si el nodo tiene un solo hijo, o ninguno.
    (void)v;
    (void)i;
    (void)n;
}

// Ejercicio 4 — Insertar
void insertar(MinHeap& heap, int valor)
{
    // TODO: implementar insertar.
    // Se espera que el heap tenga un elemento más (el valor) y siga siendo
    // un min-heap.
    (void)heap;
    (void)valor;
}

// Ejercicio 5 — Mínimo
int minimo(const MinHeap& heap)
{
    // TODO: implementar minimo.
    // Se espera el menor elemento del heap (que no está vacío), sin
    // modificarlo.
    (void)heap;
    return 0;
}

// Ejercicio 6 — Extraer mínimo
int extraerMinimo(MinHeap& heap)
{
    // TODO: implementar extraerMinimo.
    // Se espera el menor elemento del heap, que el heap quede con un
    // elemento menos (justo ese) y que siga siendo un min-heap. Pensá qué
    // pasa cuando el heap tenía un único elemento.
    (void)heap;
    return 0;
}

// Ejercicio 7 — Build heap (heapify)
void buildHeap(std::vector<int>& v)
{
    // TODO: implementar buildHeap.
    // Se espera que el vector quede con los mismos elementos, pero cumpliendo
    // la propiedad de min-heap, y en O(n): no alcanza con insertar uno por
    // uno.
    (void)v;
}

// Ejercicio 8 — ¿Es min-heap?
bool esMinHeap(const std::vector<int>& v)
{
    // TODO: implementar esMinHeap.
    // Se espera true si ningún elemento es mayor que alguno de sus hijos.
    (void)v;
    return false;
}

// Ejercicio 9 — Máximo de un min-heap
int maximoDeMinHeap(const std::vector<int>& v)
{
    // TODO: implementar maximoDeMinHeap.
    // Se espera el mayor elemento del heap, aprovechando la propiedad de
    // heap para no mirar los nodos que tienen hijos.
    (void)v;
    return 0;
}

// ============================================================================
// PARTE 2 — HEAPSORT Y SELECCIÓN
// ============================================================================

// Ejercicio 10 — Heapsort
void heapsort(std::vector<int>& v)
{
    // TODO: implementar heapsort.
    // Se espera el vector ordenado de menor a mayor, sin usar memoria extra
    // proporcional a n (nada de vectores auxiliares ni std::sort).
    (void)v;
}

// Ejercicio 11 — k-ésimo mayor
int kEsimoMayor(const std::vector<int>& v, int k)
{
    // TODO: implementar kEsimoMayor.
    // Se espera el elemento que quedaría en la posición k (contando desde 1)
    // si se ordenara el vector de mayor a menor, pero sin ordenarlo entero:
    // el heap nunca debería tener más de k + 1 elementos.
    (void)v;
    (void)k;
    return 0;
}

// ============================================================================
// PARTE 3 — EL HEAP COMO COLA DE PRIORIDAD
// ============================================================================

// Ejercicio 12 — La piedra más pesada
int piedraMasPesada(const std::vector<int>& pesos)
{
    // TODO: implementar piedraMasPesada.
    // Se espera el peso de la única piedra que sobrevive a todos los
    // choques, o 0 si no sobrevive ninguna (o si no había piedras).
    (void)pesos;
    return 0;
}

// Ejercicio 13 — Costo mínimo para unir sogas
long long costoUnirSogas(const std::vector<int>& largos)
{
    // TODO: implementar costoUnirSogas.
    // Se espera el menor costo total posible para terminar con una sola
    // soga. Ojo: los costos se suman y pueden pasarse de int.
    (void)largos;
    return 0;
}

// Ejercicio 14 — Mezclar k vectores ordenados
std::vector<int> mezclarOrdenados(const std::vector<std::vector<int>>& listas)
{
    // TODO: implementar mezclarOrdenados.
    // Se espera un vector ordenado con todos los elementos de todas las
    // listas (con repetidos), sin concatenar y ordenar todo de nuevo.
    (void)listas;
    return {};
}

// Ejercicio 15 — Medianas de un flujo
std::vector<double> medianasEnFlujo(const std::vector<int>& numeros)
{
    // TODO: implementar medianasEnFlujo.
    // Se espera un vector del mismo largo que `numeros`, donde la posición i
    // tiene la mediana de los primeros i + 1 números. Cuidado con la división
    // entera al promediar.
    (void)numeros;
    return {};
}
