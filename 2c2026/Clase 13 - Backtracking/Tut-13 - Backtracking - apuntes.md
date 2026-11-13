Tutorial 13 – Backtracking
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 13 – Backtracking](#tutorial-13--backtracking)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [Fuerza bruta](#fuerza-bruta)
  - [¿Qué es la fuerza bruta?](#qué-es-la-fuerza-bruta)
  - [Ejemplo: generar todos los subconjuntos](#ejemplo-generar-todos-los-subconjuntos)
  - [Ejemplo: generar todas las permutaciones](#ejemplo-generar-todas-las-permutaciones)
  - [Costo de la fuerza bruta](#costo-de-la-fuerza-bruta)
- [Backtracking](#backtracking)
  - [¿Qué es el backtracking?](#qué-es-el-backtracking)
  - [Plantilla general de backtracking en C++](#plantilla-general-de-backtracking-en-c)
  - [Poda: la clave de la eficiencia](#poda-la-clave-de-la-eficiencia)
  - [Backtracking vs fuerza bruta](#backtracking-vs-fuerza-bruta)
- [Greedy](#greedy)
  - [¿Qué es una estrategia greedy?](#qué-es-una-estrategia-greedy)
  - [¿Cuándo funciona greedy?](#cuándo-funciona-greedy)
  - [Ejemplos ya vistos: Kruskal, Prim y Dijkstra](#ejemplos-ya-vistos-kruskal-prim-y-dijkstra)
  - [¿Cuándo falla greedy?](#cuándo-falla-greedy)
- [Comparación entre las tres estrategias](#comparación-entre-las-tres-estrategias)



# Objetivos de la clase

- Entender la fuerza bruta como estrategia de enumerar y probar **todas** las soluciones candidatas.
- Aprender a diseñar **backtracking** con poda para explorar el espacio de soluciones sin generar todo lo que la fuerza bruta generaría.
- Reconocer cuándo una estrategia **greedy** resuelve un problema de forma óptima, y cuándo no.
- Repasar Kruskal, Prim y Dijkstra como ejemplos ya conocidos de algoritmos greedy.
- Poder elegir, frente a un problema nuevo, cuál de las tres estrategias conviene aplicar.


# Fuerza bruta

## ¿Qué es la fuerza bruta?

La **fuerza bruta** (o *brute force*) es la estrategia algorítmica más simple: consiste en **enumerar todas las soluciones candidatas** posibles y, para cada una, verificar si es válida (y eventualmente quedarnos con la mejor).

No hay ninguna "inteligencia" en la forma de recorrer el espacio de soluciones: se prueban todas, sin descartar ninguna de antemano.

> Fuerza bruta responde siempre la pregunta correcta (si existe una solución, la va a encontrar), pero casi siempre a un **costo exponencial**.

La fuerza bruta nos sirve como **punto de partida**: primero pensamos la solución que prueba todo, y recién después buscamos podar (backtracking) o encontrar una regla local que evite explorar (greedy).

> Pregunta para pensar: si tuviéramos una computadora infinitamente rápida, ¿tendría sentido estudiar otra cosa que no sea fuerza bruta?


## Ejemplo: generar todos los subconjuntos

Dado un `std::vector<int>` de tamaño `n`, queremos generar **todos sus subconjuntos** (el conjunto potencia). Cada elemento puede estar **incluido o no**, así que hay `2^n` subconjuntos posibles.

La idea recursiva es: para el elemento `i`, hay dos ramas: **lo incluyo** o **no lo incluyo**. Avanzamos a `i+1` en ambos casos.

```cpp
#include <iostream>
#include <vector>

void generarSubconjuntos(const std::vector<int>& elementos, int i,
                          std::vector<int>& parcial) {
    if (i == (int)elementos.size()) {
        // Caso base: ya decidimos para todos los elementos
        std::cout << "{ ";
        for (int x : parcial) std::cout << x << " ";
        std::cout << "}\n";
        return;
    }

    // Rama 1: no incluyo elementos[i]
    generarSubconjuntos(elementos, i + 1, parcial);

    // Rama 2: incluyo elementos[i]
    parcial.push_back(elementos[i]);
    generarSubconjuntos(elementos, i + 1, parcial);
    parcial.pop_back(); // deshago la decisión (importante!)
}

int main() {
    std::vector<int> elementos = {1, 2, 3};
    std::vector<int> parcial;
    generarSubconjuntos(elementos, 0, parcial);
    return 0;
}
```

> El `pop_back()` al final es clave: sin él, `parcial` quedaría "contaminado" con decisiones de ramas anteriores. Esta idea de **deshacer una decisión antes de probar la siguiente** es el corazón del backtracking que vemos más abajo.


## Ejemplo: generar todas las permutaciones

Dado un `std::vector<int>` de tamaño `n`, queremos generar **todas sus permutaciones** (`n!` en total). En cada paso elegimos **cuál elemento no usado va en la posición actual**.

```cpp
#include <iostream>
#include <vector>

void generarPermutaciones(std::vector<int>& elementos, int inicio) {
    if (inicio == (int)elementos.size()) {
        for (int x : elementos) std::cout << x << " ";
        std::cout << "\n";
        return;
    }

    for (int i = inicio; i < (int)elementos.size(); i++) {
        std::swap(elementos[inicio], elementos[i]);
        generarPermutaciones(elementos, inicio + 1);
        std::swap(elementos[inicio], elementos[i]); // deshago el swap
    }
}

int main() {
    std::vector<int> elementos = {1, 2, 3};
    generarPermutaciones(elementos, 0);
    return 0;
}
```

> Pregunta para pensar: ¿por qué hace falta deshacer el `swap` después de la llamada recursiva? ¿Qué pasaría si no lo hiciéramos?


## Costo de la fuerza bruta

| Problema                          | Cantidad de soluciones candidatas | Costo típico     |
| ---------------------------------- | ---------------------------------- | ----------------- |
| Subconjuntos de un conjunto de `n` | `2^n`                               | O(2^n)             |
| Permutaciones de `n` elementos     | `n!`                                | O(n!)              |
| Asignar `k` valores a `n` variables | `k^n`                               | O(k^n)             |

> El costo de la fuerza bruta crece **exponencialmente** (o peor) con el tamaño de la entrada. Para `n = 30`, `2^n` ya son más de mil millones de subconjuntos: en la práctica es intratable.

Por eso, en la mayoría de los casos usamos fuerza bruta solo cuando `n` es muy chico, o como punto de partida para después optimizar con backtracking (podando ramas que sabemos que no pueden llevar a una solución) o con greedy (si el problema lo permite).


# Backtracking

## ¿Qué es el backtracking?

El **backtracking** es una forma organizada de hacer fuerza bruta: en vez de generar todas las soluciones candidatas "a ciegas", vamos **construyendo una solución parcial paso a paso**, y en cada paso chequeamos si la decisión tomada **todavía puede llevar a una solución válida**.

Si en algún punto la solución parcial **no puede completarse** de forma válida, **abandonamos esa rama** (hacemos *backtrack*, "volvemos atrás") sin seguir explorándola. Esto se llama **poda**.

> Backtracking sigue siendo, en el peor caso, exponencial. La diferencia con la fuerza bruta pura es que en la práctica **poda muchísimas ramas** y termina explorando un espacio mucho más chico.


## Plantilla general de backtracking en C++

Casi todos los problemas de backtracking se pueden escribir con la misma estructura:

```cpp
void backtrack(EstadoParcial& solucionParcial, /* otros parámetros */) {
    if (esSolucionCompleta(solucionParcial)) {
        procesarSolucion(solucionParcial); // guardarla, imprimirla, contarla, etc.
        return;
    }

    for (Decision d : decisionesPosibles(solucionParcial)) {
        if (esValida(d, solucionParcial)) {   // PODA: si no es válida, ni la pruebo
            aplicar(d, solucionParcial);       // tomo la decisión
            backtrack(solucionParcial, /* ... */);
            deshacer(d, solucionParcial);      // la deshago (backtrack)
        }
    }
}
```

Los cuatro bloques que siempre hay que definir para un problema nuevo son:

1. **¿Qué es una solución completa?** (caso base de la recursión)
2. **¿Cuáles son las decisiones posibles** en cada paso?
3. **¿Cómo podo?** (`esValida`: qué condición descarta una decisión sin explorarla)
4. **¿Cómo deshago una decisión** para poder probar la siguiente?

> Pregunta para pensar: en el ejemplo de subconjuntos de más arriba, ¿dónde estaría la poda si quisiéramos generar solo los subconjuntos cuya suma no supere un `target`?


## Poda: la clave de la eficiencia

La poda es lo que separa al backtracking de la fuerza bruta pura. Sin poda (`esValida` siempre `true`), el backtracking degenera exactamente en fuerza bruta: se generan todas las soluciones candidatas sin excepción.

Con una buena poda, se evita construir ramas enteras del árbol de decisiones que sabemos, de antemano, que no van a poder completarse. Cuanto **antes** se detecta que una rama es inválida, más trabajo nos ahorramos (porque se corta un subárbol completo).

> Tip: la poda no cambia el peor caso teórico del problema (sigue siendo exponencial en general), pero en la práctica puede ser la diferencia entre un programa que corre en milisegundos y uno que no termina nunca.


## Backtracking vs fuerza bruta

| Aspecto                         | Fuerza bruta                  | Backtracking                          |
| -------------------------------- | ------------------------------ | --------------------------------------- |
| ¿Genera todas las soluciones?   | Sí, siempre                    | Solo las que pueden ser válidas         |
| ¿Usa poda?                       | No                              | Sí                                       |
| Costo peor caso                  | Exponencial                    | Exponencial (pero con mejor constante)  |
| ¿Garantiza la solución óptima?  | Sí (si se comparan todas)       | Sí (si se recorre todo el espacio podado) |
| Ejemplos típicos                | Generar subconjuntos/permutaciones | N-reinas, Sudoku, coloreo de grafos    |


# Greedy

## ¿Qué es una estrategia greedy?

Un algoritmo **greedy** (voraz) construye la solución tomando, en cada paso, la **decisión que parece mejor en ese momento** (localmente óptima), **sin volver nunca atrás** a reconsiderarla.

A diferencia de backtracking, greedy **nunca deshace una decisión**: una vez tomada, queda fija para siempre. Esto lo hace mucho más rápido (generalmente polinomial), pero **no siempre da la solución óptima**.

> Greedy es "ambicioso": agarra lo mejor que ve ahora, apostando a que esa serie de decisiones locales termina siendo, en conjunto, una solución global óptima. En algunos problemas esa apuesta es correcta; en otros, no.


## ¿Cuándo funciona greedy?

Greedy da la solución óptima cuando el problema tiene una propiedad llamada **subestructura greedy** (o *matroid* en casos más formales): la mejor decisión local, combinada con la solución óptima del resto del problema, da la solución óptima global.

En la práctica, para confiar en un greedy hay que **demostrar** (o al menos convencerse con un argumento de intercambio) que la elección local nunca perjudica el resultado final. No alcanza con que "funcione en los ejemplos que probé".

> Pregunta para pensar: ¿por qué no alcanza con probar greedy en un par de casos y ver que da bien, para confiar en que siempre funciona?


## Ejemplos ya vistos: Kruskal, Prim y Dijkstra

Ya conocemos tres algoritmos greedy de la clase de grafos:

| Algoritmo | Decisión greedy en cada paso                                   | Qué construye                     |
| --------- | ---------------------------------------------------------------- | ----------------------------------- |
| Kruskal   | Elegir la arista de **menor peso** que no forme ciclo             | Árbol de tendido mínimo (MST)      |
| Prim      | Elegir la arista de **menor peso** que conecta el árbol con un nodo nuevo | Árbol de tendido mínimo (MST)      |
| Dijkstra  | Elegir el nodo **no visitado con menor distancia acumulada**       | Caminos mínimos desde un origen    |

En los tres casos, la decisión local (agarrar el mínimo disponible) resulta ser también parte de la solución óptima global. Eso **no es casualidad**: se puede demostrar formalmente (por ejemplo, con el "cut property" para Kruskal/Prim).

> Estos tres algoritmos son la prueba de que greedy, cuando el problema tiene la estructura correcta, es tanto **correcto** como **eficiente**.


## ¿Cuándo falla greedy?

Greedy no siempre funciona. Un ejemplo clásico es el **problema del cambio de monedas**: dado un sistema de monedas y un monto, dar el cambio usando la menor cantidad de monedas.

Con el sistema de monedas usual (`{1, 5, 10, 25}`), la estrategia greedy de "usar siempre la moneda más grande posible" **sí** da el óptimo. Pero con un sistema de monedas arbitrario, greedy puede fallar:

```cpp
// Sistema de monedas {1, 3, 4}, target = 6
// Greedy: 4 + 1 + 1 = 3 monedas
// Óptimo: 3 + 3 = 2 monedas
```

Otro ejemplo clásico donde greedy falla es la **mochila 0/1** (elegir objetos enteros, sin poder fraccionarlos, para maximizar valor sin superar una capacidad): elegir siempre el objeto de mayor valor, o el de mejor relación valor/peso, no garantiza el óptimo. Ahí hace falta programación dinámica (la vemos en el próximo tutorial) o backtracking.

> Pregunta para pensar: ¿por qué la mochila **fraccional** sí se puede resolver con greedy, pero la mochila **0/1** no?


# Comparación entre las tres estrategias

| Aspecto                     | Fuerza bruta          | Backtracking                     | Greedy                          |
| ---------------------------- | ---------------------- | ----------------------------------- | ---------------------------------- |
| ¿Vuelve atrás?               | No aplica (prueba todo) | Sí, ante cualquier decisión inválida | Nunca                              |
| ¿Garantiza solución óptima?  | Sí                      | Sí                                   | Solo si el problema lo permite     |
| Costo típico                | Exponencial             | Exponencial (con poda)              | Polinomial                         |
| Cuándo usarlo                | `n` muy chico, prototipo | El espacio de soluciones se puede podar mucho | El problema tiene subestructura greedy demostrable |
| Ejemplos                    | Subconjuntos, permutaciones | N-reinas, Sudoku, coloreo de grafos | Kruskal, Prim, Dijkstra, cambio de monedas (sistema estándar) |

> En resumen: **fuerza bruta** prueba todo, **backtracking** prueba todo lo que "vale la pena" probar (podando lo demás), y **greedy** ni siquiera prueba: decide y sigue, apostando a que la suma de decisiones locales da el óptimo global. Elegir cuál usar depende de si podemos demostrar (o no) que la decisión greedy siempre es segura.

---

# Sobre los ejercicios

Los ejercicios de esta clase (en la carpeta `ejercicios`) siguen el orden de la teórica, con el mismo pseudocódigo del handout.

- **Parte 1 — Fuerza bruta y enumeraciones**: averiguar un PIN probando todos los candidatos, y generar **variaciones**, **permutaciones**, **combinaciones** y **subconjuntos**.
- **Parte 2 — Backtracking**: el **laberinto** (con el hilo), el **sudoku** (con su poda) y la **suma de subconjuntos**.
- **Parte 3 — Greedy**: la **selección de actividades** (un greedy que funciona) y el **vuelto** (un greedy que falla, y el backtracking que sí llega al óptimo).
- **Parte 4 — Opcionales**: cuántas aulas hacen falta para todas las actividades, y el laberinto sin deshacer.
