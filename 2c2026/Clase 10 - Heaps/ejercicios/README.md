# Ejercicios - Tut 10 (Heaps)

Cáscara para resolver los ejercicios de la décima clase: el heap implementado
"a mano" sobre un arreglo, heapsort, y problemas clásicos que se resuelven
usando el heap como cola de prioridad.

## Archivos

- `ejercicios.h` — declaraciones de las funciones a implementar (con la descripción de cada una) y la estructura `MinHeap`. **No hace falta modificarlo.**
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
   porque las funciones están vacías (algunas pruebas, como "una hoja no se
   mueve" o "falla en la raíz", pasan de casualidad: no te confíes).
   Implementá cada función en `ejercicios.cpp` y volvé a correr `make run`
   hasta que pasen todas.

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

## La estructura

Un min-heap de enteros guardado en un vector, nivel por nivel y de izquierda a
derecha, indexando desde 0. No hay punteros: el árbol está implícito en las
posiciones.

```cpp
struct MinHeap {
    std::vector<int> datos;   // datos[0] es la raíz (el mínimo)
};
```

| Relación | Posición |
|---|---|
| Padre de `i` | `(i - 1) / 2` |
| Hijo izquierdo de `i` | `2i + 1` |
| Hijo derecho de `i` | `2i + 2` |

**Propiedad de min-heap:** cada elemento es menor o igual que sus hijos.
Todas las operaciones tienen que dejarla cumplida.

`siftUp`, `siftDown` y `buildHeap` reciben directamente un `std::vector<int>`
(y no un `MinHeap`) para que se puedan reusar en `buildHeap` y en `heapsort`.
`siftDown` además recibe `n`, el tamaño del heap "activo": solo se miran las
posiciones de `0` a `n - 1`.

> Los índices son `size_t`. Ojo al recorrer para atrás: con un `size_t`,
> `i >= 0` es **siempre** verdadero, así que `for (size_t i = ...; i >= 0; i--)`
> no termina nunca.

## Sobre los ejercicios

### Parte 1 — El heap sobre un arreglo

- **padre**, **hijoIzq**, **hijoDer**: la aritmética de posiciones.
- **siftUp**: subir un elemento mientras sea menor que su padre.
- **siftDown**: bajar un elemento intercambiándolo con el **menor** de sus hijos.
- **insertar** / **minimo** / **extraerMinimo**: las primitivas de la cola de prioridad.
- **buildHeap**: convertir un vector cualquiera en heap en **O(n)** (no O(n log n)).
- **esMinHeap**: chequear la propiedad de heap.
- **maximoDeMinHeap**: la "pregunta para pensar" de los apuntes: ¿dónde está el máximo de un min-heap?

### Parte 2 — Heapsort y selección

- **heapsort**: ordenar in-place con un **max-heap** (vas a tener que escribir un `siftDown` para max-heap).
- **kEsimoMayor**: un min-heap de tamaño k para no ordenar todo.

### Parte 3 — El heap como cola de prioridad

- **piedraMasPesada**: sacar siempre los dos más grandes.
- **costoUnirSogas**: sacar siempre los dos más chicos (la idea de Huffman).
- **mezclarOrdenados**: un heap con "el siguiente de cada lista".
- **medianasEnFlujo**: dos heaps, uno para cada mitad.

En esta parte podés usar tu `MinHeap` o `std::priority_queue` (es más cómodo
cuando necesitás un max-heap o guardar pares):

```cpp
#include <queue>
#include <functional>   // std::greater

std::priority_queue<int> maxHeap;   // por defecto: max-heap
maxHeap.push(5);
maxHeap.top();                      // el máximo (sin sacarlo)
maxHeap.pop();                      // lo saca

std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
```

> Repasá los apuntes (`Tut-10 - Heaps - apuntes.md`) antes de empezar:
> `siftDown` (comparar con el **menor** de los hijos, y cuidar el caso de un
> solo hijo) y el `buildHeap` en O(n) son los dos puntos donde más se tropieza.
