Tutorial 10 – Heaps
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 10 – Heaps](#tutorial-10--heaps)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [Heaps](#heaps)
  - [¿Qué es un heap?](#qué-es-un-heap)
  - [Representación sobre un arreglo](#representación-sobre-un-arreglo)
  - [Primitivas](#primitivas)
    - [insert (sift-up)](#insert-sift-up)
    - [extractMin / extractMax (sift-down)](#extractmin--extractmax-sift-down)
    - [heapify / buildHeap](#heapify--buildheap)
  - [Heapsort](#heapsort)
  - [El heap como cola de prioridad](#el-heap-como-cola-de-prioridad)
  - [Patrones típicos con heaps](#patrones-típicos-con-heaps)
  - [Repaso de complejidades](#repaso-de-complejidades)
  - [La STL: `std::priority_queue`](#la-stl-stdpriority_queue)



# Objetivos de la clase
- Comprender la definición de heap (min-heap y max-heap) como árbol binario casi completo.
- Entender cómo se representa un heap sobre un arreglo, y cómo funcionan `insert`, `extractMin`/`extractMax` y `buildHeap`.
- Entender heapsort y el uso del heap como cola de prioridad.
- Analizar la complejidad de cada primitiva del heap.

> En estos apuntes los algoritmos están en **pseudocódigo**. La implementación en C++ es parte de los ejercicios.



# Heaps

## ¿Qué es un heap?

Un **heap** es una estructura de datos pensada para responder eficientemente una pregunta muy puntual: *¿cuál es el elemento de mínima (o máxima) prioridad que tengo guardado?*

Se implementa como un **árbol binario casi completo**: todos los niveles están completamente llenos, excepto posiblemente el último, que se llena **de izquierda a derecha**. Esta forma tan regular es justamente lo que permite representarlo de manera compacta con un simple arreglo, sin necesidad de punteros.

Además de la forma, un heap cumple la **propiedad de heap**:

- **Min-heap**: el valor de cada nodo es **menor o igual** que el de sus hijos. La raíz es siempre el mínimo.
- **Max-heap**: el valor de cada nodo es **mayor o igual** que el de sus hijos. La raíz es siempre el máximo.

> Un heap **no está totalmente ordenado**: solo garantiza la relación entre un nodo y sus hijos directos. Dos elementos que no tienen relación de ancestro-descendiente pueden estar en cualquier orden entre sí.

> Pregunta para pensar: si un min-heap tiene la propiedad de que un padre es menor que sus hijos, ¿dónde se encuentra el elemento máximo del heap? ¿Se puede encontrar en O(1)?

## Representación sobre un arreglo

Como el heap es un árbol *casi completo*, podemos numerar sus nodos por niveles (0, 1, 2, ...) y guardarlos en un arreglo en ese mismo orden, sin usar punteros. Por ejemplo, este min-heap:

```
                 1
            /         \
           3           2
         /   \       /   \
        7     4     5     9
       / \   /
      8  10 6

posición:  0  1  2  3  4  5  6  7  8  9
A       = [1, 3, 2, 7, 4, 5, 9, 8, 10, 6]
```

Indexando desde 0, las relaciones padre-hijo quedan:

| Relación | Fórmula (índice `i`) |
|---|---|
| Padre de `i` | `(i - 1) / 2` (división entera) |
| Hijo izquierdo de `i` | `2 * i + 1` |
| Hijo derecho de `i` | `2 * i + 2` |

Un hijo **existe** solo si su posición es menor que `n`, la cantidad de elementos. Las hojas son las posiciones de `n/2` a `n - 1`, y los nodos con al menos un hijo, las de `0` a `n/2 - 1`.

El heap se representa entonces con:

```
MinHeap:
    A: arreglo de elementos     // A[0] es la raíz
    n: cantidad de elementos

Padre(i)   = (i - 1) / 2
HijoIzq(i) = 2 * i + 1
HijoDer(i) = 2 * i + 2

Vacío(H)  = (H.n == 0)
Mínimo(H) = H.A[0]               // pre: no vacío
```

> Tip: no hace falta guardar punteros a "padre" o "hijos": toda la estructura del árbol está implícita en la posición dentro del arreglo. Por eso el heap es tan compacto en memoria.

## Primitivas

### insert (sift-up)

Para insertar un nuevo valor:

1. Se agrega al **final** del arreglo (siguiente posición libre del árbol casi completo). O(1) (amortizado, si el arreglo tiene que crecer).
2. Puede violar la propiedad de heap con respecto a su padre, así que "sube" intercambiándose con él mientras sea menor (en un min-heap). A esto se lo llama **sift-up** o **heapUp**.

```
SiftUp(A, i):
    mientras i > 0  y  A[Padre(i)] > A[i]:
        intercambiar A[Padre(i)] con A[i]
        i ← Padre(i)

Insertar(H, valor):
    H.A[H.n] ← valor
    H.n ← H.n + 1
    SiftUp(H.A, H.n - 1)
```

Como la altura del árbol es O(log n), el elemento sube a lo sumo O(log n) niveles: **insert es O(log n)**.

### extractMin / extractMax (sift-down)

Para extraer el mínimo (la raíz):

1. Se guarda el valor de la raíz para devolverlo al final.
2. Se mueve el **último elemento** del arreglo a la posición de la raíz y se achica el heap en uno.
3. Ese elemento probablemente viole la propiedad de heap, así que "baja" intercambiándose con el **menor de sus dos hijos**, mientras sea mayor que alguno de ellos. A esto se lo llama **sift-down** o **heapDown**.

`SiftDown` recibe también `n`, el tamaño del heap: solo se consideran las posiciones de `0` a `n - 1` (esto va a ser útil para heapsort).

```
SiftDown(A, i, n):
    repetir:
        menor ← i
        izq ← HijoIzq(i)
        der ← HijoDer(i)

        si izq < n  y  A[izq] < A[menor]:  menor ← izq
        si der < n  y  A[der] < A[menor]:  menor ← der

        si menor == i:  terminar           // ya cumple la propiedad de heap

        intercambiar A[i] con A[menor]
        i ← menor

ExtraerMínimo(H):                          // pre: no vacío
    mínimo ← H.A[0]
    H.A[0] ← H.A[H.n - 1]
    H.n ← H.n - 1
    si H.n > 0:
        SiftDown(H.A, 0, H.n)
    devolver mínimo
```

Al igual que el insert, el elemento baja a lo sumo la altura del árbol: **extractMin es O(log n)**.

> Pregunta para pensar: en `SiftDown`, ¿por qué hay que comparar con el **menor** de los dos hijos y no alcanza con comparar con cualquiera de los dos?

> Ojo con los nodos que tienen **un solo hijo** (solo puede haber uno así, el último nodo interno): el hijo derecho no existe, y por eso cada comparación chequea primero que la posición sea menor que `n`.

### heapify / buildHeap

Si ya tenemos un arreglo con `n` elementos desordenados y queremos convertirlo en un heap válido, hay varias estrategias posibles:

| Estrategia | Complejidad |
|---|---|
| Insertar los `n` elementos uno por uno con `insert` | O(n log n) |
| Hacer `siftUp` desde la posición 0 hasta la n-1 | O(n log n) |
| Hacer `siftDown` desde la posición 0 hasta la n-1 | No funciona: los subárboles de abajo todavía no son heaps válidos |
| Hacer `siftDown` desde el **último nodo con hijos** hasta la raíz | **O(n), óptimo** |

La versión óptima aprovecha que las hojas ya son heaps válidos por definición (no tienen hijos), así que alcanza con arreglar los nodos internos, empezando por los de más abajo:

```
BuildHeap(A, n):
    para i desde n/2 - 1 bajando hasta 0:
        SiftDown(A, i, n)
```

> ¿Por qué esto es O(n) y no O(n log n)? Porque la mayoría de los nodos están en los niveles inferiores del árbol, y esos nodos hacen `siftDown` de muy pocos niveles (las hojas, de hecho, no hacen ninguno). Solo unos pocos nodos, cerca de la raíz, bajan hasta O(log n) niveles. Al sumar el trabajo de todos los niveles, la serie converge a O(n).

> Detalle de implementación: si el índice es un entero **sin signo** (como `size_t` en C++), la condición "mientras i ≥ 0" es siempre verdadera. Hay que pensar el recorrido para atrás de otra forma.

## Heapsort

Una vez que sabemos construir un heap en O(n) y extraer el máximo en O(log n), **heapsort** sale casi gratis:

1. Construir un **max-heap** a partir del arreglo. O(n).
2. Repetir n - 1 veces: intercambiar la raíz (el máximo) con el último elemento del heap "activo", achicar el heap en uno, y hacer `siftDown` desde la raíz. O(log n) cada vez.

Para ordenar de menor a mayor se usa un **max-heap**: así el máximo va a parar al final del arreglo, que es donde tiene que quedar. `SiftDownMax` es igual a `SiftDown`, pero intercambia con el **mayor** de los hijos.

```
Heapsort(A, n):
    // 1. buildHeap (max-heap), O(n)
    para i desde n/2 - 1 bajando hasta 0:
        SiftDownMax(A, i, n)

    // 2. extraer el máximo n - 1 veces
    para fin desde n - 1 bajando hasta 1:
        intercambiar A[0] con A[fin]       // el máximo va a su posición final
        SiftDownMax(A, 0, fin)             // el heap activo ahora tiene tamaño fin
```

En todo momento el arreglo queda partido en dos: las posiciones `0` a `fin - 1` son el heap activo, y de `fin` en adelante están los más grandes, ya ordenados.

La complejidad total es **O(n log n)**, con la ventaja de que ordena **in-place** (usa O(1) memoria extra), a diferencia de mergesort.

> Pregunta para pensar: ¿heapsort es un algoritmo de ordenamiento estable? Es decir, ¿dos elementos iguales conservan su orden relativo original?

## El heap como cola de prioridad

Una **cola de prioridad** (*priority queue*) es un TAD (tipo abstracto de datos) que permite:

- agregar elementos con una prioridad asociada,
- extraer siempre el elemento de mayor (o menor) prioridad.

El heap es la implementación clásica de una cola de prioridad, porque ofrece exactamente esas dos operaciones en O(log n). Se usa, entre otras cosas, en:

- **Algoritmos de grafos**: Dijkstra, Prim, A*.
- **Simulaciones de eventos discretos**: procesar siempre el próximo evento en el tiempo.
- **Sistemas operativos**: planificadores que eligen el próximo proceso según su prioridad.
- **Compresión de Huffman**: combinar siempre los dos nodos de menor frecuencia.

## Patrones típicos con heaps

Muchos problemas que a primera vista piden "ordenar todo" se resuelven mejor con un heap. Algunos patrones que conviene reconocer:

- **Sacar siempre los dos extremos y volver a meter el resultado.** Si en cada paso hay que combinar los dos elementos más chicos (o más grandes) y el resultado vuelve al conjunto, un heap da cada par en O(log n). Es la idea del código de Huffman: unir siempre las dos frecuencias más chicas.
- **Top-k con un heap de tamaño k.** Para quedarse con los k elementos más grandes de una secuencia, se mantiene un **min-heap** con a lo sumo k elementos: cada vez que se pasa de k, se saca el mínimo (el que ya no está entre los k más grandes). Al final, la raíz es el k-ésimo mayor. Cuesta O(n log k) en vez de O(n log n), y sirve aunque los datos lleguen de a uno.
- **Mezclar k secuencias ordenadas.** El heap guarda, para cada secuencia, su próximo elemento pendiente (junto con de qué secuencia viene). El mínimo del heap es el próximo de la salida; al sacarlo, se mete el siguiente de esa misma secuencia. Con N elementos en total, cuesta O(N log k).
- **Dos heaps para la mediana.** Un **max-heap** con la mitad más chica y un **min-heap** con la mitad más grande, balanceados para que difieran en a lo sumo un elemento. La mediana está siempre en las raíces, y cada número nuevo cuesta O(log n).

## Repaso de complejidades

| Primitiva | Complejidad |
|---|---|
| `build` (heap vacío) | O(1) |
| `getMin` / `getMax` | O(1) |
| `insert` | O(log n) |
| `extractMin` / `extractMax` | O(log n) |
| `buildHeap` (heapify) | O(n) |
| `heapsort` | O(n log n) |

## La STL: `std::priority_queue`

C++ ya trae un heap implementado en la librería estándar: `std::priority_queue` (en el header `<queue>`). Sus operaciones corresponden a las primitivas de arriba:

| Primitiva | `std::priority_queue` | Complejidad |
|---|---|---|
| `insert` | `push(x)` | O(log n) |
| `getMax` | `top()` | O(1) |
| `extractMax` | `top()` para leerlo y después `pop()` para sacarlo | O(log n) |
| ¿vacío? / tamaño | `empty()` / `size()` | O(1) |

Dos detalles para tener en cuenta:

- Por defecto es un **max-heap**: `top()` devuelve el **máximo**.
- Para que sea un **min-heap** hay que pasarle el comparador `std::greater` (del header `<functional>`), que da vuelta el orden. El tipo completo queda `std::priority_queue<int, std::vector<int>, std::greater<int>>`.

> Tip: en los ejercicios y parciales suele pedirse implementar el heap "a mano" para entender cómo funciona por dentro, pero en código de uso general siempre conviene usar `std::priority_queue`.
