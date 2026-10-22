#include "ejercicios.h"

// ============================================================================
// Acá va TU código. Cada función tiene una cáscara con un "TODO": borrá el
// contenido de ejemplo y escribí la implementación.
//
// Los `(void)parametro;` que ves abajo están solo para que la cáscara
// compile sin advertencias antes de que la implementes. Cuando uses el
// parámetro en tu código, borrá la línea `(void)...` correspondiente.
//
// Podés agregar funciones auxiliares (por ejemplo, recursivas) arriba de la
// función que las usa. Si querés que sean privadas del archivo, declaralas
// `static`.
// ============================================================================

// ============================================================================
// PARTE 1 — OPERACIONES BÁSICAS
// ============================================================================

// Ejercicio 1 — Insertar
void insertar(Trie& trie, const std::string& palabra)
{
    // TODO: implementar insertar.
    // Recordá:
    //   - Arrancá en trie.raiz y recorré la palabra letra por letra.
    //   - Si el nodo actual no tiene hijo para esa letra, creálo con `new`
    //     (nodo->hijos[letra] = new NodoTrie();).
    //   - Avanzá al hijo.
    //   - Al terminar, marcá el nodo como final. Si NO era final todavía,
    //     es una palabra nueva: incrementá trie.cantidad.
    (void)trie;
    (void)palabra;
}

// Ejercicio 2 — Contiene
bool contiene(const Trie& trie, const std::string& palabra)
{
    // TODO: implementar contiene.
    // Recordá:
    //   - Recorré el camino letra por letra. Si en algún paso no existe el
    //     hijo (nodo->hijos.find(letra) == nodo->hijos.end()), la palabra no está.
    //   - Si llegaste al final del camino, la palabra está solo si el nodo es esFinal.
    //   - La palabra vacía nunca está guardada.
    (void)trie;
    (void)palabra;
    return false;
}

// Ejercicio 3 — Tiene prefijo
bool tienePrefijo(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar tienePrefijo.
    // Recordá:
    //   - Es casi igual a contiene, pero NO mira esFinal: alcanza con que el
    //     camino exista completo.
    //   - Prefijo vacío: true solo si el trie tiene alguna palabra
    //     (trie.cantidad > 0).
    // Tip: acá conviene escribir (arriba de esta función) un auxiliar
    //   static NodoTrie* buscarNodo(const Trie& trie, const std::string& s)
    // que devuelva el nodo al que se llega con `s` (o nullptr), y reusarlo
    // en contiene, contarConPrefijo y autocompletar.
    (void)trie;
    (void)prefijo;
    return false;
}

// Ejercicio 4 — Contar con prefijo
int contarConPrefijo(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar contarConPrefijo.
    // Recordá:
    //   - Llegá hasta el nodo del prefijo. Si el camino se corta, hay 0 palabras.
    //   - Desde ese nodo, contá los nodos esFinal de su subárbol (incluido
    //     él) con una función recursiva auxiliar:
    //       cuenta = (nodo->esFinal ? 1 : 0) + suma de cuenta(hijo) para cada hijo
    (void)trie;
    (void)prefijo;
    return 0;
}

// Ejercicio 5 — Borrar
bool borrar(Trie& trie, const std::string& palabra)
{
    // TODO: implementar borrar.
    // Recordá:
    //   - Primero verificá que la palabra esté (contiene). Si no, devolvé false.
    //   - Escribí una función recursiva auxiliar que baje por el camino y
    //     devuelva `true` si el nodo quedó INÚTIL (sin hijos y no final).
    //       * Caso base (llegaste al final de la palabra): esFinal = false.
    //       * Caso recursivo: llamá para el hijo; si devolvió true, hacé
    //         delete del hijo y sacalo del map (hijos.erase(letra)).
    //       * Devolvé: ¿este nodo quedó sin hijos y no es final?
    //   - La raíz nunca se borra, aunque quede vacía.
    //   - No te olvides de decrementar trie.cantidad.
    (void)trie;
    (void)palabra;
    return false;
}

// ============================================================================
// PARTE 2 — RECORRIDOS (DFS sobre el trie)
// ============================================================================

// Ejercicio 6 — Contar nodos
int contarNodos(const Trie& trie)
{
    // TODO: implementar contarNodos.
    // Recordá:
    //   - Es igual que contar nodos en un rosetree: 1 (el nodo actual) más la
    //     suma de contar cada hijo. Usá una función auxiliar recursiva que
    //     reciba un NodoTrie*.
    (void)trie;
    return 0;
}

// Ejercicio 7 — Listar palabras
std::vector<std::string> listarPalabras(const Trie& trie)
{
    // TODO: implementar listarPalabras.
    // Recordá:
    //   - Auxiliar recursiva: void recolectar(NodoTrie* nodo, std::string& camino,
    //                                         std::vector<std::string>& resultado)
    //   - Si el nodo es esFinal, agregá `camino` al resultado.
    //   - Para cada (letra, hijo) del map: camino.push_back(letra);
    //     recolectar(hijo, ...); camino.pop_back();
    //   - Como std::map itera ordenado por letra, el resultado sale en orden
    //     alfabético sin ordenar nada.
    (void)trie;
    return {};
}

// Ejercicio 8 — Autocompletar
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo, int maxResultados)
{
    // TODO: implementar autocompletar.
    // Recordá:
    //   - Llegá al nodo del prefijo. Si no existe, devolvé un vector vacío.
    //   - Hacé el mismo DFS que en listarPalabras, pero arrancando con
    //     camino = prefijo.
    //   - Cortá apenas resultado.size() llegue a maxResultados (chequeá al
    //     entrar a la recursión y dentro del for de los hijos).
    (void)trie;
    (void)prefijo;
    (void)maxResultados;
    return {};
}

// Ejercicio 9 — Prefijo común más largo
std::string prefijoComunMasLargo(const Trie& trie)
{
    // TODO: implementar prefijoComunMasLargo.
    // Recordá:
    //   - Arrancá en la raíz con prefijo = "".
    //   - Mientras el nodo tenga exactamente 1 hijo Y no sea final: agregá la
    //     letra de ese hijo al prefijo y bajá.
    //   - Con std::map, el único hijo es nodo->hijos.begin() (->first es la
    //     letra, ->second el nodo).
    (void)trie;
    return "";
}

// ============================================================================
// PARTE 3 — PROBLEMAS CON TRIES
// ============================================================================

// Ejercicio 10 — Buscar con comodín
bool buscarConComodin(const Trie& trie, const std::string& patron)
{
    // TODO: implementar buscarConComodin.
    // Recordá:
    //   - Auxiliar recursiva: bool buscar(NodoTrie* nodo, const std::string& patron, size_t i)
    //   - Caso base: i == patron.size() -> la respuesta es nodo->esFinal.
    //   - Si patron[i] == '.', probá con TODOS los hijos; alcanza con que
    //     UNO devuelva true.
    //   - Si es una letra común, seguí solo por ese hijo (si no existe, false).
    //   - Patrón vacío: false (no hay palabra vacía guardada).
    (void)trie;
    (void)patron;
    return false;
}

// Ejercicio 11 — Reemplazar por raíces
std::string reemplazarRaices(const std::vector<std::string>& raices, const std::string& oracion)
{
    // TODO: implementar reemplazarRaices.
    // Recordá:
    //   - Armá un Trie con todas las raíces (usá tu función insertar).
    //   - Separá la oración en palabras (por ejemplo con std::istringstream,
    //     `#include <sstream>`, o recorriendo los caracteres hasta cada ' ').
    //   - Para cada palabra, bajá por el trie letra por letra. Si el nodo al
    //     que llegás es esFinal, esa es la raíz más corta: usá
    //     palabra.substr(0, i + 1). Si el camino se corta antes, dejá la
    //     palabra tal cual.
    //   - Volvé a unir las palabras con un espacio entre cada una.
    (void)raices;
    (void)oracion;
    return "";
}

// Ejercicio 12 — Palabra más larga construible
std::string palabraMasLargaConstruible(const std::vector<std::string>& palabras)
{
    // TODO: implementar palabraMasLargaConstruible.
    // Recordá:
    //   - Insertá todas las palabras en un Trie.
    //   - DFS desde la raíz llevando el camino actual, pero SOLO bajá a un
    //     hijo si ese hijo es esFinal (eso garantiza que todos los prefijos
    //     están en la lista).
    //   - Cada vez que llegues a un nodo, si el camino es más largo (estricto)
    //     que el mejor hallado, actualizá el mejor. Como se visita en orden
    //     alfabético, el estricto ya resuelve el desempate.
    //   - Si ninguna palabra cumple, el mejor queda en "".
    (void)palabras;
    return "";
}

// ============================================================================
// PARTE 4 — TRIES BINARIOS
// ============================================================================

// Ejercicio 13 — Máximo XOR
int maximoXor(const std::vector<int>& numeros)
{
    // TODO: implementar maximoXor.
    // Recordá:
    //   - Los números tienen a lo sumo 30 bits (0 <= x < 2^30): los bits van
    //     del 29 (más significativo) al 0.
    //   - Bit i de x:  (x >> i) & 1
    //   - Insertar x: desde la raíz, para i = 29 .. 0 creá (si falta) el
    //     hijo[bit] y bajá.
    //   - Mejor pareja para x: para i = 29 .. 0, querés ir por el bit
    //     OPUESTO (1 - bit). Si existe ese hijo, sumá (1 << i) al XOR
    //     resultante y bajá por él; si no, bajá por el hijo igual al bit.
    //   - La respuesta es el máximo de esos XOR sobre todos los x.
    //   - Liberá los nodos al final (ej.: con una función recursiva
    //     postorder, o guardando todos los nodos en un std::vector<NodoBit*>
    //     para hacer delete en un loop).
    (void)numeros;
    return 0;
}

// ============================================================================
// PARTE 5 — ROSETREE (árbol n-ario)
// ============================================================================

// Ejercicio 14 — Contar hojas
int contarHojas(Rosetree<int>* nodo)
{
    // TODO: implementar contarHojas.
    // Recordá:
    //   - Si el nodo es nullptr, hay 0 hojas.
    //   - Si el nodo no tiene hijos (nodo->hijos.empty()), es una hoja: 1.
    //   - Si no, sumá las hojas de cada hijo (con un for sobre nodo->hijos).
    (void)nodo;
    return 0;
}

// Ejercicio 15 — Nodos en un nivel
int nodosEnNivel(Rosetree<int>* nodo, int nivel)
{
    // TODO: implementar nodosEnNivel.
    // Recordá:
    //   - Si el nodo es nullptr, hay 0.
    //   - Si nivel == 0, el nodo actual es el único que cuenta: devolvé 1.
    //   - Si no, sumá nodosEnNivel(hijo, nivel - 1) para cada hijo.
    (void)nodo;
    (void)nivel;
    return 0;
}
