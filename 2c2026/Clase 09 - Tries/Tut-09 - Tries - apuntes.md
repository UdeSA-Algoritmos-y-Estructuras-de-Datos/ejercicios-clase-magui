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
- [Representación en C++](#representación-en-c)
  - [¿Cómo guardar los hijos?](#cómo-guardar-los-hijos)
- [Operaciones del diccionario](#operaciones-del-diccionario)
  - [Diccionario vacío](#diccionario-vacío)
  - [Inserción](#inserción)
  - [Búsqueda: `nodoDe`, `pertenece` y `obtener`](#búsqueda-nodode-pertenece-y-obtener)
  - [Eliminación](#eliminación)
- [Prefijos y recorridos](#prefijos-y-recorridos)
  - [Buscar por prefijo](#buscar-por-prefijo)
  - [Listar y autocompletar](#listar-y-autocompletar)
- [Complejidad y espacio](#complejidad-y-espacio)
  - [El peor caso de espacio](#el-peor-caso-de-espacio)
  - [Comprimiendo el espacio: árboles PATRICIA](#comprimiendo-el-espacio-árboles-patricia)
- [Comparación de representaciones](#comparación-de-representaciones)
- [Problemas típicos que se resuelven con un trie](#problemas-típicos-que-se-resuelven-con-un-trie)
- [Más allá de los strings: tries binarios](#más-allá-de-los-strings-tries-binarios)
- [Repaso relámpago](#repaso-relámpago)
- [Anexo: Rosetree (árbol n-ario)](#anexo-rosetree-árbol-n-ario)
  - [¿Qué es un rosetree?](#qué-es-un-rosetree)
  - [Representación en C++ del rosetree](#representación-en-c-del-rosetree)
  - [Recorridos y recursión](#recorridos-y-recursión)
  - [Ejemplos donde aparece naturalmente](#ejemplos-donde-aparece-naturalmente)
  - [Relación con el trie](#relación-con-el-trie)



# Objetivos de la clase
- Entender qué es un trie y por qué permite que `Pertenece` y `Obtener` cuesten O(m) en **peor caso**, sin depender de cuántas claves haya guardadas.
- Implementar el TAD Diccionario sobre un trie en C++: inserción, búsqueda, y eliminación preservando el invariante.
- Recorrer un trie con DFS para listar claves y armar un autocompletado.
- Reconocer problemas donde un trie es la herramienta natural (prefijos, comodines, frecuencias, XOR máximo).
- Comparar el trie con el AVL y la tabla de hash, incluyendo su costo en espacio.
- Reconocer al trie como un caso particular de rosetree (árbol n-ario) y trabajar con rosetrees genéricos.



# ¿Dónde estamos?

Ya representamos el TAD Diccionario de dos maneras. Con claves string de longitud `m` y `n` la cantidad de claves guardadas:

| | `Pertenece` / `Obtener` | ¿Recorre en orden? |
|---|---|---|
| AVL | O(m log n) peor caso | Sí |
| Tabla de hash | O(m) promedio, O(n · m) peor caso | No |

En las dos, el peor caso **depende de cuántas claves hay guardadas**.

> Pregunta para pensar: leer una clave ya cuesta O(m). ¿Se puede lograr que `Pertenece` y `Obtener` cuesten O(m) en **peor caso**, sin depender de `n`?

Un caso donde esto importa mucho es la **búsqueda por prefijo** (el teclado del celular sugiriendo palabras, el buscador completando tu consulta): dado un prefijo, encontrar todas las claves que empiezan con él. Una tabla de hash no sirve (el hash de `"cas"` no se parece en nada al de `"casa"`: habría que revisar todas las claves, O(n)), y el AVL tiene que comparar strings enteros en cada paso. Un trie guarda las claves **letra por letra, compartiendo los prefijos**.



# ¿Qué es un trie?

Sea Σ un **alfabeto**: un conjunto finito de símbolos (por ejemplo, las letras de `a` a `z`).

> **Trie**: un árbol en el que cada nodo tiene `|Σ|` hijos, uno por cada símbolo de Σ.
> - El camino desde la raíz hasta un nodo corresponde a una **clave** (concatenando los símbolos de las aristas recorridas).
> - El valor guardado en un nodo (si tiene) es el valor asociado a esa clave.

Cada hijo de un trie es a su vez la raíz de un trie más chico (un **subtrie**).

Un trie (del inglés *re**trie**val*) se pronuncia como "try". Las aristas están etiquetadas con un símbolo, pero el nodo **no guarda su símbolo**: queda determinado por la posición del hijo dentro de su padre.

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

Fijate que existe el camino `a → l`, pero **`"al"` no es una clave guardada**: es solo un prefijo de `"ala"`. Por eso cada nodo necesita saber si **el camino hasta él es una clave completa** (`hayClave`) y, en ese caso, cuál es su valor.

Además, `hayClave` permite guardar claves que son prefijo de otras: si el trie tiene `"ca"` y `"casa"`, el nodo de la `'a'` tiene `hayClave == true` **y** además tiene un hijo.

> Esta es la fuente de error más común: olvidarse de `hayClave` y devolver `true` para cualquier prefijo. Si el trie tiene `"casa"`, `pertenece("cas")` tiene que dar `false`.

## El invariante del trie

La definición de arriba también permitiría un trie que **desperdicia espacio**: con nodos que son hojas, no tienen hijos y tampoco tienen valor. Mirando solo uno de esos nodos no se puede saber si es una clave con un valor "vacío" o si esa clave directamente no está. Para evitar la ambigüedad, todo trie debe cumplir:

> **Invariante de un trie**: todo nodo hoja (sin hijos) tiene un valor asociado (`hayClave == true`), **excepto la raíz**, que puede no tener hijos ni valor (el trie vacío).

Es decir: **no puede haber "ramas muertas"**. Tiene una consecuencia muy útil: si el camino de un prefijo existe, seguro hay alguna clave que empieza con él. Por eso `tienePrefijo` alcanza con comprobar que el camino exista. La que tiene que trabajar para mantener el invariante es la **eliminación**.



# Representación en C++

Cada nodo tiene tres campos (los mismos de la teórica):

- `hayClave`: ¿el camino hasta el nodo es una clave completa?
- `valor`: el valor asociado a esa clave, si `hayClave`.
- `hijos`: un arreglo de `|Σ|` punteros, uno por símbolo. `hijos[i]` es el subtrie al que se baja si el próximo símbolo es el `i`-ésimo de Σ (`nullptr` si no hay ninguna clave por ese lado).

Para pasar de un símbolo a su posición usamos `Indice(s)`, que debe costar O(1). Con Σ = {`a`, ..., `z`}, `Indice(s) = s - 'a'`.

```cpp
const size_t TAM_ALFABETO = 26;

size_t indice(char c) {                  // Indice(s): 'a' -> 0, ..., 'z' -> 25
    return static_cast<size_t>(c - 'a');
}

char simbolo(size_t i) {                 // la inversa: 0 -> 'a', ..., 25 -> 'z'
    return static_cast<char>('a' + i);
}

struct NodoTrie {
    std::array<NodoTrie*, TAM_ALFABETO> hijos;
    bool hayClave;
    int valor;

    NodoTrie() : hayClave(false), valor(0) {
        hijos.fill(nullptr);
    }
};

// Postorder: primero liberar los hijos, después el nodo.
void liberarNodoTrie(NodoTrie* nodo) {
    if (nodo == nullptr) return;
    for (NodoTrie* hijo : nodo->hijos) {
        liberarNodoTrie(hijo);
    }
    delete nodo;
}

struct Trie {
    NodoTrie* raiz;   // siempre existe; representa el prefijo vacío
    int cantidad;     // cantidad de claves guardadas

    Trie() : raiz(new NodoTrie()), cantidad(0) {}
    ~Trie() { liberarNodoTrie(raiz); }
};
```

> Como el trie crea nodos con `new`, tiene que liberarlos: es el mismo patrón de `liberarArbol` de la clase de ABB (postorder). Si no, Valgrind se va a quejar.

> Las claves solo pueden usar las letras de Σ. Con una mayúscula o un carácter raro, `indice` se sale del arreglo y accede a memoria que no es del nodo. Siempre hay que respetar la precondición.

## ¿Cómo guardar los hijos?

El arreglo de `|Σ|` punteros es lo que muestra la teórica, pero no es la única opción:

| Representación | Buscar un hijo | Memoria por nodo | Recorrido en orden | Comentario |
|---|---|---|---|---|
| `std::array<NodoTrie*, 26>` | O(1) **en peor caso** | 26 punteros siempre | Sí (recorriendo 0..25) | La de la teórica. La que usamos. |
| `std::vector<NodoTrie*>` de tamaño 26 | O(1) | 26 punteros + una alocación extra | Sí | Equivalente al array, pero el tamaño deja de formar parte del tipo. |
| `std::map<char, NodoTrie*>` | O(log \|Σ\|) | Solo los hijos que existen | Sí (el map está ordenado) | Sirve para cualquier alfabeto y ahorra memoria si hay pocos hijos. |
| Tabla de hash (`std::unordered_map`) | O(1) **esperado** | Solo los que existen | **No** | Pierde justo lo que hace valioso al trie. |

> Pregunta para pensar: ¿y si guardáramos los hijos de cada nodo en la tabla de hash de la clase anterior? ¿Qué se pierde? (Pista: mirá la tabla de comparación de más abajo.) Con un alfabeto chico y constante, el arreglo ya da acceso directo, así que el hash no aporta nada y rompe el orden y el peor caso O(m).



# Operaciones del diccionario

En todas las operaciones el patrón es el mismo: **partir de la raíz y bajar un símbolo a la vez**. Los nombres siguen a la teórica (`Insertar`, `NodoDe`, `Pertenece`, `Obtener`, `Eliminar`).

## Diccionario vacío

La raíz siempre es un nodo real, aunque el trie esté vacío (es el constructor de `Trie`): sin clave y con todos los hijos en `nullptr`.

## Inserción

Se baja por la clave; si falta el hijo para algún símbolo, se crea un trie vacío en ese lugar. Al terminar, se marca el último nodo como clave y se guarda el valor:

```cpp
void insertar(Trie& trie, const std::string& clave, int valor) {
    NodoTrie* a = trie.raiz;
    for (char c : clave) {
        size_t i = indice(c);
        if (a->hijos[i] == nullptr) {
            a->hijos[i] = new NodoTrie();      // el camino no existía: lo creo
        }
        a = a->hijos[i];
    }
    if (!a->hayClave) {        // si ya era clave, solo reemplazo el valor
        trie.cantidad++;
    }
    a->hayClave = true;
    a->valor = valor;
}
```

Costo: **O(m)**, con `m` el largo de la clave: un paso O(1) por símbolo, y no depende de cuántas claves haya ya.

## Búsqueda: `nodoDe`, `pertenece` y `obtener`

Las dos consultas bajan por el camino de la clave. Conviene tener un auxiliar que devuelva el nodo al que se llega siguiendo la clave (o `nullptr` si el camino se corta):

```cpp
NodoTrie* nodoDe(const Trie& trie, const std::string& clave) {
    NodoTrie* a = trie.raiz;
    for (char c : clave) {
        if (a == nullptr) return nullptr;
        a = a->hijos[indice(c)];
    }
    return a;
}

bool pertenece(const Trie& trie, const std::string& clave) {
    NodoTrie* a = nodoDe(trie, clave);
    return a != nullptr && a->hayClave;     // tiene que ser una clave completa
}

int obtener(const Trie& trie, const std::string& clave) {
    // Pre: pertenece(trie, clave)
    return nodoDe(trie, clave)->valor;
}
```

Costo de las tres: **O(m)**, no depende de cuántas claves haya en el trie.

## Eliminación

No alcanza con poner `hayClave = false`: si borramos `"casa"` y era la única clave que pasaba por esos nodos, quedarían **hojas vacías** (nodos sin hijos y sin clave), y se rompería el invariante. Hay que revisar, **subiendo**, si el borrado dejó hojas vacías y eliminarlas. Para poder subir (los nodos no tienen puntero al padre) se usa una **pila** donde se van guardando los ancestros mientras se baja:

```cpp
void eliminar(Trie& trie, const std::string& clave) {
    // Pre: pertenece(trie, clave)
    std::vector<NodoTrie*> pila;                 // pila de ancestros
    NodoTrie* a = trie.raiz;
    for (char c : clave) {
        pila.push_back(a);                       // Push
        a = a->hijos[indice(c)];
    }
    a->hayClave = false;
    a->valor = 0;
    trie.cantidad--;
    limpiarHojasVacias(pila, a, clave);
}
```

Un nodo es una **hoja vacía** si no es una clave y todos sus hijos son `nullptr` (O(|Σ|) = O(1)):

```cpp
bool esHojaVacia(const NodoTrie* a) {
    if (a->hayClave) return false;
    for (const NodoTrie* hijo : a->hijos) {
        if (hijo != nullptr) return false;
    }
    return true;
}
```

Y la limpieza va sacando ancestros de la pila mientras el nodo actual siga siendo una hoja vacía:

```cpp
void limpiarHojasVacias(std::vector<NodoTrie*>& pila, NodoTrie* nodo,
                        const std::string& clave) {
    size_t k = clave.size();
    while (!pila.empty() && esHojaVacia(nodo)) {
        NodoTrie* padre = pila.back();           // Pop
        pila.pop_back();
        k--;
        padre->hijos[indice(clave[k])] = nullptr;   // el padre ya no apunta al nodo
        delete nodo;
        nodo = padre;                            // sigo subiendo
    }
}
```

Costo total de `eliminar`: **O(m)**: a lo sumo `m` nodos en la pila, y cada paso de la limpieza es O(1). La raíz nunca se elimina (cuando la pila queda vacía el ciclo termina).

Tres casos para tener en la cabeza (con `"casa"` y `"casas"` en el trie):

| Qué borro | Qué pasa |
|---|---|
| `"casas"` | Se poda solo el nodo de la `'s'` final; `"casa"` sigue. |
| `"casa"` | No se poda nada: solo se desmarca `hayClave` en la `'a'` (todavía hay un hijo `'s'`). |
| `"casa"` y `"casas"` (las dos) | Se poda toda la rama hasta llegar a un nodo que sirva a otra clave (o a la raíz). |

> Pregunta para pensar: ¿por qué hay que apilar los ancestros? ¿Qué otra forma de "subir" tendríamos en una versión recursiva de `eliminar`? (La función recursiva devuelve "este nodo quedó inútil" y el padre decide si lo borra: la pila es la versión iterativa de ese mismo mecanismo.)



# Prefijos y recorridos

## Buscar por prefijo

Con `nodoDe` ya tenemos casi todo. Gracias al invariante (no hay ramas muertas), alcanza con que el camino exista:

```cpp
bool tienePrefijo(const Trie& trie, const std::string& prefijo) {
    if (prefijo.empty()) return trie.cantidad > 0;   // la raíz existe aunque el trie esté vacío
    return nodoDe(trie, prefijo) != nullptr;
}
```

> Esto solo es correcto porque `eliminar` poda: si quedara una rama muerta, `tienePrefijo` daría `true` para un prefijo sin ninguna clave. Es un buen ejemplo de **por qué se mantiene un invariante**.

Contar cuántas claves empiezan con un prefijo es llegar a su nodo y contar los nodos con `hayClave` de ese subtrie:

```cpp
int contarFinales(const NodoTrie* nodo) {
    int total = nodo->hayClave ? 1 : 0;
    for (const NodoTrie* hijo : nodo->hijos) {
        if (hijo != nullptr) total += contarFinales(hijo);
    }
    return total;
}

int contarConPrefijo(const Trie& trie, const std::string& prefijo) {
    NodoTrie* nodo = nodoDe(trie, prefijo);
    return nodo == nullptr ? 0 : contarFinales(nodo);
}
```

## Listar y autocompletar

Para obtener las claves guardadas se hace un **DFS** llevando un `std::string` con el camino actual: al bajar a un hijo se agrega su símbolo, y al volver se lo saca (*backtracking*). Cada vez que se llega a un nodo con `hayClave`, el camino actual es una clave.

```cpp
void recolectar(const NodoTrie* nodo, std::string& camino,
                std::vector<std::string>& resultado) {
    if (nodo->hayClave) {
        resultado.push_back(camino);
    }
    for (size_t i = 0; i < TAM_ALFABETO; i++) {   // de 'a' a 'z': orden alfabético
        if (nodo->hijos[i] != nullptr) {
            camino.push_back(simbolo(i));          // bajo: agrego el símbolo
            recolectar(nodo->hijos[i], camino, resultado);
            camino.pop_back();                     // vuelvo: lo saco
        }
    }
}
```

Como los hijos se recorren de la `a` a la `z`, el resultado sale **en orden alfabético** sin ordenar nada. Esto es lo que quiere decir "recorre en orden" en la tabla de comparación.

El **autocompletado** es lo mismo, pero arrancando desde el nodo del prefijo en vez de desde la raíz, y con el camino inicial igual al prefijo:

```cpp
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo) {
    std::vector<std::string> resultado;
    NodoTrie* nodo = nodoDe(trie, prefijo);       // O(m): me ubico en el prefijo
    if (nodo == nullptr) return resultado;        // nadie empieza así
    std::string camino = prefijo;
    recolectar(nodo, camino, resultado);          // todo lo que cuelga de ahí
    return resultado;
}
```

> Tip: los buscadores no devuelven *todas* las claves sino unas pocas sugerencias. Una versión con tope de resultados puede cortar el DFS apenas junta `maxResultados` claves. Y como el trie es un diccionario, el **valor** puede ser una frecuencia de uso: así se puede sugerir primero la clave con mayor valor (ambas variantes están en los ejercicios).



# Complejidad y espacio

Sea `m` el largo de la clave o prefijo y `n` la cantidad de claves.

| Operación | Complejidad |
|---|---|
| `insertar`, `nodoDe`, `pertenece`, `obtener`, `eliminar` | **O(m)** en peor caso |
| `tienePrefijo` | O(m) |
| `listarClaves` | O(cantidad de nodos del trie) |
| `autocompletar(prefijo)` | O(m + tamaño del subtrie recorrido) |

Lo notable es que **el costo no depende de `n`**: buscar en un trie de 10 claves o de 10 millones cuesta lo mismo si la clave tiene el mismo largo. Eso es lo que no lograban ni el AVL (`O(m log n)`) ni la tabla de hash (`O(n · m)` en peor caso).

## El peor caso de espacio

Un trie puede necesitar **O(n · m) nodos** para `n` claves de longitud `m`: muchos más que los O(n) nodos de un ABB o un AVL, porque cada clave puede recorrer un camino de `m` nodos exclusivo suyo (y además cada nodo gasta `|Σ|` punteros).

Un ejemplo concreto: con Σ = {`a`, `b`}, las `n + 1` claves `aⁱ bⁿ⁻ⁱ` (para `0 ≤ i ≤ n`, todas de longitud `n`) forman un trie con **O(n²) nodos**. Por ejemplo, con `n = 3`: `bbb`, `abb`, `aab`, `aaa`. Las ramas se separan enseguida y casi no comparten prefijo, así que casi todos los nodos son exclusivos de una sola clave.

> Pregunta para pensar: ¿cuántos nodos tiene (como máximo) un trie con las claves `"a"`, `"aa"`, `"aaa"`, ..., `"aaaa…a"` (hasta `n` letras)? ¿Y uno con `n` claves de largo `m` que no comparten ni una letra?

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

> Tip: si el enunciado habla de **prefijos**, de **autocompletar**, de **claves que empiezan con...**, de **diccionario con comodines** o de **el prefijo común más largo**, pensar en un trie. Si solo hay que preguntar "¿está o no está esta clave exacta?" y la memoria importa, una tabla de hash alcanza y gasta menos.



# Problemas típicos que se resuelven con un trie

Algunos patrones que aparecen una y otra vez (varios están en los ejercicios):

- **Autocompletado / sugerencias**: ubicarse en el prefijo y hacer DFS desde ahí. Si el valor es una frecuencia, quedarse con la clave de mayor valor.
- **Prefijo común más largo** de un conjunto de claves: bajar desde la raíz mientras haya **un único hijo** y el nodo no sea una clave.
- **Búsqueda con comodín** (`"b.d"` matchea `"bad"`, `"bid"`, ...): cuando el patrón tiene un `'.'`, se prueban **todos** los hijos (backtracking); con una letra común, se sigue por un solo hijo.
- **Reemplazar palabras por su raíz** más corta: bajar por el trie y cortar en el primer nodo con `hayClave`.
- **Enrutamiento de IPs** (*longest prefix match*): los routers usan un trie sobre los bits de la dirección IP.
- **Corrector / verificador de diccionario**: consultas rápidas de palabras válidas.



# Más allá de los strings: tries binarios

Un trie no tiene por qué ser de letras: puede ser de **cualquier secuencia de símbolos**. Un caso muy usado es el **trie binario**, donde Σ = {0, 1}: cada nodo tiene dos hijos (bit `0` y bit `1`) y cada número entero se guarda como el camino de sus bits, del más significativo al menos significativo.

```cpp
struct NodoBit {
    NodoBit* hijo[2];   // hijo[0]: bit 0, hijo[1]: bit 1
    NodoBit() { hijo[0] = nullptr; hijo[1] = nullptr; }
};

// Insertar un número de 30 bits:
void insertarNumero(NodoBit* raiz, int x) {
    NodoBit* nodo = raiz;
    for (int i = 29; i >= 0; i--) {
        size_t bit = static_cast<size_t>((x >> i) & 1);   // el bit i-ésimo de x
        if (nodo->hijo[bit] == nullptr) {
            nodo->hijo[bit] = new NodoBit();
        }
        nodo = nodo->hijo[bit];
    }
}
```

**Aplicación clásica: máximo XOR de un par.** Dado un arreglo, encontrar el mayor `a ^ b`. Probar todos los pares es O(n²). Con un trie binario: para cada `x`, se baja por el trie intentando en cada nivel ir por el bit **opuesto** al de `x` (cada bit en el que difieren aporta un `1` al XOR, y los bits altos pesan más). Si el hijo opuesto no existe, se va por el otro. Total: **O(n · B)**, con `B` la cantidad de bits.

> Pregunta para pensar: ¿por qué es "goloso" elegir siempre el bit opuesto en cada nivel? ¿Por qué un `1` en un bit alto vale más que todos los bits bajos juntos?



# Repaso relámpago

- ¿Por qué un trie le gana a un AVL cuando las claves son strings largas?
- En un trie, ¿qué invariante evita que un nodo sin hijos sea ambiguo?
- ¿Por qué un trie puede necesitar más memoria que un ABB o un AVL con las mismas claves?
- ¿Qué pasa con `tienePrefijo` si `eliminar` no poda las hojas vacías?
- ¿Por qué no conviene guardar los hijos en una tabla de hash?



# Anexo: Rosetree (árbol n-ario)

El trie es un caso particular de una estructura más general: el **rosetree**, que es un árbol donde cada nodo puede tener cualquier cantidad de hijos. Esta sección repasa la versión genérica.

## ¿Qué es un rosetree?

Hasta ahora trabajamos sobre todo con árboles **binarios**, donde cada nodo tiene a lo sumo dos hijos. Un **rosetree** (también llamado árbol n-ario o *árbol general*) generaliza esa idea: cada nodo puede tener **cualquier cantidad de hijos**, desde cero hasta n.

> Un rosetree no tiene "hijo izquierdo" ni "hijo derecho": tiene una **colección ordenada de hijos**.

Es la estructura que usamos naturalmente cuando pensamos en jerarquías: una carpeta que contiene archivos y otras carpetas, un organigrama de una empresa, el árbol genealógico de un torneo, o el árbol de decisiones de un juego.

## Representación en C++ del rosetree

En Python uno podría representar un nodo con una lista de hijos sin pensarlo demasiado. En C++, como es un lenguaje de tipado estático, necesitamos definir explícitamente la estructura. Usamos un `struct` con un `std::vector` de punteros a los hijos:

```cpp
template <typename T>
struct Rosetree {
    T valor;
    std::vector<Rosetree<T>*> hijos;
};
```

> ¿Por qué `std::vector<Rosetree<T>*>` y no `std::vector<Rosetree<T>>`? Porque el tipo `Rosetree<T>` se está definiendo a sí mismo (es un tipo recursivo). El compilador necesita conocer el tamaño exacto de la estructura, y un vector de punteros tiene tamaño fijo (son direcciones de memoria), mientras que un vector de "Rosetree por valor" tendría tamaño indefinido.

Crear un nodo hoja y agregarle hijos es directo:

```cpp
Rosetree<std::string>* raiz = new Rosetree<std::string>{"Documentos", {}};
Rosetree<std::string>* hijo1 = new Rosetree<std::string>{"Fotos", {}};
Rosetree<std::string>* hijo2 = new Rosetree<std::string>{"Tesis.pdf", {}};

raiz->hijos.push_back(hijo1);
raiz->hijos.push_back(hijo2);
```

## Recorridos y recursión

Un rosetree se recorre de forma muy parecida a un árbol binario, solo que en vez de "visitar izquierda y derecha" hay que **recorrer el vector de hijos**. La receta general de cualquier función recursiva sobre un rosetree es:

1. **Caso base**: el nodo es `nullptr` (árbol vacío) o no tiene hijos (es una hoja).
2. **Caso recursivo**: procesar el nodo actual y combinar el resultado de aplicar la misma función a cada uno de sus hijos, recorriendo el `std::vector<Rosetree<T>*>` con un `for`.

Por ejemplo, contar la cantidad de nodos de un rosetree:

```cpp
int contarNodos(Rosetree<int>* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    int total = 1; // me cuento a mí mismo
    for (Rosetree<int>* hijo : nodo->hijos) {
        total += contarNodos(hijo);
    }
    return total;
}
```

Calcular la altura es análogo, pero en vez de sumar hay que quedarse con el **máximo** entre los hijos:

```cpp
int altura(Rosetree<int>* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    if (nodo->hijos.empty()) {
        return 1; // una hoja tiene altura 1
    }
    int alturaMax = 0;
    for (Rosetree<int>* hijo : nodo->hijos) {
        alturaMax = std::max(alturaMax, altura(hijo));
    }
    return 1 + alturaMax;
}
```

> Pregunta para pensar: en un árbol binario la recursión hace dos llamados (izquierda y derecha). En un rosetree, ¿cuántos llamados recursivos se hacen? ¿Depende del nodo?

> Pregunta para pensar: ¿cómo cambiaría `contarNodos` si quisiéramos contar solamente las **hojas** del árbol?

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

Mirá otra vez `NodoTrie`: es un rosetree con dos particularidades.

| | Rosetree genérico | Trie |
|---|---|---|
| Hijos | `std::vector` de punteros, uno por hijo existente | `std::array` de `\|Σ\|` punteros, indexado por **símbolo** (`nullptr` si no hay) |
| Dato del nodo | Un `valor` arbitrario | `hayClave` + `valor` (la información principal está en el **camino**) |
| Qué significa un camino | Una ruta cualquiera | Un prefijo / una clave |

Por eso las recetas son las mismas: caso base + `for` sobre los hijos + combinar resultados. `contarNodos` del trie es idéntico al del rosetree (salvo que hay que saltear los `nullptr`), y listar claves es un recorrido DFS igual que cualquier otro, solo que arrastrando el camino (los símbolos de las aristas) en un `std::string`.
