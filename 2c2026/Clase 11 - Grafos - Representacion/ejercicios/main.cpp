#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
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
// Las pruebas NO usan tus funciones para armar ni inspeccionar los grafos
// (usan los helpers de abajo, que escriben directo en g.n y g.ady), así que
// un error en una función no hace fallar las pruebas de las demás.
//
// Si el programa se queda colgado en la Parte 3 o la 4, casi seguro tu DFS no está
// marcando los vértices como visitados (y da vueltas por un ciclo).
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
// Helpers: armado de grafos (sin usar las funciones del TAD del alumno)
// ----------------------------------------------------------------------------

typedef std::pair<size_t, size_t> Arista;
typedef std::vector<size_t> Vertices;

static Vertices ordenado(Vertices v)
{
    std::sort(v.begin(), v.end());
    return v;
}

// Los vecinos de v según una lista de aristas, ordenados de menor a mayor.
static Vertices vecinosEsperados(const std::vector<Arista>& aristas, size_t v)
{
    Vertices res;
    for (const Arista& e : aristas) {
        if (e.first == v) res.push_back(e.second);
        else if (e.second == v) res.push_back(e.first);
    }
    std::sort(res.begin(), res.end());
    return res;
}

// Grafo con n vértices y esas aristas, como listas de adyacencia. Cada lista
// queda ordenada de menor a mayor (importa para el orden del DFS).
static Grafo listasDe(size_t n, const std::vector<Arista>& aristas)
{
    Grafo g;
    g.n = n;
    g.ady = std::vector<std::vector<size_t>>(n);
    for (const Arista& e : aristas) {
        g.ady[e.first].push_back(e.second);
        g.ady[e.second].push_back(e.first);
    }
    for (size_t v = 0; v < n; v++) std::sort(g.ady[v].begin(), g.ady[v].end());
    return g;
}

// ¿Las listas representan exactamente ese grafo? (sin importar el orden de
// los vecinos dentro de cada lista, pero sin repetidos)
static bool listasSon(const Grafo& g, size_t n, const std::vector<Arista>& aristas)
{
    if (g.n != n || g.ady.size() != n) return false;
    for (size_t v = 0; v < n; v++) {
        if (ordenado(g.ady[v]) != vecinosEsperados(aristas, v)) return false;
    }
    return true;
}

// Familias de grafos
static std::vector<Arista> aristasCompleto(size_t n)
{
    std::vector<Arista> res;
    for (size_t i = 0; i < n; i++)
        for (size_t j = i + 1; j < n; j++) res.push_back(Arista(i, j));
    return res;
}

static std::vector<Arista> aristasCamino(size_t n)   // 0-1-2-...-(n-1)
{
    std::vector<Arista> res;
    for (size_t i = 0; i + 1 < n; i++) res.push_back(Arista(i, i + 1));
    return res;
}

static std::vector<Arista> aristasCiclo(size_t n)    // el camino más la arista (n-1, 0); n >= 3
{
    std::vector<Arista> res = aristasCamino(n);
    res.push_back(Arista(0, n - 1));
    return res;
}

static std::vector<Arista> aristasEstrella(size_t n) // el centro es el 0
{
    std::vector<Arista> res;
    for (size_t i = 1; i < n; i++) res.push_back(Arista(0, i));
    return res;
}

// Números pseudo-aleatorios, siempre los mismos para la misma semilla.
static unsigned int siguiente(unsigned int& semilla)
{
    semilla = semilla * 1103515245u + 12345u;
    return semilla >> 8;
}

// Hasta `intentos` aristas al azar entre n vértices (sin bucles ni repetidas).
static std::vector<Arista> aristasAleatorias(size_t n, size_t intentos, unsigned int semilla)
{
    std::vector<Arista> res;
    std::set<Arista> usadas;
    for (size_t i = 0; i < intentos; i++) {
        size_t a = siguiente(semilla) % n;
        size_t b = siguiente(semilla) % n;
        if (a == b) continue;
        Arista e(std::min(a, b), std::max(a, b));
        if (usadas.insert(e).second) res.push_back(e);
    }
    return res;
}

// Un árbol al azar: cada vértice i >= 1 se conecta con algún j < i.
static std::vector<Arista> aristasArbolAleatorio(size_t n, unsigned int semilla)
{
    std::vector<Arista> res;
    for (size_t i = 1; i < n; i++) res.push_back(Arista(siguiente(semilla) % i, i));
    return res;
}

// ----------------------------------------------------------------------------
// Helpers: respuestas de referencia (sin DFS), para comparar en grafos al azar
// ----------------------------------------------------------------------------

struct UnionFind {
    std::vector<size_t> padre;
    size_t componentes;

    explicit UnionFind(size_t n) : padre(n), componentes(n)
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
    // Devuelve false si ya estaban en la misma componente (la arista cierra un ciclo).
    bool unir(size_t a, size_t b)
    {
        size_t ra = buscar(a), rb = buscar(b);
        if (ra == rb) return false;
        padre[ra] = rb;
        componentes--;
        return true;
    }
};

static UnionFind componentesDe(size_t n, const std::vector<Arista>& aristas)
{
    UnionFind uf(n);
    for (const Arista& e : aristas) uf.unir(e.first, e.second);
    return uf;
}

// ¿Es un árbol? Ninguna arista cierra un ciclo y hay una sola componente.
static bool esArbolReferencia(size_t n, const std::vector<Arista>& aristas)
{
    UnionFind uf(n);
    for (const Arista& e : aristas) {
        if (!uf.unir(e.first, e.second)) return false;
    }
    return uf.componentes == 1;
}

// ¿Es bipartito? 2-coloreo con una cola (BFS), desde cada vértice sin pintar.
static bool esBipartitoReferencia(size_t n, const std::vector<Arista>& aristas)
{
    std::vector<int> color(n, -1);
    for (size_t s = 0; s < n; s++) {
        if (color[s] != -1) continue;
        color[s] = 0;
        std::queue<size_t> cola;
        cola.push(s);
        while (!cola.empty()) {
            size_t x = cola.front();
            cola.pop();
            for (size_t y : vecinosEsperados(aristas, x)) {
                if (color[y] == -1) {
                    color[y] = 1 - color[x];
                    cola.push(y);
                } else if (color[y] == color[x]) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Distancias mínimas desde v, calculadas SIN cola ni pila: se relajan todas las
// aristas n veces (como Bellman-Ford). Es lento, pero es otra forma de llegar
// al mismo resultado.
static std::vector<size_t> distanciasReferencia(size_t n, const std::vector<Arista>& aristas, size_t v)
{
    std::vector<size_t> dist(n, NO_ALCANZABLE);
    dist[v] = 0;
    for (size_t vuelta = 0; vuelta < n; vuelta++) {
        for (const Arista& e : aristas) {
            if (dist[e.first] != NO_ALCANZABLE && dist[e.first] + 1 < dist[e.second]) dist[e.second] = dist[e.first] + 1;
            if (dist[e.second] != NO_ALCANZABLE && dist[e.second] + 1 < dist[e.first]) dist[e.first] = dist[e.second] + 1;
        }
    }
    return dist;
}

int main()
{
    // Grafo de la teórica (clase 17).
    //     v0 --- v1
    //     |
    //     v3 --- v2
    const size_t N_TEORICA = 4;
    const std::vector<Arista> TEORICA = {{0, 1}, {0, 3}, {2, 3}};

    // Grafo del ejemplo de DFS de la teórica (clase 17):
    //     0 --- 1 --- 2 --- 3
    //     |            |
    //     |            4
    //     |            |
    //     5 ---------- 6 --- 7
    const size_t N_DFS = 8;
    const std::vector<Arista> DFS = {{0, 1}, {1, 2}, {2, 3}, {2, 4}, {4, 6}, {0, 5}, {5, 6}, {6, 7}};

    // Grafo de ejemplo de la teórica (clase 16), con a, ..., f = 0, ..., 5:
    //     a --- b --- c --- d        tiene el triángulo a-b-e: no es bipartito.
    //      \   / \   /
    //       \ /   \ /
    //        e --- f
    const size_t N_EJEMPLO = 6;
    const std::vector<Arista> EJEMPLO = {{0, 1}, {0, 4}, {1, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 5}, {4, 5}};

    // ========================================================================
    // Ejercicio 1 — grafoSinAristas
    // ========================================================================
    {
        chequear("Ej1 - grafoSinAristas (4 vértices, sin aristas)", listasSon(grafoSinAristas(4), 4, {}));
        chequear("Ej1 - grafoSinAristas (1 vértice)", listasSon(grafoSinAristas(1), 1, {}));
        chequear("Ej1 - grafoSinAristas (0 vértices)", listasSon(grafoSinAristas(0), 0, {}));
        chequear("Ej1 - grafoSinAristas (1000 vértices)", listasSon(grafoSinAristas(1000), 1000, {}));
    }

    // ========================================================================
    // Ejercicio 2 — cantidadDeVertices 
    // ========================================================================
    {
        chequear("Ej2 - cantidadDeVertices (sin aristas)", cantidadDeVertices(listasDe(5, {})) == 5);
        chequear("Ej2 - cantidadDeVertices (con aristas)", cantidadDeVertices(listasDe(N_DFS, DFS)) == 8);
        chequear("Ej2 - cantidadDeVertices (vacío)", cantidadDeVertices(listasDe(0, {})) == 0);
    }

    // ========================================================================
    // Ejercicio 3 — agregarArista 
    // ========================================================================
    {
        Grafo g = listasDe(4, {});
        agregarArista(g, 0, 1);
        chequear("Ej3 - agregarArista (una arista, en las dos listas)", listasSon(g, 4, {{0, 1}}));

        agregarArista(g, 3, 0);
        agregarArista(g, 2, 3);
        chequear("Ej3 - agregarArista (el grafo de la teórica)", listasSon(g, N_TEORICA, TEORICA));

        size_t total = 0;
        for (size_t v = 0; v < g.n; v++) total += g.ady[v].size();
        chequear("Ej3 - agregarArista (cada arista aparece dos veces: 2m elementos)", total == 2 * TEORICA.size());

        Grafo k = listasDe(6, {});
        for (const Arista& e : aristasCompleto(6)) agregarArista(k, e.second, e.first);
        chequear("Ej3 - agregarArista (K6, agregando con v > w)", listasSon(k, 6, aristasCompleto(6)));
    }

    // ========================================================================
    // Ejercicio 4 — sonVecinos 
    // ========================================================================
    {
        Grafo g = listasDe(N_TEORICA, TEORICA);
        chequear("Ej4 - sonVecinos (hay arista, en los dos sentidos)",
                 sonVecinos(g, 0, 1) && sonVecinos(g, 1, 0) && sonVecinos(g, 2, 3) && sonVecinos(g, 3, 2));
        chequear("Ej4 - sonVecinos (no hay arista)",
                 !sonVecinos(g, 0, 2) && !sonVecinos(g, 1, 3) && !sonVecinos(g, 1, 2) && !sonVecinos(g, 2, 1));

        bool todos = true;
        for (unsigned int s = 1; s <= 20; s++) {
            size_t n = 2 + s % 10;
            std::vector<Arista> ar = aristasAleatorias(n, n * 2, s);
            Grafo r = listasDe(n, ar);
            std::set<Arista> conj(ar.begin(), ar.end());
            for (size_t v = 0; v < n; v++) {
                for (size_t w = 0; w < n; w++) {
                    if (v == w) continue;
                    bool esperado = conj.count(Arista(std::min(v, w), std::max(v, w))) > 0;
                    if (sonVecinos(r, v, w) != esperado) todos = false;
                }
            }
        }
        chequear("Ej4 - sonVecinos (grafos al azar, todos los pares)", todos);
    }

    // ========================================================================
    // Ejercicio 5 — vecinos 
    // ========================================================================
    {
        Grafo g = listasDe(N_TEORICA, TEORICA);
        chequear("Ej5 - vecinos (v0 -> {1, 3})", ordenado(vecinos(g, 0)) == Vertices({1, 3}));
        chequear("Ej5 - vecinos (v3 -> {0, 2})", ordenado(vecinos(g, 3)) == Vertices({0, 2}));
        chequear("Ej5 - vecinos (vértice aislado)", vecinos(listasDe(3, {}), 1).empty());

        Grafo e = listasDe(N_EJEMPLO, EJEMPLO);
        bool todos = true;
        for (size_t v = 0; v < N_EJEMPLO; v++) {
            if (ordenado(vecinos(e, v)) != vecinosEsperados(EJEMPLO, v)) todos = false;
        }
        chequear("Ej5 - vecinos (todos los vértices del grafo de ejemplo)", todos);

        chequear("Ej5 - vecinos (devuelve la lista de g, sin copiarla)", &vecinos(g, 3) == &g.ady[3]);
    }

    // ========================================================================
    // Ejercicio 6 — grado 
    // ========================================================================
    {
        Grafo g = listasDe(N_TEORICA, TEORICA);
        chequear("Ej6 - grado (el grafo de la teórica)",
                 grado(g, 0) == 2 && grado(g, 1) == 1 && grado(g, 2) == 1 && grado(g, 3) == 2);
        chequear("Ej6 - grado (vértice aislado)", grado(listasDe(3, {}), 2) == 0);

        Grafo e = listasDe(N_EJEMPLO, EJEMPLO);
        chequear("Ej6 - grado (el grafo de ejemplo de la clase 16)",
                 grado(e, 0) == 2 && grado(e, 1) == 4 && grado(e, 2) == 3 && grado(e, 3) == 1 &&
                 grado(e, 4) == 3 && grado(e, 5) == 3);
    }

    // ========================================================================
    // Ejercicio 7 — cantidadDeAristas
    // ========================================================================
    {
        chequear("Ej7 - cantidadDeAristas (sin aristas)", cantidadDeAristas(listasDe(5, {})) == 0);
        chequear("Ej7 - cantidadDeAristas (sin vértices)", cantidadDeAristas(listasDe(0, {})) == 0);
        chequear("Ej7 - cantidadDeAristas (el grafo de la teórica)", cantidadDeAristas(listasDe(N_TEORICA, TEORICA)) == 3);
        chequear("Ej7 - cantidadDeAristas (el grafo de ejemplo)", cantidadDeAristas(listasDe(N_EJEMPLO, EJEMPLO)) == 8);
        chequear("Ej7 - cantidadDeAristas (K10 tiene 45)", cantidadDeAristas(listasDe(10, aristasCompleto(10))) == 45);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 25; s++) {
            size_t n = 1 + s % 14;
            std::vector<Arista> ar = aristasAleatorias(n, n * 3, s);
            if (cantidadDeAristas(listasDe(n, ar)) != ar.size()) alAzar = false;
        }
        chequear("Ej7 - cantidadDeAristas (grafos al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 8 — esCompleto
    // ========================================================================
    {
        chequear("Ej8 - esCompleto (K5)", esCompleto(listasDe(5, aristasCompleto(5))));
        chequear("Ej8 - esCompleto (K2)", esCompleto(listasDe(2, aristasCompleto(2))));
        chequear("Ej8 - esCompleto (1 vértice)", esCompleto(listasDe(1, {})));
        chequear("Ej8 - esCompleto (0 vértices)", esCompleto(listasDe(0, {})));

        std::vector<Arista> casi = aristasCompleto(5);
        casi.pop_back();
        chequear("Ej8 - esCompleto (K5 sin una arista -> no)", !esCompleto(listasDe(5, casi)));
        chequear("Ej8 - esCompleto (camino de 4 vértices -> no)", !esCompleto(listasDe(4, aristasCamino(4))));
        chequear("Ej8 - esCompleto (2 vértices sin arista -> no)", !esCompleto(listasDe(2, {})));
        chequear("Ej8 - esCompleto (K4 más un vértice aislado -> no)", !esCompleto(listasDe(5, aristasCompleto(4))));
    }

    // ========================================================================
    // Ejercicio 9 — vecinosEnComun
    // ========================================================================
    {
        Grafo g = listasDe(4, {{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}});
        chequear("Ej9 - vecinosEnComun (ejemplo del enunciado: 1 y 2 -> {0, 3})",
                 vecinosEnComun(g, 1, 2) == Vertices({0, 3}));
        chequear("Ej9 - vecinosEnComun (se puede pedir en cualquier orden)",
                 vecinosEnComun(g, 2, 1) == Vertices({0, 3}));
        chequear("Ej9 - vecinosEnComun (0 y 3 -> {1, 2})", vecinosEnComun(g, 0, 3) == Vertices({1, 2}));
        Grafo t = listasDe(N_TEORICA, TEORICA);
        chequear("Ej9 - vecinosEnComun (0 y 2 comparten sólo al 3)", vecinosEnComun(t, 0, 2) == Vertices({3}));
        chequear("Ej9 - vecinosEnComun (sin vecinos en común -> vacío)", vecinosEnComun(t, 1, 2).empty());
        chequear("Ej9 - vecinosEnComun (triángulo: v y w no aparecen en el resultado)",
                 vecinosEnComun(listasDe(3, aristasCompleto(3)), 0, 1) == Vertices({2}));
        chequear("Ej9 - vecinosEnComun (dos hojas de una estrella -> el centro)",
                 vecinosEnComun(listasDe(6, aristasEstrella(6)), 2, 5) == Vertices({0}));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 25; s++) {
            size_t n = 3 + s % 10;
            std::vector<Arista> ar = aristasAleatorias(n, n * 3, s);
            Grafo r = listasDe(n, ar);
            for (size_t v = 0; v < n; v++) {
                for (size_t w = 0; w < n; w++) {
                    if (v == w) continue;
                    Vertices esperado;
                    Vertices deV = vecinosEsperados(ar, v);
                    Vertices deW = vecinosEsperados(ar, w);
                    std::set_intersection(deV.begin(), deV.end(), deW.begin(), deW.end(), std::back_inserter(esperado));
                    if (vecinosEnComun(r, v, w) != esperado) alAzar = false;
                }
            }
        }
        chequear("Ej9 - vecinosEnComun (grafos al azar, todos los pares)", alAzar);
    }

    // ========================================================================
    // Ejercicio 10 — cumpleInvariante
    // ========================================================================
    {
        Grafo lbien = listasDe(N_TEORICA, TEORICA);
        chequear("Ej10 - cumpleInvariante (grafo bien armado)", cumpleInvariante(lbien));
        chequear("Ej10 - cumpleInvariante (sin vértices)", cumpleInvariante(listasDe(0, {})));
        chequear("Ej10 - cumpleInvariante (K5)", cumpleInvariante(listasDe(5, aristasCompleto(5))));

        Grafo lasim = lbien;
        lasim.ady[0].push_back(2);   // 2 no tiene a 0 en su lista
        chequear("Ej10 - cumpleInvariante (w en ady[v] pero v no en ady[w] -> no)", !cumpleInvariante(lasim));

        Grafo lrep = lbien;
        lrep.ady[0].push_back(1);    // 1 aparece dos veces en la lista de 0
        chequear("Ej10 - cumpleInvariante (vecino repetido -> no)", !cumpleInvariante(lrep));

        Grafo lbucle = lbien;
        lbucle.ady[2].push_back(2);  // 2 es vecino de sí mismo
        chequear("Ej10 - cumpleInvariante (un vértice en su propia lista -> no)", !cumpleInvariante(lbucle));

        Grafo lfuera = lbien;
        lfuera.ady[0].push_back(9);  // 9 no es un vértice (n = 4)
        chequear("Ej10 - cumpleInvariante (vecino que no es un vértice -> no)", !cumpleInvariante(lfuera));

        Grafo lcant = lbien;
        lcant.ady.pop_back();        // n = 4 pero 3 listas
        chequear("Ej10 - cumpleInvariante (cantidad de listas distinta de n -> no)", !cumpleInvariante(lcant));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 20; s++) {
            size_t n = 1 + s % 12;
            std::vector<Arista> ar = aristasAleatorias(n, n * 3, s);
            if (!cumpleInvariante(listasDe(n, ar))) alAzar = false;
        }
        chequear("Ej10 - cumpleInvariante (grafos al azar bien armados -> sí)", alAzar);
    }

    // ========================================================================
    // Ejercicio 11 — gradosDeEntrada
    // ========================================================================
    {
        chequear("Ej11 - gradosDeEntrada (ejemplo del enunciado)",
                 gradosDeEntrada({{1, 2}, {2}, {0, 3}, {3}}) == Vertices({1, 1, 2, 2}));
        chequear("Ej11 - gradosDeEntrada (sin vértices)", gradosDeEntrada({}).empty());
        chequear("Ej11 - gradosDeEntrada (sin aristas)", gradosDeEntrada({{}, {}, {}}) == Vertices({0, 0, 0}));
        chequear("Ej11 - gradosDeEntrada (un bucle)", gradosDeEntrada({{0}}) == Vertices({1}));
        chequear("Ej11 - gradosDeEntrada (todos apuntan al 3)",
                 gradosDeEntrada({{3}, {3}, {3}, {}}) == Vertices({0, 0, 0, 3}));
        chequear("Ej11 - gradosDeEntrada (grado de salida != grado de entrada)",
                 gradosDeEntrada({{1, 2, 3}, {}, {}, {}}) == Vertices({0, 1, 1, 1}));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 20; s++) {
            size_t n = 1 + s % 10;
            unsigned int semilla = s;
            std::vector<std::vector<size_t>> salientes(n);
            Vertices esperado(n, 0);
            for (size_t v = 0; v < n; v++) {
                size_t cuantas = siguiente(semilla) % 4;
                std::set<size_t> destinos;
                for (size_t i = 0; i < cuantas; i++) destinos.insert(siguiente(semilla) % n);
                for (size_t w : destinos) {
                    salientes[v].push_back(w);
                    esperado[w]++;
                }
            }
            if (gradosDeEntrada(salientes) != esperado) alAzar = false;
        }
        chequear("Ej11 - gradosDeEntrada (grafos dirigidos al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 12 — esAlcanzableDFS
    // ========================================================================
    {
        Grafo g = listasDe(N_DFS, DFS);
        chequear("Ej12 - esAlcanzableDFS (el ejemplo de la teórica: de 0 a 3)", esAlcanzableDFS(g, 0, 3));
        chequear("Ej12 - esAlcanzableDFS (de 3 a 0)", esAlcanzableDFS(g, 3, 0));
        chequear("Ej12 - esAlcanzableDFS (de 7 a 1)", esAlcanzableDFS(g, 7, 1));

        bool todosPares = true;
        for (size_t v = 0; v < N_DFS; v++) {
            for (size_t w = 0; w < N_DFS; w++) {
                if (!esAlcanzableDFS(g, v, w)) todosPares = false;
            }
        }
        chequear("Ej12 - esAlcanzableDFS (grafo conexo: todos los pares)", todosPares);

        chequear("Ej12 - esAlcanzableDFS (v == w, en un vértice aislado)", esAlcanzableDFS(listasDe(3, {}), 1, 1));
        chequear("Ej12 - esAlcanzableDFS (v == w, en un grafo con aristas)", esAlcanzableDFS(g, 5, 5));

        // Dos componentes y un vértice aislado: {0, 1}, {3, 4}, {2}.
        Grafo dos = listasDe(5, {{0, 1}, {3, 4}});
        chequear("Ej12 - esAlcanzableDFS (misma componente)", esAlcanzableDFS(dos, 0, 1) && esAlcanzableDFS(dos, 4, 3));
        chequear("Ej12 - esAlcanzableDFS (distinta componente -> no)",
                 !esAlcanzableDFS(dos, 0, 3) && !esAlcanzableDFS(dos, 4, 1) && !esAlcanzableDFS(dos, 0, 2));

        // Con un ciclo y un destino inalcanzable: si no se marcan visitados, no termina.
        Grafo ciclo = listasDe(4, aristasCiclo(3));
        chequear("Ej12 - esAlcanzableDFS (ciclo, destino inalcanzable: tiene que terminar)", !esAlcanzableDFS(ciclo, 0, 3));

        Grafo cam = listasDe(20000, aristasCamino(20000));
        chequear("Ej12 - esAlcanzableDFS (camino de 20000 vértices)",
                 esAlcanzableDFS(cam, 0, 19999) && esAlcanzableDFS(cam, 19999, 0));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 15;
            std::vector<Arista> ar = aristasAleatorias(n, n * (s % 4), s);
            Grafo r = listasDe(n, ar);
            UnionFind uf = componentesDe(n, ar);
            for (size_t v = 0; v < n; v++) {
                for (size_t w = 0; w < n; w++) {
                    if (esAlcanzableDFS(r, v, w) != (uf.buscar(v) == uf.buscar(w))) alAzar = false;
                }
            }
        }
        chequear("Ej12 - esAlcanzableDFS (grafos al azar, todos los pares)", alAzar);
    }

    // ========================================================================
    // Ejercicio 13 — ordenDeVisitaDFS
    // ========================================================================
    {
        Grafo g = listasDe(N_DFS, DFS);
        chequear("Ej13 - ordenDeVisitaDFS (la traza de la teórica: desde 0)",
                 ordenDeVisitaDFS(g, 0) == Vertices({0, 5, 6, 7, 4, 2, 3, 1}));
        chequear("Ej13 - ordenDeVisitaDFS (desde 7)",
                 ordenDeVisitaDFS(g, 7) == Vertices({7, 6, 5, 0, 1, 2, 4, 3}));
        chequear("Ej13 - ordenDeVisitaDFS (desde 3)",
                 ordenDeVisitaDFS(g, 3) == Vertices({3, 2, 4, 6, 7, 5, 0, 1}));

        Grafo cam = listasDe(4, aristasCamino(4));
        chequear("Ej13 - ordenDeVisitaDFS (camino, desde un extremo)", ordenDeVisitaDFS(cam, 0) == Vertices({0, 1, 2, 3}));
        chequear("Ej13 - ordenDeVisitaDFS (camino, desde un vértice del medio)", ordenDeVisitaDFS(cam, 1) == Vertices({1, 2, 3, 0}));

        Grafo dos = listasDe(5, {{0, 1}, {3, 4}});
        chequear("Ej13 - ordenDeVisitaDFS (sólo los alcanzables)", ordenDeVisitaDFS(dos, 0) == Vertices({0, 1}));
        chequear("Ej13 - ordenDeVisitaDFS (vértice aislado)", ordenDeVisitaDFS(dos, 2) == Vertices({2}));

        bool sinRepetidos = true;
        for (unsigned int s = 1; s <= 20; s++) {
            size_t n = 1 + s % 15;
            std::vector<Arista> ar = aristasAleatorias(n, n * 2, s);
            Grafo r = listasDe(n, ar);
            UnionFind uf = componentesDe(n, ar);
            Vertices orden = ordenDeVisitaDFS(r, 0);
            size_t enLaComponente = 0;
            for (size_t v = 0; v < n; v++) {
                if (uf.buscar(v) == uf.buscar(0)) enLaComponente++;
            }
            if (orden.empty() || orden[0] != 0) sinRepetidos = false;
            else if (orden.size() != enLaComponente) sinRepetidos = false;
            else if (std::set<size_t>(orden.begin(), orden.end()).size() != orden.size()) sinRepetidos = false;
        }
        chequear("Ej13 - ordenDeVisitaDFS (grafos al azar: cada alcanzable una sola vez)", sinRepetidos);
    }

    // ========================================================================
    // Ejercicio 14 — cantidadDeComponentes
    // ========================================================================
    {
        chequear("Ej14 - cantidadDeComponentes (sin vértices)", cantidadDeComponentes(listasDe(0, {})) == 0);
        chequear("Ej14 - cantidadDeComponentes (un vértice)", cantidadDeComponentes(listasDe(1, {})) == 1);
        chequear("Ej14 - cantidadDeComponentes (sin aristas: una por vértice)", cantidadDeComponentes(listasDe(5, {})) == 5);
        chequear("Ej14 - cantidadDeComponentes (grafo conexo)", cantidadDeComponentes(listasDe(N_DFS, DFS)) == 1);
        chequear("Ej14 - cantidadDeComponentes (grafo de ejemplo de la clase 16)",
                 cantidadDeComponentes(listasDe(N_EJEMPLO, EJEMPLO)) == 1);
        // Como el de la teórica: {0,...,4}, {5, 6} y {7}.
        chequear("Ej14 - cantidadDeComponentes (3 componentes)",
                 cantidadDeComponentes(listasDe(8, {{0, 1}, {1, 2}, {0, 3}, {3, 4}, {5, 6}})) == 3);
        chequear("Ej14 - cantidadDeComponentes (dos triángulos)",
                 cantidadDeComponentes(listasDe(6, {{0, 1}, {1, 2}, {0, 2}, {3, 4}, {4, 5}, {3, 5}})) == 2);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 20;
            std::vector<Arista> ar = aristasAleatorias(n, n * (s % 3), s);
            if (cantidadDeComponentes(listasDe(n, ar)) != componentesDe(n, ar).componentes) alAzar = false;
        }
        chequear("Ej14 - cantidadDeComponentes (grafos al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 15 — esConexo
    // ========================================================================
    {
        chequear("Ej15 - esConexo (un vértice)", esConexo(listasDe(1, {})));
        chequear("Ej15 - esConexo (dos vértices con arista)", esConexo(listasDe(2, {{0, 1}})));
        chequear("Ej15 - esConexo (dos vértices sin arista -> no)", !esConexo(listasDe(2, {})));
        chequear("Ej15 - esConexo (el grafo del DFS de la teórica)", esConexo(listasDe(N_DFS, DFS)));
        chequear("Ej15 - esConexo (K5)", esConexo(listasDe(5, aristasCompleto(5))));
        chequear("Ej15 - esConexo (un vértice aislado -> no)", !esConexo(listasDe(5, aristasCompleto(4))));
        chequear("Ej15 - esConexo (dos componentes -> no)", !esConexo(listasDe(4, {{0, 1}, {2, 3}})));
        chequear("Ej15 - esConexo (camino de 20000 vértices)", esConexo(listasDe(20000, aristasCamino(20000))));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 12;
            std::vector<Arista> ar = aristasAleatorias(n, n * (1 + s % 3), s);
            if (esConexo(listasDe(n, ar)) != (componentesDe(n, ar).componentes == 1)) alAzar = false;
        }
        chequear("Ej15 - esConexo (grafos al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 16 — esArbol
    // ========================================================================
    {
        chequear("Ej16 - esArbol (un vértice)", esArbol(listasDe(1, {})));
        chequear("Ej16 - esArbol (una arista)", esArbol(listasDe(2, {{0, 1}})));
        chequear("Ej16 - esArbol (camino)", esArbol(listasDe(6, aristasCamino(6))));
        chequear("Ej16 - esArbol (estrella)", esArbol(listasDe(6, aristasEstrella(6))));
        chequear("Ej16 - esArbol (ciclo -> no)", !esArbol(listasDe(5, aristasCiclo(5))));
        chequear("Ej16 - esArbol (el grafo del DFS de la teórica tiene un ciclo -> no)", !esArbol(listasDe(N_DFS, DFS)));
        chequear("Ej16 - esArbol (sin conectar -> no)", !esArbol(listasDe(3, {})));
        // n - 1 aristas, pero no conexo: un triángulo y un vértice aislado.
        chequear("Ej16 - esArbol (n - 1 aristas pero no conexo -> no)",
                 !esArbol(listasDe(4, {{0, 1}, {1, 2}, {0, 2}})));

        bool arboles = true;
        bool conExtra = true;
        for (unsigned int s = 1; s <= 25; s++) {
            size_t n = 3 + s % 12;
            std::vector<Arista> ar = aristasArbolAleatorio(n, s);
            if (!esArbol(listasDe(n, ar))) arboles = false;

            // Le agrego una arista que no estaba: ya no es un árbol.
            std::set<Arista> conj(ar.begin(), ar.end());
            bool agregada = false;
            for (size_t i = 0; i < n && !agregada; i++) {
                for (size_t j = i + 1; j < n && !agregada; j++) {
                    if (conj.count(Arista(i, j)) == 0) {
                        ar.push_back(Arista(i, j));
                        agregada = true;
                    }
                }
            }
            if (agregada && esArbol(listasDe(n, ar))) conExtra = false;
        }
        chequear("Ej16 - esArbol (árboles al azar)", arboles);
        chequear("Ej16 - esArbol (árboles al azar, más una arista -> no)", conExtra);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 40; s++) {
            size_t n = 1 + s % 8;
            std::vector<Arista> ar = aristasAleatorias(n, n, s);
            if (esArbol(listasDe(n, ar)) != esArbolReferencia(n, ar)) alAzar = false;
        }
        chequear("Ej16 - esArbol (grafos al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 17 — esBipartito
    // ========================================================================
    {
        chequear("Ej17 - esBipartito (sin vértices)", esBipartito(listasDe(0, {})));
        chequear("Ej17 - esBipartito (sin aristas)", esBipartito(listasDe(4, {})));
        chequear("Ej17 - esBipartito (una arista)", esBipartito(listasDe(2, {{0, 1}})));
        chequear("Ej17 - esBipartito (camino)", esBipartito(listasDe(7, aristasCamino(7))));
        chequear("Ej17 - esBipartito (ciclo par: C4)", esBipartito(listasDe(4, aristasCiclo(4))));
        chequear("Ej17 - esBipartito (ciclo par: C6)", esBipartito(listasDe(6, aristasCiclo(6))));
        chequear("Ej17 - esBipartito (ciclo impar: C3 -> no)", !esBipartito(listasDe(3, aristasCiclo(3))));
        chequear("Ej17 - esBipartito (ciclo impar: C5 -> no)", !esBipartito(listasDe(5, aristasCiclo(5))));
        chequear("Ej17 - esBipartito (K4 -> no)", !esBipartito(listasDe(4, aristasCompleto(4))));
        chequear("Ej17 - esBipartito (el grafo de ejemplo de la clase 16 tiene un triángulo -> no)",
                 !esBipartito(listasDe(N_EJEMPLO, EJEMPLO)));
        chequear("Ej17 - esBipartito (el grafo del DFS de la teórica: ciclo de 6 -> sí)",
                 esBipartito(listasDe(N_DFS, DFS)));

        // K_{3,3}: {0,1,2} contra {3,4,5}; y el mismo con una arista dentro de un lado.
        std::vector<Arista> k33;
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 3; j < 6; j++) k33.push_back(Arista(i, j));
        chequear("Ej17 - esBipartito (K_{3,3})", esBipartito(listasDe(6, k33)));
        std::vector<Arista> k33Mas = k33;
        k33Mas.push_back(Arista(0, 1));
        chequear("Ej17 - esBipartito (K_{3,3} más una arista dentro de un lado -> no)", !esBipartito(listasDe(6, k33Mas)));

        // No conexos: ambas componentes bipartitas / una sola con ciclo impar.
        chequear("Ej17 - esBipartito (dos componentes bipartitas)",
                 esBipartito(listasDe(7, {{0, 1}, {1, 2}, {3, 4}, {4, 5}, {5, 6}, {3, 6}})));
        chequear("Ej17 - esBipartito (la segunda componente tiene un ciclo impar -> no)",
                 !esBipartito(listasDe(6, {{0, 1}, {3, 4}, {4, 5}, {3, 5}})));

        bool arboles = true;
        for (unsigned int s = 1; s <= 20; s++) {
            size_t n = 1 + s % 14;
            if (!esBipartito(listasDe(n, aristasArbolAleatorio(n, s)))) arboles = false;
        }
        chequear("Ej17 - esBipartito (todo árbol es bipartito)", arboles);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 40; s++) {
            size_t n = 1 + s % 12;
            std::vector<Arista> ar = aristasAleatorias(n, n + s % 5, s);
            if (esBipartito(listasDe(n, ar)) != esBipartitoReferencia(n, ar)) alAzar = false;
        }
        chequear("Ej17 - esBipartito (grafos al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 18 — ordenDeVisitaBFS
    // ========================================================================
    {
        Grafo g = listasDe(N_DFS, DFS);
        chequear("Ej18 - ordenDeVisitaBFS (grafo del DFS de la teórica, desde 0)",
                 ordenDeVisitaBFS(g, 0) == Vertices({0, 1, 5, 2, 6, 3, 4, 7}));
        chequear("Ej18 - ordenDeVisitaBFS (desde 7)",
                 ordenDeVisitaBFS(g, 7) == Vertices({7, 6, 4, 5, 2, 0, 1, 3}));

        Grafo cam = listasDe(4, aristasCamino(4));
        chequear("Ej18 - ordenDeVisitaBFS (camino, desde un extremo)", ordenDeVisitaBFS(cam, 0) == Vertices({0, 1, 2, 3}));
        chequear("Ej18 - ordenDeVisitaBFS (camino, desde un vértice del medio)", ordenDeVisitaBFS(cam, 1) == Vertices({1, 0, 2, 3}));
        chequear("Ej18 - ordenDeVisitaBFS (estrella, desde el centro)",
                 ordenDeVisitaBFS(listasDe(5, aristasEstrella(5)), 0) == Vertices({0, 1, 2, 3, 4}));

        Grafo dos = listasDe(5, {{0, 1}, {3, 4}});
        chequear("Ej18 - ordenDeVisitaBFS (sólo los alcanzables)", ordenDeVisitaBFS(dos, 3) == Vertices({3, 4}));
        chequear("Ej18 - ordenDeVisitaBFS (vértice aislado)", ordenDeVisitaBFS(dos, 2) == Vertices({2}));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 20; s++) {
            size_t n = 1 + s % 15;
            std::vector<Arista> ar = aristasAleatorias(n, n * 2, s);
            Grafo r = listasDe(n, ar);
            std::vector<size_t> ref = distanciasReferencia(n, ar, 0);
            Vertices orden = ordenDeVisitaBFS(r, 0);
            // El BFS saca a los vértices por distancia no decreciente, sin repetir y sin dejar ninguno afuera.
            size_t alcanzables = 0;
            for (size_t w = 0; w < n; w++) {
                if (ref[w] != NO_ALCANZABLE) alcanzables++;
            }
            if (orden.size() != alcanzables || orden.empty() || orden[0] != 0) alAzar = false;
            else if (std::set<size_t>(orden.begin(), orden.end()).size() != orden.size()) alAzar = false;
            else {
                for (size_t i = 1; i < orden.size(); i++) {
                    if (ref[orden[i]] == NO_ALCANZABLE || ref[orden[i - 1]] > ref[orden[i]]) alAzar = false;
                }
            }
        }
        chequear("Ej18 - ordenDeVisitaBFS (grafos al azar: por niveles, sin repetidos)", alAzar);
    }

    // ========================================================================
    // Ejercicio 19 — distanciasDesde
    // ========================================================================
    {
        Grafo g = listasDe(N_DFS, DFS);
        chequear("Ej19 - distanciasDesde (grafo del DFS de la teórica, desde 0)",
                 distanciasDesde(g, 0) == Vertices({0, 1, 2, 3, 3, 1, 2, 3}));
        chequear("Ej19 - distanciasDesde (desde 7)",
                 distanciasDesde(g, 7) == Vertices({3, 4, 3, 4, 2, 2, 1, 0}));
        chequear("Ej19 - distanciasDesde (camino de 5 vértices, desde el medio)",
                 distanciasDesde(listasDe(5, aristasCamino(5)), 2) == Vertices({2, 1, 0, 1, 2}));

        Grafo dos = listasDe(5, {{0, 1}, {3, 4}});
        chequear("Ej19 - distanciasDesde (los que no se alcanzan: NO_ALCANZABLE)",
                 distanciasDesde(dos, 0) == Vertices({0, 1, NO_ALCANZABLE, NO_ALCANZABLE, NO_ALCANZABLE}));
        chequear("Ej19 - distanciasDesde (vértice aislado)",
                 distanciasDesde(dos, 2) == Vertices({NO_ALCANZABLE, NO_ALCANZABLE, 0, NO_ALCANZABLE, NO_ALCANZABLE}));

        // El DFS encuentra un camino, pero no necesariamente el más corto: en un ciclo de 6, el 3 está a distancia 3.
        chequear("Ej19 - distanciasDesde (ciclo de 6: se elige el lado más corto)",
                 distanciasDesde(listasDe(6, aristasCiclo(6)), 0) == Vertices({0, 1, 2, 3, 2, 1}));

        Grafo largo = listasDe(20000, aristasCamino(20000));
        chequear("Ej19 - distanciasDesde (camino de 20000 vértices)", distanciasDesde(largo, 0)[19999] == 19999);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 15;
            std::vector<Arista> ar = aristasAleatorias(n, n * (1 + s % 3), s);
            Grafo r = listasDe(n, ar);
            for (size_t v = 0; v < n; v++) {
                if (distanciasDesde(r, v) != distanciasReferencia(n, ar, v)) alAzar = false;
            }
        }
        chequear("Ej19 - distanciasDesde (grafos al azar, desde todos los vértices)", alAzar);
    }

    // ========================================================================
    // Ejercicio 20 — caminoMasCorto
    // ========================================================================
    {
        Grafo g = listasDe(N_DFS, DFS);
        // Un único camino mínimo entre 0 y 3 (0-1-2-3, de longitud 3; el otro lado del ciclo mide 5).
        chequear("Ej20 - caminoMasCorto (de 0 a 3 en el grafo del DFS de la teórica)",
                 caminoMasCorto(g, 0, 3) == Vertices({0, 1, 2, 3}));
        chequear("Ej20 - caminoMasCorto (de 3 a 0)", caminoMasCorto(g, 3, 0) == Vertices({3, 2, 1, 0}));
        chequear("Ej20 - caminoMasCorto (v == w)", caminoMasCorto(g, 4, 4) == Vertices({4}));
        chequear("Ej20 - caminoMasCorto (v y w vecinos)", caminoMasCorto(g, 6, 7) == Vertices({6, 7}));

        Grafo dos = listasDe(5, {{0, 1}, {3, 4}});
        chequear("Ej20 - caminoMasCorto (no hay camino -> vacío)",
                 caminoMasCorto(dos, 0, 3).empty() && caminoMasCorto(dos, 2, 0).empty());
        chequear("Ej20 - caminoMasCorto (vértice aislado a sí mismo)", caminoMasCorto(dos, 2, 2) == Vertices({2}));

        chequear("Ej20 - caminoMasCorto (camino de 20000 vértices)",
                 caminoMasCorto(listasDe(20000, aristasCamino(20000)), 0, 19999).size() == 20000);

        // En grafos al azar hay varios caminos mínimos posibles: se chequea que sea un camino válido y de longitud mínima.
        bool alAzar = true;
        for (unsigned int s = 1; s <= 30; s++) {
            size_t n = 1 + s % 15;
            std::vector<Arista> ar = aristasAleatorias(n, n * (1 + s % 3), s);
            Grafo r = listasDe(n, ar);
            std::set<Arista> conj(ar.begin(), ar.end());
            for (size_t v = 0; v < n; v++) {
                std::vector<size_t> ref = distanciasReferencia(n, ar, v);
                for (size_t w = 0; w < n; w++) {
                    Vertices camino = caminoMasCorto(r, v, w);
                    if (ref[w] == NO_ALCANZABLE) {
                        if (!camino.empty()) alAzar = false;
                        continue;
                    }
                    if (camino.size() != ref[w] + 1 || camino.front() != v || camino.back() != w) {
                        alAzar = false;
                        continue;
                    }
                    for (size_t i = 0; i + 1 < camino.size(); i++) {
                        size_t a = camino[i], b = camino[i + 1];
                        if (conj.count(Arista(std::min(a, b), std::max(a, b))) == 0) alAzar = false;
                    }
                }
            }
        }
        chequear("Ej20 - caminoMasCorto (grafos al azar: camino válido y de longitud mínima)", alAzar);
    }

    // ========================================================================
    // Ejercicio 21 — verticesADistancia
    // ========================================================================
    {
        Grafo g = listasDe(N_DFS, DFS);
        chequear("Ej21 - verticesADistancia (k = 0 es el propio vértice)", verticesADistancia(g, 0, 0) == Vertices({0}));
        chequear("Ej21 - verticesADistancia (k = 1: los vecinos)", verticesADistancia(g, 0, 1) == Vertices({1, 5}));
        chequear("Ej21 - verticesADistancia (k = 2)", verticesADistancia(g, 0, 2) == Vertices({2, 6}));
        chequear("Ej21 - verticesADistancia (k = 3)", verticesADistancia(g, 0, 3) == Vertices({3, 4, 7}));
        chequear("Ej21 - verticesADistancia (k demasiado grande -> vacío)", verticesADistancia(g, 0, 4).empty());
        chequear("Ej21 - verticesADistancia (estrella, desde una hoja, k = 2)",
                 verticesADistancia(listasDe(5, aristasEstrella(5)), 1, 2) == Vertices({2, 3, 4}));
        chequear("Ej21 - verticesADistancia (no cuenta a los inalcanzables)",
                 verticesADistancia(listasDe(5, {{0, 1}, {3, 4}}), 0, 1) == Vertices({1}));

        bool alAzar = true;
        for (unsigned int s = 1; s <= 25; s++) {
            size_t n = 1 + s % 15;
            std::vector<Arista> ar = aristasAleatorias(n, n * 2, s);
            Grafo r = listasDe(n, ar);
            std::vector<size_t> ref = distanciasReferencia(n, ar, 0);
            for (size_t k = 0; k <= n; k++) {
                Vertices esperado;
                for (size_t w = 0; w < n; w++) {
                    if (ref[w] == k) esperado.push_back(w);
                }
                if (verticesADistancia(r, 0, k) != esperado) alAzar = false;
            }
        }
        chequear("Ej21 - verticesADistancia (grafos al azar, todos los k)", alAzar);
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
