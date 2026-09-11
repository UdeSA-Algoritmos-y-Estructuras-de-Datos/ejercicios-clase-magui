#include "ejercicios.h"
#include <algorithm>  // para std::swap

// ============================================================================
// Acá va TU código. Cada función tiene una cáscara con un "TODO": borrá el
// contenido de ejemplo y escribí la implementación.
//
// Los `(void)parametro;` que ves abajo están solo para que la cáscara
// compile sin advertencias antes de que la implementes. Cuando uses el
// parámetro en tu código, borrá la línea `(void)...` correspondiente.
// ============================================================================

// ============================================================================
// MERGE SORT
// ============================================================================

// Ejercicio 1 — Merge
void merge(std::vector<int>& v, int izq, int medio, int der)
{
    // TODO: implementar merge.
    // Recordá:
    //   - Usar un vector auxiliar donde ir volcando los elementos en orden.
    //   - Dos punteros: i arranca en izq, j arranca en medio+1.
    //   - Mientras i <= medio y j <= der, comparar v[i] con v[j] y agregar
    //     al auxiliar el menor (usar <= para que sea estable).
    //   - Cuando uno de los dos se termina, volcar lo que queda del otro.
    //   - Copiar el auxiliar de vuelta a v[izq..der].
    (void)v;
    (void)izq;
    (void)medio;
    (void)der;
}

// Ejercicio 2 — Merge Sort
void mergeSort(std::vector<int>& v, int izq, int der)
{
    // TODO: implementar merge sort.
    // Recordá:
    //   - Caso base: si izq >= der, no hay nada que hacer.
    //   - Calcular medio = izq + (der - izq) / 2.
    //   - Llamar recursivamente a mergeSort para [izq, medio] y
    //     [medio+1, der].
    //   - Combinar ambas mitades con merge(v, izq, medio, der).
    (void)v;
    (void)izq;
    (void)der;
}

// Ejercicio 3 — Combinar dos vectores ordenados
std::vector<int> mergeVectoresOrdenados(const std::vector<int>& a, const std::vector<int>& b)
{
    // TODO: combinar a y b (ya ordenados) en un nuevo vector ordenado.
    // Es la misma idea que merge(), pero con dos vectores separados en vez
    // de dos sub-rangos de un mismo vector.
    (void)a;
    (void)b;
    return {};
}

// ============================================================================
// QUICK SORT
// ============================================================================

// Ejercicio 4 — Partición (esquema de Lomuto)
int partition(std::vector<int>& v, int izq, int der)
{
    // TODO: implementar la partición de Lomuto.
    // Recordá:
    //   - El pivot es v[der].
    //   - i arranca en izq - 1 (límite de la zona "menor al pivot").
    //   - Para cada j desde izq hasta der - 1: si v[j] < pivot, incrementar
    //     i e intercambiar v[i] con v[j].
    //   - Al final, intercambiar v[i+1] con v[der] (mover el pivot a su
    //     posición final) y devolver i + 1.
    (void)v;
    (void)izq;
    (void)der;
    return izq;
}

// Ejercicio 5 — Quick Sort
void quickSort(std::vector<int>& v, int izq, int der)
{
    // TODO: implementar quick sort.
    // Recordá:
    //   - Caso base: si izq >= der, no hay nada que hacer.
    //   - Particionar con p = partition(v, izq, der).
    //   - Llamar recursivamente a quickSort para [izq, p-1] y [p+1, der].
    (void)v;
    (void)izq;
    (void)der;
}

// ============================================================================
// UNA APLICACIÓN DE MERGE SORT
// ============================================================================

// Ejercicio 6 — Contar inversiones
int contarInversiones(const std::vector<int>& v)
{
    // TODO: implementar contarInversiones adaptando merge sort.
    // Pista: no modifiques el parámetro v (es const). Hacé una copia interna
    // y trabajá sobre esa copia con una función auxiliar recursiva propia
    // (podés declararla como función estática arriba de esta, antes de
    // contarInversiones, ya que no hace falta que esté en el .h).
    (void)v;
    return 0;
}

// ============================================================================
// EJERCICIOS PROPUESTOS
// ============================================================================

// Propuesto 1 — Quick Sort con mediana de tres
void quickSortMediana3(std::vector<int>& v, int izq, int der)
{
    // TODO: implementar quickSort usando mediana de tres para elegir el
    // pivot. Podés declarar tu propia función de partición auxiliar (no
    // hace falta que esté en el .h) que, antes de particionar al estilo
    // Lomuto, calcule la mediana entre v[izq], v[izq + (der-izq)/2] y
    // v[der], y la intercambie con v[der].
    (void)v;
    (void)izq;
    (void)der;
}

// Propuesto 2 — k-ésimo menor (quickselect)
int kEsimoMenor(std::vector<int>& v, int k)
{
    // TODO: implementar quickselect.
    // Pista: particioná con partition(v, izq, der). Si la posición del
    // pivot es igual a k, ese es el valor buscado. Si es mayor a k, seguí
    // buscando a la izquierda; si es menor, seguí buscando a la derecha.
    (void)v;
    (void)k;
    return -1;
}

// Propuesto 3 — Suma máxima de subarreglo (Divide & Conquer)
long long sumaMaximaSubarreglo(const std::vector<int>& v)
{
    // TODO: implementar con Divide & Conquer.
    // Pista: usá una función auxiliar recursiva sumaMaximaRec(v, izq, der)
    // que devuelva el máximo entre:
    //   - la suma máxima en la mitad izquierda,
    //   - la suma máxima en la mitad derecha,
    //   - la suma máxima que "cruza" el medio (extender hacia la izquierda
    //     desde medio y hacia la derecha desde medio+1, sumando).
    (void)v;
    return 0;
}

// Propuesto 4 — Potencia rápida (Divide & Conquer)
long long potencia(long long base, int exponente)
{
    // TODO: implementar exponenciación rápida.
    // Recordá:
    //   - Caso base: exponente == 0 devuelve 1.
    //   - Calcular mitad = potencia(base, exponente / 2).
    //   - Si exponente es par, devolver mitad * mitad.
    //   - Si exponente es impar, devolver mitad * mitad * base.
    (void)base;
    (void)exponente;
    return 1;
}
