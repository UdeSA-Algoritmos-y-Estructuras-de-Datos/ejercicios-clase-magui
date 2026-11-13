#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <cstddef>
#include <limits>
#include <vector>

// ============================================================================
// Tut 13 - Fuerza bruta, backtracking y greedy
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
//
// Los ejercicios siguen el orden de la teórica (clase 21), con el mismo
// pseudocódigo y los mismos nombres del handout.
//   - Parte 1: FUERZA BRUTA Y ENUMERACIONES: el PIN, variaciones,
//              permutaciones, combinaciones y subconjuntos.
//   - Parte 2: BACKTRACKING: el laberinto, el sudoku y la suma de
//              subconjuntos.
//   - Parte 3: GREEDY: selección de actividades y el vuelto (con greedy y
//              con backtracking).
//   - Parte 4: OPCIONALES, para después: cuántas aulas hacen falta y el
//              laberinto sin deshacer.
//
// Notación de los comentarios de cada ejercicio:
//   - pre:  lo que se le garantiza a la función al llamarla (si no se cumple,
//           no hay nada que cumplir: no hace falta chequearlo).
//   - post: lo que la función tiene que cumplir al terminar.
//   - |A| es A.size().
// ============================================================================

// Una lista de enteros, y una lista de listas (lo que devuelven las
// enumeraciones).
typedef std::vector<int> Lista;
typedef std::vector<Lista> Listas;

// ============================================================================
// PARTE 1 — FUERZA BRUTA Y ENUMERACIONES COMBINATORIAS
//
// Fuerza bruta: generar una lista de candidatos y, para cada uno, verificar si
// es la solución. Calcular la solución se reduce a VERIFICAR si un candidato
// es solución.
// ============================================================================

// Resultado de encontrarPIN cuando ningún PIN sirve.
const long long NO_HAY = -1;

// La función de hash H del ejemplo de la teórica. Ya está hecha: no hay que
// tocarla. Calcularla es fácil; calcular su inversa (dado H(K), encontrar K)
// es "impracticable"... salvo que haya pocos candidatos.
inline unsigned long long hashPIN(long long pin)
{
    unsigned long long x = static_cast<unsigned long long>(pin) + 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

// Ejercicio 1 — Fuerza bruta: averiguar el PIN
// La clave K es un PIN de cierta cantidad de dígitos. El sistema no guarda K
// sino H(K). Si averiguo H(K), ¿cómo averiguo K? Calculando
//     H(00...0), H(00...1), ..., H(99...9)
// hasta dar con el que da el valor buscado.
// Un PIN de d dígitos se representa con un número entre 0 y 10^d - 1 (el PIN
// 0042 es el número 42).
// pre:  1 <= digitos <= 9.
// post: res es el PIN (entre 0 y 10^digitos - 1) con hashPIN(res) ==
//       hashBuscado; o NO_HAY si ningún PIN de esa cantidad de dígitos da ese
//       hash.
// Complejidad: O(10^digitos).
long long encontrarPIN(unsigned long long hashBuscado, size_t digitos);

// ---------------------------------------------------------------------------
// Enumeraciones combinatorias
//
// Para aplicar fuerza bruta hay que saber generar todos los candidatos. Idea
// común: construir cada candidato de a una posición: un arreglo `actual` con
// las primeras i posiciones ya decididas, y un llamado recursivo por cada
// opción para la posición i.
//
// En la teórica cada candidato se imprime. Acá, en vez de imprimirlo, se lo
// agrega al final de `res`:   imprimir(actual)  ->  res.push_back(actual)
//
// Las pruebas hacen el primer llamado: con `actual` del largo de un candidato
// (su contenido inicial no importa), i = 0 y `res` vacío.
// ---------------------------------------------------------------------------

// Ejercicio 2 — Variaciones
// Todas las listas de longitud k que se pueden formar tomando elementos de A
// (con repeticiones; el orden es relevante).
//
//     Variaciones(A, k, actual, i)
//     1  if i = k
//     2      imprimir(actual)
//     3      return
//     4  for j = 0 to |A| - 1
//     5      actual[i] = A[j]
//     6      Variaciones(A, k, actual, i + 1)
//
// pre:  actual tiene largo k; 0 <= i <= k.
// post: agrega al final de res todas las variaciones que empiezan con
//       actual[0..i - 1], en el orden en que las genera el pseudocódigo.
// Ejemplo: con A = [0, 1] y k = 3:
//   [0,0,0], [0,0,1], [0,1,0], [0,1,1], [1,0,0], [1,0,1], [1,1,0], [1,1,1]
// Complejidad: O(k * n^k), con n = |A|.
void variaciones(const Lista& A, size_t k, Lista& actual, size_t i, Listas& res);

// Ejercicio 3 — Permutaciones
// Todos los órdenes posibles de A.
//
//     Permutaciones(A, actual, usado, i)
//     1  if i = |A|
//     2      imprimir(actual)
//     3      return
//     4  for j = 0 to |A| - 1
//     5      if no usado[j]
//     6          usado[j] = true     // elegir
//     7          actual[i] = A[j]
//     8          Permutaciones(A, actual, usado, i + 1)
//     9          usado[j] = false    // deshacer
//
// pre:  actual y usado tienen largo |A|; usado[j] si y sólo si A[j] está en
//       actual[0..i - 1]. (En el primer llamado, usado está todo en false.)
// post: agrega al final de res las permutaciones que empiezan con
//       actual[0..i - 1], en el orden del pseudocódigo; usado queda como
//       estaba.
// Ejemplo: con A = [0, 1, 2]:
//   [0,1,2], [0,2,1], [1,0,2], [1,2,0], [2,0,1], [2,1,0]
// Complejidad: O(n * n!).
void permutaciones(const Lista& A, Lista& actual, std::vector<bool>& usado, size_t i, Listas& res);

// Ejercicio 4 — Combinaciones
// Todas las listas de longitud k que se pueden formar tomando elementos de A
// (sin repetir; el orden es irrelevante). Para no contar dos veces el mismo
// conjunto, se eligen en orden creciente de posición.
//
//     Combinaciones(A, k, actual, i, desde)
//     1  if i = k
//     2      imprimir(actual)
//     3      return
//     4  for j = desde to |A| - 1
//     5      actual[i] = A[j]
//     6      Combinaciones(A, k, actual, i + 1, j + 1)
//
// pre:  actual tiene largo k; 0 <= i <= k; actual[0..i - 1] sale de
//       A[0..desde - 1], en orden. (En el primer llamado, i = 0 y desde = 0.)
// post: agrega al final de res las combinaciones que empiezan con
//       actual[0..i - 1] y siguen con elementos de A[desde..], en el orden del
//       pseudocódigo.
// Ejemplo: con A = [0, 1, 2, 3] y k = 3:
//   [0,1,2], [0,1,3], [0,2,3], [1,2,3]
// Complejidad: O(2^n + k * (n sobre k)).
void combinaciones(const Lista& A, size_t k, Lista& actual, size_t i, size_t desde, Listas& res);

// Ejercicio 5 — Subconjuntos
// Los subconjuntos de A, de cualquier tamaño: para cada elemento, usarlo o no.
// Son las variaciones de largo |A| de [sí, no]: usar[i] dice si el elemento
// A[i] está en el subconjunto.
// pre:  usar tiene largo |A|; 0 <= i <= |A|.
// post: agrega al final de res todos los subconjuntos que coinciden con
//       usar[0..i - 1] en los primeros i elementos. Para cada elemento se
//       prueba primero "sí" y después "no". Cada subconjunto es la lista de
//       los A[j] con usar[j] == true, en el orden de A.
// Ejemplo: con A = [1, 2, 3]:
//   [1,2,3], [1,2], [1,3], [1], [2,3], [2], [3], []
// Complejidad: O(n * 2^n).
void subconjuntos(const Lista& A, std::vector<bool>& usar, size_t i, Listas& res);

// ============================================================================
// PARTE 2 — BACKTRACKING
//
// La diferencia con la fuerza bruta: no se espera a tener un candidato
// completo para verificarlo. Si un candidato parcial ya no puede llevar a una
// solución, se abandona ahí mismo, junto con todos los que lo extienden.
//
//     Backtracking(estado)
//     1  if EsSolucion(estado)
//     2      ProcesarSolucion(estado)
//     3      return
//     4  for cada decision en DecisionesPosibles(estado)
//     5      if EsValida(decision, estado)       // la poda
//     6          Aplicar(decision, estado)       // elegir
//     7          Backtracking(estado)
//     8          Deshacer(decision, estado)      // deshacer
// ============================================================================

// ---------------------------------------------------------------------------
// El laberinto
//
// Un laberinto es una matriz de n filas por m columnas. El inicio está en la
// posición (0, 0) y el final en la posición (n - 1, m - 1).
// ---------------------------------------------------------------------------

// El contenido de una celda del laberinto.
//   - VACIO:    camino por el que es posible transitar.
//   - PARED:    pared infranqueable.
//   - HILO:     parte del camino que ya fue recorrida.
//   - VISITADA: sólo para el Ejercicio 15 (opcional).
enum Celda { VACIO, PARED, HILO, VISITADA };

typedef std::vector<std::vector<Celda>> Laberinto;

// Una posición (fila, columna). Son int (y no size_t) porque al moverse
// pueden quedar en -1: afuera del laberinto.
struct Posicion {
    int fila;
    int col;
};

// Las cuatro direcciones, en el orden de la teórica: oeste, sur, este, norte.
// Ya están hechas. Para moverse:  Posicion prox = mover(pos, DIRECCIONES[d]);
inline const Posicion DIRECCIONES[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

inline Posicion mover(Posicion pos, Posicion dir)
{
    return Posicion{pos.fila + dir.fila, pos.col + dir.col};
}

// Las operaciones del laberinto que usa el pseudocódigo. Ya están hechas.
// Contenido y Ubicar tienen como precondición que pos caiga adentro de L.
inline Celda contenido(const Laberinto& L, Posicion pos)
{
    return L[static_cast<size_t>(pos.fila)][static_cast<size_t>(pos.col)];
}

inline void ubicar(Laberinto& L, Posicion pos, Celda celda)
{
    L[static_cast<size_t>(pos.fila)][static_cast<size_t>(pos.col)] = celda;
}

inline Posicion salida(const Laberinto& L)
{
    return Posicion{static_cast<int>(L.size()) - 1, static_cast<int>(L[0].size()) - 1};
}

// Ejercicio 6 — ¿Cae adentro?
// Devuelve si la posición es una celda del laberinto.
// pre:  L tiene al menos una fila y todas sus filas tienen el mismo largo.
// post: res == true si y sólo si 0 <= pos.fila < n y 0 <= pos.col < m.
// Ojo: L.size() es un size_t y pos.fila es un int. Para compararlos,
// static_cast<int>(L.size()).
// Complejidad: O(1).
bool caeAdentro(const Laberinto& L, Posicion pos);

// Ejercicio 7 — Resolver el laberinto
// Devuelve si es posible llegar a la salida desde pos. Además, modifica el
// laberinto dejando un rastro de hilo desde pos hasta la salida.
//
//     Resolver(L, pos)
//      1  if pos = Salida(L)
//      2      return true
//      3  for cada dir en [oeste, sur, este, norte]
//      4      prox = pos + dir
//      5      if CaeAdentro(L, prox) and Contenido(L, prox) = vacio
//      6          Ubicar(L, prox, hilo)     // elegir
//      7          if Resolver(L, prox)
//      8              return true
//      9          Ubicar(L, prox, vacio)    // deshacer
//     10  return false
//
// pre:  pos es una celda de L marcada con HILO; el resto de las celdas son
//       VACIO o PARED.
// post: devuelve si se llega de pos a la salida. Si se llega, el camino que
//       encontró el pseudocódigo (probando las direcciones en ese orden) queda
//       marcado con HILO. Si no se llega, L queda como estaba.
// El hilo cumple dos funciones: es la respuesta (el camino) y es la poda (no
// volver a pisar una celda del camino actual).
// Complejidad: exponencial en la cantidad de celdas, en peor caso.
bool resolverLaberinto(Laberinto& L, Posicion pos);

// ---------------------------------------------------------------------------
// El sudoku
//
// Una matriz de n filas por n columnas (n = 9, o n = 4 en el ejemplo chico de
// la teórica). Cada celda puede estar vacía o contener un dígito entre 1 y n.
// Hay que completar todas las celdas sin dígitos repetidos en ninguna fila,
// en ninguna columna, ni en ningún cuadrante (de 3 x 3 si n = 9, de 2 x 2 si
// n = 4).
// ---------------------------------------------------------------------------

// Un sudoku: S[i][j] es el dígito de la fila i, columna j, o VACIA.
typedef std::vector<std::vector<int>> Sudoku;

const int VACIA = 0;

// El lado de un cuadrante: 3 si el sudoku es de 9 x 9, 2 si es de 4 x 4. Ya
// está hecha. El cuadrante de la celda (i, j) empieza en la fila
// (i / lado) * lado y en la columna (j / lado) * lado.
inline size_t ladoDeCuadrante(const Sudoku& S)
{
    size_t lado = 1;
    while (lado * lado < S.size()) lado++;
    return lado;
}

// Ejercicio 8 — La poda del sudoku: ¿se puede poner este dígito?
// Devuelve si poner el dígito d en la celda (i, j) no choca con nada.
// pre:  i, j < n; 1 <= d <= n; S[i][j] == VACIA.
// post: res == true si y sólo si d no aparece en la fila i, ni en la columna
//       j, ni en el cuadrante de (i, j).
// Complejidad: O(n).
bool esValido(const Sudoku& S, size_t i, size_t j, int d);

// Ejercicio 9 — Resolver el sudoku
// Completa el sudoku recorriéndolo fila por fila. En cada celda vacía se
// prueba 1, 2, ..., n: se pone el primero que no choca y se sigue con la
// celda siguiente; si más adelante no hay solución, se deshace y se prueba
// con el dígito siguiente.
// Es la actividad de la teórica: ¿cuáles son las decisiones posibles en cada
// estado? ¿Cuál es la poda? ¿Qué hay que deshacer?
// pre:  i <= n y j < n. Todas las filas anteriores a la fila i están
//       completas, y también las celdas de la fila i anteriores a la columna
//       j. Los dígitos que ya están puestos no chocan entre sí.
// post: devuelve si existe solución. Si existe, el sudoku queda completo (sin
//       cambiar los dígitos que ya estaban); si no, queda como estaba.
// Casos para pensar: si i == n ya no quedan celdas (está resuelto); la celda
// siguiente a (i, j) es (i, j + 1), salvo al final de la fila, donde es
// (i + 1, 0); y si la celda (i, j) ya tiene un dígito, se sigue de largo.
// Las pruebas hacen el primer llamado con i = 0, j = 0.
// Complejidad: exponencial en la cantidad de celdas vacías, en peor caso.
bool resolverSudoku(Sudoku& S, size_t i, size_t j);

// ---------------------------------------------------------------------------
// Suma de subconjuntos
// ---------------------------------------------------------------------------

// Ejercicio 10 — Suma de subconjuntos
// Dada una lista A de enteros no negativos y un entero k, decide si se pueden
// elegir algunos elementos de A (cada posición a lo sumo una vez) que sumen k.
// Para cada elemento hay dos decisiones posibles: usarlo o no usarlo.
//
//     HaySubconjunto(A, i, k)
//     1  if k = 0
//     2      return true
//     3  if i = |A| or k < 0      // poda: k < 0
//     4      return false
//     5  if HaySubconjunto(A, i + 1, k - A[i])     // usando A[i]
//     6      return true
//     7  return HaySubconjunto(A, i + 1, k)        // sin usar A[i]
//
// pre:  los elementos de A son no negativos; 0 <= i <= |A|. (k puede ser
//       negativo: por eso es un long long y no un size_t.)
// post: devuelve si se pueden elegir elementos de A[i..|A| - 1] que sumen k.
// Ejemplo: con A = [8, 11, 11, 8, 12, 7] y k = 29, sí: 11 + 11 + 7.
// Acá no hay nada que deshacer: el estado (i y k) viaja en los parámetros.
// Complejidad: O(2^n) en peor caso.
bool haySubconjunto(const Lista& A, size_t i, long long k);

// ============================================================================
// PARTE 3 — GREEDY
//
// Backtracking explora todo el árbol de decisiones (salvo lo que poda). Un
// algoritmo greedy baja por UNA sola rama: construye la solución paso a paso,
// eligiendo en cada paso la opción que parece mejor en ese momento, sin
// reconsiderar las decisiones ya tomadas.
// ============================================================================

// Una actividad, con su horario de inicio y de fin (inicio < fin).
struct Actividad {
    int inicio;
    int fin;
};

// "Desde cuándo está libre el aula", antes de poner ninguna actividad.
const int MENOS_INFINITO = std::numeric_limits<int>::min();

// Ejercicio 11 — Selección de actividades
// Hay un aula y n actividades. Queremos realizar la mayor cantidad posible de
// actividades, sin que dos se superpongan (una puede empezar justo cuando
// termina otra). Criterio greedy: elegir siempre, entre las compatibles con
// las ya elegidas, la que termina antes.
//
//     SeleccionarActividades(A)
//     1  ordenar A por horario de fin, de menor a mayor
//     2  S = vacío
//     3  libre = -infinito      // a partir de cuándo está libre el aula
//     4  for cada a en A
//     5      if a.inicio >= libre
//     6          agregar a a S
//     7          libre = a.fin
//     8  return S
//
// pre:  toda actividad tiene inicio < fin.
// post: res es un conjunto de actividades de A sin superposiciones, de tamaño
//       máximo, en el orden en que se eligieron (por fin creciente).
// Ejemplo (la teórica): con a = [0,6], b = [8,11], c = [1,4], d = [5,12],
// e = [4,7], f = [3,5] y g = [6,10], res es c, e, b.
// En "ejercicios.cpp" el ordenamiento (línea 1) ya está escrito.
// Complejidad: O(n log n).
std::vector<Actividad> seleccionarActividades(std::vector<Actividad> A);

// Resultado de los ejercicios del vuelto cuando no se puede dar.
const size_t IMPOSIBLE = static_cast<size_t>(-1);

// Ejercicio 12 — Cuando el greedy falla: el vuelto
// Hay que dar un vuelto de v pesos con la menor cantidad de monedas, con
// monedas de ciertas denominaciones (tantas como hagan falta de cada una).
// Greedy: usar siempre la moneda MÁS GRANDE que no se pase de lo que falta.
// Devuelve cuántas monedas usa.
// pre:  todas las monedas son > 0; monedas puede estar en cualquier orden.
// post: res es la cantidad de monedas que usa el greedy para dar exactamente
//       v (0 si v == 0); o IMPOSIBLE si en algún momento falta algo y ninguna
//       moneda entra.
// Ejemplos (la teórica): con monedas 1, 5, 10, 25 y v = 40 da 25 + 10 + 5:
// tres monedas, óptimo. Con monedas 1, 3, 4 y v = 6 da 4 + 1 + 1: tres
// monedas, pero el óptimo es 3 + 3. Falla.
// Complejidad: no hay una exigida.
size_t vueltoGreedy(const std::vector<size_t>& monedas, size_t v);

// Ejercicio 13 — El vuelto con backtracking
// "Con backtracking (probar cada moneda para lo que falta) siempre se llega al
// óptimo, pero en tiempo exponencial."
// pre:  todas las monedas son > 0; monedas puede estar en cualquier orden.
// post: res es la MENOR cantidad de monedas con las que se puede dar
//       exactamente v (0 si v == 0); o IMPOSIBLE si no hay forma de dar v.
// Idea: si v == 0 no hace falta ninguna moneda. Si no, para cada moneda m que
// no se pase de v (la poda), dar v - m de la mejor manera (el llamado
// recursivo) y sumarle 1. Quedarse con la mejor de todas las opciones.
// Ojo: IMPOSIBLE + 1 se desborda. Si un llamado recursivo devuelve IMPOSIBLE,
// esa moneda no sirve.
// Ejemplo: con monedas 1, 3, 4 y v = 6, res == 2 (3 + 3).
// Complejidad: exponencial en v, en peor caso.
size_t vueltoMinimo(const std::vector<size_t>& monedas, size_t v);

// ============================================================================
// PARTE 4 — OPCIONALES (para después)
//
// Las dos preguntas que la teórica deja picando.
// ============================================================================

// Ejercicio 14 (opcional) — ¿Cuántas aulas?
// Otro problema con las mismas actividades: hacerlas TODAS, usando la menor
// cantidad de aulas. Repetir el greedy del Ejercicio 11 no sirve (en el
// ejemplo usa 4 aulas, y alcanzan 3). El criterio greedy que sí funciona:
// ordenar por inicio y poner cada actividad en cualquier aula que ya esté
// libre; abrir una nueva sólo si están todas ocupadas.
// pre:  toda actividad tiene inicio < fin.
// post: res es la menor cantidad de aulas con las que se pueden hacer todas
//       las actividades sin que dos de la misma aula se superpongan (una
//       puede empezar justo cuando termina otra). Sin actividades, res == 0.
// Ejemplo (la teórica): con las actividades del Ejercicio 11, res == 3.
// Pista: alcanza con guardar, para cada aula abierta, desde cuándo está
// libre. En "ejercicios.cpp" el ordenamiento ya está escrito.
// Complejidad: O(n^2) alcanza.
size_t cantidadDeAulas(std::vector<Actividad> A);

// Ejercicio 15 (opcional) — El laberinto: ¿hace falta deshacer?
// Al deshacer, la celda vuelve a VACIO, y otro camino puede volver a pasar
// por ella: en el peor caso Resolver prueba todos los caminos sin celdas
// repetidas, que son exponencialmente muchos. ¿Y si al volver la marcamos
// VISITADA en lugar de VACIO?
// Es el mismo algoritmo del Ejercicio 7, cambiando sólo la línea 9: en vez de
// Ubicar(L, prox, vacio), Ubicar(L, prox, visitada).
// pre:  pos es una celda de L marcada con HILO; el resto de las celdas son
//       VACIO o PARED.
// post: devuelve si se llega de pos a la salida. Si se llega, hay un camino
//       de pos a la salida marcado con HILO. Las celdas por las que se pasó y
//       no sirvieron quedan VISITADA; las paredes no cambian.
// Para pensar: ¿sigue siendo correcto? ¿Cuánto cuesta ahora? ¿A qué algoritmo
// de grafos se parece?
// Complejidad: O(n * m).
bool resolverLaberintoSinDeshacer(Laberinto& L, Posicion pos);

#endif
