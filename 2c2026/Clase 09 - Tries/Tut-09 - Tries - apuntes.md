Tutorial 9 – Tries
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 9 – Tries](#tutorial-9--tries)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [¿Dónde estamos?](#dónde-estamos)
- [¿Qué es un trie?](#qué-es-un-trie)
  - [Ejemplo](#ejemplo)
  - [El invariante del trie](#el-invariante-del-trie)
- [Cómo se representa](#cómo-se-representa)
  - [¿Cómo guardar los hijos?](#cómo-guardar-los-hijos)
- [Las operaciones del diccionario](#las-operaciones-del-diccionario)
  - [Inserción](#inserción)
  - [Búsqueda](#búsqueda)
  - [Eliminación](#eliminación)
- [Prefijos y recorridos](#prefijos-y-recorridos)
- [Complejidad y espacio](#complejidad-y-espacio)
  - [El peor caso de espacio](#el-peor-caso-de-espacio)
  - [Comprimiendo el espacio: árboles PATRICIA](#comprimiendo-el-espacio-árboles-patricia)
- [Comparación de representaciones](#comparación-de-representaciones)
- [Dónde aparecen los tries](#dónde-aparecen-los-tries)
- [Más allá de los strings: tries binarios](#más-allá-de-los-strings-tries-binarios)
- [Repaso relámpago](#repaso-relámpago)
- [Anexo: Rosetree (árbol n-ario)](#anexo-rosetree-árbol-n-ario)
  - [¿Qué es un rosetree?](#qué-es-un-rosetree)
  - [Representación del rosetree](#representación-del-rosetree)
  - [Recorridos y recursión](#recorridos-y-recursión)
  - [Ejemplos donde aparece naturalmente](#ejemplos-donde-aparece-naturalmente)
  - [Relación con el trie](#relación-con-el-trie)



# Objetivos de la clase
- Entender qué es un trie y por qué permite que `Pertenece` y `Obtener` cuesten O(m) en **peor caso**, sin depender de cuántas claves haya guardadas.
- Conocer cómo se representa un trie y qué hace cada operación del TAD Diccionario sobre él, incluyendo el invariante que hay que preservar al eliminar.
- Entender cómo se buscan claves por prefijo y cómo se recorre un trie.
- Reconocer los problemas donde un trie es la herramienta natural.
- Comparar el trie con el AVL y la tabla de hash, incluyendo su costo en espacio.
- Reconocer al trie como un caso particular de rosetree (árbol n-ario).



# ¿Dónde estamos?

Ya representamos el TAD Diccionario de dos maneras. Con claves string de longitud `m` y `n` la cantidad de claves guardadas:

| | `Pertenece` / `Obtener` | ¿Recorre en orden? |
|---|---|---|
| AVL | O(m log n) peor caso | Sí |
| Tabla de hash | O(m) promedio, O(n · m) peor caso | No |

En las dos, el peor caso **depende de cuántas claves hay guardadas**.

> Pregunta para pensar: leer una clave ya cuesta O(m). ¿Se puede lograr que `Pertenece` y `Obtener` cuesten O(m) en **peor caso**, sin depender de `n`?

Un caso donde esto importa mucho es la **búsqueda por prefijo** (el teclado del celular sugiriendo palabras, el buscador completando tu consulta): dado un prefijo, encontrar todas las claves que empiezan con él. Una tabla de hash no sirve para esto (el hash de `"cas"` no se parece en nada al de `"casa"`, así que habría que revisar todas las claves), y el AVL tiene que comparar strings enteros en cada paso. El trie resuelve el problema guardando las claves **símbolo por símbolo, compartiendo los prefijos**.



# ¿Qué es un trie?

Sea Σ un **alfabeto**: un conjunto finito de símbolos (por ejemplo, las letras de `a` a `z`). Escribimos `|Σ|` para la cantidad de símbolos.

> **Trie**: un árbol en el que cada nodo tiene `|Σ|` hijos, uno por cada símbolo de Σ.
> - El camino desde la raíz hasta un nodo corresponde a una **clave** (concatenando los símbolos de las aristas recorridas).
> - El valor guardado en un nodo (si tiene) es el valor asociado a esa clave.

Cada hijo de un trie es a su vez la raíz de un trie más chico (un **subtrie**).

La palabra viene de *re**trie**val* (recuperación) y se pronuncia como "try". El nodo **no guarda su propio símbolo**: queda determinado por el lugar que ocupa dentro de su padre (el hijo número 0 corresponde a la `a`, el 1 a la `b`, etcétera).

## Ejemplo

Un trie con las claves `ala → 1`, `casa → 5`, `caso → 9` y `cosa → 2`. Los nodos con `[valor]` son claves; los otros son solo prefijos (todavía no son una clave completa):

```
(raíz)
 ├── a
 │    └── l
 │         └── a   [1]     "ala"  → 1
 └── c
      ├── a
      │    └── s
      │         ├── a   [5]     "casa" → 5
      │         └── o   [9]     "caso" → 9
      └── o
           └── s
                └── a   [2]     "cosa" → 2
```

Fijate que existe el camino `a → l`, pero **`"al"` no es una clave guardada**: es solo un prefijo de `"ala"`. Por eso cada nodo necesita registrar si **el camino hasta él es una clave completa** y, en ese caso, cuál es su valor.

Esa marca también permite guardar claves que son prefijo de otras: si el trie tiene `"ca"` y `"casa"`, el nodo de la `'a'` es una clave **y** además tiene un hijo.

> Esta es la fuente de error más común: olvidarse de esa marca y responder que cualquier prefijo es una clave. Si el trie tiene `"casa"`, la pregunta "¿pertenece `"cas"`?" tiene que dar que no.

## El invariante del trie

La definición de arriba también permitiría un trie que **desperdicia espacio**: con nodos que son hojas, no tienen hijos y tampoco tienen valor. Mirando solo uno de esos nodos no se puede saber si es una clave con un valor "vacío" o si esa clave directamente no está. Para evitar la ambigüedad, todo trie debe cumplir:

> **Invariante de un trie**: todo nodo hoja (sin hijos) tiene un valor asociado (es una clave), **excepto la raíz**, que puede no tener hijos ni valor (el trie vacío).

Dicho de otra forma: **no puede haber "ramas muertas"**, caminos que no llevan a ninguna clave. Esto tiene una consecuencia muy útil: si el camino de un prefijo existe, seguro hay alguna clave que empieza con él. El que tiene que trabajar para sostener el invariante es el **borrado**.



# Cómo se representa

Cada nodo del trie guarda tres cosas (las mismas que en la teórica):

| Campo | Qué es |
|---|---|
| `hayClave` | Un booleano: ¿el camino hasta este nodo es una clave completa? |
| `valor` | El valor asociado a esa clave (solo tiene sentido si `hayClave` es verdadero). |
| `hijos` | Un arreglo de `|Σ|` punteros, uno por símbolo. La posición `i` apunta al subtrie al que se baja si el próximo símbolo es el `i`-ésimo de Σ, y vale `nil` (puntero nulo) si no hay ninguna clave por ese lado. |

El trie vacío no es un puntero nulo: es **un nodo real, sin clave y con todos los hijos nulos**. Así la raíz siempre existe, y "diccionario vacío" cuesta O(|Σ|) = O(1) (el alfabeto es de tamaño constante).

Para pasar de un símbolo a su posición en el arreglo se usa una función `Indice(s)`, que debe costar O(1). Con Σ = {`a`, ..., `z`}, la posición de `a` es 0, la de `b` es 1, y así hasta la `z`, que es 25 (alcanza con restarle la `a`).

> Las claves solo pueden usar símbolos de Σ. Con un símbolo que no esté en el alfabeto (una mayúscula, por ejemplo), `Indice` daría una posición fuera del arreglo. Siempre hay que respetar esa precondición.

Como el trie reserva memoria dinámica para sus nodos, también tiene que liberarla al destruirse: se libera cada subtrie antes que su padre, igual que con los árboles binarios.

## ¿Cómo guardar los hijos?

El arreglo de `|Σ|` punteros es lo que muestra la teórica, pero no es la única opción:

| Representación | Buscar un hijo | Memoria por nodo | Recorrido en orden | Comentario |
|---|---|---|---|---|
| Arreglo de `\|Σ\|` punteros | O(1) **en peor caso** | `\|Σ\|` punteros siempre | Sí (recorriendo las posiciones de 0 a `\|Σ\|-1`) | La de la teórica. La que usamos. |
| `std::vector` de tamaño `\|Σ\|` | O(1) | `\|Σ\|` punteros + una alocación extra | Sí | Equivalente al arreglo, pero el tamaño deja de ser fijo. |
| `std::map` de símbolo a nodo | O(log \|Σ\|) | Solo los hijos que existen | Sí (el map está ordenado) | Sirve para cualquier alfabeto y ahorra memoria si hay pocos hijos. |
| Tabla de hash de símbolo a nodo | O(1) **esperado** | Solo los que existen | **No** | Pierde justo lo que hace valioso al trie. |

> Pregunta para pensar: ¿y si guardáramos los hijos de cada nodo en la tabla de hash de la clase anterior? ¿Qué se pierde? (Pista: mirá la tabla de comparación de más abajo.) Con un alfabeto chico y constante, el arreglo ya da acceso directo, así que el hash no aporta nada y rompe el orden y el peor caso O(m).



# Las operaciones del diccionario

En todas las operaciones el patrón es el mismo: **se parte de la raíz y se baja un símbolo a la vez**, siguiendo la clave. Los nombres siguen a la teórica: `Insertar`, `NodoDe`, `Pertenece`, `Obtener`, `Eliminar`.

## Inserción

`Insertar(D, clave, valor)` guarda la clave con su valor (si la clave ya estaba, reemplaza el valor anterior). Se recorre la clave símbolo por símbolo: si el camino ya existe se lo sigue, y si en algún punto falta el hijo correspondiente, se crea ahí un trie vacío. Al terminar la clave, el último nodo se marca como clave y se le asigna el valor.

Costo: **O(m)**, con `m` el largo de la clave. Es un paso O(1) por símbolo, y no depende de cuántas claves haya ya en el trie.

## Búsqueda

Las consultas se apoyan en una operación común, `NodoDe(D, clave)`: devolver el nodo al que se llega siguiendo la clave desde la raíz, o nulo si el camino se corta antes de terminar.

- `Pertenece(D, clave)`: la clave está si `NodoDe` devuelve un nodo **y** ese nodo está marcado como clave. Que el camino exista no alcanza: puede ser solo un prefijo.
- `Obtener(D, clave)`: con la precondición de que la clave pertenece, devuelve el valor guardado en el nodo al que se llega.

Costo de las dos: **O(m)**, y tampoco depende de cuántas claves haya en el trie.

## Eliminación

No alcanza con "desmarcar" el nodo de la clave. Si se borra `"casa"` y era la única clave que pasaba por esos nodos, quedarían **hojas vacías** (nodos sin hijos y sin clave) y se rompería el invariante. Por eso, al eliminar hay que revisar **hacia arriba** si el borrado dejó hojas vacías, y quitarlas (liberando su memoria).

Un detalle de diseño: los nodos no conocen a su padre. Para poder subir después de haber bajado, hay que **recordar el camino recorrido**; la teórica lo hace con una pila donde se guardan los ancestros mientras se baja. Una **hoja vacía** es un nodo que no es clave y cuyos hijos son todos nulos (revisarlo cuesta O(|Σ|) = O(1)). La poda sube mientras se encuentre una hoja vacía, y se detiene en el primer nodo que todavía sirve; la raíz nunca se elimina.

Costo total: **O(m)**: a lo sumo `m` ancestros a revisar, y cada paso es O(1).

Tres casos para tener en la cabeza (con `"casa"` y `"casas"` en el trie):

| Qué borro | Qué pasa |
|---|---|
| `"casas"` | Se poda solo el nodo de la `'s'` final; `"casa"` sigue. |
| `"casa"` | No se poda nada: solo se desmarca el nodo de la `'a'`, porque todavía tiene un hijo (`'s'`). |
| `"casa"` y `"casas"` (las dos) | Se poda toda la rama hasta llegar a un nodo que sirva a otra clave (o a la raíz). |

> Pregunta para pensar: ¿por qué hay que recordar los ancestros? ¿Qué otra forma de "subir" habría en una solución recursiva? (Pista: una llamada recursiva ya "recuerda" por dónde bajó.)



# Prefijos y recorridos

**¿Hay alguna clave con este prefijo?** Basta con ubicarse en el nodo del prefijo con `NodoDe`: si el camino existe, la respuesta es sí. Esto es correcto **gracias al invariante**: como no hay ramas muertas, todo camino que existe lleva a alguna clave. Si el borrado no podara, un prefijo podría existir en el trie sin ninguna clave detrás, y la respuesta sería incorrecta. Es un buen ejemplo de **por qué se mantiene un invariante**.

**¿Cuántas claves tienen este prefijo?** Hay que ubicarse en el nodo del prefijo y contar las claves que cuelgan de él (el subtrie).

**Recorrer todas las claves.** Se hace con un **recorrido en profundidad (DFS)** que va acumulando el camino desde la raíz: cada vez que se llega a un nodo marcado como clave, el camino acumulado en ese momento es una clave del trie. Como los hijos de cada nodo se visitan en el orden de Σ, las claves salen **en orden alfabético**, sin ordenar nada. Esto es lo que quiere decir "recorre en orden" en la tabla de comparación, y es algo que la tabla de hash no puede ofrecer.

**Autocompletar.** Es el mismo recorrido, pero arrancando desde el nodo del prefijo en lugar de desde la raíz: se encuentran todas las claves que empiezan con lo que el usuario escribió. Como el costo de ubicarse en el prefijo es O(m), no depende de cuán grande sea el diccionario. Los buscadores no devuelven *todas* las claves sino unas pocas sugerencias; y, como el trie es un diccionario, el valor asociado puede ser una **frecuencia de uso**, de modo que se puedan sugerir primero las claves más usadas.



# Complejidad y espacio

Sea `m` el largo de la clave o prefijo y `n` la cantidad de claves.

| Operación | Complejidad |
|---|---|
| `Insertar`, `NodoDe`, `Pertenece`, `Obtener`, `Eliminar` | **O(m)** en peor caso |
| Saber si hay alguna clave con un prefijo | O(m) |
| Recorrer todas las claves | O(cantidad de nodos del trie) |
| Autocompletar un prefijo | O(m + tamaño del subtrie recorrido) |

Lo notable es que **el costo no depende de `n`**: buscar en un trie de 10 claves o de 10 millones cuesta lo mismo si la clave tiene el mismo largo. Eso es lo que no lograban ni el AVL (O(m log n)) ni la tabla de hash (O(n · m) en peor caso).

## El peor caso de espacio

Un trie puede necesitar **O(n · m) nodos** para `n` claves de longitud `m`: muchos más que los O(n) nodos de un ABB o un AVL, porque cada clave puede recorrer un camino de `m` nodos exclusivo suyo (y además cada nodo gasta `|Σ|` punteros).

Un ejemplo concreto: con Σ = {`a`, `b`}, las `n + 1` claves formadas por `i` letras `a` seguidas de `n − i` letras `b` (para `0 ≤ i ≤ n`, todas de longitud `n`) forman un trie con **O(n²) nodos**. Con `n = 3` son `bbb`, `abb`, `aab` y `aaa`: las ramas se separan enseguida y casi no comparten prefijo, así que casi todos los nodos son exclusivos de una sola clave.

> Pregunta para pensar: ¿cuántos nodos tiene (como máximo) un trie con las claves `"a"`, `"aa"`, `"aaa"`, ..., hasta `n` letras? ¿Y uno con `n` claves de largo `m` que no comparten ni una letra?

## Comprimiendo el espacio: árboles PATRICIA

Se puede evitar buena parte de ese desperdicio **comprimiendo los caminos que no tienen bifurcaciones**: una cadena de nodos con un único hijo (`"c" → "a" → "s"`) se fusiona en una sola arista etiquetada con un string (`"cas"`). Estos son los **árboles PATRICIA** (también llamados *radix trees*). No vamos a entrar en el detalle.



# Comparación de representaciones

Costo de `Pertenece` / `Obtener` / `Insertar` / `Eliminar`, con `n` la cantidad de claves y `m` la longitud de una clave string:

| | Claves numéricas | Claves string | ¿Recorre en orden? |
|---|---|---|---|
| AVL | O(log n) peor caso | O(m log n) peor caso | Sí |
| Tabla de hash | O(1) promedio, O(n) peor caso | O(m) promedio, O(n · m) peor caso | No |
| Trie | — | **O(m) peor caso** | **Sí** |

El trie es el **único con O(m) en peor caso** para claves string, y además **conserva el orden**. Lo paga en espacio: hasta O(n · m) nodos.

> Tip: si el enunciado habla de **prefijos**, de **autocompletar**, de **claves que empiezan con...**, de **diccionarios con comodines** o de **el prefijo común más largo**, pensar en un trie. Si solo hay que preguntar "¿está o no está esta clave exacta?" y la memoria importa, una tabla de hash alcanza y gasta menos.



# Dónde aparecen los tries

Algunas situaciones donde un trie es la estructura natural:

- **Autocompletado y sugerencias** en teclados y buscadores, a veces ordenadas por frecuencia de uso.
- **Correctores y verificadores de diccionario**: consultas rápidas de palabras válidas.
- **Prefijos compartidos**: encontrar el prefijo común de un conjunto de claves, o agrupar palabras por raíz.
- **Búsquedas con patrones**, por ejemplo con comodines que reemplazan una letra cualquiera.
- **Enrutamiento de redes** (*longest prefix match*): los routers usan un trie sobre los bits de la dirección IP para decidir por dónde reenviar un paquete.

> Pregunta para pensar: de la lista anterior, ¿en cuáles el hecho de que el trie conserve el orden alfabético es lo que lo hace útil?



# Más allá de los strings: tries binarios

Un trie no tiene por qué ser de letras: puede construirse sobre **cualquier secuencia de símbolos**. Un caso muy usado es el **trie binario**, donde Σ = {0, 1}: cada nodo tiene a lo sumo dos hijos (bit `0` y bit `1`) y cada número entero se guarda como el camino formado por sus bits, del más significativo al menos significativo.

Como todos los números de un mismo ancho tienen la misma cantidad de bits, todos los caminos tienen el mismo largo, y las operaciones cuestan O(cantidad de bits), no O(cantidad de números). Esto vuelve útil al trie binario para problemas donde importa **cómo se parecen los números bit a bit**, como los que involucran la operación XOR.

> Pregunta para pensar: en un número binario, ¿por qué un `1` en un bit alto vale más que todos los bits bajos juntos? ¿Qué implica eso para una búsqueda que quiere un resultado lo más grande posible?



# Repaso relámpago

- ¿Por qué un trie le gana a un AVL cuando las claves son strings largas?
- En un trie, ¿qué invariante evita que un nodo sin hijos sea ambiguo?
- ¿Por qué un trie puede necesitar más memoria que un ABB o un AVL con las mismas claves?
- ¿Qué pasa con la pregunta "¿hay alguna clave con este prefijo?" si el borrado no poda las hojas vacías?
- ¿Por qué no conviene guardar los hijos en una tabla de hash?
- ¿Qué diferencia hay entre que exista el camino de un prefijo y que ese prefijo sea una clave?



# Anexo: Rosetree (árbol n-ario)

El trie es un caso particular de una estructura más general: el **rosetree**, un árbol donde cada nodo puede tener cualquier cantidad de hijos. Esta sección repasa la versión genérica.

## ¿Qué es un rosetree?

Hasta ahora trabajamos sobre todo con árboles **binarios**, donde cada nodo tiene a lo sumo dos hijos. Un **rosetree** (también llamado árbol n-ario o *árbol general*) generaliza esa idea: cada nodo puede tener **cualquier cantidad de hijos**, desde cero hasta n.

> Un rosetree no tiene "hijo izquierdo" ni "hijo derecho": tiene una **colección ordenada de hijos**.

Es la estructura que usamos naturalmente cuando pensamos en jerarquías: una carpeta que contiene archivos y otras carpetas, un organigrama de una empresa, el árbol genealógico de un torneo, o el árbol de decisiones de un juego.

## Representación del rosetree

En C++, como es un lenguaje de tipado estático, hay que definir explícitamente la estructura. Cada nodo guarda un **valor** y un `std::vector` de **punteros a sus hijos** (la definición ya viene dada en los ejercicios).

> ¿Por qué un vector de punteros a nodos y no un vector de nodos por valor? Porque el tipo del nodo se está definiendo a sí mismo (es un tipo recursivo). El compilador necesita conocer el tamaño exacto de la estructura, y un vector de punteros tiene tamaño fijo (son direcciones de memoria), mientras que un vector de "nodos por valor" tendría tamaño indefinido.

Un nodo hoja es el que tiene el vector de hijos vacío, y el árbol vacío se representa con un puntero nulo.

## Recorridos y recursión

Un rosetree se recorre de forma muy parecida a un árbol binario, solo que en vez de "visitar izquierda y derecha" hay que **recorrer el vector de hijos**. La receta general de cualquier función recursiva sobre un rosetree es:

1. **Caso base**: el nodo es nulo (árbol vacío) o no tiene hijos (es una hoja).
2. **Caso recursivo**: procesar el nodo actual y **combinar** el resultado de aplicar la misma función a cada uno de sus hijos.

La forma de combinar depende de la pregunta. Para contar nodos, se **suma** (1 por el nodo actual más lo que devuelve cada hijo). Para calcular la altura, se toma el **máximo** entre los hijos y se suma 1.

> Pregunta para pensar: en un árbol binario la recursión hace dos llamados (izquierda y derecha). En un rosetree, ¿cuántos llamados recursivos se hacen? ¿Depende del nodo?

> Pregunta para pensar: ¿cómo cambiaría la función que cuenta nodos si quisiéramos contar solamente las **hojas** del árbol?

## Ejemplos donde aparece naturalmente

El rosetree no es una curiosidad teórica: aparece todo el tiempo en problemas reales.

| Ejemplo | Nodo | Hijos |
|---|---|---|
| Sistema de archivos | Carpeta o archivo | Subcarpetas y archivos que contiene |
| Organigrama de una empresa | Empleado | Sus reportes directos |
| Árbol de decisión de un juego | Estado del juego | Cada jugada posible desde ese estado |
| Árbol sintáctico de una expresión | Operador o subexpresión | Sus operandos |
| Comentarios anidados (foro, redes sociales) | Comentario | Respuestas a ese comentario |
| **Trie** | Prefijo | Un hijo posible por cada símbolo del alfabeto |

> Tip: cuando en un enunciado aparece la palabra "jerarquía", "carpeta", "subordinados" o "anidado", es una buena pista de que conviene modelar el problema con un rosetree en vez de forzarlo a un árbol binario.

## Relación con el trie

Mirá otra vez el nodo del trie: es un rosetree con dos particularidades.

| | Rosetree genérico | Trie |
|---|---|---|
| Hijos | Un vector con un puntero por cada hijo que existe | Un arreglo de `\|Σ\|` posiciones, indexado por **símbolo** (nulo si no hay) |
| Dato del nodo | Un valor arbitrario | Una marca de clave y un valor (la información principal está en el **camino**) |
| Qué significa un camino | Una ruta cualquiera | Un prefijo o una clave |

Por eso las recetas son las mismas: caso base, recorrer los hijos y combinar resultados. Contar los nodos de un trie es lo mismo que contarlos en un rosetree (salvo que hay que saltear los hijos nulos), y recorrer las claves del trie es un recorrido en profundidad como cualquier otro, solo que arrastrando el camino (los símbolos de las aristas) que se va formando.
