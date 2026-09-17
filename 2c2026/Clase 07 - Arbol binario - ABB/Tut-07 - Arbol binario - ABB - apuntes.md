Tutorial 7 – Árbol binario - ABB
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 7 – Árbol binario - ABB](#tutorial-7--árbol-binario---abb)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [Árboles binarios](#árboles-binarios)
  - [¿Qué es un árbol binario?](#qué-es-un-árbol-binario)
  - [Representación](#representación)
  - [Recorridos de árboles binarios](#recorridos-de-árboles-binarios)
    - [Preorder](#preorder)
    - [Inorder](#inorder)
    - [Postorder](#postorder)
  - [Altura y tamaño](#altura-y-tamaño)
- [Árbol Binario de Búsqueda (ABB)](#árbol-binario-de-búsqueda-abb)
  - [¿Qué es un ABB?](#qué-es-un-abb)
  - [Invariante del ABB](#invariante-del-abb)
  - [Operaciones básicas](#operaciones-básicas)
    - [Búsqueda](#búsqueda)
    - [Inserción](#inserción)
    - [Mínimo y máximo](#mínimo-y-máximo)
    - [Eliminación](#eliminación)
  - [Complejidad](#complejidad)
  - [El problema del desbalanceo](#el-problema-del-desbalanceo)



# Objetivos de la clase
- Entender qué es un árbol binario y cómo representarlo en C++.
- Implementar los recorridos clásicos: preorder, inorder y postorder.
- Comprender la definición y el invariante del Árbol Binario de Búsqueda (ABB).
- Implementar las operaciones básicas: búsqueda, inserción y eliminación.
- Analizar la complejidad de las operaciones y entender el problema del desbalanceo.



# Árboles binarios

## ¿Qué es un árbol binario?

Un **árbol binario** es una estructura de datos jerárquica donde cada nodo tiene **a lo sumo dos hijos**: un hijo izquierdo y un hijo derecho.

```
        A
       / \
      B   C
     / \   \
    D   E   F
```

Terminología básica:
- **Raíz**: el nodo superior del árbol (A en el ejemplo).
- **Hoja**: un nodo sin hijos (D, E, F en el ejemplo).
- **Padre/Hijo**: relación entre nodos conectados.
- **Altura**: la longitud del camino más largo desde la raíz hasta una hoja.
- **Subárbol**: cualquier nodo junto con todos sus descendientes forma un árbol.

> Un árbol binario es una estructura recursiva: un árbol es o bien vacío, o bien un nodo con dos subárboles (izquierdo y derecho), que a su vez son árboles binarios.

## Representación

La representación más común usa punteros:

```
tipo NodoAB:
    valor
    izq: NodoAB  // puntero al hijo izquierdo
    der: NodoAB  // puntero al hijo derecho
```

Crear un árbol manualmente:

```
raiz ← nuevo NodoAB(10)
raiz.izq ← nuevo NodoAB(5)
raiz.der ← nuevo NodoAB(15)
raiz.izq.izq ← nuevo NodoAB(3)
raiz.izq.der ← nuevo NodoAB(7)
```

> A diferencia de un vector o una lista, el árbol binario no tiene un "orden lineal" natural: hay múltiples formas de recorrerlo, y cada una tiene sus aplicaciones.

## Recorridos de árboles binarios

Los tres recorridos clásicos difieren en **cuándo se visita el nodo actual** respecto de sus hijos:

### Preorder

Visita el nodo **antes** de sus hijos: **raíz → izquierdo → derecho**.

```
procedimiento preorder(nodo):
    si nodo = nulo:
        retornar

    imprimir(nodo.valor)  // visito primero
    preorder(nodo.izq)
    preorder(nodo.der)
```

Útil para: copiar un árbol, serializar su estructura.

### Inorder

Visita el nodo **entre** sus hijos: **izquierdo → raíz → derecho**.

```
procedimiento inorder(nodo):
    si nodo = nulo:
        retornar

    inorder(nodo.izq)
    imprimir(nodo.valor)  // visito en el medio
    inorder(nodo.der)
```

Útil para: en un ABB, recorrer los elementos **en orden creciente**.

### Postorder

Visita el nodo **después** de sus hijos: **izquierdo → derecho → raíz**.

```
procedimiento postorder(nodo):
    si nodo = nulo:
        retornar

    postorder(nodo.izq)
    postorder(nodo.der)
    imprimir(nodo.valor)  // visito al final
```

Útil para: liberar memoria (hay que eliminar los hijos antes que el padre), evaluar expresiones.

> Pregunta para pensar: si tenés el recorrido preorder e inorder de un árbol binario, ¿podés reconstruir el árbol original de forma única? ¿Y con preorder y postorder?

## Altura y tamaño

```
función altura(nodo) → entero:
    si nodo = nulo:
        retornar 0
    retornar 1 + máximo(altura(nodo.izq), altura(nodo.der))

función tamaño(nodo) → entero:
    si nodo = nulo:
        retornar 0
    retornar 1 + tamaño(nodo.izq) + tamaño(nodo.der)
```

> Estas funciones muestran el patrón típico de recursión sobre árboles: caso base (nodo nulo) y caso recursivo (combinar resultados de los subárboles).



# Árbol Binario de Búsqueda (ABB)

## ¿Qué es un ABB?

Un **Árbol Binario de Búsqueda** (ABB, o *BST* por *Binary Search Tree*) es un árbol binario que cumple una propiedad especial que permite buscar elementos de forma eficiente.

## Invariante del ABB

> Para **todo** nodo del árbol:
> - Todos los valores en su subárbol **izquierdo** son **menores** que el valor del nodo.
> - Todos los valores en su subárbol **derecho** son **mayores** que el valor del nodo.

```
        10
       /  \
      5    15
     / \   / \
    3   7 12  20
```

Este árbol cumple el invariante: a la izquierda de 10 están {3, 5, 7} (todos menores), a la derecha están {12, 15, 20} (todos mayores). Y lo mismo se cumple recursivamente en cada nodo.

> El invariante del ABB es lo que permite descartar la mitad del árbol en cada paso de la búsqueda, similar a la búsqueda binaria en un arreglo ordenado.

## Operaciones básicas

### Búsqueda

Para buscar un valor, comparamos con el nodo actual:
- Si es igual, lo encontramos.
- Si es menor, buscamos en el subárbol izquierdo.
- Si es mayor, buscamos en el subárbol derecho.

```
función buscar(nodo, valor) → NodoAB:
    si nodo = nulo:
        retornar nulo  // no está

    si valor = nodo.valor:
        retornar nodo  // encontrado
    sino si valor < nodo.valor:
        retornar buscar(nodo.izq, valor)
    sino:
        retornar buscar(nodo.der, valor)
```

Versión iterativa:

```
función buscarIterativo(nodo, valor) → NodoAB:
    mientras nodo ≠ nulo y nodo.valor ≠ valor:
        si valor < nodo.valor:
            nodo ← nodo.izq
        sino:
            nodo ← nodo.der
    retornar nodo
```

### Inserción

Para insertar, buscamos dónde debería estar el valor y lo agregamos como hoja:

```
función insertar(nodo, valor) → NodoAB:
    si nodo = nulo:
        retornar nuevo NodoAB(valor)

    si valor < nodo.valor:
        nodo.izq ← insertar(nodo.izq, valor)
    sino si valor > nodo.valor:
        nodo.der ← insertar(nodo.der, valor)
    // si valor = nodo.valor, no hacemos nada (no permitimos duplicados)

    retornar nodo
```

> La inserción siempre agrega una nueva hoja; nunca reorganiza el árbol existente. Esto es simple pero puede llevar a desbalanceo.

### Mínimo y máximo

El mínimo está en el nodo más a la izquierda; el máximo en el más a la derecha:

```
función minimo(nodo) → NodoAB:
    mientras nodo.izq ≠ nulo:
        nodo ← nodo.izq
    retornar nodo

función maximo(nodo) → NodoAB:
    mientras nodo.der ≠ nulo:
        nodo ← nodo.der
    retornar nodo
```

### Eliminación

La eliminación es la operación más compleja. Hay tres casos:

1. **El nodo es una hoja**: simplemente lo eliminamos.
2. **El nodo tiene un solo hijo**: lo reemplazamos por su hijo.
3. **El nodo tiene dos hijos**: lo reemplazamos por su **sucesor inorder** (el mínimo del subárbol derecho) o su **predecesor inorder** (el máximo del subárbol izquierdo).

```
función eliminar(nodo, valor) → NodoAB:
    si nodo = nulo:
        retornar nulo

    si valor < nodo.valor:
        nodo.izq ← eliminar(nodo.izq, valor)
    sino si valor > nodo.valor:
        nodo.der ← eliminar(nodo.der, valor)
    sino:
        // Encontramos el nodo a eliminar

        // Caso 1 y 2: tiene 0 o 1 hijo
        si nodo.izq = nulo:
            temp ← nodo.der
            liberar nodo
            retornar temp

        si nodo.der = nulo:
            temp ← nodo.izq
            liberar nodo
            retornar temp

        // Caso 3: tiene dos hijos
        // Buscamos el sucesor inorder (mínimo del subárbol derecho)
        sucesor ← minimo(nodo.der)
        nodo.valor ← sucesor.valor  // copiamos el valor
        nodo.der ← eliminar(nodo.der, sucesor.valor)  // eliminamos el sucesor

    retornar nodo
```

> Pregunta para pensar: ¿por qué elegimos el sucesor inorder y no cualquier otro nodo? ¿Qué pasaría con el invariante del ABB si eligiéramos otro?

## Complejidad

| Operación | Caso promedio | Peor caso |
|-----------|---------------|-----------|
| Búsqueda  | O(log n)      | O(n)      |
| Inserción | O(log n)      | O(n)      |
| Eliminación | O(log n)    | O(n)      |
| Mínimo/Máximo | O(log n)  | O(n)      |

El caso promedio asume un árbol "razonablemente balanceado" (altura ≈ log n). El peor caso ocurre cuando el árbol degenera en una lista.

## El problema del desbalanceo

Si insertamos elementos en orden creciente (1, 2, 3, 4, 5...), el ABB degenera en una lista enlazada:

```
1
 \
  2
   \
    3
     \
      4
       \
        5
```

En este caso, todas las operaciones pasan a ser O(n) en lugar de O(log n).

> Pregunta para pensar: ¿qué orden de inserción produce el árbol más balanceado posible para los elementos {1, 2, 3, 4, 5, 6, 7}?

Para solucionar esto, existen **árboles balanceados** (AVL, rojo-negro, etc.) que mantienen automáticamente la altura en O(log n) después de cada inserción/eliminación. Estos árboles son los que usan internamente estructuras como `std::set` y `std::map` en C++.

> Tip: en la materia vamos a usar el ABB simple para entender los conceptos, pero en código de producción siempre se usan árboles balanceados (o directamente `std::set`/`std::map`).
