#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <map>
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
// ============================================================================

// ============================================================================
// ESTRUCTURA DEL TRIE
// ============================================================================

// Nodo de un trie. No guarda su letra: la letra es la ETIQUETA de la arista
// que lo une con su padre (la clave del map del padre).
//   - hijos:   letra -> nodo hijo. Al ser un std::map, los hijos se recorren
//              siempre en orden alfabético.
//   - esFinal: true si el camino desde la raíz hasta este nodo forma una
//              palabra completa guardada en el trie (y no solo un prefijo).
struct NodoTrie {
    std::map<char, NodoTrie*> hijos;
    bool esFinal;

    NodoTrie() : esFinal(false) {}
};

// Libera un nodo y todo su subárbol (postorder: primero los hijos).
inline void liberarNodoTrie(NodoTrie* nodo)
{
    if (nodo == nullptr) return;
    for (auto& par : nodo->hijos) {
        liberarNodoTrie(par.second);
    }
    delete nodo;
}

// El trie en sí: un puntero a la raíz (que SIEMPRE existe y representa el
// prefijo vacío) y la cantidad de palabras guardadas.
// El destructor ya está resuelto: libera todos los nodos, así Valgrind no
// se queja mientras te concentrás en las operaciones.
struct Trie {
    NodoTrie* raiz;
    int cantidad;   // cantidad de palabras distintas guardadas

    Trie() : raiz(new NodoTrie()), cantidad(0) {}
    ~Trie() { liberarNodoTrie(raiz); }

    // Un Trie es dueño de sus nodos: copiarlo duplicaría punteros.
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
};

// ============================================================================
// PARTE 1 — OPERACIONES BÁSICAS
// ============================================================================

// Ejercicio 1 — Insertar
// Agrega la palabra al trie, creando los nodos que falten, y marca como
// final el último nodo. Si la palabra ya estaba, no hace nada (no se
// duplica ni cambia `cantidad`).
// Precondición: la palabra no es vacía.
// Complejidad: O(k), donde k es el largo de la palabra (no depende de
// cuántas palabras haya en el trie).
void insertar(Trie& trie, const std::string& palabra);

// Ejercicio 2 — Contiene
// Devuelve true si la palabra COMPLETA está guardada en el trie.
// Ojo: si el trie tiene "casa", contiene("cas") es false (es un prefijo,
// no una palabra guardada). Alcanza con recorrer el camino y mirar esFinal.
// La palabra vacía nunca está guardada.
// Complejidad: O(k).
bool contiene(const Trie& trie, const std::string& palabra);

// Ejercicio 3 — Tiene prefijo
// Devuelve true si alguna palabra del trie empieza con `prefijo`.
// Si el prefijo es vacío, devuelve true solo si el trie no está vacío.
// Ojo: toda palabra guardada es prefijo de sí misma.
// Pista: contiene y tienePrefijo comparten el recorrido. Conviene escribir
// una función auxiliar que devuelva el nodo al que se llega siguiendo el
// camino (o nullptr si el camino se corta).
// Complejidad: O(k).
bool tienePrefijo(const Trie& trie, const std::string& prefijo);

// Ejercicio 4 — Contar con prefijo
// Devuelve cuántas palabras guardadas empiezan con `prefijo` (incluida la
// propia palabra `prefijo`, si está). Con prefijo vacío devuelve el total.
// Pista: llegá hasta el nodo del prefijo y contá cuántos nodos esFinal hay
// en su subárbol (incluido él mismo).
// Complejidad: O(k + tamaño del subárbol).
int contarConPrefijo(const Trie& trie, const std::string& prefijo);

// Ejercicio 5 — Borrar
// Borra la palabra del trie. Devuelve true si estaba, false si no.
// Además de desmarcar esFinal, hay que PODAR: eliminar (con delete) los
// nodos que quedaron sin hijos y que no marcan el final de otra palabra,
// subiendo desde la hoja. Si no, el trie se queda con "ramas muertas".
//   - Borrar "casa" cuando también existe "casas" NO borra nodos
//     (solo desmarca esFinal en el nodo de la 'a').
//   - Borrar "casas" cuando existe "casa" borra únicamente el nodo de la 's'.
// Pista: una función recursiva que devuelva "este nodo quedó inútil, que
// el padre lo borre" resuelve la poda de forma natural.
// Complejidad: O(k).
bool borrar(Trie& trie, const std::string& palabra);

// ============================================================================
// PARTE 2 — RECORRIDOS (DFS sobre el trie)
// ============================================================================

// Ejercicio 6 — Contar nodos
// Devuelve la cantidad de nodos del trie, INCLUYENDO la raíz. Un trie vacío
// tiene 1 nodo (la raíz). Sirve para medir cuánta memoria usa el trie y
// cuánto ahorran los prefijos compartidos: guardar "casa" y "caso" ocupa
// 1 + 4 + 1 = 6 nodos, no 9.
// Complejidad: O(n), donde n es la cantidad de nodos.
int contarNodos(const Trie& trie);

// Ejercicio 7 — Listar palabras
// Devuelve TODAS las palabras guardadas, en orden alfabético.
// Pista: DFS llevando un std::string con el camino actual. Al bajar a un
// hijo agregás su letra (push_back); al volver, la sacás (pop_back). Si el
// nodo actual es esFinal, el camino actual es una palabra.
// Complejidad: O(L), donde L es la suma de los largos de todas las palabras.
std::vector<std::string> listarPalabras(const Trie& trie);

// Ejercicio 8 — Autocompletar
// Devuelve las palabras que empiezan con `prefijo`, en orden alfabético,
// pero a lo sumo `maxResultados` (como hacen los buscadores: sugieren unas
// pocas opciones, no todas). Si el prefijo no está, devuelve un vector vacío.
// Precondición: maxResultados >= 0.
// Pista: llegá al nodo del prefijo y hacé el DFS desde ahí, arrancando con
// el camino actual igual a `prefijo`. Cortá el recorrido en cuanto juntes
// maxResultados palabras: no hace falta seguir.
// Complejidad: O(k + tamaño de lo recorrido).
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo, int maxResultados);

// Ejercicio 9 — Prefijo común más largo
// Devuelve el prefijo común más largo de TODAS las palabras del trie
// (cadena vacía si no hay ninguno, o si el trie está vacío).
// Ejemplos: {"flower", "flow", "flight"} -> "fl"
//           {"dog", "racecar"}           -> ""
//           {"casa"}                     -> "casa"
//           {"ca", "casa", "cascada"}    -> "ca"
// Pista: bajá desde la raíz mientras el nodo tenga EXACTAMENTE un hijo y no
// sea final (si fuera final, esa palabra se termina ahí y no se puede seguir).
// Complejidad: O(largo del prefijo resultante).
std::string prefijoComunMasLargo(const Trie& trie);

// ============================================================================
// PARTE 3 — PROBLEMAS CON TRIES
// ============================================================================

// Ejercicio 10 — Buscar con comodín
// Devuelve true si alguna palabra del trie coincide EXACTAMENTE con el
// patrón, donde el carácter '.' puede reemplazar a cualquier letra.
// Ejemplos con {"bad", "dad", "mad"}:
//   "bad" -> true     ".ad" -> true     "b.." -> true     "..." -> true
//   "pad" -> false    "b."  -> false    "...." -> false
// Pista: recursión sobre (nodo, posición en el patrón). Si la letra es
// '.', probás TODOS los hijos del nodo; si no, seguís por un único hijo.
// Complejidad: O(k) sin comodines; en el peor caso, O(tamaño del trie).
bool buscarConComodin(const Trie& trie, const std::string& patron);

// Ejercicio 11 — Reemplazar por raíces
// En inglés (y en español) muchas palabras derivan de una raíz: "cat" ->
// "cattle", "catalog". Dado un diccionario de raíces y una oración (palabras
// separadas por UN espacio), reemplazá cada palabra por la raíz MÁS CORTA
// del diccionario que sea prefijo de ella. Si ninguna raíz es prefijo, la
// palabra queda igual.
// Ejemplo: raíces {"cat", "bat", "rat"}
//   "the cattle was rattled by the battery"
//   -> "the cat was rat by the bat"
// Pista: armá un Trie con las raíces. Para cada palabra bajá por el trie
// letra por letra y cortá en cuanto pases por un nodo esFinal (la primera
// que encuentres es la más corta).
// Complejidad: O(L), con L la cantidad de caracteres de la oración (más lo
// que cueste armar el trie).
std::string reemplazarRaices(const std::vector<std::string>& raices, const std::string& oracion);

// Ejercicio 12 — Palabra más larga construible
// Dada una lista de palabras, devolvé la palabra más larga que se pueda
// "construir letra por letra": que ella y TODOS sus prefijos (de largo 1 en
// adelante) estén en la lista. Si hay empate de largo, devolvé la menor en
// orden alfabético. Si ninguna cumple, devolvé "".
// Ejemplos:
//   {"w", "wo", "wor", "worl", "world"}                 -> "world"
//   {"a", "banana", "app", "appl", "ap", "apply", "apple"} -> "apple"
//     ("apply" empata en largo con "apple", pero "apple" es alfabéticamente
//      menor; "banana" no cuenta porque "b" no está en la lista)
// Pista: insertá todo en un Trie y hacé un DFS que SOLO avance por nodos
// esFinal. Como el map recorre las letras en orden alfabético, si te
// quedás con la primera palabra de cada largo ya resolvés el desempate.
// Complejidad: O(L), con L la suma de los largos de las palabras.
std::string palabraMasLargaConstruible(const std::vector<std::string>& palabras);

// ============================================================================
// PARTE 4 — TRIES BINARIOS
// ============================================================================

// Los tries no son solo para strings: se puede hacer un trie sobre los BITS
// de un número. Cada nodo tiene dos hijos (bit 0 y bit 1) y cada número de
// 31 bits es un camino de largo 31 desde la raíz hasta una hoja.
//
// Este nodo es opcional: podés usarlo para el ejercicio 13 o armar tu propia
// estructura. Si usás `new`, acordate de liberar todo antes de salir.
struct NodoBit {
    NodoBit* hijo[2];   // hijo[0]: bit 0, hijo[1]: bit 1

    NodoBit() { hijo[0] = nullptr; hijo[1] = nullptr; }
};

// Ejercicio 13 — Máximo XOR
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

// Ejercicio 14 — Contar hojas
// Devuelve la cantidad de hojas del rosetree (nodos sin hijos). Un árbol
// vacío (nullptr) tiene 0 hojas; un único nodo es una hoja.
// Es la "pregunta para pensar" de los apuntes: cambia solo el caso base.
// Complejidad: O(n), donde n es la cantidad de nodos.
int contarHojas(Rosetree<int>* nodo);

// Ejercicio 15 — Nodos en un nivel
// Devuelve cuántos nodos hay a profundidad `nivel` (la raíz está en el
// nivel 0, sus hijos en el 1, etc.). Si el árbol no llega a ese nivel,
// devuelve 0.
// Pista: cada llamada recursiva a un hijo "baja" un nivel, así que podés
// pasar nivel - 1. Cuando nivel llega a 0, el nodo actual cuenta como 1.
// Complejidad: O(n), donde n es la cantidad de nodos.
int nodosEnNivel(Rosetree<int>* nodo, int nivel);

#endif
