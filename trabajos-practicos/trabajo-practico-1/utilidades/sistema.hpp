// ---------------------------------------------------------------------------
// sistema.hpp
// Cabeceras de la biblioteca estandar y primitivas del sistema operativo.
//
// El programa se compila con GNU-GCC TDM-GCC-64 sobre Windows, de modo que
// todo lo que sigue usa directamente <windows.h>, <conio.h> y <io.h>, tal como
// los emplea el ModulosHB.cpp entregado por la catedra.
//
// Este modulo concentra lo unico que depende del sistema operativo -demora,
// preparacion de la consola, lectura de teclas y duplicacion de la salida
// estandar-, de manera que el resto del programa trabaje solamente con
// funciones propias.
// ---------------------------------------------------------------------------
#ifndef SISTEMA_HPP
#define SISTEMA_HPP

// ----- Biblioteca estandar -----
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include <climits>
#include <limits>
#include <string>

// ----- Cabeceras de Windows -----
#include <windows.h>
#include <conio.h>
#include <io.h>

using namespace std;

// ---------------------------------------------------------------------------
// Bandera global de FIN DE LA ENTRADA.
//
// Se enciende cuando la entrada estandar se agota (EOF). A partir de ese
// momento ninguna funcion vuelve a insistir con el pedido de datos y todos los
// menues se cierran en forma ordenada, en lugar de quedar girando en un ciclo
// infinito. Vive aca, en el nivel mas bajo, porque la consultan tanto el
// modulo de consola (Screen) como el de validaciones (Validar).
// ---------------------------------------------------------------------------
bool entradaAgotada = false;

// Copia del descriptor de la salida estandar, usada para volver a la pantalla
// despues de haberla redirigido al archivo de listados (ver mas abajo).
int descriptorSalidaOriginal = -1;

// Dispositivo de consola usado por freopen() para restaurar la salida estandar
// luego de haberla redirigido al archivo de listados.
const char DISPOSITIVO_CONSOLA[] = "CON";

// ---------------------------------------------------------------------------
// Pausa la ejecucion la cantidad de milisegundos indicada.
// ---------------------------------------------------------------------------
void Demorar(long milisegundos) {
  Sleep(milisegundos);
} // Demorar

// ---------------------------------------------------------------------------
// Prepara la consola: pagina de codigos UTF-8 (para los caracteres graficos de
// los marcos), bloqueo del historial de teclas y habilitacion del procesamiento
// de secuencias ANSI, disponible en Windows 10 o superior.
// ---------------------------------------------------------------------------
void PrepararConsola() {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  CONSOLE_HISTORY_INFO history = {sizeof(history), 0, 0, 0};
  SetConsoleHistoryInfo(&history);

  HANDLE hSalida = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD  modo = 0;
  if (GetConsoleMode(hSalida, &modo))
    SetConsoleMode(hSalida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
} // PrepararConsola

// ---------------------------------------------------------------------------
// Restaura la consola al estado inicial.
// ---------------------------------------------------------------------------
void RestaurarConsola() {
} // RestaurarConsola

// ---------------------------------------------------------------------------
// Indican si la entrada y la salida estandar son una consola real (y no un
// archivo o una tuberia). Cuando NO lo son se evitan los colores y el
// posicionamiento absoluto, para que la salida capturada resulte legible.
// ---------------------------------------------------------------------------
bool EntradaEsConsola() {
  return _isatty(_fileno(stdin)) != 0;
} // EntradaEsConsola

bool SalidaEsConsola() {
  return _isatty(_fileno(stdout)) != 0;
} // SalidaEsConsola

// ---------------------------------------------------------------------------
// Lee un caracter sin mostrarlo en pantalla (usado para las claves).
// ---------------------------------------------------------------------------
int LeerCaracterOculto() {
  return _getch();
} // LeerCaracterOculto

// ---------------------------------------------------------------------------
// Lee una tecla completa y deja sus codigos en <<codigos>>; devuelve cuantos
// codigos la componen (0 si se agoto la entrada). Las teclas extendidas -entre
// ellas las flechas- llegan como DOS codigos: un prefijo 0 o 224 y enseguida
// el codigo propiamente dicho.
//
// Cuando la entrada no es un teclado (por ejemplo si se toma de un archivo
// para probar el programa) _getch() no sirve, y se lee del flujo normal.
// ---------------------------------------------------------------------------
short LeerSecuenciaTecla(int codigos[]) {
  int car;

  if (not EntradaEsConsola()) {
    car = getchar();
    if (car == EOF)
      return 0;
    codigos[0] = car;
    return 1;
  }

  car = _getch();
  if (car == EOF)
    return 0;
  codigos[0] = car;
  if (car == 0 or car == 224) {          // prefijo de tecla extendida
    codigos[1] = _getch();
    return 2;
  }
  return 1;
} // LeerSecuenciaTecla

// ---------------------------------------------------------------------------
// Redireccion de la salida estandar al archivo de listados
//
// El enunciado pide redirigir la salida con freopen() para volcar los listados
// a ListadosHB.Txt. Para VOLVER a la pantalla, en cambio, no alcanza con
// freopen() sobre el dispositivo de consola: si el programa se ejecuta con la
// salida ya redirigida (por ejemplo <<programa > corrida.txt>>), ese
// dispositivo no es el destino original y la salida posterior se pierde.
//
// Por eso, antes de redirigir se guarda una COPIA del descriptor de la salida
// estandar, y al terminar se lo restituye. El freopen() al dispositivo de
// consola queda como alternativa por si la copia no estuviera disponible.
// ---------------------------------------------------------------------------
void GuardarSalidaEstandar() {
  fflush(stdout);
  descriptorSalidaOriginal = _dup(_fileno(stdout));
} // GuardarSalidaEstandar

bool RestaurarSalidaEstandar() {
  fflush(stdout);
  if (descriptorSalidaOriginal != -1) {
    if (_dup2(descriptorSalidaOriginal, _fileno(stdout)) != -1) {
      _close(descriptorSalidaOriginal);
      descriptorSalidaOriginal = -1;
      return true;
    }
  }
  return freopen(DISPOSITIVO_CONSOLA, "w", stdout) != NULL;
} // RestaurarSalidaEstandar

#endif // SISTEMA_HPP
