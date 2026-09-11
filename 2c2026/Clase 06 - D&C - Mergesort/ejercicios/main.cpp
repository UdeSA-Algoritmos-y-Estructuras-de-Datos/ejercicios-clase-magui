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

int main()
{
    // ========================================================================
    // Ejercicio 1 — Merge
    // ========================================================================
    {
        // Sub-rangos [0,2] = {1,3,5} y [3,4] = {2,4}, ya ordenados cada uno.
        std::vector<int> v = {1, 3, 5, 2, 4};
        std::vector<int> esperado = {1, 2, 3, 4, 5};
        merge(v, 0, 2, 4);
        chequear("Ej1 - merge (caso general)", vectoresIguales(v, esperado));
    }
    {
        // La mitad izquierda entera es menor que la derecha.
        std::vector<int> v = {1, 2, 3, 4, 5, 6};
        std::vector<int> esperado = {1, 2, 3, 4, 5, 6};
        merge(v, 0, 2, 5);
        chequear("Ej1 - merge (ya combinado)", vectoresIguales(v, esperado));
    }
    {
        // Rango de un solo elemento a cada lado.
        std::vector<int> v = {9, 3};
        std::vector<int> esperado = {3, 9};
        merge(v, 0, 0, 1);
        chequear("Ej1 - merge (dos elementos)", vectoresIguales(v, esperado));
    }
    {
        // Con duplicados: debe ser estable (no hace falta chequear
        // estabilidad acá, solo que el resultado esté bien ordenado).
        std::vector<int> v = {2, 4, 4, 1, 3, 4};
        std::vector<int> esperado = {1, 2, 3, 4, 4, 4};
        merge(v, 0, 2, 5);
        chequear("Ej1 - merge (con duplicados)", vectoresIguales(v, esperado));
    }

    // ========================================================================
    // Ejercicio 2 — Merge Sort
    // ========================================================================
    {
        std::vector<int> v = {5, 2, 8, 1, 9};
        std::vector<int> esperado = {1, 2, 5, 8, 9};
        mergeSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej2 - mergeSort (caso general)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        std::vector<int> esperado = {1, 2, 3, 4, 5};
        mergeSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej2 - mergeSort (ya ordenado)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {5, 4, 3, 2, 1};
        std::vector<int> esperado = {1, 2, 3, 4, 5};
        mergeSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej2 - mergeSort (orden inverso)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
        std::vector<int> esperado = {1, 1, 2, 3, 4, 5, 6, 9};
        mergeSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej2 - mergeSort (con duplicados)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {};
        std::vector<int> esperado = {};
        mergeSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej2 - mergeSort (vacío)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {42};
        std::vector<int> esperado = {42};
        mergeSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej2 - mergeSort (un elemento)", vectoresIguales(v, esperado));
    }

    // ========================================================================
    // Ejercicio 3 — Combinar dos vectores ordenados
    // ========================================================================
    {
        std::vector<int> a = {1, 3, 5};
        std::vector<int> b = {2, 4, 6};
        std::vector<int> esperado = {1, 2, 3, 4, 5, 6};
        chequear("Ej3 - mergeVectoresOrdenados (caso general)",
                 vectoresIguales(mergeVectoresOrdenados(a, b), esperado));
    }
    {
        std::vector<int> a = {};
        std::vector<int> b = {1, 2, 3};
        std::vector<int> esperado = {1, 2, 3};
        chequear("Ej3 - mergeVectoresOrdenados (a vacío)",
                 vectoresIguales(mergeVectoresOrdenados(a, b), esperado));
    }
    {
        std::vector<int> a = {1, 2, 3};
        std::vector<int> b = {};
        std::vector<int> esperado = {1, 2, 3};
        chequear("Ej3 - mergeVectoresOrdenados (b vacío)",
                 vectoresIguales(mergeVectoresOrdenados(a, b), esperado));
    }
    {
        std::vector<int> a = {};
        std::vector<int> b = {};
        std::vector<int> esperado = {};
        chequear("Ej3 - mergeVectoresOrdenados (ambos vacíos)",
                 vectoresIguales(mergeVectoresOrdenados(a, b), esperado));
    }
    {
        std::vector<int> a = {2, 2, 5};
        std::vector<int> b = {2, 3};
        std::vector<int> esperado = {2, 2, 2, 3, 5};
        chequear("Ej3 - mergeVectoresOrdenados (con duplicados)",
                 vectoresIguales(mergeVectoresOrdenados(a, b), esperado));
    }

    // ========================================================================
    // Ejercicio 4 — Partición (Lomuto)
    // ========================================================================
    {
        // Pivot = v[5] = 5.
        std::vector<int> v = {4, 8, 2, 9, 1, 5};
        std::vector<int> esperado = {4, 2, 1, 5, 8, 9};
        int p = partition(v, 0, 5);
        chequear("Ej4 - partition (posición del pivot)", p == 3);
        chequear("Ej4 - partition (vector resultante)", vectoresIguales(v, esperado));
    }
    {
        // Un solo elemento: queda igual, pivot en su propia posición.
        std::vector<int> v = {7};
        int p = partition(v, 0, 0);
        chequear("Ej4 - partition (un elemento)", p == 0 && v[0] == 7);
    }

    // ========================================================================
    // Ejercicio 5 — Quick Sort
    // ========================================================================
    {
        std::vector<int> v = {5, 2, 8, 1, 9};
        std::vector<int> esperado = {1, 2, 5, 8, 9};
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej5 - quickSort (caso general)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        std::vector<int> esperado = {1, 2, 3, 4, 5};
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej5 - quickSort (ya ordenado)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {5, 4, 3, 2, 1};
        std::vector<int> esperado = {1, 2, 3, 4, 5};
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej5 - quickSort (orden inverso)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
        std::vector<int> esperado = {1, 1, 2, 3, 4, 5, 6, 9};
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej5 - quickSort (con duplicados)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {};
        std::vector<int> esperado = {};
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej5 - quickSort (vacío)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {42};
        std::vector<int> esperado = {42};
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
        chequear("Ej5 - quickSort (un elemento)", vectoresIguales(v, esperado));
    }

    // ========================================================================
    // Ejercicio 6 — Contar inversiones
    // ========================================================================
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        chequear("Ej6 - contarInversiones (ordenado, 0 inversiones)", contarInversiones(v) == 0);
    }
    {
        std::vector<int> v = {5, 4, 3, 2, 1};
        chequear("Ej6 - contarInversiones (orden inverso, máximo)", contarInversiones(v) == 10);
    }
    {
        std::vector<int> v = {2, 4, 1, 3, 5};
        chequear("Ej6 - contarInversiones (caso general)", contarInversiones(v) == 3);
    }
    {
        std::vector<int> v = {};
        chequear("Ej6 - contarInversiones (vacío)", contarInversiones(v) == 0);
    }
    {
        // No debe modificar el vector original.
        std::vector<int> v = {5, 4, 3, 2, 1};
        std::vector<int> original = {5, 4, 3, 2, 1};
        contarInversiones(v);
        chequear("Ej6 - contarInversiones (no modifica el vector)", vectoresIguales(v, original));
    }

    // ========================================================================
    // Propuesto 1 — Quick Sort con mediana de tres
    // ========================================================================
    {
        std::vector<int> v = {5, 2, 8, 1, 9};
        std::vector<int> esperado = {1, 2, 5, 8, 9};
        quickSortMediana3(v, 0, static_cast<int>(v.size()) - 1);
        chequear("P1 - quickSortMediana3 (caso general)", vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> esperado = {1, 2, 3, 4, 5, 6, 7, 8};
        quickSortMediana3(v, 0, static_cast<int>(v.size()) - 1);
        chequear("P1 - quickSortMediana3 (ya ordenado, peor caso de Lomuto puro)",
                 vectoresIguales(v, esperado));
    }
    {
        std::vector<int> v = {};
        std::vector<int> esperado = {};
        quickSortMediana3(v, 0, static_cast<int>(v.size()) - 1);
        chequear("P1 - quickSortMediana3 (vacío)", vectoresIguales(v, esperado));
    }

    // ========================================================================
    // Propuesto 2 — k-ésimo menor (quickselect)
    // ========================================================================
    {
        std::vector<int> v1 = {5, 2, 8, 1, 9};
        chequear("P2 - kEsimoMenor (k=0, el mínimo)", kEsimoMenor(v1, 0) == 1);
    }
    {
        std::vector<int> v2 = {5, 2, 8, 1, 9};
        chequear("P2 - kEsimoMenor (k=4, el máximo)", kEsimoMenor(v2, 4) == 9);
    }
    {
        std::vector<int> v3 = {5, 2, 8, 1, 9};
        chequear("P2 - kEsimoMenor (k=2, la mediana)", kEsimoMenor(v3, 2) == 5);
    }
    {
        std::vector<int> v4 = {7};
        chequear("P2 - kEsimoMenor (un elemento)", kEsimoMenor(v4, 0) == 7);
    }

    // ========================================================================
    // Propuesto 3 — Suma máxima de subarreglo
    // ========================================================================
    {
        std::vector<int> v = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        chequear("P3 - sumaMaximaSubarreglo (caso clásico)", sumaMaximaSubarreglo(v) == 6);
    }
    {
        std::vector<int> v = {-3, -1, -2};
        chequear("P3 - sumaMaximaSubarreglo (todos negativos)", sumaMaximaSubarreglo(v) == -1);
    }
    {
        std::vector<int> v = {1, 2, 3, 4};
        chequear("P3 - sumaMaximaSubarreglo (todos positivos)", sumaMaximaSubarreglo(v) == 10);
    }
    {
        std::vector<int> v = {5};
        chequear("P3 - sumaMaximaSubarreglo (un elemento)", sumaMaximaSubarreglo(v) == 5);
    }

    // ========================================================================
    // Propuesto 4 — Potencia rápida
    // ========================================================================
    {
        chequear("P4 - potencia (2^10)", potencia(2, 10) == 1024);
        chequear("P4 - potencia (exponente 0)", potencia(5, 0) == 1);
        chequear("P4 - potencia (exponente impar)", potencia(3, 5) == 243);
        chequear("P4 - potencia (base 1)", potencia(1, 100) == 1);
    }

    // ========================================================================
    // Resumen
    // ========================================================================
    std::cout << "\nResultado: " << pruebasOk << "/" << totalPruebas
              << " pruebas pasadas." << std::endl;

    return 0;
}
