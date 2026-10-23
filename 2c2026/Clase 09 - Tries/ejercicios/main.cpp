#include <iostream>
#include <string>
#include <utility>
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
// Las pruebas NO usan tus funciones para armar ni inspeccionar el trie (usan
// los helpers de abajo, que trabajan sobre los nodos a mano), así que un
// error en una función no hace fallar las pruebas de las demás.
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

using ClaveValor = std::pair<std::string, int>;

// Arma un trie directamente con `new`, sin usar tu insertar. Sirve para
// probar las demás funciones aunque insertar todavía no esté hecho.
static void cargarConValores(Trie& trie, const std::vector<ClaveValor>& claves)
{
    for (const ClaveValor& cv : claves) {
        NodoTrie* nodo = trie.raiz;
        for (char c : cv.first) {
            if (nodo->hijos[indice(c)] == nullptr) {
                nodo->hijos[indice(c)] = new NodoTrie();
            }
            nodo = nodo->hijos[indice(c)];
        }
        if (!nodo->hayClave) {
            nodo->hayClave = true;
            trie.cantidad++;
        }
        nodo->valor = cv.second;
    }
}

// Igual que cargarConValores, pero todas las claves con valor 0.
static void cargarAMano(Trie& trie, const std::vector<std::string>& claves)
{
    std::vector<ClaveValor> conValor;
    for (const std::string& c : claves) {
        conValor.push_back({c, 0});
    }
    cargarConValores(trie, conValor);
}

// Nodo al que se llega siguiendo `camino` desde la raíz (nullptr si se corta).
static NodoTrie* caminar(const Trie& trie, const std::string& camino)
{
    NodoTrie* nodo = trie.raiz;
    for (char c : camino) {
        nodo = nodo->hijos[indice(c)];
        if (nodo == nullptr) return nullptr;
    }
    return nodo;
}

// Chequeos a prueba de nullptr (así una función sin implementar hace FALLAR
// la prueba en vez de romper el programa).
static bool esClaveConValor(const Trie& trie, const std::string& camino, int valor)
{
    NodoTrie* nodo = caminar(trie, camino);
    return nodo != nullptr && nodo->hayClave && nodo->valor == valor;
}

static bool esPrefijoSinClave(const Trie& trie, const std::string& camino)
{
    NodoTrie* nodo = caminar(trie, camino);
    return nodo != nullptr && !nodo->hayClave;
}

static int cantidadHijos(const NodoTrie* nodo)
{
    int total = 0;
    for (const NodoTrie* hijo : nodo->hijos) {
        if (hijo != nullptr) total++;
    }
    return total;
}

// Cantidad real de nodos (incluida la raíz), contada a mano.
static int nodosReales(const NodoTrie* nodo)
{
    int total = 1;
    for (const NodoTrie* hijo : nodo->hijos) {
        if (hijo != nullptr) total += nodosReales(hijo);
    }
    return total;
}

// Chequea el invariante: ninguna hoja sin clave, salvo la raíz.
static bool sinRamasMuertas(const NodoTrie* nodo, bool esRaiz)
{
    if (!esRaiz && cantidadHijos(nodo) == 0 && !nodo->hayClave) return false;
    for (const NodoTrie* hijo : nodo->hijos) {
        if (hijo != nullptr && !sinRamasMuertas(hijo, false)) return false;
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
        insertar(trie, "casa", 1);
        insertar(trie, "caso", 2);
        insertar(trie, "cosa", 3);
        chequear("Ej1 - insertar (cantidad)", trie.cantidad == 3);
        chequear("Ej1 - insertar (la raíz tiene un solo hijo: 'c')",
                 cantidadHijos(trie.raiz) == 1 && trie.raiz->hijos[indice('c')] != nullptr);
        NodoTrie* c = caminar(trie, "c");
        chequear("Ej1 - insertar (comparten prefijo: 'c' tiene hijos 'a' y 'o')",
                 c != nullptr && cantidadHijos(c) == 2);
        NodoTrie* casa = caminar(trie, "casa");
        chequear("Ej1 - insertar (marca hayClave y guarda el valor)",
                 casa != nullptr && casa->hayClave && casa->valor == 1);
        NodoTrie* cosa = caminar(trie, "cosa");
        chequear("Ej1 - insertar (valor de otra clave)", cosa != nullptr && cosa->hayClave && cosa->valor == 3);
        NodoTrie* cas = caminar(trie, "cas");
        chequear("Ej1 - insertar (un prefijo no es clave)", cas != nullptr && !cas->hayClave);

        insertar(trie, "casa", 10);
        chequear("Ej1 - insertar (clave repetida reemplaza el valor)",
                 esClaveConValor(trie, "casa", 10));
        chequear("Ej1 - insertar (clave repetida no cambia la cantidad)", trie.cantidad == 3);
        chequear("Ej1 - insertar (clave repetida no crea nodos)", nodosReales(trie.raiz) == 9);

        insertar(trie, "cas", 4);
        chequear("Ej1 - insertar (clave que es prefijo de otra: cantidad)", trie.cantidad == 4);
        cas = caminar(trie, "cas");
        chequear("Ej1 - insertar (clave que es prefijo de otra: marca y valor)",
                 cas != nullptr && cas->hayClave && cas->valor == 4 && nodosReales(trie.raiz) == 9);
    }

    // ========================================================================
    // Ejercicio 2 — nodoDe
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "caso", "cosa"});
        chequear("Ej2 - nodoDe (clave completa)",
                 nodoDe(trie, "casa") != nullptr && nodoDe(trie, "casa") == caminar(trie, "casa"));
        NodoTrie* cas = nodoDe(trie, "cas");
        chequear("Ej2 - nodoDe (un prefijo existe pero no es clave)", cas != nullptr && !cas->hayClave);
        chequear("Ej2 - nodoDe (más larga que la clave)", nodoDe(trie, "casas") == nullptr);
        chequear("Ej2 - nodoDe (camino que se corta)", nodoDe(trie, "cu") == nullptr);
        chequear("Ej2 - nodoDe (letra que no está)", nodoDe(trie, "x") == nullptr);
        chequear("Ej2 - nodoDe (clave vacía: la raíz)", nodoDe(trie, "") == trie.raiz);
    }

    // ========================================================================
    // Ejercicio 3 — pertenece
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "caso", "cosa"});
        chequear("Ej3 - pertenece (clave guardada)", pertenece(trie, "casa") && pertenece(trie, "cosa"));
        chequear("Ej3 - pertenece (un prefijo NO es una clave)", !pertenece(trie, "cas"));
        chequear("Ej3 - pertenece (más larga que una clave guardada)", !pertenece(trie, "casas"));
        chequear("Ej3 - pertenece (camino que no existe)", !pertenece(trie, "perro"));
        chequear("Ej3 - pertenece (clave vacía)", !pertenece(trie, ""));
        Trie vacio;
        chequear("Ej3 - pertenece (trie vacío)", !pertenece(vacio, "a"));
    }

    // ========================================================================
    // Ejercicio 4 — obtener
    // ========================================================================
    {
        Trie trie;
        cargarConValores(trie, {{"casa", 1}, {"caso", 2}, {"cosa", 3}, {"cas", 0}});
        chequear("Ej4 - obtener (valores distintos)",
                 obtener(trie, "casa") == 1 && obtener(trie, "caso") == 2 && obtener(trie, "cosa") == 3);
        chequear("Ej4 - obtener (clave que es prefijo de otras, valor 0)", obtener(trie, "cas") == 0);
        Trie t2;
        cargarConValores(t2, {{"hola", -7}});
        chequear("Ej4 - obtener (valor negativo)", obtener(t2, "hola") == -7);
    }

    // ========================================================================
    // Ejercicio 5 — esHojaVacia
    // ========================================================================
    {
        NodoTrie nodo;
        chequear("Ej5 - esHojaVacia (nodo nuevo)", esHojaVacia(&nodo));
        nodo.hayClave = true;
        chequear("Ej5 - esHojaVacia (hoja con clave NO es vacía)", !esHojaVacia(&nodo));
        nodo.hayClave = false;
        NodoTrie* hijo = new NodoTrie();
        nodo.hijos[indice('z')] = hijo;
        chequear("Ej5 - esHojaVacia (con un hijo, aunque sea el último)", !esHojaVacia(&nodo));
        nodo.hijos[indice('z')] = nullptr;
        delete hijo;
        Trie vacio;
        chequear("Ej5 - esHojaVacia (raíz de un trie vacío)", esHojaVacia(vacio.raiz));
        Trie t;
        cargarAMano(t, {"a"});
        chequear("Ej5 - esHojaVacia (raíz con hijo)", !esHojaVacia(t.raiz));
    }

    // ========================================================================
    // Ejercicio 6 — eliminar
    // ========================================================================
    {
        Trie trie;
        cargarConValores(trie, {{"casa", 1}, {"casas", 2}, {"caso", 3}});
        // Nodos: raíz, c, a, s, a, s, o = 7
        chequear("Ej6 - eliminar (nodos iniciales de la prueba)", nodosReales(trie.raiz) == 7);

        eliminar(trie, "casas");
        chequear("Ej6 - eliminar (poda el nodo que quedó inútil)", nodosReales(trie.raiz) == 6);
        chequear("Ej6 - eliminar (cantidad)", trie.cantidad == 2);
        chequear("Ej6 - eliminar (ya no está)", caminar(trie, "casas") == nullptr);
        chequear("Ej6 - eliminar (no afecta a la clave más corta)",
                 esClaveConValor(trie, "casa", 1));

        eliminar(trie, "casa");
        // Queda: raíz, c, a, s, o. La 's' sigue porque "caso" pasa por ahí.
        chequear("Ej6 - eliminar (poda hasta el primer nodo útil, no más)", nodosReales(trie.raiz) == 5);
        chequear("Ej6 - eliminar (no toca a la otra clave)",
                 esClaveConValor(trie, "caso", 3) && trie.cantidad == 1);

        eliminar(trie, "caso");
        chequear("Ej6 - eliminar (última clave: el trie queda solo con la raíz)",
                 trie.cantidad == 0 && cantidadHijos(trie.raiz) == 0 && !trie.raiz->hayClave);
    }
    {
        // Eliminar una clave que es PREFIJO de otra: no se poda nada.
        Trie trie;
        cargarConValores(trie, {{"cas", 1}, {"casa", 2}});
        eliminar(trie, "cas");
        chequear("Ej6 - eliminar (clave que es prefijo de otra: no poda)",
                 nodosReales(trie.raiz) == 5 && esPrefijoSinClave(trie, "cas"));
        chequear("Ej6 - eliminar (la otra clave sigue con su valor)",
                 esClaveConValor(trie, "casa", 2) && trie.cantidad == 1);
    }
    {
        // Después de varios borrados no deben quedar ramas muertas.
        Trie trie;
        cargarAMano(trie, diccionario);
        eliminar(trie, "ala");
        eliminar(trie, "alto");
        eliminar(trie, "cosa");
        // Quedan: algo, casa, caso -> raíz + a,l,g,o + c,a,s,a,o = 10
        chequear("Ej6 - eliminar (sin ramas muertas tras varios borrados)",
                 sinRamasMuertas(trie) && trie.cantidad == 3 && nodosReales(trie.raiz) == 10);
    }
    {
        // Eliminar y volver a insertar: el trie sigue funcionando.
        Trie trie;
        cargarConValores(trie, {{"casa", 1}});
        eliminar(trie, "casa");
        insertar(trie, "casa", 5);
        chequear("Ej6 - eliminar (se puede volver a insertar)",
                 trie.cantidad == 1 && esClaveConValor(trie, "casa", 5));
    }

    // ========================================================================
    // Ejercicio 7 — tienePrefijo
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "caso", "cosa"});
        chequear("Ej7 - tienePrefijo (prefijo propio)", tienePrefijo(trie, "ca") && tienePrefijo(trie, "cos"));
        chequear("Ej7 - tienePrefijo (clave completa)", tienePrefijo(trie, "casa"));
        chequear("Ej7 - tienePrefijo (más larga que la clave)", !tienePrefijo(trie, "casas"));
        chequear("Ej7 - tienePrefijo (letra que no está)", !tienePrefijo(trie, "d"));
        chequear("Ej7 - tienePrefijo (camino que se corta)", !tienePrefijo(trie, "cu"));
        chequear("Ej7 - tienePrefijo (prefijo vacío en trie con claves)", tienePrefijo(trie, ""));
        Trie vacio;
        chequear("Ej7 - tienePrefijo (prefijo vacío en trie vacío)", !tienePrefijo(vacio, ""));
    }

    // ========================================================================
    // Ejercicio 8 — contarConPrefijo
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"casa", "casas", "caso", "cosa", "col"});
        chequear("Ej8 - contarConPrefijo (\"ca\")", contarConPrefijo(trie, "ca") == 3);
        chequear("Ej8 - contarConPrefijo (\"co\")", contarConPrefijo(trie, "co") == 2);
        chequear("Ej8 - contarConPrefijo (el prefijo es una clave: se cuenta)",
                 contarConPrefijo(trie, "casa") == 2);
        chequear("Ej8 - contarConPrefijo (clave sin extensiones)", contarConPrefijo(trie, "col") == 1);
        chequear("Ej8 - contarConPrefijo (prefijo que no está)", contarConPrefijo(trie, "x") == 0);
        chequear("Ej8 - contarConPrefijo (más largo que las claves)", contarConPrefijo(trie, "casaa") == 0);
        chequear("Ej8 - contarConPrefijo (prefijo vacío = total)", contarConPrefijo(trie, "") == 5);
    }

    // ========================================================================
    // Ejercicio 9 — contarNodos
    // ========================================================================
    {
        Trie vacio;
        chequear("Ej9 - contarNodos (trie vacío: solo la raíz)", contarNodos(vacio) == 1);
        Trie t1;
        cargarAMano(t1, {"casa", "caso"});
        chequear("Ej9 - contarNodos (prefijo compartido: 6, no 9)", contarNodos(t1) == 6);
        Trie t2;
        cargarAMano(t2, {"casa", "caso", "cosa"});
        chequear("Ej9 - contarNodos (tres claves)", contarNodos(t2) == 9);
        Trie t3;
        cargarAMano(t3, diccionario);
        chequear("Ej9 - contarNodos (diccionario)", contarNodos(t3) == nodosReales(t3.raiz));
    }

    // ========================================================================
    // Ejercicio 10 — listarClaves
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, diccionario);
        chequear("Ej10 - listarClaves (todas, en orden alfabético)",
                 vectoresIguales(listarClaves(trie),
                                 {"ala", "algo", "alto", "casa", "caso", "cosa"}));
        Trie vacio;
        chequear("Ej10 - listarClaves (trie vacío)", listarClaves(vacio).empty());
        Trie t2;
        cargarAMano(t2, {"casa", "ca", "c"});
        chequear("Ej10 - listarClaves (claves que son prefijo de otras)",
                 vectoresIguales(listarClaves(t2), {"c", "ca", "casa"}));
    }

    // ========================================================================
    // Ejercicio 11 — autocompletar
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, diccionario);
        chequear("Ej11 - autocompletar (\"al\")",
                 vectoresIguales(autocompletar(trie, "al", 10), {"ala", "algo", "alto"}));
        chequear("Ej11 - autocompletar (\"ca\")",
                 vectoresIguales(autocompletar(trie, "ca", 10), {"casa", "caso"}));
        chequear("Ej11 - autocompletar (límite de resultados)",
                 vectoresIguales(autocompletar(trie, "al", 2), {"ala", "algo"}));
        chequear("Ej11 - autocompletar (límite 0)", autocompletar(trie, "al", 0).empty());
        chequear("Ej11 - autocompletar (el prefijo es una clave)",
                 vectoresIguales(autocompletar(trie, "algo", 5), {"algo"}));
        chequear("Ej11 - autocompletar (prefijo que no está)", autocompletar(trie, "x", 5).empty());
        chequear("Ej11 - autocompletar (prefijo vacío + límite)",
                 vectoresIguales(autocompletar(trie, "", 4), {"ala", "algo", "alto", "casa"}));
    }

    // ========================================================================
    // Ejercicio 12 — prefijoComunMasLargo
    // ========================================================================
    {
        Trie t1;
        cargarAMano(t1, {"flower", "flow", "flight"});
        chequear("Ej12 - prefijoComunMasLargo (\"fl\")", prefijoComunMasLargo(t1) == "fl");
        Trie t2;
        cargarAMano(t2, {"dog", "racecar"});
        chequear("Ej12 - prefijoComunMasLargo (ninguno)", prefijoComunMasLargo(t2) == "");
        Trie t3;
        cargarAMano(t3, {"casa"});
        chequear("Ej12 - prefijoComunMasLargo (una sola clave: ella misma)", prefijoComunMasLargo(t3) == "casa");
        Trie t4;
        cargarAMano(t4, {"ca", "casa", "cascada"});
        chequear("Ej12 - prefijoComunMasLargo (corta en una clave)", prefijoComunMasLargo(t4) == "ca");
        Trie vacio;
        chequear("Ej12 - prefijoComunMasLargo (trie vacío)", prefijoComunMasLargo(vacio) == "");
    }

    // ========================================================================
    // Ejercicio 13 — buscarConComodin
    // ========================================================================
    {
        Trie trie;
        cargarAMano(trie, {"bad", "dad", "mad"});
        chequear("Ej13 - buscarConComodin (sin comodines, existe)", buscarConComodin(trie, "bad"));
        chequear("Ej13 - buscarConComodin (sin comodines, no existe)", !buscarConComodin(trie, "pad"));
        chequear("Ej13 - buscarConComodin (comodín al principio)", buscarConComodin(trie, ".ad"));
        chequear("Ej13 - buscarConComodin (comodines al final)", buscarConComodin(trie, "b.."));
        chequear("Ej13 - buscarConComodin (todo comodines)", buscarConComodin(trie, "..."));
        chequear("Ej13 - buscarConComodin (comodín en el medio)", buscarConComodin(trie, "m.d"));
        chequear("Ej13 - buscarConComodin (patrón más corto: es un prefijo)", !buscarConComodin(trie, "b."));
        chequear("Ej13 - buscarConComodin (patrón más largo)", !buscarConComodin(trie, "...."));
        chequear("Ej13 - buscarConComodin (el comodín no alcanza)", !buscarConComodin(trie, "x.."));
        chequear("Ej13 - buscarConComodin (patrón vacío)", !buscarConComodin(trie, ""));

        // Hay que probar TODOS los hijos: la primera rama no alcanza.
        Trie t2;
        cargarAMano(t2, {"abc", "bcd", "bxd"});
        chequear("Ej13 - buscarConComodin (backtracking entre ramas)", buscarConComodin(t2, ".xd"));
    }

    // ========================================================================
    // Ejercicio 14 — reemplazarRaices
    // ========================================================================
    {
        chequear("Ej14 - reemplazarRaices (ejemplo del enunciado)",
                 reemplazarRaices({"cat", "bat", "rat"}, "the cattle was rattled by the battery") ==
                 "the cat was rat by the bat");
        chequear("Ej14 - reemplazarRaices (gana la raíz más corta)",
                 reemplazarRaices({"a", "aa", "aaa"}, "aaaa aa b") == "a a b");
        chequear("Ej14 - reemplazarRaices (raíz más corta aunque se cargue después)",
                 reemplazarRaices({"catt", "cat", "ca"}, "cattle") == "ca");
        chequear("Ej14 - reemplazarRaices (ninguna raíz aplica)",
                 reemplazarRaices({"xyz"}, "hola mundo") == "hola mundo");
        chequear("Ej14 - reemplazarRaices (sin raíces)",
                 reemplazarRaices({}, "hola mundo") == "hola mundo");
        chequear("Ej14 - reemplazarRaices (la palabra es la raíz)",
                 reemplazarRaices({"hola"}, "hola holas") == "hola hola");
        chequear("Ej14 - reemplazarRaices (una sola palabra)",
                 reemplazarRaices({"pre"}, "preparar") == "pre");
    }

    // ========================================================================
    // Ejercicio 15 — sugerenciaMasFrecuente
    // ========================================================================
    {
        Trie trie;
        cargarConValores(trie, {{"casa", 5}, {"caso", 9}, {"cosa", 9}, {"casas", 3}});
        chequear("Ej15 - sugerenciaMasFrecuente (\"ca\")", sugerenciaMasFrecuente(trie, "ca") == "caso");
        chequear("Ej15 - sugerenciaMasFrecuente (empate: gana la menor alfabéticamente)",
                 sugerenciaMasFrecuente(trie, "c") == "caso");
        chequear("Ej15 - sugerenciaMasFrecuente (prefijo vacío)", sugerenciaMasFrecuente(trie, "") == "caso");
        chequear("Ej15 - sugerenciaMasFrecuente (el prefijo es una clave y gana)",
                 sugerenciaMasFrecuente(trie, "casa") == "casa");
        chequear("Ej15 - sugerenciaMasFrecuente (única candidata)", sugerenciaMasFrecuente(trie, "casas") == "casas");
        chequear("Ej15 - sugerenciaMasFrecuente (otra rama)", sugerenciaMasFrecuente(trie, "co") == "cosa");
        chequear("Ej15 - sugerenciaMasFrecuente (prefijo que no está)", sugerenciaMasFrecuente(trie, "x") == "");

        Trie t2;
        cargarConValores(t2, {{"a", -5}, {"b", -2}});
        chequear("Ej15 - sugerenciaMasFrecuente (valores negativos)", sugerenciaMasFrecuente(t2, "") == "b");
        Trie t3;
        cargarConValores(t3, {{"hola", 0}});
        chequear("Ej15 - sugerenciaMasFrecuente (valor 0 también es una sugerencia)",
                 sugerenciaMasFrecuente(t3, "h") == "hola");
        Trie vacio;
        chequear("Ej15 - sugerenciaMasFrecuente (trie vacío)", sugerenciaMasFrecuente(vacio, "") == "");
    }

    // ========================================================================
    // Ejercicio 16 — maximoXor
    // ========================================================================
    {
        chequear("Ej16 - maximoXor (ejemplo: 5 ^ 25 = 28)", maximoXor({3, 10, 5, 25, 2, 8}) == 28);
        chequear("Ej16 - maximoXor (otro ejemplo: 127)",
                 maximoXor({14, 70, 53, 83, 49, 91, 36, 80, 92, 51, 66, 70}) == 127);
        chequear("Ej16 - maximoXor (un solo elemento)", maximoXor({7}) == 0);
        chequear("Ej16 - maximoXor (iguales)", maximoXor({4, 4, 4}) == 0);
        chequear("Ej16 - maximoXor (8 ^ 2 = 10)", maximoXor({8, 10, 2}) == 10);
        chequear("Ej16 - maximoXor (usa los 30 bits)", maximoXor({1073741823, 0}) == 1073741823);

        // Comparación con fuerza bruta sobre números pseudo-aleatorios.
        std::vector<int> v;
        unsigned int semilla = 12345;
        for (int i = 0; i < 300; i++) {
            semilla = semilla * 1103515245u + 12345u;
            v.push_back(static_cast<int>((semilla >> 4) % 1073741824u));
        }
        chequear("Ej16 - maximoXor (coincide con fuerza bruta en 300 números)",
                 maximoXor(v) == maximoXorFuerzaBruta(v));
    }

    // ========================================================================
    // Ejercicio 17 — contarHojas
    // ========================================================================
    {
        Rosetree<int>* arbol = construirRosetreeEjemplo();
        chequear("Ej17 - contarHojas (árbol de ejemplo: 5, 6, 3 y 8)", contarHojas(arbol) == 4);
        chequear("Ej17 - contarHojas (subárbol)", contarHojas(arbol->hijos[0]) == 2);
        chequear("Ej17 - contarHojas (una hoja)", contarHojas(arbol->hijos[1]) == 1);
        chequear("Ej17 - contarHojas (árbol vacío)", contarHojas(nullptr) == 0);
        liberarRosetree(arbol);
    }

    // ========================================================================
    // Ejercicio 18 — nodosEnNivel
    // ========================================================================
    {
        Rosetree<int>* arbol = construirRosetreeEjemplo();
        chequear("Ej18 - nodosEnNivel (nivel 0: la raíz)", nodosEnNivel(arbol, 0) == 1);
        chequear("Ej18 - nodosEnNivel (nivel 1)", nodosEnNivel(arbol, 1) == 3);
        chequear("Ej18 - nodosEnNivel (nivel 2)", nodosEnNivel(arbol, 2) == 3);
        chequear("Ej18 - nodosEnNivel (nivel 3)", nodosEnNivel(arbol, 3) == 1);
        chequear("Ej18 - nodosEnNivel (nivel que no existe)", nodosEnNivel(arbol, 4) == 0);
        chequear("Ej18 - nodosEnNivel (árbol vacío)", nodosEnNivel(nullptr, 0) == 0);
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
