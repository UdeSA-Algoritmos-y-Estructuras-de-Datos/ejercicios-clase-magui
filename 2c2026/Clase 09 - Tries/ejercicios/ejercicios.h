#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <array>
#include <cstddef>
#include <string>
#include <vector>

// ============================================================================
// Tut 09 - Tries
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
//
// Seguimos la teórica (Clase 14 - Tries): el trie implementa el TAD
// Diccionario (clave -> valor) con claves string. Los nombres de los campos
// y de las operaciones son los de la teórica:
//     hayClave, valor, hijos, Indice, NodoDe, Pertenece, Obtener, Eliminar...
// ============================================================================

// ============================================================================
// ESTRUCTURA DEL TRIE
// ============================================================================

// Alfabeto Σ = {'a', ..., 'z'}. Las claves SOLO pueden tener letras
// minúsculas: con cualquier otro carácter, indice se sale del arreglo.
// (Son size_t porque son posiciones de un arreglo: así no hace falta
// convertir cada vez que se indexa `hijos`, y -Wconversion queda contento.)
const size_t TAM_ALFABETO = 26;

// Indice(s): posición de un símbolo en Σ, en O(1).  'a' -> 0, ..., 'z' -> 25.
inline size_t indice(char c)
{
    return static_cast<size_t>(c - 'a');
}

// Operación inversa: el símbolo que está en la posición i.  0 -> 'a', etc.
inline char simbolo(size_t i)
{
    return static_cast<char>('a' + i);
}

// Nodo de un trie (el trie vacío se representa con un nodo sin clave y sin
// hijos; nullptr significa "no hay nada por ese lado").
//   - hayClave: ¿el camino desde la raíz hasta este nodo es una clave completa?
//   - valor:    el valor asociado a esa clave (solo tiene sentido si hayClave).
//   - hijos:    un puntero por cada símbolo de Σ. hijos[i] es el subtrie al que
//               se baja si el próximo símbolo es el i-ésimo de Σ (nullptr si no
//               hay ninguna clave por ese lado).
//
// Invariante del trie: todo nodo hoja (sin hijos) tiene hayClave == true,
// excepto la raíz, que puede estar vacía (el trie vacío).
// Es decir: NO puede haber "ramas muertas".
struct NodoTrie {
    std::array<NodoTrie*, TAM_ALFABETO> hijos;
    bool hayClave;
    int valor;

    NodoTrie() : hayClave(false), valor(0) { hijos.fill(nullptr); }
};

// Libera un nodo y todo su subárbol (postorder: primero los hijos).
inline void liberarNodoTrie(NodoTrie* nodo)
{
    if (nodo == nullptr) return;
    for (NodoTrie* hijo : nodo->hijos) {
        liberarNodoTrie(hijo);
    }
    delete nodo;
}

// El diccionario en sí: un puntero a la raíz (que SIEMPRE existe y representa
// el prefijo vacío) y la cantidad de claves guardadas.
// El destructor ya está resuelto: libera todos los nodos, así Valgrind no se
// queja mientras te concentrás en las operaciones.
struct Trie {
    NodoTrie* raiz;
    int cantidad;   // cantidad de claves distintas guardadas

    Trie() : raiz(new NodoTrie()), cantidad(0) {}
    ~Trie() { liberarNodoTrie(raiz); }

    // Un Trie es dueño de sus nodos: copiarlo duplicaría punteros y el
    // destructor liberaría dos veces la misma memoria.
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
};

// ============================================================================
// PARTE 1 — TAD DICCIONARIO SOBRE UN TRIE (como en la teórica)
// ============================================================================

// Ejercicio 1 — Insertar
// Guarda clave -> valor. Baja por la clave creando los nodos que falten
// (con `new NodoTrie()`) y marca el último con hayClave = true y el valor.
// Si la clave ya estaba, REEMPLAZA el valor (y no cambia `cantidad`).
// Precondición: la clave es no vacía y solo tiene letras minúsculas.
// Complejidad: O(m), con m el largo de la clave (no depende de cuántas
// claves haya guardadas).
void insertar(Trie& trie, const std::string& clave, int valor);

// Ejercicio 2 — NodoDe
// Devuelve el nodo al que se llega siguiendo la clave desde la raíz, o
// nullptr si el camino se corta. Ojo: el nodo puede existir y NO ser una
// clave (por ejemplo, si el trie tiene "casa", nodoDe(trie, "cas") existe,
// pero tiene hayClave == false). Con la clave vacía devuelve la raíz.
// Es la base de Pertenece, Obtener, tienePrefijo y contarConPrefijo.
// Complejidad: O(m).
NodoTrie* nodoDe(const Trie& trie, const std::string& clave);

// Ejercicio 3 — Pertenece
// Devuelve true si la clave COMPLETA está guardada en el trie.
// Si el trie tiene "casa", pertenece("cas") es false (es solo un prefijo).
// Complejidad: O(m).
bool pertenece(const Trie& trie, const std::string& clave);

// Ejercicio 4 — Obtener
// Devuelve el valor asociado a la clave.
// Precondición: pertenece(trie, clave).
// Complejidad: O(m).
int obtener(const Trie& trie, const std::string& clave);

// Ejercicio 5 — EsHojaVacia
// Devuelve true si el nodo no es una clave (hayClave == false) Y todos sus
// hijos son nullptr. Es la pregunta que se hace al eliminar para decidir si
// hay que podar el nodo.
// Complejidad: O(|Σ|) = O(1).
bool esHojaVacia(const NodoTrie* nodo);

// Ejercicio 6 — Eliminar
// Borra la clave (y su valor) del trie.
// Precondición: pertenece(trie, clave).
// Para respetar el invariante hay que PODAR: subiendo desde el nodo de la
// clave, eliminar (con delete) los nodos que quedaron como hoja vacía.
//   - Eliminar "casa" cuando también existe "casas" NO borra nodos
//     (solo pone hayClave = false en el nodo de la 'a').
//   - Eliminar "casas" cuando existe "casa" borra únicamente el nodo de la 's'.
// La teórica lo resuelve con una PILA que guarda los ancestros del nodo
// mientras se baja, y una función LimpiarHojasVacias que los va sacando.
// La raíz nunca se borra, aunque quede vacía.
// Complejidad: O(m).
void eliminar(Trie& trie, const std::string& clave);

// ============================================================================
// PARTE 2 — PREFIJOS Y RECORRIDOS (DFS sobre el trie)
// ============================================================================

// Ejercicio 7 — Tiene prefijo
// Devuelve true si alguna clave del trie empieza con `prefijo`.
// Con prefijo vacío devuelve true solo si el trie no está vacío.
// Gracias al invariante (no hay ramas muertas), alcanza con que el camino
// del prefijo exista: todo camino lleva a alguna clave.
// Complejidad: O(m).
bool tienePrefijo(const Trie& trie, const std::string& prefijo);

// Ejercicio 8 — Contar con prefijo
// Devuelve cuántas claves guardadas empiezan con `prefijo` (incluida la
// propia clave `prefijo`, si está). Con prefijo vacío devuelve el total.
// Pista: llegá hasta el nodo del prefijo con nodoDe y contá cuántos nodos con
// hayClave hay en su subárbol (incluido él mismo).
// Complejidad: O(m + tamaño del subárbol).
int contarConPrefijo(const Trie& trie, const std::string& prefijo);

// Ejercicio 9 — Contar nodos
// Devuelve la cantidad de nodos del trie, INCLUYENDO la raíz. Un trie vacío
// tiene 1 nodo. Sirve para medir cuánta memoria usa el trie y cuánto ahorran
// los prefijos compartidos: guardar "casa" y "caso" ocupa 1 + 4 + 1 = 6
// nodos, no 9. Es exactamente el contarNodos del rosetree (¡ver Parte 5!).
// Complejidad: O(n), donde n es la cantidad de nodos.
int contarNodos(const Trie& trie);

// Ejercicio 10 — Listar claves
// Devuelve TODAS las claves guardadas, en orden alfabético.
// Pista: DFS llevando un std::string con el camino actual. Al bajar a un
// hijo agregás su símbolo (push_back); al volver, lo sacás (pop_back). Si el
// nodo actual tiene hayClave, el camino actual es una clave. Recorrer los
// hijos de 0 a |Σ|-1 ya los visita en orden alfabético: no hay que ordenar.
// Complejidad: O(n), con n la cantidad de nodos (en un trie, los nodos
// alcanzan a lo sumo la suma de los largos de las claves).
std::vector<std::string> listarClaves(const Trie& trie);

// Ejercicio 11 — Autocompletar
// Devuelve las claves que empiezan con `prefijo`, en orden alfabético, pero
// a lo sumo `maxResultados` (como hacen los buscadores: sugieren unas pocas
// opciones, no todas). Si el prefijo no está, devuelve un vector vacío.
// Precondición: maxResultados >= 0.
// Pista: llegá al nodo del prefijo y hacé el DFS desde ahí, arrancando con
// el camino actual igual a `prefijo`. Cortá el recorrido en cuanto juntes
// maxResultados claves: no hace falta seguir.
// Complejidad: O(m + tamaño de lo recorrido).
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo, int maxResultados);

// Ejercicio 12 — Prefijo común más largo
// Devuelve el prefijo común más largo de TODAS las claves del trie (cadena
// vacía si no hay ninguno, o si el trie está vacío).
// Ejemplos: {"flower", "flow", "flight"} -> "fl"
//           {"dog", "racecar"}           -> ""
//           {"casa"}                     -> "casa"
//           {"ca", "casa", "cascada"}    -> "ca"
// Pista: bajá desde la raíz mientras el nodo tenga EXACTAMENTE un hijo y no
// sea una clave (si hayClave, esa clave se termina ahí y no se puede seguir).
// Complejidad: O(largo del prefijo resultante).
std::string prefijoComunMasLargo(const Trie& trie);

// ============================================================================
// PARTE 3 — PROBLEMAS CON TRIES
// ============================================================================

// Ejercicio 13 — Buscar con comodín
// Devuelve true si alguna clave del trie coincide EXACTAMENTE con el patrón,
// donde el carácter '.' puede reemplazar a cualquier letra.
// Ejemplos con {"bad", "dad", "mad"}:
//   "bad" -> true     ".ad" -> true     "b.." -> true     "..." -> true
//   "pad" -> false    "b."  -> false    "...." -> false
// Pista: recursión sobre (nodo, posición en el patrón). Si el carácter es
// '.', probás TODOS los hijos del nodo que no sean nullptr; si no, seguís
// por un único hijo.
// Precondición: el patrón solo tiene letras minúsculas y '.'.
// Complejidad: O(m) sin comodines; en el peor caso, O(tamaño del trie).
bool buscarConComodin(const Trie& trie, const std::string& patron);

// Ejercicio 14 — Reemplazar por raíces
// Muchas palabras derivan de una raíz: "cat" -> "cattle", "catalog". Dado un
// diccionario de raíces y una oración (palabras separadas por UN espacio),
// reemplazá cada palabra por la raíz MÁS CORTA del diccionario que sea
// prefijo de ella. Si ninguna raíz es prefijo, la palabra queda igual.
// Ejemplo: raíces {"cat", "bat", "rat"}
//   "the cattle was rattled by the battery"
//   -> "the cat was rat by the bat"
// Pista: armá un Trie con las raíces (el valor no importa: usá 0). Para cada
// palabra bajá por el trie símbolo por símbolo y cortá en cuanto pases por
// un nodo con hayClave (la primera que encuentres es la más corta).
// Precondición: solo letras minúsculas y espacios simples.
// Complejidad: O(L), con L la cantidad de caracteres de la oración (más lo
// que cueste armar el trie).
std::string reemplazarRaices(const std::vector<std::string>& raices, const std::string& oracion);

// Ejercicio 15 — Sugerencia más frecuente
// Usamos el trie como un diccionario palabra -> frecuencia (cuántas veces
// escribió esa palabra el usuario). Devuelve la clave de MAYOR valor entre
// las que empiezan con `prefijo`; si hay empate, la menor en orden
// alfabético. Si ninguna clave empieza con el prefijo, devuelve "".
// Es lo que hace el teclado del celular: sugerirte primero lo que más usás.
// Ejemplo con {"casa": 5, "caso": 9, "cosa": 9, "casas": 3}:
//   "ca" -> "caso"    "c" -> "caso" (empata con "cosa", pero caso < cosa)
//   "cas" -> "caso"   "casa" -> "casa" (5 > 3)    "x" -> ""
// Pista: DFS desde el nodo del prefijo. Como los hijos se visitan en orden
// alfabético, si solo reemplazás la mejor sugerencia cuando el valor es
// ESTRICTAMENTE mayor, el desempate sale solo.
// Complejidad: O(m + tamaño del subárbol del prefijo).
std::string sugerenciaMasFrecuente(const Trie& trie, const std::string& prefijo);

// ============================================================================
// PARTE 4 — TRIES BINARIOS
// ============================================================================

// Los tries no son solo para strings: se puede hacer un trie sobre los BITS
// de un número. Acá Σ = {0, 1}: cada nodo tiene dos hijos y cada número de
// 30 bits es un camino de largo 30 desde la raíz hasta una hoja.
//
// Este nodo es opcional: podés usarlo para el ejercicio 16 o armar tu propia
// estructura. Si usás `new`, acordate de liberar todo antes de salir.
struct NodoBit {
    NodoBit* hijo[2];   // hijo[0]: bit 0, hijo[1]: bit 1

    NodoBit() { hijo[0] = nullptr; hijo[1] = nullptr; }
};

// Ejercicio 16 — Máximo XOR
// Devuelve el máximo valor de (a ^ b) entre todos los pares de elementos
// del vector (a y b pueden ser el mismo elemento, lo que da 0).
// Ejemplo: {3, 10, 5, 25, 2, 8} -> 28   (5 ^ 25 = 28)
// Precondiciones: el vector no es vacío y 0 <= numeros[i] < 2^30.
// Idea: la fuerza bruta (probar todos los pares) es O(n²). Con un trie
// binario sale en O(n * 30): insertá cada número bit a bit (del más al menos
// significativo) y, para cada número x, recorré el trie intentando ir por
// el bit OPUESTO al de x en cada nivel (eso hace crecer el XOR); si ese
// hijo no existe, vas por el otro.
// Complejidad: O(n * B), con B = 30 bits.
int maximoXor(const std::vector<int>& numeros);

// ============================================================================
// PARTE 5 — ROSETREE (árbol n-ario)
// ============================================================================

// Un trie es un caso particular de rosetree: un nodo con una cantidad
// cualquiera de hijos. Acá vamos a la versión genérica, con un valor en
// cada nodo y un vector de punteros a los hijos (la de los apuntes).
// Se arma así:  new Rosetree<int>{5, {}}  y después  nodo->hijos.push_back(h);
template <typename T>
struct Rosetree {
    T valor;
    std::vector<Rosetree<T>*> hijos;
};

// Ejercicio 17 — Contar hojas
// Devuelve la cantidad de hojas del rosetree (nodos sin hijos). Un árbol
// vacío (nullptr) tiene 0 hojas; un único nodo es una hoja.
// Es la "pregunta para pensar" de los apuntes: cambia solo el caso base.
// Complejidad: O(n), donde n es la cantidad de nodos.
int contarHojas(Rosetree<int>* nodo);

// Ejercicio 18 — Nodos en un nivel
// Devuelve cuántos nodos hay a profundidad `nivel` (la raíz está en el
// nivel 0, sus hijos en el 1, etc.). Si el árbol no llega a ese nivel,
// devuelve 0.
// Pista: cada llamada recursiva a un hijo "baja" un nivel, así que podés
// pasar nivel - 1. Cuando nivel llega a 0, el nodo actual cuenta como 1.
// Complejidad: O(n), donde n es la cantidad de nodos.
int nodosEnNivel(Rosetree<int>* nodo, int nivel);

#endif
