#include <algorithm>
#include <iostream>
#include <queue>
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
// Las pruebas siguen el orden de "ejercicios.h": fuerza bruta y
// enumeraciones, backtracking, greedy y, al final, los opcionales (que se
// cuentan aparte).
//
// Si el programa se queda colgado, casi seguro una recursión tuya no está
// avanzando (por ejemplo, un llamado recursivo con los mismos parámetros).
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

static void titulo(const std::string& texto)
{
    std::cout << "\n=== " << texto << " ===" << std::endl;
}

// Números pseudo-aleatorios, siempre los mismos para la misma semilla.
static unsigned int siguiente(unsigned int& semilla)
{
    semilla = semilla * 1103515245u + 12345u;
    return semilla >> 8;
}

// ----------------------------------------------------------------------------
// Helpers: enumeraciones (el primer llamado y las respuestas de referencia)
// ----------------------------------------------------------------------------

static Listas todasLasVariaciones(const Lista& A, size_t k)
{
    Lista actual(k, -99);
    Listas res;
    variaciones(A, k, actual, 0, res);
    return res;
}

static Listas todasLasPermutaciones(const Lista& A)
{
    Lista actual(A.size(), -99);
    std::vector<bool> usado(A.size(), false);
    Listas res;
    permutaciones(A, actual, usado, 0, res);
    return res;
}

static Listas todasLasCombinaciones(const Lista& A, size_t k)
{
    Lista actual(k, -99);
    Listas res;
    combinaciones(A, k, actual, 0, 0, res);
    return res;
}

static Listas todosLosSubconjuntos(const Lista& A)
{
    std::vector<bool> usar(A.size(), false);
    Listas res;
    subconjuntos(A, usar, 0, res);
    return res;
}

// Variaciones de referencia: contar en base |A|, con k cifras.
static Listas variacionesReferencia(const Lista& A, size_t k)
{
    Listas res;
    if (A.empty() && k > 0) return res;
    std::vector<size_t> cifras(k, 0);
    while (true) {
        Lista v;
        for (size_t c : cifras) v.push_back(A[c]);
        res.push_back(v);
        size_t pos = k;
        while (pos > 0 && cifras[pos - 1] + 1 == A.size()) {
            cifras[pos - 1] = 0;
            pos--;
        }
        if (pos == 0) break;
        cifras[pos - 1]++;
    }
    return res;
}

// Permutaciones de referencia: las de las posiciones, en orden lexicográfico.
static Listas permutacionesReferencia(const Lista& A)
{
    Listas res;
    std::vector<size_t> pos(A.size());
    for (size_t i = 0; i < pos.size(); i++) pos[i] = i;
    do {
        Lista p;
        for (size_t i : pos) p.push_back(A[i]);
        res.push_back(p);
    } while (std::next_permutation(pos.begin(), pos.end()));
    return res;
}

// Combinaciones de referencia: los conjuntos de k posiciones, en orden
// lexicográfico.
static Listas combinacionesReferencia(const Lista& A, size_t k)
{
    std::vector<std::vector<size_t>> conjuntos;
    size_t n = A.size();
    for (size_t mascara = 0; mascara < (static_cast<size_t>(1) << n); mascara++) {
        std::vector<size_t> posiciones;
        for (size_t i = 0; i < n; i++) {
            if ((mascara >> i) & 1) posiciones.push_back(i);
        }
        if (posiciones.size() == k) conjuntos.push_back(posiciones);
    }
    std::sort(conjuntos.begin(), conjuntos.end());
    Listas res;
    for (const std::vector<size_t>& posiciones : conjuntos) {
        Lista c;
        for (size_t i : posiciones) c.push_back(A[i]);
        res.push_back(c);
    }
    return res;
}

// Subconjuntos de referencia: primero "sí", después "no", elemento por
// elemento.
static Listas subconjuntosReferencia(const Lista& A)
{
    Listas res;
    size_t n = A.size();
    size_t total = static_cast<size_t>(1) << n;
    for (size_t k = 0; k < total; k++) {
        size_t mascara = total - 1 - k;
        Lista s;
        for (size_t i = 0; i < n; i++) {
            if ((mascara >> (n - 1 - i)) & 1) s.push_back(A[i]);
        }
        res.push_back(s);
    }
    return res;
}

static Lista listaAlAzar(size_t n, unsigned int& semilla, int maximo)
{
    Lista res;
    for (size_t i = 0; i < n; i++) res.push_back(static_cast<int>(siguiente(semilla) % static_cast<unsigned int>(maximo + 1)));
    return res;
}

// ----------------------------------------------------------------------------
// Helpers: laberinto
// ----------------------------------------------------------------------------

// Arma un laberinto a partir de un dibujo: '.' vacío, '#' pared, 'o' hilo,
// 'x' visitada.
static Laberinto laberinto(const std::vector<std::string>& dibujo)
{
    Laberinto L;
    for (const std::string& fila : dibujo) {
        std::vector<Celda> f;
        for (char c : fila) {
            if (c == '#') f.push_back(PARED);
            else if (c == 'o') f.push_back(HILO);
            else if (c == 'x') f.push_back(VISITADA);
            else f.push_back(VACIO);
        }
        L.push_back(f);
    }
    return L;
}

// El laberinto de la teórica, con el hilo ya puesto en la entrada.
static const std::vector<std::string> LABERINTO_TEORICA = {
    "o.#..",
    "#.#.#",
    "....#",
    ".##..",
    "..##.",
};

static bool adentro(const Laberinto& L, int f, int c)
{
    return f >= 0 && c >= 0 && f < static_cast<int>(L.size()) && c < static_cast<int>(L[0].size());
}

static Celda celda(const Laberinto& L, int f, int c)
{
    return L[static_cast<size_t>(f)][static_cast<size_t>(c)];
}

// ¿Se llega de (0, 0) a la salida pasando sólo por celdas que cumplen `sirve`?
// (BFS; la entrada y la salida también tienen que cumplirlo.)
static bool seLlega(const Laberinto& L, bool (*sirve)(Celda))
{
    int n = static_cast<int>(L.size()), m = static_cast<int>(L[0].size());
    if (!sirve(celda(L, 0, 0)) || !sirve(celda(L, n - 1, m - 1))) return false;
    std::vector<std::vector<bool>> visto(L.size(), std::vector<bool>(L[0].size(), false));
    std::queue<std::pair<int, int>> cola;
    cola.push(std::make_pair(0, 0));
    visto[0][0] = true;
    const int df[4] = {0, 1, 0, -1};
    const int dc[4] = {-1, 0, 1, 0};
    while (!cola.empty()) {
        int f = cola.front().first, c = cola.front().second;
        cola.pop();
        if (f == n - 1 && c == m - 1) return true;
        for (int d = 0; d < 4; d++) {
            int nf = f + df[d], nc = c + dc[d];
            if (adentro(L, nf, nc) && sirve(celda(L, nf, nc)) && !visto[static_cast<size_t>(nf)][static_cast<size_t>(nc)]) {
                visto[static_cast<size_t>(nf)][static_cast<size_t>(nc)] = true;
                cola.push(std::make_pair(nf, nc));
            }
        }
    }
    return false;
}

static bool noEsPared(Celda c) { return c != PARED; }
static bool esHilo(Celda c) { return c == HILO; }

// El backtracking de la teórica, de referencia (oeste, sur, este, norte).
static bool resolverReferencia(Laberinto& L, int f, int c)
{
    int n = static_cast<int>(L.size()), m = static_cast<int>(L[0].size());
    if (f == n - 1 && c == m - 1) return true;
    const int df[4] = {0, 1, 0, -1};
    const int dc[4] = {-1, 0, 1, 0};
    for (int d = 0; d < 4; d++) {
        int nf = f + df[d], nc = c + dc[d];
        if (adentro(L, nf, nc) && celda(L, nf, nc) == VACIO) {
            L[static_cast<size_t>(nf)][static_cast<size_t>(nc)] = HILO;
            if (resolverReferencia(L, nf, nc)) return true;
            L[static_cast<size_t>(nf)][static_cast<size_t>(nc)] = VACIO;
        }
    }
    return false;
}

// Un laberinto al azar, con el hilo ya puesto en la entrada.
static Laberinto laberintoAlAzar(size_t filas, size_t columnas, unsigned int& semilla)
{
    Laberinto L(filas, std::vector<Celda>(columnas, VACIO));
    for (size_t f = 0; f < filas; f++) {
        for (size_t c = 0; c < columnas; c++) {
            if (siguiente(semilla) % 100 < 30) L[f][c] = PARED;
        }
    }
    if (siguiente(semilla) % 4 != 0) L[filas - 1][columnas - 1] = VACIO;
    L[0][0] = HILO;
    return L;
}

// ¿Las paredes de `despues` son exactamente las de `antes`?
static bool mismasParedes(const Laberinto& antes, const Laberinto& despues)
{
    if (antes.size() != despues.size()) return false;
    for (size_t f = 0; f < antes.size(); f++) {
        if (antes[f].size() != despues[f].size()) return false;
        for (size_t c = 0; c < antes[f].size(); c++) {
            if ((antes[f][c] == PARED) != (despues[f][c] == PARED)) return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------------
// Helpers: sudoku
// ----------------------------------------------------------------------------

// El sudoku chico de la teórica (4 x 4).
static const Sudoku SUDOKU_TEORICA = {
    {2, 0, 1, 0},
    {0, 1, 2, 0},
    {1, 0, 4, 2},
    {0, 2, 3, 1},
};

static const Sudoku SUDOKU_TEORICA_RESUELTO = {
    {2, 4, 1, 3},
    {3, 1, 2, 4},
    {1, 3, 4, 2},
    {4, 2, 3, 1},
};

static const Sudoku SUDOKU_9 = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9},
};

// ¿Está completo y sin repetidos en filas, columnas y cuadrantes?
static bool sudokuResuelto(const Sudoku& S)
{
    size_t n = S.size();
    size_t lado = 1;
    while (lado * lado < n) lado++;
    for (size_t i = 0; i < n; i++) {
        if (S[i].size() != n) return false;
        std::vector<bool> fila(n + 1, false), columna(n + 1, false), cuadrante(n + 1, false);
        for (size_t j = 0; j < n; j++) {
            int enFila = S[i][j];
            int enColumna = S[j][i];
            int enCuadrante = S[(i / lado) * lado + j / lado][(i % lado) * lado + j % lado];
            int valores[3] = {enFila, enColumna, enCuadrante};
            for (int v : valores) {
                if (v < 1 || v > static_cast<int>(n)) return false;
            }
            if (fila[static_cast<size_t>(enFila)] || columna[static_cast<size_t>(enColumna)] || cuadrante[static_cast<size_t>(enCuadrante)]) return false;
            fila[static_cast<size_t>(enFila)] = true;
            columna[static_cast<size_t>(enColumna)] = true;
            cuadrante[static_cast<size_t>(enCuadrante)] = true;
        }
    }
    return true;
}

// ¿`resuelto` conserva todos los dígitos que ya estaban en `original`?
static bool conservaLosDatos(const Sudoku& original, const Sudoku& resuelto)
{
    if (original.size() != resuelto.size()) return false;
    for (size_t i = 0; i < original.size(); i++) {
        if (original[i].size() != resuelto[i].size()) return false;
        for (size_t j = 0; j < original[i].size(); j++) {
            if (original[i][j] != VACIA && original[i][j] != resuelto[i][j]) return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------------
// Helpers: suma de subconjuntos, actividades y vuelto
// ----------------------------------------------------------------------------

// ¿Hay un subconjunto de A[i..] que sume k? Probando todos.
static bool haySubconjuntoReferencia(const Lista& A, size_t i, long long k)
{
    size_t n = A.size() - i;
    for (size_t mascara = 0; mascara < (static_cast<size_t>(1) << n); mascara++) {
        long long suma = 0;
        for (size_t t = 0; t < n; t++) {
            if ((mascara >> t) & 1) suma += A[i + t];
        }
        if (suma == k) return true;
    }
    return false;
}

typedef std::vector<Actividad> Actividades;

static bool operator==(const Actividad& a, const Actividad& b)
{
    return a.inicio == b.inicio && a.fin == b.fin;
}

// Las actividades de la teórica: a, b, c, d, e, f, g.
static const Actividades ACTIVIDADES_TEORICA = {
    {0, 6}, {8, 11}, {1, 4}, {5, 12}, {4, 7}, {3, 5}, {6, 10},
};

static Actividades actividadesAlAzar(size_t n, unsigned int& semilla)
{
    Actividades res;
    for (size_t i = 0; i < n; i++) {
        int inicio = static_cast<int>(siguiente(semilla) % 12);
        int largo = 1 + static_cast<int>(siguiente(semilla) % 5);
        res.push_back(Actividad{inicio, inicio + largo});
    }
    return res;
}

// La mayor cantidad de actividades sin superposiciones, probando todos los
// subconjuntos.
static size_t maximoDeActividades(const Actividades& A)
{
    size_t n = A.size(), mejor = 0;
    for (size_t mascara = 0; mascara < (static_cast<size_t>(1) << n); mascara++) {
        bool compatibles = true;
        size_t cant = 0;
        for (size_t i = 0; i < n; i++) {
            if (((mascara >> i) & 1) == 0) continue;
            cant++;
            for (size_t j = i + 1; j < n; j++) {
                if (((mascara >> j) & 1) == 0) continue;
                if (A[i].inicio < A[j].fin && A[j].inicio < A[i].fin) compatibles = false;
            }
        }
        if (compatibles && cant > mejor) mejor = cant;
    }
    return mejor;
}

// ¿S es una selección válida de actividades de A, en orden y sin
// superposiciones?
static bool seleccionValida(const Actividades& A, const Actividades& S)
{
    std::vector<bool> usada(A.size(), false);
    for (size_t i = 0; i < S.size(); i++) {
        if (i > 0 && S[i].inicio < S[i - 1].fin) return false;
        bool esta = false;
        for (size_t j = 0; j < A.size() && !esta; j++) {
            if (!usada[j] && A[j] == S[i]) {
                usada[j] = true;
                esta = true;
            }
        }
        if (!esta) return false;
    }
    return true;
}

// La mayor cantidad de actividades que se superponen en algún momento.
static size_t maximaSuperposicion(const Actividades& A)
{
    size_t mejor = 0;
    for (const Actividad& a : A) {
        size_t cant = 0;
        for (const Actividad& b : A) {
            if (b.inicio <= a.inicio && a.inicio < b.fin) cant++;
        }
        if (cant > mejor) mejor = cant;
    }
    return mejor;
}

// Cantidad mínima de monedas para dar `v` (programación dinámica).
static size_t vueltoOptimo(const std::vector<size_t>& monedas, size_t v)
{
    std::vector<size_t> mejor(v + 1, IMPOSIBLE);
    mejor[0] = 0;
    for (size_t m = 1; m <= v; m++) {
        for (size_t c : monedas) {
            if (c <= m && mejor[m - c] != IMPOSIBLE && (mejor[m] == IMPOSIBLE || mejor[m - c] + 1 < mejor[m])) {
                mejor[m] = mejor[m - c] + 1;
            }
        }
    }
    return mejor[v];
}

// ============================================================================
// main
// ============================================================================

int main()
{
    titulo("PARTE 1 - FUERZA BRUTA Y ENUMERACIONES");

    // ========================================================================
    // Ejercicio 1 — encontrarPIN
    // ========================================================================
    {
        chequear("Ej01 - encontrarPIN (un PIN de 4 dígitos: 1234)", encontrarPIN(hashPIN(1234), 4) == 1234);
        chequear("Ej01 - encontrarPIN (el primer candidato: 0000)", encontrarPIN(hashPIN(0), 4) == 0);
        chequear("Ej01 - encontrarPIN (el último candidato: 9999)", encontrarPIN(hashPIN(9999), 4) == 9999);
        chequear("Ej01 - encontrarPIN (con ceros adelante: 0042)", encontrarPIN(hashPIN(42), 4) == 42);
        chequear("Ej01 - encontrarPIN (un solo dígito)", encontrarPIN(hashPIN(7), 1) == 7);
        chequear("Ej01 - encontrarPIN (6 dígitos: un millón de candidatos)", encontrarPIN(hashPIN(987654), 6) == 987654);
        // Estas dos pasan con la cáscara vacía: no te confíes.
        chequear("Ej01 - encontrarPIN (el PIN tiene más dígitos: NO_HAY)", encontrarPIN(hashPIN(12345), 4) == NO_HAY);
        chequear("Ej01 - encontrarPIN (10 no es un PIN de un dígito: NO_HAY)", encontrarPIN(hashPIN(10), 1) == NO_HAY);
    }

    // ========================================================================
    // Ejercicio 2 — variaciones
    // ========================================================================
    {
        chequear("Ej02 - variaciones (la teórica: A = [0, 1], k = 3)",
                 todasLasVariaciones(Lista{0, 1}, 3) ==
                     Listas{{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}});
        chequear("Ej02 - variaciones (k = 0: una sola, la vacía)", todasLasVariaciones(Lista{4, 5}, 0) == Listas{{}});
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej02 - variaciones (A vacía y k = 2: ninguna)", todasLasVariaciones(Lista{}, 2).empty());
        chequear("Ej02 - variaciones (un solo elemento)", todasLasVariaciones(Lista{5}, 3) == Listas{{5, 5, 5}});
        chequear("Ej02 - variaciones (k = 1: los elementos de A, en orden)", todasLasVariaciones(Lista{7, 3, 9}, 1) == Listas{{7}, {3}, {9}});
        chequear("Ej02 - variaciones (hay n^k: 4^5 = 1024)", todasLasVariaciones(Lista{1, 2, 3, 4}, 5).size() == 1024);

        Lista actual = {1, -99, -99};
        Listas res = {{8, 8, 8}};
        variaciones(Lista{0, 1}, 3, actual, 1, res);
        chequear("Ej02 - variaciones (desde i = 1: las que empiezan con actual[0], agregadas al final de res)",
                 res == Listas{{8, 8, 8}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}});

        bool alAzar = true;
        for (unsigned int s = 1; s <= 20; s++) {
            unsigned int semilla = s;
            Lista A = listaAlAzar(1 + s % 4, semilla, 9);
            size_t k = s % 5;
            if (todasLasVariaciones(A, k) != variacionesReferencia(A, k)) alAzar = false;
        }
        chequear("Ej02 - variaciones (listas al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 3 — permutaciones
    // ========================================================================
    {
        chequear("Ej03 - permutaciones (la teórica: A = [0, 1, 2])",
                 todasLasPermutaciones(Lista{0, 1, 2}) == Listas{{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}});
        chequear("Ej03 - permutaciones (A vacía: una sola, la vacía)", todasLasPermutaciones(Lista{}) == Listas{{}});
        chequear("Ej03 - permutaciones (un solo elemento)", todasLasPermutaciones(Lista{4}) == Listas{{4}});
        chequear("Ej03 - permutaciones (elementos repetidos: se permutan las posiciones)",
                 todasLasPermutaciones(Lista{7, 7}) == Listas{{7, 7}, {7, 7}});
        chequear("Ej03 - permutaciones (hay n!: 6! = 720)", todasLasPermutaciones(Lista{1, 2, 3, 4, 5, 6}).size() == 720);

        Lista A = {5, 6, 7};
        Lista actual(3, -99);
        std::vector<bool> usado(3, false);
        Listas res;
        permutaciones(A, actual, usado, 0, res);
        chequear("Ej03 - permutaciones (deshacer: usado queda todo en false)", res.size() == 6 && usado == std::vector<bool>{false, false, false});

        Lista actual2 = {6, -99, -99};
        std::vector<bool> usado2 = {false, true, false};
        Listas res2;
        permutaciones(A, actual2, usado2, 1, res2);
        chequear("Ej03 - permutaciones (desde i = 1: las que empiezan con actual[0])", res2 == Listas{{6, 5, 7}, {6, 7, 5}});

        bool alAzar = true;
        for (unsigned int s = 1; s <= 15; s++) {
            unsigned int semilla = s;
            Lista B = listaAlAzar(s % 6, semilla, 9);
            if (todasLasPermutaciones(B) != permutacionesReferencia(B)) alAzar = false;
        }
        chequear("Ej03 - permutaciones (listas al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 4 — combinaciones
    // ========================================================================
    {
        chequear("Ej04 - combinaciones (la teórica: A = [0, 1, 2, 3], k = 3)",
                 todasLasCombinaciones(Lista{0, 1, 2, 3}, 3) == Listas{{0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3}});
        chequear("Ej04 - combinaciones (k = 0: una sola, la vacía)", todasLasCombinaciones(Lista{4, 5}, 0) == Listas{{}});
        chequear("Ej04 - combinaciones (k = |A|: sólo A)", todasLasCombinaciones(Lista{9, 4, 6}, 3) == Listas{{9, 4, 6}});
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej04 - combinaciones (k > |A|: ninguna)", todasLasCombinaciones(Lista{1, 2}, 3).empty());
        chequear("Ej04 - combinaciones (k = 2: los pares, sin repetir y en orden)",
                 todasLasCombinaciones(Lista{5, 6, 7, 8}, 2) == Listas{{5, 6}, {5, 7}, {5, 8}, {6, 7}, {6, 8}, {7, 8}});
        chequear("Ej04 - combinaciones (hay n sobre k: 10 sobre 4 = 210)", todasLasCombinaciones(Lista{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 4).size() == 210);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 20; s++) {
            unsigned int semilla = s;
            Lista A = listaAlAzar(s % 8, semilla, 9);
            size_t k = s % 4;
            if (todasLasCombinaciones(A, k) != combinacionesReferencia(A, k)) alAzar = false;
        }
        chequear("Ej04 - combinaciones (listas al azar)", alAzar);
    }

    // ========================================================================
    // Ejercicio 5 — subconjuntos
    // ========================================================================
    {
        chequear("Ej05 - subconjuntos (A = [1, 2, 3]: primero usando cada elemento, después sin usarlo)",
                 todosLosSubconjuntos(Lista{1, 2, 3}) == Listas{{1, 2, 3}, {1, 2}, {1, 3}, {1}, {2, 3}, {2}, {3}, {}});
        chequear("Ej05 - subconjuntos (A vacía: sólo el vacío)", todosLosSubconjuntos(Lista{}) == Listas{{}});
        chequear("Ej05 - subconjuntos (un solo elemento)", todosLosSubconjuntos(Lista{4}) == Listas{{4}, {}});
        chequear("Ej05 - subconjuntos (hay 2^n: 2^10 = 1024)", todosLosSubconjuntos(Lista{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}).size() == 1024);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 15; s++) {
            unsigned int semilla = s;
            Lista A = listaAlAzar(s % 7, semilla, 9);
            if (todosLosSubconjuntos(A) != subconjuntosReferencia(A)) alAzar = false;
        }
        chequear("Ej05 - subconjuntos (listas al azar)", alAzar);
    }

    titulo("PARTE 2 - BACKTRACKING");

    // ========================================================================
    // Ejercicio 6 — caeAdentro
    // ========================================================================
    {
        Laberinto L = laberinto({"...", "..."}); // 2 filas, 3 columnas
        chequear("Ej06 - caeAdentro (la entrada y la salida)", caeAdentro(L, Posicion{0, 0}) && caeAdentro(L, Posicion{1, 2}));
        chequear("Ej06 - caeAdentro (una celda del medio)", caeAdentro(L, Posicion{1, 1}));
        // Las que siguen pasan con la cáscara vacía: no te confíes.
        chequear("Ej06 - caeAdentro (al oeste de la entrada: columna -1)", !caeAdentro(L, Posicion{0, -1}));
        chequear("Ej06 - caeAdentro (al norte de la entrada: fila -1)", !caeAdentro(L, Posicion{-1, 0}));
        chequear("Ej06 - caeAdentro (al sur de la salida: fila n)", !caeAdentro(L, Posicion{2, 2}));
        chequear("Ej06 - caeAdentro (al este de la salida: columna m)", !caeAdentro(L, Posicion{1, 3}));
        chequear("Ej06 - caeAdentro (no es cuadrado: la fila 2 no existe, la columna 2 sí)", !caeAdentro(L, Posicion{2, 0}) && caeAdentro(L, Posicion{0, 2}));
    }

    // ========================================================================
    // Ejercicio 7 — resolverLaberinto
    // ========================================================================
    {
        Laberinto L = laberinto(LABERINTO_TEORICA);
        bool hay = resolverLaberinto(L, Posicion{0, 0});
        chequear("Ej07 - resolverLaberinto (la teórica: se llega a la salida)", hay);
        chequear("Ej07 - resolverLaberinto (la teórica: el hilo queda sobre el camino encontrado)",
                 L == laberinto({"oo#..", "#o#.#", ".ooo#", ".##oo", "..##o"}));
        chequear("Ej07 - resolverLaberinto (la teórica: lo que se deshizo vuelve a quedar vacío)",
                 hay && L[2][0] == VACIO && L[3][0] == VACIO && L[4][0] == VACIO && L[4][1] == VACIO);

        Laberinto uno = laberinto({"o"});
        chequear("Ej07 - resolverLaberinto (una sola celda: ya está en la salida)", resolverLaberinto(uno, Posicion{0, 0}) && uno == laberinto({"o"}));

        Laberinto pasillo = laberinto({"o..."});
        chequear("Ej07 - resolverLaberinto (un pasillo)", resolverLaberinto(pasillo, Posicion{0, 0}) && pasillo == laberinto({"oooo"}));

        // Las dos que siguen pasan a medias con la cáscara vacía: no te confíes.
        Laberinto cerrado = laberinto({"o.#", ".##", "#.."});
        chequear("Ej07 - resolverLaberinto (sin salida: false, y el laberinto queda como estaba)",
                 !resolverLaberinto(cerrado, Posicion{0, 0}) && cerrado == laberinto({"o.#", ".##", "#.."}));

        Laberinto salidaTapada = laberinto({"o..", "..#"});
        chequear("Ej07 - resolverLaberinto (la salida es una pared: false)",
                 !resolverLaberinto(salidaTapada, Posicion{0, 0}) && salidaTapada == laberinto({"o..", "..#"}));

        Laberinto abierto = laberinto({"o..", "...", "..."});
        chequear("Ej07 - resolverLaberinto (sin paredes: primero al sur, después al este)",
                 resolverLaberinto(abierto, Posicion{0, 0}) && abierto == laberinto({"o..", "o..", "ooo"}));

        Laberinto desdeElMedio = laberinto({"...", ".o.", "..."});
        chequear("Ej07 - resolverLaberinto (desde otra posición: primero al oeste)",
                 resolverLaberinto(desdeElMedio, Posicion{1, 1}) && desdeElMedio == laberinto({"...", "oo.", "ooo"}));

        bool respuesta = true, hilo = true;
        for (unsigned int s = 1; s <= 40; s++) {
            unsigned int semilla = s;
            Laberinto original = laberintoAlAzar(1 + s % 4, 1 + s % 5, semilla);
            Laberinto esperado = original;
            bool seLlegaRef = resolverReferencia(esperado, 0, 0);
            Laberinto tuyo = original;
            if (resolverLaberinto(tuyo, Posicion{0, 0}) != seLlegaRef) respuesta = false;
            if (tuyo != esperado) hilo = false;
        }
        chequear("Ej07 - resolverLaberinto (laberintos al azar: se llega o no)", respuesta);
        chequear("Ej07 - resolverLaberinto (laberintos al azar: el hilo, o el laberinto intacto)", respuesta && hilo);
    }

    // ========================================================================
    // Ejercicio 8 — esValido
    // ========================================================================
    {
        Sudoku S(9, std::vector<int>(9, VACIA));
        S[0][0] = 5;
        chequear("Ej08 - esValido (un dígito que no está en ningún lado)", esValido(S, 4, 4, 5) && esValido(S, 0, 1, 3));
        chequear("Ej08 - esValido (otra fila, otra columna y otro cuadrante)", esValido(S, 1, 3, 5) && esValido(S, 3, 1, 5));
        // Las que dicen "no" pasan con la cáscara vacía: no te confíes.
        chequear("Ej08 - esValido (ya está en la fila -> no)", !esValido(S, 0, 8, 5));
        chequear("Ej08 - esValido (ya está en la columna -> no)", !esValido(S, 8, 0, 5));
        chequear("Ej08 - esValido (ya está en el cuadrante, en otra fila y otra columna -> no)", !esValido(S, 1, 1, 5) && !esValido(S, 2, 2, 5));

        S[4][7] = 9;
        chequear("Ej08 - esValido (un cuadrante del medio: filas 3 a 5, columnas 6 a 8)",
                 !esValido(S, 3, 6, 9) && !esValido(S, 5, 8, 9) && esValido(S, 6, 6, 9) && esValido(S, 3, 5, 9));

        chequear("Ej08 - esValido (la teórica: en (0, 1) entran el 3 y el 4, no el 1 ni el 2)",
                 esValido(SUDOKU_TEORICA, 0, 1, 3) && esValido(SUDOKU_TEORICA, 0, 1, 4) &&
                     !esValido(SUDOKU_TEORICA, 0, 1, 1) && !esValido(SUDOKU_TEORICA, 0, 1, 2));

        Sudoku chico(4, std::vector<int>(4, VACIA));
        chico[3][3] = 2;
        chequear("Ej08 - esValido (4 x 4: los cuadrantes son de 2 x 2)", !esValido(chico, 2, 2, 2) && esValido(chico, 1, 1, 2) && esValido(chico, 2, 1, 2));
    }

    // ========================================================================
    // Ejercicio 9 — resolverSudoku
    // ========================================================================
    {
        Sudoku S = SUDOKU_TEORICA;
        bool hay = resolverSudoku(S, 0, 0);
        chequear("Ej09 - resolverSudoku (la teórica, 4 x 4: tiene solución)", hay);
        chequear("Ej09 - resolverSudoku (la teórica: queda completo, con (0,1) = 4 y (2,1) = 3)", S == SUDOKU_TEORICA_RESUELTO);

        // El primer intento de la teórica: con (0, 1) = 3 no hay solución.
        Sudoku sinSolucion = SUDOKU_TEORICA;
        sinSolucion[0][1] = 3;
        Sudoku copia = sinSolucion;
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej09 - resolverSudoku (la teórica con (0,1) = 3: no hay solución, y queda como estaba)",
                 !resolverSudoku(sinSolucion, 0, 0) && sinSolucion == copia);

        Sudoku completo = SUDOKU_TEORICA_RESUELTO;
        chequear("Ej09 - resolverSudoku (ya está completo: true, sin tocar nada)", resolverSudoku(completo, 0, 0) && completo == SUDOKU_TEORICA_RESUELTO);

        Sudoku vacio(4, std::vector<int>(4, VACIA));
        chequear("Ej09 - resolverSudoku (4 x 4 todo vacío: alguna solución)", resolverSudoku(vacio, 0, 0) && sudokuResuelto(vacio));

        Sudoku desdeLaFila2 = {{2, 4, 1, 3}, {3, 1, 2, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}};
        Sudoku original2 = desdeLaFila2;
        chequear("Ej09 - resolverSudoku (llamado desde (2, 0), con las dos primeras filas completas)",
                 resolverSudoku(desdeLaFila2, 2, 0) && sudokuResuelto(desdeLaFila2) && conservaLosDatos(original2, desdeLaFila2));

        Sudoku grande = SUDOKU_9;
        bool hayGrande = resolverSudoku(grande, 0, 0);
        chequear("Ej09 - resolverSudoku (9 x 9: tiene solución)", hayGrande);
        chequear("Ej09 - resolverSudoku (9 x 9: queda completo y sin repetidos)", hayGrande && sudokuResuelto(grande));
        chequear("Ej09 - resolverSudoku (9 x 9: no cambia los dígitos que ya estaban)", hayGrande && conservaLosDatos(SUDOKU_9, grande));

        Sudoku grandeSinSolucion = SUDOKU_9;
        grandeSinSolucion[0][2] = 1; // la solución (única) tiene un 4 ahí
        Sudoku copiaGrande = grandeSinSolucion;
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej09 - resolverSudoku (9 x 9 con un dígito mal puesto: no hay solución, y queda como estaba)",
                 !resolverSudoku(grandeSinSolucion, 0, 0) && grandeSinSolucion == copiaGrande);
    }

    // ========================================================================
    // Ejercicio 10 — haySubconjunto
    // ========================================================================
    {
        Lista teorica = {8, 11, 11, 8, 12, 7};
        chequear("Ej10 - haySubconjunto (la teórica: k = 29 con 11 + 11 + 7)", haySubconjunto(teorica, 0, 29));
        chequear("Ej10 - haySubconjunto (la teórica: todos los elementos, k = 57)", haySubconjunto(teorica, 0, 57));
        Lista chica = {3, 5, 2};
        chequear("Ej10 - haySubconjunto (A = [3, 5, 2], k = 5: el árbol de la teórica)", haySubconjunto(chica, 0, 5));
        chequear("Ej10 - haySubconjunto (k = 0: siempre, sin elegir nada)", haySubconjunto(chica, 0, 0) && haySubconjunto(Lista{}, 0, 0));
        chequear("Ej10 - haySubconjunto (desde i = 1 sólo se pueden usar el 5 y el 2: k = 7)", haySubconjunto(chica, 1, 7));
        // Las que dicen "no" pasan con la cáscara vacía: no te confíes.
        chequear("Ej10 - haySubconjunto (la teórica: k = 6 no se puede)", !haySubconjunto(teorica, 0, 6));
        chequear("Ej10 - haySubconjunto (A = [3, 5, 2]: k = 4 no se puede, k = 11 se pasa del total)", !haySubconjunto(chica, 0, 4) && !haySubconjunto(chica, 0, 11));
        chequear("Ej10 - haySubconjunto (desde i = 1 ya no se puede usar el 3: k = 3)", !haySubconjunto(chica, 1, 3));
        chequear("Ej10 - haySubconjunto (cada posición a lo sumo una vez: [4], k = 8)", !haySubconjunto(Lista{4}, 0, 8));
        chequear("Ej10 - haySubconjunto (A vacía y k > 0)", !haySubconjunto(Lista{}, 0, 5));
        chequear("Ej10 - haySubconjunto (k negativo)", !haySubconjunto(chica, 0, -2));

        bool alAzar = true;
        size_t si = 0, no = 0;
        for (unsigned int s = 1; s <= 60; s++) {
            unsigned int semilla = s;
            Lista A = listaAlAzar(s % 9, semilla, 12);
            long long k = static_cast<long long>(siguiente(semilla) % 40);
            size_t i = A.empty() ? 0 : s % (A.size() + 1);
            bool esperado = haySubconjuntoReferencia(A, i, k);
            if (esperado) si++; else no++;
            if (haySubconjunto(A, i, k) != esperado) alAzar = false;
        }
        chequear("Ej10 - haySubconjunto (listas al azar, con ceros y repetidos)", alAzar && si > 0 && no > 0);
    }

    titulo("PARTE 3 - GREEDY");

    // ========================================================================
    // Ejercicio 11 — seleccionarActividades
    // ========================================================================
    {
        chequear("Ej11 - seleccionarActividades (la teórica: c, e, b)",
                 seleccionarActividades(ACTIVIDADES_TEORICA) == Actividades{{1, 4}, {4, 7}, {8, 11}});
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej11 - seleccionarActividades (sin actividades)", seleccionarActividades(Actividades{}).empty());
        chequear("Ej11 - seleccionarActividades (una sola)", seleccionarActividades(Actividades{{3, 9}}) == Actividades{{3, 9}});
        chequear("Ej11 - seleccionarActividades (una empieza justo cuando termina otra: entran las dos)",
                 seleccionarActividades(Actividades{{5, 8}, {2, 5}}) == Actividades{{2, 5}, {5, 8}});
        chequear("Ej11 - seleccionarActividades (todas superpuestas: una sola, la que termina antes)",
                 seleccionarActividades(Actividades{{0, 10}, {2, 6}, {1, 8}}) == Actividades{{2, 6}});
        chequear("Ej11 - seleccionarActividades (elegir la más corta fallaría: [0,5], [4,6], [5,10])",
                 seleccionarActividades(Actividades{{0, 5}, {4, 6}, {5, 10}}) == Actividades{{0, 5}, {5, 10}});
        chequear("Ej11 - seleccionarActividades (elegir la que empieza antes fallaría: [0,10], [1,2], [3,4])",
                 seleccionarActividades(Actividades{{0, 10}, {1, 2}, {3, 4}}) == Actividades{{1, 2}, {3, 4}});
        chequear("Ej11 - seleccionarActividades (horarios negativos)",
                 seleccionarActividades(Actividades{{-5, -2}, {-9, -6}}) == Actividades{{-9, -6}, {-5, -2}});

        bool valida = true, maxima = true;
        for (unsigned int s = 1; s <= 40; s++) {
            unsigned int semilla = s;
            Actividades A = actividadesAlAzar(1 + s % 9, semilla);
            Actividades S = seleccionarActividades(A);
            if (!seleccionValida(A, S)) valida = false;
            if (S.size() != maximoDeActividades(A)) maxima = false;
        }
        chequear("Ej11 - seleccionarActividades (al azar: son actividades de A, en orden y sin superposiciones)", valida && maxima);
        chequear("Ej11 - seleccionarActividades (al azar: la cantidad es la máxima, contra fuerza bruta)", maxima);
    }

    // ========================================================================
    // Ejercicio 12 — vueltoGreedy
    // ========================================================================
    {
        std::vector<size_t> usuales = {1, 5, 10, 25};
        chequear("Ej12 - vueltoGreedy (la teórica: monedas 1, 5, 10, 25; v = 40 -> 3: 25 + 10 + 5)", vueltoGreedy(usuales, 40) == 3);
        chequear("Ej12 - vueltoGreedy (monedas 1, 5, 10, 25; v = 63 -> 6)", vueltoGreedy(usuales, 63) == 6);
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej12 - vueltoGreedy (v = 0 -> 0 monedas)", vueltoGreedy(usuales, 0) == 0);
        chequear("Ej12 - vueltoGreedy (una moneda justa)", vueltoGreedy(usuales, 25) == 1);
        chequear("Ej12 - vueltoGreedy (las monedas en cualquier orden)", vueltoGreedy(std::vector<size_t>{25, 1, 10, 5}, 63) == 6);

        std::vector<size_t> raras = {1, 3, 4};
        chequear("Ej12 - vueltoGreedy (la teórica: monedas 1, 3, 4; v = 6 -> 3: 4 + 1 + 1, aunque 3 + 3 es mejor)", vueltoGreedy(raras, 6) == 3);
        chequear("Ej12 - vueltoGreedy (sin moneda que entre -> IMPOSIBLE)", vueltoGreedy(std::vector<size_t>{5, 10}, 3) == IMPOSIBLE);
        chequear("Ej12 - vueltoGreedy (el greedy se queda trabado: 5, 2 para dar 6 -> IMPOSIBLE)", vueltoGreedy(std::vector<size_t>{5, 2}, 6) == IMPOSIBLE);

        bool optimo = true;
        for (size_t v = 0; v <= 100; v++) {
            if (vueltoGreedy(usuales, v) != vueltoOptimo(usuales, v)) optimo = false;
        }
        chequear("Ej12 - vueltoGreedy (con 1, 5, 10, 25 coincide con el óptimo de 0 a 100)", optimo);
    }

    // ========================================================================
    // Ejercicio 13 — vueltoMinimo
    // ========================================================================
    {
        std::vector<size_t> raras = {1, 3, 4};
        chequear("Ej13 - vueltoMinimo (la teórica: monedas 1, 3, 4; v = 6 -> 2: 3 + 3)", vueltoMinimo(raras, 6) == 2);
        chequear("Ej13 - vueltoMinimo (la teórica: monedas 1, 5, 10, 25; v = 40 -> 3)", vueltoMinimo(std::vector<size_t>{1, 5, 10, 25}, 40) == 3);
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej13 - vueltoMinimo (v = 0 -> 0 monedas)", vueltoMinimo(raras, 0) == 0);
        chequear("Ej13 - vueltoMinimo (una moneda justa)", vueltoMinimo(raras, 4) == 1);
        chequear("Ej13 - vueltoMinimo (donde el greedy se traba: 5, 2 para dar 6 -> 3: 2 + 2 + 2)", vueltoMinimo(std::vector<size_t>{5, 2}, 6) == 3);
        chequear("Ej13 - vueltoMinimo (no hay forma: 5, 10 para dar 3 -> IMPOSIBLE)", vueltoMinimo(std::vector<size_t>{5, 10}, 3) == IMPOSIBLE);
        chequear("Ej13 - vueltoMinimo (no hay forma: 4, 6 para dar 7 -> IMPOSIBLE)", vueltoMinimo(std::vector<size_t>{4, 6}, 7) == IMPOSIBLE);
        chequear("Ej13 - vueltoMinimo (las monedas en cualquier orden: 4, 1, 3 para dar 10 -> 3)", vueltoMinimo(std::vector<size_t>{4, 1, 3}, 10) == 3);

        bool alAzar = true;
        const std::vector<std::vector<size_t>> juegos = {{1, 3, 4}, {2, 5}, {1, 5, 10}, {3, 7}, {4, 1, 6}, {2}, {5, 3, 8}};
        for (const std::vector<size_t>& monedas : juegos) {
            for (size_t v = 0; v <= 18; v++) {
                if (vueltoMinimo(monedas, v) != vueltoOptimo(monedas, v)) alAzar = false;
            }
        }
        chequear("Ej13 - vueltoMinimo (varios juegos de monedas, de 0 a 18: siempre el óptimo)", alAzar);
    }

    // Las pruebas obligatorias llegan hasta acá.
    int totalObligatorias = totalPruebas;
    int okObligatorias = pruebasOk;

    titulo("PARTE 4 - OPCIONALES");

    // ========================================================================
    // Ejercicio 14 (opcional) — cantidadDeAulas
    // ========================================================================
    {
        chequear("Ej14 (opcional) - cantidadDeAulas (la teórica: alcanzan 3)", cantidadDeAulas(ACTIVIDADES_TEORICA) == 3);
        // Esta pasa con la cáscara vacía: no te confíes.
        chequear("Ej14 (opcional) - cantidadDeAulas (sin actividades: 0)", cantidadDeAulas(Actividades{}) == 0);
        chequear("Ej14 (opcional) - cantidadDeAulas (una sola: 1)", cantidadDeAulas(Actividades{{3, 9}}) == 1);
        chequear("Ej14 (opcional) - cantidadDeAulas (una atrás de la otra: 1)", cantidadDeAulas(Actividades{{5, 8}, {0, 2}, {2, 5}}) == 1);
        chequear("Ej14 (opcional) - cantidadDeAulas (todas superpuestas: una por actividad)", cantidadDeAulas(Actividades{{0, 10}, {2, 6}, {1, 8}, {5, 7}}) == 4);
        chequear("Ej14 (opcional) - cantidadDeAulas (un aula se libera y se vuelve a usar)", cantidadDeAulas(Actividades{{0, 4}, {1, 3}, {3, 6}, {4, 8}}) == 2);

        bool alAzar = true;
        for (unsigned int s = 1; s <= 40; s++) {
            unsigned int semilla = s;
            Actividades A = actividadesAlAzar(1 + s % 12, semilla);
            if (cantidadDeAulas(A) != maximaSuperposicion(A)) alAzar = false;
        }
        chequear("Ej14 (opcional) - cantidadDeAulas (al azar: tantas como actividades superpuestas en el peor momento)", alAzar);
    }

    // ========================================================================
    // Ejercicio 15 (opcional) — resolverLaberintoSinDeshacer
    // ========================================================================
    {
        Laberinto L = laberinto(LABERINTO_TEORICA);
        bool hay = resolverLaberintoSinDeshacer(L, Posicion{0, 0});
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (la teórica: se llega a la salida)", hay);
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (la teórica: el camino con hilo, y lo que no sirvió queda visitada)",
                 L == laberinto({"oo#..", "#o#.#", "xooo#", "x##oo", "xx##o"}));

        Laberinto cerrado = laberinto({"o.#", ".##", "#.."});
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (sin salida: false, y todo lo alcanzable queda visitada)",
                 !resolverLaberintoSinDeshacer(cerrado, Posicion{0, 0}) && cerrado == laberinto({"ox#", "x##", "#.."}));

        Laberinto uno = laberinto({"o"});
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (una sola celda)", resolverLaberintoSinDeshacer(uno, Posicion{0, 0}) && uno == laberinto({"o"}));

        bool respuesta = true, camino = true;
        for (unsigned int s = 1; s <= 40; s++) {
            unsigned int semilla = s;
            Laberinto original = laberintoAlAzar(1 + s % 4, 1 + s % 5, semilla);
            bool esperado = seLlega(original, noEsPared);
            Laberinto tuyo = original;
            bool res = resolverLaberintoSinDeshacer(tuyo, Posicion{0, 0});
            if (res != esperado) respuesta = false;
            if (!mismasParedes(original, tuyo)) camino = false;
            if (esperado && !seLlega(tuyo, esHilo)) camino = false;
        }
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (laberintos al azar: se llega o no)", respuesta);
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (laberintos al azar: hay un camino de hilo y no se tocan las paredes)", respuesta && camino);

        // Sin paredes y sin salida alcanzable, el Ejercicio 7 probaría una
        // cantidad exponencial de caminos. Sin deshacer, cada celda se visita
        // una sola vez.
        Laberinto grande(30, std::vector<Celda>(30, VACIO));
        grande[0][0] = HILO;
        grande[29][28] = PARED;
        grande[28][29] = PARED;
        bool resGrande = resolverLaberintoSinDeshacer(grande, Posicion{0, 0});
        chequear("Ej15 (opcional) - resolverLaberintoSinDeshacer (30 x 30 con la salida encerrada: termina enseguida, y da false)",
                 !resGrande && grande[15][15] == VISITADA && grande[29][29] == VACIO);
    }

    // ========================================================================
    // Resumen
    // ========================================================================
    std::cout << "\nResultado (Partes 1 a 3): " << okObligatorias << "/" << totalObligatorias
              << " pruebas pasadas." << std::endl;
    std::cout << "Opcionales (Parte 4):     " << (pruebasOk - okObligatorias) << "/"
              << (totalPruebas - totalObligatorias) << " pruebas pasadas." << std::endl;

    if (pruebasOk == totalPruebas)
    {
        std::cout << "\nPara verificar que no hay memory leaks, corré: make valgrind" << std::endl;
    }

    return 0;
}
