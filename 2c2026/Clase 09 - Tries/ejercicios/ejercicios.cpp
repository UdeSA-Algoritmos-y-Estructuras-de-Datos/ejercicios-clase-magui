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
    // Se espera que, después de la llamada, la clave quede guardada con su
    // valor. Si ya estaba, solo cambia el valor. Cuidá que `cantidad` refleje
    // la cantidad de claves distintas.
    (void)trie;
    (void)clave;
    (void)valor;
}

// Ejercicio 2 — NodoDe
NodoTrie* nodoDe(const Trie& trie, const std::string& clave)
{
    // TODO: implementar nodoDe.
    // Se espera el nodo que corresponde a la clave, o nullptr si no hay
    // ningún camino que la represente. Que el nodo exista no implica que la
    // clave esté guardada.
    (void)trie;
    (void)clave;
    return nullptr;
}

// Ejercicio 3 — Pertenece
bool pertenece(const Trie& trie, const std::string& clave)
{
    // TODO: implementar pertenece.
    // Se espera true solo si la clave completa está guardada (no alcanza con
    // que sea el comienzo de otra clave).
    (void)trie;
    (void)clave;
    return false;
}

// Ejercicio 4 — Obtener
int obtener(const Trie& trie, const std::string& clave)
{
    // TODO: implementar obtener.
    // Se espera el valor asociado a la clave (que, por precondición, está
    // guardada).
    (void)trie;
    (void)clave;
    return 0;
}

// Ejercicio 5 — EsHojaVacia
bool esHojaVacia(const NodoTrie* nodo)
{
    // TODO: implementar esHojaVacia.
    // Se espera true solo si el nodo no representa una clave y además no
    // tiene ningún hijo.
    (void)nodo;
    return false;
}

// Ejercicio 6 — Eliminar
void eliminar(Trie& trie, const std::string& clave)
{
    // TODO: implementar eliminar.
    // Se espera que la clave y su valor dejen de estar guardados, que las
    // demás claves (incluidas las que comparten camino con la eliminada)
    // sigan intactas, y que el trie siga cumpliendo el invariante: no pueden
    // quedar nodos que no lleven a ninguna clave, y la memoria de los nodos
    // que sobran tiene que liberarse. Los nodos no conocen a su padre: pensá
    // qué necesitás guardar mientras bajás para poder subir después.
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
    // Se espera true si alguna clave guardada empieza con el prefijo. Pensá
    // qué debería pasar con el prefijo vacío en un trie vacío.
    (void)trie;
    (void)prefijo;
    return false;
}

// Ejercicio 8 — Contar con prefijo
int contarConPrefijo(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar contarConPrefijo.
    // Se espera la cantidad de claves guardadas que empiezan con el prefijo,
    // contando a la propia clave si el prefijo coincide exactamente con una.
    (void)trie;
    (void)prefijo;
    return 0;
}

// Ejercicio 9 — Contar nodos
int contarNodos(const Trie& trie)
{
    // TODO: implementar contarNodos.
    // Se espera la cantidad total de nodos que tiene el trie, contando la
    // raíz. Es el mismo problema que contar nodos en un rosetree.
    (void)trie;
    return 0;
}

// Ejercicio 10 — Listar claves
std::vector<std::string> listarClaves(const Trie& trie)
{
    // TODO: implementar listarClaves.
    // Se espera la lista de todas las claves guardadas, ordenadas
    // alfabéticamente. Pensá cómo reconstruir cada clave a partir del camino
    // recorrido, y si hace falta ordenar algo o el propio trie ya lo da.
    (void)trie;
    return {};
}

// Ejercicio 11 — Autocompletar
std::vector<std::string> autocompletar(const Trie& trie, const std::string& prefijo, int maxResultados)
{
    // TODO: implementar autocompletar.
    // Se espera la lista (alfabética) de claves que empiezan con el prefijo,
    // con no más de maxResultados elementos. Si ya juntaste los que hacen
    // falta, no tiene sentido seguir recorriendo.
    (void)trie;
    (void)prefijo;
    (void)maxResultados;
    return {};
}

// Ejercicio 12 — Prefijo común más largo
std::string prefijoComunMasLargo(const Trie& trie)
{
    // TODO: implementar prefijoComunMasLargo.
    // Se espera el prefijo más largo que comparten todas las claves guardadas.
    // Pensá en qué momento el camino deja de ser el mismo para todas: cuando
    // se bifurca, o cuando alguna clave termina.
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
    // Se espera true si existe alguna clave guardada que encaje con el patrón
    // de punta a punta, donde '.' acepta cualquier letra. Un comodín puede
    // encajar de muchas maneras: pensá cómo explorar todas.
    (void)trie;
    (void)patron;
    return false;
}

// Ejercicio 14 — Reemplazar por raíces
std::string reemplazarRaices(const std::vector<std::string>& raices, const std::string& oracion)
{
    // TODO: implementar reemplazarRaices.
    // Se espera la oración con cada palabra reemplazada por la raíz más corta
    // que sea prefijo suyo (o sin cambios si no hay ninguna), respetando los
    // espacios que separan las palabras. Podés usar tu Trie para guardar las
    // raíces.
    (void)raices;
    (void)oracion;
    return "";
}

// Ejercicio 15 — Sugerencia más frecuente
std::string sugerenciaMasFrecuente(const Trie& trie, const std::string& prefijo)
{
    // TODO: implementar sugerenciaMasFrecuente.
    // Se espera la clave de mayor valor entre las que empiezan con el prefijo,
    // o "" si no hay ninguna. En caso de empate, la que va primero
    // alfabéticamente. Ojo: los valores pueden ser 0 o negativos.
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
    // Se espera el mayor XOR posible entre dos elementos del vector. Probar
    // todos los pares funciona pero es O(n²): la idea es usar un trie sobre
    // los bits de los números (30 bits, del más al menos significativo) para
    // bajar la complejidad. Acordate de liberar la memoria que pidas.
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
    // Se espera la cantidad de nodos sin hijos del árbol. Pensá qué pasa con
    // el árbol vacío y con un árbol de un solo nodo.
    (void)nodo;
    return 0;
}

// Ejercicio 18 — Nodos en un nivel
int nodosEnNivel(Rosetree<int>* nodo, int nivel)
{
    // TODO: implementar nodosEnNivel.
    // Se espera cuántos nodos hay exactamente a esa profundidad, contando la
    // raíz como nivel 0 (0 si el árbol no llega a ese nivel).
    (void)nodo;
    (void)nivel;
    return 0;
}
