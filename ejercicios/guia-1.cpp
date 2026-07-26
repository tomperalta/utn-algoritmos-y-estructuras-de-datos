#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

/**
 * EJERCICIO 1
 * Dados los catetos a y b, ambos reales y positivos,
 * de un triángulo rectángulo, hallar la medida de su hipotenusa.
 */
int ejercicio1(float cateto1, float cateto2) {
  cout << "EJERCICIO 1" << endl;
  cout << "Dados los catetos a y b, ambos reales y positivos, de un triángulo rectángulo, hallar la medida de su hipotenusa." << endl;
  cout << "=====" << endl;
  cout << "Cateto 1: " << cateto1 << endl;
  cout << "Cateto 2: " << cateto2 << endl;
  cout << "=====" << endl;

  float hipotenusa = sqrt(pow(cateto1, 2) + pow(cateto2, 2));
  cout << "Hipotenusa: " << hipotenusa << endl;

  return hipotenusa;
}

/**
 * EJERCICIO 2
 * Dados dos valores a y b, enteros y distintos,
 * emitir un cartel apropiado que informe cuál es el mayor entre ellos.
 */
void ejercicio2(int a, int b) {
  cout << "EJERCICIO 2" << endl;
  cout << "Dados dos valores a y b, enteros y distintos, emitir un cartel apropiado que informe cuál es el mayor entre ellos." << endl;
  cout << "=====" << endl;
  cout << "a: " << a << endl;
  cout << "b: " << b << endl;
  cout << "=====" << endl;
  
  if (a == b) {
    cout << "[ERROR] a y b deben ser distintos" << endl;
  } else if (a > b) {
    cout << "a es mayor a b" << endl;
  } else {
    cout << "b es mayor a a" << endl;
  }
}

/**
 * EJERCICIO 3
 * Dados los catetos de un triángulo rectángulo, hallar la superficie de un
 * cuadrado siendo uno de sus lados la medida de su hipotenusa:
 * - Si la hipotenusa es mayor a 100 → usar la hipotenusa como lado.
 * - Caso contrario → usar el doble de la hipotenusa como lado.
 */
void ejercicio3(float cateto1, float cateto2) {
  cout << "EJERCICIO 3" << endl;
  cout << "Dados los catetos de un triángulo rectángulo, hallar la superficie de un cuadrado siendo uno de sus lados la medida de su hipotenusa:" << endl;
  cout << "- Si la hipotenusa es mayor a 100 → usar la hipotenusa como lado." << endl;
  cout << "- Caso contrario → usar el doble de la hipotenusa como lado." << endl;
  cout << "=====" << endl;
  cout << "Cateto 1: " << cateto1 << endl;
  cout << "Cateto 2: " << cateto2 << endl;
  cout << "=====" << endl;

  float hipotenusa = sqrt(pow(cateto1, 2) + pow(cateto2, 2));
  float lado;


  if (hipotenusa > 100) {
    lado = hipotenusa;
  } else {
    lado = hipotenusa * 2;
  }

  float superficieCuadrado = pow(lado, 2);

  cout << "Hipotenusa: " << hipotenusa << endl;
  cout << "Lado: " << lado << endl;
  cout << "Superficie cuadrado: " << superficieCuadrado << endl;
}

/**
 * EJERCICIO 4
 * Hallar la medida de la hipotenusa de cada triángulo rectángulo sabiendo las medidas de sus catetos.
 * El proceso finaliza cuando se ingrese un valor menor o igual a cero. Informar:
 * - Cantidad de triángulos y porcentaje con hipotenusa mayor a 100.
 * - Promedio de las medidas de las hipotenusas.
 * - Cantidad de triángulos y porcentaje con hipotenusa menor o igual a 100.
 */
void ejercicio4() {
  cout << "EJERCICIO 4" << endl;
  cout << "Hallar la medida de la hipotenusa de cada triángulo rectángulo sabiendo las medidas de sus catetos." << endl;
  cout << "El proceso finaliza cuando se ingrese un valor menor o igual a cero. Informar:" << endl;
  cout << "- Cantidad de triángulos y porcentaje con hipotenusa mayor a 100." << endl;
  cout << "- Cantidad de triángulos y porcentaje con hipotenusa menor o igual a 100." << endl;
  cout << "- Promedio de las medidas de las hipotenusas." << endl;
  cout << "=====" << endl;

  int cantidadTriangulos = 0;
  int cantidadTriangulosHipotenusaMayor100 = 0;
  float porcentajeHipotenusasMayor100 = 0;
  float promedioHipotenusas = 0;

  float cateto1, cateto2; 
  cout << "[Triángulo " << cantidadTriangulos + 1 << "]" << " Ingresá cateto 1: " << endl;
  cin  >> cateto1;

  while (cateto1 > 0) {
    cout << "[Triángulo " << cantidadTriangulos + 1 << "]" << " Ingresá cateto 2: " << endl;
    cin >> cateto2;

    if (cateto2 <= 0) {
      cateto1 = 0;
      break;
    }

    cantidadTriangulos++;
    float hipotenusa = sqrt(pow(cateto1, 2) + pow(cateto2, 2));
    promedioHipotenusas = (promedioHipotenusas + hipotenusa) / cantidadTriangulos;

    if (hipotenusa > 100) {
      cantidadTriangulosHipotenusaMayor100++;
      porcentajeHipotenusasMayor100 = cantidadTriangulosHipotenusaMayor100 * 100 / cantidadTriangulos;
    }

    cout << "[Triángulo " << cantidadTriangulos + 1 << "]" << " Ingresá cateto 1: " << endl;
    cin >> cateto1;
  }

  cout << "=====" << endl;
  cout << "Cantidad de triángulos y porcentaje con hipotenusa mayor a 100: " << cantidadTriangulosHipotenusaMayor100 << "(" << porcentajeHipotenusasMayor100 << "%)" << endl;
  cout << "Cantidad de triángulos y porcentaje con hipotenusa menor a 100: " << cantidadTriangulos - cantidadTriangulosHipotenusaMayor100 << "(" << 100 - porcentajeHipotenusasMayor100 << "%)" << endl;
  cout << "Promedio medidas hipotenusas: " << promedioHipotenusas << endl;
}

/**
 * Dados dos valores a y b, enteros positivos (incluido el cero), hallar la potencia aᵇ.
 * - Si a = 0 entonces b > 0.
 * - Si b = 0 entonces a > 0.
 */
void ejercicio5() {
  cout << "Dados dos valores a y b, enteros positivos (incluido el cero), hallar la potencia aᵇ." << endl;
  cout << "- Si a = 0 entonces b > 0." << endl;
  cout << "- Si b = 0 entonces a > 0." << endl;
  cout << "=====" << endl;

  int a, b;
  cout << "Ingresar a: " << endl;
  cin >> a;
  cout << "Ingresar b: " << endl;
  cin >> b;

  if (a < 0 || b < 0) {
    cout << "[ERROR] a y b deben ser mayores o iguales a cero." << endl;
  } else if (a == 0 && b == 0) {
    cout << "[ERROR] a y b no pueden ser cero simultáneamente." << endl;
  } else {
    float potencia = pow(a, b);
    cout << a << " elevado a la " << b << " es: " << potencia << endl;
  }
}

/**
 * EJERCICIO 13
 * Dada una lista de valores enteros y un valor x también entero, determinar si x está en la lista:
 * - Si está: indicar en qué posición fue encontrado.
 * - Si no está: informar con un mensaje apropiado.
 */
void ejercicio13(vector<int> lista, int x) {
  cout << "Dada una lista de valores enteros y un valor x también entero, determinar si x está en la lista:" << endl;
  cout << "- Si está: indicar en qué posición fue encontrado." << endl;
  cout << "- Si no está: informar con un mensaje apropiado." << endl;
  cout << "=====" << endl;

  bool estaEnLaLista = false;
  int posicion = 0;

  for(int i = 0; i <= lista.size() && !estaEnLaLista; i++) {
    if (lista[i] == x) {
      estaEnLaLista = true;
      posicion = i + 1;
    }
  }

  if (estaEnLaLista) {
    cout << "El elemento " << x << " se encuentra en la lista " << ", en la posición " << posicion << "." << endl;
  } else {
    cout << "El elemento" << x << " no se encuentra en la lista." << endl;
  }
}


int main() {
  // ejercicio1(4, 2);

  // ejercicio2(3, 4);
  // ejercicio2(4, 3);
  // ejercicio2(4, 4);

  // ejercicio3(20, 16);

  // ejercicio4();

  // ejercicio5();

  ejercicio13({1, 2, 3 ,4}, 2);

  return 0;
}