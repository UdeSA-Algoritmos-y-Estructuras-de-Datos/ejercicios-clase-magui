# Ejercicios - Tut 07 (Árbol binario - ABB)

Cáscara para resolver los ejercicios de la séptima clase: árboles binarios,
recorridos y operaciones sobre Árboles Binarios de Búsqueda (ABB).

## Archivos

- `ejercicios.h` — declaraciones de las funciones a implementar (con la descripción de cada una). **No hace falta modificarlo.**
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

3. Vas a ver una lista de `[PASA]` / `[FALLA]`. Al principio falla casi todo,
   porque las funciones están vacías. Implementá cada función en
   `ejercicios.cpp` y volvé a correr `make run` hasta que pasen todas.

4. **Opcional**: corré Valgrind para confirmar que no hay memory leaks ni
   accesos inválidos a memoria (importante en esta clase porque usamos
   memoria dinámica):

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

## Sobre los ejercicios

Los ejercicios trabajan sobre árboles binarios representados con punteros.
El tipo `NodoAB<T>` ya está definido en `ejercicios.h`.

### Recorridos de árboles binarios

- **preorder**: recorre el árbol visitando raíz → izquierdo → derecho.
- **inorder**: recorre el árbol visitando izquierdo → raíz → derecho.
- **postorder**: recorre el árbol visitando izquierdo → derecho → raíz.

### Propiedades del árbol

- **altura**: devuelve la altura del árbol (0 si es vacío).
- **tamanio**: devuelve la cantidad de nodos del árbol.

### Operaciones de ABB

- **buscar**: busca un valor en el ABB y devuelve el nodo que lo contiene.
- **insertar**: inserta un valor en el ABB manteniendo el invariante.
- **minimo / maximo**: devuelve el nodo con el valor mínimo/máximo del ABB.
- **eliminar**: elimina un valor del ABB manteniendo el invariante.

### Ejercicios propuestos

- **esABB**: verifica si un árbol binario cumple el invariante de ABB.
- **kEsimoInorder**: devuelve el k-ésimo elemento en el recorrido inorder.
- **ancestroComun**: encuentra el ancestro común más bajo de dos valores.
- **liberarArbol**: libera toda la memoria del árbol (importante para evitar memory leaks).

> Repasá el pseudocódigo en los apuntes (`Tut-07 - Arbol binario - ABB - apuntes.md`)
> para tener clara la estructura de cada algoritmo antes de implementarlo en C++.
