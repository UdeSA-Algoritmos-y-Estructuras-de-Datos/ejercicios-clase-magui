#include "ejercicios.h"

#include <algorithm> // std::sort

// ============================================================================
// Acá va TU código. Cada función tiene una cáscara con un "TODO": borrá el
// contenido de ejemplo y escribí la implementación.
//
// Los `(void)parametro;` que ves abajo están solo para que la cáscara
// compile sin advertencias antes de que la implementes. Cuando uses el
// parámetro en tu código, borrá la línea `(void)...` correspondiente.
//
// Podés agregar funciones auxiliares arriba de la función que las usa. Si
// querés que sean privadas del archivo, declaralas `static`.
//
// El pseudocódigo de cada algoritmo (el de la teórica) está en
// "ejercicios.h", arriba de cada ejercicio.
//
// Ojo con -Wconversion: no mezcles size_t con int sin un static_cast.
// ============================================================================

// ============================================================================
// PARTE 1 — FUERZA BRUTA Y ENUMERACIONES COMBINATORIAS
// ============================================================================

// Ejercicio 1 — Fuerza bruta: averiguar el PIN
long long encontrarPIN(unsigned long long hashBuscado, size_t digitos)
{
    // TODO: implementar encontrarPIN.
    // pre:  1 <= digitos <= 9.
    // post: res es el PIN (entre 0 y 10^digitos - 1) con hashPIN(res) ==
    //       hashBuscado; o NO_HAY si ninguno da ese hash.
    // Primero hay que calcular cuántos candidatos hay (10^digitos, con un
    // ciclo) y después probarlos todos, de a uno.
    (void)hashBuscado;
    (void)digitos;
    return NO_HAY;
}

// Ejercicio 2 — Variaciones
void variaciones(const Lista& A, size_t k, Lista& actual, size_t i, Listas& res)
{
    // TODO: implementar variaciones.
    // pre:  actual tiene largo k; 0 <= i <= k.
    // post: agrega al final de res todas las variaciones que empiezan con
    //       actual[0..i - 1], en el orden del pseudocódigo.
    // "imprimir(actual)" acá es res.push_back(actual).
    (void)A;
    (void)k;
    (void)actual;
    (void)i;
    (void)res;
}

// Ejercicio 3 — Permutaciones
void permutaciones(const Lista& A, Lista& actual, std::vector<bool>& usado, size_t i, Listas& res)
{
    // TODO: implementar permutaciones.
    // pre:  actual y usado tienen largo |A|; usado[j] si y sólo si A[j] está
    //       en actual[0..i - 1].
    // post: agrega al final de res las permutaciones que empiezan con
    //       actual[0..i - 1], en el orden del pseudocódigo; usado queda como
    //       estaba.
    // Elegir, llamar, deshacer.
    (void)A;
    (void)actual;
    (void)usado;
    (void)i;
    (void)res;
}

// Ejercicio 4 — Combinaciones
void combinaciones(const Lista& A, size_t k, Lista& actual, size_t i, size_t desde, Listas& res)
{
    // TODO: implementar combinaciones.
    // pre:  actual tiene largo k; 0 <= i <= k; actual[0..i - 1] sale de
    //       A[0..desde - 1], en orden.
    // post: agrega al final de res las combinaciones que empiezan con
    //       actual[0..i - 1] y siguen con elementos de A[desde..], en el
    //       orden del pseudocódigo.
    (void)A;
    (void)k;
    (void)actual;
    (void)i;
    (void)desde;
    (void)res;
}

// Ejercicio 5 — Subconjuntos
void subconjuntos(const Lista& A, std::vector<bool>& usar, size_t i, Listas& res)
{
    // TODO: implementar subconjuntos.
    // pre:  usar tiene largo |A|; 0 <= i <= |A|.
    // post: agrega al final de res todos los subconjuntos que coinciden con
    //       usar[0..i - 1] en los primeros i elementos; para cada elemento se
    //       prueba primero "sí" (usar[i] = true) y después "no".
    // Es Variaciones con las opciones [true, false]. Cuando i llega a |A|,
    // hay que armar la lista de los A[j] con usar[j] == true y agregarla a
    // res.
    (void)A;
    (void)usar;
    (void)i;
    (void)res;
}

// ============================================================================
// PARTE 2 — BACKTRACKING
// ============================================================================

// Ejercicio 6 — ¿Cae adentro?
bool caeAdentro(const Laberinto& L, Posicion pos)
{
    // TODO: implementar caeAdentro.
    // pre:  L tiene al menos una fila y todas sus filas tienen el mismo largo.
    // post: res == true si y sólo si 0 <= pos.fila < n y 0 <= pos.col < m.
    // La cantidad de filas es static_cast<int>(L.size()) y la de columnas,
    // static_cast<int>(L[0].size()).
    (void)L;
    (void)pos;
    return false;
}

// Ejercicio 7 — Resolver el laberinto
bool resolverLaberinto(Laberinto& L, Posicion pos)
{
    // TODO: implementar resolverLaberinto.
    // pre:  pos es una celda de L marcada con HILO.
    // post: devuelve si se llega de pos a la salida. Si se llega, el camino
    //       queda marcado con HILO; si no, L queda como estaba.
    // Es el pseudocódigo de la teórica, usando el Ejercicio 6.
    //
    // Pista: lo que ya viene hecho en "ejercicios.h":
    //     Posicion fin = salida(L);                    // Salida(L)
    //     pos.fila == fin.fila && pos.col == fin.col   // pos = Salida(L)
    //     for (size_t d = 0; d < 4; d++)               // para cada dirección
    //     Posicion prox = mover(pos, DIRECCIONES[d]);  // prox = pos + dir
    //     contenido(L, prox) == VACIO                  // Contenido(L, prox)
    //     ubicar(L, prox, HILO);                       // Ubicar(L, prox, hilo)
    (void)L;
    (void)pos;
    return false;
}

// Ejercicio 8 — La poda del sudoku: ¿se puede poner este dígito?
bool esValido(const Sudoku& S, size_t i, size_t j, int d)
{
    // TODO: implementar esValido.
    // pre:  i, j < n; 1 <= d <= n; S[i][j] == VACIA.
    // post: res == true si y sólo si d no aparece en la fila i, ni en la
    //       columna j, ni en el cuadrante de (i, j).
    // Pista: con lado = ladoDeCuadrante(S), el cuadrante de (i, j) son las
    // filas (i / lado) * lado, ..., (i / lado) * lado + lado - 1, y lo mismo
    // con las columnas y j.
    (void)S;
    (void)i;
    (void)j;
    (void)d;
    return false;
}

// Ejercicio 9 — Resolver el sudoku
bool resolverSudoku(Sudoku& S, size_t i, size_t j)
{
    // TODO: implementar resolverSudoku.
    // pre:  i <= n y j < n; las celdas anteriores a (i, j), fila por fila,
    //       están completas.
    // post: devuelve si existe solución. Si existe, el sudoku queda completo;
    //       si no, queda como estaba.
    // Es el esquema general de backtracking: la decisión es qué dígito va en
    // la celda (i, j), la poda es el Ejercicio 8 y deshacer es volver a dejar
    // la celda VACIA.
    // Pista: para probar los dígitos, for (int d = 1; d <= n; d++), con
    // int n = static_cast<int>(S.size()).
    (void)S;
    (void)i;
    (void)j;
    return false;
}

// Ejercicio 10 — Suma de subconjuntos
bool haySubconjunto(const Lista& A, size_t i, long long k)
{
    // TODO: implementar haySubconjunto.
    // pre:  los elementos de A son no negativos; 0 <= i <= |A|.
    // post: devuelve si se pueden elegir elementos de A[i..|A| - 1] que
    //       sumen k.
    // Es el pseudocódigo de la teórica, tal cual.
    (void)A;
    (void)i;
    (void)k;
    return false;
}

// ============================================================================
// PARTE 3 — GREEDY
// ============================================================================

// Dice si la actividad a va antes que la actividad b al ordenar por horario
// de fin. La usa el std::sort del Ejercicio 11. (Ya está hecha: no hay que
// tocarla.)
static bool terminaAntes(const Actividad& a, const Actividad& b)
{
    return a.fin < b.fin;
}

// Ejercicio 11 — Selección de actividades
std::vector<Actividad> seleccionarActividades(std::vector<Actividad> A)
{
    // pre:  toda actividad tiene inicio < fin.
    // post: res es un conjunto de actividades de A sin superposiciones, de
    //       tamaño máximo, en el orden en que se eligieron.

    // La línea 1 del pseudocódigo ya está hecha: std::sort ordena A de
    // principio a fin (A.begin(), A.end()), y terminaAntes le dice cuándo una
    // actividad va antes que otra.
    std::sort(A.begin(), A.end(), terminaAntes);

    std::vector<Actividad> res;

    // TODO: las líneas 2 a 8 del pseudocódigo. `libre` empieza en
    // MENOS_INFINITO. Para agregar una actividad: res.push_back(a).

    return res;
}

// Ejercicio 12 — Cuando el greedy falla: el vuelto
size_t vueltoGreedy(const std::vector<size_t>& monedas, size_t v)
{
    // TODO: implementar vueltoGreedy.
    // pre:  todas las monedas son > 0 (en cualquier orden).
    // post: res es la cantidad de monedas que usa el greedy "siempre la más
    //       grande que entre" para dar exactamente v (0 si v == 0), o
    //       IMPOSIBLE si en algún momento falta algo y ninguna moneda entra.
    (void)monedas;
    (void)v;
    return 0;
}

// Ejercicio 13 — El vuelto con backtracking
size_t vueltoMinimo(const std::vector<size_t>& monedas, size_t v)
{
    // TODO: implementar vueltoMinimo.
    // pre:  todas las monedas son > 0 (en cualquier orden).
    // post: res es la MENOR cantidad de monedas con las que se puede dar
    //       exactamente v (0 si v == 0), o IMPOSIBLE si no hay forma.
    // Probar cada moneda que entre para lo que falta, y quedarse con la
    // mejor. Ojo: IMPOSIBLE + 1 se desborda.
    (void)monedas;
    (void)v;
    return 0;
}

// ============================================================================
// PARTE 4 — OPCIONALES (para después)
// ============================================================================

// Dice si la actividad a va antes que la actividad b al ordenar por horario
// de inicio. La usa el std::sort del Ejercicio 14. (Ya está hecha.)
static bool empiezaAntes(const Actividad& a, const Actividad& b)
{
    return a.inicio < b.inicio;
}

// Ejercicio 14 (opcional) — ¿Cuántas aulas?
size_t cantidadDeAulas(std::vector<Actividad> A)
{
    // pre:  toda actividad tiene inicio < fin.
    // post: res es la menor cantidad de aulas con las que se pueden hacer
    //       todas las actividades (0 si no hay ninguna).

    // El ordenamiento por inicio ya está hecho.
    std::sort(A.begin(), A.end(), empiezaAntes);

    // TODO: recorrer las actividades en orden. Cada una va a cualquier aula
    // que ya esté libre; si están todas ocupadas, se abre una nueva.
    // Pista: un std::vector<int> con, para cada aula abierta, desde cuándo
    // está libre.

    return 0;
}

// Ejercicio 15 (opcional) — El laberinto: ¿hace falta deshacer?
bool resolverLaberintoSinDeshacer(Laberinto& L, Posicion pos)
{
    // TODO: implementar resolverLaberintoSinDeshacer.
    // pre:  pos es una celda de L marcada con HILO.
    // post: devuelve si se llega de pos a la salida. Si se llega, hay un
    //       camino de pos a la salida marcado con HILO. Las celdas por las
    //       que se pasó y no sirvieron quedan VISITADA.
    // Es tu resolverLaberinto del Ejercicio 7, cambiando sólo lo que se hace
    // al volver.
    (void)L;
    (void)pos;
    return false;
}
