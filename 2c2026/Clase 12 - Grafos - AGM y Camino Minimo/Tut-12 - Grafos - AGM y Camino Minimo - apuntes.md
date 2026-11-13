Tutorial 12 – Grafos: AGM y Camino Mínimo
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 12 – Grafos: AGM y Camino Mínimo](#tutorial-12--grafos-agm-y-camino-mínimo)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [¿Dónde estamos?](#dónde-estamos)
- [Lo que ya resolvemos con los recorridos](#lo-que-ya-resolvemos-con-los-recorridos)
- [Árbol generador mínimo (AGM)](#árbol-generador-mínimo-agm)
  - [Un problema de conexión](#un-problema-de-conexión)
  - [Árbol generador](#árbol-generador)
  - [Árbol generador mínimo](#árbol-generador-mínimo)
  - [¿Y la fuerza bruta?](#y-la-fuerza-bruta)
- [Algoritmo de Kruskal](#algoritmo-de-kruskal)
  - [La idea](#la-idea)
  - [Un ejemplo paso a paso](#un-ejemplo-paso-a-paso)
  - [Conjuntos disjuntos (Union-Find)](#conjuntos-disjuntos-union-find)
  - [Representación con un arreglo](#representación-con-un-arreglo)
  - [Una representación mejor: el bosque](#una-representación-mejor-el-bosque)
  - [¿Cuánto cuesta Kruskal?](#cuánto-cuesta-kruskal)
- [Algoritmo de Prim](#algoritmo-de-prim)
  - [La idea de Prim](#la-idea-de-prim)
  - [El mismo ejemplo, con Prim](#el-mismo-ejemplo-con-prim)
  - [La cola de prioridad de aristas](#la-cola-de-prioridad-de-aristas)
  - [¿Cuánto cuesta Prim?](#cuánto-cuesta-prim)
- [¿Por qué funcionan? Algoritmos greedy](#por-qué-funcionan-algoritmos-greedy)
  - [Propiedad del corte](#propiedad-del-corte)
  - [Algoritmos greedy](#algoritmos-greedy)
  - [Kruskal vs Prim](#kruskal-vs-prim)
- [Camino mínimo](#camino-mínimo)
  - [El camino más barato](#el-camino-más-barato)
  - [Peso de un camino y distancia](#peso-de-un-camino-y-distancia)
  - [Dos ideas que usan todos los algoritmos](#dos-ideas-que-usan-todos-los-algoritmos)
  - [Mejorar una estimación con una arista](#mejorar-una-estimación-con-una-arista)
- [Algoritmo de Dijkstra](#algoritmo-de-dijkstra)
  - [Condición sobre los pesos](#condición-sobre-los-pesos)
  - [La idea de Dijkstra](#la-idea-de-dijkstra)
  - [Un ejemplo paso a paso](#un-ejemplo-paso-a-paso-1)
  - [Detalles que importan](#detalles-que-importan)
  - [¿Cuánto cuesta Dijkstra?](#cuánto-cuesta-dijkstra)
  - [No sólo la distancia: el camino](#no-sólo-la-distancia-el-camino)
  - [¿Y con pesos negativos?](#y-con-pesos-negativos)
- [Algoritmo de Bellman-Ford](#algoritmo-de-bellman-ford)
  - [Pesos y ciclos negativos](#pesos-y-ciclos-negativos)
  - [La idea de Bellman-Ford](#la-idea-de-bellman-ford)
  - [Un ejemplo paso a paso](#un-ejemplo-paso-a-paso-2)
  - [Detección de ciclos negativos y costo](#detección-de-ciclos-negativos-y-costo)
  - [Camino mínimo desde un origen: resumen](#camino-mínimo-desde-un-origen-resumen)
- [Material adicional: flujo máximo](#material-adicional-flujo-máximo)
  - [Red de flujo](#red-de-flujo)
  - [Ford-Fulkerson y Edmonds-Karp](#ford-fulkerson-y-edmonds-karp)
  - [Teorema max-flow min-cut](#teorema-max-flow-min-cut)
  - [Un último algoritmo: Floyd-Warshall](#un-último-algoritmo-floyd-warshall)
- [Repaso relámpago](#repaso-relámpago)
- [Próximamente](#próximamente)
- [Sobre los ejercicios](#sobre-los-ejercicios)



# Objetivos de la clase
- Aplicar los recorridos (DFS y BFS) a problemas clásicos: componentes conexas, ciclos y grafos bipartitos.
- Entender qué es un **árbol generador mínimo (AGM)** y para qué sirve.
- Conocer el TAD **Conjuntos disjuntos (Union-Find)** y dos formas de representarlo.
- Resolver el AGM con **Kruskal** (ordenando las aristas) y con **Prim** (haciendo crecer un árbol, con una cola de prioridad).
- Entender qué es un algoritmo **greedy**, y por qué Kruskal y Prim funcionan (propiedad del corte).
- Definir el problema de **camino mínimo** en un grafo ponderado y la operación básica sobre la que se apoyan todos los algoritmos: **mejorar** una estimación.
- Resolver el camino mínimo desde un origen con **Dijkstra** (pesos no negativos) y con **Bellman-Ford** (pesos cualesquiera, detectando ciclos negativos).
- *(Adicional)* Conocer el problema de **flujo máximo**.

> En estos apuntes no hay código ni pseudocódigo: los algoritmos y las estructuras se explican con palabras. La implementación en C++ es parte de los ejercicios.



# ¿Dónde estamos?

En la clase anterior vimos qué es un grafo, cómo representarlo con **listas de adyacencia**, y dos recorridos: **DFS** (con una pila) y **BFS** (con una cola). BFS nos dio la **distancia** entre dos vértices: la cantidad de aristas del camino más corto.

Pero en muchos problemas las aristas **no son todas iguales**: una ruta es más larga que otra, un enlace cuesta más que otro, un tramo de cable es más caro. Los grafos **ponderados** guardan, en cada arista, un **peso** (un costo, una distancia, un tiempo). Y ahora las preguntas cambian:

- Si hay que **conectar todo** gastando lo menos posible, ¿qué aristas elegimos? → **árbol generador mínimo**.
- Si hay que **ir de un lado a otro** gastando lo menos posible, ¿por dónde vamos? → **camino mínimo**.

Las dos preguntas parecen parecidas y no lo son. Los algoritmos que las resuelven tienen una estrategia en común (son *greedy*), pero resuelven problemas distintos y dan resultados distintos.

> Pregunta para pensar: BFS desde A dice que B está a distancia 1 si hay una arista directa A–B. Pero si esa arista cuesta 100 y hay un camino de dos aristas que cuesta 3, ¿cuál es el "más corto"?



# Lo que ya resolvemos con los recorridos

Antes de seguir, un repaso de lo que se puede resolver con DFS y BFS (sin ponderar), porque lo vamos a necesitar:

- **Componentes conexas.** Un recorrido desde `v` visita exactamente la componente conexa de `v`. Para encontrarlas todas, se lanza un recorrido desde cada vértice que todavía no fue visitado, **compartiendo** el arreglo de visitados entre todos los recorridos. Cuesta **O(n + m)** en total, y un grafo es **conexo** si y sólo si tiene una única componente.
- **Detección de ciclos.** En un DFS sobre un grafo no dirigido, si desde `x` aparece un vecino ya visitado que **no es el vértice desde el que llegamos a `x`** (su padre en el recorrido), hay dos caminos distintos hasta ese vecino: hay un ciclo. Cuesta **O(n + m)**.
- **Grafos bipartitos.** Con un BFS desde un vértice, un grafo (conexo) es bipartito si y sólo si **ninguna arista une dos vértices a la misma distancia** del origen. Si los hubiera, los caminos más cortos hasta ellos más esa arista cierran un ciclo impar. Cuesta **O(n + m)**.

Ahora, a lo nuevo.



# Árbol generador mínimo (AGM)

## Un problema de conexión

Hay que conectar con fibra óptica los 6 edificios de un campus. Cada tramo posible tiene un costo, y queremos que todos los edificios queden comunicados (directa o indirectamente) gastando lo menos posible. Este es el grafo que vamos a usar en todo el apunte (los vértices son los edificios `A, ..., F`; las aristas son los tramos posibles, con su costo):

| Arista | A–B | A–C | B–C | B–D | C–D | C–E | D–E | D–F | E–F |
|---|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| Peso | 7 | 3 | 1 | 6 | 8 | 9 | 2 | 5 | 4 |

> Pregunta para pensar: la solución más barata, ¿puede tener un ciclo?

No: si tuviera un ciclo, se podría sacar cualquier arista del ciclo y todos los edificios seguirían conectados, gastando menos (los pesos son positivos). Entonces la solución buscada **es un árbol**.

## Árbol generador

Sea `G = (V, E)` un grafo **no dirigido, conexo y ponderado**, con pesos `p : E → R`.

> **Árbol generador**: un subconjunto de aristas `T ⊆ E` tal que `(V, T)` es un árbol: **conecta a todos los vértices** de `G` y **no tiene ciclos**.

Dos propiedades:

- Tiene exactamente **`n - 1` aristas** (como todo árbol de `n` vértices).
- Su **peso** es la suma de los pesos de sus aristas.

Un grafo puede tener muchísimos árboles generadores. En el ejemplo, las aristas `A–B, B–D, C–D, C–E, D–F` forman uno, de peso `7 + 6 + 8 + 9 + 5 = 35`.

## Árbol generador mínimo

> **Árbol generador mínimo (AGM)**: un árbol generador de `G` de **peso mínimo** entre todos los árboles generadores de `G`.

- **Entrada**: un grafo `G` no dirigido, conexo y ponderado.
- **Salida**: un árbol generador mínimo de `G`.

En el ejemplo, el AGM tiene peso `1 + 2 + 3 + 4 + 6 = 16`: las aristas `B–C, D–E, A–C, E–F, B–D`.

Ojo: si hay pesos repetidos, puede haber **más de un AGM**. Lo que es único es el **peso** del AGM; las aristas, no necesariamente.

## ¿Y la fuerza bruta?

> Pregunta para pensar: ¿se puede resolver probando todos los árboles generadores y quedándose con el más liviano?

Un grafo completo de `n` vértices tiene `n^(n-2)` árboles generadores: con `n = 20`, más de `10^23`. Imposible. Vamos a ver dos algoritmos que lo resuelven de forma eficiente, y los dos siguen la misma estrategia: **elegir, en cada paso, la arista más liviana que sirva**.



# Algoritmo de Kruskal

## La idea

> **Kruskal**: recorrer las aristas **de la más liviana a la más pesada**. Cada arista se agrega a la solución, **salvo que forme un ciclo** con las que ya se agregaron.

Para entender por qué funciona la "regla del ciclo":

- Se empieza con los `n` vértices **sueltos** (ninguna arista): `n` grupos de un vértice.
- Cada arista que se agrega **une dos grupos** de vértices que estaban separados.
- Una arista forma un ciclo **exactamente cuando sus dos extremos ya están en el mismo grupo**.
- Se termina cuando todos los vértices quedan en un único grupo: un árbol generador.

## Un ejemplo paso a paso

Las aristas, ordenadas por peso:

| Peso | Arista | ¿Va? | Grupos después |
|:-:|:-:|---|---|
| | | | A — B — C — D — E — F |
| 1 | B–C | sí | A — **BC** — D — E — F |
| 2 | D–E | sí | A — BC — **DE** — F |
| 3 | A–C | sí | **ABC** — DE — F |
| 4 | E–F | sí | ABC — **DEF** |
| 5 | D–F | **no**: ciclo (D y F ya están juntos) | ABC — DEF |
| 6 | B–D | sí | **ABCDEF** |
| 7 | A–B | no: ciclo | |
| 8 | C–D | no: ciclo | |
| 9 | C–E | no: ciclo | |

Quedan `n - 1 = 5` aristas, de peso total `1 + 2 + 3 + 4 + 6 = 16`. (Una vez que se juntaron `n - 1` aristas, ya se puede cortar: las que quedan sólo cerrarían ciclos.)

Fijate que la arista D–F (peso 5) se descartó **aunque era más liviana** que B–D (peso 6): para cuando le tocó el turno, D y F ya estaban conectados por D–E–F.

## Conjuntos disjuntos (Union-Find)

Para programar Kruskal hacen falta dos cosas:

1. **Ordenar las aristas** por peso: ya sabemos hacerlo, `O(m log m)`.
2. **Mantener los grupos** de vértices, con dos preguntas: ¿están `v` y `w` en el mismo grupo?, y: unir el grupo de `v` con el de `w`.

> Pregunta para pensar: ¿se puede responder "¿están en el mismo grupo?" con un DFS sobre las aristas ya elegidas? ¿Cuánto costaría Kruskal así?

Costaría un recorrido completo por cada arista: demasiado. Conviene una estructura hecha a medida: el TAD **Conjuntos disjuntos**, también llamado **Union-Find**.

> **Conjuntos disjuntos (Union-Find)**: una partición de los elementos `0, ..., n - 1` en grupos, donde cada grupo se identifica por uno de sus elementos: su **representante**.

Sus operaciones:

- **CrearParticion(n)**: cada elemento arranca en un grupo propio.
- **Representante(P, x)**: el representante del grupo de `x`. Para todos los elementos de un mismo grupo devuelve lo mismo.
- **Unir(P, x, y)**: junta los grupos de `x` y de `y` en uno solo (con la precondición de que tengan distinto representante).

`x` e `y` están en el mismo grupo **si y sólo si** `Representante(P, x) = Representante(P, y)`.

## Representación con un arreglo

La representación más simple es un arreglo `P` de `n` posiciones, donde `P[x]` es el representante del grupo de `x`. Por ejemplo, con `n = 6` y los grupos `{0, 1, 3}`, `{2}` y `{4, 5}`:

| `x` | 0 | 1 | 2 | 3 | 4 | 5 |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| `P[x]` | 0 | 0 | 2 | 0 | 4 | 4 |

**Invariante de representación**: el representante es su propio representante, es decir, para todo `x`, `P[P[x]] = P[x]`.

- **CrearParticion**: pone cada `P[x]` en `x`. Cuesta **O(n)**.
- **Representante**: es leer `P[x]`. Cuesta **O(1)**.
- **Unir**: hay que recorrer **todo el arreglo**, cambiando el representante de todos los que tenían el del grupo de `y` por el del grupo de `x`. Cuesta **O(n)**.

> Pregunta para pensar: ¿cómo queda `P` después de `Unir(P, 1, 5)`? (Respuesta: los de los grupos `{0,1,3}` y `{4,5}` pasan a tener el representante 0: `P = [0, 0, 2, 0, 0, 0]`.)

Preguntar es barato, pero unir es caro. Como Kruskal une `n - 1` veces, esto nos va a costar.

## Una representación mejor: el bosque

La idea: representar cada grupo como un **árbol**, y que el representante sea la **raíz**. Cada elemento guarda a su padre; la raíz es su propio padre. Para saber el representante de `x`, se **sube** de padre en padre hasta llegar a la raíz. Para unir dos grupos, alcanza con colgar **una raíz de la otra**: no hace falta tocar a nadie más.

El truco es *cuál* colgar de cuál. Si se cuelga siempre el árbol **más chico** (el de menos elementos) del **más grande**, los árboles nunca se vuelven muy altos: un elemento solo se hace más profundo cuando su árbol se une a otro **al menos tan grande**, o sea, cuando el tamaño de su árbol por lo menos se **duplica**. Como el tamaño no puede pasar de `n`, la altura de cualquier árbol es a lo sumo **`log₂ n`**. Se llama **unión por tamaño**, y para aplicarla hay que guardar, en cada raíz, el tamaño de su árbol.

Con esto, las dos operaciones costosas bajan a **O(log n)**: encontrar la raíz (subir por un árbol de altura a lo sumo `log₂ n`), y unir (dos búsquedas de raíz y un cambio de padre).

> Existe una optimización más, la **compresión de caminos** (al buscar una raíz, se actualizan los elementos del camino para que apunten directamente a ella), que deja el costo amortizado prácticamente en `O(1)`. No la usamos en la materia, pero es la que se ve en la bibliografía.

## ¿Cuánto cuesta Kruskal?

Con `n` vértices y `m` aristas (como el grafo es conexo, `m ≥ n - 1`):

- Crear la partición: `O(n)`.
- **Ordenar** las aristas: `O(m log m)`.
- Recorrer las `m` aristas preguntando si los extremos están en el mismo grupo: `m` consultas de representantes.
- Unir: sólo se une `n - 1` veces (cada unión agrega una arista al árbol).

Con la representación de **arreglo**: las consultas cuestan `O(1)` y las `n - 1` uniones `O(n)` cada una, así que el total es **O(m log m + n²)**.

Con el **bosque** y unión por tamaño: consultas y uniones cuestan `O(log n)`, y el total baja a **O(m log m)** (el ordenamiento domina).



# Algoritmo de Prim

## La idea de Prim

> **Prim**: hacer crecer **un solo árbol** desde un vértice inicial. En cada paso, agregar la arista **más liviana** entre las que conectan un vértice del árbol con uno de afuera.

- Kruskal mira **todas** las aristas del grafo, de menor a mayor, y va uniendo grupos (arma varios árboles que se van juntando).
- Prim mira **sólo las aristas que salen del árbol** que lleva construido (siempre hay un único árbol).
- En los dos casos se elige siempre la **más liviana disponible**.

## El mismo ejemplo, con Prim

Empezando desde A:

| En el árbol | Aristas que salen del árbol | Se elige |
|---|---|---|
| A | A–B (7), A–C (3) | **A–C** (3) |
| A, C | A–B (7), C–B (1), C–D (8), C–E (9) | **C–B** (1) |
| A, B, C | B–D (6), C–D (8), C–E (9) | **B–D** (6) |
| A, B, C, D | C–E (9), D–E (2), D–F (5) | **D–E** (2) |
| A, B, C, D, E | D–F (5), E–F (4) | **E–F** (4) |

Es el mismo árbol que encontró Kruskal, de peso 16 (pero las aristas se eligieron en otro orden).

## La cola de prioridad de aristas

En cada paso hay que encontrar **la mínima** entre las aristas que salen del árbol: es un trabajo para una **cola de prioridad** (un min-heap), que ya conocemos de la clase de heaps. La cola guarda las aristas candidatas con su peso, y sale primero la de menor peso.

El algoritmo, en palabras:

- Se empieza con el árbol vacío y se **incorpora** el vértice inicial: se lo marca como "en el árbol" y se meten en la cola todas sus aristas hacia vértices que todavía están afuera.
- Mientras la cola no esté vacía, se saca la arista más liviana. Si su extremo de afuera **sigue estando afuera**, la arista se agrega al árbol, y se **incorpora** ese vértice (se marca y se meten en la cola sus aristas hacia afuera).
- Si ese extremo ya estaba en el árbol, la arista se descarta.

Es la **misma estructura que BFS**: una colección de pendientes y un arreglo de marcas. Lo único que cambia es **cuál pendiente sale primero**: ni el más viejo (cola), ni el más nuevo (pila), sino el de menor peso.

Fijate que una arista de la cola puede haber quedado **obsoleta**: se metió cuando su extremo de afuera estaba afuera, pero después ese vértice entró al árbol por otra arista más liviana. Por eso, al sacarla, **hay que volver a preguntar** si el extremo sigue afuera (igual que en DFS había que volver a preguntar si el vértice ya estaba visitado).

## ¿Cuánto cuesta Prim?

- Cada vértice se incorpora una sola vez, y al hacerlo se recorre su lista de vecinos.
- Cada arista entra a la cola a lo sumo **dos veces** (una por extremo), así que hay a lo sumo `2m` inserciones y `2m` eliminaciones.
- Cada inserción y cada eliminación del mínimo cuesta `O(log m)`.

Costo: **O(m log m) = O(m log n)** en peor caso (porque `m ≤ n²`, y entonces `log m ≤ 2 log n`), sobre listas de adyacencia.



# ¿Por qué funcionan? Algoritmos greedy

## Propiedad del corte

Los dos algoritmos toman, en cada paso, la arista más liviana que tienen a mano, y **nunca se arrepienten**. ¿Por qué eso da un árbol óptimo?

> **Propiedad del corte**: separemos los vértices en **dos grupos no vacíos**, de cualquier manera. La arista **más liviana** entre las que **cruzan** de un grupo al otro pertenece a algún AGM.

En el ejemplo, con los grupos `{A, B, C}` y `{D, E, F}`: cruzan B–D (6), C–D (8) y C–E (9). La más liviana, B–D, está en el AGM.

**Idea de la demostración.** Si algún árbol generador no usa esa arista, al agregarla se forma un ciclo, y ese ciclo tiene que cruzar el corte **por otra arista** (para volver al grupo del que salió), más pesada. Cambiando una por otra se obtiene un árbol generador que **no pesa más**. Entonces siempre existe un AGM que usa la arista más liviana del corte.

Esto justifica los dos algoritmos:

- **Kruskal**: cuando elige una arista (que une dos grupos), esa es la más liviana de todas las que cruzan de uno de esos grupos al resto.
- **Prim**: el corte es "los vértices del árbol" contra "los de afuera", y elige justo la más liviana que cruza.

## Algoritmos greedy

> **Algoritmo greedy (goloso)**: un algoritmo que construye la solución **paso a paso**, eligiendo en cada paso la opción que **parece mejor en ese momento**, **sin reconsiderar** las decisiones ya tomadas.

- **Ventaja**: suelen ser simples y muy eficientes (no exploran alternativas).
- **Cuidado**: en la mayoría de los problemas, elegir lo mejor localmente **no** lleva a la mejor solución global. Un greedy necesita una **demostración** de que funciona (para el AGM, la propiedad del corte).

> Pregunta para pensar: hay que dar un vuelto de 6 con la menor cantidad de monedas, y hay monedas de 1, 3 y 4. ¿Qué hace el greedy "usar siempre la moneda más grande que entre"? ¿Es óptimo?

El greedy usa 4, después 1, después 1: **3 monedas**. Pero 3 + 3 son sólo **2**. Con monedas de 1, 5, 10 y 25 sí es óptimo, pero eso hay que demostrarlo: no es automático.

## Kruskal vs Prim

| | Kruskal | Prim |
|---|---|---|
| Elige la arista más liviana... | que **no forma ciclo** | que **sale del árbol** |
| Va construyendo | varios árboles que se unen | un único árbol que crece |
| Estructura auxiliar | conjuntos disjuntos | cola de prioridad |
| Costo temporal (peor caso) | O(m log m + n²) con arreglo, O(m log m) con bosque | O(m log m) |
| Representación cómoda | lista de aristas | listas de adyacencia |

Los dos son correctos por la misma razón (la propiedad del corte), y los dos se apoyan en una estructura que hace eficiente la elección greedy.



# Camino mínimo

## El camino más barato

En un mapa, los vértices son esquinas y las aristas son calles, cada una con el tiempo que lleva recorrerla. ¿Cuál es la forma más rápida de ir de A a F (en el mismo grafo del ejemplo)?

> Pregunta para pensar: BFS desde A dice que B está a distancia 1 (una arista, de peso 7). ¿Es ese el camino más barato de A a B?

No: A → C → B pesa `3 + 1 = 4`. BFS cuenta aristas, no pesos. Hace falta otro algoritmo.

## Peso de un camino y distancia

Sea `G` un grafo ponderado (dirigido o no), con pesos `p : E → R`.

> **Peso de un camino**: la suma de los pesos de sus aristas.
>
> **Camino mínimo** de `v` a `w`: un camino de `v` a `w` de peso mínimo.
>
> **Distancia** `δ(v, w)`: el peso de un camino mínimo (`∞` si no hay ningún camino).

**El problema (desde un origen):**

- **Entrada**: un grafo ponderado `G` y un vértice `v`.
- **Salida**: la distancia `δ(v, x)` para **todo** vértice `x`.

Con todos los pesos iguales a 1 es exactamente lo que calcula BFS.

## Dos ideas que usan todos los algoritmos

**Subestructura óptima.** Si un camino mínimo de `v` a `w` pasa por `x`, entonces su tramo de `v` a `x` es un camino mínimo de `v` a `x`. (Si hubiera uno mejor hasta `x`, lo usaríamos y mejoraríamos el camino a `w`.)

**Estimaciones que mejoran.** Los algoritmos mantienen un arreglo `dist`, con `dist[x]` = el peso del mejor camino de `v` a `x` encontrado **hasta ahora** (una *cota superior* de `δ(v, x)`). Empieza en **0 para `v` y en ∞ para los demás**, y se va mejorando de a una arista.

## Mejorar una estimación con una arista

La operación básica, que en la bibliografía se llama **relajar** (*relax*):

> "Si sé llegar a `x` con costo `dist[x]`, y hay una arista de `x` a `y` de peso `p`, entonces sé llegar a `y` con costo `dist[x] + p`."

**Mejorar(x, y, p)**: si `dist[x] + p < dist[y]`, se actualiza `dist[y] = dist[x] + p` y se informa que hubo una mejora. Después de invocarla, vale siempre `dist[y] ≤ dist[x] + p`.

Dos casos:

- Ya se conocía un camino a `y` de peso 10, y `dist[x] = 3`, con una arista `x → y` de peso 2: se mejora, `dist[y]` pasa de 10 a 5.
- Todavía no se conocía ninguno (`dist[y] = ∞`): `dist[y]` pasa de `∞` a 5.

Si `dist[x] = ∞`, todavía no se sabe llegar a `x`: `∞ + p` no es menor que nada, y no cambia nada. (Ojo al programarlo: sumar sobre un `∞` representado con el mayor entero posible **se desborda**.)

Todos los algoritmos de camino mínimo son maneras distintas de decidir **en qué orden** aplicar Mejorar.



# Algoritmo de Dijkstra

## Condición sobre los pesos

> Todos los pesos son **no negativos**: `p(e) ≥ 0` para toda arista `e`.

Es lo que pasa cuando los pesos son distancias, tiempos o costos.

**Consecuencia**: agregarle aristas a un camino **nunca lo hace más liviano**. Entonces, el vértice *no terminado* con menor `dist` ya tiene su distancia definitiva: cualquier otro camino hacia él pasa antes por un vértice que está, al menos, igual de lejos.

## La idea de Dijkstra

> **Dijkstra**: ir **terminando** los vértices de a uno, del más cercano al origen al más lejano. En cada paso:
>
> 1. Tomar el vértice **no terminado** con menor `dist`, y darlo por terminado.
> 2. Aplicar **Mejorar** a todas las aristas que salen de él.

Dos cosas para notar:

- Es un algoritmo **greedy**: una vez que un vértice se da por terminado, su distancia **no se revisa más**.
- Es **BFS con una cola de prioridad** en lugar de una cola: sale primero el pendiente más cercano al origen, no el más viejo.

## Un ejemplo paso a paso

Desde A. En cada paso se termina el vértice no terminado de menor `dist`, y se aplica Mejorar a las aristas que salen de él hacia los no terminados. Cada fila es el estado de `dist` **después** del paso:

| Se termina | A | B | C | D | E | F |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| inicio | 0 | ∞ | ∞ | ∞ | ∞ | ∞ |
| A | 0 | 7 | 3 | ∞ | ∞ | ∞ |
| C | 0 | **4** | 3 | 11 | 12 | ∞ |
| B | 0 | 4 | 3 | **10** | 12 | ∞ |
| D | 0 | 4 | 3 | 10 | 12 | **15** |
| E | 0 | 4 | 3 | 10 | 12 | 15 |
| F | 0 | 4 | 3 | 10 | 12 | 15 |

Fijate qué pasó:

- `dist[B]` empezó en 7 (la arista directa desde A) y **mejoró a 4** al terminar C (`3 + 1`).
- Al terminar D, la arista D–E **no mejora**: `10 + 2 = 12` no es menor que 12.
- Los vértices se terminan **en orden de distancia**: `0, 3, 4, 10, 12, 15` (A, C, B, D, E, F).

## Detalles que importan

La cola de prioridad guarda pares **⟨distancia, vértice⟩** y saca primero el de menor distancia.

- Cada vez que Mejorar tiene éxito, el vértice **entra a la cola** con su nueva distancia.
- Por eso un vértice puede estar **varias veces** en la cola, con distintas distancias (una por cada vez que mejoró). **La primera vez que sale es con la menor**; las demás veces ya está terminado y se **descartan**.

**Invariante.**

- Para todo vértice **terminado** `x`: `dist[x] = δ(v, x)`.
- Para todo vértice **no terminado** `y`: `dist[y]` es el peso del mejor camino de `v` a `y` que pasa **sólo por vértices terminados** (salvo el propio `y`).

**¿Por qué el de menor `dist` ya es definitivo?** Cualquier otro camino hacia él tiene que salir del conjunto de terminados por algún vértice `y` con `dist[y]` mayor o igual, y después sólo suma pesos no negativos.

## ¿Cuánto cuesta Dijkstra?

Sobre listas de adyacencia, con la cola de prioridad sobre un heap:

- Cada vértice se termina **una sola vez**; al terminarlo se recorren sus vecinos: en total, `O(m)` llamados a Mejorar.
- Cada Mejorar exitoso hace una inserción: a lo sumo `O(m)` elementos pasan por la cola.
- Cada inserción y cada eliminación del mínimo cuesta `O(log m)`.

Costo: **O((n + m) log m) = O((n + m) log n)** en peor caso.

(Con **matriz de adyacencia** y buscando el mínimo con un recorrido lineal de `dist`, cuesta `O(n²)`: conviene para grafos densos.)

## No sólo la distancia: el camino

Hasta ahora calculamos *cuánto* cuesta llegar, pero no *por dónde*.

**Idea**: guardar, para cada vértice `y`, **desde qué vértice se llegó** la última vez que `dist[y]` mejoró: su *padre*. Se actualiza en Mejorar, junto con `dist[y]`. Los punteros padre forman un **árbol con raíz en el origen**: el **árbol de caminos mínimos**.

Para reconstruir un camino mínimo de `v` a `w`, se parte de `w` y se va saltando al padre hasta llegar a `v` (y se da vuelta lo que quedó), igual que con BFS. Cuesta `O(ℓ)`, con `ℓ` la cantidad de aristas del camino.

En el ejemplo, el camino mínimo de A a F es **A → C → B → D → F**, de peso 15.

> Ojo: el árbol de caminos mínimos **no es** el árbol generador mínimo. Comparalo con el AGM de arriba: el AGM usaba D–E y E–F; el árbol de caminos mínimos llega a E desde C y a F desde D. Son problemas distintos: uno minimiza el costo **total** de conectar todo, el otro minimiza el costo de **cada camino desde el origen**.

## ¿Y con pesos negativos?

Tomemos tres vértices: A → B de peso 2, A → C de peso 3, C → B de peso −2.

> Pregunta para pensar: ¿qué distancia de A a B calcula Dijkstra? ¿Cuál es la real?

- Dijkstra termina B con `dist[B] = 2` **antes** de mirar C (que está a distancia 3).
- Pero A → C → B pesa `3 + (−2) = 1`. Dijkstra se equivoca.
- Falló la consecuencia en la que se apoyaba: con pesos negativos, un camino más largo **sí** puede ser más liviano, y entonces lo "definitivo" ya no es definitivo.

Para este caso necesitamos otro algoritmo.



# Algoritmo de Bellman-Ford

## Pesos y ciclos negativos

Hay problemas donde los pesos negativos son naturales: una arista puede representar una **ganancia** en vez de un costo (un tramo donde se cobra, una operación de cambio favorable, ...).

> **Ciclo negativo**: un ciclo cuyo peso total es negativo.

Por ejemplo, un grafo con A → B (1), B → C (2), C → D (−4), D → B (1) y C → E (3). El ciclo B → C → D → B pesa `2 − 4 + 1 = −1`: **cada vuelta abarata el camino**. De A a E no hay un camino mínimo: la distancia sería `−∞`.

(Un grafo **no dirigido** con una arista de peso negativo ya tiene un ciclo negativo: ir y volver por la misma arista.)

## La idea de Bellman-Ford

**Condición**: el grafo es **dirigido** y puede tener pesos negativos, pero no tiene ciclos negativos **alcanzables desde el origen**. (Si los tiene, el algoritmo lo **detecta**.)

> **Bellman-Ford**: no intentar adivinar en qué orden terminar los vértices. Aplicar **Mejorar a todas las aristas, una y otra vez**.

¿Cuántas veces?

- Sin ciclos negativos, un camino mínimo **no repite vértices**: tiene a lo sumo `n − 1` aristas.
- Después de la **ronda `i`** (una pasada por todas las aristas), quedan bien calculadas todas las distancias cuyo camino mínimo tiene **a lo sumo `i` aristas**.
- Entonces alcanzan **`n − 1` rondas**.

## Un ejemplo paso a paso

Desde A, con cinco vértices y las aristas A → B (6), A → C (4), C → B (−2), B → D (3), C → E (9) y D → E (2), que se recorren en cada ronda en este orden: D → E, B → D, C → E, C → B, A → B, A → C. Cada fila es `dist` al terminar la ronda:

| Ronda | A | B | C | D | E |
|:-:|:-:|:-:|:-:|:-:|:-:|
| inicio | 0 | ∞ | ∞ | ∞ | ∞ |
| 1 | 0 | 6 | 4 | ∞ | ∞ |
| 2 | 0 | 2 | 4 | 9 | 13 |
| 3 | 0 | 2 | 4 | 5 | 11 |
| 4 | 0 | 2 | 4 | 5 | 7 |
| control | nada mejora: no hay ciclo negativo | | | | |

- E necesitó las 4 rondas: su camino mínimo, A → C → B → D → E, tiene **4 aristas**, y cada ronda asegura una más.
- Con las aristas en el **orden inverso**, todo quedaba bien en la primera ronda. El orden cambia **cuántas rondas hacen falta**, pero `n − 1` alcanzan **siempre**.

> Pregunta para pensar: si en una ronda completa ningún Mejorar cambió nada, ¿hace falta seguir con las rondas que quedan?

(No: si nada cambió, las rondas siguientes harían exactamente lo mismo. Se puede cortar antes.)

## Detección de ciclos negativos y costo

- Si **no hay** ciclos negativos, después de `n − 1` rondas ninguna arista mejora nada.
- Se hace una **ronda extra de control**: si alguna arista **todavía mejora** algo, es porque hay un camino de `n` o más aristas que es mejor que todos los más cortos. Un camino así tiene que repetir un vértice, y ese ciclo tiene que ser negativo. Entonces, **hay un ciclo negativo**.
- Sólo importan los ciclos negativos **alcanzables desde el origen**: uno que no se alcanza nunca se llega a recorrer.

**Costo**: `n − 1` rondas; en cada una se recorren las `m` aristas (y obtener la lista de aristas cuesta `O(n + m)`). En total, **O(n · (n + m))** en peor caso. Si todos los vértices son alcanzables desde el origen, `m ≥ n − 1` y queda **O(n · m)**: así suele aparecer en la bibliografía.

## Camino mínimo desde un origen: resumen

Con `n` la cantidad de vértices y `m` la de aristas, en peor caso:

| | BFS | Dijkstra | Bellman-Ford |
|---|---|---|---|
| Pesos | todos iguales | no negativos | cualesquiera |
| Estrategia | por niveles | **greedy** | mejorar con todas, `n − 1` veces |
| Estructura auxiliar | cola | cola de prioridad | ninguna |
| Costo temporal | O(n + m) | O((n + m) log n) | O(n · (n + m)) |
| Ciclos negativos | — | — | los detecta |

Cuanto menos suponemos sobre los pesos, **más cuesta** el algoritmo. Conviene usar el más específico que el problema permita.

> Tip para el parcial: si te preguntan "¿qué algoritmo usarías?", primero identificá el tipo de problema (AGM, o camino mínimo desde un origen) y recién después fijate en las restricciones (¿pesos negativos?, ¿tamaño?, ¿grafo denso o ralo?).



# Material adicional: flujo máximo

> Esta sección es **material adicional**: no se evalúa con los ejercicios de esta clase, pero es el próximo paso natural con los grafos ponderados.

## Red de flujo

Hasta ahora el peso de una arista era un **costo**. En una **red de flujo**, el peso es una **capacidad**: cuánto puede pasar, como máximo, por un caño, una ruta o un enlace.

> **Red de flujo**: un grafo **dirigido** donde cada arista `(u, v)` tiene una **capacidad** máxima. Hay un vértice **fuente** `s` (de donde "sale" todo el flujo) y un vértice **sumidero** `t` (a donde "llega" todo el flujo).

Un **flujo** es una asignación de valores a las aristas que respeta dos reglas:

- **Restricción de capacidad**: el flujo por una arista no puede superar su capacidad.
- **Conservación de flujo**: para todo vértice que no sea `s` ni `t`, lo que **entra** es igual a lo que **sale**.

El **flujo máximo** es el mayor valor total de flujo que puede salir de `s` y llegar a `t` sin violar ninguna regla.

Ejemplos típicos: la capacidad máxima de una red de tuberías, el ancho de banda máximo entre dos routers, o la cantidad máxima de mercadería que se puede transportar entre dos ciudades usando rutas con capacidades limitadas.

## Ford-Fulkerson y Edmonds-Karp

La idea de **Ford-Fulkerson**: mientras exista un **camino aumentante** (un camino de `s` a `t` por aristas que todavía tienen capacidad disponible), **aumentar** el flujo total en la cantidad del **cuello de botella** (la menor capacidad disponible a lo largo de ese camino).

Para que el algoritmo pueda **corregir decisiones anteriores**, se trabaja sobre el **grafo residual**: por cada arista `(u, v)` con flujo asignado, aparece también una arista "de vuelta" `(v, u)` que permite **deshacer** parte de ese flujo si conviene.

**Edmonds-Karp** es la implementación concreta de Ford-Fulkerson que busca el camino aumentante con **BFS**: siempre el que tiene menos aristas. Eso garantiza una cota de costo, **O(n · m²)**, que no depende de los valores de las capacidades.

- Se suele representar la red con una **matriz** de capacidades y otra de flujos, porque hay que consultar y actualizar la capacidad entre cualquier par de vértices, incluyendo las aristas "de vuelta" que al principio no existían.
- Ford-Fulkerson **no es greedy en sentido estricto**: gracias a las aristas inversas, puede deshacer flujo asignado antes. Pero cada iteración sí elige de forma local el próximo camino a explotar.

> Pregunta para pensar: ¿por qué buscar el camino aumentante con BFS (Edmonds-Karp) da una cota de costo garantizada, mientras que Ford-Fulkerson con cualquier otra búsqueda (por ejemplo, DFS) puede necesitar muchísimas más iteraciones?

## Teorema max-flow min-cut

Un **corte** de la red es una partición de los vértices en dos conjuntos: uno que contiene a `s` y otro que contiene a `t`. La **capacidad del corte** es la suma de las capacidades de las aristas que van del conjunto de `s` al de `t`.

> **Teorema max-flow min-cut**: el valor del flujo máximo de `s` a `t` es **exactamente igual** a la capacidad del **corte mínimo**.

Intuitivamente, el corte mínimo es el verdadero **cuello de botella** de toda la red: no importa cuántos caminos alternativos existan, el flujo nunca puede superar la capacidad total del conjunto de aristas que separa a la fuente del sumidero.

Una vez calculado el flujo máximo, se puede encontrar un corte mínimo así: se hace un último BFS sobre el grafo residual final desde `s`; los vértices alcanzables forman un lado del corte, y los no alcanzables el otro. Las aristas originales que cruzan de un lado al otro son las del corte mínimo.

> Pregunta para pensar: ¿por qué las aristas del corte mínimo tienen que estar **saturadas** (con flujo igual a su capacidad) en cualquier flujo máximo?

## Un último algoritmo: Floyd-Warshall

Cuando se necesitan las distancias mínimas entre **todos los pares** de vértices (y no sólo desde un origen), una opción es **Floyd-Warshall**: programación dinámica sobre una matriz de distancias, que prueba para cada par `(i, j)` si pasar por un vértice intermedio `k` mejora la distancia.

- Soporta pesos negativos (aunque no ciclos negativos, que se detectan por una diagonal negativa).
- Cuesta **O(n³)**: conviene cuando `n` es chico, típicamente hasta unos pocos cientos de vértices.
- Alternativa: correr Dijkstra (o Bellman-Ford) desde cada vértice.

Resumen de algoritmos y estrategias de la clase:

| Algoritmo | Problema | Costo | ¿Es greedy? |
|---|---|---|---|
| Kruskal | AGM | O(m log m) | Sí |
| Prim | AGM | O(m log m) | Sí |
| Dijkstra | Camino mínimo (un origen, pesos ≥ 0) | O((n + m) log n) | Sí |
| Bellman-Ford | Camino mínimo (un origen, cualquier peso) | O(n · (n + m)) | No (mejora iterativa) |
| Floyd-Warshall | Camino mínimo (todos contra todos) | O(n³) | No (programación dinámica) |
| Edmonds-Karp | Flujo máximo | O(n · m²) | Parcialmente |



# Repaso relámpago

- ¿Qué es un árbol generador? ¿Cuántas aristas tiene? ¿Puede haber más de un AGM?
- ¿Cómo sabe Kruskal si una arista forma un ciclo?
- ¿Qué operaciones tiene el TAD Conjuntos disjuntos? ¿Cuál es la cara con la representación de arreglo? ¿Qué mejora el bosque con unión por tamaño?
- ¿Qué guarda la cola de prioridad de Prim? ¿Por qué hay que volver a preguntar si el extremo sigue afuera al sacar una arista?
- ¿Qué dice la propiedad del corte?
- ¿Qué es un algoritmo greedy? ¿Todo algoritmo greedy da la solución óptima? (Pensá en el vuelto con monedas de 1, 3 y 4.)
- ¿En qué se parecen Kruskal y Prim? ¿En qué se diferencian?
- ¿Qué hace Mejorar? ¿Cómo se llama en la bibliografía?
- ¿Qué le pide Dijkstra a los pesos, y dónde se usa esa condición?
- ¿Por qué un vértice puede estar varias veces en la cola de prioridad de Dijkstra?
- ¿El árbol de caminos mínimos es el árbol generador mínimo?
- ¿Por qué Bellman-Ford hace `n − 1` rondas? ¿Cómo detecta un ciclo negativo?
- Si el grafo tiene todos los pesos iguales, ¿qué algoritmo de camino mínimo conviene? ¿Y si tiene pesos negativos?



# Próximamente

- **Backtracking**: cuando no hay una regla greedy que alcance, ¿cómo se exploran sistemáticamente *todas* las soluciones posibles, descartando las que no pueden servir?



# Sobre los ejercicios

Los ejercicios de esta clase (en la carpeta `ejercicios`) siguen el orden de las teóricas y cada parte arma **un algoritmo**, con el mismo pseudocódigo de los handouts. **No hay ejercicios de flujo máximo** (es material adicional).

- **Parte 1 — Kruskal**: los conjuntos disjuntos con un arreglo (`CrearParticion`, `Representante`, `Unir`), las aristas ordenadas por peso y el algoritmo.
- **Parte 2 — Prim**: `Incorporar` un vértice al árbol y el algoritmo, con la cola de prioridad de aristas.
- **Parte 3 — Camino mínimo**: la operación `Mejorar`, **Dijkstra**, el árbol de caminos mínimos y la reconstrucción del camino, y **Bellman-Ford** con la detección de ciclos negativos.
- **Parte 4 — Opcionales**: Kruskal con un bosque con unión por tamaño, la propiedad del corte, un greedy que **no** es óptimo y cuántas rondas necesita Bellman-Ford.
