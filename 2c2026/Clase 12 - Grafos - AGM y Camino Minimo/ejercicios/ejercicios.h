#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <cstddef>
#include <limits>
#include <vector>

// Para las colas de prioridad de Prim y de Dijkstra.
#include <functional>
#include <queue>
#include <tuple>
#include <utility>

// ============================================================================
// Tut 12 - Grafos: árbol generador mínimo y camino mínimo
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
//
// Los ejercicios siguen el orden de las teóricas (clases 18 y 19): cada parte
// arma UN algoritmo, con el mismo pseudocódigo y los mismos nombres de los
// handouts.
//   - Parte 1: KRUSKAL (clase 18): la partición, las aristas ordenadas y el
//              algoritmo.
//   - Parte 2: PRIM (clase 18): Incorporar y el algoritmo.
//   - Parte 3: CAMINO MÍNIMO (clase 19): Mejorar, Dijkstra, el camino y
//              Bellman-Ford.
//   - Parte 4: OPCIONALES, para después: Kruskal con un bosque, la propiedad
//              del corte, el greedy del vuelto y las rondas de Bellman-Ford.
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
//   - n es g.n. Todo grafo g que se recibe cumple el invariante de
//     representación.
// ============================================================================

// ============================================================================
// ESTRUCTURAS
// ============================================================================

// Peso de una arista. Puede ser negativo (Bellman-Ford).
typedef long long Peso;

// Un vecino de un vértice, junto con el peso de la arista que los une. En la
// teórica, Vecinos(G, x) devuelve pares <vecino, peso>.
struct Vecino {
    size_t vertice;
    Peso peso;
};

// Representación con LISTAS DE ADYACENCIA PONDERADAS.
//   - n: cantidad de vértices.
//   - ady: arreglo de n secuencias. ady[x] contiene los vecinos de x (con el
//     peso de la arista), en algún orden. En un grafo dirigido, "vecinos"
//     quiere decir vecinos SALIENTES: los y tales que hay una arista de x a y.
//
// Invariante de representación: ady tiene n posiciones; todos los vértices
// que aparecen son < n; ady[x] no tiene a x (no hay bucles) y no tiene
// repetidos (a lo sumo una arista de x a y).
//   - Grafo NO DIRIGIDO: además, y está en ady[x] con peso p si y sólo si x
//     está en ady[y] con peso p. Cada arista aparece dos veces, una por
//     extremo, con el mismo peso.
//   - Grafo DIRIGIDO: cada arista (x, y) aparece una sola vez, en ady[x].
//
// En las Partes 1 y 2 (árbol generador mínimo) los grafos son NO DIRIGIDOS.
// En la Parte 3 (camino mínimo) pueden ser de cualquiera de los dos tipos:
// los algoritmos sólo usan los vecinos salientes de cada vértice.
struct GrafoPonderado {
    size_t n;
    std::vector<std::vector<Vecino>> ady;
};

// Una arista con sus dos extremos y su peso. Es la unidad con la que trabaja
// Kruskal, y lo que devuelven los algoritmos de árbol generador.
struct Arista {
    size_t v;
    size_t w;
    Peso peso;
};

// ============================================================================
// PARTE 1 — KRUSKAL (clase 18)
//
// Idea: recorrer las aristas de la más liviana a la más pesada. Cada arista se
// agrega a la solución, salvo que forme un ciclo con las que ya se agregaron.
//
// Para programarlo hacen falta dos cosas, y después se juntan:
//   1. Mantener los grupos de vértices: el TAD Conjuntos disjuntos
//      (Ejercicios 1 a 3).
//   2. Las aristas de G, ordenadas por peso (Ejercicio 4).
//   3. El algoritmo (Ejercicio 5).
//
//     Kruskal(G)
//     1  T = vacío
//     2  P = CrearParticion(n)
//     3  aristas = las aristas de G, ordenadas por peso de menor a mayor
//     4  for cada {v, w} en aristas
//     5      if Representante(P, v) != Representante(P, w)
//     6          agregar {v, w} a T
//     7          Unir(P, v, w)
//     8  return T
// ============================================================================

// Conjuntos disjuntos: una partición de los elementos 0, ..., n - 1 en grupos,
// donde cada grupo se identifica con uno de sus elementos: su representante.
//
// Representación (la de la teórica): P es un arreglo de n posiciones, y P[x] es
// el representante del grupo de x. Por ejemplo, con n = 6 y los grupos
// {0, 1, 3}, {2} y {4, 5}:
//     x     0 1 2 3 4 5
//     P[x]  0 0 2 0 4 4
//
// Invariante de representación: el representante es su propio representante:
// para todo x, P[x] < n y P[P[x]] == P[x].
typedef std::vector<size_t> Particion;

// Ejercicio 1 — CrearParticion
// Crea una partición de 0, ..., n - 1 con cada elemento en un grupo propio.
// pre:  true (n puede ser 0).
// post: res tiene n posiciones y res[x] == x para todo x.
// Complejidad: O(n).
Particion crearParticion(size_t n);

// Ejercicio 2 — Representante
// Devuelve el representante del grupo de x.
// pre:  x < p.size().
// post: res es el representante del grupo de x: el mismo para todos los x de
//       un mismo grupo.
// Complejidad: O(1).
size_t representante(const Particion& p, size_t x);

// Ejercicio 3 — Unir
// Junta el grupo de x con el grupo de y en uno solo. El representante del
// grupo resultante es el que tenía el grupo de x.
// pre:  x, y < p.size() y representante(p, x) != representante(p, y).
// post: los elementos de los dos grupos pasan a tener, todos, el representante
//       que tenía x; el resto de la partición queda igual.
// Ejemplo (la pregunta de la teórica): con p = {0, 0, 2, 0, 4, 4},
//          unir(p, 1, 5) deja p = {0, 0, 2, 0, 0, 0}.
// Complejidad: O(n).
void unir(Particion& p, size_t x, size_t y);

// Ejercicio 4 — Las aristas de G, ordenadas por peso
// Es la línea 3 de Kruskal. Devuelve todas las aristas de g, cada una UNA sola
// vez (aunque en las listas aparezca dos veces), de la más liviana a la más
// pesada.
// pre:  g es no dirigido.
// post: res tiene exactamente una Arista por cada arista de g, con v < w y el
//       peso de la arista, y res[i].peso <= res[i + 1].peso para todo i. Si
//       dos aristas pesan lo mismo, pueden quedar en cualquier orden.
// En "ejercicios.cpp" el ordenamiento (std::sort) ya está escrito: sólo hay
// que juntar las aristas.
// Complejidad: O(n + m log m).
std::vector<Arista> aristasOrdenadasPorPeso(const GrafoPonderado& g);

// Ejercicio 5 — Kruskal
// Calcula un árbol generador mínimo de g, siguiendo el pseudocódigo de arriba.
// pre:  g es no dirigido y conexo, y n >= 1.
// post: res es un árbol generador de g de peso mínimo (n - 1 aristas), en el
//       orden en que Kruskal las fue eligiendo. Para cada arista, el orden de
//       sus extremos (v, w) no importa.
// Un grafo puede tener más de un AGM (con pesos repetidos): las pruebas miran
// que lo que devolvés sea un árbol generador de peso mínimo, no que sean
// exactamente las mismas aristas.
// Complejidad: O(m log m + n^2).
std::vector<Arista> kruskal(const GrafoPonderado& g);

// Ejercicio 6 — Peso de un árbol
// El peso de un árbol generador es la suma de los pesos de sus aristas.
// pre:  true.
// post: res es la suma de los pesos de las aristas (0 si no hay ninguna).
// Ejemplo: el AGM de la teórica pesa 1 + 2 + 3 + 4 + 6 = 16.
// Complejidad: O(cantidad de aristas).
Peso pesoDeAristas(const std::vector<Arista>& aristas);

// ============================================================================
// PARTE 2 — PRIM (clase 18)
//
// Idea: hacer crecer UN SOLO árbol desde un vértice inicial. En cada paso,
// agregar la arista más liviana entre las que conectan un vértice del árbol
// con uno de afuera.
//
// Se usa una cola de prioridad de aristas <peso, x, y> donde sale primero la
// de menor peso (un min-heap): x es el extremo que está en el árbol.
//
//     Prim(G, r)
//      1  enArbol = arreglo de n booleanos, todos en false
//      2  T = vacío;  C = ColaDePrioridadVacia()
//      3  Incorporar(G, r, enArbol, C)
//      4  while no Vacia(C)
//      5      <p, x, y> = Minimo(C)
//      6      EliminarMinimo(C)
//      7      if no enArbol[y]
//      8          agregar {x, y} a T
//      9          Incorporar(G, y, enArbol, C)
//     10  return T
//
//     Incorporar(G, y, enArbol, C)
//     1  enArbol[y] = true
//     2  for cada <z, p> en Vecinos(G, y)
//     3      if no enArbol[z]
//     4          Insertar(C, <p, y, z>)
// ============================================================================

// Una arista de la cola de Prim: <peso, x, y>, con x el extremo que ya está en
// el árbol. Las tuplas se comparan de a un campo, empezando por el peso.
//     AristaDeCola e(p, x, y);        // armar una
//     std::get<0>(e)                  // el peso; std::get<1>(e) es x, etc.
typedef std::tuple<Peso, size_t, size_t> AristaDeCola;

// La cola de prioridad de Prim, ya armada como MIN-heap (sale primero la
// arista de menor peso).
//     cola.push(e)    Insertar
//     cola.top()      Minimo (sin sacarlo)
//     cola.pop()      EliminarMinimo (no devuelve nada)
//     cola.empty()    Vacia
typedef std::priority_queue<AristaDeCola, std::vector<AristaDeCola>, std::greater<AristaDeCola>> ColaDeAristas;

// Ejercicio 7 — Incorporar un vértice al árbol
// Marca a y como parte del árbol e inserta en la cola sus aristas hacia
// afuera.
// pre:  enArbol.size() == n, y < n y enArbol[y] == false.
// post: enArbol[y] == true (el resto de enArbol no cambia). La cola tiene lo
//       que tenía y, además, una entrada <p, y, z> por cada vecino z de y con
//       enArbol[z] == false, con p el peso de la arista {y, z}.
// Complejidad: O(d(y) log m).
void incorporar(const GrafoPonderado& g, size_t y, std::vector<bool>& enArbol, ColaDeAristas& cola);

// Ejercicio 8 — Prim
// Calcula un árbol generador mínimo de g haciendo crecer un árbol desde r,
// siguiendo el pseudocódigo de arriba.
// pre:  g es no dirigido y conexo, y r < n.
// post: res es un árbol generador de g de peso mínimo (n - 1 aristas), en el
//       orden en que Prim las fue agregando. Cada arista {v, w} figura con v
//       el extremo que YA estaba en el árbol y w el que se acaba de agregar
//       (la primera arista tiene v == r).
// Ejemplo (la tabla de la teórica, desde A): A-C, C-B, B-D, D-E, E-F.
// Ojo: una arista de la cola puede haber quedado obsoleta (cuando sale, sus
// dos extremos ya están en el árbol). Por eso la línea 7 vuelve a preguntar.
// Complejidad: O(m log m).
std::vector<Arista> prim(const GrafoPonderado& g, size_t r);

// ============================================================================
// PARTE 3 — CAMINO MÍNIMO (clase 19)
//
// El peso de un camino es la suma de los pesos de sus aristas; la distancia
// de v a w es el peso de un camino mínimo (INFINITO si no hay camino). El
// problema: dado un origen v, calcular la distancia de v a todos los vértices.
//
// Los grafos son ponderados, dirigidos o no. Los algoritmos mantienen un
// arreglo dist: dist[x] es el peso del mejor camino de v a x encontrado hasta
// ahora. Empieza en 0 para v y en INFINITO para los demás, y se va mejorando
// de a una arista.
// ============================================================================

// La distancia a un vértice al que (todavía) no se sabe llegar. Es el mayor
// Peso posible: ojo, INFINITO + algo se desborda, así que nunca sumes sobre un
// INFINITO.
const Peso INFINITO = std::numeric_limits<Peso>::max();

// El "padre" de un vértice que no tiene ninguno (el origen, o uno que no es
// alcanzable) en el árbol de caminos mínimos.
const size_t SIN_PADRE = static_cast<size_t>(-1);

// Ejercicio 9 — Mejorar una estimación con una arista
// "Si sé llegar a x con costo dist[x], y hay una arista de x a y de peso p,
// entonces sé llegar a y con costo dist[x] + p." Si eso mejora la estimación
// de y, la actualiza. Todos los algoritmos de camino mínimo se apoyan en esta
// operación (en la bibliografía se la llama relajar, relax).
//
//     Mejorar(x, y, p, dist)
//     1  if dist[x] + p < dist[y]
//     2      dist[y] = dist[x] + p
//     3      return true
//     4  return false
//
// pre:  x, y < dist.size() y hay una arista de x a y de ese peso (que puede
//       ser negativo).
// post: dist[y] <= dist[x] + peso (si dist[x] es INFINITO, no se sabe llegar
//       a x y no cambia nada); devuelve true si y sólo si dist[y] cambió.
//       Las demás posiciones de dist quedan igual.
// Ojo: en el pseudocódigo infinito + p "no es menor que nada", pero acá
// INFINITO + peso se desborda. Hay que chequear que dist[x] no sea INFINITO
// antes de sumar.
// Complejidad: O(1).
bool mejorar(size_t x, size_t y, Peso peso, std::vector<Peso>& dist);

// La cola de prioridad de Dijkstra: un MIN-heap de pares <distancia, vértice>
// (sale primero el de menor distancia y, si empatan, el de menor vértice).
// Se usa igual que la de Prim (push, top, pop, empty).
//     cola.push(DistanciaYVertice(0, v));
//     cola.top().first     // la distancia;  cola.top().second es el vértice
typedef std::pair<Peso, size_t> DistanciaYVertice;
typedef std::priority_queue<DistanciaYVertice, std::vector<DistanciaYVertice>, std::greater<DistanciaYVertice>> ColaDeDistancias;

// Ejercicio 10 — Dijkstra
// Calcula la distancia de v a todos los vértices. Idea: ir terminando los
// vértices de a uno, del más cercano al origen al más lejano.
//
//     Dijkstra(G, v)
//      1  n = CantidadDeVertices(G)
//      2  dist = arreglo de n números, todos en infinito
//      3  terminado = arreglo de n booleanos, todos en false
//      4  C = ColaDePrioridadVacia()
//      5  dist[v] = 0;  Insertar(C, <0, v>)
//      6  while no Vacia(C)
//      7      <d, x> = Minimo(C)
//      8      EliminarMinimo(C)
//      9      if no terminado[x]
//     10          terminado[x] = true
//     11          for cada <y, p> en Vecinos(G, x)
//     12              if Mejorar(x, y, p, dist)
//     13                  Insertar(C, <dist[y], y>)
//     14  return dist
//
// pre:  v < n y todos los pesos de g son >= 0.
// post: res tiene n posiciones; res[x] es la distancia de v a x (res[v] == 0),
//       o INFINITO si x no es alcanzable desde v.
// Ejemplo (la teórica, desde A): 0, 4, 3, 10, 12, 15.
// Ojo: un mismo vértice puede estar varias veces en la cola, con distintas
// distancias. La primera vez que sale es con la menor; las demás se descartan
// en la línea 9.
// Complejidad: O((n + m) log n).
std::vector<Peso> dijkstra(const GrafoPonderado& g, size_t v);

// Resultado de dijkstraConPadres: las distancias y, para cada vértice, el
// vértice desde el que se llegó la última vez que mejoró su distancia.
// Los punteros padre forman el ÁRBOL DE CAMINOS MÍNIMOS (con raíz en el
// origen): ojo, no es el árbol generador mínimo.
struct CaminosMinimos {
    std::vector<Peso> dist;
    std::vector<size_t> padre;
};

// Ejercicio 11 — No sólo la distancia: Dijkstra con padres
// Igual que el Ejercicio 10, pero además guarda para cada vértice y su padre:
// el x desde el que se mejoró dist[y] por última vez.
// pre:  v < n y todos los pesos de g son >= 0.
// post: res.dist es lo que devuelve dijkstra(g, v). res.padre tiene n
//       posiciones: res.padre[v] == SIN_PADRE, res.padre[y] == SIN_PADRE si y
//       no es alcanzable desde v; y para todo otro y alcanzable, res.padre[y]
//       es un vértice x con una arista de x a y de peso p tal que
//       res.dist[x] + p == res.dist[y].
// Pista: mejorar te dice si dist[y] cambió.
// Complejidad: O((n + m) log n).
CaminosMinimos dijkstraConPadres(const GrafoPonderado& g, size_t v);

// Ejercicio 12 — Reconstruir el camino
// Dado el arreglo de padres calculado desde v, reconstruye el camino de v a w
// yendo de w hacia atrás.
//
//     Camino(padre, v, w)
//     1  camino = Inicializar()
//     2  x = w
//     3  while x != v
//     4      InsertarAlPrincipio(camino, x)
//     5      x = padre[x]
//     6  InsertarAlPrincipio(camino, v)
//     7  return camino
//
// pre:  padre se calculó desde v (con dijkstraConPadres) y w es alcanzable
//       desde v.
// post: res es el camino de v a w: res[0] == v, res.back() == w, y cada
//       vértice (salvo v) tiene a su padre justo antes. Si v == w,
//       res == {v}.
// Pista: en un std::vector insertar al principio es caro. Podés insertar al
// final y dar vuelta el resultado (std::reverse, de <algorithm>).
// Complejidad: O(l), con l la cantidad de aristas del camino.
std::vector<size_t> reconstruirCamino(const std::vector<size_t>& padre, size_t v, size_t w);

// Ejercicio 13 — Camino mínimo
// Devuelve un camino de peso mínimo de v a w: junta los Ejercicios 11 y 12.
// pre:  v < n, w < n y todos los pesos de g son >= 0.
// post: - si w es alcanzable desde v: res es la secuencia de vértices de un
//         camino de peso mínimo (res[0] == v, res.back() == w; si v == w,
//         res == {v}). Si hay varios caminos mínimos, vale cualquiera.
//       - si no: res es vacío.
// Ejemplo (la teórica): de A a F es A, C, B, D, F, de peso 15.
// Complejidad: O((n + m) log n).
std::vector<size_t> caminoMinimo(const GrafoPonderado& g, size_t v, size_t w);

// Resultado de bellmanFord: el par <true, dist> o <false, -> de la teórica.
struct ResultadoBellmanFord {
    bool sinCicloNegativo;
    std::vector<Peso> dist;
};

// Ejercicio 14 — Bellman-Ford
// Con pesos negativos Dijkstra se equivoca. Bellman-Ford no intenta adivinar
// en qué orden terminar los vértices: aplica Mejorar a TODAS las aristas, una
// y otra vez. Y detecta los ciclos negativos (ciclos cuyo peso total es
// negativo: cada vuelta abarata el camino, y no hay camino mínimo).
//
//     BellmanFord(G, v)
//      1  n = CantidadDeVertices(G)
//      2  dist = arreglo de n números, todos en infinito
//      3  dist[v] = 0
//      4  for i = 1 to n - 1                    // n - 1 rondas
//      5      for cada <p, x, y> en Aristas(G)  // una ronda: todas las aristas
//      6          Mejorar(x, y, p, dist)
//      7  for cada <p, x, y> en Aristas(G)      // ronda de control
//      8      if dist[x] + p < dist[y]
//      9          return <false, ->             // hay un ciclo negativo
//     10  return <true, dist>
//
// "for cada <p, x, y> en Aristas(G)" es recorrer todas las listas: para cada
// vértice x, cada vecino <y, p> de g.ady[x].
// pre:  v < n. g es dirigido y sus pesos pueden ser negativos.
// post: - si hay algún ciclo negativo alcanzable desde v:
//         res.sinCicloNegativo == false (res.dist puede tener cualquier
//         contenido).
//       - si no: res.sinCicloNegativo == true y res.dist tiene n posiciones,
//         con res.dist[x] la distancia de v a x (res.dist[v] == 0), o
//         INFINITO si x no es alcanzable desde v. Un ciclo negativo que NO es
//         alcanzable desde v no cuenta.
// Ojo con la línea 8: si dist[x] es INFINITO no se puede sumar.
// Complejidad: O(n * (n + m)).
ResultadoBellmanFord bellmanFord(const GrafoPonderado& g, size_t v);

// ============================================================================
// PARTE 4 — OPCIONALES (para después)
//
// Lo que la teórica menciona alrededor de los algoritmos. No hace falta para
// las Partes 1 a 3.
// ============================================================================

// ---------------------------------------------------------------------------
// Kruskal con un bosque
//
// "Con una representación mejor de los conjuntos disjuntos, un bosque con
// unión por tamaño, las dos operaciones cuestan O(log n) y Kruskal baja a
// O(m log m)."
// ---------------------------------------------------------------------------

// Representación con un BOSQUE: cada grupo es un árbol, y el representante es
// la raíz de su árbol.
//   - padre[x]: el padre de x en su árbol. La raíz es su propio padre
//     (padre[r] == r).
//   - tam[r]: la cantidad de elementos del árbol cuya raíz es r. Sólo
//     importa en las raíces.
//
// Al unir dos grupos, se cuelga la raíz del árbol MÁS CHICO de la raíz del más
// grande ("unión por tamaño"): así ningún árbol se vuelve muy alto (la altura
// es a lo sumo log2 n) y encontrar la raíz cuesta O(log n).
struct Bosque {
    std::vector<size_t> padre;
    std::vector<size_t> tam;
};

// Ejercicio 15 (opcional) — Crear un bosque
// Crea un bosque de 0, ..., n - 1 con cada elemento en un árbol propio.
// pre:  true (n puede ser 0).
// post: res.padre[x] == x y res.tam[x] == 1 para todo x (n posiciones cada
//       uno).
// Complejidad: O(n).
Bosque crearBosque(size_t n);

// Ejercicio 16 (opcional) — Representante en el bosque
// Devuelve la raíz del árbol de x: el representante de su grupo.
// pre:  x < b.padre.size() y b se armó sólo con crearBosque y unirBosque.
// post: res es la raíz del árbol de x (la que es su propio padre).
// Complejidad: O(log n).
size_t representanteBosque(const Bosque& b, size_t x);

// Ejercicio 17 (opcional) — Unir en el bosque
// Junta los árboles de x y de y por tamaño: la raíz del árbol más chico (el
// de menor tam) pasa a ser hija de la raíz del más grande. Si tienen el mismo
// tam, la raíz de y pasa a ser hija de la raíz de x.
// pre:  x, y < b.padre.size() y x e y están en árboles distintos; b se armó
//       sólo con crearBosque y unirBosque.
// post: x e y quedan en el mismo grupo; el resto de los grupos no cambia.
// Complejidad: O(log n).
void unirBosque(Bosque& b, size_t x, size_t y);

// Ejercicio 18 (opcional) — Kruskal con el bosque
// El mismo algoritmo del Ejercicio 5, cambiando la Particion por el Bosque.
// pre:  g es no dirigido y conexo, y n >= 1.
// post: lo mismo que kruskal.
// Complejidad: O(m log m).
std::vector<Arista> kruskalConBosque(const GrafoPonderado& g);

// ---------------------------------------------------------------------------
// ¿Por qué funcionan? Algoritmos greedy
// ---------------------------------------------------------------------------

// Ejercicio 19 (opcional) — Arista más liviana de un corte
// Propiedad del corte: separemos los vértices en dos grupos no vacíos, de
// cualquier manera. La arista más liviana entre las que cruzan de un grupo al
// otro pertenece a algún AGM.
// enA[x] dice si x está en el grupo A (true) o en el otro (false). Devuelve la
// arista más liviana entre las que cruzan.
// pre:  g es no dirigido; enA.size() == n; hay al menos un vértice en cada
//       grupo y al menos una arista que cruza el corte.
// post: res es una arista de g con enA[res.v] == true, enA[res.w] == false y
//       el peso de la arista; ninguna otra arista que cruza el corte pesa
//       menos. Si hay varias del mismo peso mínimo, vale cualquiera.
// Ejemplo (la teórica): con los grupos {A, B, C} y {D, E, F} cruzan B-D (6),
// C-D (8) y C-E (9): la más liviana es B-D.
// Complejidad: O(n + m).
Arista aristaMasLivianaDelCorte(const GrafoPonderado& g, const std::vector<bool>& enA);

// Resultado de vueltoGreedy cuando el greedy se queda sin monedas que sirvan.
const size_t IMPOSIBLE = static_cast<size_t>(-1);

// Ejercicio 20 (opcional) — Vuelto con el algoritmo greedy
// La pregunta de la teórica. El greedy del vuelto: mientras falte dar vuelto,
// usar la moneda MÁS GRANDE que no se pase de lo que falta. Nunca se
// reconsidera una decisión. Devuelve cuántas monedas usa.
// pre:  todas las monedas son > 0; monedas puede estar en cualquier orden.
// post: res es la cantidad de monedas que usa el greedy para dar exactamente
//       monto (0 si monto == 0); o IMPOSIBLE si en algún momento falta algo
//       y ninguna moneda entra.
// Ejemplos: con monedas {1, 5, 10, 25}, monto 63 -> 6 (25 + 25 + 10 + 1 + 1 +
// 1); con monedas {1, 3, 4}, monto 6 -> 3 (4 + 1 + 1)... pero 3 + 3 es mejor:
// ¿el greedy es óptimo siempre?
// Complejidad: no hay una exigida.
size_t vueltoGreedy(const std::vector<size_t>& monedas, size_t monto);

// ---------------------------------------------------------------------------
// Bellman-Ford: ¿hacen falta todas las rondas?
// ---------------------------------------------------------------------------

// Ejercicio 21 (opcional) — Rondas que mejoran
// La pregunta de la teórica: si en una ronda completa ningún Mejorar cambió
// nada, las siguientes tampoco van a cambiar nada. Devuelve cuántas rondas
// mejoran al menos una distancia, cortando apenas haya una ronda que no
// mejore nada.
// pre:  v < n, g es dirigido y no tiene ciclos negativos alcanzables desde v.
// post: res == la cantidad de rondas (de las n - 1 posibles) en las que algún
//       mejorar cambió una distancia, cortando en la primera ronda que no
//       mejora nada. En cada ronda las aristas se recorren por origen x
//       creciente y, para el mismo x, en el orden de g.ady[x].
// Ejemplos: en un camino 0 -> 1 -> 2 -> 3 -> 4 desde 0, res == 1 (en una sola
// ronda ya está todo); en el camino con las aristas al revés, 4 -> 3 -> 2 ->
// 1 -> 0 desde 4, res == 4 (cada ronda sólo avanza un vértice). El orden en
// que se recorren las aristas cambia cuántas rondas hacen falta.
// Complejidad: O(n * (n + m)).
size_t rondasQueMejoran(const GrafoPonderado& g, size_t v);

#endif
