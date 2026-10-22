#include <iostream>
#include <string>
#include <vector>

#include "ejercicios.h"

// ============================================================================
// Driver de pruebas.
//
// NO hace falta que edites este archivo: sirve para chequear tus soluciones.
// Compilá con `make` y corré `./ejercicios`. Cada línea muestra [PASA] o
// [FALLA] según si la función correspondiente ya funciona.
//
// Al principio va a fallar casi todo (las funciones son cáscaras vacías).
// A medida que las vayas implementando en "ejercicios.cpp", van a ir pasando.
//
// Las pruebas NO usan tus funciones para inspeccionar el trie (usan los
// helpers de abajo, que recorren los nodos a mano), así que un error en una
// función no hace fallar las pruebas de las demás.
// ============================================================================

static int totalPruebas = 0;
static int pruebasOk = 0;

static void chequear(const std::string& nombre, bool condicion)
{
    totalPruebas++;
    if (condicion)
    {
        pruebasOk++;
        std::cout << "[PASA]  " << nombre << std::endl;
    }
    else
    {
        std::cout << "[FALLA] " << nombre << std::endl;
    }
}

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

// Arma un trie directamente con `new`, sin usar tu insertar. Sirve para
// probar las demás funciones aunque insertar todavía no esté hecho.
static void cargarAMano(Trie& trie, const std::vector<std::string>& palabras)
{
    for (const std::string& p : palabras) {
        NodoTrie* nodo = trie.raiz;
        for (char c : p) {
            if (nodo->hijos.find(c) == nodo->hijos.end()) {
                nodo->hijos[c] = new NodoTrie();
            }
            nodo = nodo->hijos[c];
        }
        if (!nodo->esFinal) {
            nodo->esFinal = true;
            trie.cantidad++;
        }
    }
}

// Nodo al que se llega siguiendo `camino` desde la raíz (nullptr si se corta).
static NodoTrie* nodoDe(const Trie& trie, const std::string& camino)
{
    NodoTrie* nodo = trie.raiz;
    for (char c : camino) {
        auto it = nodo->hijos.find(c);
        if (it == nodo->hijos.end()) return nullptr;
        nodo = it->second;
    }
    return nodo;
}

// Cantidad real de nodos (incluida la raíz), contada a mano.
static int nodosReales(const NodoTrie* nodo)
{
    int total = 1;
    for (const auto& par : nodo->hijos) {
        total += nodosReales(par.second);
    }
    return total;
}

// Chequea que no queden "ramas muertas": todo nodo que no sea la raíz debe
// ser final o tener hijos.
static bool sinRamasMuertas(const NodoTrie* nodo, bool esRaiz)
{
    if (!esRaiz && nodo->hijos.empty() && !nodo->esFinal) return false;
    for (const auto& par : nodo->hijos) {
        if (!sinRamasMuertas(par.second, false)) return false;
    }
    return true;
}

static bool sinRamasMuertas(const Trie& trie)
{
    return sinRamasMuertas(trie.raiz, true);
}

static bool vectoresIguales(const std::vector<std::string>& a, const std::vector<std::string>& b)
{
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Árbol de ejemplo para el Rosetree:
//            1
//         /  |  \
//        2   3   4
//       / \      |
//      5   6     7
//                |
//                8
static Rosetree<int>* nuevoNodo(int valor)
{
    return new Rosetree<int>{valor, {}};
}

static Rosetree<int>* construirRosetreeEjemplo()
{
    Rosetree<int>* raiz = nuevoNodo(1);
    Rosetree<int>* n2 = nuevoNodo(2);
    Rosetree<int>* n3 = nuevoNodo(3);
    Rosetree<int>* n4 = nuevoNodo(4);
    raiz->hijos = {n2, n3, n4};
    n2->hijos = {nuevoNodo(5), nuevoNodo(6)};
    Rosetree<int>* n7 = nuevoNodo(7);
    n4->hijos = {n7};
    n7->hijos = {nuevoNodo(8)};
    return raiz;
}

static void liberarRosetree(Rosetree<int>* nodo)
{
    if (nodo == nullptr) return;
    for (Rosetree<int>* hijo : nodo->hijos) {
        liberarRosetree(hijo);
    }
    delete nodo;
}

// Fuerza bruta O(n²) para comparar con maximoXor.
static int maximoXorFuerzaBruta(const std::vector<int>& v)
{
    int mejor = 0;
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = 0; j < v.size(); j++) {
            if ((v[i] ^ v[j]) > mejor) mejor = v[i] ^ v[j];
        }
    }
    return mejor;
}

int main()
{
    // Diccionario que se reutiliza en varias pruebas.
    const std::vector<std::string> diccionario = {"cosa", "casa", "ala", "algo", "caso", "alto"};

    // ========================================================================
    // Ejercicio 1 — insertar
    // ========================================================================
    {
        Trie trie;
        insertar(trie, "casa");
        insertar(trie, "caso");
        insertar(trie, "cosa");
        chequear("Ej1 - insertar (cantidad)", trie.cantidad == 3);
        chequear("Ej1 - insertar (la raíz tiene un solo hijo: 'c')",
                 trie.raiz->hijos.size() == 1 && trie.raiz->hijos.count('c') == 1);
        NodoTrie* c = nodoDe(trie, "c");
        chequear("Ej1 - insertar (comparten prefijo: 'c' tiene hijos 'a' y 'o')",
                 c != nullptr && c->hijos.size() == 2);
        NodoTrie* casa = nodoDe(trie, "casa");
        chequear("Ej1 - insertar (marca el final de la palabra)", casa != nullptr && casa->esFinal);
        NodoTrie* cas = nodoDe(trie, "cas");
        chequear("Ej1 - insertar (un prefijo no es final)", cas != nullptr && !cas->esFinal);

        insertar(trie, "casa");
        chequear("Ej1 - insertar (palabra repetida no cambia la cantidad)", trie.cantidad == 3);
        chequear("Ej1 - insertar (palabra repetida no crea nodos)", nodosReales(trie.raiz) == 9);

        insertar(trie, "cas");
        chequear("Ej1 - insertar (palabra que es prefijo de otra: cantidad)", trie.cantidad == 4);
        cas = nodoDe(trie, "cas");
        chequear("Ej1 - insertar (palabra que es prefijo de otra: marca final)",
                 cas != nullptr && cas->esFinal && nodosReales(trie.raiz) == 9);
    }

    // ========================================================================
    // Ejercicio 2 — contiene
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "caso", "cosa"});
        chequear("Ej2 - contiene (palabra guardada)", contiene(trie, "casa") && contiene(trie, "cosa"));
        chequear("Ej2 - contiene (un prefijo NO es una palabra)", !contiene(trie, "cas"));
        chequear("Ej2 - contiene (palabra más larga que una guardada)", !contiene(trie, "casas"));
        chequear("Ej2 - contiene (camino que no existe)", !contiene(trie, "perro"));
        chequear("Ej2 - contiene (palabra vacía)", !contiene(trie, ""));
        Trie vacio;
        chequear("Ej2 - contiene (trie vacío)", !contiene(vacio, "a"));
    }

    // ========================================================================
    // Ejercicio 3 — tienePrefijo
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "caso", "cosa"});
        chequear("Ej3 - tienePrefijo (prefijo propio)", tienePrefijo(trie, "ca") && tienePrefijo(trie, "cos"));
        chequear("Ej3 - tienePrefijo (palabra completa)", tienePrefijo(trie, "casa"));
        chequear("Ej3 - tienePrefijo (más larga que la palabra)", !tienePrefijo(trie, "casas"));
        chequear("Ej3 - tienePrefijo (letra que no está)", !tienePrefijo(trie, "d"));
        chequear("Ej3 - tienePrefijo (camino que se corta)", !tienePrefijo(trie, "cu"));
        chequear("Ej3 - tienePrefijo (prefijo vacío en trie con palabras)", tienePrefijo(trie, ""));
        Trie vacio;
        chequear("Ej3 - tienePrefijo (prefijo vacío en trie vacío)", !tienePrefijo(vacio, ""));
    }

    // ========================================================================
    // Ejercicio 4 — contarConPrefijo
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "casas", "caso", "cosa", "col"});
        chequear("Ej4 - contarConPrefijo (\"ca\")", contarConPrefijo(trie, "ca") == 3);
        chequear("Ej4 - contarConPrefijo (\"co\")", contarConPrefijo(trie, "co") == 2);
        chequear("Ej4 - contarConPrefijo (el prefijo es una palabra: se cuenta)",
                 contarConPrefijo(trie, "casa") == 2);
        chequear("Ej4 - contarConPrefijo (palabra sin extensiones)", contarConPrefijo(trie, "col") == 1);
        chequear("Ej4 - contarConPrefijo (prefijo que no está)", contarConPrefijo(trie, "x") == 0);
        chequear("Ej4 - contarConPrefijo (más largo que las palabras)", contarConPrefijo(trie, "casaa") == 0);
        chequear("Ej4 - contarConPrefijo (prefijo vacío = total)", contarConPrefijo(trie, "") == 5);
    }

    // ========================================================================
    // Ejercicio 5 — borrar
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "casas", "caso"});
        // Nodos: raíz, c, a, s, a, s, o = 7
        chequear("Ej5 - borrar (nodos iniciales de la prueba)", nodosReales(trie.raiz) == 7);

        chequear("Ej5 - borrar (palabra existente)", borrar(trie, "casas"));
        chequear("Ej5 - borrar (poda el nodo que quedó inútil)", nodosReales(trie.raiz) == 6);
        chequear("Ej5 - borrar (cantidad)", trie.cantidad == 2);
        chequear("Ej5 - borrar (ya no está)", nodoDe(trie, "casas") == nullptr);
        chequear("Ej5 - borrar (no afecta a la palabra más corta)",
                 nodoDe(trie, "casa") != nullptr && nodoDe(trie, "casa")->esFinal);

        chequear("Ej5 - borrar (palabra que no existe)", !borrar(trie, "perro") && trie.cantidad == 2);
        chequear("Ej5 - borrar (un prefijo que no es palabra)",
                 !borrar(trie, "ca") && trie.cantidad == 2 && nodosReales(trie.raiz) == 6);

        chequear("Ej5 - borrar (poda hasta el primer nodo útil)", borrar(trie, "casa"));
        // Queda: raíz, c, a, s, o. La 's' sigue porque "caso" pasa por ahí.
        chequear("Ej5 - borrar (no poda nodos que usa otra palabra)",
                 nodosReales(trie.raiz) == 5 && nodoDe(trie, "caso") != nullptr && nodoDe(trie, "caso")->esFinal);
        chequear("Ej5 - borrar (borrar dos veces devuelve false)", !borrar(trie, "casa"));

        chequear("Ej5 - borrar (última palabra)", borrar(trie, "caso"));
        chequear("Ej5 - borrar (el trie queda solo con la raíz)",
                 trie.cantidad == 0 && trie.raiz->hijos.empty() && !trie.raiz->esFinal);
    }
    {
        // Borrar una palabra que es PREFIJO de otra: no se poda nada.
        Trie trie;
        cargarAMano(trie, {"cas", "casa"});
        chequear("Ej5 - borrar (palabra que es prefijo de otra)", borrar(trie, "cas"));
        chequear("Ej5 - borrar (solo desmarca esFinal, no poda)",
                 nodosReales(trie.raiz) == 5 && nodoDe(trie, "cas") != nullptr && !nodoDe(trie, "cas")->esFinal);
        chequear("Ej5 - borrar (la otra palabra sigue)", nodoDe(trie, "casa")->esFinal && trie.cantidad == 1);
    }
    {
        // Después de varios borrados no deben quedar ramas muertas.
        Trie trie;
        cargarAMano(trie, diccionario);
        borrar(trie, "ala");
        borrar(trie, "alto");
        borrar(trie, "cosa");
        chequear("Ej5 - borrar (sin ramas muertas tras varios borrados)",
                 sinRamasMuertas(trie) && trie.cantidad == 3 && nodosReales(trie.raiz) == 1 + 4 + 5);
    }

    // ========================================================================
    // Ejercicio 6 — contarNodos
    // ========================================================================
    {
        Trie vacio;
        chequear("Ej6 - contarNodos (trie vacío: solo la raíz)", contarNodos(vacio) == 1);
        Trie t1;
        cargarAMano(t1, {"casa", "caso"});
        chequear("Ej6 - contarNodos (prefijo compartido: 6, no 9)", contarNodos(t1) == 6);
        Trie t2;
        cargarAMano(t2, {"casa", "caso", "cosa"});
        chequear("Ej6 - contarNodos (tres palabras)", contarNodos(t2) == 9);
        Trie t3;
        cargarAMano(t3, diccionario);
        chequear("Ej6 - contarNodos (diccionario)", contarNodos(t3) == nodosReales(t3.raiz));
    }

    // ========================================================================
    // Ejercicio 7 — listarPalabras
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, diccionario);
        chequear("Ej7 - listarPalabras (todas, en orden alfabético)",
                 vectoresIguales(listarPalabras(trie),
                                 {"ala", "algo", "alto", "casa", "caso", "cosa"}));
        Trie vacio;
        chequear("Ej7 - listarPalabras (trie vacío)", listarPalabras(vacio).empty());
        Trie t2;
        cargarAMano(t2, {"casa", "ca", "c"});
        chequear("Ej7 - listarPalabras (palabras que son prefijo de otras)",
                 vectoresIguales(listarPalabras(t2), {"c", "ca", "casa"}));
    }

    // ========================================================================
    // Ejercicio 8 — autocompletar
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, diccionario);
        chequear("Ej8 - autocompletar (\"al\")",
                 vectoresIguales(autocompletar(trie, "al", 10), {"ala", "algo", "alto"}));
        chequear("Ej8 - autocompletar (\"ca\")",
                 vectoresIguales(autocompletar(trie, "ca", 10), {"casa", "caso"}));
        chequear("Ej8 - autocompletar (límite de resultados)",
                 vectoresIguales(autocompletar(trie, "al", 2), {"ala", "algo"}));
        chequear("Ej8 - autocompletar (límite 0)", autocompletar(trie, "al", 0).empty());
        chequear("Ej8 - autocompletar (el prefijo es una palabra)",
                 vectoresIguales(autocompletar(trie, "algo", 5), {"algo"}));
        chequear("Ej8 - autocompletar (prefijo que no está)", autocompletar(trie, "x", 5).empty());
        chequear("Ej8 - autocompletar (prefijo vacío + límite)",
                 vectoresIguales(autocompletar(trie, "", 4), {"ala", "algo", "alto", "casa"}));
    }

    // ========================================================================
    // Ejercicio 9 — prefijoComunMasLargo
    // ========================================================================
    {
        Trie t1;
        cargarAMano(t1, {"flower", "flow", "flight"});
        chequear("Ej9 - prefijoComunMasLargo (\"fl\")", prefijoComunMasLargo(t1) == "fl");
        Trie t2;
        cargarAMano(t2, {"dog", "racecar"});
        chequear("Ej9 - prefijoComunMasLargo (ninguno)", prefijoComunMasLargo(t2) == "");
        Trie t3;
        cargarAMano(t3, {"casa"});
        chequear("Ej9 - prefijoComunMasLargo (una sola palabra: ella misma)", prefijoComunMasLargo(t3) == "casa");
        Trie t4;
        cargarAMano(t4, {"ca", "casa", "cascada"});
        chequear("Ej9 - prefijoComunMasLargo (corta en una palabra final)", prefijoComunMasLargo(t4) == "ca");
        Trie vacio;
        chequear("Ej9 - prefijoComunMasLargo (trie vacío)", prefijoComunMasLargo(vacio) == "");
    }

    // ========================================================================
    // Ejercicio 10 — buscarConComodin
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"bad", "dad", "mad"});
        chequear("Ej10 - buscarConComodin (sin comodines, existe)", buscarConComodin(trie, "bad"));
        chequear("Ej10 - buscarConComodin (sin comodines, no existe)", !buscarConComodin(trie, "pad"));
        chequear("Ej10 - buscarConComodin (comodín al principio)", buscarConComodin(trie, ".ad"));
        chequear("Ej10 - buscarConComodin (comodines al final)", buscarConComodin(trie, "b.."));
        chequear("Ej10 - buscarConComodin (todo comodines)", buscarConComodin(trie, "..."));
        chequear("Ej10 - buscarConComodin (comodín en el medio)", buscarConComodin(trie, "m.d"));
        chequear("Ej10 - buscarConComodin (patrón más corto: es un prefijo)", !buscarConComodin(trie, "b."));
        chequear("Ej10 - buscarConComodin (patrón más largo)", !buscarConComodin(trie, "...."));
        chequear("Ej10 - buscarConComodin (el comodín no alcanza)", !buscarConComodin(trie, "x.."));
        chequear("Ej10 - buscarConComodin (patrón vacío)", !buscarConComodin(trie, ""));

        // Hay que probar TODOS los hijos: la primera rama no alcanza.
        Trie t2;
        cargarAMano(t2, {"abc", "bcd", "bxd"});
        chequear("Ej10 - buscarConComodin (backtracking entre ramas)", buscarConComodin(t2, ".xd"));
    }

    // ========================================================================
    // Ejercicio 11 — reemplazarRaices
    // ========================================================================
    {
        chequear("Ej11 - reemplazarRaices (ejemplo del enunciado)",
                 reemplazarRaices({"cat", "bat", "rat"}, "the cattle was rattled by the battery") ==
                 "the cat was rat by the bat");
        chequear("Ej11 - reemplazarRaices (gana la raíz más corta)",
                 reemplazarRaices({"a", "aa", "aaa"}, "aaaa aa b") == "a a b");
        chequear("Ej11 - reemplazarRaices (raíz más corta aunque se cargue después)",
                 reemplazarRaices({"catt", "cat", "ca"}, "cattle") == "ca");
        chequear("Ej11 - reemplazarRaices (ninguna raíz aplica)",
                 reemplazarRaices({"xyz"}, "hola mundo") == "hola mundo");
        chequear("Ej11 - reemplazarRaices (sin raíces)",
                 reemplazarRaices({}, "hola mundo") == "hola mundo");
        chequear("Ej11 - reemplazarRaices (la palabra es la raíz)",
                 reemplazarRaices({"hola"}, "hola holas") == "hola hola");
        chequear("Ej11 - reemplazarRaices (una sola palabra)",
                 reemplazarRaices({"pre"}, "preparar") == "pre");
    }

    // ========================================================================
    // Ejercicio 12 — palabraMasLargaConstruible
    // ========================================================================
    {
        chequear("Ej12 - palabraMasLargaConstruible (\"world\")",
                 palabraMasLargaConstruible({"w", "wo", "wor", "worl", "world"}) == "world");
        chequear("Ej12 - palabraMasLargaConstruible (desempate alfabético)",
                 palabraMasLargaConstruible({"a", "banana", "app", "appl", "ap", "apply", "apple"}) == "apple");
        chequear("Ej12 - palabraMasLargaConstruible (desempate, otro orden)",
                 palabraMasLargaConstruible({"b", "ba", "a", "ab"}) == "ab");
        chequear("Ej12 - palabraMasLargaConstruible (falta un prefijo intermedio)",
                 palabraMasLargaConstruible({"c", "cas", "casa"}) == "c");
        chequear("Ej12 - palabraMasLargaConstruible (ninguna cumple)",
                 palabraMasLargaConstruible({"banana", "casa"}) == "");
        chequear("Ej12 - palabraMasLargaConstruible (lista vacía)",
                 palabraMasLargaConstruible({}) == "");
        chequear("Ej12 - palabraMasLargaConstruible (palabras repetidas)",
                 palabraMasLargaConstruible({"a", "a", "ab", "ab"}) == "ab");
    }

    // ========================================================================
    // Ejercicio 13 — maximoXor
    // ========================================================================
    {
        chequear("Ej13 - maximoXor (ejemplo: 5 ^ 25 = 28)", maximoXor({3, 10, 5, 25, 2, 8}) == 28);
        chequear("Ej13 - maximoXor (otro ejemplo: 127)",
                 maximoXor({14, 70, 53, 83, 49, 91, 36, 80, 92, 51, 66, 70}) == 127);
        chequear("Ej13 - maximoXor (un solo elemento)", maximoXor({7}) == 0);
        chequear("Ej13 - maximoXor (iguales)", maximoXor({4, 4, 4}) == 0);
        chequear("Ej13 - maximoXor (8 ^ 2 = 10)", maximoXor({8, 10, 2}) == 10);
        chequear("Ej13 - maximoXor (usa los 30 bits)", maximoXor({1073741823, 0}) == 1073741823);

        // Comparación con fuerza bruta sobre números pseudo-aleatorios.
        std::vector<int> v;
        unsigned int semilla = 12345;
        for (int i = 0; i < 300; i++) {
            semilla = semilla * 1103515245u + 12345u;
            v.push_back(static_cast<int>((semilla >> 4) % 1073741824u));
        }
        chequear("Ej13 - maximoXor (coincide con fuerza bruta en 300 números)",
                 maximoXor(v) == maximoXorFuerzaBruta(v));
    }

    // ========================================================================
    // Ejercicio 14 — contarHojas
    // ========================================================================
    {
        Rosetree<int>* arbol = construirRosetreeEjemplo();
        chequear("Ej14 - contarHojas (árbol de ejemplo: 5, 6, 3 y 8)", contarHojas(arbol) == 4);
        chequear("Ej14 - contarHojas (subárbol)", contarHojas(arbol->hijos[0]) == 2);
        chequear("Ej14 - contarHojas (una hoja)", contarHojas(arbol->hijos[1]) == 1);
        chequear("Ej14 - contarHojas (árbol vacío)", contarHojas(nullptr) == 0);
        liberarRosetree(arbol);
    }

    // ========================================================================
    // Ejercicio 15 — nodosEnNivel
    // ========================================================================
    {
        Rosetree<int>* arbol = construirRosetreeEjemplo();
        chequear("Ej15 - nodosEnNivel (nivel 0: la raíz)", nodosEnNivel(arbol, 0) == 1);
        chequear("Ej15 - nodosEnNivel (nivel 1)", nodosEnNivel(arbol, 1) == 3);
        chequear("Ej15 - nodosEnNivel (nivel 2)", nodosEnNivel(arbol, 2) == 3);
        chequear("Ej15 - nodosEnNivel (nivel 3)", nodosEnNivel(arbol, 3) == 1);
        chequear("Ej15 - nodosEnNivel (nivel que no existe)", nodosEnNivel(arbol, 4) == 0);
        chequear("Ej15 - nodosEnNivel (árbol vacío)", nodosEnNivel(nullptr, 0) == 0);
        liberarRosetree(arbol);
    }

    // ========================================================================
    // Resumen
    // ========================================================================
    std::cout << "\nResultado: " << pruebasOk << "/" << totalPruebas
              << " pruebas pasadas." << std::endl;

    if (pruebasOk == totalPruebas)
    {
        std::cout << "\nPara verificar que no hay memory leaks, corré: make valgrind" << std::endl;
    }

    return 0;
}
