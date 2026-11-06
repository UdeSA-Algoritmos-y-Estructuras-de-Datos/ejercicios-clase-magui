# Ejercicios - Tut 11 (Grafos: representación)

Cáscara para resolver los ejercicios de la undécima clase: el **TAD Grafo** de
la teórica implementado con **listas de adyacencia**, algunos problemas que se
resuelven mirando la representación, la **alcanzabilidad con DFS** (con una
pila explícita) y sus aplicaciones, y el **BFS** (el mismo recorrido con una
cola) para calcular distancias y caminos más cortos.

## Archivos

- `ejercicios.h` — declaraciones de las funciones a implementar (con la descripción de cada una) y la estructura `Grafo`. **No hace falta modificarlo.**
- `ejercicios.cpp` — **acá va tu código**: cada función tiene una cáscara con un `TODO`.
- `main.cpp` — programa de prueba que chequea tus soluciones. No hace falta modificarlo.
- `Makefile` — para compilar con los flags de la materia y correr Valgrind.

## Cómo trabajar

1. Compilá el proyecto:

   ```bash
   make
   ```

2. Corré las pruebas:

   ```bash
   make run
   ```

   o, equivalente:

   ```bash
   ./ejercicios
   ```

3. Vas a ver una lista de `[PASA]` / `[FALLA]`. Al principio fallan muchas,
   porque las funciones están vacías (algunas pruebas, sobre todo las del
   tipo "esto **no** es un árbol / **no** son vecinos", pasan de casualidad:
   no te confíes). Implementá cada función en `ejercicios.cpp` y volvé a
   correr `make run` hasta que pasen todas.

4. **Opcional**: corré Valgrind para confirmar que no hay memory leaks ni
   accesos inválidos a memoria:

   ```bash
   make valgrind
   ```

5. Para borrar los archivos compilados:

   ```bash
   make clean
   ```

> Tip: los `(void)parametro;` en las cáscaras están solo para que el proyecto
> compile sin advertencias antes de que implementes cada función. Cuando
> empieces a usar ese parámetro, borrá la línea `(void)...`.

> Tip: si tu Mac es Apple Silicon (M1/M2/M3), `valgrind` no está disponible
> de forma nativa. En ese caso, usá:
>
> ```bash
> make docker-valgrind
> ```

> Si en la Parte 3 o la 4 el programa **se queda colgado**, casi seguro tu
> recorrido no está marcando los vértices como visitados y da vueltas por un
> ciclo del grafo. Cortalo con `Ctrl+C`.

## La estructura

Los vértices de un grafo de `n` vértices son `0, 1, ..., n - 1` (como en la
teórica) y se representan con `size_t`. Los grafos son **no dirigidos y no
ponderados** (salvo el Ejercicio 11, que es dirigido).

```cpp
struct Grafo {
    size_t n;                              // cantidad de vértices
    std::vector<std::vector<size_t>> ady;  // ady[v] = los vecinos de v
};
```

Es la representación con **listas de adyacencia**. Su **invariante de
representación** (es el Ejercicio 10):

- `ady` tiene `n` listas;
- `w` está en `ady[v]` **si y sólo si** `v` está en `ady[w]` (cada arista aparece dos veces, una por extremo);
- ninguna lista tiene repetidos;
- `ady[v]` no contiene a `v`.

### El TAD Grafo

Las seis operaciones de la teórica, con los costos que vimos para listas:

| Operación del TAD | Función | Costo |
|---|---|---|
| `GrafoSinAristas(n)` | `grafoSinAristas(n)` | O(n) |
| `CantidadDeVertices(G)` | `cantidadDeVertices(g)` | O(1) |
| `AgregarArista(G, v, w)` | `agregarArista(g, v, w)` | O(1) amortizado |
| `SonVecinos(G, v, w)` | `sonVecinos(g, v, w)` | O(d(v)) |
| `Vecinos(G, v)` | `vecinos(g, v)` | O(1) para obtenerla, O(d(v)) para recorrerla |
| `Grado(G, v)` | `grado(g, v)` | O(1) |

`vecinos` **devuelve una referencia** a la lista que ya está en `g.ady[v]`
(la teórica dice "sin copiarla"). Nunca devuelvas una referencia a una
variable local: se destruye al terminar la función.

> Ojo con los índices: son `size_t`. Con `-Wconversion` el compilador no te
> deja mezclarlos con `int` sin un `static_cast`, y recorrer para atrás con
> `i >= 0` no termina nunca (un `size_t` siempre es `>= 0`).

## Sobre los ejercicios

### Parte 1 — El TAD Grafo con listas de adyacencia (Ej. 1 al 6)

Las seis operaciones sobre `Grafo`. Son cortas: lo importante es entender
**por qué cuestan lo que cuestan**. Fijate qué es barato (`grado`, recorrer
los vecinos) y qué no (`sonVecinos`: hay que buscar en la lista). Y no te
olvides de la **simetría** en `agregarArista`: la arista va en las dos listas.
No hace falta buscar si la arista ya estaba: para eso está la precondición.

### Parte 2 — Trabajando con la representación (Ej. 7 al 11)

- **cantidadDeAristas**: la suma de los grados es 2m.
- **esCompleto**: en K_n, todo vértice tiene grado n - 1.
- **vecinosEnComun**: dos listas de vecinos, y lo que tienen en común.
- **cumpleInvariante**: chequear el invariante de representación de arriba.
- **gradosDeEntrada**: en un grafo **dirigido** con listas, calcular `d_in` de todos los vértices con una sola pasada, O(n + m).

### Parte 3 — Alcanzabilidad y DFS (Ej. 12 al 17)

- **esAlcanzableDFS**: el algoritmo de la teórica, tal cual: una **pila** de pendientes que arranca con `v`, un arreglo de **visitados**, y el ciclo "sacar el tope, ¿es `w`?, ¿ya lo visité?, si no, marcarlo y apilar a los vecinos no visitados".
- **ordenDeVisitaDFS**: el mismo recorrido, devolviendo el orden en que se marcan los visitados. Tiene que reproducir la **tabla del ejemplo de la teórica** (desde 0: `0, 5, 6, 7, 4, 2, 3, 1`).
- **cantidadDeComponentes**, **esConexo**, **esArbol**: se construyen sobre el recorrido (lanzarlo desde cada vértice sin visitar) y sobre la caracterización "árbol = conexo con exactamente n - 1 aristas".
- **esBipartito**: el 2-coloreo de la teórica, con una pila. Tiene que andar también si el grafo **no es conexo**.

Para la pila alcanza con la de la STL:

```cpp
#include <stack>

std::stack<size_t> pila;
pila.push(v);       // apilar
pila.top();         // mirar el tope (sin sacarlo)
pila.pop();         // sacar el tope (no devuelve nada)
pila.empty();       // ¿está vacía?
```

### Parte 4 — BFS (Ej. 18 al 21)

Es el mismo recorrido que el DFS, pero con una **cola** en lugar de una pila:
el recorrido avanza **por niveles** (primero el vértice de partida, después sus
vecinos, después los vecinos de esos...), y por eso el primer camino que
encuentra a un vértice es de los **más cortos**.

- **ordenDeVisitaBFS**: el orden en que el BFS saca los vértices de la cola.
- **distanciasDesde**: la distancia mínima (en aristas) de `v` a todos los vértices; `NO_ALCANZABLE` para los que no se alcanzan.
- **caminoMasCorto**: un camino de `v` a `w` con la menor cantidad de aristas. Hace falta recordar quién descubrió a cada vértice y reconstruir el camino hacia atrás.
- **verticesADistancia**: los vértices a distancia exactamente `k`.

Una diferencia con el DFS: acá conviene **marcar al encolar**, no al sacar. Así
cada vértice entra a la cola una sola vez y su distancia se calcula en el
momento de descubrirlo (la del vértice que lo descubrió, más uno).

Para la cola, la de la STL:

```cpp
#include <queue>

std::queue<size_t> cola;
cola.push(v);       // encolar (al final)
cola.front();       // mirar el primero (sin sacarlo)
cola.pop();         // sacar el primero (no devuelve nada)
cola.empty();       // ¿está vacía?
```

> Repasá los apuntes (`Tut-11 - Grafos - Representacion - apuntes.md`) antes
> de empezar: el DFS (por qué un vértice puede quedar **repetido** en la pila
> y por qué hay que revisar si fue visitado **al sacarlo**) es el punto donde
> más se tropieza.
