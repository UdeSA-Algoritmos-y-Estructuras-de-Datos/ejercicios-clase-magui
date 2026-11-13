#include "ejercicios.h"

#include <algorithm> // std::sort, std::reverse

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
// El pseudocódigo de cada algoritmo (el de la teórica) está en
// "ejercicios.h", arriba de cada ejercicio.
//
// Los vértices son size_t. Ojo con -Wconversion: no mezcles size_t con int
// sin un static_cast. Y ojo con recorrer para atrás: con size_t, `i >= 0` es
// SIEMPRE verdadero.
//
// Las colas de prioridad ya están armadas en "ejercicios.h": ColaDeAristas
// para Prim y ColaDeDistancias para Dijkstra.
// ============================================================================

// ============================================================================
// PARTE 1 — KRUSKAL
// ============================================================================

// Ejercicio 1 — CrearParticion
Particion crearParticion(size_t n)
{
    // TODO: implementar crearParticion.
    // pre:  true (n puede ser 0).
    // post: res tiene n posiciones y res[x] == x para todo x (cada elemento
    //       es el representante de su propio grupo).
    (void)n;
    return Particion();
}

// Ejercicio 2 — Representante
size_t representante(const Particion& p, size_t x)
{
    // TODO: implementar representante.
    // pre:  x < p.size().
    // post: res es el representante del grupo de x.
    (void)p;
    (void)x;
    return 0;
}

// Ejercicio 3 — Unir
void unir(Particion& p, size_t x, size_t y)
{
    // TODO: implementar unir.
    // pre:  x, y < p.size() y representante(p, x) != representante(p, y).
    // post: todos los elementos de los dos grupos pasan a tener el
    //       representante que tenía x; el resto queda igual.
    // Hay que recorrer el arreglo buscando a los que tienen el representante
    // del grupo de y. Por eso Unir cuesta O(n).
    (void)p;
    (void)x;
    (void)y;
}

// Dice si la arista a va antes que la arista b al ordenar por peso. La usa
// el std::sort del Ejercicio 4. (Ya está hecha: no hay que tocarla.)
static bool pesaMenos(const Arista& a, const Arista& b)
{
    return a.peso < b.peso;
}

// Ejercicio 4 — Las aristas de G, ordenadas por peso
std::vector<Arista> aristasOrdenadasPorPeso(const GrafoPonderado& g)
{
    // pre:  g es no dirigido.
    // post: res tiene exactamente una Arista por cada arista de g, con v < w
    //       y su peso, ordenadas por peso de menor a mayor (si dos pesan lo
    //       mismo, en cualquier orden).
    std::vector<Arista> res;

    // TODO: juntar en res todas las aristas de g, cada una UNA sola vez.
    // Cada arista {x, y} está en dos listas (en la de x y en la de y): hay
    // que quedarse con una sola de las dos, la que tiene v < w.
    // Para agregar una: res.push_back(Arista{v, w, peso}).
    (void)g;

    // El ordenamiento ya está hecho: std::sort ordena res de principio a fin
    // (res.begin(), res.end()), y pesaMenos le dice cuándo una arista va
    // antes que otra. Cuesta O(m log m).
    std::sort(res.begin(), res.end(), pesaMenos);
    return res;
}

// Ejercicio 5 — Kruskal
std::vector<Arista> kruskal(const GrafoPonderado& g)
{
    // TODO: implementar kruskal.
    // pre:  g es no dirigido y conexo, y n >= 1.
    // post: res es un árbol generador de g de peso mínimo, con las aristas en
    //       el orden en que se eligieron.
    // Es el pseudocódigo de la teórica, usando los Ejercicios 1 a 4.
    (void)g;
    return {};
}

// Ejercicio 6 — Peso de un árbol
Peso pesoDeAristas(const std::vector<Arista>& aristas)
{
    // TODO: implementar pesoDeAristas.
    // pre:  true.
    // post: res es la suma de los pesos de las aristas (0 si no hay).
    (void)aristas;
    return 0;
}

// ============================================================================
// PARTE 2 — PRIM
// ============================================================================

// Ejercicio 7 — Incorporar un vértice al árbol
void incorporar(const GrafoPonderado& g, size_t y, std::vector<bool>& enArbol, ColaDeAristas& cola)
{
    // TODO: implementar incorporar.
    // pre:  enArbol.size() == n, y < n y enArbol[y] == false.
    // post: enArbol[y] == true; la cola tiene, además de lo que tenía, una
    //       entrada <p, y, z> por cada vecino z de y que no está en el árbol.
    // Para insertar: cola.push(AristaDeCola(p, y, z)).
    (void)g;
    (void)y;
    (void)enArbol;
    (void)cola;
}

// Ejercicio 8 — Prim
std::vector<Arista> prim(const GrafoPonderado& g, size_t r)
{
    // TODO: implementar prim.
    // pre:  g es no dirigido y conexo, y r < n.
    // post: res es un árbol generador de g de peso mínimo, con las aristas en
    //       el orden en que se agregaron; cada una con v el extremo que ya
    //       estaba en el árbol y w el nuevo (la primera tiene v == r).
    // Es el pseudocódigo de la teórica, usando el Ejercicio 7.
    // Cuidado: una arista de la cola puede haber quedado obsoleta.
    //
    // Pista: cómo sacar el mínimo de la cola y leer los valores de la tupla
    // <p, x, y> (std::get<i> devuelve el valor de la posición i):
    //     AristaDeCola e = cola.top();   // <p, x, y> = Minimo(C)
    //     cola.pop();                    // EliminarMinimo(C)
    //     Peso p = std::get<0>(e);       // el peso
    //     size_t x = std::get<1>(e);     // el extremo que ya está en el árbol
    //     size_t y = std::get<2>(e);     // el otro extremo
    (void)g;
    (void)r;
    return {};
}

// ============================================================================
// PARTE 3 — CAMINO MÍNIMO
// ============================================================================

// Ejercicio 9 — Mejorar una estimación con una arista
bool mejorar(size_t x, size_t y, Peso peso, std::vector<Peso>& dist)
{
    // TODO: implementar mejorar.
    // pre:  x, y < dist.size() y hay una arista de x a y de ese peso.
    // post: dist[y] <= dist[x] + peso (si dist[x] es INFINITO, no cambia
    //       nada); devuelve true si y sólo si dist[y] cambió.
    // Ojo con el desborde: INFINITO + peso no se puede calcular.
    (void)x;
    (void)y;
    (void)peso;
    (void)dist;
    return false;
}

// Ejercicio 10 — Dijkstra
std::vector<Peso> dijkstra(const GrafoPonderado& g, size_t v)
{
    // TODO: implementar dijkstra.
    // pre:  v < n y todos los pesos de g son >= 0.
    // post: res tiene n posiciones; res[x] es la distancia de v a x, o
    //       INFINITO si x no es alcanzable desde v.
    // Es el pseudocódigo de la teórica, usando el Ejercicio 9 y una
    // ColaDeDistancias.
    // Cuidado: un vértice puede estar varias veces en la cola de prioridad.
    //
    // Pista: cómo insertar, sacar el mínimo y leer los valores del par
    // <d, x> (.first es el primer valor y .second el segundo):
    //     cola.push(DistanciaYVertice(0, v));   // Insertar(C, <0, v>)
    //     DistanciaYVertice e = cola.top();     // <d, x> = Minimo(C)
    //     cola.pop();                           // EliminarMinimo(C)
    //     Peso d = e.first;                     // la distancia
    //     size_t x = e.second;                  // el vértice
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 11 — No sólo la distancia: Dijkstra con padres
CaminosMinimos dijkstraConPadres(const GrafoPonderado& g, size_t v)
{
    // TODO: implementar dijkstraConPadres.
    // pre:  v < n y todos los pesos de g son >= 0.
    // post: res.dist es lo que devuelve dijkstra(g, v). res.padre[v] y el
    //       padre de los no alcanzables son SIN_PADRE; para el resto,
    //       res.padre[y] es un x con una arista de x a y de peso p tal que
    //       res.dist[x] + p == res.dist[y].
    // El padre de y se actualiza cada vez que dist[y] mejora.
    (void)g;
    (void)v;
    return CaminosMinimos();
}

// Ejercicio 12 — Reconstruir el camino
std::vector<size_t> reconstruirCamino(const std::vector<size_t>& padre, size_t v, size_t w)
{
    // TODO: implementar reconstruirCamino.
    // pre:  padre se calculó desde v y w es alcanzable desde v.
    // post: res es el camino de v a w: res[0] == v, res.back() == w, y cada
    //       vértice (salvo v) tiene a su padre justo antes. Si v == w,
    //       res == {v}.
    // Se arma yendo de w hacia v siguiendo los padres.
    (void)padre;
    (void)v;
    (void)w;
    return {};
}

// Ejercicio 13 — Camino mínimo
std::vector<size_t> caminoMinimo(const GrafoPonderado& g, size_t v, size_t w)
{
    // TODO: implementar caminoMinimo.
    // pre:  v < n, w < n y todos los pesos de g son >= 0.
    // post: si w es alcanzable desde v, res es un camino de peso mínimo de v
    //       a w (res[0] == v, res.back() == w); si no, res es vacío.
    // Juntá los Ejercicios 11 y 12. ¿Cómo sabés si w es alcanzable?
    (void)g;
    (void)v;
    (void)w;
    return {};
}

// Ejercicio 14 — Bellman-Ford
ResultadoBellmanFord bellmanFord(const GrafoPonderado& g, size_t v)
{
    // TODO: implementar bellmanFord.
    // pre:  v < n. g es dirigido y sus pesos pueden ser negativos.
    // post: si hay un ciclo negativo alcanzable desde v, res.sinCicloNegativo
    //       es false. Si no, es true y res.dist[x] es la distancia de v a x
    //       (INFINITO si no es alcanzable).
    // Es el pseudocódigo de la teórica, usando el Ejercicio 9. Recorrer
    // "todas las aristas" es recorrer todas las listas de g.ady.
    // Ojo con las aristas que salen de vértices con dist INFINITO: en la
    // ronda de control no se puede sumar sobre ellas.
    (void)g;
    (void)v;
    return ResultadoBellmanFord();
}

// ============================================================================
// PARTE 4 — OPCIONALES (para después)
// ============================================================================

// Ejercicio 15 (opcional) — Crear un bosque
Bosque crearBosque(size_t n)
{
    // TODO: implementar crearBosque.
    // pre:  true (n puede ser 0).
    // post: res.padre[x] == x y res.tam[x] == 1 para todo x (n posiciones
    //       cada uno).
    (void)n;
    return Bosque();
}

// Ejercicio 16 (opcional) — Representante en el bosque
size_t representanteBosque(const Bosque& b, size_t x)
{
    // TODO: implementar representanteBosque.
    // pre:  x < b.padre.size() y b se armó sólo con crearBosque y unirBosque.
    // post: res es la raíz del árbol de x (la que es su propio padre).
    // Hay que subir de padre en padre hasta llegar a una raíz.
    (void)b;
    (void)x;
    return 0;
}

// Ejercicio 17 (opcional) — Unir en el bosque
void unirBosque(Bosque& b, size_t x, size_t y)
{
    // TODO: implementar unirBosque.
    // pre:  x, y < b.padre.size() y están en árboles distintos.
    // post: x e y quedan en el mismo grupo: la raíz del árbol más chico pasa
    //       a ser hija de la raíz del más grande (si empatan, la de y cuelga
    //       de la de x), y se actualiza el tam de la raíz que queda.
    // Los tamaños que importan son los de las raíces.
    (void)b;
    (void)x;
    (void)y;
}

// Ejercicio 18 (opcional) — Kruskal con el bosque
std::vector<Arista> kruskalConBosque(const GrafoPonderado& g)
{
    // TODO: implementar kruskalConBosque.
    // pre:  g es no dirigido y conexo, y n >= 1.
    // post: lo mismo que kruskal.
    // Es tu kruskal del Ejercicio 5, cambiando las tres operaciones de la
    // Particion por las del Bosque.
    (void)g;
    return {};
}

// Ejercicio 19 (opcional) — Arista más liviana de un corte
Arista aristaMasLivianaDelCorte(const GrafoPonderado& g, const std::vector<bool>& enA)
{
    // TODO: implementar aristaMasLivianaDelCorte.
    // pre:  g es no dirigido, enA.size() == n, hay vértices en los dos grupos
    //       y al menos una arista que cruza.
    // post: res es una arista de g con enA[res.v] == true y enA[res.w] ==
    //       false, y ninguna otra arista que cruza el corte pesa menos.
    // Alcanza con mirar los vecinos de los vértices del grupo A: ¿cuáles de
    // esas aristas cruzan?
    (void)g;
    (void)enA;
    return Arista{0, 0, 0};
}

// Ejercicio 20 (opcional) — Vuelto con el algoritmo greedy
size_t vueltoGreedy(const std::vector<size_t>& monedas, size_t monto)
{
    // TODO: implementar vueltoGreedy.
    // pre:  todas las monedas son > 0 (en cualquier orden).
    // post: res es la cantidad de monedas que usa el greedy "siempre la más
    //       grande que entre" para dar exactamente monto (0 si monto == 0), o
    //       IMPOSIBLE si en algún momento falta algo y ninguna moneda entra.
    (void)monedas;
    (void)monto;
    return 0;
}

// Ejercicio 21 (opcional) — Rondas que mejoran
size_t rondasQueMejoran(const GrafoPonderado& g, size_t v)
{
    // TODO: implementar rondasQueMejoran.
    // pre:  v < n, g es dirigido y no tiene ciclos negativos alcanzables
    //       desde v.
    // post: res es la cantidad de rondas (de las n - 1 posibles) en las que
    //       algún mejorar cambió una distancia, cortando en la primera ronda
    //       que no cambia nada. En cada ronda las aristas se recorren por
    //       origen x creciente y, para el mismo x, en el orden de g.ady[x].
    (void)g;
    (void)v;
    return 0;
}
