// ---------------------------------------------------------------------------
// compatibilidad.hpp
// Capa de compatibilidad entre plataformas.
//
// El TP se entrega compilado con TDM-GCC-64 (Windows), pero el desarrollo se
// realiza sobre macOS/clang++. Este modulo concentra TODO lo que depende del
// sistema operativo, de modo que el resto del programa sea identico en ambas
// plataformas.
//
//   * Windows  -> se usa <windows.h> tal como lo entrego la catedra.
//   * macOS/Linux -> se usan los equivalentes POSIX (termios, unistd) y
//                    secuencias de escape ANSI.
// ---------------------------------------------------------------------------
#ifndef COMPATIBILIDAD_HPP
#define COMPATIBILIDAD_HPP

// ----- Biblioteca estandar (comun a las dos plataformas) -----
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

// ---------------------------------------------------------------------------
// Deteccion de plataforma
// ---------------------------------------------------------------------------
#ifdef _WIN32

  #include <windows.h>
  #include <conio.h>
  #include <io.h>

  // Dispositivo de consola usado por freopen() para restaurar la salida
  // estandar luego de haberla redirigido al archivo de listados.
  const char DISPOSITIVO_CONSOLA[] = "CON";

  // Pausa la ejecucion la cantidad de milisegundos indicada.
  void Demorar(long milisegundos) {
    Sleep(milisegundos);
  } // Demorar

  // Prepara la consola: pagina de codigos UTF-8 (para los caracteres graficos
  // de los marcos), titulo de la ventana y bloqueo del historial de teclas.
  void PrepararConsola() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    CONSOLE_HISTORY_INFO history = {sizeof(history), 0, 0, 0};
    SetConsoleHistoryInfo(&history);
    // Habilita el procesamiento de secuencias ANSI en Windows 10 o superior.
    HANDLE hSalida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(hSalida, &modo))
      SetConsoleMode(hSalida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  } // PrepararConsola

  // Restaura la consola al estado inicial.
  void RestaurarConsola() {
  } // RestaurarConsola

  // Indica si la entrada estandar es una consola real (y no un archivo o una
  // tuberia). Cuando NO lo es, se evitan las secuencias de escape para que la
  // salida capturada resulte legible.
  bool EntradaEsConsola() {
    return _isatty(_fileno(stdin)) != 0;
  } // EntradaEsConsola

  bool SalidaEsConsola() {
    return _isatty(_fileno(stdout)) != 0;
  } // SalidaEsConsola

  // Lee un caracter sin mostrarlo en pantalla (usado para las claves).
  int LeerCaracterOculto() {
    return _getch();
  } // LeerCaracterOculto

  // Lee una tecla completa y deja sus codigos en <<codigos>>; devuelve cuantos
  // codigos la componen (0 si se agoto la entrada). Las teclas extendidas
  // -entre ellas las flechas- llegan como dos codigos: un prefijo 0 o 224 y
  // enseguida el codigo propiamente dicho.
  short LeerSecuenciaTecla(int codigos[]) {
    int car = _getch();

    if (car == EOF)
      return 0;
    codigos[0] = car;
    if (car == 0 or car == 224) {
      codigos[1] = _getch();
      return 2;
    }
    return 1;
  } // LeerSecuenciaTecla

#else // ----------------------- macOS / Linux -----------------------

  #include <termios.h>
  #include <unistd.h>

  const char DISPOSITIVO_CONSOLA[] = "/dev/tty";

  void Demorar(long milisegundos) {
    usleep(milisegundos * 1000);
  } // Demorar

  void PrepararConsola() {
    // En POSIX la consola ya trabaja en UTF-8 y acepta secuencias ANSI.
  } // PrepararConsola

  void RestaurarConsola() {
  } // RestaurarConsola

  bool EntradaEsConsola() {
    return isatty(STDIN_FILENO) != 0;
  } // EntradaEsConsola

  bool SalidaEsConsola() {
    return isatty(STDOUT_FILENO) != 0;
  } // SalidaEsConsola

  int LeerCaracterOculto() {
    if (not EntradaEsConsola())
      return getchar();

    termios configAnterior, configNueva;
    int caracter;

    tcgetattr(STDIN_FILENO, &configAnterior);
    configNueva = configAnterior;
    configNueva.c_lflag &= ~(ICANON | ECHO); // sin linea y sin eco
    tcsetattr(STDIN_FILENO, TCSANOW, &configNueva);
    caracter = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &configAnterior);
    return caracter;
  } // LeerCaracterOculto

  // -------------------------------------------------------------------------
  // Lee una tecla completa y deja sus codigos en <<codigos>>; devuelve cuantos
  // codigos la componen (0 si se agoto la entrada).
  //
  // Las flechas del teclado no llegan como un unico caracter sino como la
  // secuencia ESC [ A / ESC [ B. Para poder distinguirlas de la tecla ESCAPE
  // sola hay que leer los tres codigos DENTRO DE UNA MISMA sesion en modo
  // crudo: se lee el primero esperando indefinidamente (VMIN=1) y, si resulta
  // ser ESC, los siguientes con una espera acotada de una decima de segundo
  // (VMIN=0, VTIME=1). Si en ese lapso no llega nada, la tecla era ESCAPE.
  // -------------------------------------------------------------------------
  short LeerSecuenciaTecla(int codigos[]) {
    const short MAX_CODIGOS = 3;

    if (not EntradaEsConsola()) {          // la entrada no es un teclado
      int car = getchar();
      if (car == EOF)
        return 0;
      codigos[0] = car;
      return 1;
    }

    termios       configAnterior, configNueva;
    unsigned char byte;
    short         cant = 0;

    tcgetattr(STDIN_FILENO, &configAnterior);
    configNueva = configAnterior;
    configNueva.c_lflag &= ~(ICANON | ECHO);
    configNueva.c_cc[VMIN]  = 1;           // espera a que llegue una tecla
    configNueva.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &configNueva);

    if (read(STDIN_FILENO, &byte, 1) == 1) {
      codigos[cant++] = byte;
      if (byte == 27) {                    // puede iniciar una secuencia
        configNueva.c_cc[VMIN]  = 0;       // no espera indefinidamente
        configNueva.c_cc[VTIME] = 1;       // a lo sumo 100 milisegundos
        tcsetattr(STDIN_FILENO, TCSANOW, &configNueva);
        while (cant < MAX_CODIGOS and read(STDIN_FILENO, &byte, 1) == 1)
          codigos[cant++] = byte;
      }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &configAnterior);
    return cant;
  } // LeerSecuenciaTecla

#endif

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
#ifdef _WIN32
  descriptorSalidaOriginal = _dup(_fileno(stdout));
#else
  descriptorSalidaOriginal = dup(fileno(stdout));
#endif
} // GuardarSalidaEstandar

bool RestaurarSalidaEstandar() {
  fflush(stdout);
  if (descriptorSalidaOriginal != -1) {
#ifdef _WIN32
    if (_dup2(descriptorSalidaOriginal, _fileno(stdout)) != -1) {
      _close(descriptorSalidaOriginal);
#else
    if (dup2(descriptorSalidaOriginal, fileno(stdout)) != -1) {
      close(descriptorSalidaOriginal);
#endif
      descriptorSalidaOriginal = -1;
      return true;
    }
  }
  return freopen(DISPOSITIVO_CONSOLA, "w", stdout) != NULL;
} // RestaurarSalidaEstandar

#endif // COMPATIBILIDAD_HPP
