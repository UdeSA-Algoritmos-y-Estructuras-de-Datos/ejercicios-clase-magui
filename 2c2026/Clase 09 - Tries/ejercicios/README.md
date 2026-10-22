# Ejercicios - Tut 09 (Tries)

Cáscara para resolver los ejercicios de la novena clase: tries (árboles de
prefijos), recorridos sobre ellos, problemas clásicos que se resuelven con
un trie, y un cierre con rosetrees (árboles n-arios).

## Archivos

- `ejercicios.h` — declaraciones de las funciones a implementar (con la descripción de cada una) y las estructuras `NodoTrie`, `Trie`, `NodoBit` y `Rosetree`. **No hace falta modificarlo.**
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
   porque las funciones están vacías (algunas pruebas "negativas", como
   "esta palabra NO está", pasan de casualidad: no te confíes). Implementá
   cada función en `ejercicios.cpp` y volvé a correr `make run` hasta que
   pasen todas.

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

```cpp
struct NodoTrie {
    std::map<char, NodoTrie*> hijos;  // letra -> hijo (ordenado alfabéticamente)
    bool esFinal;                     // ¿acá termina una palabra?
};

struct Trie {
    NodoTrie* raiz;   // siempre existe; representa el prefijo vacío
    int cantidad;     // palabras distintas guardadas
};
```

El `Trie` ya trae su destructor (libera todos los nodos), así que no hace
falta que te ocupes de liberar el trie. Lo que sí tenés que cuidar:

- En `borrar`, hacer `delete` de los nodos que podás.
- En `maximoXor`, liberar los nodos que crees.

> Como `hijos` es un `std::map`, al recorrerlo las letras salen en orden
> alfabético. Varios ejercicios (listar, autocompletar, palabra más larga)
> aprovechan esto para no tener que ordenar nada.

## Sobre los ejercicios

### Parte 1 — Operaciones básicas

- **insertar**: crea los nodos que falten y marca el último como final.
- **contiene**: ¿está la palabra COMPLETA? (ojo con los prefijos).
- **tienePrefijo**: ¿alguna palabra empieza así?
- **contarConPrefijo**: cuántas palabras empiezan con el prefijo.
- **borrar**: desmarca la palabra y **poda** los nodos que quedaron inútiles.

Todas cuestan O(k), con k el largo de la palabra, sin importar cuántas
palabras haya guardadas. Esa es la gracia del trie.

### Parte 2 — Recorridos

- **contarNodos**: cuánta memoria usa el trie (¡mirá cuánto ahorran los prefijos compartidos!).
- **listarPalabras**: DFS con el camino actual; sale en orden alfabético.
- **autocompletar**: lo que hace un buscador al escribir; con un tope de resultados.
- **prefijoComunMasLargo**: bajar mientras haya un único camino posible.

### Parte 3 — Problemas con tries

- **buscarConComodin**: un patrón con `'.'` que reemplaza cualquier letra (backtracking sobre las ramas).
- **reemplazarRaices**: reemplazar cada palabra de una oración por su raíz más corta.
- **palabraMasLargaConstruible**: la palabra más larga cuyos prefijos están todos en la lista.

### Parte 4 — Tries binarios

- **maximoXor**: el mayor `a ^ b` entre los elementos de un vector, en O(n · 30) en vez de O(n²). Un trie sobre los **bits** de cada número.

### Parte 5 — Rosetree

Cierre con un par de ejercicios fáciles sobre árboles n-arios (el trie es un
caso particular de rosetree):

- **contarHojas**
- **nodosEnNivel**

> Repasá los apuntes (`Tut-09 - Tries - apuntes.md`) antes de empezar:
> la idea de `esFinal` y la poda de `borrar` son los dos puntos donde más
> se tropieza.
