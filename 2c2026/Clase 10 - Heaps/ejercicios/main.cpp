#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "ejercicios.h"

// ============================================================================
// Driver de pruebas.
//
// NO hace falta que edites este archivo: sirve para chequear tus soluciones.
// Compilá con `make` y corré `./ejercicios`. Cada línea muestra [PASA] o
// [FALLA] según si la función correspondiente ya funciona.
//
// Al principio va a fallar casi todo (las funciones son cáscaras vacías).
// A medida que las vayas implementando en "ejercicios.cpp", van a ir pasando.
//
// Las pruebas NO usan tus funciones para armar ni inspeccionar el heap (usan
// los helpers de abajo), así que un error en una función no hace fallar las
// pruebas de las demás.
// ============================================================================

static int totalPruebas = 0;
static int pruebasOk = 0;

static void chequear(const std::string& nombre, bool condicion)
{
    totalPruebas++;
    if (condicion)
    {
        pruebasOk++;
        std::cout << "[PASA]  " << nombre << std::endl;
    }
    else
    {
        std::cout << "[FALLA] " << nombre << std::endl;
    }
}

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

// Chequea la propiedad de min-heap a mano, sin usar tu esMinHeap.
static bool cumpleMinHeap(const std::vector<int>& v)
{
    for (size_t i = 1; i < v.size(); i++) {
        if (v[(i - 1) / 2] > v[i]) return false;
    }
    return true;
}

// ¿Tienen los mismos elementos (con repetidos), sin importar el orden?
static bool mismosElementos(std::vector<int> a, std::vector<int> b)
{
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

// Vector de números pseudo-aleatorios en [0, modulo), siempre los mismos.
static std::vector<int> pseudoAleatorios(size_t cantidad, unsigned int modulo, unsigned int semilla)
{
    std::vector<int> v;
    for (size_t i = 0; i < cantidad; i++) {
        semilla = semilla * 1103515245u + 12345u;
        v.push_back(static_cast<int>((semilla >> 4) % modulo));
    }
    return v;
}

// k-ésimo mayor ordenando todo, para comparar.
static int kEsimoMayorOrdenando(std::vector<int> v, int k)
{
    std::sort(v.begin(), v.end(), std::greater<int>());
    return v[static_cast<size_t>(k - 1)];
}

int main()
{
    /* Un min-heap válido que se reutiliza en varias pruebas:
                       1
                  /         \
                 3           2
               /   \       /   \
              7     4     5     9
             / \   /
            8  10 6
    */
    const std::vector<int> heapEjemplo = {1, 3, 2, 7, 4, 5, 9, 8, 10, 6};

    // ========================================================================
    // Ejercicio 1 — padre / hijoIzq / hijoDer
    // ========================================================================
    {
        chequear("Ej1 - padre (de 1 y de 2 es la raíz)", padre(1) == 0 && padre(2) == 0);
        chequear("Ej1 - padre (de 5 y de 6 es 2)", padre(5) == 2 && padre(6) == 2);
        chequear("Ej1 - padre (de 9 es 4)", padre(9) == 4);
        chequear("Ej1 - hijoIzq (de 0 es 1, de 3 es 7)", hijoIzq(0) == 1 && hijoIzq(3) == 7);
        chequear("Ej1 - hijoDer (de 0 es 2, de 3 es 8)", hijoDer(0) == 2 && hijoDer(3) == 8);

        bool idaYVuelta = true;
        for (size_t i = 0; i < 100; i++) {
            if (padre(hijoIzq(i)) != i || padre(hijoDer(i)) != i) idaYVuelta = false;
        }
        chequear("Ej1 - padre(hijoIzq(i)) == padre(hijoDer(i)) == i", idaYVuelta);
    }

    // ========================================================================
    // Ejercicio 2 — siftUp
    // ========================================================================
    {
        // Agrego un 0 al final del heap de ejemplo: tiene que subir hasta la raíz.
        std::vector<int> v = heapEjemplo;
        v.push_back(0);
        siftUp(v, v.size() - 1);
        chequear("Ej2 - siftUp (el 0 sube hasta la raíz)", v[0] == 0);
        chequear("Ej2 - siftUp (queda un min-heap)", cumpleMinHeap(v));
        chequear("Ej2 - siftUp (mismos elementos)", mismosElementos(v, {0, 1, 3, 2, 7, 4, 5, 9, 8, 10, 6}));

        // Un 11 al final ya está bien: no se mueve nada.
        std::vector<int> w = heapEjemplo;
        w.push_back(11);
        siftUp(w, w.size() - 1);
        std::vector<int> esperado = heapEjemplo;
        esperado.push_back(11);
        chequear("Ej2 - siftUp (si ya está bien, no cambia nada)", w == esperado);

        // Un 3 sube solo un nivel (su padre es el 4, y el 3 de arriba lo frena).
        std::vector<int> x = heapEjemplo;
        x.push_back(3);
        siftUp(x, x.size() - 1);
        chequear("Ej2 - siftUp (sube solo lo necesario)",
                 x == std::vector<int>({1, 3, 2, 7, 3, 5, 9, 8, 10, 6, 4}));

        std::vector<int> uno = {42};
        siftUp(uno, 0);
        chequear("Ej2 - siftUp (en la raíz no hace nada)", uno == std::vector<int>({42}));
    }

    // ========================================================================
    // Ejercicio 3 — siftDown
    // ========================================================================
    {
        // Pongo un 11 en la raíz: tiene que bajar por el camino de los menores.
        std::vector<int> v = heapEjemplo;
        v[0] = 11;
        siftDown(v, 0, v.size());
        chequear("Ej3 - siftDown (queda un min-heap)", cumpleMinHeap(v));
        chequear("Ej3 - siftDown (baja por el hijo MENOR)",
                 v == std::vector<int>({2, 3, 5, 7, 4, 11, 9, 8, 10, 6}));

        // Un nodo con un solo hijo (el 4, en la posición 4, solo tiene al 6).
        std::vector<int> w = {1, 3, 2, 7, 9, 5, 9, 8, 10, 6};
        siftDown(w, 4, w.size());
        chequear("Ej3 - siftDown (nodo con un solo hijo)",
                 w == std::vector<int>({1, 3, 2, 7, 6, 5, 9, 8, 10, 9}));

        // Una hoja no se mueve.
        std::vector<int> x = heapEjemplo;
        siftDown(x, 7, x.size());
        chequear("Ej3 - siftDown (una hoja no se mueve)", x == heapEjemplo);

        // Solo mira las primeras n posiciones: el 0 del final NO es parte del heap.
        std::vector<int> y = {9, 2, 3, 0};
        siftDown(y, 0, 3);
        chequear("Ej3 - siftDown (respeta el tamaño n)", y == std::vector<int>({2, 9, 3, 0}));
    }

    // ========================================================================
    // Ejercicio 4 — insertar
    // ========================================================================
    {
        MinHeap heap;
        insertar(heap, 5);
        chequear("Ej4 - insertar (en un heap vacío)", heap.datos == std::vector<int>({5}));

        const std::vector<int> valores = {8, 3, 9, 1, 7, 3, 2};
        bool siempreHeap = true;
        for (int x : valores) {
            insertar(heap, x);
            if (!cumpleMinHeap(heap.datos)) siempreHeap = false;
        }
        chequear("Ej4 - insertar (después de cada insert es un min-heap)", siempreHeap);
        chequear("Ej4 - insertar (tiene todos los elementos)",
                 mismosElementos(heap.datos, {5, 8, 3, 9, 1, 7, 3, 2}));
        chequear("Ej4 - insertar (el mínimo queda en la raíz)", !heap.datos.empty() && heap.datos[0] == 1);
    }

    // ========================================================================
    // Ejercicio 5 — minimo
    // ========================================================================
    {
        MinHeap heap;
        heap.datos = heapEjemplo;
        chequear("Ej5 - minimo (heap de ejemplo)", minimo(heap) == 1);
        chequear("Ej5 - minimo (no modifica el heap)", heap.datos == heapEjemplo);

        MinHeap uno;
        uno.datos = {-4};
        chequear("Ej5 - minimo (un solo elemento)", minimo(uno) == -4);
    }

    // ========================================================================
    // Ejercicio 6 — extraerMinimo
    // ========================================================================
    {
        MinHeap heap;
        heap.datos = heapEjemplo;
        int m = extraerMinimo(heap);
        chequear("Ej6 - extraerMinimo (devuelve el mínimo)", m == 1);
        chequear("Ej6 - extraerMinimo (queda un elemento menos)", heap.datos.size() == heapEjemplo.size() - 1);
        chequear("Ej6 - extraerMinimo (sigue siendo min-heap)", cumpleMinHeap(heap.datos));
        chequear("Ej6 - extraerMinimo (saca justo el mínimo)",
                 mismosElementos(heap.datos, {3, 2, 7, 4, 5, 9, 8, 10, 6}));

        // Extraer todo devuelve los elementos en orden creciente.
        MinHeap otro;
        otro.datos = heapEjemplo;
        std::vector<int> extraidos;
        for (size_t i = 0; i < heapEjemplo.size(); i++) {
            extraidos.push_back(extraerMinimo(otro));
        }
        chequear("Ej6 - extraerMinimo (sacando todo, sale ordenado)",
                 extraidos == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
        chequear("Ej6 - extraerMinimo (al final queda vacío)", otro.datos.empty());

        MinHeap conRepetidos;
        conRepetidos.datos = {2, 2, 2};
        int a = extraerMinimo(conRepetidos);
        chequear("Ej6 - extraerMinimo (con repetidos)",
                 a == 2 && conRepetidos.datos == std::vector<int>({2, 2}));
    }

    // ========================================================================
    // Ejercicio 7 — buildHeap
    // ========================================================================
    {
        std::vector<int> v = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        buildHeap(v);
        chequear("Ej7 - buildHeap (vector al revés queda min-heap)", cumpleMinHeap(v));
        chequear("Ej7 - buildHeap (mismos elementos)", mismosElementos(v, {1, 2, 3, 4, 5, 6, 7, 8, 9}));
        // Con siftDown de abajo hacia arriba, el resultado es exactamente este.
        chequear("Ej7 - buildHeap (siftDown desde el último nodo con hijos)",
                 v == std::vector<int>({1, 2, 3, 6, 5, 4, 7, 8, 9}));

        std::vector<int> vacio;
        buildHeap(vacio);
        chequear("Ej7 - buildHeap (vector vacío)", vacio.empty());

        std::vector<int> uno = {3};
        buildHeap(uno);
        chequear("Ej7 - buildHeap (un elemento)", uno == std::vector<int>({3}));

        std::vector<int> grande = pseudoAleatorios(500, 1000, 777);
        std::vector<int> original = grande;
        buildHeap(grande);
        chequear("Ej7 - buildHeap (500 números pseudo-aleatorios)",
                 cumpleMinHeap(grande) && mismosElementos(grande, original));
    }

    // ========================================================================
    // Ejercicio 8 — esMinHeap
    // ========================================================================
    {
        chequear("Ej8 - esMinHeap (heap de ejemplo)", esMinHeap(heapEjemplo));
        chequear("Ej8 - esMinHeap (vacío)", esMinHeap({}));
        chequear("Ej8 - esMinHeap (un elemento)", esMinHeap({7}));
        chequear("Ej8 - esMinHeap (con repetidos)", esMinHeap({2, 2, 2, 3}));
        chequear("Ej8 - esMinHeap (falla en la raíz)", !esMinHeap({5, 3, 8}));
        chequear("Ej8 - esMinHeap (falla en una hoja del último nivel)", !esMinHeap({1, 3, 2, 0}));
        chequear("Ej8 - esMinHeap (falla con el hijo derecho)", !esMinHeap({1, 3, 2, 7, 4, 5, 1}));
        chequear("Ej8 - esMinHeap (ordenado creciente sí es heap)", esMinHeap({1, 2, 3, 4, 5, 6}));
    }

    // ========================================================================
    // Ejercicio 9 — maximoDeMinHeap
    // ========================================================================
    {
        chequear("Ej9 - maximoDeMinHeap (heap de ejemplo)", maximoDeMinHeap(heapEjemplo) == 10);
        chequear("Ej9 - maximoDeMinHeap (un elemento)", maximoDeMinHeap({4}) == 4);
        chequear("Ej9 - maximoDeMinHeap (dos elementos)", maximoDeMinHeap({1, 6}) == 6);
        chequear("Ej9 - maximoDeMinHeap (el máximo en la última posición)",
                 maximoDeMinHeap({1, 2, 3, 4, 5, 6, 7}) == 7);
        chequear("Ej9 - maximoDeMinHeap (el máximo en la primera hoja)",
                 maximoDeMinHeap({1, 2, 3, 20, 5, 6, 7}) == 20);
    }

    // ========================================================================
    // Ejercicio 10 — heapsort
    // ========================================================================
    {
        std::vector<int> v = {5, 2, 9, 1, 5, 6, 3};
        heapsort(v);
        chequear("Ej10 - heapsort (ejemplo con repetidos)", v == std::vector<int>({1, 2, 3, 5, 5, 6, 9}));

        std::vector<int> vacio;
        heapsort(vacio);
        chequear("Ej10 - heapsort (vacío)", vacio.empty());

        std::vector<int> uno = {8};
        heapsort(uno);
        chequear("Ej10 - heapsort (un elemento)", uno == std::vector<int>({8}));

        std::vector<int> negativos = {0, -3, 7, -10, 2};
        heapsort(negativos);
        chequear("Ej10 - heapsort (con negativos)", negativos == std::vector<int>({-10, -3, 0, 2, 7}));

        std::vector<int> grande = pseudoAleatorios(1000, 5000, 2026);
        std::vector<int> esperado = grande;
        std::sort(esperado.begin(), esperado.end());
        heapsort(grande);
        chequear("Ej10 - heapsort (coincide con std::sort en 1000 números)", grande == esperado);
    }

    // ========================================================================
    // Ejercicio 11 — kEsimoMayor
    // ========================================================================
    {
        chequear("Ej11 - kEsimoMayor (ejemplo, k = 2)", kEsimoMayor({3, 2, 1, 5, 6, 4}, 2) == 5);
        chequear("Ej11 - kEsimoMayor (k = 1 es el máximo)", kEsimoMayor({3, 2, 1, 5, 6, 4}, 1) == 6);
        chequear("Ej11 - kEsimoMayor (k = n es el mínimo)", kEsimoMayor({3, 2, 1, 5, 6, 4}, 6) == 1);
        chequear("Ej11 - kEsimoMayor (con repetidos)", kEsimoMayor({3, 2, 3, 1, 2, 4, 5, 5, 6}, 4) == 4);
        chequear("Ej11 - kEsimoMayor (todos iguales)", kEsimoMayor({7, 7, 7}, 2) == 7);

        std::vector<int> grande = pseudoAleatorios(400, 100000, 31);
        chequear("Ej11 - kEsimoMayor (coincide ordenando, k = 37)",
                 kEsimoMayor(grande, 37) == kEsimoMayorOrdenando(grande, 37));
    }

    // ========================================================================
    // Ejercicio 12 — piedraMasPesada
    // ========================================================================
    {
        chequear("Ej12 - piedraMasPesada (ejemplo)", piedraMasPesada({2, 7, 4, 1, 8, 1}) == 1);
        chequear("Ej12 - piedraMasPesada (una sola piedra)", piedraMasPesada({5}) == 5);
        chequear("Ej12 - piedraMasPesada (se destruyen todas)", piedraMasPesada({3, 3}) == 0);
        chequear("Ej12 - piedraMasPesada (sin piedras)", piedraMasPesada({}) == 0);
        chequear("Ej12 - piedraMasPesada (10, 4, 2 -> 4)", piedraMasPesada({10, 4, 2}) == 4);
    }

    // ========================================================================
    // Ejercicio 13 — costoUnirSogas
    // ========================================================================
    {
        chequear("Ej13 - costoUnirSogas (ejemplo: 29)", costoUnirSogas({4, 3, 2, 6}) == 29);
        chequear("Ej13 - costoUnirSogas (otro ejemplo: 33)", costoUnirSogas({1, 2, 3, 4, 5}) == 33);
        chequear("Ej13 - costoUnirSogas (una soga)", costoUnirSogas({10}) == 0);
        chequear("Ej13 - costoUnirSogas (sin sogas)", costoUnirSogas({}) == 0);
        chequear("Ej13 - costoUnirSogas (dos sogas)", costoUnirSogas({7, 8}) == 15);
        chequear("Ej13 - costoUnirSogas (no entra en un int)",
                 costoUnirSogas({1000000000, 1000000000, 1000000000}) == 5000000000LL);
    }

    // ========================================================================
    // Ejercicio 14 — mezclarOrdenados
    // ========================================================================
    {
        chequear("Ej14 - mezclarOrdenados (ejemplo)",
                 mezclarOrdenados({{1, 4, 5}, {1, 3, 4}, {2, 6}}) ==
                     std::vector<int>({1, 1, 2, 3, 4, 4, 5, 6}));
        chequear("Ej14 - mezclarOrdenados (con vectores vacíos)",
                 mezclarOrdenados({{}, {3}, {}, {1, 2}}) == std::vector<int>({1, 2, 3}));
        chequear("Ej14 - mezclarOrdenados (ningún vector)", mezclarOrdenados({}).empty());
        chequear("Ej14 - mezclarOrdenados (un solo vector)",
                 mezclarOrdenados({{-2, 0, 9}}) == std::vector<int>({-2, 0, 9}));

        std::vector<std::vector<int>> listas;
        std::vector<int> todos;
        for (unsigned int l = 0; l < 20; l++) {
            std::vector<int> lista = pseudoAleatorios(l * 3, 1000, l + 1);
            std::sort(lista.begin(), lista.end());
            todos.insert(todos.end(), lista.begin(), lista.end());
            listas.push_back(lista);
        }
        std::sort(todos.begin(), todos.end());
        chequear("Ej14 - mezclarOrdenados (20 listas de distinto largo)", mezclarOrdenados(listas) == todos);
    }

    // ========================================================================
    // Ejercicio 15 — medianasEnFlujo
    // ========================================================================
    {
        chequear("Ej15 - medianasEnFlujo (ejemplo)",
                 medianasEnFlujo({5, 15, 1, 3}) == std::vector<double>({5, 10, 5, 4}));
        chequear("Ej15 - medianasEnFlujo (promedio no entero)",
                 medianasEnFlujo({1, 2}) == std::vector<double>({1, 1.5}));
        chequear("Ej15 - medianasEnFlujo (creciente)",
                 medianasEnFlujo({1, 2, 3, 4, 5}) == std::vector<double>({1, 1.5, 2, 2.5, 3}));
        chequear("Ej15 - medianasEnFlujo (decreciente)",
                 medianasEnFlujo({5, 4, 3, 2, 1}) == std::vector<double>({5, 4.5, 4, 3.5, 3}));
        chequear("Ej15 - medianasEnFlujo (con negativos y repetidos)",
                 medianasEnFlujo({-1, -1, 4, -1}) == std::vector<double>({-1, -1, -1, -1}));
        chequear("Ej15 - medianasEnFlujo (vacío)", medianasEnFlujo({}).empty());
    }

    // ========================================================================
    // Resumen
    // ========================================================================
    std::cout << "\nResultado: " << pruebasOk << "/" << totalPruebas
              << " pruebas pasadas." << std::endl;

    if (pruebasOk == totalPruebas)
    {
        std::cout << "\nPara verificar que no hay memory leaks, corré: make valgrind" << std::endl;
    }

    return 0;
}
