Tutorial 9 – Tries
===

> **Ingeniería en Inteligencia Artificial** — **Algoritmos y Estructura de Datos**
> **2C 2026** · Docente: **Ing. Magali Marijuan**

- [Tutorial 9 – Tries](#tutorial-9--tries)
- [Objetivos de la clase](#objetivos-de-la-clase)
- [Motivación: buscar por prefijo](#motivación-buscar-por-prefijo)
- [¿Qué es un trie?](#qué-es-un-trie)
  - [La marca de fin de palabra](#la-marca-de-fin-de-palabra)
- [Representación en C++](#representación-en-c)
  - [Variantes del nodo](#variantes-del-nodo)
- [Operaciones básicas](#operaciones-básicas)
  - [Inserción](#inserción)
  - [Búsqueda: palabra vs prefijo](#búsqueda-palabra-vs-prefijo)
  - [Borrado](#borrado)
- [Recorridos: listar y autocompletar](#recorridos-listar-y-autocompletar)
- [Complejidad y memoria](#complejidad-y-memoria)
- [Trie vs hash vs árbol de búsqueda](#trie-vs-hash-vs-árbol-de-búsqueda)
- [Problemas típicos que se resuelven con un trie](#problemas-típicos-que-se-resuelven-con-un-trie)
- [Más allá de los strings: tries binarios](#más-allá-de-los-strings-tries-binarios)
- [Variantes y extensiones](#variantes-y-extensiones)
- [Anexo: Rosetree (árbol n-ario)](#anexo-rosetree-árbol-n-ario)
  - [¿Qué es un rosetree?](#qué-es-un-rosetree)
  - [Representación en C++ del rosetree](#representación-en-c-del-rosetree)
  - [Recorridos y recursión](#recorridos-y-recursión)
  - [Ejemplos donde aparece naturalmente](#ejemplos-donde-aparece-naturalmente)
  - [Relación con el trie](#relación-con-el-trie)



# Objetivos de la clase
- Entender qué es un trie (árbol de prefijos) y qué problemas resuelve mejor que una tabla de hash o un árbol de búsqueda.
- Implementar un trie en C++: inserción, búsqueda de palabras y de prefijos, y borrado con poda.
- Recorrer un trie con DFS para listar palabras y armar un autocompletado.
- Reconocer problemas donde un trie es la herramienta natural (prefijos, diccionarios, comodines, XOR máximo).
- Reconocer al trie como un caso particular de rosetree (árbol n-ario) y trabajar con rosetrees genéricos.



# Motivación: buscar por prefijo

Imaginá el teclado de tu celular sugiriendo palabras mientras escribís, o el buscador de Google completando tu consulta. En los dos casos el problema es el mismo:

> Dado un conjunto grande de palabras y un **prefijo** (lo que el usuario escribió hasta ahora), encontrar rápido todas las palabras que empiezan con ese prefijo.

¿Qué estructuras de las que ya vimos sirven?

- **Tabla de hash**: sirve para preguntar "¿está *esta* palabra exacta?", pero el hash de `"cas"` no tiene ninguna relación con el hash de `"casa"`. Para buscar por prefijo habría que revisar **todas** las claves: O(n).
- **ABB de strings**: las palabras con un mismo prefijo quedan juntas en el recorrido inorder, pero encontrarlas cuesta O(k · log n) (cada comparación entre strings cuesta hasta O(k)), y el árbol tiene que estar balanceado.
- **Trie**: guarda las palabras **letra por letra, compartiendo los prefijos**. Para ubicar un prefijo de largo `k` hay que bajar `k` niveles: O(k), sin importar cuántas palabras haya guardadas.

> Pregunta para pensar: si el diccionario tiene 1.000.000 de palabras y el usuario escribió `"alg"`, ¿cuántas palabras hay que mirar con una tabla de hash? ¿Y cuántos pasos hacemos para *ubicarnos* en el prefijo con un trie?



# ¿Qué es un trie?

Un **trie** (del inglés *re**trie**val*, se pronuncia como "try") es un árbol donde:

- Cada **arista** está etiquetada con **una letra**.
- La **raíz** representa el prefijo vacío.
- El camino desde la raíz hasta un nodo, leyendo las letras de las aristas, forma un **prefijo**.
- Las palabras que comparten un prefijo **comparten el camino** que lo representa.

Por ejemplo, el trie con las palabras `ala`, `algo`, `alto`, `casa`, `caso` y `cosa` (el `*` marca el fin de una palabra):

```
(raíz)
 ├── a
 │    └── l
 │         ├── a*          "ala"
 │         ├── g
 │         │    └── o*     "algo"
 │         └── t
 │              └── o*     "alto"
 └── c
      ├── a
      │    └── s
      │         ├── a*     "casa"
      │         └── o*     "caso"
      └── o
           └── s
                └── a*    "cosa"
```

Guardar esas 6 palabras por separado ocuparía 3 + 4 + 4 + 4 + 4 + 4 = 23 letras; el trie usa solo 15 nodos (sin contar la raíz) porque `"al"` y `"cas"` se guardan una sola vez.

## La marca de fin de palabra

Fijate que en el trie anterior existe el camino `a → l`, pero **`"al"` no es una palabra guardada**: es solo un prefijo de `ala`, `algo` y `alto`. Por eso cada nodo necesita un booleano, `esFinal`, que diga si **acá termina una palabra**.

| Pregunta | Qué hay que chequear |
|---|---|
| ¿Existe algún camino `c-a-s`? | Se puede bajar por todas las letras |
| ¿`"cas"` es una palabra guardada? | Se puede bajar **y** el nodo final tiene `esFinal == true` |

> Esta es la fuente de error más común: olvidarse de `esFinal` y devolver `true` para cualquier prefijo. Si el trie tiene `"casa"`, `contiene("cas")` tiene que dar `false`.

Además, `esFinal` permite guardar palabras que son prefijo de otras: si el trie tiene `"ca"` y `"casa"`, el nodo de la `'a'` tiene `esFinal == true` **y** además tiene un hijo.



# Representación en C++

Usamos un `struct` para el nodo con un `std::map` que asocia cada letra con su nodo hijo, y el flag `esFinal`. El nodo **no guarda su propia letra**: la letra es la etiqueta de la arista que baja desde el padre (la clave del map).

```cpp
struct NodoTrie {
    std::map<char, NodoTrie*> hijos;
    bool esFinal;

    NodoTrie() : esFinal(false) {}
};

// Postorder: primero liberar los hijos, después el nodo.
void liberarNodoTrie(NodoTrie* nodo) {
    if (nodo == nullptr) return;
    for (auto& par : nodo->hijos) {
        liberarNodoTrie(par.second);
    }
    delete nodo;
}

struct Trie {
    NodoTrie* raiz;   // siempre existe; representa el prefijo vacío
    int cantidad;     // cantidad de palabras guardadas

    Trie() : raiz(new NodoTrie()), cantidad(0) {}
    ~Trie() { liberarNodoTrie(raiz); }
};
```

> Como el trie crea nodos con `new`, tiene que liberarlos: es el mismo patrón de `liberarArbol` de la clase de ABB (postorder). Si no, Valgrind se va a quejar.

## Variantes del nodo

Hay varias formas de guardar los hijos de un nodo. Ninguna es "la correcta": depende del problema.

| Representación | Buscar un hijo | Memoria por nodo | Recorrido en orden | Notas |
|---|---|---|---|---|
| `std::map<char, NodoTrie*>` | O(log σ) | Solo los hijos que existen | Sí, alfabético | La que usamos en la práctica. Sirve para cualquier alfabeto. |
| `std::array<NodoTrie*, 26>` | O(1) | 26 punteros siempre | Sí (recorriendo los 26) | Rápida, pero gasta mucha memoria si hay pocos hijos; solo sirve para letras `a-z` (hay que usar `c - 'a'`). |
| `std::unordered_map<char, NodoTrie*>` | O(1) esperado | Solo los que existen | No | Rápida, pero pierde el orden alfabético. |

(σ = tamaño del alfabeto. Con σ chico, `log σ` es casi constante.)

```cpp
// Versión con arreglo fijo, para letras minúsculas a-z:
struct NodoTrieArr {
    NodoTrieArr* hijos[26];
    bool esFinal;
};
// Para ir al hijo de la letra c:  nodo->hijos[c - 'a']
```



# Operaciones básicas

En todas las operaciones el patrón es el mismo: **partir de la raíz y bajar una letra a la vez**.

## Inserción

Se baja por la palabra; si falta el hijo para alguna letra, se crea. Al terminar, se marca el último nodo como final:

```cpp
void insertar(Trie& trie, const std::string& palabra) {
    NodoTrie* nodo = trie.raiz;
    for (char c : palabra) {
        if (nodo->hijos.find(c) == nodo->hijos.end()) {
            nodo->hijos[c] = new NodoTrie();   // el camino no existía: lo creo
        }
        nodo = nodo->hijos[c];
    }
    if (!nodo->esFinal) {      // si ya era final, la palabra estaba repetida
        nodo->esFinal = true;
        trie.cantidad++;
    }
}
```

Costo: **O(k)**, con `k` el largo de la palabra. No depende de cuántas palabras haya.

## Búsqueda: palabra vs prefijo

Buscar una palabra y buscar un prefijo comparten el recorrido: bajar por las letras. La única diferencia es qué se mira al final. Conviene escribir un auxiliar que devuelva el nodo al que se llega (o `nullptr` si el camino se corta):

```cpp
NodoTrie* buscarNodo(const Trie& trie, const std::string& s) {
    NodoTrie* nodo = trie.raiz;
    for (char c : s) {
        auto it = nodo->hijos.find(c);
        if (it == nodo->hijos.end()) {
            return nullptr;       // el camino se corta: no hay nada con ese prefijo
        }
        nodo = it->second;
    }
    return nodo;
}

bool contiene(const Trie& trie, const std::string& palabra) {
    NodoTrie* nodo = buscarNodo(trie, palabra);
    return nodo != nullptr && nodo->esFinal;   // tiene que ser una palabra completa
}

bool tienePrefijo(const Trie& trie, const std::string& prefijo) {
    return buscarNodo(trie, prefijo) != nullptr;
}
```

> Pregunta para pensar: ¿qué devuelve `tienePrefijo(trie, "")` según este código? ¿Es lo que esperarías si el trie está vacío?

## Borrado

Es la operación más delicada. No alcanza con poner `esFinal = false`: si borramos `"casa"` y era la única palabra que pasaba por esos nodos, quedarían **ramas muertas** (nodos que no llevan a ninguna palabra) gastando memoria. Hay que **podar**: subiendo desde la hoja, borrar los nodos que quedaron sin hijos y que no son final de otra palabra.

Se resuelve bien con una recursión que devuelve "este nodo quedó inútil, que el padre lo elimine":

```cpp
// Devuelve true si el nodo quedó sin hijos y sin ser final (se puede borrar).
bool borrarRec(NodoTrie* nodo, const std::string& palabra, size_t i) {
    if (i == palabra.size()) {
        nodo->esFinal = false;                 // llegamos al final de la palabra
    } else {
        char c = palabra[i];
        NodoTrie* hijo = nodo->hijos[c];
        if (borrarRec(hijo, palabra, i + 1)) { // el hijo quedó inútil: lo podo
            delete hijo;
            nodo->hijos.erase(c);
        }
    }
    return nodo->hijos.empty() && !nodo->esFinal;
}

bool borrar(Trie& trie, const std::string& palabra) {
    if (!contiene(trie, palabra)) return false;    // primero verifico que esté
    borrarRec(trie.raiz, palabra, 0);              // la raíz nunca se borra
    trie.cantidad--;
    return true;
}
```

Tres casos para tener en la cabeza (con `"casa"` y `"casas"` en el trie):

| Qué borro | Qué pasa |
|---|---|
| `"casas"` | Se poda solo el nodo de la `'s'` final; `"casa"` sigue. |
| `"casa"` | No se poda nada: solo se desmarca `esFinal` en la `'a'` (todavía hay un hijo `'s'`). |
| `"casa"` y `"casas"` (las dos) | Se poda toda la rama hasta llegar a un nodo que sirva a otra palabra (o a la raíz). |



# Recorridos: listar y autocompletar

Para obtener las palabras guardadas se hace un **DFS** llevando un `std::string` con el camino actual: al bajar a un hijo se agrega su letra, y al volver se la saca (*backtracking*). Cada vez que se llega a un nodo `esFinal`, el camino actual es una palabra.

```cpp
void recolectar(const NodoTrie* nodo, std::string& camino,
                std::vector<std::string>& resultado) {
    if (nodo->esFinal) {
        resultado.push_back(camino);
    }
    for (const auto& par : nodo->hijos) {   // el map itera en orden alfabético
        camino.push_back(par.first);         // bajo: agrego la letra
        recolectar(par.second, camino, resultado);
        camino.pop_back();                   // vuelvo: la saco
    }
}
```

Como `std::map` recorre las letras ordenadas, el resultado sale **en orden alfabético** sin necesidad de ordenar nada.

El **autocompletado** es lo mismo, pero arrancando desde el nodo del prefijo en vez de desde la raíz, y con el camino inicial igual al prefijo:

```cpp
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo) {
    std::vector<std::string> resultado;
    NodoTrie* nodo = buscarNodo(trie, prefijo);   // O(k): me ubico en el prefijo
    if (nodo == nullptr) return resultado;        // nadie empieza así
    std::string camino = prefijo;
    recolectar(nodo, camino, resultado);          // todo lo que cuelga de ahí
    return resultado;
}
```

> Tip: los buscadores no devuelven *todas* las palabras sino unas pocas sugerencias. Una versión con tope de resultados puede cortar el DFS apenas junta `maxResultados` palabras (esa es la variante que vas a implementar en los ejercicios).

> Pregunta para pensar: ¿en qué orden hay que visitar a los hijos si queremos las sugerencias más cortas primero? ¿Y si queremos las más frecuentes? (Pista: para lo segundo cada nodo final tendría que guardar un contador.)



# Complejidad y memoria

Sea `k` el largo de la palabra o prefijo, `n` la cantidad de palabras y `L` la suma de los largos de todas las palabras.

| Operación | Complejidad |
|---|---|
| `insertar`, `contiene`, `tienePrefijo`, `borrar` | **O(k)** (con `map` en los hijos: O(k · log σ)) |
| `listarPalabras` | O(nodos del trie) ≤ O(L) |
| `autocompletar(prefijo)` | O(k + tamaño del subárbol recorrido) |

Lo notable es que **el costo no depende de `n`**: buscar en un trie de 10 palabras o de 10 millones cuesta lo mismo si la palabra tiene el mismo largo.

**Memoria**: en el peor caso (palabras sin prefijos compartidos) el trie tiene O(L) nodos, y cada nodo gasta bastante más que una letra (un `map` o un arreglo de punteros, más el flag). Los prefijos compartidos ahorran nodos, pero cada nodo es caro. Es el precio de la velocidad.

> Pregunta para pensar: ¿cuántos nodos tiene (como máximo) un trie con las palabras `"a"`, `"aa"`, `"aaa"`, ..., hasta `n` letras? ¿Y un trie con `n` palabras de largo `k` que no comparten ni una letra?



# Trie vs hash vs árbol de búsqueda

| | Tabla de hash | ABB balanceado (`std::map`) | Trie |
|---|---|---|---|
| ¿Está la clave exacta? | O(k) esperado* | O(k · log n) | **O(k)** garantizado |
| Buscar por **prefijo** | O(n) (hay que revisar todo) | O(k · log n + resultados) | **O(k + resultados)** |
| Recorrer en orden alfabético | No | Sí | Sí (con `std::map` en los hijos) |
| Memoria | Baja | Baja | Alta |
| Claves | Cualquier tipo "hasheable" | Cualquier tipo comparable | Secuencias (strings, bits, dígitos) |

\* Hay que recorrer toda la clave para calcular el hash, así que también es O(k), pero "esperado" porque puede haber colisiones.

> Tip: si el enunciado habla de **prefijos**, de **autocompletar**, de **palabras que empiezan con...**, de **diccionario con comodines** o de **el prefijo común más largo**, pensar en un trie. Si solo hay que preguntar "¿está o no está esta clave exacta?", una tabla de hash alcanza y gasta menos memoria.



# Problemas típicos que se resuelven con un trie

Algunos patrones que aparecen una y otra vez (varios están en los ejercicios):

- **Autocompletado / sugerencias**: ubicarse en el prefijo y hacer DFS desde ahí.
- **Prefijo común más largo** de un conjunto de palabras: bajar desde la raíz mientras haya **un único hijo** y el nodo no sea final.
- **Búsqueda con comodín** (`"b.d"` matchea `"bad"`, `"bid"`, ...): cuando el patrón tiene un `'.'`, se prueban **todos** los hijos (backtracking); con una letra común, se sigue por un solo hijo.
- **Reemplazar palabras por su raíz** más corta: bajar por el trie y cortar en el primer nodo `esFinal`.
- **Construir palabras letra por letra**: bajar solo por nodos `esFinal`, así todos los prefijos de la palabra resultante están en el diccionario.
- **Corrector / verificador de diccionario**: tablas de palabras válidas con consultas rápidas.
- **Enrutamiento de IPs** (*longest prefix match*): los routers usan un trie sobre los bits de la dirección IP.



# Más allá de los strings: tries binarios

Un trie no tiene por qué ser de letras: puede ser de **cualquier secuencia de símbolos**. Un caso muy usado es el **trie binario**, donde cada nodo tiene a lo sumo dos hijos (bit `0` y bit `1`) y cada número entero se guarda como el camino de sus bits, del más significativo al menos significativo.

```cpp
struct NodoBit {
    NodoBit* hijo[2];   // hijo[0]: bit 0, hijo[1]: bit 1
};

// Insertar un número de 30 bits:
void insertarNumero(NodoBit* raiz, int x) {
    NodoBit* nodo = raiz;
    for (int i = 29; i >= 0; i--) {
        int bit = (x >> i) & 1;          // el bit i-ésimo de x
        if (nodo->hijo[bit] == nullptr) {
            nodo->hijo[bit] = new NodoBit();
        }
        nodo = nodo->hijo[bit];
    }
}
```

**Aplicación clásica: máximo XOR de un par.** Dado un arreglo, encontrar el mayor `a ^ b`. Probar todos los pares es O(n²). Con un trie binario: para cada `x`, se baja por el trie intentando en cada nivel ir por el bit **opuesto** al de `x` (cada bit en el que difieren aporta un `1` al XOR, y los bits altos pesan más). Si el hijo opuesto no existe, se va por el otro. Total: **O(n · B)**, con `B` la cantidad de bits.

> Pregunta para pensar: ¿por qué es "goloso" elegir siempre el bit opuesto en cada nivel? ¿Por qué un `1` en un bit alto vale más que todos los bits bajos juntos?



# Variantes y extensiones

Para tener en el radar (no hace falta implementarlas):

- **Contadores en los nodos**: guardar en cada nodo cuántas palabras pasan por él hace que `contarConPrefijo` sea O(k) en vez de recorrer el subárbol. Es un espacio-por-tiempo típico.
- **Trie comprimido (radix tree / Patricia)**: las cadenas de nodos con un único hijo se fusionan en una sola arista etiquetada con un string (`"c" → "a" → "s"` pasa a ser `"cas"`). Reduce muchísimo la cantidad de nodos.
- **Trie de sufijos / árbol de sufijos**: guarda todos los sufijos de un texto; permite buscar cualquier subcadena en O(k).
- **Autómata de Aho-Corasick**: un trie con "enlaces de fallo" que busca muchas palabras a la vez en un texto en una sola pasada.



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
| **Trie** | Prefijo | Una arista por cada letra que puede seguir |

> Tip: cuando en un enunciado aparece la palabra "jerarquía", "carpeta", "subordinados" o "anidado", es una buena pista de que conviene modelar el problema con un rosetree en vez de forzarlo a un árbol binario.

## Relación con el trie

Mirá otra vez `NodoTrie`: es un rosetree con dos particularidades.

| | Rosetree genérico | Trie |
|---|---|---|
| Hijos | `std::vector` ordenado por posición | `std::map` indexado por **letra** |
| Dato del nodo | Un `valor` arbitrario | Un flag `esFinal` (la información está en el **camino**) |
| Qué significa un camino | Una ruta cualquiera | Un prefijo / una palabra |

Por eso las recetas son las mismas: caso base + `for` sobre los hijos + combinar resultados. `contarNodos` del trie es idéntico al del rosetree, y listar palabras es un recorrido DFS igual que cualquier otro, solo que arrastrando el camino (las letras de las aristas) en un `std::string`.
