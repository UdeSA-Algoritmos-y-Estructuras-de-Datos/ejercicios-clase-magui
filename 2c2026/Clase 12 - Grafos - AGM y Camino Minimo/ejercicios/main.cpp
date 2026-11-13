#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
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
// Las pruebas siguen el orden de "ejercicios.h": Kruskal, Prim, camino mínimo
// y, al final, los opcionales (que se cuentan aparte).
//
// Las pruebas NO usan tus funciones para armar ni inspeccionar los grafos
// (usan los helpers de abajo, que escriben directo en g.n y g.ady), así que
// un error en una función no hace fallar las pruebas de las demás. Eso sí:
// los algoritmos (kruskal, prim, dijkstra, ...) se apoyan en TUS funciones
// de los ejercicios anteriores de la misma parte.
//
// Si el programa se queda colgado, casi seguro un recorrido tuyo no está
// avanzando (por ejemplo, una cola de prioridad que nunca se vacía).
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
// Helpers: armado de grafos (sin usar las funciones del alumno)
// ----------------------------------------------------------------------------

typedef std::vector<size_t> Vertices;
typedef std::vector<Arista> Aristas;
typedef std::vector<Peso> Pesos;

static bool operator==(const Arista& a, const Arista& b)
{
    return a.v == b.v && a.w == b.w && a.peso == b.peso;
}

static bool menorArista(const Arista& a, const Arista& b)
{
    if (a.v != b.v) return a.v < b.v;
    if (a.w != b.w) return a.w < b.w;
    return a.peso < b.peso;
}

// La misma arista, con los extremos en orden (v < w).
static Arista normalizada(const Arista& a)
{
    if (a.v <= a.w) return a;
    return Arista{a.w, a.v, a.peso};
}

static Aristas normalizadas(const Aristas& as)
{
    Aristas res;
    for (const Arista& a : as) res.push_back(normalizada(a));
    return res;
}

static Aristas ordenadas(Aristas as)
{
    std::sort(as.begin(), as.end(), menorArista);
    return as;
}

static void ordenarListas(GrafoPonderado& g)
{
    for (size_t x = 0; x < g.n; x++) {
        std::sort(g.ady[x].begin(), g.ady[x].end(),
                  [](const Vecino& a, const Vecino& b) { return a.vertice < b.vertice; });
    }
}

static GrafoPonderado sinAristas(size_t n)
{
    GrafoPonderado g;
    g.n = n;
    g.ady = std::vector<std::vector<Vecino>>(n);
    return g;
}

// Grafo NO dirigido: cada arista en las dos listas. Listas ordenadas por vecino.
static GrafoPonderado noDirigido(size_t n, const Aristas& aristas)
{
    GrafoPonderado g = sinAristas(n);
    for (const Arista& e : aristas) {
        g.ady[e.v].push_back(Vecino{e.w, e.peso});
        g.ady[e.w].push_back(Vecino{e.v, e.peso});
    }
    ordenarListas(g);
    return g;
}

// Grafo DIRIGIDO: cada arista sólo en la lista del origen. Listas ordenadas.
static GrafoPonderado dirigido(size_t n, const Aristas& arcos)
{
    GrafoPonderado g = sinAristas(n);
    for (const Arista& e : arcos) g.ady[e.v].push_back(Vecino{e.w, e.peso});
    ordenarListas(g);
    return g;
}

// Los arcos de una lista de aristas no dirigidas (cada una en los dos sentidos).
static Aristas ambosSentidos(const Aristas& aristas)
{
    Aristas res;
    for (const Arista& e : aristas) {
        res.push_back(e);
        res.push_back(Arista{e.w, e.v, e.peso});
    }
    return res;
}

// El grafo de la teórica (clase 18): A, B, C, D, E, F son 0, 1, 2, 3, 4, 5.
static const size_t N_EJ = 6;
static const Aristas EJ = {
    {0, 1, 7}, {0, 2, 3}, {1, 2, 1}, {1, 3, 6}, {2, 3, 8},
    {2, 4, 9}, {3, 4, 2}, {3, 5, 5}, {4, 5, 4},
};

// Números pseudo-aleatorios, siempre los mismos para la misma semilla.
static unsigned int siguiente(unsigned int& semilla)
{
    semilla = semilla * 1103515245u + 12345u;
    return semilla >> 8;
}

static Peso pesoAlAzar(unsigned int& semilla, Peso minimo, Peso maximo)
{
    return minimo + static_cast<Peso>(siguiente(semilla) % static_cast<unsigned int>(maximo - minimo + 1));
}

// Un grafo conexo al azar: un árbol al azar más hasta `extra` aristas más.
// Pesos entre 1 y maxPeso (con maxPeso chico hay muchos pesos repetidos).
static Aristas aristasConexasAlAzar(size_t n, size_t extra, unsigned int semilla, Peso maxPeso)
{
    Aristas res;
    std::set<std::pair<size_t, size_t>> usadas;
    for (size_t i = 1; i < n; i++) {
        size_t j = siguiente(semilla) % i;
        res.push_back(Arista{j, i, pesoAlAzar(semilla, 1, maxPeso)});
        usadas.insert(std::make_pair(j, i));
    }
    for (size_t k = 0; k < extra && n >= 2; k++) {
        size_t a = siguiente(semilla) % n;
        size_t b = siguiente(semilla) % n;
        if (a == b) continue;
        std::pair<size_t, size_t> clave(std::min(a, b), std::max(a, b));
        if (usadas.insert(clave).second) res.push_back(Arista{clave.first, clave.second, pesoAlAzar(semilla, 1, maxPeso)});
    }
    return res;
}

// Hasta `intentos` arcos dirigidos al azar (sin bucles ni repetidos).
static Aristas arcosAlAzar(size_t n, size_t intentos, unsigned int semilla, Peso minPeso, Peso maxPeso)
{
    Aristas res;
    std::set<std::pair<size_t, size_t>> usados;
    for (size_t i = 0; i < intentos; i++) {
        size_t a = siguiente(semilla) % n;
        size_t b = siguiente(semilla) % n;
        if (a == b) continue;
        if (usados.insert(std::make_pair(a, b)).second) res.push_back(Arista{a, b, pesoAlAzar(semilla, minPeso, maxPeso)});
    }
    return res;
}

// ----------------------------------------------------------------------------
// Helpers: respuestas de referencia
// ----------------------------------------------------------------------------

struct UnionFind {
    std::vector<size_t> padre;

    explicit UnionFind(size_t n) : padre(n)
    {
        for (size_t i = 0; i < n; i++) padre[i] = i;
    }
    size_t buscar(size_t x)
    {
        while (padre[x] != x) {
            padre[x] = padre[padre[x]];
            x = padre[x];
        }
        return x;
    }
    // Devuelve false si ya estaban juntos (la arista cierra un ciclo).
    bool unir(size_t a, size_t b)
    {
        size_t ra = buscar(a), rb = buscar(b);
        if (ra == rb) return false;
        padre[ra] = rb;
        return true;
    }
};

// Peso de un AGM de un grafo conexo, por Kruskal.
static Peso pesoAGMKruskal(size_t n, Aristas aristas)
{
    std::sort(aristas.begin(), aristas.end(), [](const Arista& a, const Arista& b) { return a.peso < b.peso; });
    UnionFind uf(n);
    Peso total = 0;
    for (const Arista& a : aristas) {
        if (uf.unir(a.v, a.w)) total += a.peso;
    }
    return total;
}

// Peso de un AGM por fuerza bruta: prueba todos los subconjuntos de n - 1
// aristas. Sólo para grafos chicos (m <= 16).
static Peso pesoAGMFuerzaBruta(size_t n, const Aristas& aristas)
{
    size_t m = aristas.size();
    bool hay = false;
    Peso mejor = 0;
    for (size_t mascara = 0; mascara < (static_cast<size_t>(1) << m); mascara++) {
        size_t cant = 0;
        for (size_t i = 0; i < m; i++) {
            if ((mascara >> i) & 1) cant++;
        }
        if (cant != n - 1) continue;
        UnionFind uf(n);
        bool arbol = true;
        Peso total = 0;
        for (size_t i = 0; i < m; i++) {
            if (((mascara >> i) & 1) == 0) continue;
            if (!uf.unir(aristas[i].v, aristas[i].w)) arbol = false;
            total += aristas[i].peso;
        }
        if (arbol && (!hay || total < mejor)) {
            mejor = total;
            hay = true;
        }
    }
    return mejor;
}

// ¿Es t un árbol generador del grafo de n vértices con esas aristas?
static bool esArbolGeneradorReferencia(size_t n, const Aristas& aristas, const Aristas& t)
{
    std::map<std::pair<size_t, size_t>, Peso> pesos;
    for (const Arista& a : aristas) pesos[std::make_pair(std::min(a.v, a.w), std::max(a.v, a.w))] = a.peso;
    if (t.size() + 1 != n) return false;
    UnionFind uf(n);
    for (const Arista& a : t) {
        if (a.v >= n || a.w >= n) return false;
        std::map<std::pair<size_t, size_t>, Peso>::const_iterator it =
            pesos.find(std::make_pair(std::min(a.v, a.w), std::max(a.v, a.w)));
        if (it == pesos.end() || it->second != a.peso) return false;
        if (!uf.unir(a.v, a.w)) return false;
    }
    return true;
}

// ¿Cada arista de t tiene su extremo v ya en el árbol, y el w todavía no?
// (así crece Prim desde r)
static bool creceDesde(size_t r, const Aristas& t)
{
    std::set<size_t> enArbol;
    enArbol.insert(r);
    for (const Arista& a : t) {
        if (enArbol.count(a.v) == 0 || enArbol.count(a.w) != 0) return false;
        enArbol.insert(a.w);
    }
    return true;
}

// Bellman-Ford de referencia. Devuelve false si hay un ciclo negativo alcanzable.
static bool bellmanFordReferencia(size_t n, const Aristas& arcos, size_t origen, Pesos& dist)
{
    dist.assign(n, INFINITO);
    dist[origen] = 0;
    for (size_t ronda = 0; ronda < n; ronda++) {
        bool cambio = false;
        for (const Arista& a : arcos) {
            if (dist[a.v] != INFINITO && dist[a.v] + a.peso < dist[a.w]) {
                dist[a.w] = dist[a.v] + a.peso;
                cambio = true;
            }
        }
        if (!cambio) return true;
    }
    return false;
}

// Cantidad de rondas que mejoran algo, con los arcos en este orden.
static size_t rondasReferencia(size_t n, const Aristas& arcos, size_t origen)
{
    Pesos dist(n, INFINITO);
    dist[origen] = 0;
    size_t rondas = 0;
    for (size_t i = 1; i < n; i++) {
        bool cambio = false;
        for (const Arista& a : arcos) {
            if (dist[a.v] != INFINITO && dist[a.v] + a.peso < dist[a.w]) {
                dist[a.w] = dist[a.v] + a.peso;
                cambio = true;
            }
        }
        if (!cambio) break;
        rondas++;
    }
    return rondas;
}

// ¿Es `camino` un camino de v a w que sólo usa esos arcos? Si lo es, deja su
// peso en `peso`.
static bool esCaminoReferencia(const Aristas& arcos, const Vertices& camino, size_t v, size_t w, Peso& peso)
{
    if (camino.empty() || camino.front() != v || camino.back() != w) return false;
    std::map<std::pair<size_t, size_t>, Peso> pesos;
    for (const Arista& a : arcos) pesos[std::make_pair(a.v, a.w)] = a.peso;
    peso = 0;
    for (size_t i = 0; i + 1 < camino.size(); i++) {
        std::map<std::pair<size_t, size_t>, Peso>::const_iterator it = pesos.find(std::make_pair(camino[i], camino[i + 1]));
        if (it == pesos.end()) return false;
        peso += it->second;
    }
    return true;
}

// Cantidad mínima de monedas para dar `monto` (programación dinámica).
static size_t vueltoOptimo(const std::vector<size_t>& monedas, size_t monto)
{
    std::vector<size_t> mejor(monto + 1, IMPOSIBLE);
    mejor[0] = 0;
    for (size_t m = 1; m <= monto; m++) {
        for (size_t c : monedas) {
            if (c <= m && mejor[m - c] != IMPOSIBLE && (mejor[m] == IMPOSIBLE || mejor[m - c] + 1 < mejor[m])) {
                mejor[m] = mejor[m - c] + 1;
            }
        }
    }
    return mejor[monto];
}

// ¿Tiene la raíz de este elemento a lo sumo `maxAltura` aristas hasta arriba?
static size_t alturaDe(const Bosque& b, size_t x)
{
    size_t altura = 0;
    while (b.padre[x] != x) {
        x = b.padre[x];
        altura++;
    }
    return altura;
}

// Suma de los pesos (sin usar pesoDeAristas del alumno).
static Peso pesoTotal(const Aristas& as)
{
    Peso total = 0;
    for (const Arista& a : as) total += a.peso;
    return total;
}

// Vacía la cola y devuelve su contenido como aristas {x, y, peso}, en el
// orden en que salen (de menor a mayor).
static Aristas vaciar(ColaDeAristas& cola)
{
    Aristas res;
    while (!cola.empty()) {
        AristaDeCola e = cola.top();
        cola.pop();
        res.push_back(Arista{std::get<1>(e), std::get<2>(e), std::get<0>(e)});
    }
    return res;
}

// Un bosque con cada elemento en su propio árbol (sin usar crearBosque).
static Bosque bosqueSuelto(size_t n)
{
    Bosque b;
    b.padre.resize(n);
    b.tam.assign(n, 1);
    for (size_t i = 0; i < n; i++) b.padre[i] = i;
    return b;
}

// Las pruebas de Kruskal: las mismas para kruskal y para kruskalConBosque.
static void probarKruskal(const std::string& ej, std::vector<Arista> (*alg)(const GrafoPonderado&))
{
    GrafoPonderado g = noDirigido(N_EJ, EJ);
    Aristas res = alg(g);
    chequear(ej + " (la teórica: B-C, D-E, A-C, E-F, B-D, en el orden en que se eligen)",
             normalizadas(res) == Aristas{{1, 2, 1}, {3, 4, 2}, {0, 2, 3}, {4, 5, 4}, {1, 3, 6}});
    chequear(ej + " (la teórica: es un árbol generador de peso 16)",
             esArbolGeneradorReferencia(N_EJ, EJ, res) && pesoTotal(res) == 16);
    chequear(ej + " (n = 1: ninguna arista)", alg(noDirigido(1, Aristas{})).empty());
    chequear(ej + " (n = 2)", normalizadas(alg(noDirigido(2, Aristas{{0, 1, 5}}))) == Aristas{{0, 1, 5}});

    Aristas triangulo = {{0, 1, 1}, {1, 2, 2}, {0, 2, 3}};
    chequear(ej + " (un triángulo: se descarta la más pesada)",
             ordenadas(normalizadas(alg(noDirigido(3, triangulo)))) == Aristas{{0, 1, 1}, {1, 2, 2}});

    Aristas conNegativos = {{0, 1, -4}, {1, 2, 3}, {0, 2, -1}, {2, 3, 2}, {1, 3, -5}};
    Aristas rn = alg(noDirigido(4, conNegativos));
    chequear(ej + " (con pesos negativos)",
             esArbolGeneradorReferencia(4, conNegativos, rn) && pesoTotal(rn) == pesoAGMFuerzaBruta(4, conNegativos));

    // Todos los pesos iguales: cualquier árbol generador sirve.
    Aristas iguales = aristasConexasAlAzar(8, 8, 3, 1);
    chequear(ej + " (todos los pesos iguales)", esArbolGeneradorReferencia(8, iguales, alg(noDirigido(8, iguales))));

    bool chicos = true;
    for (unsigned int s = 1; s <= 40; s++) {
        size_t n = 1 + s % 8;
        Aristas ar = aristasConexasAlAzar(n, s % 7, s, 1 + static_cast<Peso>(s % 5));
        Aristas r = alg(noDirigido(n, ar));
        if (!esArbolGeneradorReferencia(n, ar, r) || pesoTotal(r) != pesoAGMFuerzaBruta(n, ar)) chicos = false;
    }
    chequear(ej + " (grafos chicos al azar, contra fuerza bruta)", chicos);

    bool grandes = true;
    for (unsigned int s = 1; s <= 15; s++) {
        size_t n = 10 + s * 3;
        Aristas ar = aristasConexasAlAzar(n, n * 3, s, 30);
        Aristas r = alg(noDirigido(n, ar));
        if (!esArbolGeneradorReferencia(n, ar, r) || pesoTotal(r) != pesoAGMKruskal(n, ar)) grandes = false;
    }
    chequear(ej + " (grafos grandes al azar)", grandes);
}

static void titulo(const std::string& texto)
{
    std::cout << "\n=== " << texto << " ===" << std::endl;
}

// ============================================================================
// main
// ============================================================================

int main()
{
    titulo("PARTE 1 - KRUSKAL");

    // ========================================================================
    // Ejercicio 1 — crearParticion
    // ========================================================================
    {
        chequear("Ej01 - crearParticion (cada elemento en su grupo)", crearParticion(5) == Particion{0, 1, 2, 3, 4});
        chequear("Ej01 - crearParticion (n = 0)", crearParticion(0).empty());
        chequear("Ej01 - crearParticion (n = 1)", crearParticion(1) == Particion{0});
    }

    // ========================================================================
    // Ejercicio 2 — representante
    // ========================================================================
    {
        Particion p = {0, 0, 2, 0, 4, 4}; // grupos {0,1,3}, {2}, {4,5} (el ejemplo de la teórica)
        chequear("Ej02 - representante (el grupo {0, 1, 3})", representante(p, 0) == 0 && representante(p, 1) == 0 && representante(p, 3) == 0);
        chequear("Ej02 - representante (un grupo de un solo elemento)", representante(p, 2) == 2);
        chequear("Ej02 - representante (el grupo {4, 5})", representante(p, 4) == 4 && representante(p, 5) == 4);
        Particion q = {3, 3, 3, 3};
        chequear("Ej02 - representante (el representante no es el menor)", representante(q, 0) == 3 && representante(q, 3) == 3);
    }

    // ========================================================================
    // Ejercicio 3 — unir
    // ========================================================================
    {
        Particion p = {0, 0, 2, 0, 4, 4};
        unir(p, 1, 5);
        chequear("Ej03 - unir (la pregunta de la teórica: Unir(P, 1, 5))", p == Particion{0, 0, 2, 0, 0, 0});
        unir(p, 2, 5);
        chequear("Ej03 - unir (el representante resultante es el del grupo de x)", p == Particion{2, 2, 2, 2, 2, 2});

        Particion q = {0, 1, 2, 3};
        unir(q, 3, 0);
        chequear("Ej03 - unir (dos grupos de un elemento: gana el de x)", q == Particion{3, 1, 2, 3});
        unir(q, 1, 2);
        chequear("Ej03 - unir (los otros grupos no cambian)", q == Particion{3, 1, 1, 3});

        // Contra un modelo simple, mezclando uniones.
        bool alAzar = true;
        for (unsigned int s = 1; s <= 15; s++) {
            size_t n = 2 + s % 12;
            Particion r(n);
            std::vector<size_t> etiqueta(n);
            for (size_t i = 0; i < n; i++) {
                r[i] = i;
                etiqueta[i] = i;
            }
            unsigned int semilla = s;
            for (size_t k = 0; k < n; k++) {
                size_t x = siguiente(semilla) % n, y = siguiente(semilla) % n;
                if (etiqueta[x] == etiqueta[y]) continue;
                size_t viejo = etiqueta[y], nuevo = etiqueta[x];
                for (size_t i = 0; i < n; i++) {
                    if (etiqueta[i] == viejo) etiqueta[i] = nuevo;
                }
                unir(r, x, y);
            }
            if (r.size() != n) {
                alAzar = false;
                continue;
            }
            for (size_t i = 0; i < n; i++) {
                if (r[i] >= n || r[r[i]] != r[i]) alAzar = false; // el invariante
                for (size_t j = 0; j < n; j++)
                    if ((r[i] == r[j]) != (etiqueta[i] == etiqueta[j])) alAzar = false;
            }
        }
        chequear("Ej03 - unir (uniones al azar: los grupos son los correctos y vale el invariante)", alAzar);
    }

    // ========================================================================
    // Ejercicio 4 — aristasOrdenadasPorPeso
    // ========================================================================
    {
        chequear("Ej04 - aristasOrdenadasPorPeso (la teórica: B-C, D-E, A-C, E-F, D-F, B-D, A-B, C-D, C-E)",
                 aristasOrdenadasPorPeso(noDirigido(N_EJ, EJ)) ==
                     Aristas{{1, 2, 1}, {3, 4, 2}, {0, 2, 3}, {4, 5, 4}, {3, 5, 5}, {1, 3, 6}, {0, 1, 7}, {2, 3, 8}, {2, 4, 9}});
        chequear("Ej04 - aristasOrdenadasPorPeso (sin vértices)", aristasOrdenadasPorPeso(sinAristas(0)).empty());
        chequear("Ej04 - aristasOrdenadasPorPeso (sin aristas)", aristasOrdenadasPorPeso(sinAristas(5)).empty());
        chequear("Ej04 - aristasOrdenadasPorPeso (con un vértice aislado y un peso negativo)",
                 aristasOrdenadasPorPeso(noDirigido(4, Aristas{{0, 1, 4}, {1, 2, -2}})) == Aristas{{1, 2, -2}, {0, 1, 4}});

        bool unaVez = true, ordenado = true;
        for (unsigned int s = 1; s <= 25; s++) {
            size_t n = 1 + s % 12;
            Aristas ar = aristasConexasAlAzar(n, n, s, 5); // muchos pesos repetidos
            Aristas res = aristasOrdenadasPorPeso(noDirigido(n, ar));
            for (const Arista& a : res) {
                if (a.v >= a.w) unaVez = false;
            }
            if (ordenadas(res) != ordenadas(normalizadas(ar))) unaVez = false;
            for (size_t i = 0; i + 1 < res.size(); i++) {
                if (res[i].peso > res[i + 1].peso) ordenado = false;
            }
            if (res.size() != ar.size()) ordenado = false;
        }
        chequear("Ej04 - aristasOrdenadasPorPeso (grafos al azar: cada arista una sola vez, con v < w)", unaVez);
        chequear("Ej04 - aristasOrdenadasPorPeso (grafos al azar con pesos repetidos: de menor a mayor)", ordenado);
    }

    // ========================================================================
    // Ejercicio 5 — kruskal
    // ========================================================================
    probarKruskal("Ej05 - kruskal", kruskal);

    // ========================================================================
    // Ejercicio 6 — pesoDeAristas
    // ========================================================================
    {
        chequear("Ej06 - pesoDeAristas (el AGM de la teórica: 16)", pesoDeAristas(Aristas{{1, 2, 1}, {3, 4, 2}, {0, 2, 3}, {4, 5, 4}, {1, 3, 6}}) == 16);
        chequear("Ej06 - pesoDeAristas (el otro árbol generador de la teórica: 35)", pesoDeAristas(Aristas{{0, 1, 7}, {1, 3, 6}, {2, 3, 8}, {2, 4, 9}, {3, 5, 5}}) == 35);
        chequear("Ej06 - pesoDeAristas (sin aristas)", pesoDeAristas(Aristas{}) == 0);
        chequear("Ej06 - pesoDeAristas (con negativos)", pesoDeAristas(Aristas{{0, 1, -5}, {1, 2, 3}}) == -2);
    }

    titulo("PARTE 2 - PRIM");

    // ========================================================================
    // Ejercicio 7 — incorporar
    // ========================================================================
    {
        GrafoPonderado g = noDirigido(N_EJ, EJ);
        std::vector<bool> enArbol(N_EJ, false);
        ColaDeAristas cola;
        incorporar(g, 0, enArbol, cola);
        chequear("Ej07 - incorporar (A queda marcado, y sólo A)", enArbol == std::vector<bool>{true, false, false, false, false, false});
        ColaDeAristas copia = cola;
        chequear("Ej07 - incorporar (la teórica: desde A entran AC3 y AB7)", vaciar(copia) == Aristas{{0, 2, 3}, {0, 1, 7}});

        // Segundo paso de la tabla: sale AC3 y se incorpora C.
        if (!cola.empty()) cola.pop();
        incorporar(g, 2, enArbol, cola);
        chequear("Ej07 - incorporar (C queda marcado; A sigue marcado)", enArbol == std::vector<bool>{true, false, true, false, false, false});
        chequear("Ej07 - incorporar (la teórica: quedan CB1, AB7, CD8, CE9; no entra la arista hacia A)",
                 vaciar(cola) == Aristas{{2, 1, 1}, {0, 1, 7}, {2, 3, 8}, {2, 4, 9}});

        std::vector<bool> todos(N_EJ, true);
        todos[3] = false;
        ColaDeAristas vacia;
        incorporar(g, 3, todos, vacia);
        chequear("Ej07 - incorporar (todos los vecinos ya están en el árbol: no entra nada)", vacia.empty() && todos[3]);

        std::vector<bool> uno(1, false);
        ColaDeAristas c1;
        incorporar(sinAristas(1), 0, uno, c1);
        chequear("Ej07 - incorporar (un vértice sin vecinos)", uno[0] && c1.empty());
    }

    // ========================================================================
    // Ejercicio 8 — prim
    // ========================================================================
    {
        GrafoPonderado g = noDirigido(N_EJ, EJ);
        chequear("Ej08 - prim (la teórica desde A: A-C, C-B, B-D, D-E, E-F, en el orden de la tabla)",
                 prim(g, 0) == Aristas{{0, 2, 3}, {2, 1, 1}, {1, 3, 6}, {3, 4, 2}, {4, 5, 4}});
        bool desdeTodos = true;
        for (size_t r = 0; r < N_EJ; r++) {
            Aristas res = prim(g, r);
            if (!esArbolGeneradorReferencia(N_EJ, EJ, res) || pesoTotal(res) != 16 || !creceDesde(r, res)) desdeTodos = false;
        }
        chequear("Ej08 - prim (la teórica desde cualquier vértice: árbol generador de peso 16)", desdeTodos);
        chequear("Ej08 - prim (n = 1: ninguna arista)", prim(noDirigido(1, Aristas{}), 0).empty());
        chequear("Ej08 - prim (n = 2, desde el 1)", prim(noDirigido(2, Aristas{{0, 1, 5}}), 1) == Aristas{{1, 0, 5}});

        Aristas conNegativos = {{0, 1, -4}, {1, 2, 3}, {0, 2, -1}, {2, 3, 2}, {1, 3, -5}};
        Aristas rn = prim(noDirigido(4, conNegativos), 2);
        chequear("Ej08 - prim (con pesos negativos)",
                 esArbolGeneradorReferencia(4, conNegativos, rn) && pesoTotal(rn) == pesoAGMFuerzaBruta(4, conNegativos) && creceDesde(2, rn));

        bool chicos = true;
        for (unsigned int s = 1; s <= 40; s++) {
            size_t n = 1 + s % 8;
            Aristas ar = aristasConexasAlAzar(n, s % 7, s, 1 + static_cast<Peso>(s % 5));
            size_t raiz = s % n;
            Aristas r = prim(noDirigido(n, ar), raiz);
            if (!esArbolGeneradorReferencia(n, ar, r) || pesoTotal(r) != pesoAGMFuerzaBruta(n, ar) || !creceDesde(raiz, r)) chicos = false;
        }
        chequear("Ej08 - prim (grafos chicos al azar, contra fuerza bruta)", chicos);

        bool grandes = true;
        for (unsigned int s = 1; s <= 15; s++) {
            size_t n = 10 + s * 3;
            Aristas ar = aristasConexasAlAzar(n, n * 3, s, 30);
            Aristas r = prim(noDirigido(n, ar), s % n);
            if (!esArbolGeneradorReferencia(n, ar, r) || pesoTotal(r) != pesoAGMKruskal(n, ar) || !creceDesde(s % n, r)) grandes = false;
        }
        chequear("Ej08 - prim (grafos grandes al azar: el mismo peso que Kruskal)", grandes);
    }

    titulo("PARTE 3 - CAMINO MINIMO");

    // ========================================================================
    // Ejercicio 9 — mejorar
    // ========================================================================
    {
        Pesos teorica = {0, 3, 10}; // v, x, y: dist[x] = 3, dist[y] = 10, arista x -> y de peso 2
        chequear("Ej09 - mejorar (la teórica: dist[y] pasa de 10 a 5)", mejorar(1, 2, 2, teorica) && teorica[2] == 5);

        Pesos dist = {0, 10, INFINITO};
        chequear("Ej09 - mejorar (mejora una estimación: 0 + 5 < 10)", mejorar(0, 1, 5, dist) && dist[1] == 5);
        chequear("Ej09 - mejorar (no empeora: 0 + 7 no es menor que 5)", !mejorar(0, 1, 7, dist) && dist[1] == 5);
        chequear("Ej09 - mejorar (empate: no cuenta como mejora)", !mejorar(0, 1, 5, dist) && dist[1] == 5);
        chequear("Ej09 - mejorar (de INFINITO a un valor: 5 + 2)", mejorar(1, 2, 2, dist) && dist[2] == 7);
        chequear("Ej09 - mejorar (las demás posiciones no cambian)", dist[0] == 0 && dist[1] == 5);

        Pesos sinOrigen = {INFINITO, INFINITO, 4};
        chequear("Ej09 - mejorar (si dist[x] es INFINITO no cambia nada, ni se desborda)",
                 !mejorar(0, 1, 5, sinOrigen) && sinOrigen[1] == INFINITO);
        chequear("Ej09 - mejorar (dist[x] INFINITO con un peso negativo tampoco cambia nada)",
                 !mejorar(0, 2, -100, sinOrigen) && sinOrigen[2] == 4);

        Pesos neg = {0, 10};
        chequear("Ej09 - mejorar (con un peso negativo)", mejorar(0, 1, -3, neg) && neg[1] == -3);
    }

    // ========================================================================
    // Ejercicio 10 — dijkstra
    // ========================================================================
    {
        GrafoPonderado g = noDirigido(N_EJ, EJ);
        chequear("Ej10 - dijkstra (la teórica desde A: 0, 4, 3, 10, 12, 15)", dijkstra(g, 0) == Pesos{0, 4, 3, 10, 12, 15});
        chequear("Ej10 - dijkstra (la teórica desde F)", dijkstra(g, 5) == Pesos{15, 11, 12, 5, 4, 0});

        GrafoPonderado d = dirigido(4, Aristas{{0, 1, 2}, {1, 2, 3}, {0, 2, 10}});
        chequear("Ej10 - dijkstra (dirigido, con un vértice que no se alcanza)", dijkstra(d, 0) == Pesos{0, 2, 5, INFINITO});
        chequear("Ej10 - dijkstra (dirigido: desde 2 no se llega a nadie)", dijkstra(d, 2) == Pesos{INFINITO, INFINITO, 0, INFINITO});
        chequear("Ej10 - dijkstra (un solo vértice)", dijkstra(sinAristas(1), 0) == Pesos{0});
        chequear("Ej10 - dijkstra (aristas de peso 0)", dijkstra(noDirigido(3, Aristas{{0, 1, 0}, {1, 2, 0}}), 0) == Pesos{0, 0, 0});

        // Con todos los pesos en 1, es lo que calcula BFS: la cantidad de aristas.
        Aristas camino = {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 4, 1}};
        chequear("Ej10 - dijkstra (todos los pesos en 1: lo mismo que BFS)", dijkstra(noDirigido(5, camino), 0) == Pesos{0, 1, 2, 3, 4});

        bool noDir = true, dir = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 10;
            Aristas ar = n >= 2 ? aristasConexasAlAzar(n, s % 8, s, 9) : Aristas{};
            Pesos ref;
            bellmanFordReferencia(n, ambosSentidos(ar), 0, ref);
            if (dijkstra(noDirigido(n, ar), 0) != ref) noDir = false;

            Aristas arcos = arcosAlAzar(n, n * 2, s, 0, 9);
            bellmanFordReferencia(n, arcos, s % n, ref);
            if (dijkstra(dirigido(n, arcos), s % n) != ref) dir = false;
        }
        chequear("Ej10 - dijkstra (grafos no dirigidos al azar)", noDir);
        chequear("Ej10 - dijkstra (grafos dirigidos al azar, con pesos 0 y vértices inalcanzables)", dir);
    }

    // ========================================================================
    // Ejercicio 11 — dijkstraConPadres
    // ========================================================================
    {
        GrafoPonderado g = noDirigido(N_EJ, EJ);
        CaminosMinimos r = dijkstraConPadres(g, 0);
        chequear("Ej11 - dijkstraConPadres (las distancias de la teórica)", r.dist == Pesos{0, 4, 3, 10, 12, 15});
        chequear("Ej11 - dijkstraConPadres (los padres de la teórica: C viene de A, B de C, D de B, F de D)",
                 r.padre.size() == 6 && r.padre[0] == SIN_PADRE && r.padre[2] == 0 && r.padre[1] == 2 && r.padre[3] == 1 && r.padre[5] == 3);
        chequear("Ej11 - dijkstraConPadres (E viene de C, no de D: D-E empata y no mejora)", r.padre.size() == 6 && r.padre[4] == 2);

        GrafoPonderado d = dirigido(4, Aristas{{0, 1, 2}, {1, 2, 3}, {0, 2, 10}});
        CaminosMinimos rd = dijkstraConPadres(d, 0);
        chequear("Ej11 - dijkstraConPadres (el inalcanzable no tiene padre)", rd.padre.size() == 4 && rd.padre[3] == SIN_PADRE && rd.padre[0] == SIN_PADRE);
        chequear("Ej11 - dijkstraConPadres (la mejora reemplaza al padre viejo: 2 viene de 1, no de 0)",
                 rd.padre.size() == 4 && rd.padre[2] == 1 && rd.padre[1] == 0);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 2 + s % 10;
            Aristas arcos = arcosAlAzar(n, n * 2, s, 1, 9);
            size_t origen = s % n;
            Pesos ref;
            bellmanFordReferencia(n, arcos, origen, ref);
            CaminosMinimos res = dijkstraConPadres(dirigido(n, arcos), origen);
            if (res.dist != ref || res.padre.size() != n) {
                alAzar = false;
                continue;
            }
            std::map<std::pair<size_t, size_t>, Peso> pesos;
            for (const Arista& a : arcos) pesos[std::make_pair(a.v, a.w)] = a.peso;
            for (size_t y = 0; y < n; y++) {
                if (y == origen || ref[y] == INFINITO) {
                    if (res.padre[y] != SIN_PADRE) alAzar = false;
                    continue;
                }
                std::map<std::pair<size_t, size_t>, Peso>::const_iterator it =
                    res.padre[y] < n ? pesos.find(std::make_pair(res.padre[y], y)) : pesos.end();
                if (it == pesos.end() || ref[res.padre[y]] + it->second != ref[y]) alAzar = false;
            }
        }
        chequear("Ej11 - dijkstraConPadres (grafos al azar: cada padre es parte de un camino mínimo)", alAzar);
    }

    // ========================================================================
    // Ejercicio 12 — reconstruirCamino
    // ========================================================================
    {
        // Los padres de la teórica, desde A: A <- C <- B <- D <- F, y E viene de C.
        std::vector<size_t> padre = {SIN_PADRE, 2, 0, 1, 2, 3};
        chequear("Ej12 - reconstruirCamino (A a F de la teórica: A, C, B, D, F)", reconstruirCamino(padre, 0, 5) == Vertices{0, 2, 1, 3, 5});
        chequear("Ej12 - reconstruirCamino (A a E: A, C, E)", reconstruirCamino(padre, 0, 4) == Vertices{0, 2, 4});
        chequear("Ej12 - reconstruirCamino (A a C: un solo paso)", reconstruirCamino(padre, 0, 2) == Vertices{0, 2});
        chequear("Ej12 - reconstruirCamino (v == w: el camino de longitud 0)", reconstruirCamino(padre, 0, 0) == Vertices{0});

        std::vector<size_t> cadena = {1, 2, 3, SIN_PADRE}; // 3 <- 2 <- 1 <- 0, con origen 3
        chequear("Ej12 - reconstruirCamino (una cadena, el origen no es el 0)", reconstruirCamino(cadena, 3, 0) == Vertices{3, 2, 1, 0});
    }

    // ========================================================================
    // Ejercicio 13 — caminoMinimo
    // ========================================================================
    {
        GrafoPonderado g = noDirigido(N_EJ, EJ);
        Peso peso = 0;
        chequear("Ej13 - caminoMinimo (A a F de la teórica: A, C, B, D, F)", caminoMinimo(g, 0, 5) == Vertices{0, 2, 1, 3, 5});
        chequear("Ej13 - caminoMinimo (A a B: 4 por C, no 7 directo)", caminoMinimo(g, 0, 1) == Vertices{0, 2, 1});
        chequear("Ej13 - caminoMinimo (v == w)", caminoMinimo(g, 3, 3) == Vertices{3});

        GrafoPonderado d = dirigido(4, Aristas{{0, 1, 2}, {1, 2, 3}, {0, 2, 10}});
        chequear("Ej13 - caminoMinimo (dirigido: pasa por el 1)", caminoMinimo(d, 0, 2) == Vertices{0, 1, 2});
        // Estas dos pasan con la cáscara vacía: no te confíes.
        chequear("Ej13 - caminoMinimo (no alcanzable: vacío)", caminoMinimo(d, 0, 3).empty());
        chequear("Ej13 - caminoMinimo (no alcanzable por el sentido de las aristas: vacío)", caminoMinimo(d, 2, 0).empty());

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 2 + s % 10;
            Aristas arcos = arcosAlAzar(n, n * 2, s, 1, 9);
            size_t origen = s % n;
            Pesos ref;
            bellmanFordReferencia(n, arcos, origen, ref);
            GrafoPonderado r = dirigido(n, arcos);
            for (size_t w = 0; w < n; w++) {
                Vertices camino = caminoMinimo(r, origen, w);
                if (ref[w] == INFINITO) {
                    if (!camino.empty()) alAzar = false;
                } else if (!esCaminoReferencia(arcos, camino, origen, w, peso) || peso != ref[w]) {
                    alAzar = false;
                }
            }
        }
        chequear("Ej13 - caminoMinimo (grafos al azar: camino válido y de peso mínimo, o vacío)", alAzar);
    }

    // ========================================================================
    // Ejercicio 14 — bellmanFord
    // ========================================================================
    {
        // El ejemplo de la teórica: A=0, B=1, C=2, D=3, E=4.
        Aristas ejemplo = {{0, 1, 6}, {0, 2, 4}, {2, 1, -2}, {1, 3, 3}, {2, 4, 9}, {3, 4, 2}};
        ResultadoBellmanFord r = bellmanFord(dirigido(5, ejemplo), 0);
        chequear("Ej14 - bellmanFord (la teórica, con un peso negativo: sin ciclo negativo)", r.sinCicloNegativo);
        chequear("Ej14 - bellmanFord (la teórica: 0, 2, 4, 5, 7)", r.sinCicloNegativo && r.dist == Pesos{0, 2, 4, 5, 7});

        // Donde Dijkstra se equivoca (la teórica): A -> B de 2, A -> C de 3, C -> B de -2.
        ResultadoBellmanFord rn = bellmanFord(dirigido(3, Aristas{{0, 1, 2}, {0, 2, 3}, {2, 1, -2}}), 0);
        chequear("Ej14 - bellmanFord (el ejemplo donde Dijkstra falla: de A a B es 1, no 2)", rn.sinCicloNegativo && rn.dist == Pesos{0, 1, 3});

        // El ciclo negativo de la teórica: B -> C -> D -> B pesa 2 - 4 + 1 = -1.
        Aristas ciclo = {{0, 1, 1}, {1, 2, 2}, {2, 3, -4}, {3, 1, 1}, {2, 4, 3}};
        chequear("Ej14 - bellmanFord (la teórica: hay un ciclo negativo alcanzable -> false)", !bellmanFord(dirigido(5, ciclo), 0).sinCicloNegativo);

        // El mismo grafo, pero desde un vértice que no llega al ciclo.
        ResultadoBellmanFord desdeE = bellmanFord(dirigido(5, ciclo), 4);
        chequear("Ej14 - bellmanFord (el ciclo negativo no es alcanzable desde E -> true)", desdeE.sinCicloNegativo && desdeE.dist == Pesos{INFINITO, INFINITO, INFINITO, INFINITO, 0});

        Aristas inalcanzable = {{0, 1, 5}, {2, 3, -3}, {3, 2, 1}}; // el ciclo 2 <-> 3 pesa -2, pero desde 0 no se llega
        ResultadoBellmanFord ri = bellmanFord(dirigido(4, inalcanzable), 0);
        chequear("Ej14 - bellmanFord (un ciclo negativo en otra componente no cuenta)", ri.sinCicloNegativo && ri.dist == Pesos{0, 5, INFINITO, INFINITO});

        Aristas idaYVuelta = {{0, 1, -1}, {1, 0, -1}};
        chequear("Ej14 - bellmanFord (ir y volver por pesos negativos es un ciclo negativo)", !bellmanFord(dirigido(2, idaYVuelta), 0).sinCicloNegativo);

        ResultadoBellmanFord uno = bellmanFord(sinAristas(1), 0);
        chequear("Ej14 - bellmanFord (un solo vértice)", uno.sinCicloNegativo && uno.dist == Pesos{0});

        Aristas ceroCiclo = {{0, 1, 3}, {1, 2, -3}, {2, 1, 3}};
        chequear("Ej14 - bellmanFord (un ciclo de peso 0 no es negativo)", bellmanFord(dirigido(3, ceroCiclo), 0).sinCicloNegativo);

        bool sinNeg = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 9;
            Aristas arcos = arcosAlAzar(n, n * 2, s, 0, 9);
            Pesos ref;
            bellmanFordReferencia(n, arcos, 0, ref);
            ResultadoBellmanFord res = bellmanFord(dirigido(n, arcos), 0);
            if (!res.sinCicloNegativo || res.dist != ref) sinNeg = false;
        }
        chequear("Ej14 - bellmanFord (grafos al azar con pesos >= 0: coincide con Dijkstra)", sinNeg);

        bool alAzar = true;
        size_t conCiclo = 0, sinCiclo = 0;
        for (unsigned int s = 1; s <= 60; s++) {
            size_t n = 2 + s % 8;
            Aristas arcos = arcosAlAzar(n, n * 2, s, -3, 9);
            size_t origen = s % n;
            Pesos ref;
            bool esperado = bellmanFordReferencia(n, arcos, origen, ref);
            ResultadoBellmanFord res = bellmanFord(dirigido(n, arcos), origen);
            if (esperado) sinCiclo++; else conCiclo++;
            if (res.sinCicloNegativo != esperado) alAzar = false;
            if (esperado && res.dist != ref) alAzar = false;
        }
        chequear("Ej14 - bellmanFord (grafos al azar con pesos negativos: ciclo o no, y distancias)", alAzar && conCiclo > 0 && sinCiclo > 0);
    }

    // Las pruebas obligatorias llegan hasta acá.
    int totalObligatorias = totalPruebas;
    int okObligatorias = pruebasOk;

    titulo("PARTE 4 - OPCIONALES");

    // ========================================================================
    // Ejercicio 15 (opcional) — crearBosque
    // ========================================================================
    {
        Bosque b = crearBosque(4);
        chequear("Ej15 (opcional) - crearBosque (cada elemento es su propia raíz)", b.padre == Vertices{0, 1, 2, 3});
        chequear("Ej15 (opcional) - crearBosque (todos los tamaños en 1)", b.tam == Vertices{1, 1, 1, 1});
        Bosque v = crearBosque(0);
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej15 (opcional) - crearBosque (n = 0)", v.padre.empty() && v.tam.empty());
    }

    // ========================================================================
    // Ejercicio 16 (opcional) — representanteBosque
    // ========================================================================
    {
        Bosque b;
        b.padre = {0, 0, 1, 2, 4, 4}; // 3 -> 2 -> 1 -> 0 ; 5 -> 4
        b.tam = {4, 3, 2, 1, 2, 1};
        chequear("Ej16 (opcional) - representanteBosque (una raíz es su representante)", representanteBosque(b, 0) == 0 && representanteBosque(b, 4) == 4);
        chequear("Ej16 (opcional) - representanteBosque (un hijo directo)", representanteBosque(b, 1) == 0 && representanteBosque(b, 5) == 4);
        chequear("Ej16 (opcional) - representanteBosque (sube varios niveles)", representanteBosque(b, 3) == 0 && representanteBosque(b, 2) == 0);
    }

    // ========================================================================
    // Ejercicio 17 (opcional) — unirBosque
    // ========================================================================
    {
        Bosque b = bosqueSuelto(4);
        unirBosque(b, 0, 1);
        chequear("Ej17 (opcional) - unirBosque (empate: la raíz de y cuelga de la de x)", b.padre[1] == 0 && b.padre[0] == 0 && b.tam[0] == 2);
        unirBosque(b, 2, 0);
        chequear("Ej17 (opcional) - unirBosque (el árbol chico cuelga del grande, aunque sea x)", b.padre[2] == 0 && b.tam[0] == 3);
        unirBosque(b, 3, 1);
        chequear("Ej17 (opcional) - unirBosque (se unen las raíces, no los elementos)", b.padre[3] == 0 && b.tam[0] == 4 && b.padre[0] == 0);
        chequear("Ej17 (opcional) - unirBosque (los de arriba no se tocan)", b.padre[1] == 0 && b.padre[2] == 0 && b.tam[0] == 4);

        // Uniones al azar: los grupos son los correctos y los árboles no se
        // vuelven altos (a lo sumo log2 n).
        bool grupos = true, bajo = true;
        size_t uniones = 0;
        for (unsigned int s = 1; s <= 10; s++) {
            size_t n = 64;
            Bosque r = bosqueSuelto(n);
            std::vector<size_t> etiqueta(n);
            for (size_t i = 0; i < n; i++) etiqueta[i] = i;
            unsigned int semilla = s;
            for (size_t k = 0; k < 4 * n; k++) {
                size_t x = siguiente(semilla) % n, y = siguiente(semilla) % n;
                if (etiqueta[x] == etiqueta[y]) continue;
                size_t viejo = etiqueta[y], nuevo = etiqueta[x];
                for (size_t i = 0; i < n; i++) {
                    if (etiqueta[i] == viejo) etiqueta[i] = nuevo;
                }
                unirBosque(r, x, y);
                uniones++;
            }
            for (size_t i = 0; i < n; i++) {
                if (alturaDe(r, i) > 6) bajo = false;
                for (size_t j = 0; j < n; j++) {
                    // la raíz de cada uno, calculada acá
                    size_t ri = i, rj = j;
                    while (r.padre[ri] != ri) ri = r.padre[ri];
                    while (r.padre[rj] != rj) rj = r.padre[rj];
                    if ((ri == rj) != (etiqueta[i] == etiqueta[j])) grupos = false;
                }
            }
        }
        chequear("Ej17 (opcional) - unirBosque (uniones al azar: los grupos son los correctos)", grupos);
        chequear("Ej17 (opcional) - unirBosque (uniones al azar: la altura de los árboles es <= log2 n)", grupos && bajo && uniones > 0);
    }

    // ========================================================================
    // Ejercicio 18 (opcional) — kruskalConBosque
    // ========================================================================
    probarKruskal("Ej18 (opcional) - kruskalConBosque", kruskalConBosque);

    // ========================================================================
    // Ejercicio 19 (opcional) — aristaMasLivianaDelCorte
    // ========================================================================
    {
        GrafoPonderado g = noDirigido(N_EJ, EJ);
        // El corte de la teórica: {A, B, C} contra {D, E, F}. Cruzan B-D (6), C-D (8) y C-E (9).
        std::vector<bool> abc = {true, true, true, false, false, false};
        chequear("Ej19 (opcional) - aristaMasLivianaDelCorte (la teórica, {A,B,C} contra {D,E,F}: B-D)", aristaMasLivianaDelCorte(g, abc) == (Arista{1, 3, 6}));
        std::vector<bool> soloA = {true, false, false, false, false, false};
        chequear("Ej19 (opcional) - aristaMasLivianaDelCorte ({A} contra el resto: A-C)", aristaMasLivianaDelCorte(g, soloA) == (Arista{0, 2, 3}));
        std::vector<bool> soloF = {false, false, false, false, false, true};
        chequear("Ej19 (opcional) - aristaMasLivianaDelCorte ({F} contra el resto: respeta el lado A)", aristaMasLivianaDelCorte(g, soloF) == (Arista{5, 4, 4}));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 2 + s % 9;
            Aristas ar = aristasConexasAlAzar(n, s % 7, s, 6);
            GrafoPonderado r = noDirigido(n, ar);
            std::vector<bool> enA(n);
            unsigned int semilla = s * 7;
            for (size_t i = 0; i < n; i++) enA[i] = siguiente(semilla) % 2 == 0;
            enA[0] = true;
            enA[n - 1] = false;
            bool hay = false;
            Peso minimo = 0;
            for (const Arista& a : ar) {
                if (enA[a.v] != enA[a.w] && (!hay || a.peso < minimo)) {
                    minimo = a.peso;
                    hay = true;
                }
            }
            Arista res = aristaMasLivianaDelCorte(r, enA);
            bool existe = false;
            for (const Arista& a : ar) {
                if (normalizada(a) == normalizada(res)) existe = true;
            }
            if (!existe || res.v >= n || res.w >= n || !enA[res.v] || enA[res.w] || res.peso != minimo) alAzar = false;
        }
        chequear("Ej19 (opcional) - aristaMasLivianaDelCorte (grafos y cortes al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 20 (opcional) — vueltoGreedy
    // ========================================================================
    {
        std::vector<size_t> usd = {1, 5, 10, 25};
        chequear("Ej20 (opcional) - vueltoGreedy (monedas 1, 5, 10, 25; 63 -> 6)", vueltoGreedy(usd, 63) == 6);
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej20 (opcional) - vueltoGreedy (monto 0 -> 0 monedas)", vueltoGreedy(usd, 0) == 0);
        chequear("Ej20 (opcional) - vueltoGreedy (una moneda justa)", vueltoGreedy(usd, 25) == 1);
        chequear("Ej20 (opcional) - vueltoGreedy (las monedas en cualquier orden)", vueltoGreedy(std::vector<size_t>{25, 1, 10, 5}, 63) == 6);

        std::vector<size_t> raras = {1, 3, 4};
        chequear("Ej20 (opcional) - vueltoGreedy (la teórica: monedas 1, 3, 4; 6 -> 3: 4 + 1 + 1, aunque 3 + 3 es mejor)", vueltoGreedy(raras, 6) == 3);
        chequear("Ej20 (opcional) - vueltoGreedy (el greedy no es óptimo con 1, 3, 4)", vueltoOptimo(raras, 6) == 2 && vueltoGreedy(raras, 6) > vueltoOptimo(raras, 6));

        chequear("Ej20 (opcional) - vueltoGreedy (sin moneda que entre -> IMPOSIBLE)", vueltoGreedy(std::vector<size_t>{5, 10}, 3) == IMPOSIBLE);
        chequear("Ej20 (opcional) - vueltoGreedy (el greedy se queda trabado: 5, 2 para dar 6 -> IMPOSIBLE)", vueltoGreedy(std::vector<size_t>{5, 2}, 6) == IMPOSIBLE);

        bool optimo = true;
        for (size_t monto = 0; monto <= 100; monto++) {
            if (vueltoGreedy(usd, monto) != vueltoOptimo(usd, monto)) optimo = false;
        }
        chequear("Ej20 (opcional) - vueltoGreedy (con 1, 5, 10, 25 coincide con el óptimo de 0 a 100)", optimo);
    }

    // ========================================================================
    // Ejercicio 21 (opcional) — rondasQueMejoran
    // ========================================================================
    {
        Aristas ejemplo = {{0, 1, 6}, {0, 2, 4}, {2, 1, -2}, {1, 3, 3}, {2, 4, 9}, {3, 4, 2}};
        chequear("Ej21 (opcional) - rondasQueMejoran (la teórica, con las aristas por origen creciente: 2 rondas)", rondasQueMejoran(dirigido(5, ejemplo), 0) == 2);

        Aristas adelante = {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 4, 1}};
        chequear("Ej21 (opcional) - rondasQueMejoran (camino hacia adelante: 1 ronda alcanza)", rondasQueMejoran(dirigido(5, adelante), 0) == 1);

        Aristas atras = {{4, 3, 1}, {3, 2, 1}, {2, 1, 1}, {1, 0, 1}};
        chequear("Ej21 (opcional) - rondasQueMejoran (camino con las aristas al revés: n - 1 rondas)", rondasQueMejoran(dirigido(5, atras), 4) == 4);

        // Estas dos pasan con la cáscara vacía: no te confíes.
        chequear("Ej21 (opcional) - rondasQueMejoran (sin aristas desde el origen: ninguna ronda mejora)", rondasQueMejoran(sinAristas(4), 0) == 0);
        chequear("Ej21 (opcional) - rondasQueMejoran (un solo vértice)", rondasQueMejoran(sinAristas(1), 0) == 0);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 2 + s % 9;
            Aristas arcos = arcosAlAzar(n, n * 2, s, 0, 9);
            size_t origen = s % n;
            GrafoPonderado r = dirigido(n, arcos);
            // Los arcos en el orden de las listas: por origen y, dentro, por destino.
            if (rondasQueMejoran(r, origen) != rondasReferencia(n, ordenadas(arcos), origen)) alAzar = false;
        }
        chequear("Ej21 (opcional) - rondasQueMejoran (grafos al azar)", alAzar);
    }

    // ========================================================================
    // Resumen
    // ========================================================================
    std::cout << "\nResultado (Partes 1 a 3): " << okObligatorias << "/" << totalObligatorias
              << " pruebas pasadas." << std::endl;
    std::cout << "Opcionales (Parte 4):     " << (pruebasOk - okObligatorias) << "/"
              << (totalPruebas - totalObligatorias) << " pruebas pasadas." << std::endl;

    if (pruebasOk == totalPruebas)
    {
        std::cout << "\nPara verificar que no hay memory leaks, corré: make valgrind" << std::endl;
    }

    return 0;
}
