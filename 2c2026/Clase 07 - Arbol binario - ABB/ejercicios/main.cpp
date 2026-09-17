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

// Helper para comparar vectores
static bool vectoresIguales(const std::vector<int>& a, const std::vector<int>& b)
{
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Helper para construir el árbol de ejemplo:
//        10
//       /  \
//      5    15
//     / \   / \
//    3   7 12  20
static NodoAB<int>* construirArbolEjemplo()
{
    NodoAB<int>* raiz = new NodoAB<int>(10);
    raiz->izq = new NodoAB<int>(5);
    raiz->der = new NodoAB<int>(15);
    raiz->izq->izq = new NodoAB<int>(3);
    raiz->izq->der = new NodoAB<int>(7);
    raiz->der->izq = new NodoAB<int>(12);
    raiz->der->der = new NodoAB<int>(20);
    return raiz;
}

// Helper para liberar un árbol (usado en las pruebas, antes de implementar liberarArbol)
static void liberarArbolHelper(NodoAB<int>* nodo)
{
    if (nodo == nullptr) return;
    liberarArbolHelper(nodo->izq);
    liberarArbolHelper(nodo->der);
    delete nodo;
}

int main()
{
    // ========================================================================
    // Ejercicio 1 — Preorder
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        std::vector<int> resultado;
        std::vector<int> esperado = {10, 5, 3, 7, 15, 12, 20};
        preorder(arbol, resultado);
        chequear("Ej1 - preorder (árbol completo)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        std::vector<int> resultado;
        std::vector<int> esperado = {};
        preorder(nullptr, resultado);
        chequear("Ej1 - preorder (árbol vacío)", vectoresIguales(resultado, esperado));
    }
    {
        NodoAB<int>* arbol = new NodoAB<int>(42);
        std::vector<int> resultado;
        std::vector<int> esperado = {42};
        preorder(arbol, resultado);
        chequear("Ej1 - preorder (un solo nodo)", vectoresIguales(resultado, esperado));
        delete arbol;
    }

    // ========================================================================
    // Ejercicio 2 — Inorder
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        std::vector<int> resultado;
        std::vector<int> esperado = {3, 5, 7, 10, 12, 15, 20};  // ordenado!
        inorder(arbol, resultado);
        chequear("Ej2 - inorder (árbol completo, sale ordenado)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        std::vector<int> resultado;
        std::vector<int> esperado = {};
        inorder(nullptr, resultado);
        chequear("Ej2 - inorder (árbol vacío)", vectoresIguales(resultado, esperado));
    }

    // ========================================================================
    // Ejercicio 3 — Postorder
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        std::vector<int> resultado;
        std::vector<int> esperado = {3, 7, 5, 12, 20, 15, 10};
        postorder(arbol, resultado);
        chequear("Ej3 - postorder (árbol completo)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        std::vector<int> resultado;
        std::vector<int> esperado = {};
        postorder(nullptr, resultado);
        chequear("Ej3 - postorder (árbol vacío)", vectoresIguales(resultado, esperado));
    }

    // ========================================================================
    // Ejercicio 4 — Altura
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        chequear("Ej4 - altura (árbol de 3 niveles)", altura(arbol) == 3);
        liberarArbolHelper(arbol);
    }
    {
        chequear("Ej4 - altura (árbol vacío)", altura(nullptr) == 0);
    }
    {
        NodoAB<int>* arbol = new NodoAB<int>(42);
        chequear("Ej4 - altura (un solo nodo)", altura(arbol) == 1);
        delete arbol;
    }
    {
        // Árbol degenerado (lista)
        NodoAB<int>* arbol = new NodoAB<int>(1);
        arbol->der = new NodoAB<int>(2);
        arbol->der->der = new NodoAB<int>(3);
        arbol->der->der->der = new NodoAB<int>(4);
        chequear("Ej4 - altura (árbol degenerado)", altura(arbol) == 4);
        liberarArbolHelper(arbol);
    }

    // ========================================================================
    // Ejercicio 5 — Tamaño
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        chequear("Ej5 - tamanio (7 nodos)", tamanio(arbol) == 7);
        liberarArbolHelper(arbol);
    }
    {
        chequear("Ej5 - tamanio (árbol vacío)", tamanio(nullptr) == 0);
    }
    {
        NodoAB<int>* arbol = new NodoAB<int>(42);
        chequear("Ej5 - tamanio (un solo nodo)", tamanio(arbol) == 1);
        delete arbol;
    }

    // ========================================================================
    // Ejercicio 6 — Búsqueda en ABB
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        NodoAB<int>* encontrado = buscar(arbol, 7);
        chequear("Ej6 - buscar (valor existe)", encontrado != nullptr && encontrado->valor == 7);
        liberarArbolHelper(arbol);
    }
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        NodoAB<int>* encontrado = buscar(arbol, 10);
        chequear("Ej6 - buscar (raíz)", encontrado != nullptr && encontrado->valor == 10);
        liberarArbolHelper(arbol);
    }
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        NodoAB<int>* encontrado = buscar(arbol, 100);
        chequear("Ej6 - buscar (valor no existe)", encontrado == nullptr);
        liberarArbolHelper(arbol);
    }
    {
        NodoAB<int>* encontrado = buscar(nullptr, 5);
        chequear("Ej6 - buscar (árbol vacío)", encontrado == nullptr);
    }

    // ========================================================================
    // Ejercicio 7 — Inserción en ABB
    // ========================================================================
    {
        NodoAB<int>* arbol = nullptr;
        arbol = insertar(arbol, 10);
        arbol = insertar(arbol, 5);
        arbol = insertar(arbol, 15);
        arbol = insertar(arbol, 3);
        arbol = insertar(arbol, 7);

        std::vector<int> resultado;
        std::vector<int> esperado = {3, 5, 7, 10, 15};
        inorder(arbol, resultado);
        chequear("Ej7 - insertar (construir árbol)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        NodoAB<int>* arbol = nullptr;
        arbol = insertar(arbol, 10);
        arbol = insertar(arbol, 10);  // duplicado
        chequear("Ej7 - insertar (ignorar duplicado)", tamanio(arbol) == 1);
        liberarArbolHelper(arbol);
    }

    // ========================================================================
    // Ejercicio 8 — Mínimo en ABB
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        NodoAB<int>* min = minimo(arbol);
        chequear("Ej8 - minimo (árbol completo)", min != nullptr && min->valor == 3);
        liberarArbolHelper(arbol);
    }
    {
        NodoAB<int>* arbol = new NodoAB<int>(42);
        NodoAB<int>* min = minimo(arbol);
        chequear("Ej8 - minimo (un solo nodo)", min != nullptr && min->valor == 42);
        delete arbol;
    }

    // ========================================================================
    // Ejercicio 9 — Máximo en ABB
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        NodoAB<int>* max = maximo(arbol);
        chequear("Ej9 - maximo (árbol completo)", max != nullptr && max->valor == 20);
        liberarArbolHelper(arbol);
    }
    {
        NodoAB<int>* arbol = new NodoAB<int>(42);
        NodoAB<int>* max = maximo(arbol);
        chequear("Ej9 - maximo (un solo nodo)", max != nullptr && max->valor == 42);
        delete arbol;
    }

    // ========================================================================
    // Ejercicio 10 — Eliminación en ABB
    // ========================================================================
    {
        // Eliminar hoja
        NodoAB<int>* arbol = construirArbolEjemplo();
        arbol = eliminar(arbol, 3);
        std::vector<int> resultado;
        std::vector<int> esperado = {5, 7, 10, 12, 15, 20};
        inorder(arbol, resultado);
        chequear("Ej10 - eliminar (hoja)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        // Eliminar nodo con un hijo
        NodoAB<int>* arbol = construirArbolEjemplo();
        arbol = eliminar(arbol, 3);  // ahora 5 tiene solo hijo derecho
        arbol = eliminar(arbol, 5);
        std::vector<int> resultado;
        std::vector<int> esperado = {7, 10, 12, 15, 20};
        inorder(arbol, resultado);
        chequear("Ej10 - eliminar (un hijo)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        // Eliminar nodo con dos hijos
        NodoAB<int>* arbol = construirArbolEjemplo();
        arbol = eliminar(arbol, 10);  // raíz, tiene dos hijos
        std::vector<int> resultado;
        std::vector<int> esperado = {3, 5, 7, 12, 15, 20};
        inorder(arbol, resultado);
        chequear("Ej10 - eliminar (dos hijos, raíz)", vectoresIguales(resultado, esperado));
        liberarArbolHelper(arbol);
    }
    {
        // Eliminar valor que no existe
        NodoAB<int>* arbol = construirArbolEjemplo();
        arbol = eliminar(arbol, 100);
        chequear("Ej10 - eliminar (valor no existe)", tamanio(arbol) == 7);
        liberarArbolHelper(arbol);
    }

    // ========================================================================
    // Propuesto 1 — Verificar si es ABB
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        chequear("P1 - esABB (árbol válido)", esABB(arbol) == true);
        liberarArbolHelper(arbol);
    }
    {
        chequear("P1 - esABB (árbol vacío)", esABB(nullptr) == true);
    }
    {
        // Árbol que NO es ABB: el 11 está a la izquierda de 10 pero es mayor
        NodoAB<int>* arbol = new NodoAB<int>(10);
        arbol->izq = new NodoAB<int>(5);
        arbol->der = new NodoAB<int>(15);
        arbol->izq->der = new NodoAB<int>(11);  // 11 > 10 pero está en subárbol izq!
        chequear("P1 - esABB (violación del invariante)", esABB(arbol) == false);
        liberarArbolHelper(arbol);
    }

    // ========================================================================
    // Propuesto 2 — k-ésimo elemento en inorder
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        // Inorder: 3, 5, 7, 10, 12, 15, 20
        chequear("P2 - kEsimoInorder (k=0, primero)", kEsimoInorder(arbol, 0) == 3);
        chequear("P2 - kEsimoInorder (k=3, medio)", kEsimoInorder(arbol, 3) == 10);
        chequear("P2 - kEsimoInorder (k=6, último)", kEsimoInorder(arbol, 6) == 20);
        chequear("P2 - kEsimoInorder (k=7, fuera de rango)", kEsimoInorder(arbol, 7) == -1);
        liberarArbolHelper(arbol);
    }

    // ========================================================================
    // Propuesto 3 — Ancestro común más bajo
    // ========================================================================
    {
        NodoAB<int>* arbol = construirArbolEjemplo();
        //        10
        //       /  \
        //      5    15
        //     / \   / \
        //    3   7 12  20
        chequear("P3 - ancestroComun (3 y 7)", ancestroComun(arbol, 3, 7) == 5);
        chequear("P3 - ancestroComun (3 y 20)", ancestroComun(arbol, 3, 20) == 10);
        chequear("P3 - ancestroComun (12 y 20)", ancestroComun(arbol, 12, 20) == 15);
        chequear("P3 - ancestroComun (5 y 7)", ancestroComun(arbol, 5, 7) == 5);
        liberarArbolHelper(arbol);
    }

    // ========================================================================
    // Propuesto 4 — Liberar árbol
    // ========================================================================
    {
        // Esta prueba es difícil de verificar automáticamente sin Valgrind.
        // Solo chequeamos que no rompa.
        NodoAB<int>* arbol = construirArbolEjemplo();
        liberarArbol(arbol);
        // Si llegamos acá sin crash, consideramos que pasa.
        // Para verificar que no hay memory leaks, correr `make valgrind`.
        chequear("P4 - liberarArbol (no crashea)", true);
    }
    {
        // Liberar árbol vacío no debería hacer nada
        liberarArbol(nullptr);
        chequear("P4 - liberarArbol (árbol vacío)", true);
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
