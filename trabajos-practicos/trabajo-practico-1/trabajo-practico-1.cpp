//==============================================================================
// Nombre del programa : TP1V1_K____G__-<APELLIDO NOMBRE>.CPP
//                       (renombrar este archivo con el curso, grupo y apellido
//                        del lider antes de la entrega, p.e.:
//                        TP1V1_K1023G3_PEREZ JUAN.CPP)
// Fecha de entrega    : __ / __ / 2026
// Nro. de version     : V1
//
// Objetivo            : Simulacion de un sitio de Home Banking por consola.
//                       Permite el login de cinco usuarios registrados y la
//                       consulta y operacion sobre sus cuentas: caja de ahorro
//                       en pesos y en dolares, tarjetas de credito y debito,
//                       transferencias, depositos, compras, recargas, compra y
//                       venta de dolares, plazo fijo, fondo de inversion,
//                       agenda de CBU, generacion de token y emision de
//                       listados a archivo de texto.
//
//                       Estructuras de datos utilizadas: registros, arreglos de
//                       registros, registros con campos arreglo y archivos de
//                       texto de ancho fijo con acceso secuencial y aleatorio.
//                       Paradigma imperativo procedural, programacion
//                       estructurada y modular. Sin memoria dinamica.
//
// Curso               : K____        Dia: ___________  Turno: ___________
// Nro. de grupo       : ____
// Integrantes         : Legajo    Apellido y Nombre
//                       --------  -------------------------------------------
//                       ________  ___________________________________________
//                       ________  ___________________________________________
//                       ________  ___________________________________________
//
// Compilador          : GNU-GCC TDM-GCC-64 x86_64-w64-mingw32-g++.exe
// Estandar            : C++17
// IDE                 : Code::Blocks
//
// Archivos de datos   : MovimientosCA.Txt  movimientos de la caja de ahorro
//                       MovimientosTD.Txt  consumos de la tarjeta de debito
//                       MovimientosTC.Txt  consumos de la tarjeta de credito
// Archivo de salida   : ListadosHB.Txt     listados emitidos con freopen
//                       Los cuatro archivos deben estar en la misma carpeta
//                       desde la que se ejecuta el programa.
//==============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

//==============================================================================
//  CONSTANTES CON NOMBRE
//==============================================================================

//--- Dimension de las estructuras de datos ------------------------------------
const short CANT_USUARIOS   =   5;  // usuarios registrados en el sistema
const short MAX_CUENTAS     =   6;  // cuentas bancarias por usuario
const short MAX_AGENDA      =  15;  // cuentas CBU agendadas por usuario
const short MAX_PLAZOS      =  10;  // plazos fijos vigentes por usuario
const short MAX_MOV_CA      = 150;  // movimientos de caja de ahorro en memoria
const short MAX_MOV_TD      = 100;  // consumos de tarjeta de debito en memoria
const short MAX_MOV_TC      = 100;  // consumos de tarjeta de credito en memoria

//--- Posicion de la cuenta principal dentro del arreglo de cuentas ------------
const short CTA_PESOS       =   0;  // caja de ahorro en pesos  (la del archivo)
const short CTA_DOLARES     =   1;  // caja de ahorro en dolares

//--- Marcas de control --------------------------------------------------------
const short SIN_USUARIO     =  -1;  // el login no identifico a ningun usuario
const short NO_ENCONTRADO   =  -1;  // resultado de una busqueda sin exito
const short TODOS_USUARIOS  =   0;  // filtro: traer los movimientos de todos

//--- Anchos de las columnas de los archivos de texto --------------------------
const short ANCHO_FECHA     =  10;  // 99-99-9999
const short ANCHO_DETALLE   =  30;  // X(30) ajustado a izquierda
const short ANCHO_IMPORTE   =  13;  // 9(7).99 ajustado a derecha
const short ANCHO_CUOTAS    =   6;  // x/y    ajustado a derecha
const short ANCHO_CPBTE     =   9;  // 9(6)   ajustado a derecha
const short ANCHO_USUARIO   =   2;  // 9(2)   ajustado a derecha

//--- Largo logico de la linea (sin el fin de linea) ---------------------------
const short LARGO_LIN_CA    =  58;  // 10 + 1 + 1 + 1 + 30 + 13 + 2
const short LARGO_LIN_TD    =  56;  // 10 + 1 + 30 + 13 + 2
const short LARGO_LIN_TC    =  71;  // 10 + 1 + 30 + 6 + 9 + 13 + 2

//--- Largo fisico del registro en disco (incluye el CR LF de Windows) ---------
//    Se usa para calcular el desplazamiento en los accesos aleatorios.
const short LARGO_REG_CA    = LARGO_LIN_CA + 2;   // 60 bytes
const short LARGO_REG_TD    = LARGO_LIN_TD + 2;   // 58 bytes
const short LARGO_REG_TC    = LARGO_LIN_TC + 2;   // 73 bytes

//--- Columna en la que comienza cada campo (base 1) ---------------------------
const short COL_FECHA       =   1;
const short COL_TIPO_CA     =  12;
const short COL_DETALLE_CA  =  14;
const short COL_IMPORTE_CA  =  44;
const short COL_USUARIO_CA  =  57;

const short COL_DETALLE_TD  =  12;
const short COL_IMPORTE_TD  =  42;
const short COL_USUARIO_TD  =  55;

const short COL_DETALLE_TC  =  12;
const short COL_CUOTAS_TC   =  42;
const short COL_CPBTE_TC    =  48;
const short COL_IMPORTE_TC  =  57;
const short COL_USUARIO_TC  =  70;

//--- Nombre de los archivos ---------------------------------------------------
const char ARCH_MOV_CA[]    = "MovimientosCA.Txt";
const char ARCH_MOV_TD[]    = "MovimientosTD.Txt";
const char ARCH_MOV_TC[]    = "MovimientosTC.Txt";
const char ARCH_LISTADOS[]  = "ListadosHB.Txt";

//--- Tipo de movimiento de la caja de ahorro ----------------------------------
const char DEBE             = 'D';  // acredita  (aumenta el saldo)
const char HABER            = 'H';  // debita    (disminuye el saldo)

//--- Reglas del negocio -------------------------------------------------------
const short MAX_INTENTOS    =   3;  // intentos de login antes de bloquear
const long  TOKEN_MINIMO    = 100000;
const long  TOKEN_MAXIMO    = 999999;
const short SEG_VIGENCIA_TOKEN =  5;  // segundos de validez que se muestran
const short LARGO_CBU       =  22;  // digitos del CBU
const short LARGO_MIN_CLAVE =   6;  // caracteres minimos de la clave
const short LARGO_MAX_CLAVE =  15;
const short LARGO_CELULAR   =  10;  // digitos del numero de celular
const short SUCURSAL        =   9;  // nro. de sucursal para las cuentas nuevas

const double TNA_PLAZO_FIJO  = 0.2900;  // tasa nominal anual del plazo fijo
const double TNA_FONDO_INV   = 0.3800;  // tasa nominal anual del fondo comun
const double DOLAR_COMPRA    = 1180.00; // el banco vende a este valor
const double DOLAR_VENTA     = 1225.00; // el banco compra a este valor
const short DIAS_ANIO       = 365;
const short PLAZO_MINIMO    =  30;  // dias minimos de un plazo fijo
const short PLAZO_MAXIMO    = 365;
const double IMPORTE_MAXIMO  = 9999999.99; // 9(7).99, el maximo que entra en la columna
const double IMPORTE_MINIMO  = 0.01;
const double RECARGA_MINIMA  = 500.00;

//--- Colores de la consola ----------------------------------------------------
const WORD NEGRO            =  0;
const WORD AZUL             =  1;
const WORD VERDE            =  2;
const WORD CIAN             =  3;
const WORD ROJO             =  4;
const WORD MAGENTA          =  5;
const WORD MARRON           =  6;
const WORD GRIS_CLARO       =  7;
const WORD GRIS_OSCURO      =  8;
const WORD AZUL_CLARO       =  9;
const WORD VERDE_CLARO      = 10;
const WORD CIAN_CLARO       = 11;
const WORD ROJO_CLARO       = 12;
const WORD MAGENTA_CLARO    = 13;
const WORD AMARILLO         = 14;
const WORD BLANCO           = 15;

//--- Distribucion de la pantalla ----------------------------------------------
const short ANCHO_PANTALLA  = 80;
const short ALTO_PANTALLA   = 30;
const short FILA_TITULO     =  2;
const short FILA_CUERPO     =  6;   // primera fila util del cuerpo
const short FILA_MENSAJE    = 27;   // fila reservada para mensajes al usuario
const short FILA_PAUSA      = 29;   // fila reservada para "oprima una tecla"
const short COL_IZQUIERDA   =  4;
const short FILAS_PAGINA    = 15;   // renglones de detalle por pantalla
const short ANCHO_UTIL      = 75;   // columnas utiles entre los bordes del marco

//--- Ancho de las tablas que se muestran por pantalla -------------------------
//    Deben entrar dentro de ANCHO_UTIL. En los listados a archivo no rige
//    este limite y las columnas se emiten con el ancho completo.
const short ANCHO_DET_PANT  = 22;   // detalle recortado en la tabla de CA
const short ANCHO_TABLA_CA  = 74;   // 3 +1 +10 +1 +1 +1 +22 +11 +12 +12
const short ANCHO_TABLA_TD  = 54;   // 10 +1 +30 +13
const short ANCHO_TABLA_TC  = 70;   // 10 +1 +30 +6 +10 +13

//--- Ancho de las tablas que se emiten a ListadosHB.Txt -----------------------
const short LARGO_LISTADO_CA = 82;  // 10 +3 +30 +13 +13 +13
const short LARGO_LISTADO_TD = 54;  // 10 +1 +30 +13
const short LARGO_LISTADO_TC = 70;  // 10 +1 +30 +6 +10 +13

//--- Opciones del menu principal ----------------------------------------------
const short OPC_MIS_CUENTAS   =  1;
const short OPC_DATOS_PERS    =  2;
const short OPC_MOSTRAR_CBU   =  3;
const short OPC_MOVIMIENTOS   =  4;
const short OPC_TARJETAS      =  5;
const short OPC_TRANSFERIR    =  6;
const short OPC_DEPOSITO      =  7;
const short OPC_COMPRAS       =  8;
const short OPC_RECARGAR      =  9;
const short OPC_DOLARES       = 10;
const short OPC_PLAZO_FIJO    = 11;
const short OPC_SIMULACION_PF = 12;
const short OPC_AGENDA_CBU    = 13;
const short OPC_NUEVA_CUENTA  = 14;
const short OPC_GEN_TOKEN     = 15;
const short OPC_MODIF_CLAVE   = 16;
const short OPC_LISTADOS      = 17;
const short OPC_LOGOUT        =  0;

//==============================================================================
//  TIPOS DE DATOS
//
//  Los tamanios en bytes indicados en cada registro son los que produce el
//  compilador de la catedra, TDM-GCC-64 x86_64-w64-mingw32-g++ (short = 2,
//  long = 4, double = 8, con relleno de alineacion a 8 bytes).
//
//  Los campos de dinero se declaran double y no float: el formato de los
//  archivos es 9(7).99, es decir hasta nueve digitos significativos, y un
//  float solo garantiza siete. Con float un importe como 1542367.21 se
//  almacena y se vuelve a escribir como 1542367.25.
//==============================================================================

//--- Cadenas de longitud fija (el ultimo byte queda para el terminador '\0') ---
typedef char str5[6];
typedef char str10[11];
typedef char str15[16];
typedef char str20[21];
typedef char str25[26];
typedef char str30[31];
typedef char str40[41];
typedef char str80[81];

//------------------------------------------------------------------------------
// sFecha                                                            6 bytes
//   dia  [2] mes [2] anio [2]
//------------------------------------------------------------------------------
struct sFecha {
  short dia;
  short mes;
  short anio;
};

//------------------------------------------------------------------------------
// sMovCA - registro del archivo "MovimientosCA.Txt"                72 bytes
//   fecha [6] tipoMov [1] detalle [31] +1 relleno importe [8] nroUsuario [2]
//   +6 relleno saldo [8] nroReg [2] +6 relleno
//   Los campos saldo y nroReg NO se graban. saldo se calcula al leer,
//   acumulando sobre el saldo inicial de la cuenta; nroReg guarda la posicion
//   que ocupa el registro dentro del archivo, para poder volver a el por
//   acceso aleatorio.
//------------------------------------------------------------------------------
struct sMovCA {
  sFecha fecha;
  char   tipoMov;      // DEBE = acredita, HABER = debita
  str30  detalle;
  double  importe;
  short  nroUsuario;
  double  saldo;        // calculado, no persistido
  short  nroReg;       // posicion en el archivo, no persistido
};

//------------------------------------------------------------------------------
// sMovTD - registro del archivo "MovimientosTD.Txt"                 56 bytes
//   fecha [6] detalle [31] +3 relleno importe [8] nroUsuario [2] +6 relleno
//------------------------------------------------------------------------------
struct sMovTD {
  sFecha fecha;
  str30  detalle;
  double  importe;
  short  nroUsuario;
};

//------------------------------------------------------------------------------
// sMovTC - registro del archivo "MovimientosTC.Txt"                 64 bytes
//   fecha [6] detalle [31] cuotas [6] +1 relleno nroCpbte [4] importe [8]
//   nroUsuario [2] +6 relleno
//   cuotas se graba con el formato x/y y puede venir vacio.
//   nroCpbte en 0 indica que la linea no trae numero de comprobante.
//------------------------------------------------------------------------------
struct sMovTC {
  sFecha fecha;
  str30  detalle;
  str5   cuotas;
  long   nroCpbte;
  double  importe;
  short  nroUsuario;
};

//------------------------------------------------------------------------------
// sCuenta - una cuenta bancaria del usuario                         64 bytes
//   nroCta [21] tipo [6] moneda [6] estado [11] +4 relleno
//   saldoInicial [8] saldo [8]
//------------------------------------------------------------------------------
struct sCuenta {
  str20 nroCta;        // 106-4235689707-3  o  009-0000001
  str5  tipo;          // "CA" caja de ahorro / "CC" cuenta corriente
  str5  moneda;        // "$" pesos / "u$s" dolares
  str10 estado;        // "Activa" / "Cerrada"
  double saldoInicial;  // saldo antes de aplicar los movimientos del archivo
  double saldo;         // saldo actual
};

//------------------------------------------------------------------------------
// sAgenda - una cuenta de tercero agendada para transferir          78 bytes
//   alias [21] titular [31] cbu [26]
//   El arreglo de agenda se mantiene SIEMPRE ordenado por alias, para poder
//   resolver la busqueda con el metodo binario.
//------------------------------------------------------------------------------
struct sAgenda {
  str20 alias;
  str30 titular;
  str25 cbu;
};

//------------------------------------------------------------------------------
// sPlazoFijo - una imposicion a plazo fijo del usuario              56 bytes
//   fAlta [6] fVto [6] +4 relleno capital [8] plazoDias [2] +6 relleno
//   tasa [8] interes [8] total [8]
//------------------------------------------------------------------------------
struct sPlazoFijo {
  sFecha fAlta;
  sFecha fVto;
  double  capital;
  short  plazoDias;
  double  tasa;
  double  interes;
  double  total;
};

//------------------------------------------------------------------------------
// sTarjeta - datos de una tarjeta del usuario                       48 bytes
//   tipo [6] nroTarjeta [26] vence [6] +2 relleno limite [8]
//------------------------------------------------------------------------------
struct sTarjeta {
  str5   tipo;         // "TC" / "TD"
  str25  nroTarjeta;   // 4517 78** **** 3041
  sFecha vence;
  double  limite;
};

//------------------------------------------------------------------------------
// sDatosPers - registro con los datos de UN usuario del sistema    2440 bytes
//
//   Es la estructura combinada que pide el enunciado: un registro cuyos campos
//   son a su vez registros (fechaNac, tarjetaTC, tarjetaTD) y arreglos de
//   registros (aCuentas, aAgenda, aPlazos).
//
//   +----------------------------------------------------------------------+
//   | dni | apeNom | fechaNac | usuario | clave | celular | email |         |
//   | domicilio | nroCtaCA | cbu                                            |
//   +----------------------------------------------------------------------+
//   | aCuentas[0..MAX_CUENTAS-1] : sCuenta      | cantCuentas               |
//   | aAgenda [0..MAX_AGENDA -1] : sAgenda      | cantAgenda   (ordenado)   |
//   | aPlazos [0..MAX_PLAZOS -1] : sPlazoFijo   | cantPlazos                |
//   | tarjetaTC : sTarjeta        tarjetaTD : sTarjeta                      |
//   +----------------------------------------------------------------------+
//------------------------------------------------------------------------------
struct sDatosPers {
  long       dni;
  str30      apeNom;
  sFecha     fechaNac;
  str15      usuario;
  str15      clave;
  str15      celular;
  str30      email;
  str40      domicilio;
  str20      nroCtaCA;
  str25      cbu;
  sCuenta    aCuentas[MAX_CUENTAS];
  short      cantCuentas;
  sAgenda    aAgenda[MAX_AGENDA];
  short      cantAgenda;
  sPlazoFijo aPlazos[MAX_PLAZOS];
  short      cantPlazos;
  sTarjeta   tarjetaTC;
  sTarjeta   tarjetaTD;
};

//------------------------------------------------------------------------------
// Arreglo de registros con los cinco usuarios registrados
//
//   aDatosPers                          5 x 2440 =  12200 bytes
//   +---------+---------+---------+---------+---------+
//   |   [0]   |   [1]   |   [2]   |   [3]   |   [4]   |
//   +---------+---------+---------+---------+---------+
//        1         2         3         4         5      <- nro. de usuario
//
//   Se carga por codigo en CargarDatosPersonales(). El nro. de usuario que
//   figura en la ultima columna de los archivos de movimientos es el indice
//   del arreglo mas uno.
//------------------------------------------------------------------------------

//==============================================================================
//  NAMESPACE Screen - manejo de la consola (pantalla y teclado)
//
//  Modulos entregados por la catedra en ModulosHB.cpp, adaptados a este TP.
//  Los nombres que empiezan con guion bajo son los clones de las funciones
//  homonimas de Borland, que el compilador TDM-GCC-64 no provee.
//==============================================================================
namespace Screen {

  //--- Aplica simultaneamente color de texto y de fondo ------------------------
  void setConsoleColor(WORD colText, WORD colBack) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            colText + 16 * colBack);
  } // setConsoleColor

  //--- Devuelve el color de texto vigente --------------------------------------
  WORD ObtenerColorTexto() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return csbi.wAttributes & 0x000F;
    return GRIS_CLARO;
  } // ObtenerColorTexto

  //--- Devuelve el color de fondo vigente --------------------------------------
  WORD ObtenerColorFondo() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return (csbi.wAttributes >> 4) & 0x000F;
    return NEGRO;
  } // ObtenerColorFondo

  //--- Clon de textcolor() -----------------------------------------------------
  void _textcolor(WORD color) {
    setConsoleColor(color, ObtenerColorFondo());
  } // _textcolor

  //--- Clon de textbackground() ------------------------------------------------
  void _textbackground(WORD color) {
    setConsoleColor(ObtenerColorTexto(), color);
  } // _textbackground

  //--- Clon de gotoxy(): coordenadas base 1, (1,1) es el angulo superior izq. --
  void _gotoxy(short x, short y) {
    COORD coord;

    coord.X = x - 1;
    coord.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
  } // _gotoxy

  //--- Columna en la que se encuentra el cursor --------------------------------
  short _wherex() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return csbi.dwCursorPosition.X + 1;
    return 1;
  } // _wherex

  //--- Fila en la que se encuentra el cursor -----------------------------------
  short _wherey() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return csbi.dwCursorPosition.Y + 1;
    return 1;
  } // _wherey

  //--- Clon de clrscr(): limpia toda la pantalla y deja el cursor en (1,1) -----
  void _clrscr() {
    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD inicio = {0, 0};
    DWORD celdas, escritos;

    GetConsoleScreenBufferInfo(hConsola, &csbi);
    celdas = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsola, ' ', celdas, inicio, &escritos);
    FillConsoleOutputAttribute(hConsola, csbi.wAttributes, celdas, inicio,
                               &escritos);
    SetConsoleCursorPosition(hConsola, inicio);
  } // _clrscr

  //--- Clon de clreol(): borra desde el cursor hasta el fin de la linea --------
  void _clreol() {
    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD posicion;
    DWORD largo, escritos;

    GetConsoleScreenBufferInfo(hConsola, &csbi);
    posicion = csbi.dwCursorPosition;
    largo = csbi.dwSize.X - posicion.X;
    FillConsoleOutputCharacter(hConsola, ' ', largo, posicion, &escritos);
    FillConsoleOutputAttribute(hConsola, csbi.wAttributes, largo, posicion,
                               &escritos);
    SetConsoleCursorPosition(hConsola, posicion);
  } // _clreol

  //--- Borra un rectangulo de la pantalla --------------------------------------
  void BorrarArea(short x1, short y1, short x2, short y2) {
    short y;

    for (y = y1; y <= y2; y++) {
      _gotoxy(x1, y);
      cout << string(x2 - x1 + 1, ' ');
    }
  } // BorrarArea

  //--- Establece el tamanio de la ventana y del buffer de la consola -----------
  void _window(short ancho, short alto) {
    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD tamBuffer;
    SMALL_RECT ventana = {0, 0, 1, 1};

    tamBuffer.X = ancho;
    tamBuffer.Y = alto;
    SetConsoleWindowInfo(hConsola, TRUE, &ventana);
    SetConsoleScreenBufferSize(hConsola, tamBuffer);
    ventana.Right  = ancho - 1;
    ventana.Bottom = alto - 1;
    SetConsoleWindowInfo(hConsola, TRUE, &ventana);
  } // _window

  //--- Mensaje en la posicion x, y ---------------------------------------------
  void MnsgBox(short x, short y, string mensaje) {
    _gotoxy(x, y);
    cout << mensaje;
  } // MnsgBox

  //--- Mensaje con color en la posicion x, y -----------------------------------
  void MnsgBox(short x, short y, WORD color, string mensaje) {
    _gotoxy(x, y);
    _textcolor(color);
    cout << mensaje;
  } // MnsgBox

  //--- Mensaje alineado a izquierda ('i') o a derecha ('d') dentro de un ancho -
  void MnsgBox(short x, short y, string mensaje, char alinea, short ancho) {
    _gotoxy(x, y);
    if (alinea == 'i')
      cout << left << setw(ancho) << mensaje;
    else
      cout << right << setw(ancho) << mensaje;
  } // MnsgBox

  //--- Devuelve una linea separadora -------------------------------------------
  string Separador(short ancho, char caracter) {
    return string(ancho, caracter);
  } // Separador

  //--- Marco de linea doble entre los vertices (x1,y1) y (x2,y2) ---------------
  void Marco(short x1, short y1, short x2, short y2, WORD color) {
    short i;

    _textcolor(color);
    MnsgBox(x1, y1, "\xC9");                      // angulo superior izquierdo
    MnsgBox(x2, y1, "\xBB");                      // angulo superior derecho
    MnsgBox(x1, y2, "\xC8");                      // angulo inferior izquierdo
    MnsgBox(x2, y2, "\xBC");                      // angulo inferior derecho
    for (i = x1 + 1; i < x2; i++) {
      MnsgBox(i, y1, "\xCD");
      MnsgBox(i, y2, "\xCD");
    }
    for (i = y1 + 1; i < y2; i++) {
      MnsgBox(x1, i, "\xBA");
      MnsgBox(x2, i, "\xBA");
    }
  } // Marco

  //--- Oculta el cursor ---------------------------------------------------------
  void OcultarCursor() {
    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    GetConsoleCursorInfo(hConsola, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsola, &info);
  } // OcultarCursor

  //--- Muestra el cursor --------------------------------------------------------
  void MostrarCursor() {
    HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    GetConsoleCursorInfo(hConsola, &info);
    info.bVisible = TRUE;
    SetConsoleCursorInfo(hConsola, &info);
  } // MostrarCursor

  //--- Titulo de la barra de la ventana -----------------------------------------
  void BarraTitulo() {
    SetConsoleTitleA("Home Banking Mesopotamia");
  } // BarraTitulo

  //--- Impide que el usuario redimensione o maximice la consola -----------------
  void BloquearCambioTamanio() {
    HWND hVentana = GetConsoleWindow();
    LONG estilo;

    if (hVentana != NULL) {
      estilo = GetWindowLong(hVentana, GWL_STYLE);
      estilo &= ~WS_MAXIMIZEBOX;
      estilo &= ~WS_SIZEBOX;
      SetWindowLong(hVentana, GWL_STYLE, estilo);
    }
  } // BloquearCambioTamanio

  //--- Inhabilita el historial de las teclas flecha arriba / flecha abajo -------
  void InhabilitarHistorial() {
    CONSOLE_HISTORY_INFO historial = {sizeof(historial), 0, 0, 0};

    SetConsoleHistoryInfo(&historial);
  } // InhabilitarHistorial

  //--- Espera a que se oprima cualquier tecla -----------------------------------
  void Pausa(string mensaje) {
    MnsgBox(COL_IZQUIERDA, FILA_PAUSA, GRIS_CLARO, mensaje);
    OcultarCursor();
    _getch();
    MostrarCursor();
    _gotoxy(1, FILA_PAUSA);
    _clreol();
  } // Pausa

  //--- Espera "tiempo" milisegundos informando la demora ------------------------
  void Espera(short tiempo) {
    MnsgBox(COL_IZQUIERDA, FILA_MENSAJE, GRIS_CLARO,
            "Aguarde " + to_string(tiempo / 1000) + " segundo(s)...");
    Sleep(tiempo);
    _gotoxy(1, FILA_MENSAJE);
    _clreol();
  } // Espera

} // namespace Screen

//==============================================================================
//  NAMESPACE FechaHora - fecha y hora del sistema
//
//  Modulos entregados por la catedra (apunte del prof. Hugo Cuello,
//  "Teoria y Practica del Lenguaje C/C++ - ANEXOS").
//==============================================================================
namespace FechaHora {

  //--- Hora larga hhmmss; devuelve ademas hora, minutos y segundos -------------
  long GetTime(int &hora, int &min, int &seg) {
    time_t    tiempoBruto;
    struct tm *infoTiempo;

    time(&tiempoBruto);
    infoTiempo = localtime(&tiempoBruto);
    hora = infoTiempo->tm_hour;
    min  = infoTiempo->tm_min;
    seg  = infoTiempo->tm_sec;
    return (long)hora * 10000 + (long)min * 100 + seg;
  } // GetTime

  //--- Fecha larga aaaammdd; devuelve ademas anio, mes, dia y dia de semana ----
  long GetDate(int &anio, int &mes, int &dia, int &diaSem) {
    time_t    tiempoBruto;
    struct tm *infoTiempo;

    time(&tiempoBruto);
    infoTiempo = localtime(&tiempoBruto);
    anio   = 1900 + infoTiempo->tm_year;
    mes    = 1 + infoTiempo->tm_mon;
    dia    = infoTiempo->tm_mday;
    diaSem = 1 + infoTiempo->tm_wday;
    return (long)anio * 10000 + (long)mes * 100 + dia;
  } // GetDate

} // namespace FechaHora

using namespace Screen;
using namespace FechaHora;

//==============================================================================
//  UTILIDADES DE CADENAS
//==============================================================================

//--- Copia origen en destino truncando a largoMax caracteres ------------------
void CopiarCadena(char destino[], const char origen[], short largoMax) {
  short i = 0;

  while (i < largoMax and origen[i] != '\0') {
    destino[i] = origen[i];
    i++;
  }
  destino[i] = '\0';
} // CopiarCadena

//--- Copia el contenido de un string de la STL a una cadena de largo fijo -----
void CopiarCadena(char destino[], string origen, short largoMax) {
  short i = 0;
  short largo = (short)origen.length();

  while (i < largoMax and i < largo) {
    destino[i] = origen[i];
    i++;
  }
  destino[i] = '\0';
} // CopiarCadena

//--- Largo de una cadena terminada en '\0' ------------------------------------
short LargoCadena(const char cadena[]) {
  short largo = 0;

  while (cadena[largo] != '\0')
    largo++;
  return largo;
} // LargoCadena

//--- Elimina los blancos del comienzo y del final de la cadena ----------------
void QuitarBlancos(char cadena[]) {
  short inicio = 0;
  short fin = LargoCadena(cadena) - 1;
  short i;

  while (cadena[inicio] == ' ' and cadena[inicio] != '\0')
    inicio++;
  while (fin >= inicio and cadena[fin] == ' ')
    fin--;
  for (i = 0; inicio + i <= fin; i++)
    cadena[i] = cadena[inicio + i];
  cadena[i] = '\0';
} // QuitarBlancos

//--- Indica si la cadena esta vacia -------------------------------------------
bool CadenaVacia(const char cadena[]) {
  return cadena[0] == '\0';
} // CadenaVacia

//--- Convierte la cadena a mayusculas ------------------------------------------
void AMayusculas(char cadena[]) {
  short i;

  for (i = 0; cadena[i] != '\0'; i++)
    if (cadena[i] >= 'a' and cadena[i] <= 'z')
      cadena[i] = cadena[i] - 'a' + 'A';
} // AMayusculas

//--- Devuelve la cadena recortada a lo sumo a "ancho" caracteres ---------------
//    setw() solo fija un ancho minimo: si el dato es mas largo desplaza a las
//    columnas siguientes. Por eso las columnas de pantalla se recortan antes.
string Recortar(const char cadena[], short ancho) {
  string texto = cadena;

  if ((short)texto.length() > ancho)
    return texto.substr(0, ancho);
  return texto;
} // Recortar

//--- Indica si la cadena contiene unicamente digitos ---------------------------
bool SoloDigitos(const char cadena[]) {
  short i;

  if (CadenaVacia(cadena))
    return false;
  for (i = 0; cadena[i] != '\0'; i++)
    if (cadena[i] < '0' or cadena[i] > '9')
      return false;
  return true;
} // SoloDigitos

//--- Extrae cant caracteres de linea a partir de la columna desde (base 1) ----
void ExtraerCampo(const char linea[], short desde, short cant, char destino[]) {
  short largo = LargoCadena(linea);
  short i;

  for (i = 0; i < cant; i++)
    if (desde - 1 + i < largo)
      destino[i] = linea[desde - 1 + i];
    else
      destino[i] = ' ';
  destino[cant] = '\0';
  QuitarBlancos(destino);
} // ExtraerCampo

//--- Convierte una cadena a numero entero; devuelve false si no es valida -----
bool CadenaAEntero(const char cadena[], long &valor) {
  short i = 0;
  short signo = 1;

  valor = 0;
  if (cadena[0] == '-' or cadena[0] == '+') {
    if (cadena[0] == '-')
      signo = -1;
    i = 1;
  }
  if (cadena[i] == '\0')
    return false;
  while (cadena[i] != '\0') {
    if (cadena[i] < '0' or cadena[i] > '9')
      return false;
    valor = valor * 10 + (cadena[i] - '0');
    i++;
  }
  valor = valor * signo;
  return true;
} // CadenaAEntero

//--- Convierte una cadena a numero real; acepta la coma como separador --------
bool CadenaAReal(const char cadena[], double &valor) {
  short i = 0;
  short signo = 1;
  short decimales = 0;
  bool  hayPunto = false;
  bool  hayDigito = false;
  double divisor = 1;

  valor = 0;
  if (cadena[0] == '-' or cadena[0] == '+') {
    if (cadena[0] == '-')
      signo = -1;
    i = 1;
  }
  while (cadena[i] != '\0') {
    if (cadena[i] == '.' or cadena[i] == ',') {
      if (hayPunto)
        return false;
      hayPunto = true;
    }
    else if (cadena[i] >= '0' and cadena[i] <= '9') {
      hayDigito = true;
      valor = valor * 10 + (cadena[i] - '0');
      if (hayPunto)
        decimales++;
    }
    else
      return false;
    i++;
  }
  if (not hayDigito)
    return false;
  for (i = 0; i < decimales; i++)
    divisor = divisor * 10;
  valor = signo * valor / divisor;
  return true;
} // CadenaAReal

//==============================================================================
//  UTILIDADES DE FECHA
//==============================================================================

//--- Indica si el anio es bisiesto --------------------------------------------
bool EsBisiesto(short anio) {
  return (anio % 4 == 0 and anio % 100 != 0) or anio % 400 == 0;
} // EsBisiesto

//--- Cantidad de dias que tiene el mes ----------------------------------------
short DiasDelMes(short mes, short anio) {
  const short aDias[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (mes < 1 or mes > 12)
    return 0;
  if (mes == 2 and EsBisiesto(anio))
    return 29;
  return aDias[mes];
} // DiasDelMes

//--- Valida una fecha del calendario ------------------------------------------
bool FechaValida(sFecha fecha) {
  return fecha.anio >= 1900 and fecha.anio <= 2100 and
         fecha.mes  >= 1    and fecha.mes  <= 12   and
         fecha.dia  >= 1    and fecha.dia  <= DiasDelMes(fecha.mes, fecha.anio);
} // FechaValida

//--- Fecha larga aaaammdd, para comparar dos fechas con un solo numero --------
long FechaLarga(sFecha fecha) {
  return (long)fecha.anio * 10000 + (long)fecha.mes * 100 + fecha.dia;
} // FechaLarga

//--- Fecha del sistema --------------------------------------------------------
sFecha FechaDeHoy() {
  sFecha fecha;
  int anio, mes, dia, diaSem;

  GetDate(anio, mes, dia, diaSem);
  fecha.anio = (short)anio;
  fecha.mes  = (short)mes;
  fecha.dia  = (short)dia;
  return fecha;
} // FechaDeHoy

//--- Nombre del dia de la semana (1 = domingo) --------------------------------
string NombreDiaSemana(short diaSem) {
  const string aNombres[8] = {"", "Domingo", "Lunes", "Martes", "Miercoles",
                              "Jueves", "Viernes", "Sabado"};

  if (diaSem < 1 or diaSem > 7)
    return "";
  return aNombres[diaSem];
} // NombreDiaSemana

//--- Suma dias a una fecha ----------------------------------------------------
sFecha SumarDias(sFecha fecha, short dias) {
  sFecha resultado = fecha;
  short  i;

  for (i = 0; i < dias; i++) {
    resultado.dia++;
    if (resultado.dia > DiasDelMes(resultado.mes, resultado.anio)) {
      resultado.dia = 1;
      resultado.mes++;
      if (resultado.mes > 12) {
        resultado.mes = 1;
        resultado.anio++;
      }
    }
  }
  return resultado;
} // SumarDias

//--- Arma la fecha con el formato de los archivos: "99-99-9999" ---------------
string FechaATexto(sFecha fecha) {
  string texto;

  texto = to_string(fecha.dia);
  if (fecha.dia < 10)
    texto = " " + texto;
  texto = texto + "-";
  if (fecha.mes < 10)
    texto = texto + " ";
  texto = texto + to_string(fecha.mes) + "-" + to_string(fecha.anio);
  return texto;
} // FechaATexto

//--- Arma la fecha con ceros a la izquierda: "09-03-2026" ---------------------
string FechaATextoCero(sFecha fecha) {
  string texto;

  if (fecha.dia < 10)
    texto = "0";
  texto = texto + to_string(fecha.dia) + "-";
  if (fecha.mes < 10)
    texto = texto + "0";
  texto = texto + to_string(fecha.mes) + "-" + to_string(fecha.anio);
  return texto;
} // FechaATextoCero

//--- Hora del sistema como "hh:mm:ss" -----------------------------------------
string HoraATexto() {
  int hora, min, seg;
  string texto;

  GetTime(hora, min, seg);
  if (hora < 10) texto = "0";
  texto = texto + to_string(hora) + ":";
  if (min  < 10) texto = texto + "0";
  texto = texto + to_string(min) + ":";
  if (seg  < 10) texto = texto + "0";
  texto = texto + to_string(seg);
  return texto;
} // HoraATexto

//==============================================================================
//  UTILIDADES DE PANTALLA
//==============================================================================

//--- Borra la fila de mensajes -------------------------------------------------
void LimpiarMensaje() {
  _gotoxy(1, FILA_MENSAJE);
  _clreol();
} // LimpiarMensaje

//--- Mensaje de error, en rojo -------------------------------------------------
void MensajeError(string texto) {
  LimpiarMensaje();
  MnsgBox(COL_IZQUIERDA, FILA_MENSAJE, ROJO_CLARO, texto);
  _textcolor(GRIS_CLARO);
} // MensajeError

//--- Mensaje de operacion exitosa, en verde ------------------------------------
void MensajeOk(string texto) {
  LimpiarMensaje();
  MnsgBox(COL_IZQUIERDA, FILA_MENSAJE, VERDE_CLARO, texto);
  _textcolor(GRIS_CLARO);
} // MensajeOk

//--- Mensaje informativo, en cian ----------------------------------------------
void MensajeInfo(string texto) {
  LimpiarMensaje();
  MnsgBox(COL_IZQUIERDA, FILA_MENSAJE, CIAN_CLARO, texto);
  _textcolor(GRIS_CLARO);
} // MensajeInfo

//--- Encabezado comun a todas las pantallas ------------------------------------
//    Dibuja el marco, el nombre del banco, el titulo de la pantalla y la fecha.
void Encabezado(string titulo) {
  _clrscr();
  Marco(1, 1, ANCHO_PANTALLA - 1, ALTO_PANTALLA, AZUL_CLARO);
  MnsgBox(COL_IZQUIERDA, FILA_TITULO, AMARILLO,
          "HOME BANKING MESOPOTAMIA");
  MnsgBox(ANCHO_PANTALLA - 15, FILA_TITULO, GRIS_OSCURO,
          FechaATextoCero(FechaDeHoy()));
  MnsgBox(COL_IZQUIERDA, FILA_TITULO + 1, BLANCO, titulo);
  MnsgBox(COL_IZQUIERDA, FILA_TITULO + 2, AZUL_CLARO,
          Separador(ANCHO_PANTALLA - 7, '-'));
  _textcolor(GRIS_CLARO);
} // Encabezado

//--- Encabezado con la identificacion del usuario conectado --------------------
void EncabezadoUsuario(string titulo, string apeNom, short nroUsuario) {
  Encabezado(titulo);
  MnsgBox(COL_IZQUIERDA, ALTO_PANTALLA - 1, GRIS_OSCURO,
          "Usuario " + to_string(nroUsuario) + ": " + apeNom);
  _textcolor(GRIS_CLARO);
} // EncabezadoUsuario

//--- Escribe un importe alineado a derecha en la posicion indicada -------------
void MostrarImporte(short x, short y, double importe, short ancho) {
  _gotoxy(x, y);
  cout << fixed << setprecision(2) << right << setw(ancho) << importe;
} // MostrarImporte

//==============================================================================
//  ENTRADA DE DATOS VALIDADA
//
//  Todas las lecturas se hacen con getline() sobre una cadena, y recien
//  despues se convierte y se valida. De esa manera el flujo cin nunca queda en
//  estado de error y no hace falta limpiarlo.
//  El ciclo de validacion es siempre 1-x (do..while), tal como pide el
//  enunciado para la validacion de datos.
//==============================================================================

//--- Lee una linea completa del teclado ----------------------------------------
void LeerLinea(char destino[], short largoMax) {
  string linea;

  getline(cin, linea);
  CopiarCadena(destino, linea, largoMax);
} // LeerLinea

//--- Lee una cadena no vacia ---------------------------------------------------
void LeerCadena(short x, short y, string rotulo, char destino[],
                short largoMax) {
  bool valido;

  do {
    _gotoxy(x, y);
    _clreol();
    _textcolor(BLANCO);
    cout << rotulo;
    _textcolor(AMARILLO);
    LeerLinea(destino, largoMax);
    QuitarBlancos(destino);
    valido = not CadenaVacia(destino);
    if (not valido)
      MensajeError("El dato no puede quedar vacio. Reingrese.");
    else
      LimpiarMensaje();
  } while (not valido);
  _textcolor(GRIS_CLARO);
} // LeerCadena

//--- Lee un numero entero dentro del rango [minimo, maximo] --------------------
long LeerEntero(short x, short y, string rotulo, long minimo, long maximo) {
  str30 texto;
  long  valor = 0;
  bool  valido;

  do {
    _gotoxy(x, y);
    _clreol();
    _textcolor(BLANCO);
    cout << rotulo;
    _textcolor(AMARILLO);
    LeerLinea(texto, 30);
    QuitarBlancos(texto);
    valido = CadenaAEntero(texto, valor) and valor >= minimo and valor <= maximo;
    if (not valido)
      MensajeError("Ingrese un numero entero entre " + to_string(minimo) +
                   " y " + to_string(maximo) + ".");
    else
      LimpiarMensaje();
  } while (not valido);
  _textcolor(GRIS_CLARO);
  return valor;
} // LeerEntero

//--- Lee un importe dentro del rango [minimo, maximo] --------------------------
double LeerImporte(short x, short y, string rotulo, double minimo, double maximo) {
  str30 texto;
  double valor = 0;
  bool  valido;

  do {
    _gotoxy(x, y);
    _clreol();
    _textcolor(BLANCO);
    cout << rotulo;
    _textcolor(AMARILLO);
    LeerLinea(texto, 30);
    QuitarBlancos(texto);
    valido = CadenaAReal(texto, valor) and valor >= minimo and valor <= maximo;
    if (not valido)
      MensajeError("Importe invalido. Debe estar entre " +
                   to_string((long)minimo) + " y " +
                   to_string((long)maximo) + ".");
    else
      LimpiarMensaje();
  } while (not valido);
  _textcolor(GRIS_CLARO);
  return valor;
} // LeerImporte

//--- Lee la opcion de un menu --------------------------------------------------
short LeerOpcion(short x, short y, short minimo, short maximo) {
  return (short)LeerEntero(x, y, "Opcion elegida...: ", minimo, maximo);
} // LeerOpcion

//--- Lee una clave mostrando asteriscos ----------------------------------------
void LeerClave(short x, short y, string rotulo, char destino[], short largoMax) {
  short i;
  char  tecla;

  _gotoxy(x, y);
  _clreol();
  _textcolor(BLANCO);
  cout << rotulo;
  _textcolor(AMARILLO);
  i = 0;
  tecla = (char)_getch();
  while (tecla != '\r' and tecla != '\n') {         // hasta oprimir ENTER
    if (tecla == '\b') {                            // retroceso
      if (i > 0) {
        i--;
        cout << "\b \b";
      }
    }
    else if (tecla >= ' ' and i < largoMax) {
      destino[i] = tecla;
      i++;
      cout << '*';
    }
    tecla = (char)_getch();
  }
  destino[i] = '\0';
  cout << endl;
  _textcolor(GRIS_CLARO);
} // LeerClave

//--- Lee una respuesta por si o por no -----------------------------------------
bool LeerSiNo(short x, short y, string rotulo) {
  str5 texto;
  bool valido;
  bool respuesta = false;

  do {
    _gotoxy(x, y);
    _clreol();
    _textcolor(BLANCO);
    cout << rotulo << " (S/N): ";
    _textcolor(AMARILLO);
    LeerLinea(texto, 5);
    QuitarBlancos(texto);
    AMayusculas(texto);
    valido = LargoCadena(texto) == 1 and (texto[0] == 'S' or texto[0] == 'N');
    if (valido) {
      respuesta = texto[0] == 'S';
      LimpiarMensaje();
    }
    else
      MensajeError("Responda con S (si) o con N (no).");
  } while (not valido);
  _textcolor(GRIS_CLARO);
  return respuesta;
} // LeerSiNo

//--- Lee una fecha completa validando el calendario ----------------------------
sFecha LeerFecha(short x, short y, string rotulo, sFecha porOmision) {
  sFecha fecha;
  bool   valida;

  MnsgBox(x, y, BLANCO, rotulo + " (ENTER = " + FechaATextoCero(porOmision) +
          ")");
  do {
    fecha.dia  = (short)LeerEntero(x + 2, y + 1, "Dia .....: ", 0, 31);
    if (fecha.dia == 0)
      return porOmision;
    fecha.mes  = (short)LeerEntero(x + 2, y + 2, "Mes .....: ", 1, 12);
    fecha.anio = (short)LeerEntero(x + 2, y + 3, "Anio ....: ", 2000, 2100);
    valida = FechaValida(fecha);
    if (not valida)
      MensajeError("La fecha no existe en el calendario. Reingrese.");
    else
      LimpiarMensaje();
  } while (not valida);
  return fecha;
} // LeerFecha

//==============================================================================
//  ORDENAMIENTO
//
//  Metodo de la burbuja optimizada: el ciclo externo es 1-x (do..while) y
//  termina apenas una pasada completa no produce ningun intercambio; el ciclo
//  interno es exacto y se acorta en una posicion por pasada, porque el mayor
//  de los elementos ya quedo ubicado al final.
//==============================================================================

//--- Ordena los movimientos de caja de ahorro por fecha ascendente ------------
void OrdenarMovCAPorFecha(sMovCA aMov[], short cant) {
  short  pasada = 0;
  short  j;
  bool   huboCambio;
  sMovCA aux;

  if (cant < 2)
    return;
  do {
    huboCambio = false;
    for (j = 0; j < cant - 1 - pasada; j++)
      if (FechaLarga(aMov[j].fecha) > FechaLarga(aMov[j + 1].fecha)) {
        aux          = aMov[j];
        aMov[j]      = aMov[j + 1];
        aMov[j + 1]  = aux;
        huboCambio   = true;
      }
    pasada++;
  } while (huboCambio and pasada < cant - 1);
} // OrdenarMovCAPorFecha

//--- Ordena los consumos de tarjeta de debito por fecha ascendente ------------
void OrdenarMovTDPorFecha(sMovTD aMov[], short cant) {
  short  pasada = 0;
  short  j;
  bool   huboCambio;
  sMovTD aux;

  if (cant < 2)
    return;
  do {
    huboCambio = false;
    for (j = 0; j < cant - 1 - pasada; j++)
      if (FechaLarga(aMov[j].fecha) > FechaLarga(aMov[j + 1].fecha)) {
        aux          = aMov[j];
        aMov[j]      = aMov[j + 1];
        aMov[j + 1]  = aux;
        huboCambio   = true;
      }
    pasada++;
  } while (huboCambio and pasada < cant - 1);
} // OrdenarMovTDPorFecha

//--- Ordena los consumos de tarjeta de debito por importe descendente ---------
void OrdenarMovTDPorImporte(sMovTD aMov[], short cant) {
  short  pasada = 0;
  short  j;
  bool   huboCambio;
  sMovTD aux;

  if (cant < 2)
    return;
  do {
    huboCambio = false;
    for (j = 0; j < cant - 1 - pasada; j++)
      if (aMov[j].importe < aMov[j + 1].importe) {
        aux          = aMov[j];
        aMov[j]      = aMov[j + 1];
        aMov[j + 1]  = aux;
        huboCambio   = true;
      }
    pasada++;
  } while (huboCambio and pasada < cant - 1);
} // OrdenarMovTDPorImporte

//--- Ordena los consumos de tarjeta de credito por fecha ascendente -----------
void OrdenarMovTCPorFecha(sMovTC aMov[], short cant) {
  short  pasada = 0;
  short  j;
  bool   huboCambio;
  sMovTC aux;

  if (cant < 2)
    return;
  do {
    huboCambio = false;
    for (j = 0; j < cant - 1 - pasada; j++)
      if (FechaLarga(aMov[j].fecha) > FechaLarga(aMov[j + 1].fecha)) {
        aux          = aMov[j];
        aMov[j]      = aMov[j + 1];
        aMov[j + 1]  = aux;
        huboCambio   = true;
      }
    pasada++;
  } while (huboCambio and pasada < cant - 1);
} // OrdenarMovTCPorFecha

//--- Ordena la agenda de CBU por alias ascendente ------------------------------
void OrdenarAgendaPorAlias(sAgenda aAgenda[], short cant) {
  short   pasada = 0;
  short   j;
  bool    huboCambio;
  sAgenda aux;

  if (cant < 2)
    return;
  do {
    huboCambio = false;
    for (j = 0; j < cant - 1 - pasada; j++)
      if (strcmp(aAgenda[j].alias, aAgenda[j + 1].alias) > 0) {
        aux             = aAgenda[j];
        aAgenda[j]      = aAgenda[j + 1];
        aAgenda[j + 1]  = aux;
        huboCambio      = true;
      }
    pasada++;
  } while (huboCambio and pasada < cant - 1);
} // OrdenarAgendaPorAlias

//==============================================================================
//  BUSQUEDA
//==============================================================================

//--- Busqueda SECUENCIAL del usuario por su DNI --------------------------------
//    El arreglo de usuarios no esta ordenado por DNI, de modo que la unica
//    busqueda posible es la secuencial. Devuelve el indice o NO_ENCONTRADO.
short BuscarUsuarioPorDni(const sDatosPers aDatosPers[], long dni) {
  short i = 0;

  while (i < CANT_USUARIOS and aDatosPers[i].dni != dni)
    i++;
  if (i < CANT_USUARIOS)
    return i;
  return NO_ENCONTRADO;
} // BuscarUsuarioPorDni

//--- Busqueda SECUENCIAL de una cuenta por su numero ---------------------------
short BuscarCuentaPorNro(const sCuenta aCuentas[], short cant,
                         const char nroCta[]) {
  short i = 0;

  while (i < cant and strcmp(aCuentas[i].nroCta, nroCta) != 0)
    i++;
  if (i < cant)
    return i;
  return NO_ENCONTRADO;
} // BuscarCuentaPorNro

//--- Busqueda SECUENCIAL de la primera cuenta de una moneda dada ---------------
short BuscarCuentaPorMoneda(const sCuenta aCuentas[], short cant,
                            const char moneda[]) {
  short i = 0;

  while (i < cant and (strcmp(aCuentas[i].moneda, moneda) != 0 or
                       strcmp(aCuentas[i].estado, "Activa") != 0))
    i++;
  if (i < cant)
    return i;
  return NO_ENCONTRADO;
} // BuscarCuentaPorMoneda

//--- Busqueda BINARIA del alias dentro de la agenda ----------------------------
//    Solo es aplicable porque la agenda se mantiene siempre ordenada por alias
//    (ver InsertarEnOrdenAgenda). Devuelve el indice o NO_ENCONTRADO.
short BuscarAliasEnAgenda(const sAgenda aAgenda[], short cant,
                          const char alias[]) {
  short primero = 0;
  short ultimo  = cant - 1;
  short medio;
  short comparacion;

  while (primero <= ultimo) {
    medio = (primero + ultimo) / 2;
    comparacion = (short)strcmp(aAgenda[medio].alias, alias);
    if (comparacion == 0)
      return medio;
    if (comparacion > 0)
      ultimo = medio - 1;
    else
      primero = medio + 1;
  }
  return NO_ENCONTRADO;
} // BuscarAliasEnAgenda

//--- Busqueda SECUENCIAL de un CBU dentro de la agenda -------------------------
short BuscarCbuEnAgenda(const sAgenda aAgenda[], short cant, const char cbu[]) {
  short i = 0;

  while (i < cant and strcmp(aAgenda[i].cbu, cbu) != 0)
    i++;
  if (i < cant)
    return i;
  return NO_ENCONTRADO;
} // BuscarCbuEnAgenda

//==============================================================================
//  INSERCION EN ORDEN
//==============================================================================

//--- Posicion que le corresponde al alias dentro de la agenda ordenada ---------
short PosicionDeInsercion(const sAgenda aAgenda[], short cant,
                          const char alias[]) {
  short i = 0;

  while (i < cant and strcmp(aAgenda[i].alias, alias) < 0)
    i++;
  return i;
} // PosicionDeInsercion

//--- Inserta una cuenta agendada conservando el orden por alias ----------------
//    Desplaza hacia la derecha las componentes posteriores a la posicion de
//    insercion y ubica la nueva en su lugar. Devuelve false si no hay espacio.
bool InsertarEnOrdenAgenda(sAgenda aAgenda[], short &cant,
                           const sAgenda &nueva) {
  short posicion;
  short i;

  if (cant >= MAX_AGENDA)
    return false;
  posicion = PosicionDeInsercion(aAgenda, cant, nueva.alias);
  for (i = cant; i > posicion; i--)
    aAgenda[i] = aAgenda[i - 1];
  aAgenda[posicion] = nueva;
  cant++;
  return true;
} // InsertarEnOrdenAgenda

//--- Elimina la componente que ocupa la posicion indicada ----------------------
void EliminarDeAgenda(sAgenda aAgenda[], short &cant, short posicion) {
  short i;

  for (i = posicion; i < cant - 1; i++)
    aAgenda[i] = aAgenda[i + 1];
  cant--;
} // EliminarDeAgenda

//==============================================================================
//  ARCHIVOS DE TEXTO - ARMADO E INTERPRETACION DE LA LINEA
//
//  Los tres archivos son de ancho fijo. Cada campo ocupa siempre la misma
//  cantidad de columnas: los numeros ajustados a derecha y las cadenas a
//  izquierda, rellenando con blancos. Gracias a eso todos los registros miden
//  lo mismo y se los puede alcanzar por acceso aleatorio calculando el
//  desplazamiento como nroReg * LARGO_REG.
//==============================================================================

//--- Interpreta las columnas 1 a 10 de cualquiera de los tres archivos --------
void InterpretarFecha(const char linea[], sFecha &fecha) {
  str10 campo;
  long  valor;

  ExtraerCampo(linea, COL_FECHA, 2, campo);
  fecha.dia = CadenaAEntero(campo, valor) ? (short)valor : 0;
  ExtraerCampo(linea, COL_FECHA + 3, 2, campo);
  fecha.mes = CadenaAEntero(campo, valor) ? (short)valor : 0;
  ExtraerCampo(linea, COL_FECHA + 6, 4, campo);
  fecha.anio = CadenaAEntero(campo, valor) ? (short)valor : 0;
} // InterpretarFecha

//------------------------------------------------------------------------------
//  MovimientosCA.Txt
//
//   col  1  2  3  4  5  6  7  8  9 10 11 12 13 14 ..... 43 44 ..... 56 57 58
//       [d  d] - [m  m] - [a  a  a  a]    [T]    [detalle X(30)][9(7).99 ][us]
//------------------------------------------------------------------------------
void ArmarLineaCA(const sMovCA &mov, char linea[]) {
  snprintf(linea, LARGO_LIN_CA + 1, "%2hd-%2hd-%4hd %c %-*s%*.2f%*hd",
           mov.fecha.dia, mov.fecha.mes, mov.fecha.anio, mov.tipoMov,
           (int)ANCHO_DETALLE, mov.detalle,
           (int)ANCHO_IMPORTE, mov.importe,
           (int)ANCHO_USUARIO, mov.nroUsuario);
} // ArmarLineaCA

void InterpretarLineaCA(const char linea[], sMovCA &mov) {
  str30 campo;
  long  entero;
  double real;

  InterpretarFecha(linea, mov.fecha);
  mov.tipoMov = linea[COL_TIPO_CA - 1];
  ExtraerCampo(linea, COL_DETALLE_CA, ANCHO_DETALLE, mov.detalle);
  ExtraerCampo(linea, COL_IMPORTE_CA, ANCHO_IMPORTE, campo);
  mov.importe = CadenaAReal(campo, real) ? real : 0;
  ExtraerCampo(linea, COL_USUARIO_CA, ANCHO_USUARIO, campo);
  mov.nroUsuario = CadenaAEntero(campo, entero) ? (short)entero : 0;
  mov.saldo  = 0;
  mov.nroReg = 0;
} // InterpretarLineaCA

//------------------------------------------------------------------------------
//  MovimientosTD.Txt
//
//   col  1 ..... 10 11 12 ..... 41 42 ..... 54 55 56
//       [99-99-9999]    [detalle X(30)][9(7).99 ][us]
//------------------------------------------------------------------------------
void ArmarLineaTD(const sMovTD &mov, char linea[]) {
  snprintf(linea, LARGO_LIN_TD + 1, "%2hd-%2hd-%4hd %-*s%*.2f%*hd",
           mov.fecha.dia, mov.fecha.mes, mov.fecha.anio,
           (int)ANCHO_DETALLE, mov.detalle,
           (int)ANCHO_IMPORTE, mov.importe,
           (int)ANCHO_USUARIO, mov.nroUsuario);
} // ArmarLineaTD

void InterpretarLineaTD(const char linea[], sMovTD &mov) {
  str30 campo;
  long  entero;
  double real;

  InterpretarFecha(linea, mov.fecha);
  ExtraerCampo(linea, COL_DETALLE_TD, ANCHO_DETALLE, mov.detalle);
  ExtraerCampo(linea, COL_IMPORTE_TD, ANCHO_IMPORTE, campo);
  mov.importe = CadenaAReal(campo, real) ? real : 0;
  ExtraerCampo(linea, COL_USUARIO_TD, ANCHO_USUARIO, campo);
  mov.nroUsuario = CadenaAEntero(campo, entero) ? (short)entero : 0;
} // InterpretarLineaTD

//------------------------------------------------------------------------------
//  MovimientosTC.Txt
//
//   col  1 ..... 10 11 12 ..... 41 42 . 47 48 ... 56 57 ..... 69 70 71
//       [99-99-9999]    [detalle X(30)][cuot][nroCpbte][9(7).99 ][us]
//
//   Los campos cuotas y nroCpbte pueden venir en blanco: significa que el
//   consumo fue en un solo pago o que la tarjeta no informo el comprobante.
//------------------------------------------------------------------------------
void ArmarLineaTC(const sMovTC &mov, char linea[]) {
  str10 cpbte;

  if (mov.nroCpbte > 0)
    CopiarCadena(cpbte, to_string(mov.nroCpbte), 10);
  else
    cpbte[0] = '\0';
  snprintf(linea, LARGO_LIN_TC + 1, "%2hd-%2hd-%4hd %-*s%*s%*s%*.2f%*hd",
           mov.fecha.dia, mov.fecha.mes, mov.fecha.anio,
           (int)ANCHO_DETALLE, mov.detalle,
           (int)ANCHO_CUOTAS, mov.cuotas,
           (int)ANCHO_CPBTE, cpbte,
           (int)ANCHO_IMPORTE, mov.importe,
           (int)ANCHO_USUARIO, mov.nroUsuario);
} // ArmarLineaTC

void InterpretarLineaTC(const char linea[], sMovTC &mov) {
  str30 campo;
  long  entero;
  double real;

  InterpretarFecha(linea, mov.fecha);
  ExtraerCampo(linea, COL_DETALLE_TC, ANCHO_DETALLE, mov.detalle);
  ExtraerCampo(linea, COL_CUOTAS_TC, ANCHO_CUOTAS, mov.cuotas);
  ExtraerCampo(linea, COL_CPBTE_TC, ANCHO_CPBTE, campo);
  mov.nroCpbte = CadenaAEntero(campo, entero) ? entero : 0;
  ExtraerCampo(linea, COL_IMPORTE_TC, ANCHO_IMPORTE, campo);
  mov.importe = CadenaAReal(campo, real) ? real : 0;
  ExtraerCampo(linea, COL_USUARIO_TC, ANCHO_USUARIO, campo);
  mov.nroUsuario = CadenaAEntero(campo, entero) ? (short)entero : 0;
} // InterpretarLineaTC

//==============================================================================
//  ARCHIVOS DE TEXTO - ACCESO SECUENCIAL
//==============================================================================

//--- Quita el retorno de carro que deja el fin de linea de Windows ------------
void QuitarRetornoCarro(string &linea) {
  while (linea.length() > 0 and
         (linea[linea.length() - 1] == '\r' or
          linea[linea.length() - 1] == '\n'))
    linea.erase(linea.length() - 1);
} // QuitarRetornoCarro

//--- Lee TODO el archivo de caja de ahorro, de principio a fin ----------------
//    Devuelve la cantidad de movimientos que quedaron en el arreglo. Si
//    nroUsuario es TODOS_USUARIOS trae los de los cinco usuarios.
short LeerMovimientosCA(sMovCA aMov[], short nroUsuario) {
  ifstream archivo(ARCH_MOV_CA);
  string   linea;
  str80    buffer;
  sMovCA   mov;
  short    cant = 0;
  short    nroReg = 0;

  if (not archivo.is_open())
    return 0;
  while (cant < MAX_MOV_CA and getline(archivo, linea)) {
    QuitarRetornoCarro(linea);
    if (linea.length() > 0) {
      CopiarCadena(buffer, linea, 80);
      InterpretarLineaCA(buffer, mov);
      mov.nroReg = nroReg;
      if (nroUsuario == TODOS_USUARIOS or mov.nroUsuario == nroUsuario) {
        aMov[cant] = mov;
        cant++;
      }
      nroReg++;
    }
  }
  archivo.close();
  return cant;
} // LeerMovimientosCA

//--- Lee TODO el archivo de tarjeta de debito ---------------------------------
short LeerMovimientosTD(sMovTD aMov[], short nroUsuario) {
  ifstream archivo(ARCH_MOV_TD);
  string   linea;
  str80    buffer;
  sMovTD   mov;
  short    cant = 0;

  if (not archivo.is_open())
    return 0;
  while (cant < MAX_MOV_TD and getline(archivo, linea)) {
    QuitarRetornoCarro(linea);
    if (linea.length() > 0) {
      CopiarCadena(buffer, linea, 80);
      InterpretarLineaTD(buffer, mov);
      if (nroUsuario == TODOS_USUARIOS or mov.nroUsuario == nroUsuario) {
        aMov[cant] = mov;
        cant++;
      }
    }
  }
  archivo.close();
  return cant;
} // LeerMovimientosTD

//--- Lee TODO el archivo de tarjeta de credito --------------------------------
short LeerMovimientosTC(sMovTC aMov[], short nroUsuario) {
  ifstream archivo(ARCH_MOV_TC);
  string   linea;
  str80    buffer;
  sMovTC   mov;
  short    cant = 0;

  if (not archivo.is_open())
    return 0;
  while (cant < MAX_MOV_TC and getline(archivo, linea)) {
    QuitarRetornoCarro(linea);
    if (linea.length() > 0) {
      CopiarCadena(buffer, linea, 80);
      InterpretarLineaTC(buffer, mov);
      if (nroUsuario == TODOS_USUARIOS or mov.nroUsuario == nroUsuario) {
        aMov[cant] = mov;
        cant++;
      }
    }
  }
  archivo.close();
  return cant;
} // LeerMovimientosTC

//==============================================================================
//  ARCHIVOS DE TEXTO - GRABAR NUEVAS COMPONENTES AL FINAL
//==============================================================================

//--- Graba la linea al final del archivo, sin tocar las ya existentes ---------
bool AgregarLinea(const char nombreArch[], const char linea[], short largoLin) {
  ofstream archivo(nombreArch, ios::app | ios::binary);

  if (not archivo.is_open())
    return false;
  archivo.write(linea, largoLin);
  archivo.write("\r\n", 2);          // fin de linea de Windows, explicito
  archivo.close();
  return true;
} // AgregarLinea

bool AgregarMovCA(const sMovCA &mov) {
  char linea[LARGO_LIN_CA + 1];

  ArmarLineaCA(mov, linea);
  return AgregarLinea(ARCH_MOV_CA, linea, LARGO_LIN_CA);
} // AgregarMovCA

bool AgregarMovTD(const sMovTD &mov) {
  char linea[LARGO_LIN_TD + 1];

  ArmarLineaTD(mov, linea);
  return AgregarLinea(ARCH_MOV_TD, linea, LARGO_LIN_TD);
} // AgregarMovTD

bool AgregarMovTC(const sMovTC &mov) {
  char linea[LARGO_LIN_TC + 1];

  ArmarLineaTC(mov, linea);
  return AgregarLinea(ARCH_MOV_TC, linea, LARGO_LIN_TC);
} // AgregarMovTC

//==============================================================================
//  ARCHIVOS DE TEXTO - ACCESO ALEATORIO
//
//  Como todos los registros ocupan exactamente LARGO_REG bytes (incluido el
//  CR LF), la componente numero n empieza en el byte n * LARGO_REG. Eso
//  permite leer o actualizar una componente cualquiera sin recorrer las
//  anteriores, es decir, en un orden distinto al de grabacion.
//==============================================================================

//--- Cantidad de componentes que tiene el archivo -----------------------------
short CantidadRegistros(const char nombreArch[], short largoReg) {
  ifstream archivo(nombreArch, ios::binary | ios::ate);
  long     tamanio;

  if (not archivo.is_open())
    return 0;
  tamanio = (long)archivo.tellg();
  archivo.close();
  return (short)(tamanio / largoReg);
} // CantidadRegistros

//--- Lee la componente nroReg del archivo de caja de ahorro -------------------
bool LeerRegistroCA(short nroReg, sMovCA &mov) {
  ifstream archivo(ARCH_MOV_CA, ios::binary);
  char     buffer[LARGO_LIN_CA + 1];

  if (not archivo.is_open() or nroReg < 0)
    return false;
  archivo.seekg((long)nroReg * LARGO_REG_CA, ios::beg);
  archivo.read(buffer, LARGO_LIN_CA);
  if (archivo.gcount() != LARGO_LIN_CA) {
    archivo.close();
    return false;
  }
  buffer[LARGO_LIN_CA] = '\0';
  archivo.close();
  InterpretarLineaCA(buffer, mov);
  mov.nroReg = nroReg;
  return true;
} // LeerRegistroCA

//--- Reescribe en su lugar la componente nroReg del archivo de caja de ahorro -
//    Se posiciona con seekp y sobreescribe unicamente los LARGO_LIN_CA bytes
//    de datos, dejando intacto el fin de linea.
bool ActualizarRegistroCA(short nroReg, const sMovCA &mov) {
  fstream archivo(ARCH_MOV_CA, ios::in | ios::out | ios::binary);
  char    linea[LARGO_LIN_CA + 1];

  if (not archivo.is_open() or nroReg < 0)
    return false;
  ArmarLineaCA(mov, linea);
  archivo.seekp((long)nroReg * LARGO_REG_CA, ios::beg);
  archivo.write(linea, LARGO_LIN_CA);
  archivo.close();
  return true;
} // ActualizarRegistroCA

//--- Lee la componente nroReg del archivo de tarjeta de credito ---------------
bool LeerRegistroTC(short nroReg, sMovTC &mov) {
  ifstream archivo(ARCH_MOV_TC, ios::binary);
  char     buffer[LARGO_LIN_TC + 1];

  if (not archivo.is_open() or nroReg < 0)
    return false;
  archivo.seekg((long)nroReg * LARGO_REG_TC, ios::beg);
  archivo.read(buffer, LARGO_LIN_TC);
  if (archivo.gcount() != LARGO_LIN_TC) {
    archivo.close();
    return false;
  }
  buffer[LARGO_LIN_TC] = '\0';
  archivo.close();
  InterpretarLineaTC(buffer, mov);
  return true;
} // LeerRegistroTC

//==============================================================================
//  CALCULOS SOBRE LOS MOVIMIENTOS
//==============================================================================

//--- Calcula el saldo acumulado de cada movimiento -----------------------------
//    Los movimientos deben venir ordenados por fecha. Devuelve el saldo final.
double CalcularSaldos(sMovCA aMov[], short cant, double saldoInicial) {
  double saldo = saldoInicial;
  short i;

  for (i = 0; i < cant; i++) {
    if (aMov[i].tipoMov == DEBE)
      saldo = saldo + aMov[i].importe;
    else
      saldo = saldo - aMov[i].importe;
    aMov[i].saldo = saldo;
  }
  return saldo;
} // CalcularSaldos

//--- Total debitado y acreditado en la caja de ahorro --------------------------
void TotalizarMovCA(const sMovCA aMov[], short cant, double &totalDebe,
                    double &totalHaber) {
  short i;

  totalDebe  = 0;
  totalHaber = 0;
  for (i = 0; i < cant; i++)
    if (aMov[i].tipoMov == DEBE)
      totalDebe = totalDebe + aMov[i].importe;
    else
      totalHaber = totalHaber + aMov[i].importe;
} // TotalizarMovCA

//--- Total consumido con la tarjeta de debito ----------------------------------
double TotalizarMovTD(const sMovTD aMov[], short cant) {
  double total = 0;
  short i;

  for (i = 0; i < cant; i++)
    total = total + aMov[i].importe;
  return total;
} // TotalizarMovTD

//--- Total consumido con la tarjeta de credito ---------------------------------
double TotalizarMovTC(const sMovTC aMov[], short cant) {
  double total = 0;
  short i;

  for (i = 0; i < cant; i++)
    total = total + aMov[i].importe;
  return total;
} // TotalizarMovTC

//--- Recalcula el saldo de la caja de ahorro en pesos leyendo el archivo ------
//    Es la unica cuenta gobernada por el archivo de movimientos: su saldo es
//    el saldo inicial mas los DEBE menos los HABER.
void RecalcularSaldoCA(sDatosPers &datos, short nroUsuario) {
  sMovCA aMov[MAX_MOV_CA];
  short  cant;
  double  totalDebe, totalHaber;

  cant = LeerMovimientosCA(aMov, nroUsuario);
  TotalizarMovCA(aMov, cant, totalDebe, totalHaber);
  datos.aCuentas[CTA_PESOS].saldo =
      datos.aCuentas[CTA_PESOS].saldoInicial + totalDebe - totalHaber;
} // RecalcularSaldoCA

//==============================================================================
//  CARGA DE LOS DATOS PERSONALES DE LOS CINCO USUARIOS
//
//  Los datos se registran internamente en el codigo, tal como pide el
//  enunciado, combinando el arreglo aDatosPers con el registro sDatosPers.
//  El nro. de usuario que se graba en la ultima columna de los archivos de
//  movimientos es el indice del arreglo mas uno.
//==============================================================================

//--- Carga una cuenta bancaria dentro del arreglo de cuentas del usuario ------
void CargarCuenta(sCuenta &cuenta, const char nroCta[], const char tipo[],
                  const char moneda[], double saldoInicial) {
  CopiarCadena(cuenta.nroCta, nroCta, 20);
  CopiarCadena(cuenta.tipo,   tipo,    5);
  CopiarCadena(cuenta.moneda, moneda,  5);
  CopiarCadena(cuenta.estado, "Activa", 10);
  cuenta.saldoInicial = saldoInicial;
  cuenta.saldo        = saldoInicial;
} // CargarCuenta

//--- Carga los datos de una tarjeta -------------------------------------------
void CargarTarjeta(sTarjeta &tarjeta, const char tipo[], const char nro[],
                   short mesVto, short anioVto, double limite) {
  CopiarCadena(tarjeta.tipo, tipo, 5);
  CopiarCadena(tarjeta.nroTarjeta, nro, 25);
  tarjeta.vence.dia  = 1;
  tarjeta.vence.mes  = mesVto;
  tarjeta.vence.anio = anioVto;
  tarjeta.limite     = limite;
} // CargarTarjeta

//--- Agenda una cuenta de tercero conservando el orden por alias --------------
void CargarAgenda(sDatosPers &datos, const char alias[], const char titular[],
                  const char cbu[]) {
  sAgenda nueva;

  CopiarCadena(nueva.alias,   alias,   20);
  CopiarCadena(nueva.titular, titular, 30);
  CopiarCadena(nueva.cbu,     cbu,     25);
  InsertarEnOrdenAgenda(datos.aAgenda, datos.cantAgenda, nueva);
} // CargarAgenda

//--- Carga los datos comunes de un usuario ------------------------------------
void CargarUsuario(sDatosPers &datos, long dni, const char apeNom[],
                   short diaNac, short mesNac, short anioNac,
                   const char usuario[], const char clave[],
                   const char celular[], const char email[],
                   const char domicilio[], const char nroCta[],
                   const char cbu[], double saldoPesos, double saldoDolares) {
  str20 nroCtaDolares;

  datos.dni = dni;
  CopiarCadena(datos.apeNom,    apeNom,    30);
  datos.fechaNac.dia  = diaNac;
  datos.fechaNac.mes  = mesNac;
  datos.fechaNac.anio = anioNac;
  CopiarCadena(datos.usuario,   usuario,   15);
  CopiarCadena(datos.clave,     clave,     15);
  CopiarCadena(datos.celular,   celular,   15);
  CopiarCadena(datos.email,     email,     30);
  CopiarCadena(datos.domicilio, domicilio, 40);
  CopiarCadena(datos.nroCtaCA,  nroCta,    20);
  CopiarCadena(datos.cbu,       cbu,       25);

  // La caja de ahorro en pesos es siempre la componente CTA_PESOS: es la
  // unica cuenta cuyo saldo se reconstruye desde MovimientosCA.Txt.
  CargarCuenta(datos.aCuentas[CTA_PESOS], nroCta, "CA", "$", saldoPesos);

  // La caja de ahorro en dolares comparte el numero, terminado en U.
  CopiarCadena(nroCtaDolares, nroCta, 20);
  nroCtaDolares[LargoCadena(nroCtaDolares) - 1] = 'U';
  CargarCuenta(datos.aCuentas[CTA_DOLARES], nroCtaDolares, "CA", "u$s",
               saldoDolares);

  datos.cantCuentas = 2;
  datos.cantAgenda  = 0;
  datos.cantPlazos  = 0;
} // CargarUsuario

//--- Carga los cinco usuarios registrados en el sistema -----------------------
void CargarDatosPersonales(sDatosPers aDatosPers[]) {

  //--- Usuario 1 --------------------------------------------------------------
  CargarUsuario(aDatosPers[0], 30125478, "ALVAREZ MARIA SOLEDAD",
                14, 3, 1983, "malvarez", "Sol2026", "3414785296",
                "malvarez@correo.com", "Av. Pellegrini 1450 - Rosario",
                "106-4235689707-3", "1060099920000423568970",
                1250000.00, 4820.55);
  CargarTarjeta(aDatosPers[0].tarjetaTC, "TC", "4517 78** **** 3041",
                8, 2029, 1500000.00);
  CargarTarjeta(aDatosPers[0].tarjetaTD, "TD", "5045 21** **** 7712",
                5, 2028, 800000.00);
  CargarAgenda(aDatosPers[0], "alquiler.depto", "SOSA INMOBILIARIA SRL",
               "0720099950000078912345");
  CargarAgenda(aDatosPers[0], "gas.litoral", "LITORAL GAS S.A.",
               "0170099940000012345678");
  CargarAgenda(aDatosPers[0], "juan.perez", "PEREZ JUAN MANUEL",
               "0290099910000098765432");
  CargarAgenda(aDatosPers[0], "mama", "ALVAREZ ELSA NOEMI",
               "1910099930000045678912");

  //--- Usuario 2 --------------------------------------------------------------
  CargarUsuario(aDatosPers[1], 27894561, "GOMEZ CARLOS ALBERTO",
                2, 11, 1979, "cgomez", "Car2026", "3415123478",
                "cgomez@correo.com", "Sarmiento 2380 Piso 4 - Rosario",
                "106-4235710244-8", "1060099920000423571020",
                890000.00, 2150.00);
  CargarTarjeta(aDatosPers[1].tarjetaTC, "TC", "4517 78** **** 6120",
                11, 2028, 900000.00);
  CargarTarjeta(aDatosPers[1].tarjetaTD, "TD", "5045 21** **** 4483",
                3, 2029, 500000.00);
  CargarAgenda(aDatosPers[1], "ana.lopez", "LOPEZ ANA VALERIA",
               "1060099920000423573390");
  CargarAgenda(aDatosPers[1], "cochera.centro", "GARAGE SAN MARTIN SRL",
               "0720099950000011223344");
  CargarAgenda(aDatosPers[1], "expensas", "CONSORCIO SARMIENTO 2380",
               "0170099940000055667788");

  //--- Usuario 3 --------------------------------------------------------------
  CargarUsuario(aDatosPers[2], 35678214, "LOPEZ ANA VALERIA",
                25, 6, 1990, "alopez", "Ana2026", "3416987541",
                "alopez@correo.com", "Mendoza 4125 - Rosario",
                "106-4235733901-1", "1060099920000423573390",
                640000.00, 980.40);
  CargarTarjeta(aDatosPers[2].tarjetaTC, "TC", "4517 78** **** 8355",
                2, 2030, 700000.00);
  CargarTarjeta(aDatosPers[2].tarjetaTD, "TD", "5045 21** **** 9061",
                9, 2028, 400000.00);
  CargarAgenda(aDatosPers[2], "club.nautico", "CLUB NAUTICO AVELLANEDA",
               "0290099910000033445566");
  CargarAgenda(aDatosPers[2], "ferreteria", "BLACK DECKER DISTRIB SA",
               "0720099950000099001122");
  CargarAgenda(aDatosPers[2], "jorge.martinez", "MARTINEZ JORGE DANIEL",
               "1060099920000423575631");
  CargarAgenda(aDatosPers[2], "veterinaria", "VETERINARIA HUELLAS SRL",
               "1910099930000077889900");

  //--- Usuario 4 --------------------------------------------------------------
  CargarUsuario(aDatosPers[3], 24567890, "MARTINEZ JORGE DANIEL",
                9, 1, 1975, "jmartinez", "Jor2026", "3413654789",
                "jmartinez@correo.com", "Bv. Orono 985 - Rosario",
                "106-4235756318-6", "1060099920000423575631",
                1520000.00, 7310.25);
  CargarTarjeta(aDatosPers[3].tarjetaTC, "TC", "4517 78** **** 1274",
                6, 2029, 2000000.00);
  CargarTarjeta(aDatosPers[3].tarjetaTD, "TD", "5045 21** **** 3390",
                12, 2028, 1000000.00);
  CargarAgenda(aDatosPers[3], "colegio", "INSTITUTO SAN JOSE",
               "0170099940000066778899");
  CargarAgenda(aDatosPers[3], "elsa.rodriguez", "RODRIGUEZ ELSA NOEMI",
               "1060099920000423577942");
  CargarAgenda(aDatosPers[3], "seguro.auto", "MESOPOTAMIA SEGUROS SA",
               "0290099910000022334455");

  //--- Usuario 5 --------------------------------------------------------------
  CargarUsuario(aDatosPers[4], 12345678, "RODRIGUEZ ELSA NOEMI",
                30, 8, 1958, "erodriguez", "Els2026", "3417412589",
                "erodriguez@correo.com", "Zeballos 1720 - Rosario",
                "106-4235779425-4", "1060099920000423577942",
                2100000.00, 15400.00);
  CargarTarjeta(aDatosPers[4].tarjetaTC, "TC", "4517 78** **** 5508",
                4, 2030, 1200000.00);
  CargarTarjeta(aDatosPers[4].tarjetaTD, "TD", "5045 21** **** 6647",
                7, 2029, 600000.00);
  CargarAgenda(aDatosPers[4], "carlos.gomez", "GOMEZ CARLOS ALBERTO",
               "1060099920000423571020");
  CargarAgenda(aDatosPers[4], "farmacia", "FARMACIA DEL CENTRO SRL",
               "0720099950000044556677");
  CargarAgenda(aDatosPers[4], "nieta.sole", "ALVAREZ MARIA SOLEDAD",
               "1060099920000423568970");
  CargarAgenda(aDatosPers[4], "prepaga", "SALUD INTEGRAL SA",
               "0170099940000088990011");
} // CargarDatosPersonales

//==============================================================================
//  OPERACIONES COMUNES A VARIOS MODULOS
//==============================================================================

//--- Saldo disponible en la caja de ahorro en pesos ---------------------------
double SaldoEnPesos(const sDatosPers &datos) {
  return datos.aCuentas[CTA_PESOS].saldo;
} // SaldoEnPesos

//--- Muestra el saldo de la caja de ahorro en pesos en la fila indicada -------
void MostrarSaldoDisponible(const sDatosPers &datos, short fila) {
  MnsgBox(COL_IZQUIERDA, fila, CIAN_CLARO, "Saldo disponible en CA $ ..: ");
  _textcolor(BLANCO);
  cout << fixed << setprecision(2) << right << setw(14) << SaldoEnPesos(datos);
  _textcolor(GRIS_CLARO);
} // MostrarSaldoDisponible

//--- Registra un movimiento en la caja de ahorro ------------------------------
//    Graba la nueva componente al final de MovimientosCA.Txt y actualiza el
//    saldo que el programa mantiene en memoria.
bool RegistrarMovCA(sDatosPers &datos, short nroUsuario, char tipoMov,
                    const char detalle[], double importe, sFecha fecha) {
  sMovCA mov;

  mov.fecha      = fecha;
  mov.tipoMov    = tipoMov;
  CopiarCadena(mov.detalle, detalle, ANCHO_DETALLE);
  mov.importe    = importe;
  mov.nroUsuario = nroUsuario;
  mov.saldo      = 0;
  mov.nroReg     = 0;
  if (not AgregarMovCA(mov))
    return false;
  if (tipoMov == DEBE)
    datos.aCuentas[CTA_PESOS].saldo += importe;
  else
    datos.aCuentas[CTA_PESOS].saldo -= importe;
  return true;
} // RegistrarMovCA

//--- Controla que la caja de ahorro tenga saldo suficiente --------------------
bool HaySaldoSuficiente(const sDatosPers &datos, double importe) {
  return SaldoEnPesos(datos) >= importe;
} // HaySaldoSuficiente

//--- Genera un token de seguridad de seis digitos al azar ---------------------
long GenerarToken() {
  return rand() % (TOKEN_MAXIMO - TOKEN_MINIMO + 1) + TOKEN_MINIMO;
} // GenerarToken

//--- Exige el token de seguridad para confirmar una operacion -----------------
//    Muestra el token generado y pide que se lo transcriba. Devuelve true si
//    el usuario lo ingresa correctamente dentro de los intentos permitidos.
bool ValidarToken(short x, short y) {
  long  token = GenerarToken();
  long  ingresado;
  short intentos = 0;
  bool  correcto = false;

  MnsgBox(x, y, AMARILLO, "Token de seguridad enviado a su celular: " +
          to_string(token));
  do {
    ingresado = LeerEntero(x, y + 1, "Ingrese el token .....: ",
                           TOKEN_MINIMO, TOKEN_MAXIMO);
    intentos++;
    correcto = ingresado == token;
    if (not correcto)
      MensajeError("Token incorrecto. Intento " + to_string(intentos) +
                   " de " + to_string(MAX_INTENTOS) + ".");
  } while (not correcto and intentos < MAX_INTENTOS);
  if (correcto)
    MensajeOk("Token validado correctamente.");
  else
    MensajeError("Token incorrecto. La operacion fue cancelada.");
  return correcto;
} // ValidarToken

//--- Cantidad total de cuentas creadas en el sistema --------------------------
//    Se usa para numerar correlativamente las cuentas nuevas.
short TotalCuentasDelSistema(const sDatosPers aDatosPers[]) {
  short total = 0;
  short i;

  for (i = 0; i < CANT_USUARIOS; i++)
    total = total + aDatosPers[i].cantCuentas;
  return total;
} // TotalCuentasDelSistema

//--- Arma el numero de una cuenta nueva: 009-0000099 --------------------------
void ArmarNroCuentaNueva(short correlativo, char nroCta[]) {
  string texto = to_string(correlativo);

  while (texto.length() < 7)
    texto = "0" + texto;
  texto = "00" + to_string(SUCURSAL) + "-" + texto;
  CopiarCadena(nroCta, texto, 20);
} // ArmarNroCuentaNueva

//--- Valida un CBU: veintidos digitos -----------------------------------------
bool CbuValido(const char cbu[]) {
  return LargoCadena(cbu) == LARGO_CBU and SoloDigitos(cbu);
} // CbuValido

//--- Valida un numero de celular: diez digitos --------------------------------
bool CelularValido(const char celular[]) {
  return LargoCadena(celular) == LARGO_CELULAR and SoloDigitos(celular);
} // CelularValido

//==============================================================================
//  MODULOS DE NIVEL 2 - CONSULTAS
//==============================================================================

//------------------------------------------------------------------------------
//  Menu_MisCuentas
//  Lista los datos bancarios de todas las cuentas del usuario: numero, tipo,
//  moneda, saldo y estado.
//------------------------------------------------------------------------------
void Menu_MisCuentas(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short fila;
  short i;

  EncabezadoUsuario("MIS CUENTAS", datos.apeNom, usuario + 1);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, BLANCO,
          "Nro. Cuenta         Tipo  Moneda             Saldo  Estado");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, AZUL_CLARO, Separador(62, '-'));
  fila = FILA_CUERPO + 2;
  for (i = 0; i < datos.cantCuentas; i++) {
    _gotoxy(COL_IZQUIERDA, fila);
    _textcolor(GRIS_CLARO);
    cout << left  << setw(20) << datos.aCuentas[i].nroCta
         << left  << setw(6)  << datos.aCuentas[i].tipo
         << left  << setw(8)  << datos.aCuentas[i].moneda
         << right << setw(16) << fixed << setprecision(2)
         << datos.aCuentas[i].saldo
         << "  "  << left << setw(10) << datos.aCuentas[i].estado;
    fila++;
  }
  MnsgBox(COL_IZQUIERDA, fila, AZUL_CLARO, Separador(62, '-'));
  MnsgBox(COL_IZQUIERDA, fila + 1, CIAN_CLARO,
          "Cantidad de cuentas: " + to_string(datos.cantCuentas));
  MnsgBox(COL_IZQUIERDA, fila + 3, GRIS_OSCURO,
          "El saldo de la caja de ahorro en pesos surge de aplicar los");
  MnsgBox(COL_IZQUIERDA, fila + 4, GRIS_OSCURO,
          "movimientos del archivo " + string(ARCH_MOV_CA) +
          " sobre el saldo inicial.");
  Pausa("Oprima una tecla para volver al menu principal...");
} // Menu_MisCuentas

//------------------------------------------------------------------------------
//  Menu_DatosPers
//  Muestra el registro completo de datos personales del usuario conectado.
//------------------------------------------------------------------------------
void Menu_DatosPers(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short fila = FILA_CUERPO;

  EncabezadoUsuario("MIS DATOS PERSONALES", datos.apeNom, usuario + 1);
  MnsgBox(COL_IZQUIERDA, fila,      BLANCO, "Nro. de usuario ........: ");
  cout << usuario + 1;
  MnsgBox(COL_IZQUIERDA, fila +  1, BLANCO, "DNI ....................: ");
  cout << datos.dni;
  MnsgBox(COL_IZQUIERDA, fila +  2, BLANCO, "Apellido y Nombre ......: ");
  cout << datos.apeNom;
  MnsgBox(COL_IZQUIERDA, fila +  3, BLANCO, "Fecha de nacimiento ....: ");
  cout << FechaATextoCero(datos.fechaNac);
  MnsgBox(COL_IZQUIERDA, fila +  4, BLANCO, "Usuario ................: ");
  cout << datos.usuario;
  MnsgBox(COL_IZQUIERDA, fila +  5, BLANCO, "Clave ..................: ");
  cout << string(LargoCadena(datos.clave), '*');
  MnsgBox(COL_IZQUIERDA, fila +  6, BLANCO, "Nro. de celular ........: ");
  cout << datos.celular;
  MnsgBox(COL_IZQUIERDA, fila +  7, BLANCO, "Correo electronico .....: ");
  cout << datos.email;
  MnsgBox(COL_IZQUIERDA, fila +  8, BLANCO, "Domicilio ..............: ");
  cout << datos.domicilio;
  MnsgBox(COL_IZQUIERDA, fila +  9, BLANCO, "Nro. Cta. Caja Ahorro ..: ");
  cout << datos.nroCtaCA;
  MnsgBox(COL_IZQUIERDA, fila + 10, BLANCO, "CBU ....................: ");
  cout << datos.cbu;
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver al menu principal...");
} // Menu_DatosPers

//------------------------------------------------------------------------------
//  Menu_MostrarCBU
//  Muestra el CBU de 22 digitos y el alias de la cuenta del usuario.
//------------------------------------------------------------------------------
void Menu_MostrarCBU(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short i;
  short fila = FILA_CUERPO;

  EncabezadoUsuario("MI CBU", datos.apeNom, usuario + 1);
  MnsgBox(COL_IZQUIERDA, fila, BLANCO, "Titular ................: ");
  cout << datos.apeNom;
  MnsgBox(COL_IZQUIERDA, fila + 1, BLANCO, "Nro. de cuenta .........: ");
  cout << datos.nroCtaCA;
  MnsgBox(COL_IZQUIERDA, fila + 2, BLANCO, "Tipo de cuenta .........: ");
  cout << "Caja de Ahorro en pesos";
  MnsgBox(COL_IZQUIERDA, fila + 4, BLANCO, "CBU (22 digitos)");
  Marco(COL_IZQUIERDA, fila + 5, COL_IZQUIERDA + 33, fila + 7, AMARILLO);
  _gotoxy(COL_IZQUIERDA + 3, fila + 6);
  _textcolor(AMARILLO);
  // Se muestra el CBU separado en bloques para facilitar su lectura.
  for (i = 0; i < LARGO_CBU; i++) {
    cout << datos.cbu[i];
    if ((i + 1) % 4 == 0 and i + 1 < LARGO_CBU)
      cout << ' ';
  }
  MnsgBox(COL_IZQUIERDA, fila + 9, BLANCO, "Bloque de entidad ......: ");
  _textcolor(GRIS_CLARO);
  for (i = 0; i < 8; i++)
    cout << datos.cbu[i];
  MnsgBox(COL_IZQUIERDA, fila + 10, BLANCO, "Bloque de cuenta .......: ");
  _textcolor(GRIS_CLARO);
  for (i = 8; i < LARGO_CBU; i++)
    cout << datos.cbu[i];
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver al menu principal...");
} // Menu_MostrarCBU

//==============================================================================
//  MODULOS DE NIVEL 2 - MOVIMIENTOS
//==============================================================================

//------------------------------------------------------------------------------
//  Movimientos de la caja de ahorro
//  Lectura SECUENCIAL del archivo, ordenamiento por fecha con burbuja
//  optimizada, calculo del saldo acumulado y emision paginada por pantalla.
//------------------------------------------------------------------------------
void MostrarMovimientosCA(const sDatosPers &datos, short nroUsuario) {
  sMovCA aMov[MAX_MOV_CA];
  short  cant, i, fila, enPagina;
  double  totalDebe, totalHaber, saldoFinal;

  cant = LeerMovimientosCA(aMov, nroUsuario);
  OrdenarMovCAPorFecha(aMov, cant);
  saldoFinal = CalcularSaldos(aMov, cant,
                              datos.aCuentas[CTA_PESOS].saldoInicial);
  TotalizarMovCA(aMov, cant, totalDebe, totalHaber);

  i = 0;
  do {
    EncabezadoUsuario("MOVIMIENTOS DE LA CAJA DE AHORRO EN PESOS",
                      datos.apeNom, nroUsuario);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO, "Cta.CA .: ");
    _textcolor(BLANCO);
    cout << datos.aCuentas[CTA_PESOS].nroCta;
    MnsgBox(COL_IZQUIERDA + 34, FILA_CUERPO, CIAN_CLARO, "Saldo inicial $ ");
    _textcolor(BLANCO);
    cout << fixed << setprecision(2) << right << setw(13)
         << datos.aCuentas[CTA_PESOS].saldoInicial;
    // El encabezado se arma con los mismos manipuladores que las filas de
    // detalle, de modo que las columnas no puedan quedar desalineadas.
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 2);
    _textcolor(BLANCO);
    cout << right << setw(3) << "Nro"
         << ' '   << left  << setw(ANCHO_FECHA) << "Fecha"
         << " T " << left  << setw(ANCHO_DET_PANT) << "Detalle"
         << right << setw(11) << "Debe" << setw(12) << "Haber"
         << setw(12) << "Saldo";
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, AZUL_CLARO,
            Separador(ANCHO_TABLA_CA, '-'));
    fila = FILA_CUERPO + 4;
    enPagina = 0;
    while (i < cant and enPagina < FILAS_PAGINA) {
      _gotoxy(COL_IZQUIERDA, fila);
      _textcolor(GRIS_CLARO);
      cout << right << setw(3)  << aMov[i].nroReg
           << ' '   << setw(ANCHO_FECHA) << FechaATexto(aMov[i].fecha)
           << ' '   << aMov[i].tipoMov
           << ' '   << left
           << setw(ANCHO_DET_PANT) << Recortar(aMov[i].detalle,
                                               ANCHO_DET_PANT)
           << fixed << setprecision(2) << right;
      if (aMov[i].tipoMov == DEBE)
        cout << setw(11) << aMov[i].importe << setw(12) << " ";
      else
        cout << setw(11) << " " << setw(12) << aMov[i].importe;
      cout << setw(12) << aMov[i].saldo;
      i++;
      enPagina++;
      fila++;
    }
    if (i < cant)
      Pausa("Oprima una tecla para ver la pagina siguiente...");
  } while (i < cant);

  MnsgBox(COL_IZQUIERDA, fila, AZUL_CLARO, Separador(ANCHO_TABLA_CA, '-'));
  _gotoxy(COL_IZQUIERDA, fila + 1);
  _textcolor(BLANCO);
  cout << left << setw(ANCHO_DET_PANT + 17) << "  Totales"
       << fixed << setprecision(2) << right
       << setw(11) << totalDebe << setw(12) << totalHaber
       << setw(12) << saldoFinal;
  MnsgBox(COL_IZQUIERDA, fila + 2, CIAN_CLARO,
          "Cantidad de movimientos: " + to_string(cant));
  if (cant == 0)
    MensajeInfo("La cuenta no registra movimientos en el archivo.");
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver...");
} // MostrarMovimientosCA

//------------------------------------------------------------------------------
//  Movimientos de la tarjeta de debito
//------------------------------------------------------------------------------
void MostrarMovimientosTD(const sDatosPers &datos, short nroUsuario,
                          bool porImporte) {
  sMovTD aMov[MAX_MOV_TD];
  short  cant, i, fila, enPagina;
  double  total;

  cant = LeerMovimientosTD(aMov, nroUsuario);
  if (porImporte)
    OrdenarMovTDPorImporte(aMov, cant);
  else
    OrdenarMovTDPorFecha(aMov, cant);
  total = TotalizarMovTD(aMov, cant);

  i = 0;
  do {
    EncabezadoUsuario("CONSUMOS DE LA TARJETA DE DEBITO", datos.apeNom,
                      nroUsuario);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO, "Tarjeta .: ");
    _textcolor(BLANCO);
    cout << datos.tarjetaTD.nroTarjeta;
    MnsgBox(COL_IZQUIERDA + 34, FILA_CUERPO, CIAN_CLARO, "Ordenado por ");
    _textcolor(BLANCO);
    cout << (porImporte ? "importe descendente" : "fecha ascendente");
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 2);
    _textcolor(BLANCO);
    cout << left  << setw(ANCHO_FECHA) << "Fecha"
         << ' '   << left  << setw(ANCHO_DETALLE) << "Comercio"
         << right << setw(13) << "Importe";
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, AZUL_CLARO,
            Separador(ANCHO_TABLA_TD, '-'));
    fila = FILA_CUERPO + 4;
    enPagina = 0;
    while (i < cant and enPagina < FILAS_PAGINA) {
      _gotoxy(COL_IZQUIERDA, fila);
      _textcolor(GRIS_CLARO);
      cout << setw(ANCHO_FECHA) << FechaATexto(aMov[i].fecha)
           << ' ' << left
           << setw(ANCHO_DETALLE) << Recortar(aMov[i].detalle, ANCHO_DETALLE)
           << fixed << setprecision(2) << right << setw(13) << aMov[i].importe;
      i++;
      enPagina++;
      fila++;
    }
    if (i < cant)
      Pausa("Oprima una tecla para ver la pagina siguiente...");
  } while (i < cant);

  MnsgBox(COL_IZQUIERDA, fila, AZUL_CLARO, Separador(ANCHO_TABLA_TD, '-'));
  _gotoxy(COL_IZQUIERDA, fila + 1);
  _textcolor(BLANCO);
  cout << left << setw(ANCHO_TABLA_TD - 13) << "                    Total"
       << fixed << setprecision(2) << right << setw(13) << total;
  MnsgBox(COL_IZQUIERDA, fila + 2, CIAN_CLARO,
          "Cantidad de consumos: " + to_string(cant));
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver...");
} // MostrarMovimientosTD

//------------------------------------------------------------------------------
//  Movimientos de la tarjeta de credito
//------------------------------------------------------------------------------
void MostrarMovimientosTC(const sDatosPers &datos, short nroUsuario) {
  sMovTC aMov[MAX_MOV_TC];
  short  cant, i, fila, enPagina;
  double  total;

  cant = LeerMovimientosTC(aMov, nroUsuario);
  OrdenarMovTCPorFecha(aMov, cant);
  total = TotalizarMovTC(aMov, cant);

  i = 0;
  do {
    EncabezadoUsuario("CONSUMOS DE LA TARJETA DE CREDITO", datos.apeNom,
                      nroUsuario);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO, "Tarjeta .: ");
    _textcolor(BLANCO);
    cout << datos.tarjetaTC.nroTarjeta;
    MnsgBox(COL_IZQUIERDA + 34, FILA_CUERPO, CIAN_CLARO, "Limite de compra $ ");
    _textcolor(BLANCO);
    cout << fixed << setprecision(2) << datos.tarjetaTC.limite;
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 2);
    _textcolor(BLANCO);
    cout << left  << setw(ANCHO_FECHA) << "Fecha"
         << ' '   << left  << setw(ANCHO_DETALLE) << "Comercio"
         << right << setw(ANCHO_CUOTAS) << "Cuotas"
         << setw(ANCHO_CPBTE + 1) << "Nro.Cpbte"
         << setw(ANCHO_IMPORTE) << "Importe";
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, AZUL_CLARO,
            Separador(ANCHO_TABLA_TC, '-'));
    fila = FILA_CUERPO + 4;
    enPagina = 0;
    while (i < cant and enPagina < FILAS_PAGINA) {
      _gotoxy(COL_IZQUIERDA, fila);
      _textcolor(GRIS_CLARO);
      cout << setw(ANCHO_FECHA) << FechaATexto(aMov[i].fecha)
           << ' ' << left
           << setw(ANCHO_DETALLE) << Recortar(aMov[i].detalle, ANCHO_DETALLE)
           << right << setw(ANCHO_CUOTAS) << aMov[i].cuotas
           << setw(ANCHO_CPBTE + 1)
           << (aMov[i].nroCpbte > 0 ? to_string(aMov[i].nroCpbte) : "")
           << fixed << setprecision(2) << setw(ANCHO_IMPORTE)
           << aMov[i].importe;
      i++;
      enPagina++;
      fila++;
    }
    if (i < cant)
      Pausa("Oprima una tecla para ver la pagina siguiente...");
  } while (i < cant);

  MnsgBox(COL_IZQUIERDA, fila, AZUL_CLARO, Separador(ANCHO_TABLA_TC, '-'));
  _gotoxy(COL_IZQUIERDA, fila + 1);
  _textcolor(BLANCO);
  cout << left << setw(ANCHO_TABLA_TC - ANCHO_IMPORTE)
       << "                                     Total"
       << fixed << setprecision(2) << right << setw(ANCHO_IMPORTE) << total;
  MnsgBox(COL_IZQUIERDA, fila + 2, CIAN_CLARO,
          "Cantidad de consumos: " + to_string(cant));
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver...");
} // MostrarMovimientosTC

//------------------------------------------------------------------------------
//  Consulta de una componente por ACCESO ALEATORIO
//  Se pide el numero de registro y se lo alcanza directamente calculando el
//  desplazamiento, sin leer las componentes anteriores.
//------------------------------------------------------------------------------
void ConsultarRegistroCA(const sDatosPers &datos, short nroUsuario) {
  sMovCA mov;
  short  cantRegs;
  short  nroReg;

  EncabezadoUsuario("CONSULTA DIRECTA DE UN MOVIMIENTO (ACCESO ALEATORIO)",
                    datos.apeNom, nroUsuario);
  cantRegs = CantidadRegistros(ARCH_MOV_CA, LARGO_REG_CA);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
          "El archivo " + string(ARCH_MOV_CA) + " tiene " +
          to_string(cantRegs) + " componentes (0 a " +
          to_string(cantRegs - 1) + ").");
  if (cantRegs == 0) {
    MensajeError("El archivo de movimientos esta vacio.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  nroReg = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 2,
                             "Nro. de componente ...: ", 0, cantRegs - 1);
  if (not LeerRegistroCA(nroReg, mov)) {
    MensajeError("No se pudo leer la componente solicitada.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO,
          "Desplazamiento en el archivo: " +
          to_string((long)nroReg * LARGO_REG_CA) + " bytes");
  Marco(COL_IZQUIERDA, FILA_CUERPO + 5, COL_IZQUIERDA + 52, FILA_CUERPO + 12,
        AZUL_CLARO);
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 6, BLANCO, "Fecha .......: ");
  cout << FechaATextoCero(mov.fecha);
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 7, BLANCO, "Tipo mov. ...: ");
  cout << mov.tipoMov << (mov.tipoMov == DEBE ? "  (acredita)" : "  (debita)");
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 8, BLANCO, "Detalle .....: ");
  cout << mov.detalle;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 9, BLANCO, "Importe .....: ");
  cout << fixed << setprecision(2) << mov.importe;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 10, BLANCO, "Nro. usuario : ");
  cout << mov.nroUsuario;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 11, BLANCO, "Pertenece a .: ");
  cout << (mov.nroUsuario == nroUsuario ? "usted" : "otro usuario");
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver...");
} // ConsultarRegistroCA

//------------------------------------------------------------------------------
//  Actualizacion de una componente del archivo
//  Se lee la componente por acceso aleatorio, se corrige el detalle y se la
//  vuelve a grabar EN SU MISMO LUGAR con seekp, sin alterar las demas.
//------------------------------------------------------------------------------
void ActualizarDetalleMovCA(const sDatosPers &datos, short nroUsuario) {
  sMovCA mov;
  str30  nuevoDetalle;
  short  cantRegs;
  short  nroReg;

  EncabezadoUsuario("CORREGIR EL DETALLE DE UN MOVIMIENTO", datos.apeNom,
                    nroUsuario);
  cantRegs = CantidadRegistros(ARCH_MOV_CA, LARGO_REG_CA);
  if (cantRegs == 0) {
    MensajeError("El archivo de movimientos esta vacio.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
          "Componentes disponibles: 0 a " + to_string(cantRegs - 1) + ".");
  nroReg = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 2,
                             "Nro. de componente ...: ", 0, cantRegs - 1);
  if (not LeerRegistroCA(nroReg, mov)) {
    MensajeError("No se pudo leer la componente solicitada.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (mov.nroUsuario != nroUsuario) {
    MensajeError("Esa componente pertenece a otro usuario. Operacion negada.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO, "Fecha .......: ");
  cout << FechaATextoCero(mov.fecha);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, BLANCO, "Importe .....: ");
  cout << fixed << setprecision(2) << mov.importe;
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, BLANCO, "Detalle actual: ");
  _textcolor(AMARILLO);
  cout << mov.detalle;
  _textcolor(GRIS_CLARO);
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 8, "Detalle nuevo ........: ",
             nuevoDetalle, ANCHO_DETALLE);
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 10,
                   "Confirma la actualizacion?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  CopiarCadena(mov.detalle, nuevoDetalle, ANCHO_DETALLE);
  if (ActualizarRegistroCA(nroReg, mov))
    MensajeOk("Componente " + to_string(nroReg) + " actualizada en el archivo.");
  else
    MensajeError("No se pudo actualizar el archivo.");
  Pausa("Oprima una tecla para volver...");
} // ActualizarDetalleMovCA

//------------------------------------------------------------------------------
//  Menu_Movimientos - menu de nivel 2 de las consultas de movimientos
//------------------------------------------------------------------------------
void Menu_Movimientos(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short nroUsuario = usuario + 1;
  short opcion;

  do {
    EncabezadoUsuario("MOVIMIENTOS", datos.apeNom, nroUsuario);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO,     BLANCO,
            "1. Caja de Ahorro en pesos (por fecha, con saldo acumulado)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, BLANCO,
            "2. Tarjeta de Debito (por fecha)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO,
            "3. Tarjeta de Debito (por importe, de mayor a menor)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, BLANCO,
            "4. Tarjeta de Credito (por fecha)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO,
            "5. Consultar una componente de CA por su nro. (acceso aleatorio)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, BLANCO,
            "6. Corregir el detalle de un movimiento de CA");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 7, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 9, 0, 6);
    switch (opcion) {
      case 1 : MostrarMovimientosCA(datos, nroUsuario);         break;
      case 2 : MostrarMovimientosTD(datos, nroUsuario, false);  break;
      case 3 : MostrarMovimientosTD(datos, nroUsuario, true);   break;
      case 4 : MostrarMovimientosTC(datos, nroUsuario);         break;
      case 5 : ConsultarRegistroCA(datos, nroUsuario);          break;
      case 6 : ActualizarDetalleMovCA(datos, nroUsuario);       break;
    }
  } while (opcion != 0);
} // Menu_Movimientos

//------------------------------------------------------------------------------
//  Menu_Tarjetas - datos y resumen de las tarjetas de credito y de debito
//------------------------------------------------------------------------------
void Menu_Tarjetas(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short  nroUsuario = usuario + 1;
  sMovTD aMovTD[MAX_MOV_TD];
  sMovTC aMovTC[MAX_MOV_TC];
  short  cantTD, cantTC;
  double  totalTD, totalTC;
  short  opcion;

  do {
    cantTD  = LeerMovimientosTD(aMovTD, nroUsuario);
    cantTC  = LeerMovimientosTC(aMovTC, nroUsuario);
    totalTD = TotalizarMovTD(aMovTD, cantTD);
    totalTC = TotalizarMovTC(aMovTC, cantTC);

    EncabezadoUsuario("MIS TARJETAS", datos.apeNom, nroUsuario);
    Marco(COL_IZQUIERDA, FILA_CUERPO, COL_IZQUIERDA + 66, FILA_CUERPO + 6,
          AZUL_CLARO);
    MnsgBox(COL_IZQUIERDA + 2, FILA_CUERPO, AMARILLO, " TARJETA DE CREDITO ");
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 1, BLANCO, "Numero .........: ");
    cout << datos.tarjetaTC.nroTarjeta;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 2, BLANCO, "Vencimiento ....: ");
    cout << setw(2) << setfill('0') << datos.tarjetaTC.vence.mes
         << setfill(' ') << '/' << datos.tarjetaTC.vence.anio;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 3, BLANCO, "Limite de compra: ");
    cout << fixed << setprecision(2) << right << setw(14)
         << datos.tarjetaTC.limite;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 4, BLANCO, "Consumido ......: ");
    cout << fixed << setprecision(2) << right << setw(14) << totalTC;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 5, BLANCO, "Disponible .....: ");
    _textcolor(VERDE_CLARO);
    cout << fixed << setprecision(2) << right << setw(14)
         << datos.tarjetaTC.limite - totalTC;

    Marco(COL_IZQUIERDA, FILA_CUERPO + 7, COL_IZQUIERDA + 66, FILA_CUERPO + 12,
          AZUL_CLARO);
    MnsgBox(COL_IZQUIERDA + 2, FILA_CUERPO + 7, AMARILLO,
            " TARJETA DE DEBITO ");
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 8, BLANCO, "Numero .........: ");
    cout << datos.tarjetaTD.nroTarjeta;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 9, BLANCO, "Vencimiento ....: ");
    cout << setw(2) << setfill('0') << datos.tarjetaTD.vence.mes
         << setfill(' ') << '/' << datos.tarjetaTD.vence.anio;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 10, BLANCO,
            "Cta. asociada ..: ");
    cout << datos.aCuentas[CTA_PESOS].nroCta;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 11, BLANCO,
            "Consumido ......: ");
    cout << fixed << setprecision(2) << right << setw(14) << totalTD;

    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 13, GRIS_OSCURO,
            "Componentes en los archivos:  " + string(ARCH_MOV_TC) + " = " +
            to_string(CantidadRegistros(ARCH_MOV_TC, LARGO_REG_TC)) + "   " +
            string(ARCH_MOV_TD) + " = " +
            to_string(CantidadRegistros(ARCH_MOV_TD, LARGO_REG_TD)));
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 14, BLANCO,
            "1. Ver el resumen de la Tarjeta de Credito (" +
            to_string(cantTC) + " consumos)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 15, BLANCO,
            "2. Ver el resumen de la Tarjeta de Debito  (" +
            to_string(cantTD) + " consumos)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 17, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 19, 0, 2);
    switch (opcion) {
      case 1 : MostrarMovimientosTC(datos, nroUsuario);        break;
      case 2 : MostrarMovimientosTD(datos, nroUsuario, false); break;
    }
  } while (opcion != 0);
} // Menu_Tarjetas

//==============================================================================
//  MODULOS DE NIVEL 2 - OPERACIONES QUE MUEVEN LA CAJA DE AHORRO
//==============================================================================

//------------------------------------------------------------------------------
//  Cuerpo comun de una transferencia: valida saldo, pide token, graba la nueva
//  componente en MovimientosCA.Txt y actualiza el saldo en memoria.
//------------------------------------------------------------------------------
void EjecutarDebitoEnCA(sDatosPers &datos, short nroUsuario,
                        const char detalle[], double importe, short fila,
                        bool pideToken) {
  sFecha hoy = FechaDeHoy();

  if (not HaySaldoSuficiente(datos, importe)) {
    MensajeError("Saldo insuficiente. Disponible: $ " +
                 to_string((long)SaldoEnPesos(datos)) + ".");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, fila, BLANCO, "Importe de la operacion .: ");
  _textcolor(AMARILLO);
  cout << fixed << setprecision(2) << importe;
  MnsgBox(COL_IZQUIERDA, fila + 1, BLANCO, "Concepto ................: ");
  _textcolor(AMARILLO);
  cout << detalle;
  _textcolor(GRIS_CLARO);
  if (not LeerSiNo(COL_IZQUIERDA, fila + 3, "Confirma la operacion?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (pideToken and not ValidarToken(COL_IZQUIERDA, fila + 5)) {
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (RegistrarMovCA(datos, nroUsuario, HABER, detalle, importe, hoy)) {
    MensajeOk("Operacion realizada. Nuevo saldo: $ " +
              to_string((long)SaldoEnPesos(datos)) + ".");
    MostrarSaldoDisponible(datos, fila + 8);
  }
  else
    MensajeError("No se pudo grabar el movimiento en " +
                 string(ARCH_MOV_CA) + ".");
  Pausa("Oprima una tecla para volver...");
} // EjecutarDebitoEnCA

//------------------------------------------------------------------------------
//  Transferencia a una cuenta agendada, buscada por alias con BUSQUEDA BINARIA
//------------------------------------------------------------------------------
void TransferirAAgendada(sDatosPers &datos, short nroUsuario) {
  str20 alias;
  str30 detalle;
  short posicion;
  double importe;
  short i;

  EncabezadoUsuario("TRANSFERIR A UNA CUENTA AGENDADA", datos.apeNom,
                    nroUsuario);
  if (datos.cantAgenda == 0) {
    MensajeError("No tiene cuentas agendadas. Agende una desde Agenda CBU.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO,
          "Alias               Titular");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, AZUL_CLARO, Separador(52, '-'));
  for (i = 0; i < datos.cantAgenda; i++) {
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 4 + i);
    _textcolor(GRIS_CLARO);
    cout << left << setw(20) << datos.aAgenda[i].alias
         << left << setw(30) << datos.aAgenda[i].titular;
  }
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 5 + datos.cantAgenda,
             "Alias de destino ........: ", alias, 20);
  posicion = BuscarAliasEnAgenda(datos.aAgenda, datos.cantAgenda, alias);
  if (posicion == NO_ENCONTRADO) {
    MensajeError("El alias " + string(alias) + " NO EXISTE en su agenda.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 7 + datos.cantAgenda, CIAN_CLARO,
          "Destino: " + string(datos.aAgenda[posicion].titular) +
          "  CBU " + string(datos.aAgenda[posicion].cbu));
  importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 8 + datos.cantAgenda,
                        "Importe a transferir ....: ",
                        IMPORTE_MINIMO, IMPORTE_MAXIMO);
  CopiarCadena(detalle, "Transf. a " + string(datos.aAgenda[posicion].titular),
               ANCHO_DETALLE);
  EjecutarDebitoEnCA(datos, nroUsuario, detalle, importe,
                     FILA_CUERPO + 10 + datos.cantAgenda, true);
} // TransferirAAgendada

//------------------------------------------------------------------------------
//  Transferencia a un CBU ingresado a mano
//------------------------------------------------------------------------------
void TransferirACbu(sDatosPers &datos, short nroUsuario) {
  str25 cbu;
  str30 titular;
  str30 detalle;
  double importe;
  bool  valido;

  EncabezadoUsuario("TRANSFERIR A UN CBU", datos.apeNom, nroUsuario);
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  do {
    LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 2,
               "CBU de destino (22 dig.).: ", cbu, 25);
    valido = CbuValido(cbu);
    if (not valido)
      MensajeError("El CBU debe tener exactamente 22 digitos numericos.");
  } while (not valido);
  if (strcmp(cbu, datos.cbu) == 0) {
    MensajeError("No puede transferirse a su propia cuenta.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 3, "Titular de destino ......: ",
             titular, 30);
  importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 4,
                        "Importe a transferir ....: ",
                        IMPORTE_MINIMO, IMPORTE_MAXIMO);
  CopiarCadena(detalle, "Transf. a " + string(titular), ANCHO_DETALLE);
  EjecutarDebitoEnCA(datos, nroUsuario, detalle, importe, FILA_CUERPO + 6,
                     true);
} // TransferirACbu

//------------------------------------------------------------------------------
//  Transferencia a un numero de celular
//------------------------------------------------------------------------------
void TransferirACelular(sDatosPers &datos, short nroUsuario) {
  str15 celular;
  str30 detalle;
  double importe;
  bool  valido;

  EncabezadoUsuario("TRANSFERIR A UN CELULAR", datos.apeNom, nroUsuario);
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  do {
    LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 2,
               "Celular destino (10 dig.): ", celular, 15);
    valido = CelularValido(celular);
    if (not valido)
      MensajeError("El celular debe tener exactamente 10 digitos numericos.");
  } while (not valido);
  importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 3,
                        "Importe a transferir ....: ",
                        IMPORTE_MINIMO, IMPORTE_MAXIMO);
  CopiarCadena(detalle, "Transf. a Cel " + string(celular), ANCHO_DETALLE);
  EjecutarDebitoEnCA(datos, nroUsuario, detalle, importe, FILA_CUERPO + 5,
                     true);
} // TransferirACelular

//------------------------------------------------------------------------------
//  Menu_Transferir - menu de nivel 2 de las transferencias
//------------------------------------------------------------------------------
void Menu_Transferir(sDatosPers aDatosPers[], short usuario) {
  short nroUsuario = usuario + 1;
  short opcion;

  do {
    EncabezadoUsuario("TRANSFERIR DINERO", aDatosPers[usuario].apeNom,
                      nroUsuario);
    MostrarSaldoDisponible(aDatosPers[usuario], FILA_CUERPO);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO,
            "1. A una cuenta agendada (por alias)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, BLANCO,
            "2. A un CBU");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO,
            "3. A un numero de celular");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 8, 0, 3);
    switch (opcion) {
      case 1 : TransferirAAgendada(aDatosPers[usuario], nroUsuario); break;
      case 2 : TransferirACbu(aDatosPers[usuario], nroUsuario);      break;
      case 3 : TransferirACelular(aDatosPers[usuario], nroUsuario);  break;
    }
  } while (opcion != 0);
} // Menu_Transferir

//------------------------------------------------------------------------------
//  Menu_Deposito - acredita un deposito en la caja de ahorro
//------------------------------------------------------------------------------
void Menu_Deposito(sDatosPers aDatosPers[], short usuario) {
  sDatosPers &datos = aDatosPers[usuario];
  short  nroUsuario = usuario + 1;
  sFecha fecha;
  str30  descripcion;
  double  importe;

  EncabezadoUsuario("DEPOSITO EN CAJA DE AHORRO", datos.apeNom, nroUsuario);
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  fecha = LeerFecha(COL_IZQUIERDA, FILA_CUERPO + 2, "Fecha del deposito",
                    FechaDeHoy());
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 7, "Descripcion .............: ",
             descripcion, ANCHO_DETALLE);
  importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 8,
                        "Importe a depositar .....: ",
                        IMPORTE_MINIMO, IMPORTE_MAXIMO);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 10, BLANCO, "Fecha ...: ");
  _textcolor(AMARILLO);
  cout << FechaATextoCero(fecha);
  _textcolor(GRIS_CLARO);
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 12, "Confirma el deposito?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (RegistrarMovCA(datos, nroUsuario, DEBE, descripcion, importe, fecha)) {
    MensajeOk("Deposito acreditado. Nuevo saldo: $ " +
              to_string((long)SaldoEnPesos(datos)) + ".");
    MostrarSaldoDisponible(datos, FILA_CUERPO + 14);
  }
  else
    MensajeError("No se pudo grabar el movimiento en " +
                 string(ARCH_MOV_CA) + ".");
  Pausa("Oprima una tecla para volver...");
} // Menu_Deposito

//------------------------------------------------------------------------------
//  Menu_Compras
//  Registra una compra con tarjeta de debito o de credito. La compra se graba
//  como nueva componente en el archivo de la tarjeta elegida Y en el archivo
//  de la caja de ahorro, tal como pide el enunciado.
//------------------------------------------------------------------------------
void Menu_Compras(sDatosPers aDatosPers[], short usuario) {
  sDatosPers &datos = aDatosPers[usuario];
  short  nroUsuario = usuario + 1;
  sFecha fecha;
  str30  comercio, descripcion, detalleCA;
  str5   cuotas;
  short  modo, cantCuotas;
  double  importe;
  sMovTD movTD;
  sMovTC movTC;
  bool   grabado;

  EncabezadoUsuario("REGISTRAR UNA COMPRA", datos.apeNom, nroUsuario);
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  fecha = LeerFecha(COL_IZQUIERDA, FILA_CUERPO + 2, "Fecha de la compra",
                    FechaDeHoy());
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 7, "Comercio ................: ",
             comercio, ANCHO_DETALLE);
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 8, "Descripcion .............: ",
             descripcion, ANCHO_DETALLE - 3);
  importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 9,
                        "Importe de la compra ....: ",
                        IMPORTE_MINIMO, IMPORTE_MAXIMO);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 11, BLANCO,
          "Modo de pago:  1. Tarjeta de Debito (TD)   "
          "2. Tarjeta de Credito (TC)");
  modo = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 12,
                           "Modo de pago ............: ", 1, 2);

  if (modo == 1) {                                     // tarjeta de debito
    CopiarCadena(detalleCA, "TD " + string(descripcion), ANCHO_DETALLE);
    if (not HaySaldoSuficiente(datos, importe)) {
      MensajeError("Saldo insuficiente para pagar con tarjeta de debito.");
      Pausa("Oprima una tecla para volver...");
      return;
    }
    if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 14, "Confirma la compra?")) {
      MensajeInfo("Operacion cancelada por el usuario.");
      Pausa("Oprima una tecla para volver...");
      return;
    }
    movTD.fecha = fecha;
    CopiarCadena(movTD.detalle, comercio, ANCHO_DETALLE);
    movTD.importe    = importe;
    movTD.nroUsuario = nroUsuario;
    grabado = AgregarMovTD(movTD) and
              RegistrarMovCA(datos, nroUsuario, HABER, detalleCA, importe,
                             fecha);
    if (grabado) {
      MensajeOk("Compra registrada en " + string(ARCH_MOV_TD) + " y en " +
                string(ARCH_MOV_CA) + ".");
      MostrarSaldoDisponible(datos, FILA_CUERPO + 16);
    }
    else
      MensajeError("No se pudieron grabar los movimientos.");
  }
  else {                                               // tarjeta de credito
    cantCuotas = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 13,
                                   "Cantidad de cuotas ......: ", 1, 18);
    CopiarCadena(cuotas, "1/" + to_string(cantCuotas), 5);
    CopiarCadena(detalleCA, "TC " + string(descripcion), ANCHO_DETALLE);
    if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 15, "Confirma la compra?")) {
      MensajeInfo("Operacion cancelada por el usuario.");
      Pausa("Oprima una tecla para volver...");
      return;
    }
    movTC.fecha = fecha;
    CopiarCadena(movTC.detalle, comercio, ANCHO_DETALLE);
    CopiarCadena(movTC.cuotas, cuotas, 5);
    movTC.nroCpbte   = rand() % 900000 + 100000;
    movTC.importe    = importe;
    movTC.nroUsuario = nroUsuario;
    grabado = AgregarMovTC(movTC) and
              RegistrarMovCA(datos, nroUsuario, HABER, detalleCA, importe,
                             fecha);
    if (grabado) {
      MensajeOk("Compra registrada. Comprobante nro. " +
                to_string(movTC.nroCpbte) + ".");
      MostrarSaldoDisponible(datos, FILA_CUERPO + 17);
    }
    else
      MensajeError("No se pudieron grabar los movimientos.");
  }
  Pausa("Oprima una tecla para volver...");
} // Menu_Compras

//------------------------------------------------------------------------------
//  Menu_Recargar - recarga de celular y de tarjeta SUBE
//------------------------------------------------------------------------------
void Menu_Recargar(sDatosPers aDatosPers[], short usuario) {
  sDatosPers &datos = aDatosPers[usuario];
  short nroUsuario = usuario + 1;
  short opcion;
  str15 celular;
  str20 nroSube;
  str30 detalle;
  double importe;
  bool  valido;

  do {
    EncabezadoUsuario("RECARGAS", datos.apeNom, nroUsuario);
    MostrarSaldoDisponible(datos, FILA_CUERPO);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO, "1. Recargar Celular");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, BLANCO, "2. Recargar SUBE");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 7, 0, 2);

    if (opcion == 1) {
      EncabezadoUsuario("RECARGA DE CELULAR", datos.apeNom, nroUsuario);
      MostrarSaldoDisponible(datos, FILA_CUERPO);
      MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, GRIS_OSCURO,
              "Su celular registrado es " + string(datos.celular));
      do {
        LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 3,
                   "Celular a recargar ......: ", celular, 15);
        valido = CelularValido(celular);
        if (not valido)
          MensajeError("El celular debe tener exactamente 10 digitos.");
      } while (not valido);
      importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 4,
                            "Importe de la recarga ...: ",
                            RECARGA_MINIMA, IMPORTE_MAXIMO);
      CopiarCadena(detalle, "Recarga Celular " + string(celular),
                   ANCHO_DETALLE);
      EjecutarDebitoEnCA(datos, nroUsuario, detalle, importe, FILA_CUERPO + 6,
                         false);
    }
    else if (opcion == 2) {
      EncabezadoUsuario("RECARGA DE TARJETA SUBE", datos.apeNom, nroUsuario);
      MostrarSaldoDisponible(datos, FILA_CUERPO);
      do {
        LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 2,
                   "Nro. de tarjeta SUBE ....: ", nroSube, 20);
        valido = LargoCadena(nroSube) == 16 and SoloDigitos(nroSube);
        if (not valido)
          MensajeError("La tarjeta SUBE tiene 16 digitos numericos.");
      } while (not valido);
      importe = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 3,
                            "Importe de la recarga ...: ",
                            RECARGA_MINIMA, IMPORTE_MAXIMO);
      CopiarCadena(detalle, "Recarga SUBE ..." +
                   string(nroSube).substr(12, 4), ANCHO_DETALLE);
      EjecutarDebitoEnCA(datos, nroUsuario, detalle, importe, FILA_CUERPO + 5,
                         false);
    }
  } while (opcion != 0);
} // Menu_Recargar

//------------------------------------------------------------------------------
//  Menu_CompraVentaDolares
//  Compra: debita pesos de la CA en $ y acredita dolares en la CA en u$s.
//  Venta : debita dolares de la CA en u$s y acredita pesos en la CA en $.
//------------------------------------------------------------------------------
void Menu_CompraVentaDolares(sDatosPers aDatosPers[], short usuario) {
  sDatosPers &datos = aDatosPers[usuario];
  short  nroUsuario = usuario + 1;
  short  opcion;
  double  montoDolares, montoPesos;
  sFecha hoy;

  do {
    hoy = FechaDeHoy();
    EncabezadoUsuario("COMPRA Y VENTA DE DOLARES", datos.apeNom, nroUsuario);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
            "Cotizacion del dia    Compra $ " +
            to_string((long)DOLAR_COMPRA) + ",00      Venta $ " +
            to_string((long)DOLAR_VENTA) + ",00");
    MostrarSaldoDisponible(datos, FILA_CUERPO + 2);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, CIAN_CLARO,
            "Saldo disponible en u$s ...: ");
    _textcolor(BLANCO);
    cout << fixed << setprecision(2) << right << setw(14)
         << datos.aCuentas[CTA_DOLARES].saldo;
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, BLANCO,
            "1. Comprar dolares (paga con la caja de ahorro en pesos)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, BLANCO,
            "2. Vender dolares  (acredita en la caja de ahorro en pesos)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 8, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 10, 0, 2);

    if (opcion == 1) {
      montoDolares = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 12,
                                 "Dolares a comprar u$s ...: ",
                                 1.00, 100000.00);
      montoPesos = montoDolares * DOLAR_COMPRA;
      MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 13, BLANCO,
              "Costo total en pesos ....: ");
      _textcolor(AMARILLO);
      cout << fixed << setprecision(2) << montoPesos;
      _textcolor(GRIS_CLARO);
      if (montoPesos > IMPORTE_MAXIMO)
        MensajeError("El importe supera el maximo admitido por el archivo.");
      else if (not HaySaldoSuficiente(datos, montoPesos))
        MensajeError("Saldo en pesos insuficiente para esta compra.");
      else if (LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 15,
                        "Confirma la compra de dolares?")) {
        if (ValidarToken(COL_IZQUIERDA, FILA_CUERPO + 17) and
            RegistrarMovCA(datos, nroUsuario, HABER, "Compra de dolares",
                           montoPesos, hoy)) {
          datos.aCuentas[CTA_DOLARES].saldo += montoDolares;
          MensajeOk("Compra realizada. Nuevo saldo en u$s: " +
                    to_string((long)datos.aCuentas[CTA_DOLARES].saldo) + ".");
        }
      }
      else
        MensajeInfo("Operacion cancelada por el usuario.");
      Pausa("Oprima una tecla para volver...");
    }
    else if (opcion == 2) {
      if (datos.aCuentas[CTA_DOLARES].saldo < 1.00) {
        MensajeError("No tiene dolares disponibles para vender.");
        Pausa("Oprima una tecla para volver...");
      }
      else {
        montoDolares = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 12,
                                   "Dolares a vender u$s ....: ",
                                   1.00, datos.aCuentas[CTA_DOLARES].saldo);
        montoPesos = montoDolares * DOLAR_VENTA;
        MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 13, BLANCO,
                "A acreditar en pesos ....: ");
        _textcolor(AMARILLO);
        cout << fixed << setprecision(2) << montoPesos;
        _textcolor(GRIS_CLARO);
        if (montoPesos > IMPORTE_MAXIMO)
          MensajeError("El importe supera el maximo admitido por el archivo.");
        else if (LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 15,
                          "Confirma la venta de dolares?")) {
          if (ValidarToken(COL_IZQUIERDA, FILA_CUERPO + 17) and
              RegistrarMovCA(datos, nroUsuario, DEBE, "Venta de dolares",
                             montoPesos, hoy)) {
            datos.aCuentas[CTA_DOLARES].saldo -= montoDolares;
            MensajeOk("Venta realizada. Nuevo saldo: $ " +
                      to_string((long)SaldoEnPesos(datos)) + ".");
          }
        }
        else
          MensajeInfo("Operacion cancelada por el usuario.");
        Pausa("Oprima una tecla para volver...");
      }
    }
  } while (opcion != 0);
} // Menu_CompraVentaDolares

//==============================================================================
//  MODULOS DE NIVEL 2 - INVERSIONES
//==============================================================================

//--- Interes simple de una imposicion a plazo ---------------------------------
double CalcularInteres(double capital, short dias, double tna) {
  return capital * tna * dias / DIAS_ANIO;
} // CalcularInteres

//------------------------------------------------------------------------------
//  Menu_SimulacionPF
//  Simula un plazo fijo. NO mueve la caja de ahorro ni graba en los archivos.
//------------------------------------------------------------------------------
void Menu_SimulacionPF(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  double  capital, tna, interes, total;
  short  dias;
  sFecha hoy, vencimiento;
  bool   otra;

  do {
    EncabezadoUsuario("SIMULACION DE PLAZO FIJO", datos.apeNom, usuario + 1);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
            "Tasa nominal anual que ofrece el banco: " +
            to_string((long)(TNA_PLAZO_FIJO * 100)) + ",00 %");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, GRIS_OSCURO,
            "La simulacion no genera movimientos ni afecta su saldo.");
    capital = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 3,
                          "Capital a invertir $ ....: ",
                          1000.00, IMPORTE_MAXIMO);
    dias    = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 4,
                                "Plazo en dias ...........: ",
                                PLAZO_MINIMO, PLAZO_MAXIMO);
    tna     = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 5,
                          "Tasa nominal anual en % .: ", 1.00, 200.00) / 100;
    interes = CalcularInteres(capital, dias, tna);
    total   = capital + interes;
    hoy         = FechaDeHoy();
    vencimiento = SumarDias(hoy, dias);

    Marco(COL_IZQUIERDA, FILA_CUERPO + 7, COL_IZQUIERDA + 52, FILA_CUERPO + 14,
          AMARILLO);
    MnsgBox(COL_IZQUIERDA + 2, FILA_CUERPO + 7, AMARILLO,
            " RESULTADO DE LA SIMULACION ");
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 8, BLANCO,
            "Capital invertido ....: ");
    cout << fixed << setprecision(2) << right << setw(16) << capital;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 9, BLANCO,
            "Plazo ................: ");
    cout << right << setw(16) << (to_string(dias) + " dias");
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 10, BLANCO,
            "Tasa nominal anual ...: ");
    cout << fixed << setprecision(2) << right << setw(15) << tna * 100 << "%";
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 11, BLANCO,
            "Fecha de vencimiento .: ");
    cout << right << setw(16) << FechaATextoCero(vencimiento);
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 12, BLANCO,
            "Intereses a cobrar ...: ");
    _textcolor(VERDE_CLARO);
    cout << fixed << setprecision(2) << right << setw(16) << interes;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 13, BLANCO,
            "TOTAL al vencimiento .: ");
    _textcolor(VERDE_CLARO);
    cout << fixed << setprecision(2) << right << setw(16) << total;
    _textcolor(GRIS_CLARO);
    otra = LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 16, "Desea simular otro?");
  } while (otra);
} // Menu_SimulacionPF

//------------------------------------------------------------------------------
//  Constitucion de un plazo fijo: debita el capital de la caja de ahorro
//------------------------------------------------------------------------------
void ConstituirPlazoFijo(sDatosPers &datos, short nroUsuario) {
  sPlazoFijo plazo;
  double      capital;
  short      dias;
  sFecha     hoy = FechaDeHoy();

  EncabezadoUsuario("CONSTITUIR UN PLAZO FIJO", datos.apeNom, nroUsuario);
  if (datos.cantPlazos >= MAX_PLAZOS) {
    MensajeError("Alcanzo el maximo de " + to_string(MAX_PLAZOS) +
                 " plazos fijos vigentes.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, CIAN_CLARO,
          "Tasa nominal anual vigente: " +
          to_string((long)(TNA_PLAZO_FIJO * 100)) + ",00 %");
  capital = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 3,
                        "Capital a invertir $ ....: ",
                        1000.00, IMPORTE_MAXIMO);
  if (not HaySaldoSuficiente(datos, capital)) {
    MensajeError("Saldo insuficiente en la caja de ahorro.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  dias = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 4,
                           "Plazo en dias ...........: ",
                           PLAZO_MINIMO, PLAZO_MAXIMO);
  plazo.fAlta     = hoy;
  plazo.fVto      = SumarDias(hoy, dias);
  plazo.capital   = capital;
  plazo.plazoDias = dias;
  plazo.tasa      = TNA_PLAZO_FIJO;
  plazo.interes   = CalcularInteres(capital, dias, TNA_PLAZO_FIJO);
  plazo.total     = capital + plazo.interes;

  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, BLANCO, "Vence el ................: ");
  _textcolor(AMARILLO);
  cout << FechaATextoCero(plazo.fVto);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 7, BLANCO, "Intereses ...............: ");
  _textcolor(AMARILLO);
  cout << fixed << setprecision(2) << plazo.interes;
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 8, BLANCO, "Total al vencimiento ....: ");
  _textcolor(VERDE_CLARO);
  cout << fixed << setprecision(2) << plazo.total;
  _textcolor(GRIS_CLARO);
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 10,
                   "Confirma la constitucion del plazo fijo?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (not ValidarToken(COL_IZQUIERDA, FILA_CUERPO + 12)) {
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (RegistrarMovCA(datos, nroUsuario, HABER, "Constitucion Plazo Fijo",
                     capital, hoy)) {
    datos.aPlazos[datos.cantPlazos] = plazo;
    datos.cantPlazos++;
    MensajeOk("Plazo fijo constituido. Nuevo saldo: $ " +
              to_string((long)SaldoEnPesos(datos)) + ".");
  }
  else
    MensajeError("No se pudo grabar el movimiento en " +
                 string(ARCH_MOV_CA) + ".");
  Pausa("Oprima una tecla para volver...");
} // ConstituirPlazoFijo

//------------------------------------------------------------------------------
//  Suscripcion a un fondo comun de inversion
//------------------------------------------------------------------------------
void SuscribirFondoInversion(sDatosPers &datos, short nroUsuario) {
  double  capital;
  sFecha hoy = FechaDeHoy();

  EncabezadoUsuario("FONDO COMUN DE INVERSION", datos.apeNom, nroUsuario);
  MostrarSaldoDisponible(datos, FILA_CUERPO);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, CIAN_CLARO,
          "Rendimiento estimado: " +
          to_string((long)(TNA_FONDO_INV * 100)) + ",00 % nominal anual");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, GRIS_OSCURO,
          "El dinero queda disponible para rescatar en cualquier momento.");
  capital = LeerImporte(COL_IZQUIERDA, FILA_CUERPO + 4,
                        "Capital a invertir $ ....: ",
                        1000.00, IMPORTE_MAXIMO);
  if (not HaySaldoSuficiente(datos, capital)) {
    MensajeError("Saldo insuficiente en la caja de ahorro.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, BLANCO,
          "Rendimiento estimado a 30 dias: ");
  _textcolor(VERDE_CLARO);
  cout << fixed << setprecision(2)
       << CalcularInteres(capital, 30, TNA_FONDO_INV);
  _textcolor(GRIS_CLARO);
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 8, "Confirma la suscripcion?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  if (RegistrarMovCA(datos, nroUsuario, HABER, "Suscripcion Fondo Comun",
                     capital, hoy))
    MensajeOk("Suscripcion realizada. Nuevo saldo: $ " +
              to_string((long)SaldoEnPesos(datos)) + ".");
  else
    MensajeError("No se pudo grabar el movimiento en " +
                 string(ARCH_MOV_CA) + ".");
  Pausa("Oprima una tecla para volver...");
} // SuscribirFondoInversion

//------------------------------------------------------------------------------
//  Listado de los plazos fijos vigentes del usuario
//------------------------------------------------------------------------------
void MostrarPlazosFijos(const sDatosPers &datos, short nroUsuario) {
  short i;
  double totalCapital = 0;
  double totalInteres = 0;

  EncabezadoUsuario("MIS PLAZOS FIJOS VIGENTES", datos.apeNom, nroUsuario);
  if (datos.cantPlazos == 0) {
    MensajeInfo("No tiene plazos fijos constituidos en esta sesion.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, BLANCO,
          "Alta       Vencimiento  Dias   Tasa        Capital"
          "     Intereses");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, AZUL_CLARO, Separador(70, '-'));
  for (i = 0; i < datos.cantPlazos; i++) {
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 2 + i);
    _textcolor(GRIS_CLARO);
    cout << left  << setw(11) << FechaATextoCero(datos.aPlazos[i].fAlta)
         << left  << setw(13) << FechaATextoCero(datos.aPlazos[i].fVto)
         << right << setw(4)  << datos.aPlazos[i].plazoDias
         << fixed << setprecision(2)
         << right << setw(7)  << datos.aPlazos[i].tasa * 100 << "%"
         << right << setw(15) << datos.aPlazos[i].capital
         << right << setw(14) << datos.aPlazos[i].interes;
    totalCapital += datos.aPlazos[i].capital;
    totalInteres += datos.aPlazos[i].interes;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2 + datos.cantPlazos, AZUL_CLARO,
          Separador(70, '-'));
  _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 3 + datos.cantPlazos);
  _textcolor(BLANCO);
  cout << left  << setw(35) << "Totales"
       << fixed << setprecision(2)
       << right << setw(21) << totalCapital
       << right << setw(14) << totalInteres;
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para volver...");
} // MostrarPlazosFijos

//------------------------------------------------------------------------------
//  Menu_PlazoFijo - menu de nivel 2 de las inversiones
//------------------------------------------------------------------------------
void Menu_PlazoFijo(sDatosPers aDatosPers[], short usuario) {
  short nroUsuario = usuario + 1;
  short opcion;

  do {
    EncabezadoUsuario("INVERSIONES", aDatosPers[usuario].apeNom, nroUsuario);
    MostrarSaldoDisponible(aDatosPers[usuario], FILA_CUERPO);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO,
            "1. Constituir un Plazo Fijo");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, BLANCO,
            "2. Suscribir a un Fondo Comun de Inversion");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO,
            "3. Ver mis Plazos Fijos vigentes");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 8, 0, 3);
    switch (opcion) {
      case 1 : ConstituirPlazoFijo(aDatosPers[usuario], nroUsuario);     break;
      case 2 : SuscribirFondoInversion(aDatosPers[usuario], nroUsuario); break;
      case 3 : MostrarPlazosFijos(aDatosPers[usuario], nroUsuario);      break;
    }
  } while (opcion != 0);
} // Menu_PlazoFijo

//==============================================================================
//  MODULOS DE NIVEL 2 - AGENDA, CUENTAS, SEGURIDAD
//==============================================================================

//--- Listado de la agenda ordenada por alias ----------------------------------
void MostrarAgenda(const sDatosPers &datos, short nroUsuario) {
  short i;

  EncabezadoUsuario("CUENTAS AGENDADAS PARA TRANSFERIR", datos.apeNom,
                    nroUsuario);
  if (datos.cantAgenda == 0) {
    MensajeInfo("Su agenda esta vacia.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, BLANCO,
          "Nro  Alias               Titular                        CBU");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, AZUL_CLARO, Separador(72, '-'));
  for (i = 0; i < datos.cantAgenda; i++) {
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 2 + i);
    _textcolor(GRIS_CLARO);
    cout << right << setw(3) << i + 1 << "  "
         << left  << setw(20) << datos.aAgenda[i].alias
         << left  << setw(25) << datos.aAgenda[i].titular
         << datos.aAgenda[i].cbu;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2 + datos.cantAgenda, AZUL_CLARO,
          Separador(72, '-'));
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3 + datos.cantAgenda, CIAN_CLARO,
          "Cantidad de cuentas agendadas: " + to_string(datos.cantAgenda) +
          " de " + to_string(MAX_AGENDA) + ".  Orden: por alias ascendente.");
  Pausa("Oprima una tecla para volver...");
} // MostrarAgenda

//--- Busqueda de un alias por el metodo BINARIO -------------------------------
void BuscarEnAgenda(const sDatosPers &datos, short nroUsuario) {
  str20 alias;
  short posicion;

  EncabezadoUsuario("BUSCAR UNA CUENTA AGENDADA", datos.apeNom, nroUsuario);
  if (datos.cantAgenda == 0) {
    MensajeInfo("Su agenda esta vacia.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, GRIS_OSCURO,
          "La agenda esta ordenada por alias, de modo que la busqueda se");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, GRIS_OSCURO,
          "resuelve por el metodo binario sobre " +
          to_string(datos.cantAgenda) + " componentes.");
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 3, "Alias a buscar ..........: ",
             alias, 20);
  posicion = BuscarAliasEnAgenda(datos.aAgenda, datos.cantAgenda, alias);
  if (posicion == NO_ENCONTRADO)
    MensajeError("El alias " + string(alias) + " NO EXISTE en su agenda.");
  else {
    Marco(COL_IZQUIERDA, FILA_CUERPO + 5, COL_IZQUIERDA + 52, FILA_CUERPO + 10,
          VERDE_CLARO);
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 6, BLANCO, "Alias .......: ");
    cout << datos.aAgenda[posicion].alias;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 7, BLANCO, "Titular .....: ");
    cout << datos.aAgenda[posicion].titular;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 8, BLANCO, "CBU .........: ");
    cout << datos.aAgenda[posicion].cbu;
    MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 9, BLANCO, "Componente ..: ");
    cout << posicion + 1 << " de " << datos.cantAgenda;
    _textcolor(GRIS_CLARO);
    MensajeOk("Cuenta encontrada.");
  }
  Pausa("Oprima una tecla para volver...");
} // BuscarEnAgenda

//--- Alta de una cuenta agendada por INSERCION EN ORDEN -----------------------
void AgendarNuevaCuenta(sDatosPers &datos, short nroUsuario) {
  sAgenda nueva;
  str20   alias;
  str30   titular;
  str25   cbu;
  bool    valido;

  EncabezadoUsuario("AGENDAR UNA NUEVA CUENTA", datos.apeNom, nroUsuario);
  if (datos.cantAgenda >= MAX_AGENDA) {
    MensajeError("Su agenda esta completa (" + to_string(MAX_AGENDA) +
                 " cuentas).");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, GRIS_OSCURO,
          "La cuenta se inserta EN ORDEN, para conservar la agenda ordenada.");
  do {
    LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 2, "Alias ...................: ",
               alias, 20);
    valido = BuscarAliasEnAgenda(datos.aAgenda, datos.cantAgenda, alias) ==
             NO_ENCONTRADO;
    if (not valido)
      MensajeError("Ese alias ya esta agendado. Ingrese otro.");
  } while (not valido);
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 3, "Titular .................: ",
             titular, 30);
  do {
    LeerCadena(COL_IZQUIERDA, FILA_CUERPO + 4,
               "CBU (22 digitos) ........: ", cbu, 25);
    valido = CbuValido(cbu);
    if (not valido)
      MensajeError("El CBU debe tener exactamente 22 digitos numericos.");
    else if (BuscarCbuEnAgenda(datos.aAgenda, datos.cantAgenda, cbu) !=
             NO_ENCONTRADO) {
      valido = false;
      MensajeError("Ese CBU ya figura en su agenda con otro alias.");
    }
  } while (not valido);
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 6, "Confirma el alta?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  CopiarCadena(nueva.alias,   alias,   20);
  CopiarCadena(nueva.titular, titular, 30);
  CopiarCadena(nueva.cbu,     cbu,     25);
  if (InsertarEnOrdenAgenda(datos.aAgenda, datos.cantAgenda, nueva))
    MensajeOk("Cuenta agendada en la posicion " +
              to_string(BuscarAliasEnAgenda(datos.aAgenda, datos.cantAgenda,
                                            alias) + 1) +
              " de " + to_string(datos.cantAgenda) + ".");
  else
    MensajeError("No hay lugar disponible en la agenda.");
  Pausa("Oprima una tecla para volver...");
} // AgendarNuevaCuenta

//--- Baja de una cuenta agendada ----------------------------------------------
void EliminarCuentaAgendada(sDatosPers &datos, short nroUsuario) {
  str20 alias;
  short posicion;

  EncabezadoUsuario("ELIMINAR UNA CUENTA AGENDADA", datos.apeNom, nroUsuario);
  if (datos.cantAgenda == 0) {
    MensajeInfo("Su agenda esta vacia.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  LeerCadena(COL_IZQUIERDA, FILA_CUERPO, "Alias a eliminar ........: ",
             alias, 20);
  posicion = BuscarAliasEnAgenda(datos.aAgenda, datos.cantAgenda, alias);
  if (posicion == NO_ENCONTRADO) {
    MensajeError("El alias " + string(alias) + " NO EXISTE en su agenda.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, CIAN_CLARO,
          "Titular: " + string(datos.aAgenda[posicion].titular) +
          "   CBU: " + string(datos.aAgenda[posicion].cbu));
  if (LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 4, "Confirma la baja?")) {
    EliminarDeAgenda(datos.aAgenda, datos.cantAgenda, posicion);
    MensajeOk("Cuenta eliminada. Quedan " + to_string(datos.cantAgenda) +
              " cuentas agendadas.");
  }
  else
    MensajeInfo("Operacion cancelada por el usuario.");
  Pausa("Oprima una tecla para volver...");
} // EliminarCuentaAgendada

//------------------------------------------------------------------------------
//  Menu_AgendaCBU - menu de nivel 2 de la agenda de cuentas
//------------------------------------------------------------------------------
void Menu_AgendaCBU(sDatosPers aDatosPers[], short usuario) {
  short nroUsuario = usuario + 1;
  short opcion;

  do {
    EncabezadoUsuario("AGENDA DE CUENTAS CBU", aDatosPers[usuario].apeNom,
                      nroUsuario);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
            "Cuentas agendadas: " + to_string(aDatosPers[usuario].cantAgenda) +
            " de " + to_string(MAX_AGENDA));
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO,
            "1. Ver la agenda completa (ordenada por alias)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, BLANCO,
            "2. Buscar una cuenta por alias (busqueda binaria)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO,
            "3. Agendar una nueva cuenta (insercion en orden)");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, BLANCO,
            "4. Eliminar una cuenta agendada");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 7, AMARILLO,
            "0. Volver al menu principal");
    opcion = LeerOpcion(COL_IZQUIERDA, FILA_CUERPO + 9, 0, 4);
    switch (opcion) {
      case 1 : MostrarAgenda(aDatosPers[usuario], nroUsuario);          break;
      case 2 : BuscarEnAgenda(aDatosPers[usuario], nroUsuario);         break;
      case 3 : AgendarNuevaCuenta(aDatosPers[usuario], nroUsuario);     break;
      case 4 : EliminarCuentaAgendada(aDatosPers[usuario], nroUsuario); break;
    }
  } while (opcion != 0);
} // Menu_AgendaCBU

//------------------------------------------------------------------------------
//  Menu_NuevaCuenta
//  Da de alta una cuenta nueva. El numero se arma con la sucursal y un
//  correlativo respecto de las cuentas ya creadas en el sistema.
//------------------------------------------------------------------------------
void Menu_NuevaCuenta(sDatosPers aDatosPers[], short usuario) {
  sDatosPers &datos = aDatosPers[usuario];
  str20 nroCta;
  str5  tipo, moneda;
  short opcTipo, opcMoneda, correlativo;

  EncabezadoUsuario("CREAR UNA CUENTA NUEVA", datos.apeNom, usuario + 1);
  if (datos.cantCuentas >= MAX_CUENTAS) {
    MensajeError("Alcanzo el maximo de " + to_string(MAX_CUENTAS) +
                 " cuentas por usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
          "Actualmente tiene " + to_string(datos.cantCuentas) +
          " cuenta(s) abiertas.");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 2, BLANCO,
          "Tipo de cuenta:  1. Caja de Ahorro (CA)   2. Cuenta Corriente (CC)");
  opcTipo = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 3,
                              "Tipo de cuenta ..........: ", 1, 2);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, BLANCO,
          "Moneda:          1. Pesos $               2. Dolares u$s");
  opcMoneda = (short)LeerEntero(COL_IZQUIERDA, FILA_CUERPO + 6,
                                "Moneda ..................: ", 1, 2);
  CopiarCadena(tipo,   opcTipo   == 1 ? "CA" : "CC", 5);
  CopiarCadena(moneda, opcMoneda == 1 ? "$"  : "u$s", 5);
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 8,
                   "Confirma la apertura de la cuenta?")) {
    MensajeInfo("Operacion cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  correlativo = TotalCuentasDelSistema(aDatosPers) + 1;
  ArmarNroCuentaNueva(correlativo, nroCta);
  CargarCuenta(datos.aCuentas[datos.cantCuentas], nroCta, tipo, moneda, 0.00);
  datos.cantCuentas++;
  Marco(COL_IZQUIERDA, FILA_CUERPO + 10, COL_IZQUIERDA + 52, FILA_CUERPO + 14,
        VERDE_CLARO);
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 11, VERDE_CLARO,
          "Cuenta creada nro. " + string(nroCta));
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 12, BLANCO,
          "Tipo: " + string(tipo) + "   Moneda: " + string(moneda) +
          "   Saldo inicial: 0,00");
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 13, BLANCO,
          "Estado: Activa");
  _textcolor(GRIS_CLARO);
  MensajeOk("La cuenta ya figura en Mis Cuentas.");
  Pausa("Oprima una tecla para volver...");
} // Menu_NuevaCuenta

//------------------------------------------------------------------------------
//  Menu_GenToken - genera un token de seguridad de seis digitos al azar
//------------------------------------------------------------------------------
void Menu_GenToken(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  long  token;
  short segundos;
  bool  otro;

  do {
    token = GenerarToken();
    EncabezadoUsuario("GENERAR TOKEN DE SEGURIDAD", datos.apeNom, usuario + 1);
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, GRIS_OSCURO,
            "El token se genera al azar y se solicita en las operaciones");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, GRIS_OSCURO,
            "que requieren una confirmacion adicional por seguridad.");
    Marco(COL_IZQUIERDA, FILA_CUERPO + 3, COL_IZQUIERDA + 40, FILA_CUERPO + 7,
          AMARILLO);
    MnsgBox(COL_IZQUIERDA + 2, FILA_CUERPO + 3, AMARILLO, " TOKEN ");
    MnsgBox(COL_IZQUIERDA + 14, FILA_CUERPO + 5, AMARILLO, to_string(token));
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 9, BLANCO, "Enviado al celular ......: ");
    cout << datos.celular;
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 10, BLANCO, "Generado el .............: ");
    cout << FechaATextoCero(FechaDeHoy()) << "  " << HoraATexto() << " hs.";
    // Cuenta regresiva de validez del token: ciclo de repeticion exacta.
    for (segundos = SEG_VIGENCIA_TOKEN; segundos >= 1; segundos--) {
      _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 12);
      _textcolor(CIAN_CLARO);
      cout << "Vence en " << setw(2) << segundos << " segundo(s)... ";
      Sleep(1000);
    }
    _gotoxy(COL_IZQUIERDA, FILA_CUERPO + 12);
    _textcolor(ROJO_CLARO);
    cout << "El token vencio. Genere uno nuevo si lo necesita.";
    _textcolor(GRIS_CLARO);
    otro = LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 14, "Desea generar otro token?");
  } while (otro);
} // Menu_GenToken

//------------------------------------------------------------------------------
//  Menu_ModifClave
//  Pide la clave actual, la nueva y su confirmacion, con los mensajes que
//  corresponden si la clave actual no coincide o si la confirmacion difiere.
//------------------------------------------------------------------------------
void Menu_ModifClave(sDatosPers aDatosPers[], short usuario) {
  sDatosPers &datos = aDatosPers[usuario];
  str15 claveActual, claveNueva, claveConfirma;
  short intentos = 0;
  bool  correcta;

  EncabezadoUsuario("MODIFICAR MI CONTRASENA", datos.apeNom, usuario + 1);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, GRIS_OSCURO,
          "La contrasena debe tener entre " + to_string(LARGO_MIN_CLAVE) +
          " y " + to_string(LARGO_MAX_CLAVE) + " caracteres.");

  //--- Verificacion de la contrasena actual -----------------------------------
  do {
    LeerClave(COL_IZQUIERDA, FILA_CUERPO + 2, "Contrasena actual .......: ",
              claveActual, LARGO_MAX_CLAVE);
    intentos++;
    correcta = strcmp(claveActual, datos.clave) == 0;
    if (not correcta)
      MensajeError("La contrasena actual NO EXISTE o es incorrecta. "
                   "Intento " + to_string(intentos) + " de " +
                   to_string(MAX_INTENTOS) + ".");
  } while (not correcta and intentos < MAX_INTENTOS);
  if (not correcta) {
    MensajeError("Se agotaron los intentos. La contrasena no fue modificada.");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  LimpiarMensaje();

  //--- Ingreso y confirmacion de la contrasena nueva --------------------------
  do {
    do {
      LeerClave(COL_IZQUIERDA, FILA_CUERPO + 4, "Contrasena nueva ........: ",
                claveNueva, LARGO_MAX_CLAVE);
      correcta = LargoCadena(claveNueva) >= LARGO_MIN_CLAVE;
      if (not correcta)
        MensajeError("La contrasena nueva debe tener al menos " +
                     to_string(LARGO_MIN_CLAVE) + " caracteres.");
      else if (strcmp(claveNueva, datos.clave) == 0) {
        correcta = false;
        MensajeError("La contrasena nueva no puede ser igual a la actual.");
      }
      else
        LimpiarMensaje();
    } while (not correcta);
    LeerClave(COL_IZQUIERDA, FILA_CUERPO + 5, "Confirmar la nueva ......: ",
              claveConfirma, LARGO_MAX_CLAVE);
    correcta = strcmp(claveNueva, claveConfirma) == 0;
    if (not correcta)
      MensajeError("La confirmacion es DISTINTA a la contrasena nueva.");
  } while (not correcta);
  LimpiarMensaje();

  if (LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 7, "Confirma el cambio?")) {
    CopiarCadena(datos.clave, claveNueva, LARGO_MAX_CLAVE);
    MensajeOk("La contrasena fue modificada correctamente.");
  }
  else
    MensajeInfo("Operacion cancelada. La contrasena no fue modificada.");
  Pausa("Oprima una tecla para volver...");
} // Menu_ModifClave

//==============================================================================
//  MODULO DE NIVEL 2 - EMISION DE LISTADOS
//
//  La salida se redirige de la pantalla al archivo de texto ListadosHB.Txt
//  con freopen, de manera que los mismos cout escriben en el archivo. Al
//  terminar se vuelve a dirigir la salida a la consola con freopen("CON",...).
//==============================================================================

//--- Linea separadora del listado ---------------------------------------------
void LineaListado(char caracter, short ancho) {
  cout << string(ancho, caracter) << endl;
} // LineaListado

//--- Titulo de una seccion del listado ----------------------------------------
void TituloSeccion(short numero, string texto) {
  cout << endl;
  LineaListado('-', 78);
  cout << "  " << numero << ". " << texto << endl;
  LineaListado('-', 78);
  cout << endl;
} // TituloSeccion

//--- Encabezado del listado ---------------------------------------------------
void EncabezadoListado(const sDatosPers &datos, short nroUsuario) {
  int anio, mes, dia, diaSem;

  GetDate(anio, mes, dia, diaSem);
  LineaListado('=', 78);
  cout << "  HOME BANKING MESOPOTAMIA  -  Sucursal 00" << SUCURSAL << endl;
  cout << "  LISTADOS DEL USUARIO" << endl;
  cout << "  Emitido el " << NombreDiaSemana((short)diaSem) << ' '
       << FechaATextoCero(FechaDeHoy()) << " a las " << HoraATexto()
       << " hs." << endl;
  cout << "  Usuario: " << datos.apeNom << "   (nro. " << nroUsuario << ')'
       << endl;
  cout << "  Archivo: " << ARCH_LISTADOS << endl;
  LineaListado('=', 78);
} // EncabezadoListado

//--- Seccion 1: datos personales ----------------------------------------------
void ListarDatosPersonales(const sDatosPers &datos, short nroUsuario) {
  TituloSeccion(1, "LISTADO DE DATOS PERSONALES");
  cout << "  Nro. de usuario...........: " << nroUsuario << endl;
  cout << "  DNI.......................: " << datos.dni << endl;
  cout << "  Apellido y Nombre.........: " << datos.apeNom << endl;
  cout << "  Fecha de nacimiento.......: " << FechaATextoCero(datos.fechaNac)
       << endl;
  cout << "  Usuario...................: " << datos.usuario << endl;
  cout << "  Nro. de celular...........: " << datos.celular << endl;
  cout << "  Correo electronico........: " << datos.email << endl;
  cout << "  Domicilio.................: " << datos.domicilio << endl;
  cout << "  Nro. Cta. Caja Ahorro.....: " << datos.nroCtaCA << endl;
  cout << "  CBU.......................: " << datos.cbu << endl;
} // ListarDatosPersonales

//--- Seccion 2: cuentas del usuario -------------------------------------------
void ListarCuentas(const sDatosPers &datos) {
  short i;

  TituloSeccion(2, "LISTADO DE CUENTAS");
  cout << "  Nro. Cuenta         Tipo  Moneda             Saldo  Estado"
       << endl;
  cout << "  ";
  LineaListado('-', 62);
  for (i = 0; i < datos.cantCuentas; i++)
    cout << "  " << left  << setw(20) << datos.aCuentas[i].nroCta
         << left  << setw(6)  << datos.aCuentas[i].tipo
         << left  << setw(8)  << datos.aCuentas[i].moneda
         << right << setw(16) << fixed << setprecision(2)
         << datos.aCuentas[i].saldo
         << "  "  << left << setw(10) << datos.aCuentas[i].estado << endl;
  cout << endl << "  Cantidad de cuentas: " << datos.cantCuentas << endl;
} // ListarCuentas

//--- Seccion 3: agenda de cuentas ---------------------------------------------
void ListarAgenda(const sDatosPers &datos) {
  short i;

  TituloSeccion(3, "LISTADO DE CUENTAS AGENDADAS (por alias)");
  cout << "   Nro.  Alias               Titular                        CBU"
       << endl;
  cout << "  ";
  LineaListado('-', 76);
  for (i = 0; i < datos.cantAgenda; i++)
    cout << "  " << right << setw(4) << i + 1 << "  "
         << left  << setw(20) << datos.aAgenda[i].alias
         << left  << setw(31) << datos.aAgenda[i].titular
         << datos.aAgenda[i].cbu << endl;
  cout << endl << "  Cantidad de cuentas agendadas: " << datos.cantAgenda
       << endl;
} // ListarAgenda

//--- Seccion 4: movimientos de la caja de ahorro ------------------------------
double ListarMovimientosCA(const sDatosPers &datos, short nroUsuario,
                          short &cantMov) {
  sMovCA aMov[MAX_MOV_CA];
  short  i;
  double  totalDebe, totalHaber, saldoFinal;

  cantMov = LeerMovimientosCA(aMov, nroUsuario);
  OrdenarMovCAPorFecha(aMov, cantMov);
  saldoFinal = CalcularSaldos(aMov, cantMov,
                              datos.aCuentas[CTA_PESOS].saldoInicial);
  TotalizarMovCA(aMov, cantMov, totalDebe, totalHaber);

  TituloSeccion(4, "MOVIMIENTOS DE LA CAJA DE AHORRO (por fecha)");
  cout << "  Cta.CA:  " << datos.aCuentas[CTA_PESOS].nroCta << endl;
  cout << "  Saldo inicial $ " << fixed << setprecision(2) << right << setw(13)
       << datos.aCuentas[CTA_PESOS].saldoInicial << endl << endl;
  cout << "  " << left  << setw(ANCHO_FECHA) << "Fecha"
       << " T "  << left  << setw(ANCHO_DETALLE) << "Detalle"
       << right << setw(ANCHO_IMPORTE) << "Debe"
       << setw(ANCHO_IMPORTE) << "Haber"
       << setw(ANCHO_IMPORTE) << "Saldo" << endl;
  cout << "  ";
  LineaListado('-', LARGO_LISTADO_CA);
  for (i = 0; i < cantMov; i++) {
    cout << "  " << setw(ANCHO_FECHA) << FechaATexto(aMov[i].fecha)
         << ' '  << aMov[i].tipoMov
         << ' '  << left << setw(ANCHO_DETALLE) << aMov[i].detalle
         << fixed << setprecision(2) << right;
    if (aMov[i].tipoMov == DEBE)
      cout << setw(ANCHO_IMPORTE) << aMov[i].importe
           << setw(ANCHO_IMPORTE) << " ";
    else
      cout << setw(ANCHO_IMPORTE) << " "
           << setw(ANCHO_IMPORTE) << aMov[i].importe;
    cout << setw(ANCHO_IMPORTE) << aMov[i].saldo << endl;
  }
  cout << "  ";
  LineaListado('-', LARGO_LISTADO_CA);
  cout << "  " << right << setw(ANCHO_FECHA + 3 + ANCHO_DETALLE) << "Totales  "
       << fixed << setprecision(2) << right
       << setw(ANCHO_IMPORTE) << totalDebe
       << setw(ANCHO_IMPORTE) << totalHaber
       << setw(ANCHO_IMPORTE) << saldoFinal << endl;
  cout << endl << "  Cantidad de movimientos: " << cantMov << endl;
  return saldoFinal;
} // ListarMovimientosCA

//--- Seccion 5: consumos de la tarjeta de debito ------------------------------
double ListarMovimientosTD(short nroUsuario, short &cantMov) {
  sMovTD aMov[MAX_MOV_TD];
  short  i;
  double  total;

  cantMov = LeerMovimientosTD(aMov, nroUsuario);
  OrdenarMovTDPorFecha(aMov, cantMov);
  total = TotalizarMovTD(aMov, cantMov);

  TituloSeccion(5, "MOVIMIENTOS DE TARJETA DE DEBITO (por fecha)");
  cout << "  " << left  << setw(ANCHO_FECHA) << "Fecha"
       << ' '  << left  << setw(ANCHO_DETALLE) << "Comercio"
       << right << setw(ANCHO_IMPORTE) << "Importe" << endl;
  cout << "  ";
  LineaListado('-', LARGO_LISTADO_TD);
  for (i = 0; i < cantMov; i++)
    cout << "  " << setw(ANCHO_FECHA) << FechaATexto(aMov[i].fecha)
         << ' '  << left << setw(ANCHO_DETALLE) << aMov[i].detalle
         << fixed << setprecision(2) << right << setw(ANCHO_IMPORTE)
         << aMov[i].importe << endl;
  cout << "  ";
  LineaListado('-', LARGO_LISTADO_TD);
  cout << "  " << left << setw(LARGO_LISTADO_TD - ANCHO_IMPORTE)
       << "                    Total"
       << fixed << setprecision(2) << right << setw(ANCHO_IMPORTE) << total
       << endl;
  cout << endl << "  Cantidad de consumos: " << cantMov << endl;
  return total;
} // ListarMovimientosTD

//--- Seccion 6: consumos de la tarjeta de credito -----------------------------
double ListarMovimientosTC(short nroUsuario, short &cantMov) {
  sMovTC aMov[MAX_MOV_TC];
  short  i;
  double  total;

  cantMov = LeerMovimientosTC(aMov, nroUsuario);
  OrdenarMovTCPorFecha(aMov, cantMov);
  total = TotalizarMovTC(aMov, cantMov);

  TituloSeccion(6, "MOVIMIENTOS DE TARJETA DE CREDITO (por fecha)");
  cout << "  " << left  << setw(ANCHO_FECHA) << "Fecha"
       << ' '  << left  << setw(ANCHO_DETALLE) << "Comercio"
       << right << setw(ANCHO_CUOTAS) << "Cuotas"
       << setw(ANCHO_CPBTE + 1) << "Nro.Cpbte"
       << setw(ANCHO_IMPORTE) << "Importe" << endl;
  cout << "  ";
  LineaListado('-', LARGO_LISTADO_TC);
  for (i = 0; i < cantMov; i++)
    cout << "  " << setw(ANCHO_FECHA) << FechaATexto(aMov[i].fecha)
         << ' '  << left << setw(ANCHO_DETALLE) << aMov[i].detalle
         << right << setw(ANCHO_CUOTAS) << aMov[i].cuotas
         << setw(ANCHO_CPBTE + 1)
         << (aMov[i].nroCpbte > 0 ? to_string(aMov[i].nroCpbte) : "")
         << fixed << setprecision(2) << setw(ANCHO_IMPORTE)
         << aMov[i].importe << endl;
  cout << "  ";
  LineaListado('-', LARGO_LISTADO_TC);
  cout << "  " << left << setw(LARGO_LISTADO_TC - ANCHO_IMPORTE)
       << "                                     Total"
       << fixed << setprecision(2) << right << setw(ANCHO_IMPORTE) << total
       << endl;
  cout << endl << "  Cantidad de consumos: " << cantMov << endl;
  return total;
} // ListarMovimientosTC

//--- Seccion 7: resumen final --------------------------------------------------
void ListarResumen(const sDatosPers &datos, double saldoCA, double totalTD,
                   double totalTC, short registros) {
  TituloSeccion(7, "RESUMEN FINAL");
  cout << "  Saldo Caja de Ahorro $....: " << fixed << setprecision(2)
       << right << setw(16) << saldoCA << endl;
  cout << "  Saldo en dolares u$s......: " << right << setw(16)
       << datos.aCuentas[CTA_DOLARES].saldo << endl;
  cout << "  Consumido Tarj. Debito....: " << right << setw(16) << totalTD
       << endl;
  cout << "  Consumido Tarj. Credito...: " << right << setw(16) << totalTC
       << endl;
  cout << "  TOTAL consumido tarjetas..: " << right << setw(16)
       << totalTD + totalTC << endl;
  cout << endl;
  LineaListado('=', 78);
  cout << "  Registros emitidos en este listado: " << registros << endl;
  cout << "  FIN DE LOS LISTADOS" << endl;
  LineaListado('=', 78);
} // ListarResumen

//------------------------------------------------------------------------------
//  Menu_Listados - emite todos los listados del usuario a ListadosHB.Txt
//------------------------------------------------------------------------------
void Menu_Listados(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short nroUsuario = usuario + 1;
  short cantCA, cantTD, cantTC;
  double saldoCA, totalTD, totalTC;

  EncabezadoUsuario("EMITIR LISTADOS", datos.apeNom, nroUsuario);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO, GRIS_OSCURO,
          "Se emitiran los siguientes listados al archivo de texto");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, AMARILLO, ARCH_LISTADOS);
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 3, BLANCO, "1. Datos personales");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 4, BLANCO, "2. Cuentas");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 5, BLANCO, "3. Cuentas agendadas");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 6, BLANCO,
          "4. Movimientos de la Caja de Ahorro");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 7, BLANCO,
          "5. Consumos de la Tarjeta de Debito");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 8, BLANCO,
          "6. Consumos de la Tarjeta de Credito");
  MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 9, BLANCO, "7. Resumen final");
  if (not LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 11, "Confirma la emision?")) {
    MensajeInfo("Emision cancelada por el usuario.");
    Pausa("Oprima una tecla para volver...");
    return;
  }

  //--- Se redirige la salida de la pantalla al archivo de texto --------------
  if (freopen(ARCH_LISTADOS, "w", stdout) == NULL) {
    MensajeError("No se pudo crear el archivo " + string(ARCH_LISTADOS) + ".");
    Pausa("Oprima una tecla para volver...");
    return;
  }
  EncabezadoListado(datos, nroUsuario);
  ListarDatosPersonales(datos, nroUsuario);
  ListarCuentas(datos);
  ListarAgenda(datos);
  saldoCA = ListarMovimientosCA(datos, nroUsuario, cantCA);
  totalTD = ListarMovimientosTD(nroUsuario, cantTD);
  totalTC = ListarMovimientosTC(nroUsuario, cantTC);
  ListarResumen(datos, saldoCA, totalTD, totalTC, cantCA + cantTD + cantTC);
  fflush(stdout);

  //--- Se vuelve a dirigir la salida a la consola ----------------------------
  freopen("CON", "w", stdout);
  cout.clear();

  EncabezadoUsuario("EMITIR LISTADOS", datos.apeNom, nroUsuario);
  Marco(COL_IZQUIERDA, FILA_CUERPO, COL_IZQUIERDA + 56, FILA_CUERPO + 7,
        VERDE_CLARO);
  MnsgBox(COL_IZQUIERDA + 2, FILA_CUERPO, VERDE_CLARO, " EMISION FINALIZADA ");
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 1, BLANCO, "Archivo generado ...: ");
  cout << ARCH_LISTADOS;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 2, BLANCO, "Movimientos de CA ..: ");
  cout << cantCA;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 3, BLANCO, "Consumos con TD ....: ");
  cout << cantTD;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 4, BLANCO, "Consumos con TC ....: ");
  cout << cantTC;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 5, BLANCO, "Registros emitidos .: ");
  cout << cantCA + cantTD + cantTC;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 6, BLANCO, "Saldo final de CA ..: ");
  cout << fixed << setprecision(2) << saldoCA;
  _textcolor(GRIS_CLARO);
  MensajeOk("Los listados quedaron en " + string(ARCH_LISTADOS) + ".");
  Pausa("Oprima una tecla para volver...");
} // Menu_Listados

//------------------------------------------------------------------------------
//  Menu_Logout - cierra la sesion del usuario
//  Devuelve true si se desea ingresar con otro usuario.
//------------------------------------------------------------------------------
bool Menu_Logout(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  bool otroUsuario;

  Encabezado("CIERRE DE SESION");
  Marco(COL_IZQUIERDA, FILA_CUERPO, COL_IZQUIERDA + 56, FILA_CUERPO + 6,
        AMARILLO);
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 1, BLANCO, "Usuario ..........: ");
  cout << datos.apeNom;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 2, BLANCO, "Cuenta ...........: ");
  cout << datos.nroCtaCA;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 3, BLANCO, "Saldo final $ ....: ");
  cout << fixed << setprecision(2) << SaldoEnPesos(datos);
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 4, BLANCO, "Saldo final u$s ..: ");
  cout << fixed << setprecision(2) << datos.aCuentas[CTA_DOLARES].saldo;
  MnsgBox(COL_IZQUIERDA + 3, FILA_CUERPO + 5, BLANCO, "Hora de salida ...: ");
  cout << HoraATexto() << " hs.";
  _textcolor(GRIS_CLARO);
  MensajeOk("La sesion de " + string(datos.usuario) + " fue cerrada.");
  otroUsuario = LeerSiNo(COL_IZQUIERDA, FILA_CUERPO + 8,
                         "Desea ingresar con otro usuario?");
  return otroUsuario;
} // Menu_Logout

//==============================================================================
//  MODULOS DE NIVEL 1
//==============================================================================

//------------------------------------------------------------------------------
//  MenuLogin
//  Solicita DNI, usuario y clave. Ubica al usuario con una BUSQUEDA SECUENCIAL
//  por DNI sobre el arreglo aDatosPers y verifica que el nombre de usuario y
//  la clave se correspondan. Devuelve el indice del usuario identificado o
//  SIN_USUARIO si se agotaron los intentos.
//------------------------------------------------------------------------------
short MenuLogin(const sDatosPers aDatosPers[], short usuario) {
  long  dni;
  str15 nomUsuario;
  str15 clave;
  short posicion;
  short intentos = 0;
  bool  identificado = false;

  do {
    Encabezado("INGRESO AL SISTEMA");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, CIAN_CLARO,
            "Ingrese sus datos para operar en su Home Banking.");
    Marco(COL_IZQUIERDA, FILA_CUERPO + 2, COL_IZQUIERDA + 52, FILA_CUERPO + 8,
          AZUL_CLARO);
    MnsgBox(COL_IZQUIERDA + 2, FILA_CUERPO + 2, AMARILLO, " LOGIN ");
    dni = LeerEntero(COL_IZQUIERDA + 3, FILA_CUERPO + 4,
                     "DNI .............: ", 1000000, 99999999);
    LeerCadena(COL_IZQUIERDA + 3, FILA_CUERPO + 5,
               "Usuario .........: ", nomUsuario, 15);
    LeerClave(COL_IZQUIERDA + 3, FILA_CUERPO + 6,
              "Clave ...........: ", clave, LARGO_MAX_CLAVE);

    posicion = BuscarUsuarioPorDni(aDatosPers, dni);
    identificado = posicion != NO_ENCONTRADO and
                   strcmp(aDatosPers[posicion].usuario, nomUsuario) == 0 and
                   strcmp(aDatosPers[posicion].clave, clave) == 0;
    intentos++;
    if (identificado)
      usuario = posicion;
    else {
      MensajeError("DNI, usuario o clave incorrectos. Intento " +
                   to_string(intentos) + " de " + to_string(MAX_INTENTOS) +
                   ".");
      Pausa("Oprima una tecla para reintentar...");
    }
  } while (not identificado and intentos < MAX_INTENTOS);

  if (not identificado) {
    Encabezado("INGRESO AL SISTEMA");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO, ROJO_CLARO,
            "Se agotaron los " + to_string(MAX_INTENTOS) +
            " intentos permitidos.");
    MnsgBox(COL_IZQUIERDA, FILA_CUERPO + 1, GRIS_CLARO,
            "Por su seguridad, el acceso quedo bloqueado.");
    Pausa("Oprima una tecla para salir...");
    return SIN_USUARIO;
  }
  MensajeOk("Bienvenido/a " + string(aDatosPers[usuario].apeNom) + ".");
  Pausa("Oprima una tecla para ingresar al menu principal...");
  return usuario;
} // MenuLogin

//------------------------------------------------------------------------------
//  MenuPpal
//  Presenta el menu de nivel 1 y devuelve la opcion elegida por el usuario.
//------------------------------------------------------------------------------
short MenuPpal(const sDatosPers aDatosPers[], short usuario) {
  const sDatosPers &datos = aDatosPers[usuario];
  short fila = FILA_CUERPO;
  short columnaDer = COL_IZQUIERDA + 38;

  EncabezadoUsuario("MENU PRINCIPAL", datos.apeNom, usuario + 1);
  MostrarSaldoDisponible(datos, fila);
  MnsgBox(columnaDer, fila, CIAN_CLARO, "Saldo u$s: ");
  _textcolor(BLANCO);
  cout << fixed << setprecision(2) << datos.aCuentas[CTA_DOLARES].saldo;
  fila = fila + 2;

  MnsgBox(COL_IZQUIERDA, fila,     BLANCO, " 1. Mis Cuentas");
  MnsgBox(COL_IZQUIERDA, fila + 1, BLANCO, " 2. Mis Datos Personales");
  MnsgBox(COL_IZQUIERDA, fila + 2, BLANCO, " 3. Mostrar mi CBU");
  MnsgBox(COL_IZQUIERDA, fila + 3, BLANCO, " 4. Movimientos");
  MnsgBox(COL_IZQUIERDA, fila + 4, BLANCO, " 5. Mis Tarjetas");
  MnsgBox(COL_IZQUIERDA, fila + 5, BLANCO, " 6. Transferir dinero");
  MnsgBox(COL_IZQUIERDA, fila + 6, BLANCO, " 7. Deposito");
  MnsgBox(COL_IZQUIERDA, fila + 7, BLANCO, " 8. Registrar una compra");
  MnsgBox(COL_IZQUIERDA, fila + 8, BLANCO, " 9. Recargas (Celular / SUBE)");

  MnsgBox(columnaDer, fila,     BLANCO, "10. Compra / Venta de dolares");
  MnsgBox(columnaDer, fila + 1, BLANCO, "11. Inversiones (P.Fijo / FCI)");
  MnsgBox(columnaDer, fila + 2, BLANCO, "12. Simulacion de Plazo Fijo");
  MnsgBox(columnaDer, fila + 3, BLANCO, "13. Agenda de cuentas CBU");
  MnsgBox(columnaDer, fila + 4, BLANCO, "14. Crear una cuenta nueva");
  MnsgBox(columnaDer, fila + 5, BLANCO, "15. Generar Token");
  MnsgBox(columnaDer, fila + 6, BLANCO, "16. Modificar mi contrasena");
  MnsgBox(columnaDer, fila + 7, BLANCO, "17. Emitir listados a archivo");
  MnsgBox(columnaDer, fila + 8, AMARILLO, " 0. Cerrar la sesion (Logout)");

  MnsgBox(COL_IZQUIERDA, fila + 10, AZUL_CLARO, Separador(70, '-'));
  return LeerOpcion(COL_IZQUIERDA, fila + 11, OPC_LOGOUT, OPC_LISTADOS);
} // MenuPpal

//==============================================================================
//  MODULO DE NIVEL 0 - SISTEMA HOME BANKING
//==============================================================================

//--- Prepara la consola: tamanio, titulo, pagina de codigos y azar ------------
void IniciarConsola() {
  SetConsoleOutputCP(437);            // pagina de codigos con caracteres graficos
  Screen::_window(ANCHO_PANTALLA, ALTO_PANTALLA + 1);
  Screen::BarraTitulo();
  Screen::BloquearCambioTamanio();
  Screen::InhabilitarHistorial();
  srand((unsigned)time(NULL));        // semilla para la generacion de tokens
} // IniciarConsola

//--- Pantalla de presentacion --------------------------------------------------
void PantallaBienvenida() {
  Encabezado("BIENVENIDO");
  Marco(COL_IZQUIERDA + 8, FILA_CUERPO + 2, COL_IZQUIERDA + 60,
        FILA_CUERPO + 10, AMARILLO);
  MnsgBox(COL_IZQUIERDA + 20, FILA_CUERPO + 4, AMARILLO,
          "BANCO MESOPOTAMIA S.A.");
  MnsgBox(COL_IZQUIERDA + 22, FILA_CUERPO + 6, BLANCO,
          "Home Banking - Version 1");
  MnsgBox(COL_IZQUIERDA + 18, FILA_CUERPO + 8, GRIS_OSCURO,
          "Trabajo Practico Nro. 1 - Curso K____");
  _textcolor(GRIS_CLARO);
  Pausa("Oprima una tecla para comenzar...");
} // PantallaBienvenida

//--- Pantalla de despedida -----------------------------------------------------
void PantallaDespedida() {
  Encabezado("FIN DEL SISTEMA");
  MnsgBox(COL_IZQUIERDA + 14, FILA_CUERPO + 4, AMARILLO,
          "Gracias por operar en Home Banking Mesopotamia.");
  MnsgBox(COL_IZQUIERDA + 22, FILA_CUERPO + 6, GRIS_CLARO,
          "Hasta la proxima.");
  _gotoxy(1, ALTO_PANTALLA + 1);
  _textcolor(GRIS_CLARO);
} // PantallaDespedida

//------------------------------------------------------------------------------
//  SistemaHomeBanking
//
//  Estrategia:
//    1. Prepara la consola y carga los datos personales de los cinco usuarios.
//    2. Invoca a MenuLogin(aDatosPers, usuario), que devuelve el usuario
//       identificado.
//    3. Reconstruye el saldo de la caja de ahorro leyendo MovimientosCA.Txt.
//    4. Invoca a MenuPpal(), que devuelve la opcion elegida, y despacha al
//       modulo correspondiente por seleccion multiple.
//    5. Al cerrar la sesion permite el ingreso de otro usuario.
//------------------------------------------------------------------------------
void SistemaHomeBanking() {
  sDatosPers aDatosPers[CANT_USUARIOS];
  short usuario;
  short opcion;
  bool  continuar;

  IniciarConsola();
  CargarDatosPersonales(aDatosPers);
  PantallaBienvenida();

  do {
    usuario = MenuLogin(aDatosPers, SIN_USUARIO);
    if (usuario == SIN_USUARIO)
      continuar = false;
    else {
      RecalcularSaldoCA(aDatosPers[usuario], usuario + 1);
      do {
        opcion = MenuPpal(aDatosPers, usuario);
        switch (opcion) {
          case OPC_MIS_CUENTAS   : Menu_MisCuentas(aDatosPers, usuario);   break;
          case OPC_DATOS_PERS    : Menu_DatosPers(aDatosPers, usuario);    break;
          case OPC_MOSTRAR_CBU   : Menu_MostrarCBU(aDatosPers, usuario);   break;
          case OPC_MOVIMIENTOS   : Menu_Movimientos(aDatosPers, usuario);  break;
          case OPC_TARJETAS      : Menu_Tarjetas(aDatosPers, usuario);     break;
          case OPC_TRANSFERIR    : Menu_Transferir(aDatosPers, usuario);   break;
          case OPC_DEPOSITO      : Menu_Deposito(aDatosPers, usuario);     break;
          case OPC_COMPRAS       : Menu_Compras(aDatosPers, usuario);      break;
          case OPC_RECARGAR      : Menu_Recargar(aDatosPers, usuario);     break;
          case OPC_DOLARES       : Menu_CompraVentaDolares(aDatosPers,
                                                           usuario);       break;
          case OPC_PLAZO_FIJO    : Menu_PlazoFijo(aDatosPers, usuario);    break;
          case OPC_SIMULACION_PF : Menu_SimulacionPF(aDatosPers, usuario); break;
          case OPC_AGENDA_CBU    : Menu_AgendaCBU(aDatosPers, usuario);    break;
          case OPC_NUEVA_CUENTA  : Menu_NuevaCuenta(aDatosPers, usuario);  break;
          case OPC_GEN_TOKEN     : Menu_GenToken(aDatosPers, usuario);     break;
          case OPC_MODIF_CLAVE   : Menu_ModifClave(aDatosPers, usuario);   break;
          case OPC_LISTADOS      : Menu_Listados(aDatosPers, usuario);     break;
        }
      } while (opcion != OPC_LOGOUT);
      continuar = Menu_Logout(aDatosPers, usuario);
    }
  } while (continuar);

  PantallaDespedida();
} // SistemaHomeBanking

//==============================================================================
//  BLOQUE PRINCIPAL
//==============================================================================
int main() {
  SistemaHomeBanking();
  return 0;
} // main
