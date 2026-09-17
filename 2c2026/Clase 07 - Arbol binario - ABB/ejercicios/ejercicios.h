#ifndef EJERCICIOS_H
#define EJERCICIOS_H

#include <vector>

// ============================================================================
// Tut 07 - Árbol binario - ABB
//
// Este archivo declara las funciones que tenés que implementar en
// "ejercicios.cpp".
//
// El programa de prueba está en "main.cpp": compilá con `make` y corré
// `./ejercicios` para ver qué ejercicios pasan y cuáles todavía fallan.
// ============================================================================

// ============================================================================
// ESTRUCTURA DEL NODO
// ============================================================================

// Nodo de un árbol binario genérico.
// Cada nodo tiene un valor y punteros a sus hijos izquierdo y derecho.
template <typename T>
struct NodoAB {
    T valor;
    NodoAB<T>* izq;
    NodoAB<T>* der;

    NodoAB(T v) : valor(v), izq(nullptr), der(nullptr) {}
};

// ============================================================================
// RECORRIDOS DE ÁRBOLES BINARIOS
// ============================================================================

// Ejercicio 1 — Preorder
// Recorre el árbol en preorder (raíz → izquierdo → derecho) y agrega los
// valores al vector resultado.
// Complejidad: O(n), donde n es la cantidad de nodos.
void preorder(NodoAB<int>* nodo, std::vector<int>& resultado);

// Ejercicio 2 — Inorder
// Recorre el árbol en inorder (izquierdo → raíz → derecho) y agrega los
// valores al vector resultado.
// En un ABB, este recorrido devuelve los elementos en orden creciente.
// Complejidad: O(n), donde n es la cantidad de nodos.
void inorder(NodoAB<int>* nodo, std::vector<int>& resultado);

// Ejercicio 3 — Postorder
// Recorre el árbol en postorder (izquierdo → derecho → raíz) y agrega los
// valores al vector resultado.
// Útil para liberar memoria (hay que eliminar hijos antes que el padre).
// Complejidad: O(n), donde n es la cantidad de nodos.
void postorder(NodoAB<int>* nodo, std::vector<int>& resultado);

// ============================================================================
// PROPIEDADES DEL ÁRBOL
// ============================================================================

// Ejercicio 4 — Altura
// Devuelve la altura del árbol: la longitud del camino más largo desde la
// raíz hasta una hoja. Un árbol vacío tiene altura 0; un solo nodo tiene
// altura 1.
// Complejidad: O(n), donde n es la cantidad de nodos.
int altura(NodoAB<int>* nodo);

// Ejercicio 5 — Tamaño
// Devuelve la cantidad de nodos del árbol.
// Complejidad: O(n), donde n es la cantidad de nodos.
int tamanio(NodoAB<int>* nodo);

// ============================================================================
// OPERACIONES DE ABB
// ============================================================================

// Ejercicio 6 — Búsqueda en ABB
// Busca un valor en el ABB. Si lo encuentra, devuelve un puntero al nodo
// que lo contiene. Si no está, devuelve nullptr.
// Precondición: el árbol cumple el invariante de ABB.
// Complejidad: O(h), donde h es la altura del árbol (O(log n) en promedio,
// O(n) en el peor caso si el árbol está desbalanceado).
NodoAB<int>* buscar(NodoAB<int>* nodo, int valor);

// Ejercicio 7 — Inserción en ABB
// Inserta un valor en el ABB manteniendo el invariante. Si el valor ya
// existe, no hace nada (no permite duplicados). Devuelve la raíz del árbol
// (puede cambiar si el árbol estaba vacío).
// Complejidad: O(h), donde h es la altura del árbol.
NodoAB<int>* insertar(NodoAB<int>* nodo, int valor);

// Ejercicio 8 — Mínimo en ABB
// Devuelve un puntero al nodo con el valor mínimo del ABB.
// Precondición: el árbol no está vacío.
// Complejidad: O(h), donde h es la altura del árbol.
NodoAB<int>* minimo(NodoAB<int>* nodo);

// Ejercicio 9 — Máximo en ABB
// Devuelve un puntero al nodo con el valor máximo del ABB.
// Precondición: el árbol no está vacío.
// Complejidad: O(h), donde h es la altura del árbol.
NodoAB<int>* maximo(NodoAB<int>* nodo);

// Ejercicio 10 — Eliminación en ABB
// Elimina un valor del ABB manteniendo el invariante. Si el valor no existe,
// no hace nada. Devuelve la raíz del árbol (puede cambiar si se elimina la
// raíz).
// Recordá los tres casos: hoja, un hijo, dos hijos (usar sucesor inorder).
// Complejidad: O(h), donde h es la altura del árbol.
NodoAB<int>* eliminar(NodoAB<int>* nodo, int valor);

// ============================================================================
// EJERCICIOS PROPUESTOS
// ============================================================================

// Propuesto 1 — Verificar si es ABB
// Devuelve true si el árbol cumple el invariante de ABB, false en caso
// contrario. Un árbol vacío es un ABB válido.
// Pista: no alcanza con comparar cada nodo solo con sus hijos directos;
// hay que verificar que TODOS los valores del subárbol izquierdo sean
// menores y TODOS los del derecho sean mayores. Usá cotas mínimas y máximas.
// Complejidad: O(n), donde n es la cantidad de nodos.
bool esABB(NodoAB<int>* nodo);

// Propuesto 2 — k-ésimo elemento en inorder
// Devuelve el valor del k-ésimo elemento en el recorrido inorder (k=0 es
// el primero, k=1 el segundo, etc.). Si k está fuera de rango, devuelve -1.
// Pista: podés hacer un recorrido inorder llevando un contador, o usar una
// función auxiliar que devuelva cuántos nodos procesó.
// Complejidad: O(n) en el peor caso.
int kEsimoInorder(NodoAB<int>* nodo, int k);

// Propuesto 3 — Ancestro común más bajo (en ABB)
// Dados dos valores que existen en el ABB, devuelve el valor del ancestro
// común más bajo (Lowest Common Ancestor, LCA). El LCA de dos nodos es el
// nodo más profundo que es ancestro de ambos.
// Pista: en un ABB, si ambos valores son menores que el nodo actual, el LCA
// está a la izquierda; si ambos son mayores, está a la derecha; si están
// "separados" (uno a cada lado), el nodo actual es el LCA.
// Precondición: ambos valores existen en el árbol.
// Complejidad: O(h), donde h es la altura del árbol.
int ancestroComun(NodoAB<int>* nodo, int valor1, int valor2);

// Propuesto 4 — Liberar árbol
// Libera toda la memoria ocupada por el árbol (todos los nodos).
// Pista: hay que liberar los hijos antes que el padre (¿qué recorrido usarías?).
// Complejidad: O(n), donde n es la cantidad de nodos.
void liberarArbol(NodoAB<int>* nodo);

#endif
