Tutorial 6 – Divide & Conquer - Mergesort
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 6 – Divide & Conquer - Mergesort](#tutorial-6--divide--conquer---mergesort)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [Divide y Conquer](#divide-y-conquer)
  - [La idea general](#la-idea-general)
  - [Merge sort](#merge-sort)
    - [La función merge](#la-función-merge)
  - [Quick sort](#quick-sort)
    - [Partición (partition)](#partición-partition)
  - [El teorema maestro](#el-teorema-maestro)
    - [Enunciado general](#enunciado-general)
    - [Los tres casos](#los-tres-casos)
    - [Ejemplos aplicados](#ejemplos-aplicados)



# Objetivos de la clase
- Incorporar la técnica de **Divide & Conquer** (dividir, resolver recursivamente y combinar).
- Implementar y analizar **merge sort** y **quick sort**.
- Aprender a usar el **teorema maestro** para resolver recurrencias de la forma `T(n) = a·T(n/b) + f(n)`.



# Divide y Conquer

## La idea general

**Divide & Conquer** ("dividir para conquistar") es una técnica de diseño de algoritmos que consiste en tres pasos:

1. **Dividir** el problema en subproblemas más chicos (típicamente, del mismo tipo que el original).
2. **Conquistar** (resolver) cada subproblema recursivamente. El **caso base** de la recursión es un subproblema lo suficientemente chico como para resolverse directamente.
3. **Combinar** las soluciones de los subproblemas para construir la solución del problema original.

Esta estructura se refleja directamente en una **recurrencia**:

```
T(n) = a·T(n/b) + f(n)
```

donde:
- `a` = cantidad de subproblemas en los que se divide el problema.
- `n/b` = tamaño de cada subproblema (se divide el tamaño original por `b`).
- `f(n)` = costo de dividir el problema y combinar las soluciones (sin contar las llamadas recursivas).

> Esta misma recurrencia la vamos a usar más abajo con el **teorema maestro** para calcular la complejidad de merge sort y de la búsqueda binaria.

## Merge sort

Merge sort aplica Divide & Conquer directamente:

1. **Dividir**: partir el vector en dos mitades.
2. **Conquistar**: ordenar recursivamente cada mitad.
3. **Combinar**: mezclar (`merge`) las dos mitades ya ordenadas en un único vector ordenado.

```
función mergeSort(v, izq, der):
    si izq >= der:
        devolver                       // caso base: 0 o 1 elemento

    medio ← izq + (der - izq) / 2

    mergeSort(v, izq, medio)           // ordeno mitad izquierda
    mergeSort(v, medio + 1, der)       // ordeno mitad derecha
    merge(v, izq, medio, der)          // combino ambas mitades
```

### La función merge

`merge` recibe dos sub-rangos **ya ordenados** (`[izq, medio]` y `[medio+1, der]`) y los combina en un único rango ordenado, usando un vector auxiliar.

```
función merge(v, izq, medio, der):
    aux ← arreglo vacío

    i ← izq             // puntero a la mitad izquierda
    j ← medio + 1        // puntero a la mitad derecha

    mientras i <= medio y j <= der:
        si v[i] <= v[j]:
            agregar v[i] al final de aux
            i ← i + 1
        si no:
            agregar v[j] al final de aux
            j ← j + 1

    mientras i <= medio:
        agregar v[i] al final de aux
        i ← i + 1

    mientras j <= der:
        agregar v[j] al final de aux
        j ← j + 1

    para k desde 0 hasta longitud(aux) - 1:
        v[izq + k] ← aux[k]
```

> El uso de `<=` (y no `<`) en la comparación `v[i] <= v[j]` es lo que hace que merge sort sea **estable**: ante un empate, siempre se prioriza el elemento de la mitad izquierda, que apareció antes en el vector original.

`merge` cuesta O(n) porque recorre una sola vez ambos sub-rangos. Por lo tanto la recurrencia de merge sort es:

```
T(n) = 2·T(n/2) + O(n)
```

## Quick sort

Quick sort también es Divide & Conquer, pero divide el trabajo de otra manera:

1. **Dividir**: elegir un **pivot** y particionar el vector de forma que todos los elementos menores queden a su izquierda y todos los mayores a su derecha.
2. **Conquistar**: ordenar recursivamente la parte izquierda y la parte derecha del pivot.
3. **Combinar**: ¡no hace falta combinar nada! Una vez ordenadas ambas partes, el vector completo ya está ordenado.

```
función quickSort(v, izq, der):
    si izq >= der:
        devolver                       // caso base: 0 o 1 elemento

    p ← partition(v, izq, der)

    quickSort(v, izq, p - 1)
    quickSort(v, p + 1, der)
```

### Partición (partition)

Usamos el esquema de **Lomuto**, que elige como pivot el último elemento del rango:

```
función partition(v, izq, der):
    pivot ← v[der]
    i ← izq - 1               // límite de la zona "menor al pivot"

    para j desde izq hasta der - 1:
        si v[j] < pivot:
            i ← i + 1
            intercambiar v[i] y v[j]

    intercambiar v[i + 1] y v[der]
    devolver i + 1             // posición final del pivot
```

> Pregunta para pensar: ¿por qué quick sort **no es estable** en general? Pensá qué pasa con dos elementos iguales cuando uno de ellos termina siendo swapeado con el pivot.

La complejidad de quick sort depende de qué tan "parejo" sea el particionamiento:

| Caso              | Partición típica            | Recurrencia               | Complejidad |
| ------------------ | ---------------------------- | -------------------------- | ------------ |
| Mejor/promedio     | Las dos partes son de tamaño similar | T(n) = 2·T(n/2) + O(n)  | O(n log n)   |
| Peor caso           | Una parte queda vacía (pivot siempre mínimo o máximo, ej. vector ya ordenado) | T(n) = T(n-1) + O(n) | O(n²)        |

> Elegir bien el pivot (por ejemplo, con la mediana de tres elementos, o un pivot aleatorio) es clave para evitar el peor caso en la práctica.

## El teorema maestro

El teorema maestro es una herramienta que nos permite resolver recurrencias de Divide & Conquer sin tener que desarrollar el árbol de recursión a mano cada vez.

### Enunciado general

Dada una recurrencia de la forma:

```
T(n) = a·T(n/b) + f(n)      con a ≥ 1, b > 1
```

se compara `f(n)` contra `n^(log_b a)` (el costo "puro" de las llamadas recursivas, sin combinar).

### Los tres casos

| Caso | Condición                                                          | Resultado                          |
| ---- | ------------------------------------------------------------------- | ------------------------------------ |
| 1    | `f(n) = O(n^(log_b a - ε))` para algún ε > 0 (f(n) crece **más lento**) | `T(n) = Θ(n^(log_b a))`             |
| 2    | `f(n) = Θ(n^(log_b a))` (f(n) crece **igual**)                       | `T(n) = Θ(n^(log_b a) · log n)`     |
| 3    | `f(n) = Ω(n^(log_b a + ε))` para algún ε > 0 **y** se cumple la condición de regularidad (f(n) crece **más rápido**) | `T(n) = Θ(f(n))`                    |

> Intuición: el teorema maestro compara "cuánto trabajo se hace repartiendo/combinando en cada nivel" (`f(n)`) contra "cuántas hojas tiene el árbol de recursión" (`n^(log_b a)`). Gana (domina la complejidad) el que sea asintóticamente mayor; si empatan, se agrega un factor `log n`.

### Ejemplos aplicados

**Merge sort:** `T(n) = 2·T(n/2) + O(n)` → `a = 2`, `b = 2`, `f(n) = n`.
- `n^(log_b a) = n^(log_2 2) = n^1 = n`.
- `f(n) = n = Θ(n^1)` → **Caso 2**.
- `T(n) = Θ(n log n)`.

**Búsqueda binaria:** `T(n) = T(n/2) + O(1)` → `a = 1`, `b = 2`, `f(n) = 1`.
- `n^(log_b a) = n^(log_2 1) = n^0 = 1`.
- `f(n) = 1 = Θ(n^0)` → **Caso 2**.
- `T(n) = Θ(log n)`.

**Un ejemplo de Caso 1:** `T(n) = 4·T(n/2) + n` → `a = 4`, `b = 2`, `f(n) = n`.
- `n^(log_b a) = n^(log_2 4) = n²`.
- `f(n) = n` crece más lento que `n²` → **Caso 1**.
- `T(n) = Θ(n²)`.

**Un ejemplo de Caso 3:** `T(n) = 2·T(n/2) + n²` → `a = 2`, `b = 2`, `f(n) = n²`.
- `n^(log_b a) = n`.
- `f(n) = n²` crece más rápido que `n` → **Caso 3**.
- `T(n) = Θ(n²)`.

> Pregunta para pensar: `T(n) = 2·T(n/2) + n log n` no encaja exactamente en ninguno de los tres casos clásicos (es un "caso límite" del Caso 2 extendido). ¿Qué resultado te parece razonable? *Ayuda: pensá el árbol de recursión nivel por nivel.*

> No todas las recurrencias son "del tipo Divide & Conquer". Por ejemplo, `T(n) = T(n-1) + n` (recursión que reduce de a 1 el tamaño) **no** se puede resolver con el teorema maestro porque no tiene la forma `a·T(n/b) + f(n)`. Hay que resolverla por sustitución o por el método del árbol de recursión: da `T(n) = O(n²)`.
