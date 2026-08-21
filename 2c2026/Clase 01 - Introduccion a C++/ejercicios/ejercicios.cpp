// ============================================================================
// Ingeniería en Inteligencia Artificial - Algoritmos y Estructura de Datos
// 2C 2026 | Docente: Ing. Magali Marijuan
// ============================================================================

#include "ejercicios.h"
#include <cmath>

// ============================================================================
// Acá va TU código. Cada función tiene una cáscara con un "TODO": borrá el
// contenido de ejemplo y escribí la implementación.
//
// Los `(void)parametro;` que ves abajo están solo para que la cáscara
// compile sin advertencias antes de que la implementes. Cuando uses el
// parámetro en tu código, borrá la línea `(void)...` correspondiente.
// ============================================================================


// Ejercicio 1 — Hola, C++
std::string saludo(const std::string &nombre, int edad) {
    // TODO: armar y devolver el saludo con el formato pedido.
    (void)nombre;
    (void)edad;
    return "";
}


// Ejercicio 2 — Par o impar
bool esPar(int numero) {
    // TODO: devolver true si numero es par.
    (void)numero;
    return false;
}


// Ejercicio 3 — Tabla de multiplicar
std::vector<int> tablaDeMultiplicar(int n) {
    // TODO: llenar el vector con n*1, n*2, ..., n*10 y devolverlo.
    (void)n;
    return {};
}


// Ejercicio 4 — esPrimo
bool esPrimo(int n) {
    // TODO: determinar si n es primo.
    (void)n;
    return false;
}


// Ejercicio 5 — swap por referencia
void swap(int &a, int &b) {
    // TODO: intercambiar los valores de a y b.
    (void)a;
    (void)b;
}


// Ejercicio 6 — Sobrecarga: maximo
int maximo(int a, int b) {
    // TODO: devolver el mayor entre a y b.
    (void)a;
    (void)b;
    return 0;
}

double maximo(double a, double b) {
    // TODO: devolver el mayor entre a y b.
    (void)a;
    (void)b;
    return 0.0;
}


// Ejercicio 7 — Promedio de un arreglo
double promedioArreglo(const int numeros[], int tamanio) {
    // TODO: sumar los elementos y dividir por tamanio (ojo con la división entera).
    (void)numeros;
    (void)tamanio;
    return 0.0;
}


// Ejercicio 8 — vector de notas
double promedio(const std::vector<double> &notas) {
    // TODO: devolver el promedio de las notas.
    (void)notas;
    return 0.0;
}

double maxima(const std::vector<double> &notas) {
    // TODO: devolver la nota más alta.
    (void)notas;
    return 0.0;
}


// Ejercicio 9 — Contar vocales
int contarVocales(const std::string &texto) {
    // TODO: contar vocales sin distinguir mayúsculas de minúsculas.
    (void)texto;
    return 0;
}


// Propuesto 1 — invertir in-place
void invertir(std::vector<int> &v) {
    // TODO: invertir el orden de los elementos sin crear otro vector.
    (void)v;
}


// Propuesto 2 — contar palabras
int contarPalabras(const std::string &oracion) {
    // TODO: contar palabras separadas por un único espacio.
    (void)oracion;
    return 0;
}


// Ejercicio 10 — Clase Punto
Punto::Punto(double xInicial, double yInicial)
{
    // TODO: inicializar x e y con los valores recibidos.
    x = xInicial;
    y = yInicial;
}

double Punto::distanciaAlOrigen() const
{
    // TODO: devolver la distancia entre este punto y el origen (0, 0).
    return sqrt((x*x) + (y*y));
}

double Punto::distanciaA(const Punto &otro) const
{
    // TODO: devolver la distancia entre este punto y "otro".
    double otroX = otro.x;
    double otroY = otro.y;
    double xFinal = x - otroX;
    double yFinal = y - otroY;
    return sqrt((xFinal*xFinal) + (yFinal*yFinal));
}

// Ejercicio 11 — Clase Rectangulo
Rectangulo::Rectangulo(double baseInicial, double alturaInicial)
{
    // TODO: inicializar base y altura con los valores recibidos.
    base = baseInicial;
    altura = alturaInicial;
}

double Rectangulo::area() const
{
    // TODO: devolver el área del rectángulo (base * altura).
    return base * altura;
}

double Rectangulo::perimetro() const
{
    // TODO: devolver el perímetro del rectángulo (2 * (base + altura)).
    return (2 * altura) + (2 * base);
}

bool Rectangulo::esCuadrado() const
{
    // TODO: devolver true si el rectángulo es un cuadrado (base == altura).
    if (base == altura) {return true;}
    return false;
}
