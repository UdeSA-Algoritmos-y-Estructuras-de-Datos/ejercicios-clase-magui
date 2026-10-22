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
//
// Del header tenés a mano: indice(c) (de 'a'..'z' a 0..25) y simbolo(i) (de
// 0..25 a 'a'..'z'). Los dos trabajan con size_t: para recorrer los hijos
// usá `for (size_t i = 0; i < TAM_ALFABETO; i++)`. Ojo con -Wconversion:
// para armar un char a partir de un número usá simbolo(i).
// ============================================================================

// ============================================================================
// PARTE 1 — TAD DICCIONARIO SOBRE UN TRIE
// ============================================================================

// Ejercicio 1 — Insertar
void insertar(Trie& trie, const std::string& clave, int valor)
{
    // TODO: implementar insertar.
    // Recordá:
    //   - Arrancá en trie.raiz y recorré la clave símbolo por símbolo.
    //   - Para cada símbolo c, el hijo está en nodo->hijos[indice(c)]. Si es
    //     nullptr, creálo con `new NodoTrie()`.
    //   - Avanzá al hijo.
    //   - Al terminar, si el nodo NO tenía hayClave todavía, es una clave
    //     nueva: incrementá trie.cantidad. Después poné hayClave = true y
    //     guardá el valor (si ya existía, esto reemplaza el valor anterior).
    (void)trie;
    (void)clave;
    (void)valor;
}

// Ejercicio 2 — NodoDe
NodoTrie* nodoDe(const Trie& trie, const std::string& clave)
{
    // TODO: implementar nodoDe.
    // Recordá:
    //   - Bajá desde la raíz símbolo por símbolo.
    //   - Si en algún momento el nodo actual es nullptr, el camino se cortó:
    //     devolvé nullptr.
    //   - Al terminar de recorrer la clave, devolvé el nodo al que llegaste.
    (void)trie;
    (void)clave;
    return nullptr;
}

// Ejercicio 3 — Pertenece
bool pertenece(const Trie& trie, const std::string& clave)
{
    // TODO: implementar pertenece.
    // Recordá:
    //   - Usá nodoDe. La clave pertenece si el nodo existe Y tiene hayClave.
    //   - Con la clave vacía, nodoDe devuelve la raíz, que nunca tiene hayClave.
    (void)trie;
    (void)clave;
    return false;
}

// Ejercicio 4 — Obtener
int obtener(const Trie& trie, const std::string& clave)
{
    // TODO: implementar obtener.
    // Recordá:
    //   - La precondición dice que la clave pertenece, así que nodoDe no
    //     devuelve nullptr. Devolvé el valor de ese nodo.
    (void)trie;
    (void)clave;
    return 0;
}

// Ejercicio 5 — EsHojaVacia
bool esHojaVacia(const NodoTrie* nodo)
{
    // TODO: implementar esHojaVacia.
    // Recordá:
    //   - Si el nodo tiene hayClave, NO es hoja vacía (aunque no tenga hijos).
    //   - Si algún hijo no es nullptr, tampoco.
    //   - Si no, sí lo es.
    (void)nodo;
    return false;
}

// Ejercicio 6 — Eliminar
void eliminar(Trie& trie, const std::string& clave)
{
    // TODO: implementar eliminar.
    // Recordá (es la versión de la teórica, con una pila):
    //   - Una pila de ancestros: un std::vector<NodoTrie*> con push_back,
    //     back y pop_back alcanza (o std::stack, con #include <stack>).
    //   - Bajá por la clave: antes de pasar al hijo, apilá el nodo actual.
    //   - En el nodo de la clave: hayClave = false, valor = 0, y bajá en 1
    //     trie.cantidad.
    //   - Limpieza (la teórica la llama LimpiarHojasVacias): mientras la pila
    //     no esté vacía Y el nodo sea hoja vacía:
    //         padre = tope de la pila (y lo desapilás)
    //         padre->hijos[indice(símbolo que llevaba del padre al nodo)] = nullptr
    //         delete nodo
    //         nodo = padre
    //     El símbolo que lleva del padre al nodo es clave[k], con k yendo de
    //     clave.size() - 1 hacia atrás.
    //   - La raíz nunca se borra (cuando la pila queda vacía, se corta).
    (void)trie;
    (void)clave;
}

// ============================================================================
// PARTE 2 — PREFIJOS Y RECORRIDOS (DFS sobre el trie)
// ============================================================================

// Ejercicio 7 — Tiene prefijo
bool tienePrefijo(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar tienePrefijo.
    // Recordá:
    //   - Con prefijo vacío, nodoDe devuelve la raíz aunque el trie esté
    //     vacío: tratá ese caso aparte (true solo si trie.cantidad > 0).
    //   - Con cualquier otro prefijo, alcanza con que nodoDe no devuelva
    //     nullptr (el invariante garantiza que de ahí cuelga alguna clave).
    (void)trie;
    (void)prefijo;
    return false;
}

// Ejercicio 8 — Contar con prefijo
int contarConPrefijo(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar contarConPrefijo.
    // Recordá:
    //   - Llegá hasta el nodo del prefijo (nodoDe). Si es nullptr, hay 0 claves.
    //   - Desde ese nodo, contá los nodos con hayClave de su subárbol (incluido
    //     él) con una función recursiva auxiliar:
    //       cuenta = (nodo->hayClave ? 1 : 0) + suma de cuenta(hijo) para cada hijo no nulo
    (void)trie;
    (void)prefijo;
    return 0;
}

// Ejercicio 9 — Contar nodos
int contarNodos(const Trie& trie)
{
    // TODO: implementar contarNodos.
    // Recordá:
    //   - Es igual que contar nodos en un rosetree: 1 (el nodo actual) más la
    //     suma de contar cada hijo no nulo. Usá una función auxiliar recursiva
    //     que reciba un NodoTrie*.
    (void)trie;
    return 0;
}

// Ejercicio 10 — Listar claves
std::vector<std::string> listarClaves(const Trie& trie)
{
    // TODO: implementar listarClaves.
    // Recordá:
    //   - Auxiliar recursiva: void recolectar(NodoTrie* nodo, std::string& camino,
    //                                         std::vector<std::string>& resultado)
    //   - Si el nodo tiene hayClave, agregá `camino` al resultado.
    //   - Para i de 0 a TAM_ALFABETO - 1 (size_t), si hijos[i] no es nullptr:
    //       camino.push_back(simbolo(i)); recolectar(hijo, ...); camino.pop_back();
    //   - Recorriendo i en orden, el resultado sale en orden alfabético sin
    //     ordenar nada.
    (void)trie;
    return {};
}

// Ejercicio 11 — Autocompletar
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo, int maxResultados)
{
    // TODO: implementar autocompletar.
    // Recordá:
    //   - Llegá al nodo del prefijo (nodoDe). Si es nullptr, devolvé un vector vacío.
    //   - Hacé el mismo DFS que en listarClaves, pero arrancando con
    //     camino = prefijo.
    //   - Cortá apenas resultado.size() llegue a maxResultados (chequeá al
    //     entrar a la recursión y dentro del for de los hijos).
    (void)trie;
    (void)prefijo;
    (void)maxResultados;
    return {};
}

// Ejercicio 12 — Prefijo común más largo
std::string prefijoComunMasLargo(const Trie& trie)
{
    // TODO: implementar prefijoComunMasLargo.
    // Recordá:
    //   - Arrancá en la raíz con prefijo = "".
    //   - Mientras el nodo NO tenga hayClave y tenga exactamente 1 hijo no
    //     nulo: agregá el símbolo de ese hijo al prefijo y bajá.
    //   - Para saber cuántos hijos tiene (y cuál es el único) recorré
    //     hijos[0..TAM_ALFABETO-1] contando los que no son nullptr.
    (void)trie;
    return "";
}

// ============================================================================
// PARTE 3 — PROBLEMAS CON TRIES
// ============================================================================

// Ejercicio 13 — Buscar con comodín
bool buscarConComodin(const Trie& trie, const std::string& patron)
{
    // TODO: implementar buscarConComodin.
    // Recordá:
    //   - Auxiliar recursiva: bool buscar(NodoTrie* nodo, const std::string& patron, size_t i)
    //   - Caso base: i == patron.size() -> la respuesta es nodo->hayClave.
    //   - Si patron[i] == '.', probá con TODOS los hijos no nulos; alcanza con
    //     que UNO devuelva true.
    //   - Si es una letra común, seguí solo por ese hijo (si es nullptr, false).
    //   - Patrón vacío: false (no hay clave vacía guardada).
    (void)trie;
    (void)patron;
    return false;
}

// Ejercicio 14 — Reemplazar por raíces
std::string reemplazarRaices(const std::vector<std::string>& raices, const std::string& oracion)
{
    // TODO: implementar reemplazarRaices.
    // Recordá:
    //   - Armá un Trie con todas las raíces (usá tu función insertar con valor 0).
    //   - Separá la oración en palabras (por ejemplo con std::istringstream,
    //     `#include <sstream>`, o recorriendo los caracteres hasta cada ' ').
    //   - Para cada palabra, bajá por el trie símbolo por símbolo. Si el nodo
    //     al que llegás tiene hayClave, esa es la raíz más corta: usá
    //     palabra.substr(0, i + 1). Si el camino se corta antes (nullptr),
    //     dejá la palabra tal cual.
    //   - Volvé a unir las palabras con un espacio entre cada una.
    (void)raices;
    (void)oracion;
    return "";
}

// Ejercicio 15 — Sugerencia más frecuente
std::string sugerenciaMasFrecuente(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar sugerenciaMasFrecuente.
    // Recordá:
    //   - Llegá al nodo del prefijo (nodoDe). Si es nullptr, devolvé "".
    //   - DFS desde ahí llevando el camino actual (arrancando en `prefijo`),
    //     y la mejor clave y su valor hasta ahora. Podés pasarlos por
    //     referencia a la auxiliar, junto con un bool "encontré alguna".
    //   - En cada nodo con hayClave: si todavía no hay mejor, o si su valor es
    //     ESTRICTAMENTE mayor que el mejor, actualizá. Como se visita en orden
    //     alfabético, el estricto ya resuelve el desempate.
    (void)trie;
    (void)prefijo;
    return "";
}

// ============================================================================
// PARTE 4 — TRIES BINARIOS
// ============================================================================

// Ejercicio 16 — Máximo XOR
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

// Ejercicio 17 — Contar hojas
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

// Ejercicio 18 — Nodos en un nivel
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
