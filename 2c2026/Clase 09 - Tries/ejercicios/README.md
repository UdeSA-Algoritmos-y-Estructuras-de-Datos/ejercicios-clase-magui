# Ejercicios - Tut 09 (Tries)

Cáscara para resolver los ejercicios de la novena clase: tries (árboles de
prefijos) como implementación del TAD Diccionario, recorridos sobre ellos,
problemas clásicos que se resuelven con un trie, y un cierre con rosetrees
(árboles n-arios).

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

Seguimos la teórica (Clase 14 - Tries): el trie implementa el TAD
**Diccionario** (clave → valor) con claves string, y los campos y las
operaciones se llaman como en la teórica.

```cpp
const size_t TAM_ALFABETO = 26;      // Σ = {'a', ..., 'z'}
size_t indice(char c);               // Indice(s): 'a' -> 0, ..., 'z' -> 25
char   simbolo(size_t i);            // la inversa: 0 -> 'a', ...

struct NodoTrie {
    std::array<NodoTrie*, TAM_ALFABETO> hijos;  // hijos[i]: subtrie del i-ésimo símbolo
    bool hayClave;                              // ¿el camino hasta acá es una clave?
    int  valor;                                 // valor asociado (si hayClave)
};

struct Trie {
    NodoTrie* raiz;   // siempre existe; representa el prefijo vacío
    int cantidad;     // claves distintas guardadas
};
```

**Invariante:** todo nodo hoja tiene `hayClave == true`, excepto la raíz (que
puede estar vacía). O sea: no hay "ramas muertas". `eliminar` tiene que
preservarlo.

> Las claves solo pueden tener letras minúsculas (`a-z`). Con otro carácter,
> `indice` se sale del arreglo.

El `Trie` ya trae su destructor (libera todos los nodos), así que no hace
falta que te ocupes de liberar el trie. Lo que sí tenés que cuidar:

- En `eliminar`, hacer `delete` de los nodos que podás.
- En `maximoXor`, liberar los nodos que crees.

> Los hijos están en un arreglo indexado por letra, así que recorrerlos de 0
> a 25 los visita en **orden alfabético**. Varios ejercicios (listar,
> autocompletar, sugerencia más frecuente) aprovechan esto para no tener que
> ordenar nada.

## Sobre los ejercicios

### Parte 1 — TAD Diccionario sobre un trie

- **insertar**: crea los nodos que falten; si la clave ya estaba, reemplaza el valor.
- **nodoDe**: el nodo al que se llega con una clave (o `nullptr`). Es la base de casi todo lo demás.
- **pertenece** / **obtener**: ¿está la clave COMPLETA? / ¿qué valor tiene?
- **esHojaVacia**: ¿el nodo no es clave y no tiene hijos?
- **eliminar**: desmarca la clave y **poda** con una pila de ancestros, como en la teórica.

Todas cuestan O(m), con m el largo de la clave, sin importar cuántas claves
haya guardadas. Esa es la gracia del trie.

### Parte 2 — Prefijos y recorridos

- **tienePrefijo**, **contarConPrefijo**: preguntas sobre prefijos (acá se nota por qué importa el invariante).
- **contarNodos**: cuánta memoria usa el trie (¡mirá cuánto ahorran los prefijos compartidos!).
- **listarClaves**: DFS con el camino actual; sale en orden alfabético.
- **autocompletar**: lo que hace un buscador al escribir; con un tope de resultados.
- **prefijoComunMasLargo**: bajar mientras haya un único camino posible.

### Parte 3 — Problemas con tries

- **buscarConComodin**: un patrón con `'.'` que reemplaza cualquier letra (backtracking sobre las ramas).
- **reemplazarRaices**: reemplazar cada palabra de una oración por su raíz más corta.
- **sugerenciaMasFrecuente**: el trie como diccionario palabra → frecuencia; la sugerencia de mayor valor para un prefijo.

### Parte 4 — Tries binarios

- **maximoXor**: el mayor `a ^ b` entre los elementos de un vector, en O(n · 30) en vez de O(n²). Un trie sobre los **bits** de cada número (acá Σ = {0, 1}).

### Parte 5 — Rosetree

Cierre con un par de ejercicios fáciles sobre árboles n-arios (el trie es un
caso particular de rosetree):

- **contarHojas**
- **nodosEnNivel**

> Repasá los apuntes (`Tut-09 - Tries - apuntes.md`) antes de empezar:
> el invariante y la poda de `eliminar` son los dos puntos donde más se
> tropieza.
