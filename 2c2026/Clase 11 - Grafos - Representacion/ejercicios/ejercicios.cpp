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
// Los vértices son size_t. Ojo con -Wconversion: no mezcles size_t con int
// sin un static_cast. Y ojo con recorrer para atrás: con size_t, `i >= 0` es
// SIEMPRE verdadero.
//
// Para las Partes 2, 3 y 4 vas a necesitar algunos headers de la STL; agregalos
// arriba según los uses (por ejemplo, <stack> para la pila del DFS, y
// <algorithm> para std::find o std::sort).
// ============================================================================

// ============================================================================
// PARTE 1 — EL TAD GRAFO CON LISTAS DE ADYACENCIA
// ============================================================================

// Ejercicio 1 — GrafoSinAristas
Grafo grafoSinAristas(size_t n)
{
    // TODO: implementar grafoSinAristas.
    // pre:  true (n puede ser 0).
    // post: res.n == n y res.ady tiene n listas de vecinos, todas vacías.
    //
    // Pista: para crear un vector de vectores con n vectores vacíos se puede
    // usar el constructor vector<T>(n), que crea n elementos por defecto:
    //     std::vector<std::vector<size_t>>(n)
    // (también sirve g.ady.resize(n)).
    (void)n;
    Grafo g;
    g.n = 0;
    return g;
}

// Ejercicio 2 — CantidadDeVertices
size_t cantidadDeVertices(const Grafo& g)
{
    // TODO: implementar cantidadDeVertices.
    // pre:  true.
    // post: res == n.
    (void)g;
    return 0;
}

// Ejercicio 3 — AgregarArista
void agregarArista(Grafo& g, size_t v, size_t w)
{
    // TODO: implementar agregarArista.
    // pre:  v < n, w < n, v != w y {v, w} todavía no es una arista de g.
    // post: {v, w} es una arista de g; el resto del grafo queda igual.
    // Acordate de que w tiene que quedar en la lista de v, y v en la de w.
    (void)g;
    (void)v;
    (void)w;
}

// Ejercicio 4 — SonVecinos
bool sonVecinos(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar sonVecinos.
    // pre:  v < n, w < n.
    // post: res == true si y sólo si {v, w} es una arista de g.
    (void)g;
    (void)v;
    (void)w;
    return false;
}

// Ejercicio 5 — Vecinos
const std::vector<size_t>& vecinos(const Grafo& g, size_t v)
{
    // TODO: implementar vecinos.
    // pre:  v < n.
    // post: res es la lista de vecinos de v, SIN copiarla (devolvé una
    //       referencia a la lista que ya está dentro de g).
    // Nunca devuelvas una
    // referencia a una variable local: se destruye al salir de la función.
    // La línea de abajo sólo existe para que la cáscara compile; borrala.
    (void)g;
    (void)v;
    static const std::vector<size_t> vacio;
    return vacio;
}

// Ejercicio 6 — Grado
size_t grado(const Grafo& g, size_t v)
{
    // TODO: implementar grado.
    // pre:  v < n.
    // post: res == d(v), la cantidad de vecinos de v.
    (void)g;
    (void)v;
    return 0;
}

// ============================================================================
// PARTE 2 — TRABAJANDO CON LA REPRESENTACIÓN
// ============================================================================

// Ejercicio 7 — Cantidad de aristas
size_t cantidadDeAristas(const Grafo& g)
{
    // TODO: implementar cantidadDeAristas.
    // pre:  true.
    // post: res == m, la cantidad de aristas de g.
    // Pensá en la suma de los grados.
    (void)g;
    return 0;
}

// Ejercicio 8 — ¿Es completo?
bool esCompleto(const Grafo& g)
{
    // TODO: implementar esCompleto.
    // pre:  true.
    // post: res == true si y sólo si todo par de vértices distintos es una
    //       arista. Con 0 o 1 vértices, el grafo es completo.
    (void)g;
    return false;
}

// Ejercicio 9 — Vecinos en común
std::vector<size_t> vecinosEnComun(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar vecinosEnComun.
    // pre:  v < n, w < n.
    // post: res tiene exactamente a los vértices que son vecinos de v y de w,
    //       en orden creciente y sin repetidos.
    (void)g;
    (void)v;
    (void)w;
    return {};
}

// Ejercicio 10 — Invariante de representación
bool cumpleInvariante(const Grafo& g)
{
    // TODO: implementar cumpleInvariante.
    // pre:  true (g puede estar mal armado: es justo lo que se chequea).
    // post: res == true si y sólo si ady tiene n listas, con vértices
    //       válidos (< n), sin repetidos, sin el propio vértice, y w está en
    //       ady[v] <=> v está en ady[w].
    (void)g;
    return false;
}

// Ejercicio 11 — Grados de entrada (grafo dirigido)
std::vector<size_t> gradosDeEntrada(const std::vector<std::vector<size_t>>& salientes)
{
    // TODO: implementar gradosDeEntrada.
    // pre:  todos los elementos de las listas de salientes son <
    //       salientes.size().
    // post: res tiene salientes.size() posiciones y res[v] es la cantidad de
    //       veces que v aparece en las listas de salientes (d_in(v)).
    (void)salientes;
    return {};
}

// ============================================================================
// PARTE 3 — ALCANZABILIDAD Y DFS
// ============================================================================

// Ejercicio 12 — Alcanzabilidad con DFS
bool esAlcanzableDFS(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar esAlcanzableDFS.
    // pre:  v < n, w < n.
    // post: res == true si y sólo si hay un camino de v a w en g (si v == w,
    //       el camino de longitud 0 cuenta).
    // Pila de pendientes con v, arreglo de visitados, y el ciclo de la
    // teórica. Recordá: al sacar un vértice de la pila, primero preguntá si
    // es w, y después si ya fue visitado.
    (void)g;
    (void)v;
    (void)w;
    return false;
}

// Ejercicio 13 — Orden de visita del DFS
std::vector<size_t> ordenDeVisitaDFS(const Grafo& g, size_t v)
{
    // TODO: implementar ordenDeVisitaDFS.
    // pre:  v < n.
    // post: res tiene exactamente a los vértices alcanzables desde v, sin
    //       repetidos, en el orden en que el DFS los marca como visitados
    //       (res[0] == v).
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 14 — Componentes conexas
size_t contarComponentes(const Grafo& g)
{
    // TODO: implementar contarComponentes.
    // pre:  true.
    // post: res == cantidad de componentes conexas de g (un grafo sin
    //       vértices tiene 0).
    (void)g;
    return 0;
}

// Ejercicio 15 — ¿Es conexo?
bool esConexo(const Grafo& g)
{
    // TODO: implementar esConexo.
    // pre:  n >= 1.
    // post: res == true si y sólo si hay un camino entre todo par de vértices.
    (void)g;
    return false;
}

// Ejercicio 16 — ¿Es un árbol?
bool esArbol(const Grafo& g)
{
    // TODO: implementar esArbol.
    // pre:  n >= 1.
    // post: res == true si y sólo si g es conexo y no tiene ciclos.
    (void)g;
    return false;
}

// Ejercicio 17 — ¿Hay un ciclo?
bool hayCicloDesde(const Grafo& g, size_t v)
{
    // TODO: implementar hayCicloDesde.
    // pre:  v < n.
    // post: res == true si y sólo si hay un ciclo entre los vértices
    //       alcanzables desde v (un ciclo en otra componente no cuenta).
    (void)g;
    (void)v;
    return false;
}

// ============================================================================
// PARTE 4 — BFS: EL MISMO RECORRIDO, CON UNA COLA
// ============================================================================

// Ejercicio 18 — Orden de visita del BFS
std::vector<size_t> ordenDeVisitaBFS(const Grafo& g, size_t v)
{
    // TODO: implementar ordenDeVisitaBFS.
    // pre:  v < n.
    // post: res tiene exactamente a los vértices alcanzables desde v, sin
    //       repetidos, en el orden en que el BFS los saca de la cola
    //       (res[0] == v).
    // Marcá como visitado al ENCOLAR.
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 19 — Distancias desde un vértice
std::vector<size_t> distanciasDesde(const Grafo& g, size_t v)
{
    // TODO: implementar distanciasDesde.
    // pre:  v < n.
    // post: res tiene n posiciones; res[w] es la distancia mínima de v a w
    //       (res[v] == 0), o NO_ALCANZABLE si w no es alcanzable desde v.
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 20 — Camino más corto
std::vector<size_t> caminoMasCorto(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar caminoMasCorto.
    // pre:  v < n, w < n.
    // post: si w es alcanzable desde v, res es un camino de v a w con la
    //       menor cantidad de aristas (res[0] == v, res.back() == w); si no,
    //       res es vacío.
    (void)g;
    (void)v;
    (void)w;
    return {};
}

// Ejercicio 21 — Vértices a distancia k
std::vector<size_t> verticesADistancia(const Grafo& g, size_t v, size_t k)
{
    // TODO: implementar verticesADistancia.
    // pre:  v < n.
    // post: res tiene exactamente a los vértices a distancia mínima k de v,
    //       en orden creciente (con k = 0, res == {v}).
    (void)g;
    (void)v;
    (void)k;
    return {};
}

// Ejercicio 22 — ¿Es bipartito?
bool esBipartito(const Grafo& g)
{
    // TODO: implementar esBipartito.
    // pre:  true (g puede no ser conexo).
    // post: res == true si y sólo si los vértices se pueden partir en dos
    //       conjuntos tales que toda arista une un vértice de cada uno.
    (void)g;
    return false;
}
