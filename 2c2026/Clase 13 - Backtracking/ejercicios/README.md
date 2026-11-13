# Ejercicios - Tut 13 (Fuerza bruta, backtracking y greedy)

Cáscara para resolver los ejercicios de la decimotercera clase. Siguen el
orden de la teórica (clase 21), con el mismo pseudocódigo del handout: primero
**fuerza bruta** y las enumeraciones combinatorias, después **backtracking**
(laberinto, sudoku, suma de subconjuntos) y al final **greedy**.

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
   "**no** hay solución", pasan de casualidad: no te confíes). Implementá cada
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

> Si el programa **se queda colgado**, casi seguro alguna recursión tuya no
> avanza (por ejemplo, un llamado recursivo con los mismos parámetros, o un
> backtracking que no deshace bien). Cortalo con `Ctrl+C`.

## Sobre los ejercicios

El pseudocódigo de cada algoritmo está copiado en `ejercicios.h`, arriba del
ejercicio que lo implementa.

### Parte 1 — Fuerza bruta y enumeraciones (Ej. 1 al 5)

*Generar todos los candidatos y verificar cada uno.*

- **encontrarPIN** (Ej. 1): el ejemplo de la teórica. Se conoce `H(K)` y hay que averiguar el PIN `K` probando todos. La función de hash `hashPIN` ya viene hecha.
- **variaciones**, **permutaciones**, **combinaciones** (Ej. 2 a 4): las tres enumeraciones de la teórica. La idea común: un arreglo `actual` con las primeras `i` posiciones ya decididas, y un llamado recursivo por cada opción para la posición `i`.
- **subconjuntos** (Ej. 5): para cada elemento, usarlo o no. Son las variaciones de `[sí, no]`.

En la teórica cada candidato se **imprime**. Acá, en vez de imprimirlo, se lo
agrega al final de `res`: donde el pseudocódigo dice `imprimir(actual)`, va
`res.push_back(actual)`. Las pruebas hacen el primer llamado (con `i = 0`) y
miran que los candidatos estén **en el mismo orden** que en la teórica.

### Parte 2 — Backtracking (Ej. 6 al 10)

*No esperar a tener un candidato completo: si uno parcial ya no puede llevar a
una solución, se abandona ahí mismo. Elegir, llamar, deshacer.*

- **caeAdentro** y **resolverLaberinto** (Ej. 6 y 7): el laberinto de la teórica. El hilo es la respuesta (el camino) y también la poda. Tiene que reproducir el seguimiento del handout: prueba las direcciones en el orden oeste, sur, este, norte.
- **esValido** y **resolverSudoku** (Ej. 8 y 9): la poda del sudoku y el backtracking. Es la actividad de la teórica: ¿cuáles son las decisiones posibles en cada estado? ¿Cuál es la poda? ¿Qué hay que deshacer? Funciona con el de `9 x 9` y con el ejemplo chico de `4 x 4`.
- **haySubconjunto** (Ej. 10): suma de subconjuntos. Acá no hay nada que deshacer: el estado viaja en los parámetros.

El laberinto ya trae hechas las operaciones del pseudocódigo:

```cpp
enum Celda { VACIO, PARED, HILO, VISITADA };
typedef std::vector<std::vector<Celda>> Laberinto;
struct Posicion { int fila; int col; };

Posicion fin = salida(L);                     // Salida(L)
Posicion prox = mover(pos, DIRECCIONES[d]);   // prox = pos + dir (d de 0 a 3)
contenido(L, prox) == VACIO                   // Contenido(L, prox)
ubicar(L, prox, HILO);                        // Ubicar(L, prox, hilo)
```

Un `Sudoku` es un `std::vector<std::vector<int>>` donde `VACIA` (un `0`) marca
las celdas sin dígito. `ladoDeCuadrante(S)` devuelve `3` para el de `9 x 9` y
`2` para el de `4 x 4`.

### Parte 3 — Greedy (Ej. 11 al 13)

*Bajar por una sola rama del árbol de decisiones: elegir lo que parece mejor
en cada paso, sin reconsiderar.*

- **seleccionarActividades** (Ej. 11): el greedy que funciona. Elegir siempre la actividad que termina antes. El ordenamiento (`std::sort`) ya viene escrito en la cáscara. En el ejemplo de la teórica quedan `c, e, b`.
- **vueltoGreedy** (Ej. 12): el greedy que falla. Con monedas `1, 3, 4` da `6` con tres monedas (`4 + 1 + 1`), y el óptimo es `3 + 3`.
- **vueltoMinimo** (Ej. 13): el mismo problema con backtracking: probar cada moneda para lo que falta. Siempre llega al óptimo, pero en tiempo exponencial.

### Parte 4 — Opcionales (Ej. 14 y 15)

Para después de terminar las tres partes. Sus pruebas se cuentan aparte. Son
las dos preguntas que la teórica deja picando.

- **cantidadDeAulas** (Ej. 14): hacer **todas** las actividades con la menor cantidad de aulas. Repetir el greedy del Ej. 11 no sirve; ordenar por inicio, sí.
- **resolverLaberintoSinDeshacer** (Ej. 15): ¿y si al volver marcamos la celda como visitada en lugar de vacía? ¿Sigue siendo correcto? ¿Cuánto cuesta?

### Para pensar

- `resolverLaberinto`, ¿encuentra el camino más corto? ¿Qué algoritmo de grafos lo haría?
- En el sudoku de `4 x 4` de la teórica, sin poda serían `4^6 = 4096` candidatos. ¿Cuántos dígitos prueba tu `resolverSudoku`?
- En `haySubconjunto`, ¿por qué la poda `k < 0` sólo vale si los elementos son no negativos?
- ¿Para qué valores de `v` empieza a tardar `vueltoMinimo`? ¿Qué subproblemas resuelve una y otra vez?

> Ojo con los tipos: los índices son `size_t`, pero las posiciones del
> laberinto y los dígitos del sudoku son `int`. Con `-Wconversion` el
> compilador no te deja mezclarlos sin un `static_cast`.
