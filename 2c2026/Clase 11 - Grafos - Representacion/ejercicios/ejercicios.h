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
//   - Parte 4: BFS (el mismo recorrido con una cola): distancias mínimas y
//              2-coloreo.
//
// Convención de la teórica: los vértices de un grafo con n vértices son
// 0, 1, ..., n - 1. Toda precondición que mencione a un vértice v o w incluye,
// además, 0 <= v, w < n. Los vértices son size_t (igual que las posiciones
// de un vector), así -Wconversion queda contento.
//
// Notación de los comentarios de cada ejercicio:
//   - pre:  lo que se le garantiza a la función al llamarla (si no se cumple,
//           no hay nada que cumplir: no hace falta chequearlo).
//   - post: lo que la función tiene que cumplir al terminar.
//   - n es g.n. Salvo que se diga lo contrario, todo Grafo g que se recibe
//     cumple el invariante de representación, y la función no puede dejarlo
//     de cumplir.
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
// pre:  true (n puede ser 0).
// post: res.n == n y res.ady tiene n listas, todas vacías.
// Complejidad: O(n).
Grafo grafoSinAristas(size_t n);

// Ejercicio 2 — CantidadDeVertices
// Devuelve la cantidad de vértices de g.
// pre:  true.
// post: res == n.
// Complejidad: O(1).
size_t cantidadDeVertices(const Grafo& g);

// Ejercicio 3 — AgregarArista
// Conecta v con w: agrega w a la lista de v y v a la lista de w.
// pre:  v < n, w < n, v != w y {v, w} todavía no es una arista de g.
//       (Justamente por esto no hay que buscar si la arista ya estaba.)
// post: {v, w} es una arista de g; el resto del grafo queda igual.
// Complejidad: O(1) amortizado.
void agregarArista(Grafo& g, size_t v, size_t w);

// Ejercicio 4 — SonVecinos
// Devuelve si {v, w} es una arista de g: hay que buscar a w en la lista de v.
// pre:  v < n, w < n.
// post: res == true si y sólo si {v, w} es una arista de g.
// Complejidad: O(d(v)). (Bonus: como el grado sale en O(1), podés buscar en
// la más corta de las dos listas y quedar en O(mín(d(v), d(w))).)
bool sonVecinos(const Grafo& g, size_t v, size_t w);

// Ejercicio 5 — Vecinos
// Devuelve la secuencia de vecinos de v SIN COPIARLA (por eso devuelve una
// referencia): los vecinos ya están todos juntos en ady[v], y sólo ellos.
// Quien la use la recorre en O(d(v)).
// pre:  v < n.
// post: res tiene exactamente a los vecinos de v (es ady[v]).
// Complejidad: O(1) para obtenerla.
const std::vector<size_t>& vecinos(const Grafo& g, size_t v);

// Ejercicio 6 — Grado
// Devuelve d(v). El vector guarda su tamaño: no hay que recorrer nada.
// pre:  v < n.
// post: res == d(v), la cantidad de vecinos de v.
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
// pre:  true.
// post: res == m.
// Pista: la suma de los grados de todos los vértices es 2m (cada arista
// aporta 1 al grado de cada uno de sus dos extremos).
// Complejidad: O(n).
size_t cantidadDeAristas(const Grafo& g);

// Ejercicio 8 — ¿Es completo?
// Devuelve true si todo par de vértices distintos es adyacente (el grafo K_n).
// pre:  true.
// post: res == true si y sólo si todo par de vértices distintos de g es una
//       arista. Con 0 o 1 vértices es completo.
// Pista: en K_n todo vértice tiene grado n - 1; no hace falta mirar pares.
// Complejidad: O(n).
bool esCompleto(const Grafo& g);

// Ejercicio 9 — Vecinos en común
// Devuelve los vértices que son vecinos de v Y de w.
// pre:  v < n, w < n. (v y w pueden ser vecinos entre sí o no.)
// post: res tiene exactamente a los x que son vecinos de v y de w, en orden
//       CRECIENTE y sin repetidos. Ni v ni w aparecen en res (un vértice no
//       es vecino de sí mismo).
// Ejemplo: en el grafo con aristas {0,1},{0,2},{1,2},{1,3},{2,3}, los
// vecinos en común de 1 y 2 son {0, 3}.
// Complejidad esperada: O(n + d(v) + d(w)).
std::vector<size_t> vecinosEnComun(const Grafo& g, size_t v, size_t w);

// Ejercicio 10 — Invariante de representación
// Devuelve true si el grafo cumple el invariante de representación (ver
// arriba, en la definición de la struct). Sirve para detectar estructuras mal
// armadas: por ejemplo, una lista que tiene un vecino repetido o una arista
// que está sólo de un lado.
// pre:  true. (Acá g PUEDE estar mal armado: es justo lo que se chequea.)
// post: res == true si y sólo si
//       - ady tiene n posiciones; todos los elementos son vértices válidos
//         (< n);
//       - ninguna lista tiene repetidos y ningún vértice está en su propia
//         lista;
//       - w está en ady[v] si y sólo si v está en ady[w].
// Complejidad: no hay una exigida. Pensá una forma razonable.
bool cumpleInvariante(const Grafo& g);

// Ejercicio 11 — Grados de entrada (grafo DIRIGIDO)
// Un grafo dirigido se guarda con listas de adyacencia donde salientes[v]
// tiene SÓLO los vecinos a los que v apunta (cada arista (v, w) aparece una
// única vez, en la lista de v; puede haber bucles (v, v)).
// Devuelve el grado de entrada d_in(v) de cada vértice v: cuántas aristas
// llegan a v. Acá n es salientes.size().
// pre:  todos los elementos de todas las listas de salientes son < n.
// post: res tiene n posiciones y res[v] es la cantidad de veces que v
//       aparece en las listas de salientes.
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
// Devuelve si hay un camino de v a w en g.
// pre:  v < n, w < n.
// post: res == true si y sólo si hay un camino de v a w en g (si v == w, el
//       camino de longitud 0 cuenta).
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
// Mismo recorrido que el Ejercicio 12, pero sin objetivo.
// pre:  v < n.
// post: res tiene exactamente a los vértices alcanzables desde v, sin
//       repetidos, en el orden en que el DFS los MARCA como visitados
//       (res[0] == v).
// Seguí EXACTAMENTE el algoritmo de la teórica:
// los vecinos de x se apilan en el orden en que aparecen en g.ady[x], así
// que el último en apilarse es el primero en salir.
// Ejemplo (el grafo de la teórica, con las listas ordenadas de menor a
// mayor): desde 0 el orden es {0, 5, 6, 7, 4, 2, 3, 1}.
// Complejidad: O(n + m).
std::vector<size_t> ordenDeVisitaDFS(const Grafo& g, size_t v);

// Ejercicio 14 — Componentes conexas
// Cuenta las componentes conexas de g.
// pre:  true.
// post: res == cantidad de componentes conexas de g (un grafo sin vértices
//       tiene 0).
// Pista: recorrer todos los vértices; cada vez que aparece uno sin visitar,
// es una componente nueva: se recorre (con DFS) todo lo alcanzable desde él.
// Complejidad: O(n + m).
size_t contarComponentes(const Grafo& g);

// Ejercicio 15 — ¿Es conexo?
// Devuelve si g es conexo.
// pre:  n >= 1.
// post: res == true si y sólo si para todo par de vértices hay un camino
//       entre ellos.
// Complejidad: O(n + m).
bool esConexo(const Grafo& g);

// Ejercicio 16 — ¿Es un árbol?
// Devuelve si g es un árbol (como grafo).
// pre:  n >= 1.
// post: res == true si y sólo si g es conexo y no tiene ciclos.
// Pista: la teórica da una caracterización: un grafo de n vértices es un
// árbol si y sólo si es conexo y tiene exactamente n - 1 aristas. Ojo: tener
// n - 1 aristas SOLO no alcanza.
// Complejidad: O(n + m).
bool esArbol(const Grafo& g);

// Ejercicio 17 — ¿Hay un ciclo?
// Devuelve si hay algún ciclo en la componente conexa de v.
// pre:  v < n.
// post: res == true si y sólo si hay un CICLO entre los vértices alcanzables
//       desde v (un ciclo en otra componente no cuenta).
// Pista: pensá en el DFS de la teórica. Un vértice puede quedar REPETIDO en
// la pila. ¿En un árbol puede pasar? ¿Y si el grafo tiene un ciclo? (Otra
// forma de resolverlo: un DFS que se acuerde de quién es el "padre" de cada
// vértice, y detecte un vecino ya visitado que no sea el padre.)
// Ejemplos: un árbol -> false; un triángulo -> true; un triángulo con una
// cola colgando, desde un vértice de la cola -> true.
// Complejidad: O(n + m).
bool hayCicloDesde(const Grafo& g, size_t v);

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
// Recorre con BFS a partir de v.
// pre:  v < n.
// post: res tiene exactamente a los vértices alcanzables desde v, sin
//       repetidos, en el orden en que el BFS los SACA de la cola
//       (res[0] == v). Los vecinos de x se encolan en el orden en que
//       aparecen en g.ady[x].
// Ejemplo (el grafo del DFS de la teórica, con las listas ordenadas de
// menor a mayor): desde 0 el orden es {0, 1, 5, 2, 6, 3, 4, 7}.
// Complejidad: O(n + m).
std::vector<size_t> ordenDeVisitaBFS(const Grafo& g, size_t v);

// Ejercicio 19 — Distancias desde un vértice
// Calcula la distancia de v a cada vértice.
// pre:  v < n.
// post: res tiene n posiciones. res[w] es la distancia MÍNIMA de v a w, en
//       cantidad de aristas (res[v] == 0), o NO_ALCANZABLE si w no es
//       alcanzable desde v.
// Ejemplo (grafo del DFS de la teórica): desde 0 -> {0, 1, 2, 3, 3, 1, 2, 3}.
// Complejidad: O(n + m).
std::vector<size_t> distanciasDesde(const Grafo& g, size_t v);

// Ejercicio 20 — Camino más corto
// Devuelve un camino de v a w con la MENOR cantidad de aristas.
// pre:  v < n, w < n.
// post: - si w es alcanzable desde v: res es la secuencia de vértices del
//         camino, con res[0] == v, res.back() == w, cada par de consecutivos
//         vecinos, y la menor longitud posible (si v == w, res == {v}).
//         Si hay varios caminos mínimos, vale cualquiera.
//       - si no: res es vacío.
// Pista: durante el BFS, guardá para cada vértice descubierto quién lo
// descubrió (su "padre"); después, reconstruí el camino yendo de w para
// atrás hasta v, y dalo vuelta.
// Complejidad: O(n + m).
std::vector<size_t> caminoMasCorto(const Grafo& g, size_t v, size_t w);

// Ejercicio 21 — Vértices a distancia k
// Devuelve los vértices a distancia k de v.
// pre:  v < n.
// post: res tiene exactamente a los vértices cuya distancia mínima a v es
//       EXACTAMENTE k, en orden creciente y sin repetidos. (Con k = 0,
//       res == {v}.)
// Ejemplo (grafo del DFS de la teórica): desde 0, con k = 3 -> {3, 4, 7}.
// Complejidad: O(n + m).
std::vector<size_t> verticesADistancia(const Grafo& g, size_t v, size_t k);

// Ejercicio 22 — ¿Es bipartito?
// Devuelve si g admite un 2-coloreo.
// pre:  true. (g puede no ser conexo.)
// post: res == true si y sólo si los vértices se pueden partir en dos
//       conjuntos tales que toda arista une un vértice de cada uno.
// Pista: pintá un vértice de un color; eso obliga a pintar a sus vecinos del
// otro color, a los vecinos de esos del primero, y así. Hacelo con un BFS:
// cada vértice que se descubre se pinta del color contrario al del vértice
// que lo descubrió (los niveles pares de un color, los impares del otro).
// Si alguna arista une dos vértices del mismo color, no es bipartito. Repetí
// desde cada vértice todavía sin pintar.
// Complejidad: O(n + m).
bool esBipartito(const Grafo& g);

#endif
