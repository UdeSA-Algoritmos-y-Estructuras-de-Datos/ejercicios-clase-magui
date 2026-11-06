#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <cstddef>
#include <vector>

// Distancia de un vértice que NO es alcanzable desde el vértice de partida
// (Parte 4). Es el mayor size_t posible: ningún camino real llega a tener
// esa longitud.
const size_t NO_ALCANZABLE = static_cast<size_t>(-1);

// ============================================================================
// Tut 11 - Grafos: representación
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
//
// Se trabaja con el TAD Grafo de la teórica (clase 17), con grafos NO
// DIRIGIDOS y NO PONDERADOS, implementado con LISTAS DE ADYACENCIA (la
// representación que se usa en la materia):
//   - Parte 1: las operaciones del TAD.
//   - Parte 2: problemas sobre la representación (grado, invariante,
//              vecinos).
//   - Parte 3: alcanzabilidad con DFS y problemas que se resuelven con él.
//   - Parte 4: BFS (el mismo recorrido con una cola) y distancias mínimas.
//
// Convención de la teórica: los vértices de un grafo con n vértices son
// 0, 1, ..., n - 1. Toda precondición que mencione a un vértice v o w incluye,
// además, 0 <= v, w < n. Los vértices son size_t (igual que las posiciones
// de un vector), así -Wconversion queda contento.
// ============================================================================

// ============================================================================
// ESTRUCTURAS
// ============================================================================

// Representación con LISTAS DE ADYACENCIA.
//   - n: cantidad de vértices.
//   - ady: arreglo de n secuencias. ady[v] contiene a los vecinos de v (en
//     algún orden).
//
// Invariante de representación: ady tiene n posiciones; w está en ady[v] si y
// sólo si v está en ady[w]; ninguna secuencia tiene repetidos; y ady[v] no
// contiene a v. Cada arista aparece dos veces (una por extremo).
struct Grafo {
    size_t n;
    std::vector<std::vector<size_t>> ady;
};

// ============================================================================
// PARTE 1 — EL TAD GRAFO CON LISTAS DE ADYACENCIA
//
// Las seis operaciones del TAD de la teórica, sobre la estructura de listas
// de adyacencia.
// ============================================================================

// Ejercicio 1 — GrafoSinAristas
// Crea un grafo de n vértices y ninguna arista: n listas vacías.
// Complejidad: O(n).
Grafo grafoSinAristas(size_t n);

// Ejercicio 2 — CantidadDeVertices
// Complejidad: O(1).
size_t cantidadDeVertices(const Grafo& g);

// Ejercicio 3 — AgregarArista
// Conecta v con w: agrega w a la lista de v y v a la lista de w (la
// simetría, otra vez).
// Precondición: v != w y no son vecinos todavía. (Justamente por esa
// precondición no hay que buscar si la arista ya estaba.)
// Complejidad: O(1) amortizado.
void agregarArista(Grafo& g, size_t v, size_t w);

// Ejercicio 4 — SonVecinos
// Devuelve si {v, w} es una arista de g: hay que buscar a w en la lista de v.
// Complejidad: O(d(v)). (Bonus: como el grado sale en O(1), podés buscar en
// la más corta de las dos listas y quedar en O(mín(d(v), d(w))).)
bool sonVecinos(const Grafo& g, size_t v, size_t w);

// Ejercicio 5 — Vecinos
// Devuelve la secuencia de vecinos de v SIN COPIARLA (por eso devuelve una
// referencia): los vecinos ya están todos juntos en ady[v], y sólo ellos.
// Quien la use la recorre en O(d(v)).
// Complejidad: O(1) para obtenerla.
const std::vector<size_t>& vecinos(const Grafo& g, size_t v);

// Ejercicio 6 — Grado
// Devuelve d(v). El vector guarda su tamaño: no hay que recorrer nada.
// Complejidad: O(1).
size_t grado(const Grafo& g, size_t v);

// ============================================================================
// PARTE 2 — TRABAJANDO CON LA REPRESENTACIÓN
//
// Desde acá podés usar las funciones del TAD que ya escribiste... o acceder
// directo a g.n y g.ady. Las pruebas NO dependen de tus funciones de la
// Parte 1 (arman los grafos a mano), así que un error allá no hace fallar
// acá.
// ============================================================================

// Ejercicio 7 — Cantidad de aristas
// Devuelve m, la cantidad de aristas del grafo.
// Pista: la suma de los grados de todos los vértices es 2m (cada arista
// aporta 1 al grado de cada uno de sus dos extremos).
// Complejidad: O(n).
size_t cantidadDeAristas(const Grafo& g);

// Ejercicio 8 — ¿Es completo?
// Devuelve true si todo par de vértices distintos es adyacente (el grafo K_n).
// Pista: en K_n todo vértice tiene grado n - 1; no hace falta mirar pares.
// Con 0 o 1 vértices es completo.
// Complejidad: O(n).
bool esCompleto(const Grafo& g);

// Ejercicio 9 — Vecinos en común
// Devuelve los vértices que son vecinos de v Y de w, en orden CRECIENTE y sin
// repetidos. v y w pueden ser vecinos entre sí o no; ninguno de los dos
// aparece en el resultado (un vértice no es vecino de sí mismo).
// Ejemplo: en el grafo con aristas {0,1},{0,2},{1,2},{1,3},{2,3}, los
// vecinos en común de 1 y 2 son {0, 3}.
// Complejidad esperada: O(n + d(v) + d(w)).
std::vector<size_t> vecinosEnComun(const Grafo& g, size_t v, size_t w);

// Ejercicio 10 — Invariante de representación
// Devuelve true si el grafo cumple el invariante de representación (ver
// arriba, en la definición de la struct). Sirve para detectar estructuras mal
// armadas: por ejemplo, una lista que tiene un vecino repetido o una arista
// que está sólo de un lado.
//   - n posiciones; todos los elementos son vértices válidos (< n);
//     sin repetidos; ningún vértice en su propia lista; y w está en ady[v]
//     si y sólo si v está en ady[w].
// Complejidad: no hay una exigida. Pensá una forma razonable.
bool cumpleInvariante(const Grafo& g);

// Ejercicio 11 — Grados de entrada (grafo DIRIGIDO)
// Un grafo dirigido se guarda con listas de adyacencia donde salientes[v]
// tiene SÓLO los vecinos a los que v apunta (cada arista (v, w) aparece una
// única vez, en la lista de v; puede haber bucles (v, v)).
// Devuelve un vector de n posiciones con el grado de entrada d_in(v) de cada
// vértice v: cuántas aristas llegan a v.
// Ejemplo: salientes = {{1, 2}, {2}, {0, 3}, {3}} -> {1, 1, 2, 2}.
// Pista: con listas, el grado de entrada obliga a mirar TODAS las listas
// (no hay una lista de "entrantes"); se puede calcular el de todos los
// vértices juntos con una sola pasada. La suma de todos es m.
// Complejidad: O(n + m).
std::vector<size_t> gradosDeEntrada(const std::vector<std::vector<size_t>>& salientes);

// ============================================================================
// PARTE 3 — ALCANZABILIDAD Y DFS
//
// En esta parte todos los grafos son Grafo: es la representación que
// conviene (los recorridos sólo piden los vecinos de un vértice, y con
// listas eso cuesta O(d(v)) en lugar de O(n)).
// ============================================================================

// Ejercicio 12 — Alcanzabilidad con DFS
// Devuelve true si hay un camino de v a w en g (si v == w, el camino de
// longitud 0 cuenta).
// Implementalo como en la teórica, con una PILA explícita (std::stack) y un
// arreglo de visitados:
//   1. Pila de pendientes = [v]; nadie visitado.
//   2. Mientras la pila no esté vacía: sacá el tope x.
//        - si x == w: terminar, hay camino.
//        - si x no fue visitado: marcalo visitado y apilá cada vecino de x
//          que todavía no haya sido visitado.
//   3. Si la pila se vació, no hay camino.
// Ojo: un vértice puede quedar repetido en la pila; por eso, al sacarlo,
// hay que revisar si ya fue visitado. Si el programa se cuelga, seguramente
// falta marcar visitados.
// Complejidad: O(n + m).
bool esAlcanzableDFS(const Grafo& g, size_t v, size_t w);

// Ejercicio 13 — Orden de visita del DFS
// Mismo recorrido que el Ejercicio 12, pero sin objetivo: devuelve los
// vértices alcanzables desde v en el orden en que el DFS los MARCA como
// visitados (el primero es v). Seguí EXACTAMENTE el algoritmo de la teórica:
// los vecinos de x se apilan en el orden en que aparecen en g.ady[x], así
// que el último en apilarse es el primero en salir.
// Ejemplo (el grafo de la teórica, con las listas ordenadas de menor a
// mayor): desde 0 el orden es {0, 5, 6, 7, 4, 2, 3, 1}.
// Complejidad: O(n + m).
std::vector<size_t> ordenDeVisitaDFS(const Grafo& g, size_t v);

// Ejercicio 14 — Componentes conexas
// Devuelve la cantidad de componentes conexas de g. Un grafo sin vértices
// tiene 0.
// Pista: recorrer todos los vértices; cada vez que aparece uno sin visitar,
// es una componente nueva: se recorre (con DFS) todo lo alcanzable desde él.
// Complejidad: O(n + m).
size_t cantidadDeComponentes(const Grafo& g);

// Ejercicio 15 — ¿Es conexo?
// Devuelve true si para todo par de vértices hay un camino entre ellos.
// Precondición: g tiene al menos un vértice.
// Complejidad: O(n + m).
bool esConexo(const Grafo& g);

// Ejercicio 16 — ¿Es un árbol?
// Devuelve true si g es un árbol (como grafo): conexo y sin ciclos.
// Pista: la teórica da una caracterización: un grafo de n vértices es un
// árbol si y sólo si es conexo y tiene exactamente n - 1 aristas. Ojo: tener
// n - 1 aristas SOLO no alcanza.
// Precondición: g tiene al menos un vértice.
// Complejidad: O(n + m).
bool esArbol(const Grafo& g);

// Ejercicio 17 — ¿Es bipartito?
// Devuelve true si los vértices se pueden partir en dos conjuntos tales que
// toda arista une un vértice de cada uno (es decir, si g admite un
// 2-coloreo). El grafo puede no ser conexo.
// Pista: pintá un vértice de un color; eso obliga a pintar a sus vecinos del
// otro color, a los vecinos de esos del primero, y así. Si alguna arista une
// dos vértices del mismo color, no es bipartito. Hacelo con una pila, como
// el DFS, y repetí desde cada vértice todavía sin pintar.
// Complejidad: O(n + m).
bool esBipartito(const Grafo& g);

// ============================================================================
// PARTE 4 — BFS: EL MISMO RECORRIDO, CON UNA COLA
//
// Igual que el DFS, pero los vértices pendientes se guardan en una COLA
// (std::queue) en lugar de una pila: el que se descubrió primero es el
// primero en procesarse, así que el recorrido avanza "por niveles": primero
// el vértice de partida, después sus vecinos (a distancia 1), después los
// vecinos de esos (a distancia 2), etcétera.
//
// Diferencia con el DFS: acá conviene marcar un vértice como visitado en el
// momento en que se ENCOLA (no cuando se lo saca). Así cada vértice entra a
// la cola UNA sola vez, y la distancia se calcula al descubrirlo:
// distancia[y] = distancia[x] + 1, con x el vértice que se está procesando.
// ============================================================================

// Ejercicio 18 — Orden de visita del BFS
// Devuelve los vértices alcanzables desde v en el orden en que el BFS los
// SACA de la cola (el primero es v). Los vecinos de x se encolan en el
// orden en que aparecen en g.ady[x].
// Ejemplo (el grafo del DFS de la teórica, con las listas ordenadas de
// menor a mayor): desde 0 el orden es {0, 1, 5, 2, 6, 3, 4, 7}.
// Complejidad: O(n + m).
std::vector<size_t> ordenDeVisitaBFS(const Grafo& g, size_t v);

// Ejercicio 19 — Distancias desde un vértice
// Devuelve un vector de n posiciones: la posición w tiene la distancia
// MÍNIMA de v a w, en cantidad de aristas (0 para v mismo). Para los
// vértices que no son alcanzables desde v, NO_ALCANZABLE.
// Ejemplo (grafo del DFS de la teórica): desde 0 -> {0, 1, 2, 3, 3, 1, 2, 3}.
// Complejidad: O(n + m).
std::vector<size_t> distanciasDesde(const Grafo& g, size_t v);

// Ejercicio 20 — Camino más corto
// Devuelve un camino de v a w con la MENOR cantidad de aristas, como la
// secuencia de vértices por los que pasa: el primero es v y el último es w.
// Si v == w, es {v}. Si w no es alcanzable desde v, devuelve un vector vacío.
// Si hay varios caminos mínimos, vale cualquiera.
// Pista: durante el BFS, guardá para cada vértice descubierto quién lo
// descubrió (su "padre"); después, reconstruí el camino yendo de w para
// atrás hasta v, y dalo vuelta.
// Complejidad: O(n + m).
std::vector<size_t> caminoMasCorto(const Grafo& g, size_t v, size_t w);

// Ejercicio 21 — Vértices a distancia k
// Devuelve los vértices cuya distancia mínima a v es EXACTAMENTE k, en orden
// creciente. (Con k = 0, es {v}.)
// Ejemplo (grafo del DFS de la teórica): desde 0, con k = 3 -> {3, 4, 7}.
// Complejidad: O(n + m).
std::vector<size_t> verticesADistancia(const Grafo& g, size_t v, size_t k);

#endif
