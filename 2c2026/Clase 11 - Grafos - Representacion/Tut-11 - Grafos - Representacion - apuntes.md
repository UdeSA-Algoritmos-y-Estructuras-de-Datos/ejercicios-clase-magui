Tutorial 11 – Grafos: Representación
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 11 – Grafos: Representación](#tutorial-11--grafos-representación)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [¿Dónde estamos?](#dónde-estamos)
- [¿Qué es un grafo?](#qué-es-un-grafo)
  - [Tipos de grafos](#tipos-de-grafos)
- [Nociones básicas](#nociones-básicas)
  - [Vecinos, grado y cantidad de aristas](#vecinos-grado-y-cantidad-de-aristas)
  - [Caminos y ciclos](#caminos-y-ciclos)
  - [Subgrafos](#subgrafos)
  - [Conexidad y componentes conexas](#conexidad-y-componentes-conexas)
  - [Los árboles también son grafos](#los-árboles-también-son-grafos)
  - [Grafos dirigidos](#grafos-dirigidos)
  - [Coloreo y grafos bipartitos](#coloreo-y-grafos-bipartitos)
  - [Grafos planares](#grafos-planares)
- [El TAD Grafo](#el-tad-grafo)
- [Representaciones de grafos](#representaciones-de-grafos)
  - [Matriz de adyacencia (para comparar)](#matriz-de-adyacencia-para-comparar)
  - [Listas de adyacencia](#listas-de-adyacencia)
  - [¿Y si cambiamos las secuencias?](#y-si-cambiamos-las-secuencias)
  - [Matriz vs listas: ¿cuál uso?](#matriz-vs-listas-cuál-uso)
  - [¿Y los grafos dirigidos y ponderados?](#y-los-grafos-dirigidos-y-ponderados)
- [Algoritmos sobre grafos: alcanzabilidad](#algoritmos-sobre-grafos-alcanzabilidad)
  - [El problema](#el-problema)
  - [Depth-first search (DFS)](#depth-first-search-dfs)
  - [Un ejemplo paso a paso](#un-ejemplo-paso-a-paso)
  - [Detalles que importan](#detalles-que-importan)
  - [¿Cuánto cuesta?](#cuánto-cuesta)
  - [Qué se resuelve con este recorrido](#qué-se-resuelve-con-este-recorrido)
- [Breadth-first search (BFS)](#breadth-first-search-bfs)
  - [La idea: cambiar la pila por una cola](#la-idea-cambiar-la-pila-por-una-cola)
  - [El mismo ejemplo, con BFS](#el-mismo-ejemplo-con-bfs)
  - [Distancias y caminos más cortos](#distancias-y-caminos-más-cortos)
  - [DFS vs BFS](#dfs-vs-bfs)
- [Repaso relámpago](#repaso-relámpago)
- [Próximamente](#próximamente)
- [Sobre los ejercicios](#sobre-los-ejercicios)



# Objetivos de la clase
- Entender qué es un grafo, qué situaciones modela y manejar su vocabulario: vértices, aristas, grado, camino, ciclo, conexidad, componentes conexas.
- Diferenciar grafos dirigidos y no dirigidos, ponderados y no ponderados.
- Conocer algunas familias de grafos y sus propiedades: completos, árboles, bipartitos, planares.
- Conocer el **TAD Grafo** y sus operaciones.
- Representar un grafo en memoria con **listas de adyacencia** (la representación que vamos a usar en la materia), conocer la **matriz de adyacencia** para poder compararlas, y saber cuánto cuesta cada operación del TAD en cada una.
- Resolver el problema de **alcanzabilidad** (¿hay un camino entre dos vértices?) con **DFS**, usando una pila.
- Recorrer un grafo **por niveles** con **BFS**, usando una cola, y calcular con él distancias y caminos más cortos (en cantidad de aristas).

> En estos apuntes no hay código ni pseudocódigo: los algoritmos y las estructuras se explican con palabras. La implementación en C++ es parte de los ejercicios.



# ¿Dónde estamos?

Hasta ahora trabajamos con estructuras que organizan los datos **en forma de árbol**: el ABB y el AVL, los tries, los heaps. En un árbol de `n` nodos hay exactamente `n - 1` aristas, y entre la raíz y cualquier nodo hay **un único camino**. Eso hace que recorrerlo sea fácil: bajando desde la raíz, nunca se llega dos veces al mismo nodo.

> Pregunta para pensar: en un árbol (binario) de `n` nodos, ¿cuántas aristas hay? ¿Cuántos caminos hay entre la raíz y un nodo dado?

Pero muchas situaciones no tienen esa forma tan prolija: una red de rutas puede tener varias maneras de ir de una ciudad a otra, y puede tener "vueltas" que regresan al punto de partida. Para modelarlas necesitamos una estructura más general. Esa estructura es el **grafo**, y los árboles que ya conocemos son un caso particular.



# ¿Qué es un grafo?

Un **grafo** es una "red" de **vértices** conectados por **aristas**. Los vértices también se llaman *nodos*; las aristas también se llaman *ejes* o *arcos*.

Sirven para modelar un montón de objetos, situaciones y problemas:

- Lugares y rutas (redes de transporte).
- Servidores y conexiones (redes informáticas).
- Personas y relaciones (redes sociales).
- Conceptos y relaciones (redes de conocimiento).
- Moléculas y enlaces (química y descubrimiento de fármacos).
- Tareas y dependencias (planificación y *workflows*).

En todos ellos aparece la misma estructura: **objetos + relaciones**.

> **Grafo (no dirigido)**: un par `G = (V, E)`, donde
> - `V` es un conjunto finito de **vértices**,
> - `E` es un conjunto finito de **aristas**. Cada arista es un **par no ordenado** de vértices **distintos** `{v, w} ⊆ V`.

Un ejemplo, con `V = {a, b, c, d, e, f}` y `E = {{a,b}, {a,e}, {b,c}, {b,e}, {b,f}, {c,d}, {c,f}, {e,f}}`:

```
  a --- b --- c --- d
   \   / \   /
    \ /   \ /
     e --- f
```

Que el par sea **no ordenado** significa que la arista `{a, b}` es lo mismo que `{b, a}`: la relación es simétrica. Y que los dos vértices sean **distintos** significa que, por definición, un vértice no se conecta consigo mismo (no hay bucles) y que una misma arista no puede aparecer dos veces (no hay aristas repetidas, porque `E` es un conjunto).

> Cuando en la materia digamos "grafo" a secas, nos referimos a esto: no dirigido, sin pesos, sin bucles y sin aristas repetidas.

## Tipos de grafos

No toda relación es simétrica ni todas las conexiones "valen" lo mismo. Por eso hay varias variantes:

| Tipo | Qué cambia |
|---|---|
| **No dirigido** | La relación es simétrica: `{v, w}` conecta a `v` con `w` y a `w` con `v`. |
| **Dirigido** (digrafo) | Cada arista tiene un sentido: es un **par ordenado** `(v, w)`, una arista **de `v` a `w`**. Puede haber un **bucle** `(v, v)`. |
| **Ponderado** | Cada arista tiene un **peso**: hay una función de peso `p : E → ℝ` que le asigna un número (una distancia, un costo, un tiempo...). Puede ser dirigido o no. |
| **Multigrafo** | Puede haber aristas repetidas entre los mismos vértices. |

> Pregunta para pensar: ¿dirigido o no dirigido? ¿Ponderado o no? (a) Amistades en una red social. (b) Seguidores en una red social. (c) Rutas entre ciudades, con sus kilómetros. (d) Correlatividades entre materias.



# Nociones básicas

## Vecinos, grado y cantidad de aristas

Sea `G = (V, E)` un grafo no dirigido. Se acostumbra notar:

- `n` a la cantidad total de vértices.
- `m` a la cantidad total de aristas.

Si `{v, w}` es una arista, decimos que `v` y `w` son **adyacentes** o **vecinos**. El **grado** de un vértice `v`, que se nota `d(v)`, es la cantidad de vecinos que tiene.

**Propiedad.** La suma de los grados de todos los vértices es `2m`. La razón: cada arista `{v, w}` aporta 1 al grado de `v` y 1 al de `w`, así que se cuenta dos veces.

**¿Cuántas aristas puede haber?** Un vértice puede tener, como mucho, `n - 1` vecinos (todos los demás, pero no él mismo). Entonces la suma de los grados es a lo sumo `n · (n - 1)`, y como esa suma es `2m`, resulta que:

> Un grafo de `n` vértices tiene **a lo sumo `n(n - 1) / 2` aristas**. Es decir, `m ∈ O(n²)`.

Un grafo con `m` cerca de `n²` se dice **denso**; uno con `m` cerca de `n`, **ralo**. Por eso los costos de los algoritmos sobre grafos se expresan en función de **dos variables**, `n` y `m`: no alcanza con una sola para saber si el grafo es chico o grande.

El **grafo completo** `K_n` es el que tiene `n` vértices y **todos** los pares de vértices distintos son adyacentes. En `K_n` todo vértice tiene grado `n - 1`, así que `m = n(n - 1) / 2`: es el caso en que la cota anterior se alcanza, y es el grafo más denso posible. (`K_3` es un triángulo; `K_4`, un cuadrado con sus dos diagonales.)

## Caminos y ciclos

Un **camino** de `v` a `w` es una secuencia de aristas que conectan a `v` con `w`, pasando por vértices intermedios: `v = v₀ — v₁ — v₂ — ... — v_ℓ = w`. La cantidad `ℓ` de aristas es la **longitud** del camino (y pasa por `ℓ + 1` vértices en total). Un camino de longitud 0 es un solo vértice.

- Un camino es **simple** si no repite vértices.
- Un **ciclo** es un camino de longitud `ℓ > 0` que va de un vértice a sí mismo, sin repetir aristas. (Ir de `v` a `w` y volver por la misma arista no es un ciclo.)

En el grafo del ejemplo: `a — b — c — d` es un camino simple de longitud 3, y `a — b — e — a` es un ciclo.

## Subgrafos

Un grafo `H = (V', E')` es **subgrafo** de `G = (V, E)` si `V' ⊆ V` y `E' ⊆ E` (y las aristas de `E'` unen vértices de `V'`). Si además `V' = V` (están todos los vértices), `H` es un **subgrafo generador**.

Muchos problemas piden **buscar un subgrafo con alguna propiedad**: un camino, un ciclo, un árbol que toque a todos los vértices, etcétera. Los vamos a ir viendo a lo largo de la materia.

## Conexidad y componentes conexas

Un grafo `G` es **conexo** si para todo par de vértices `v, w` hay un camino entre ellos. El grafo del ejemplo es conexo: desde cualquier vértice se puede llegar a cualquier otro. Un grafo con un vértice al que no llega ninguna arista, en cambio, no es conexo: ese vértice queda "solo" y el grafo se parte en pedazos.

Esos pedazos son las **componentes conexas**: un conjunto **maximal** de vértices tal que entre cualquier par de ellos hay un camino ("maximal" quiere decir que no se le puede agregar ningún vértice más sin perder esa propiedad).

```
  a --- b --- c        f --- g        h
  |     |
  d --- e
```

Este grafo no es conexo: tiene 3 componentes conexas, `{a, b, c, d, e}`, `{f, g}` y `{h}`. Un grafo es conexo si y sólo si tiene **una única** componente conexa.

## Los árboles también son grafos

> **Árbol (como grafo)**: un grafo no dirigido, **conexo** y **sin ciclos**.

Para un grafo `G` de `n` vértices, son equivalentes:

- `G` es un árbol (conexo y sin ciclos).
- `G` es conexo y tiene exactamente `n - 1` aristas.
- Entre cada par de vértices de `G` hay **exactamente un** camino simple.

Los árboles que vimos (binarios, tries, heaps) son esto mismo, **con una raíz elegida**. Y un árbol es el grafo conexo con **menos aristas** posibles: si se le saca cualquiera, se desconecta.

> Ojo: tener `n - 1` aristas **no alcanza** para ser árbol. Un triángulo más un vértice aislado tiene 4 vértices y 3 aristas, pero no es conexo. La caracterización pide **las dos cosas**: conexo *y* `n - 1` aristas.

## Grafos dirigidos

En un grafo dirigido, todas las nociones anteriores tienen en cuenta **el sentido de las aristas**:

- El **grado de entrada** `d_in(v)` es la cantidad de aristas que **llegan** a `v`; el **grado de salida** `d_out(v)`, la cantidad de aristas que **salen** de `v`.
- Un **camino** respeta el sentido de las aristas. Si hay un camino de `v` a `w`, decimos que `w` es **alcanzable** desde `v`. Ojo: que `w` sea alcanzable desde `v` no dice nada sobre si `v` lo es desde `w`.
- Un **ciclo dirigido** es un camino de `v` a sí mismo. Un bucle `(v, v)` es un ciclo de longitud 1.

Por ejemplo, con 4 vértices y las aristas `0→1`, `0→2`, `1→2`, `2→0`, `2→3` y `3→3`:

| Vértice | `d_in` | `d_out` |
|---|---|---|
| 0 | 1 | 2 |
| 1 | 1 | 1 |
| 2 | 2 | 2 |
| 3 | 2 | 1 |

El 3 es alcanzable desde el 0, pero el 0 no es alcanzable desde el 3 (del 3 sólo sale un bucle hacia sí mismo). Hay un ciclo dirigido `0 → 2 → 0`.

**Propiedad.** Cada arista sale de un vértice y llega a uno (quizás el mismo), así que la suma de los grados de entrada es `m`, y la suma de los grados de salida también es `m`. (Compará con el `2m` del no dirigido.)

## Coloreo y grafos bipartitos

Un **k-coloreo** de `G` le asigna a cada vértice uno de `k` colores, de modo que **dos vértices adyacentes tengan colores distintos**. El **número cromático** `χ(G)` es el menor `k` tal que `G` admite un `k`-coloreo.

Por ejemplo, el grafo de ejemplo de arriba tiene un triángulo (`a`, `b`, `e`): sus tres vértices son adyacentes de a pares, así que necesitan tres colores distintos y con 2 no alcanza. Con 3 sí se puede, y por eso `χ(G) = 3`.

Un grafo es **bipartito** si sus vértices se pueden partir en dos conjuntos tales que **toda arista une un vértice de cada conjunto**. Es lo mismo que decir que admite un **2-coloreo**.

```
  1     2     3
   \   / \   / \
    \ /   \ /   \
     4     5     6
```

Con las aristas `{1,4}`, `{2,4}`, `{2,5}`, `{3,5}` y `{3,6}`, este grafo es bipartito: pintando `{1, 2, 3}` de un color y `{4, 5, 6}` del otro, toda arista une un vértice de cada color.

- Aparecen cuando hay **dos clases de objetos** y las relaciones van siempre de una clase a la otra. Por ejemplo: alumnos y materias, con una arista si el alumno cursa la materia.
- Todo **árbol** es bipartito.
- `K_3` **no** es bipartito: un triángulo necesita 3 colores. En general, un grafo con un **ciclo de longitud impar** no es bipartito.

> Pregunta para pensar: dado un grafo `G`, ¿cómo podemos saber si admite un 2-coloreo? ¿Cuánto cuesta?

La idea: pintar un vértice cualquiera de un color **obliga** a pintar a sus vecinos del otro color, a los vecinos de esos del primero, y así sucesivamente. Si en algún momento aparece una arista cuyos dos extremos quedaron del mismo color, el grafo no es bipartito. Cada vértice se pinta una sola vez y cada lista de vecinos se recorre una sola vez: **O(n + m)** con listas de adyacencia.

## Grafos planares

Un grafo es **planar** si se puede dibujar en el plano **sin que se crucen las aristas**.

- `K_4` es planar, aunque el dibujo más obvio (el cuadrado con sus dos diagonales) tenga un cruce: lo que importa es que **exista** algún dibujo sin cruces (por ejemplo, un triángulo con el cuarto vértice adentro).
- `K_5` **no** es planar: cualquier dibujo tiene al menos un cruce.
- **Teorema de los cuatro colores**: todo grafo planar admite un 4-coloreo. (Un mapa, con países como vértices y una arista entre países limítrofes, es un grafo planar: alcanzan cuatro colores para pintarlo sin que dos países vecinos compartan color.)



# El TAD Grafo

Para escribir algoritmos sobre grafos sin atarse a cómo están guardados, definimos un **TAD (tipo abstracto de datos)**: una interfaz con operaciones y contratos. Después veremos distintas formas de implementarlo.

Una convención: los vértices de un grafo de `n` vértices son los números `0, 1, ..., n - 1`. (Si los vértices son otra cosa, por ejemplo nombres de ciudades, se los numera usando un diccionario.) Toda precondición que mencione a un vértice `v` o `w` incluye, además, que `0 ≤ v, w < n`, aunque no lo diga.

| Operación | Qué hace | Precondición |
|---|---|---|
| `GrafoSinAristas(n)` | Crea un grafo con los vértices `0, ..., n - 1` y ninguna arista. | `n ≥ 0` |
| `CantidadDeVertices(G)` | Devuelve cuántos vértices tiene `G`. | — |
| `AgregarArista(G, v, w)` | Conecta `v` con `w`: `G` tiene, además, la arista `{v, w}`. | `v ≠ w` y `v`, `w` todavía **no** son vecinos |
| `SonVecinos(G, v, w)` | Devuelve si hay una arista entre `v` y `w`. | — |
| `Vecinos(G, v)` | Devuelve la secuencia de los vecinos de `v`, en algún orden. | — |
| `Grado(G, v)` | Devuelve cuántos vecinos tiene `v`, es decir `d(v)`. | — |

Fijate cómo las precondiciones de `AgregarArista` reflejan la definición de grafo: **`v ≠ w`** porque no hay bucles, y **"no son vecinos todavía"** porque no hay aristas repetidas.

**El costo de cada operación no está en el TAD**: depende de **cómo se represente** el grafo. Eso es justamente lo que vamos a analizar a continuación.



# Representaciones de grafos

Para guardar un grafo en memoria hay que elegir una estructura de datos, y **no hay una única manera** de hacerlo: cada representación tiene sus ventajas y desventajas. En lo que sigue, `G = (V, E)` con `V = {v₀, ..., v_{n-1}}` y `E = {e₀, ..., e_{m-1}}`, e identificamos al vértice `v_i` con el número `i`.

Usamos como ejemplo un grafo de 4 vértices y 3 aristas, que en realidad es un camino: `v1 — v0 — v3 — v2`. Sus aristas son `{v0, v1}`, `{v0, v3}` y `{v2, v3}`.

## Matriz de adyacencia (para comparar)

Es la otra representación clásica, y la conocemos sobre todo para **compararla con las listas**: en la materia vamos a usar listas.

El grafo se representa con una **matriz `A` de `n × n` booleanos**, de tal modo que `{v_i, v_j}` es una arista **si y sólo si** la celda `(i, j)` vale verdadero:

| | v0 | v1 | v2 | v3 |
|---|:-:|:-:|:-:|:-:|
| **v0** | 0 | 1 | 0 | 1 |
| **v1** | 1 | 0 | 0 | 0 |
| **v2** | 0 | 0 | 0 | 1 |
| **v3** | 1 | 0 | 1 | 0 |

(1 es verdadero y 0, falso.) Su **invariante de representación**: la matriz es **simétrica** (`{v_i, v_j}` y `{v_j, v_i}` son la misma arista) y su **diagonal es toda falsa** (no hay bucles).

Las operaciones del TAD son directas: `AgregarArista` pone en verdadero **las dos** celdas simétricas y `SonVecinos` lee una celda, ambas en **O(1)**. En cambio, `Vecinos` y `Grado` tienen que recorrer la fila entera de `v`: **O(n)** aunque `v` tenga pocos vecinos. El espacio es **O(n²)** siempre, y crear el grafo también. Ninguno de estos costos depende de `m`.

> Pregunta para pensar: una red social tiene 10⁶ usuarios, cada uno con unos 200 amigos. ¿Cuántas celdas tiene la matriz? ¿Cuántas de ellas están en verdadero?

Existe también la **matriz de incidencia** (vértices contra aristas, de `n × m`, con exactamente dos verdaderos por columna), pero ocupa `O(n · m)` y se usa poco para programar: aparece más en contextos matemáticos.

## Listas de adyacencia

El grafo se representa con un **arreglo `A` de `n` secuencias**, de tal modo que `A[i]` contiene **los vecinos de `v_i`**. Para el mismo ejemplo:

| Vértice | Su secuencia de vecinos |
|---|---|
| v0 | [v1, v3] |
| v1 | [v0] |
| v2 | [v3] |
| v3 | [v0, v2] |

**Invariante de representación.** `w` está en la secuencia de `v` **si y sólo si** `v` está en la de `w` (la simetría de nuevo); ninguna secuencia tiene repetidos; y la secuencia de `v` no contiene a `v`. Observá que **cada arista aparece dos veces**, una por cada extremo: en total, las secuencias tienen `2m` elementos.

Cada secuencia puede ser, por ejemplo, un **arreglo dinámico**. Entonces, ¿qué hace cada operación?

- **`GrafoSinAristas(n)`**: crea `n` secuencias vacías: **O(n)**.
- **`CantidadDeVertices(G)`**: **O(1)**.
- **`AgregarArista(G, v, w)`**: agrega `w` al final de la secuencia de `v` y `v` al final de la de `w`. Son dos inserciones al final de un arreglo dinámico: **O(1) amortizado**. Fijate que **no hay que buscar si la arista ya estaba**: justamente para eso es la precondición. Si hubiera que chequearlo, costaría `O(d(v))`.
- **`SonVecinos(G, v, w)`**: hay que **buscar** a `w` en la secuencia de `v`, que no está ordenada: **O(d(v))**.
- **`Vecinos(G, v)`**: devuelve **la secuencia de `v` tal cual, sin copiarla**: ya están todos los vecinos juntos y sólo ellos. Quien la recorra hace `d(v)` pasos, en lugar de mirar `n` celdas como en la matriz.
- **`Grado(G, v)`**: el arreglo dinámico guarda su tamaño, así que no hay que recorrer nada: **O(1)**.

**Resumen de costos** (con `n` vértices, `m` aristas y `d(v)` el grado de `v`; cotas de peor caso salvo la indicada):

| Concepto | Costo |
|---|---|
| Espacio de almacenamiento: `n` secuencias con `2m` elementos en total | O(n + m) |
| Crear el grafo (`GrafoSinAristas`) | O(n) |
| `SonVecinos` | O(d(v)) |
| `AgregarArista` | O(1) amortizado |
| `Grado` | O(1) |
| `Vecinos` (recorrerlos) | O(d(v)) |
| Recorrer **todas** las aristas: la suma de `(1 + d(v))` sobre todos los `v` es `n + 2m` pasos | O(n + m) |

Una mejora para `SonVecinos`: como `Grado` es O(1), se puede **buscar en la más corta** de las dos secuencias (la de `v` o la de `w`), y el costo pasa a ser `O(mín(d(v), d(w)))`.

## ¿Y si cambiamos las secuencias?

Una secuencia de vecinos es, en el fondo, un conjunto: lo que importa es **quiénes están**. Entonces cada `A[v]` podría ser **otro TAD** que ya conocemos. Comparemos tres opciones (con `d(v)` el grado de `v`):

| | Arreglo dinámico | Conjunto sobre AVL | Tabla de hash |
|---|---|---|---|
| Espacio total | O(n + m) | O(n + m) | O(n + m) |
| `SonVecinos` | O(d(v)) peor caso | O(log d(v)) peor caso | O(1) en promedio |
| `AgregarArista` | O(1) amortizado | O(log d(v)) peor caso | O(1) en promedio |
| Recorrer `Vecinos` | O(d(v)) | O(d(v)) y además **ordenados** | O(d(v)) |

Es un ejercicio de la teórica: conviene tapar la tabla e intentar completarla, diciendo en qué caso vale cada cota (peor caso, promedio o amortizado). Las respuestas: el AVL garantiza el peor caso porque siempre está balanceado, así que `SonVecinos` y la inserción cuestan `O(log d(v))` siempre; con la tabla de hash las cotas valen **en promedio** (si la función de hash dispersa bien); con el arreglo dinámico, la inserción es **amortizada** (de vez en cuando hay que agrandar el arreglo). La contrapartida es que las estructuras más sofisticadas pesan más en memoria (por constantes) y son más complejas de implementar. Con un arreglo dinámico ya alcanza para casi todo, porque los algoritmos que vamos a ver recorren los vecinos mucho más de lo que preguntan por una arista en particular.

## Matriz vs listas: ¿cuál uso?

Con `n` la cantidad de vértices, `m` la de aristas y `d(v)` el grado de `v`:

| | Matriz de adyacencia | Listas de adyacencia |
|---|---|---|
| Espacio | O(n²) | O(n + m) |
| `GrafoSinAristas(n)` | O(n²) | O(n) |
| `CantidadDeVertices(G)` | O(1) | O(1) |
| `AgregarArista(G, v, w)` | O(1) | O(1) amortizado |
| `SonVecinos(G, v, w)` | O(1) | O(d(v)) |
| `Grado(G, v)` | O(n) | O(1) |
| Recorrer `Vecinos(G, v)` | O(n) | O(d(v)) |
| Recorrer todas las aristas | O(n²) | O(n + m) |

- **Matriz**: conviene para grafos **densos** (`m` cerca de `n²`, donde el espacio cuadrático no se desperdicia), o cuando la operación dominante es **`SonVecinos`**.
- **Listas**: conviene para grafos **ralos** (los más comunes en la práctica), o cuando la operación dominante es **recorrer los vecinos**, que es lo que hacen casi todos los algoritmos que vamos a ver. Por eso las listas son la representación que vamos a usar por defecto.

> Pregunta para pensar: si un grafo tiene 100.000 vértices pero solo 100.000 aristas, ¿cuánta memoria ocuparía la matriz de adyacencia comparada con las listas?

## ¿Y los grafos dirigidos y ponderados?

Las mismas representaciones sirven, con cambios chicos:

- **Dirigidos**
  - *Matriz*: la celda `(i, j)` es verdadera si hay una arista **de `v_i` a `v_j`**. Ya **no es simétrica**, y la diagonal puede tener verdaderos (bucles).
  - *Listas*: la secuencia de `v` tiene **sólo los vecinos salientes** de `v`. Cada arista aparece **una sola vez** (no dos), y la secuencia de `v` puede contener a `v`.
- **Ponderados**
  - *Matriz*: en vez de un booleano, la celda `(i, j)` guarda **el peso** de la arista (y un valor especial, como `∞`, si no hay arista).
  - *Listas*: cada secuencia es una secuencia de **pares `⟨vecino, peso⟩`**.

Los costos de la tabla no cambian, leyendo `d(v)` como `d_out(v)`.

> Ojo: con listas, encontrar las aristas que **llegan** a `v` (y por lo tanto calcular `d_in(v)`) obliga a **recorrer todas las listas**: **O(n + m)**. Con la matriz es recorrer la columna `v`: **O(n)**.



# Algoritmos sobre grafos: alcanzabilidad

## El problema

Queremos un algoritmo con este contrato:

- **Entrada**: un grafo `G = (V, E)` y dos vértices `v, w ∈ V`.
- **Salida**: un booleano que indique si **hay un camino de `v` a `w`**.

Mirando un dibujo es fácil. Pero **el algoritmo no ve el dibujo**: sólo puede preguntar, **de a un vértice por vez**, quiénes son sus vecinos (es decir, usar la operación `Vecinos` del TAD).

> Pregunta para pensar: en un rosetree, recorrer desde la raíz siempre termina. ¿Qué puede salir mal si hacemos lo mismo en un grafo?

Lo que sale mal es que **el grafo puede tener ciclos**. Si desde un vértice vamos a sus vecinos, desde esos a los suyos, y así, podemos volver a un vértice por el que ya pasamos y dar vueltas por el ciclo **para siempre**. En un árbol esto no ocurre porque hay un único camino hasta cada nodo. En un grafo, hay que **acordarse de por dónde ya pasamos**.

## Depth-first search (DFS)

**Depth-first search** (búsqueda en profundidad) resuelve el problema usando dos estructuras auxiliares:

- Un **conjunto de vértices ya visitados** (en la práctica, un arreglo de `n` booleanos, uno por vértice).
- Una **pila de vértices pendientes por visitar**, que arranca con el vértice de partida `v`.

El algoritmo, en palabras:

1. Mientras la pila de pendientes no esté vacía, **sacar el vértice `x` del tope**.
2. Si `x` es el vértice buscado `w`, **terminar**: hay un camino de `v` a `w`.
3. Si `x` **no fue visitado** todavía: **marcarlo como visitado** y, para **cada vecino `y` de `x` que no haya sido visitado**, **apilarlo** como pendiente.
4. Si la pila se vació sin haber encontrado a `w`, **no hay un camino** de `v` a `w`.

Si `x` ya había sido visitado, simplemente se lo descarta y se sigue con el próximo de la pila.

## Un ejemplo paso a paso

Veamos si hay un camino de `0` a `3` en este grafo:

```
  0 --- 1 --- 2 --- 3
  |            \
  |             4
  |            /
  5 --------- 6 --- 7
```

Los vecinos de cada vértice se recorren **de menor a mayor**. En la columna de la pila, **el tope está a la derecha**:

| Paso | Se saca | Pila de pendientes | Visitados |
|:-:|:-:|---|---|
| inicio | — | [0] | ∅ |
| 1 | 0 | [1, 5] | {0} |
| 2 | 5 | [1, 6] | {0, 5} |
| 3 | 6 | [1, 4, 7] | {0, 5, 6} |
| 4 | 7 | [1, 4] | {0, 5, 6, 7} |
| 5 | 4 | [1, 2] | {0, 4, 5, 6, 7} |
| 6 | 2 | [1, 1, 3] | {0, 2, 4, 5, 6, 7} |
| 7 | 3 | | `3 = w`: **hay camino** |

Fijate qué pasó:

- **Paso 1**: se saca el 0, se lo marca y se apilan sus vecinos 1 y 5. El tope queda en el **5** (el último que se apiló).
- **Pasos 2 a 4**: el recorrido sigue por el 5, después el 6, después el 7. Es decir, **se mete en profundidad** por una rama. En el 7 no hay vecinos nuevos, y recién ahí **retrocede**: lo siguiente que sale de la pila es el 4, que había quedado pendiente desde el paso 3.
- **Paso 6**: al sacar el 2 se apilan el 1 y el 3. Pero el **1 ya estaba en la pila** (desde el paso 1): quedó **repetido**.
- **Paso 7**: el 3 sale del tope y es el vértice buscado.

## Detalles que importan

- **La pila es lo que da el "en profundidad".** El último vértice apilado es el primero en salir, así que el recorrido sigue avanzando por lo que acaba de descubrir y deja para después lo que quedó pendiente. (Qué pasa si cambiamos la pila por una cola lo vemos en la sección de BFS.)
- **Un vértice puede quedar repetido en la pila.** Un vértice se apila cada vez que algún vecino suyo se visita y él todavía no fue visitado; puede pasar varias veces antes de que le toque salir. Por eso, **al sacarlo hay que revisar si ya fue visitado** y, si lo fue, descartarlo. Si no se revisara, se volvería a expandir y se perdería la garantía de que cada vértice se procesa una sola vez.
- **Por qué hace falta marcar los visitados.** Para no dar vueltas infinitas por un ciclo. El conjunto de visitados es lo que hace que el recorrido **termine**.
- **Cuándo se pregunta si es `w`.** Se pregunta **al sacar** el vértice de la pila, antes de revisar si fue visitado. Con eso también queda cubierto el caso `v = w`: el primero que sale es el propio `v`, y la respuesta es "sí" (un camino de longitud 0).
- **Si no hay camino**, el recorrido termina igual: cuando se vacía la pila, ya se visitaron **todos** los vértices alcanzables desde `v`, y `w` no estaba entre ellos.
- **Grafos dirigidos.** Funciona igual, pero con `Vecinos` entendido como los vecinos **salientes**: sólo se avanza en el sentido de las aristas.

## ¿Cuánto cuesta?

Una idea de por qué es eficiente (la demostración de que es correcto y su análisis fino los vemos la clase que viene):

- Cada vértice **se marca y se expande una sola vez**, y en ese momento se recorre **su lista de vecinos una sola vez**.
- Cada arista se mira dos veces como máximo (una desde cada extremo), así que en total se apilan a lo sumo `2m` vértices (más el inicial).
- Con **listas de adyacencia**, recorrer los vecinos de `x` cuesta `d(x)`: sumando para todos los vértices se obtiene **O(n + m)**.
- Con **matriz de adyacencia**, obtener los vecinos de cada vértice cuesta `O(n)` aunque tenga pocos, y entonces el recorrido completo cuesta **O(n²)**, sin importar cuántas aristas haya.

Por eso, para recorridos, las **listas de adyacencia** son siempre mejores o iguales.

## Qué se resuelve con este recorrido

Una vez que sabemos recorrer todo lo alcanzable desde un vértice, varios problemas de las nociones básicas se vuelven recorridos con alguna variante:

- **Componentes conexas.** Recorrer todos los vértices; cada vez que aparece uno **sin visitar**, es el comienzo de una componente nueva: se lanza el recorrido desde él (eso marca como visitada toda su componente) y se suma uno al contador. Cuesta **O(n + m)** en total, porque los visitados se comparten entre todos los recorridos.
- **Conexo.** Un grafo (con al menos un vértice) es conexo si y sólo si tiene **una única** componente conexa.
- **¿Es un árbol?** Usando la caracterización: es conexo **y** tiene exactamente `n - 1` aristas. La cantidad de aristas sale de la propiedad de los grados: `m` es la suma de los grados dividida por 2.
- **¿Es bipartito?** El 2-coloreo de la sección anterior se hace con el mismo esquema: una pila, y en vez de "marcar como visitado" se **pinta** al vecino del color contrario al del vértice actual. Si un vecino ya está pintado del **mismo** color, no es bipartito. Como el grafo puede no ser conexo, hay que repetirlo desde cada vértice todavía sin pintar.

> Pregunta para pensar: ¿por qué esos problemas no necesitan guardar el camino ni la distancia? ¿Qué información extra necesitaríamos si quisiéramos devolver **un** camino de `v` a `w`, y no sólo si existe?



# Breadth-first search (BFS)

## La idea: cambiar la pila por una cola

Recordemos el recorrido por niveles de un rosetree: usaba una **cola**, y si la cambiábamos por una pila el recorrido pasaba a ir en profundidad. Con los grafos pasa exactamente lo mismo. **BFS** (búsqueda en anchura) es el mismo recorrido que DFS, pero los vértices pendientes se guardan en una **cola**: el primero que se descubrió es el primero en procesarse.

El efecto es que el recorrido avanza **por niveles**: primero el vértice de partida `v`; después todos sus vecinos (a distancia 1); después los vecinos de esos que todavía no se vieron (a distancia 2); y así sucesivamente. No se pasa al nivel `k + 1` hasta no haber terminado con **todo** el nivel `k`.

El algoritmo, en palabras:

1. Arrancar con una cola que tiene sólo a `v`, y marcar a `v` como visitado.
2. Mientras la cola no esté vacía, **sacar el vértice `x` del frente**.
3. Para **cada vecino `y` de `x` que no haya sido visitado**: marcarlo como visitado y **encolarlo** al final.

Hay una diferencia chica pero importante con el DFS: **acá se marca al encolar, no al sacar**. En el DFS, un vértice podía quedar repetido en la pila porque se lo apilaba varias veces antes de visitarlo, y por eso había que revisar al sacarlo. En BFS, como el vértice queda marcado en el mismo momento en que entra a la cola, **no vuelve a entrar nunca más**: cada vértice pasa por la cola una sola vez, y al sacarlo no hace falta revisar nada.

## El mismo ejemplo, con BFS

Recorramos el grafo de antes desde el 0, con los vecinos de menor a mayor. En la columna de la cola, **el frente está a la izquierda** y se agrega por la derecha:

```
  0 --- 1 --- 2 --- 3
  |            \
  |             4
  |            /
  5 --------- 6 --- 7
```

| Paso | Se saca | Cola | Distancia de los nuevos |
|:-:|:-:|---|---|
| inicio | — | [0] | el 0 está a 0 |
| 1 | 0 | [1, 5] | el 1 y el 5 están a 1 |
| 2 | 1 | [5, 2] | el 2 está a 2 |
| 3 | 5 | [2, 6] | el 6 está a 2 |
| 4 | 2 | [6, 3, 4] | el 3 y el 4 están a 3 |
| 5 | 6 | [3, 4, 7] | el 7 está a 3 |
| 6 | 3 | [4, 7] | — |
| 7 | 4 | [7] | — |
| 8 | 7 | [] | — |

El orden en que se sacan los vértices es **0, 1, 5, 2, 6, 3, 4, 7**: primero el nivel 0, después los del nivel 1 (`1`, `5`), después los del nivel 2 (`2`, `6`) y por último los del nivel 3 (`3`, `4`, `7`). Compará con el DFS sobre el mismo grafo, que fue 0, 5, 6, 7, 4, 2, 3, 1: se metió por una rama hasta el fondo antes de volver.

## Distancias y caminos más cortos

La **distancia** entre `v` y `w` es la longitud del **camino más corto** entre ellos (la menor cantidad de aristas). BFS la calcula gratis:

- Se guarda, para cada vértice, su distancia a `v`. La de `v` es 0.
- Cuando se descubre a un vecino `y` desde un vértice `x`, la distancia de `y` es **la de `x` más uno**.
- Los vértices que nunca se descubren **no son alcanzables**: no tienen distancia.

¿Por qué es la menor? Porque los vértices entran y salen de la cola **en orden no decreciente de distancia**: primero todos los de distancia `k`, y recién después los de `k + 1`. Entonces, la primera vez que un vértice se descubre es desde un vértice del nivel más bajo posible, y no puede existir un camino más corto que ese. (La demostración formal la hacemos con más rigor en la próxima clase.)

> Pregunta para pensar: ¿por qué esto **no** funciona igual si el grafo es ponderado? (Pista: el camino con menos aristas ya no es necesariamente el de menor costo.)

**Reconstruir el camino.** Para devolver **un** camino más corto (y no sólo su longitud), se guarda también, para cada vértice descubierto, **quién lo descubrió** (su *padre* en el recorrido). Al terminar, se parte de `w`, se va saltando al padre una vez tras otra hasta llegar a `v`, y se da vuelta lo que quedó. Si `w` nunca se descubrió, no hay camino. (Esa es la respuesta a la pregunta sobre qué información extra hace falta que dejamos en DFS.)

**Costo.** Igual que DFS: cada vértice entra y sale de la cola una vez, y cada lista de vecinos se recorre una vez, así que con listas de adyacencia es **O(n + m)**, y con matriz de adyacencia **O(n²)**.

## DFS vs BFS

| | DFS | BFS |
|---|---|---|
| Estructura de pendientes | **Pila** | **Cola** |
| Orden | En profundidad: sigue por lo último que descubrió | Por niveles: termina un nivel antes de pasar al siguiente |
| Cuándo se marca | Al **sacar** (el vértice puede estar repetido en la pila) | Al **encolar** (cada vértice entra una sola vez) |
| ¿Camino más corto? | No: encuentra *un* camino, no necesariamente el más corto | **Sí**: en grafos sin pesos, el primer camino que llega a cada vértice es de los más cortos |
| Costo (listas / matriz) | O(n + m) / O(n²) | O(n + m) / O(n²) |

En el ejemplo, DFS llegó al 3 pasando por `0, 5, 6, 4, 2, 3` (5 aristas), pero el camino más corto es `0, 1, 2, 3` (3 aristas): BFS lo encuentra, DFS no lo garantiza. Para saber **si hay** un camino (alcanzabilidad), componentes conexas, conexidad, árboles o bipartitos, **cualquiera de los dos sirve**; para **distancias y caminos más cortos**, se usa BFS.



# Repaso relámpago

- ¿Cuántas aristas puede tener, como máximo, un grafo no dirigido de `n` vértices?
- ¿Cuánto suman los grados de todos los vértices? ¿Y los de entrada y de salida en un grafo dirigido?
- ¿Qué es un árbol, visto como grafo? ¿Alcanza con que tenga `n - 1` aristas?
- ¿Es bipartito `K_4`? ¿Y un árbol?
- Para un grafo **ralo**, ¿qué representación conviene? ¿Por qué?
- ¿Qué operación del TAD es **más barata** con la matriz que con las listas? ¿Y cuáles son más baratas con las listas?
- En DFS, ¿dónde guardamos los vértices pendientes por visitar? ¿En qué orden salen?
- ¿Por qué hace falta marcar los visitados? ¿Por qué hay que revisar, **al sacar** un vértice de la pila, si ya fue visitado?
- ¿Qué cambia entre DFS y BFS? ¿Por qué en BFS se puede marcar al encolar?
- ¿Qué recorrido uso para calcular la distancia mínima entre dos vértices de un grafo sin pesos? ¿Y para saber si hay un camino?



# Próximamente

- **DFS y BFS con más rigor**: por qué son correctos y cuánto cuestan, y por qué BFS da los caminos más cortos.
- **Árbol generador mínimo**: en un grafo ponderado y conexo, ¿cuál es la forma más barata de mantener conectados a todos los vértices?



# Sobre los ejercicios

Los ejercicios de esta clase (en la carpeta `ejercicios`) siguen el mismo orden que estos apuntes:

- **Parte 1**: el TAD Grafo, operación por operación, con **listas de adyacencia**. La idea es ver con las manos por qué cada operación cuesta lo que cuesta.
- **Parte 2**: trabajar con la representación: cantidad de aristas a partir de los grados, grafos completos, vecinos en común, chequear el invariante de representación y calcular grados de entrada en un grafo dirigido.
- **Parte 3**: alcanzabilidad con **DFS** y una pila explícita, y los problemas que se resuelven con ese recorrido: componentes conexas, conexidad, árboles y grafos bipartitos.
- **Parte 4**: **BFS** con una cola: orden de visita, distancias mínimas, camino más corto y vértices a distancia `k`.
