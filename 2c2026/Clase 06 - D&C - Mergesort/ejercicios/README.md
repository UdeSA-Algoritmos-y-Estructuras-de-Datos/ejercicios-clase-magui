# Ejercicios - Tut 06 (Divide & Conquer - Mergesort)

Cáscara para resolver los ejercicios de la sexta clase: merge sort, quick
sort y algunas aplicaciones de Divide & Conquer.

## Archivos

- `ejercicios.h` — declaraciones de las funciones a implementar (con la descripción de cada una). **No hace falta modificarlo.**
- `ejercicios.cpp` — **acá va tu código**: cada función tiene una cáscara con un `TODO`.
- `main.cpp` — programa de prueba que chequea tus soluciones. No hace falta modificarlo.
- `Makefile` — para compilar con los flags de la materia y correr Valgrind.

## Cómo trabajar

1. Compilá el proyecto:

   ```bash
   make
   ```

2. Corré las pruebas:

   ```bash
   make run
   ```

   o, equivalente:

   ```bash
   ./ejercicios
   ```

3. Vas a ver una lista de `[PASA]` / `[FALLA]`. Al principio falla casi todo,
   porque las funciones están vacías. Implementá cada función en
   `ejercicios.cpp` y volvé a correr `make run` hasta que pasen todas.

4. **Opcional**: corré Valgrind para confirmar que no hay accesos inválidos
   a memoria (aunque en esta clase no usamos memoria dinámica explícita):

   ```bash
   make valgrind
   ```

5. Para borrar los archivos compilados:

   ```bash
   make clean
   ```

> Tip: los `(void)parametro;` en las cáscaras están solo para que el proyecto
> compile sin advertencias antes de que implementes cada función. Cuando
> empieces a usar ese parámetro, borrá la línea `(void)...`.

> Tip: si tu Mac es Apple Silicon (M1/M2/M3), `valgrind` no está disponible
> de forma nativa. En ese caso, usá:
>
> ```bash
> make docker-valgrind
> ```

## Sobre los ejercicios

Los ejercicios trabajan sobre `std::vector<int>`, siguiendo la técnica de
**Divide & Conquer**: dividir el problema en subproblemas más chicos,
resolverlos recursivamente y combinar las soluciones.

### Merge sort

- **merge**: combina dos sub-rangos ya ordenados de un mismo vector en uno solo ordenado (el paso de "combinar").
- **mergeSort**: divide el vector al medio, ordena cada mitad recursivamente y las combina con `merge`.
- **mergeVectoresOrdenados**: la misma idea de combinar, pero con dos vectores independientes en vez de dos sub-rangos.

### Quick sort

- **partition**: particiona un rango alrededor de un pivot (esquema de Lomuto), dejando los menores a la izquierda y los mayores (o iguales) a la derecha.
- **quickSort**: particiona y ordena recursivamente cada lado del pivot. A diferencia de merge sort, acá el trabajo fuerte está en "dividir" (particionar) y no hace falta combinar nada al final.

### Una aplicación de merge sort

- **contarInversiones**: cuenta pares "desordenados" del vector adaptando el paso de `merge`, en O(n log n) en vez de los O(n²) de la fuerza bruta.

### Ejercicios propuestos

- **quickSortMediana3**: mejora la elección del pivot de quick sort usando la mediana de tres elementos, para evitar el peor caso O(n²) en vectores ya ordenados.
- **kEsimoMenor**: usa la misma idea de partición de quick sort (quickselect) para encontrar el k-ésimo elemento más chico sin ordenar todo el vector.
- **sumaMaximaSubarreglo**: aplica Divide & Conquer a un problema que no es de ordenamiento, para practicar el patrón general (dividir, resolver cada mitad, combinar considerando también el caso que "cruza" el medio).
- **potencia**: exponenciación rápida con Divide & Conquer, el mismo patrón de recurrencia `T(n) = T(n/2) + O(1)` que la búsqueda binaria de los apuntes.

> Repasá el teorema maestro de los apuntes (`Tut-06 - D&C - Mergesort - apuntes.md`)
> para poder justificar la complejidad de cada uno de estos ejercicios.
