# Ejercicios - Tut 12 (Grafos: AGM y camino mínimo)

Cáscara para resolver los ejercicios de la duodécima clase. Siguen el orden de
las teóricas (clases 18 y 19) y cada parte arma **un algoritmo**, con el mismo
pseudocódigo de los handouts: primero **Kruskal**, después **Prim** y al final
**camino mínimo** (Dijkstra y Bellman-Ford).

## Archivos

- `ejercicios.h` — declaraciones de las funciones a implementar (con la descripción de cada una) y las estructuras. **No hace falta modificarlo.**
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

3. Vas a ver una lista de `[PASA]` / `[FALLA]`, en el orden de los
   ejercicios. Al principio fallan casi todas, porque las funciones están
   vacías (algunas pruebas, sobre todo las de casos vacíos o las del tipo
   "**no** hay camino", pasan de casualidad: no te confíes). Implementá cada
   función en `ejercicios.cpp` y volvé a correr `make run`. Al final se
   informan por separado las pruebas de las Partes 1 a 3 y las de los
   opcionales.

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

> Si el programa **se queda colgado**, casi seguro alguno de tus recorridos
> no avanza (por ejemplo, una cola de prioridad que nunca se vacía porque
> volvés a insertar vértices ya terminados). Cortalo con `Ctrl+C`.

## Las estructuras

Los vértices de un grafo de `n` vértices son `0, 1, ..., n - 1` (como en la
teórica) y se representan con `size_t`. Los pesos son `Peso` (un `long long`):
pueden ser negativos.

```cpp
struct Vecino {
    size_t vertice;
    Peso peso;
};

struct GrafoPonderado {
    size_t n;                                 // cantidad de vértices
    std::vector<std::vector<Vecino>> ady;     // ady[x] = los vecinos de x, con el peso
};

struct Arista {
    size_t v;
    size_t w;
    Peso peso;
};
```

Es la representación con **listas de adyacencia**, ahora guardando el peso de
cada arista junto al vecino (en la teórica, `Vecinos(G, x)` devuelve pares
`<vecino, peso>`). Según el grafo:

- **No dirigido** (Kruskal y Prim): cada arista `{x, y}` aparece dos veces, en `ady[x]` y en `ady[y]`, con el mismo peso.
- **Dirigido** (Bellman-Ford; Dijkstra acepta los dos): la arista `(x, y)` aparece una sola vez, en `ady[x]`. `ady[x]` son los vecinos **salientes**.

Los grafos de las pruebas ya vienen armados: no hay que construirlos.

> Ojo con los índices: son `size_t`. Con `-Wconversion` el compilador no te
> deja mezclarlos con `int` sin un `static_cast`, y recorrer para atrás con
> `i >= 0` no termina nunca (un `size_t` siempre es `>= 0`).

## Sobre los ejercicios

El pseudocódigo de cada algoritmo está copiado en `ejercicios.h`, arriba del
ejercicio que lo implementa. En cada parte, los primeros ejercicios son las
piezas y el último las junta.

### Parte 1 — Kruskal (Ej. 1 al 6)

*Recorrer las aristas de la más liviana a la más pesada; cada una se agrega,
salvo que forme un ciclo con las que ya se agregaron.*

- **crearParticion**, **representante**, **unir** (Ej. 1 a 3): el TAD Conjuntos disjuntos con la representación de la teórica. Una `Particion` es directamente un arreglo (`std::vector<size_t>`) donde `P[x]` es el representante del grupo de `x`. Fijate cuánto cuesta cada operación: ¿cuál es la cara?
- **aristasOrdenadasPorPeso** (Ej. 4): la línea 3 del pseudocódigo. Cada arista **una sola vez** (en las listas está dos veces) y de menor a mayor. El ordenamiento (`std::sort`) ya viene escrito en la cáscara: sólo hay que juntar las aristas.
- **kruskal** (Ej. 5): el algoritmo, con lo anterior. Tiene que reproducir el ejemplo de la teórica: `B-C, D-E, A-C, E-F, B-D`.
- **pesoDeAristas** (Ej. 6): el peso de un árbol (el AGM de la teórica pesa 16).

Con pesos repetidos puede haber **más de un AGM**: las pruebas miran que lo que
devolvés sea un árbol generador de peso mínimo, no que sean exactamente las
mismas aristas.

### Parte 2 — Prim (Ej. 7 y 8)

*Hacer crecer un solo árbol desde un vértice; en cada paso, agregar la arista
más liviana entre las que salen del árbol.*

- **incorporar** (Ej. 7): marcar un vértice como parte del árbol e insertar en la cola sus aristas hacia afuera.
- **prim** (Ej. 8): el algoritmo. Tiene que reproducir el **orden de la tabla de la teórica** (desde A: `A-C, C-B, B-D, D-E, E-F`).

### Parte 3 — Camino mínimo (Ej. 9 al 14)

- **mejorar** (Ej. 9): la operación básica de todos los algoritmos de camino mínimo ("si sé llegar a `x` con costo `dist[x]` y hay una arista a `y`, sé llegar a `y` con `dist[x] + peso`"). Ojo con el desborde.
- **dijkstra** (Ej. 10): las distancias desde un origen, con pesos no negativos. En el ejemplo de la teórica, desde A: `0, 4, 3, 10, 12, 15`.
- **dijkstraConPadres** y **reconstruirCamino** (Ej. 11 y 12): no sólo cuánto cuesta llegar, sino por dónde. Los padres forman el **árbol de caminos mínimos** (que **no** es el AGM).
- **caminoMinimo** (Ej. 13): junta los dos anteriores. De A a F: `A, C, B, D, F`.
- **bellmanFord** (Ej. 14): las distancias aunque haya pesos negativos, y la detección de **ciclos negativos** alcanzables desde el origen. Un ciclo negativo que no se alcanza no cuenta.

Hay dos constantes: `INFINITO`, la distancia a un vértice al que todavía no se
sabe llegar (es el mayor `Peso` posible, así que **`INFINITO + algo` se
desborda**: nunca sumes sobre un `INFINITO`), y `SIN_PADRE`, el padre de un
vértice que no tiene.

### Parte 4 — Opcionales (Ej. 15 al 21)

Para después de terminar las tres partes. Sus pruebas se cuentan aparte.

- **Kruskal con un bosque** (Ej. 15 a 18): la mejora que menciona la teórica. Cada grupo es un árbol y se une **por tamaño** (el árbol más chico cuelga del más grande): `representanteBosque` y `unirBosque` cuestan `O(log n)` y Kruskal baja a `O(m log m)`.
- **aristaMasLivianaDelCorte** (Ej. 19): la **propiedad del corte**, la razón por la que Kruskal y Prim funcionan.
- **vueltoGreedy** (Ej. 20): la pregunta de la teórica para ver que **un greedy no siempre es óptimo**: con monedas `1, 3, 4`, ¿cuántas usa para dar `6`?
- **rondasQueMejoran** (Ej. 21): ¿hacen falta siempre las `n - 1` rondas de Bellman-Ford?

### Para pensar

- ¿Se puede responder "¿están en el mismo grupo?" con un DFS sobre las aristas ya elegidas? ¿Cuánto costaría Kruskal así?
- ¿Kruskal y Prim siempre devuelven las mismas aristas? ¿Y el mismo peso?
- ¿Qué pasa si corrés `dijkstra` con un peso negativo? Probalo con `0 -> 1` de peso `2`, `0 -> 2` de peso `3` y `2 -> 1` de peso `-2`.
- Si todos los pesos valen 1, ¿qué calcula Dijkstra? ¿Y quién lo hace más barato?

## Las colas de prioridad

Kruskal usa la partición, pero Prim y Dijkstra necesitan una **cola de
prioridad** donde salga primero el elemento *más chico* (un min-heap). Ya
están armadas en `ejercicios.h`, sobre la `std::priority_queue` de la STL:

```cpp
// Prim: aristas <peso, x, y>, sale primero la de menor peso.
ColaDeAristas cola;
cola.push(AristaDeCola(p, x, y));   // Insertar
AristaDeCola e = cola.top();        // Minimo (sin sacarlo)
cola.pop();                         // EliminarMinimo (no devuelve nada)
cola.empty();                       // Vacia
std::get<0>(e);                     // el peso; std::get<1>(e) es x y std::get<2>(e) es y

// Dijkstra: pares <distancia, vértice>, sale primero el de menor distancia.
ColaDeDistancias cola;
cola.push(DistanciaYVertice(0, v));
cola.top().first;                   // la distancia
cola.top().second;                  // el vértice
```

> Repasá los apuntes (`Tut-12 - Grafos - AGM y Camino Minimo - apuntes.md`)
> antes de empezar: el punto donde más se tropieza es que en Dijkstra un
> vértice puede estar **varias veces** en la cola (hay que descartar los que
> ya están terminados) y que `INFINITO + peso` se desborda.
