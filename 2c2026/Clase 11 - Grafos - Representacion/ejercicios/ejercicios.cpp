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
    // Se espera un grafo con n vértices y n listas de vecinos vacías.
    (void)n;
    Grafo g;
    g.n = 0;
    return g;
}

// Ejercicio 2 — CantidadDeVertices
size_t cantidadDeVertices(const Grafo& g)
{
    // TODO: implementar cantidadDeVertices .
    (void)g;
    return 0;
}

// Ejercicio 3 — AgregarArista
void agregarArista(Grafo& g, size_t v, size_t w)
{
    // TODO: implementar agregarArista .
    // Se espera que, después de la llamada, {v, w} sea una arista. Acordate
    // de la simetría.
    (void)g;
    (void)v;
    (void)w;
}

// Ejercicio 4 — SonVecinos
bool sonVecinos(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar sonVecinos .
    (void)g;
    (void)v;
    (void)w;
    return false;
}

// Ejercicio 5 — Vecinos
const std::vector<size_t>& vecinos(const Grafo& g, size_t v)
{
    // TODO: implementar vecinos .
    // Se espera la lista de vecinos de v, SIN copiarla (devolvé una
    // referencia a la lista que ya está dentro de g). Nunca devuelvas una
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
    // TODO: implementar grado .
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
    // Se espera m. Pensá en la suma de los grados.
    (void)g;
    return 0;
}

// Ejercicio 8 — ¿Es completo?
bool esCompleto(const Grafo& g)
{
    // TODO: implementar esCompleto.
    // Con 0 o 1 vértices, el grafo es completo.
    (void)g;
    return false;
}

// Ejercicio 9 — Vecinos en común
std::vector<size_t> vecinosEnComun(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar vecinosEnComun.
    // Se espera el resultado en orden creciente y sin repetidos.
    (void)g;
    (void)v;
    (void)w;
    return {};
}

// Ejercicio 10 — Invariante de representación
bool cumpleInvariante(const Grafo& g)
{
    // TODO: implementar cumpleInvariante .
    // Se espera true si g tiene n listas, con vértices válidos, sin
    // repetidos, sin el propio vértice, y con la simetría w en ady[v] <=> v
    // en ady[w].
    (void)g;
    return false;
}

// Ejercicio 11 — Grados de entrada (grafo dirigido)
std::vector<size_t> gradosDeEntrada(const std::vector<std::vector<size_t>>& salientes)
{
    // TODO: implementar gradosDeEntrada.
    // Se espera un vector de salientes.size() posiciones con d_in de cada
    // vértice.
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
    // Se espera el orden en que el DFS marca como visitados a los vértices
    // alcanzables desde v.
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 14 — Componentes conexas
size_t cantidadDeComponentes(const Grafo& g)
{
    // TODO: implementar cantidadDeComponentes.
    // Se espera la cantidad de componentes conexas. Un grafo sin vértices
    // tiene 0.
    (void)g;
    return 0;
}

// Ejercicio 15 — ¿Es conexo?
bool esConexo(const Grafo& g)
{
    // TODO: implementar esConexo.
    // Precondición: g tiene al menos un vértice.
    (void)g;
    return false;
}

// Ejercicio 16 — ¿Es un árbol?
bool esArbol(const Grafo& g)
{
    // TODO: implementar esArbol.
    // Precondición: g tiene al menos un vértice.
    (void)g;
    return false;
}

// Ejercicio 17 — ¿Es bipartito?
bool esBipartito(const Grafo& g)
{
    // TODO: implementar esBipartito.
    // El grafo puede no ser conexo.
    (void)g;
    return false;
}

// ============================================================================
// PARTE 4 — BFS: EL MISMO RECORRIDO, CON UNA COLA
// ============================================================================

// Ejercicio 18 — Orden de visita del BFS
std::vector<size_t> ordenDeVisitaBFS(const Grafo& g, size_t v)
{
    // TODO: implementar ordenDeVisitaBFS.
    // Se espera el orden en que el BFS saca de la cola a los vértices
    // alcanzables desde v. Marcá como visitado al ENCOLAR.
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 19 — Distancias desde un vértice
std::vector<size_t> distanciasDesde(const Grafo& g, size_t v)
{
    // TODO: implementar distanciasDesde.
    // Se espera un vector de g.n posiciones con la distancia mínima de v a
    // cada vértice, o NO_ALCANZABLE.
    (void)g;
    (void)v;
    return {};
}

// Ejercicio 20 — Camino más corto
std::vector<size_t> caminoMasCorto(const Grafo& g, size_t v, size_t w)
{
    // TODO: implementar caminoMasCorto.
    // Se espera la secuencia de vértices de v a w, o vacía si no hay camino.
    (void)g;
    (void)v;
    (void)w;
    return {};
}

// Ejercicio 21 — Vértices a distancia k
std::vector<size_t> verticesADistancia(const Grafo& g, size_t v, size_t k)
{
    // TODO: implementar verticesADistancia.
    // Se espera los vértices a distancia exactamente k de v, en orden
    // creciente.
    (void)g;
    (void)v;
    (void)k;
    return {};
}
