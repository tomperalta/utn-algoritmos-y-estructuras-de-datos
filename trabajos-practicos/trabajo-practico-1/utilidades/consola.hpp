// ---------------------------------------------------------------------------
// consola.hpp
// Version portable de los modulos de consola entregados por la catedra en
// ModulosHB.cpp (el original se conserva sin modificar en lib/lib.cpp).
//
// Se respetan los nombres y la semantica de las funciones de la catedra
// (_gotoxy, _textcolor, _clrscr, _clreol, MnsgBox, Marco, OcultarCursor,
// MostrarCursor, MenuNavegar, ...) pero cada una tiene dos implementaciones:
// la de Windows con <windows.h> y la de macOS/Linux con secuencias ANSI.
//
// Modo texto plano
// ----------------
// Cuando la salida estandar NO es una consola (por ejemplo cuando se redirige
// a ListadosHB.Txt con freopen, o cuando se ejecuta el programa con la entrada
// tomada de un archivo para probarlo), se activa el <<modo texto plano>>: se
// suprimen colores, marcos y posicionamiento absoluto, y la salida se emite en
// forma secuencial. Asi el archivo de listados queda legible y sin basura.
// ---------------------------------------------------------------------------
#ifndef CONSOLA_HPP
#define CONSOLA_HPP

namespace Screen {

  // ----- Codigos devueltos por LeerTecla() -----
  const short TECLA_ARRIBA = -10;
  const short TECLA_ABAJO  = -11;
  const short TECLA_ENTER  = -12;
  const short TECLA_ESCAPE = -13;
  const short TECLA_FIN    = -14; // fin de la entrada (EOF)

  // ----- Estado interno del modulo -----
  bool  modoTextoPlano = false;   // true -> sin ANSI ni posicionamiento
  short colorTextoAct  = BLANCO;
  short colorFondoAct  = NEGRO;
  short posX = 1, posY = 1;       // posicion del cursor que se lleva en cuenta
  short filaTexto = 0;            // ultima fila emitida en modo texto plano

  // Traduccion de los 16 colores de Windows a codigos ANSI de texto.
  const short ANSI_TEXTO[16] = {30, 34, 32, 36, 31, 35, 33, 37,
                                90, 94, 92, 96, 91, 95, 93, 97};

  // -------------------------------------------------------------------------
  // Activa o desactiva el modo texto plano.
  // -------------------------------------------------------------------------
  void ModoTextoPlano(bool activar) {
    modoTextoPlano = activar;
    filaTexto = 0;
  } // ModoTextoPlano

  bool EnModoTextoPlano() {
    return modoTextoPlano;
  } // EnModoTextoPlano

  // -------------------------------------------------------------------------
  // Aplica simultaneamente color de texto y de fondo.
  // -------------------------------------------------------------------------
  void setConsoleColor(short colText, short colBack) {
    colorTextoAct = colText;
    colorFondoAct = colBack;
    if (modoTextoPlano)
      return;
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            colText + 16 * colBack);
#else
    cout << "\033[" << ANSI_TEXTO[colText % 16] << ';'
         << ANSI_TEXTO[colBack % 16] + 10 << 'm';
#endif
  } // setConsoleColor

  void ActualizaColores(short colText, short colBack) {
    setConsoleColor(colText, colBack);
  } // ActualizaColores

  void _textcolor(short color) {
    setConsoleColor(color, colorFondoAct);
  } // _textcolor

  void _textbackground(short color) {
    setConsoleColor(colorTextoAct, color);
  } // _textbackground

  // Restaura los colores por omision.
  void ColorNormal() {
    setConsoleColor(GRIS_CLARO, NEGRO);
  } // ColorNormal

  // -------------------------------------------------------------------------
  // Ubica el cursor en la columna x, fila y (base 1, igual que gotoxy()).
  // En modo texto plano se emiten los saltos de linea y los espacios
  // necesarios para aproximar la misma disposicion.
  // -------------------------------------------------------------------------
  void _gotoxy(short x, short y) {
    if (modoTextoPlano) {
      if (y != filaTexto) {
        cout << '\n';
        filaTexto = y;
        posX = 1;
      }
      for (short i = posX; i < x; i++)
        cout << ' ';
      if (x > posX)
        posX = x;
      posY = y;
      return;
    }
    posX = x;
    posY = y;
#ifdef _WIN32
    COORD coord;
    coord.X = x - 1;
    coord.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y << ';' << x << 'H';
#endif
  } // _gotoxy

  int _wherex() {
    return posX;
  } // _wherex

  int _wherey() {
    return posY;
  } // _wherey

  // -------------------------------------------------------------------------
  // Establece los limites logicos de la ventana de trabajo. La version
  // portable solo registra los limites: se conserva por compatibilidad con la
  // funcion window() de la catedra.
  // -------------------------------------------------------------------------
  short venIzq = 1, venSup = 1, venDer = ANCHO_PANT, venInf = ALTO_PANT;

  void _window(short izq, short sup, short der, short inf) {
    venIzq = izq;
    venSup = sup;
    venDer = der;
    venInf = inf;
  } // _window

  // -------------------------------------------------------------------------
  // Limpia la pantalla y deja el cursor en (1,1).
  // -------------------------------------------------------------------------
  void _clrscr() {
    if (modoTextoPlano) {
      cout << '\n';
      filaTexto = 0;
      posX = 1;
      posY = 1;
      return;
    }
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD escritos;
    COORD origen = {0, 0};
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD celdas = csbi.dwSize.X * csbi.dwSize.Y;
    WORD atributo = colorTextoAct | (colorFondoAct << 4);
    FillConsoleOutputCharacter(hConsole, ' ', celdas, origen, &escritos);
    FillConsoleOutputAttribute(hConsole, atributo, celdas, origen, &escritos);
#else
    cout << "\033[2J";
#endif
    _gotoxy(1, 1);
  } // _clrscr

  // -------------------------------------------------------------------------
  // Limpia desde la posicion del cursor hasta el fin de la linea.
  // -------------------------------------------------------------------------
  void _clreol() {
    if (modoTextoPlano)
      return;
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD escritos;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD desde = csbi.dwCursorPosition;
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X - desde.X, desde,
                               &escritos);
    SetConsoleCursorPosition(hConsole, desde);
#else
    cout << "\033[K";
#endif
  } // _clreol

  void OcultarCursor() {
    if (modoTextoPlano)
      return;
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsole, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &info);
#else
    cout << "\033[?25l";
#endif
  } // OcultarCursor

  void MostrarCursor() {
    if (modoTextoPlano)
      return;
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsole, &info);
    info.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &info);
#else
    cout << "\033[?25h";
#endif
  } // MostrarCursor

  void BarraTitulo(const char titulo[]) {
    if (modoTextoPlano)
      return;
#ifdef _WIN32
    SetConsoleTitleA(titulo);
#else
    cout << "\033]0;" << titulo << "\007";
#endif
  } // BarraTitulo

  // -------------------------------------------------------------------------
  // Largo VISIBLE de una cadena UTF-8: cuenta caracteres, no bytes. Los bytes
  // de continuacion de UTF-8 (10xxxxxx) no suman ancho de columna.
  // -------------------------------------------------------------------------
  short LargoVisible(const char cad[]) {
    short cant = 0;

    for (short i = 0; cad[i] != '\0'; i++)
      if ((cad[i] & 0xC0) != 0x80)
        cant++;
    return cant;
  } // LargoVisible

  // -------------------------------------------------------------------------
  // Emite una cadena OCUPANDO EXACTAMENTE <<ancho>> columnas.
  //   alinea = 'i' -> a izquierda    alinea = 'd' -> a derecha
  //
  // Si el texto es mas corto se lo rellena con espacios en blanco; si es mas
  // largo se lo RECORTA. El recorte es indispensable: el enunciado exige
  // respetar el formato tabular, y un dato que se pasara de su columna
  // desplazaria todas las columnas siguientes de ese renglon.
  //
  // Tanto el relleno como el recorte se calculan sobre el largo VISIBLE y no
  // sobre la cantidad de bytes, de modo que las tablas queden alineadas aunque
  // el texto tenga letras acentuadas (en UTF-8 ocupan dos bytes). Al recortar
  // nunca se parte un caracter por la mitad.
  // -------------------------------------------------------------------------
  void Texto(const char cad[], short ancho, char alinea) {
    short visible = LargoVisible(cad);
    short relleno = (short)(ancho - visible);

    if (ancho <= 0)
      return;

    if (relleno >= 0) {                    // entra: se rellena
      if (alinea == 'd')
        for (short i = 0; i < relleno; i++)
          cout << ' ';
      cout << cad;
      if (alinea != 'd')
        for (short i = 0; i < relleno; i++)
          cout << ' ';
      return;
    }

    // No entra: se emiten solamente los primeros <<ancho>> caracteres
    // visibles, saltando los bytes de continuacion de UTF-8.
    short emitidos = 0;

    for (short i = 0; cad[i] != '\0' and emitidos < ancho; i++) {
      cout << cad[i];
      if ((cad[i + 1] & 0xC0) != 0x80)     // el proximo no es continuacion
        emitidos++;
    }
  } // Texto

  // -------------------------------------------------------------------------
  // Mensaje en la posicion x, y (tres formas, como en ModulosHB.cpp).
  // -------------------------------------------------------------------------
  void MnsgBox(short x, short y, const char mensaje[]) {
    _gotoxy(x, y);
    cout << mensaje;
    posX = posX + LargoVisible(mensaje);
  } // MnsgBox

  void MnsgBox(short x, short y, short colorText, const char mensaje[]) {
    _textcolor(colorText);
    MnsgBox(x, y, mensaje);
  } // MnsgBox

  void MnsgBox(short x, short y, const char mensaje[], char alinea,
               short ancho) {
    _gotoxy(x, y);
    Texto(mensaje, ancho, alinea);
    posX = posX + (ancho > LargoVisible(mensaje) ? ancho
                                                 : LargoVisible(mensaje));
  } // MnsgBox

  // -------------------------------------------------------------------------
  // Marco rectangular con caracteres graficos, de (x1,y1) a (x2,y2).
  // -------------------------------------------------------------------------
  void Marco(short x1, short y1, short x2, short y2, short color) {
    if (modoTextoPlano)
      return;
    _textcolor(color);
    MnsgBox(x1, y1, "╔");
    MnsgBox(x2, y1, "╗");
    MnsgBox(x1, y2, "╚");
    MnsgBox(x2, y2, "╝");
    for (short x = x1 + 1; x < x2; x++) {
      MnsgBox(x, y1, "═");
      MnsgBox(x, y2, "═");
    }
    for (short y = y1 + 1; y < y2; y++) {
      MnsgBox(x1, y, "║");
      MnsgBox(x2, y, "║");
    }
  } // Marco

  // Linea horizontal simple dentro de un marco.
  void LineaInterna(short x1, short x2, short y, short color) {
    if (modoTextoPlano)
      return;
    _textcolor(color);
    MnsgBox(x1, y, "╠");
    MnsgBox(x2, y, "╣");
    for (short x = x1 + 1; x < x2; x++)
      MnsgBox(x, y, "═");
  } // LineaInterna

  // -------------------------------------------------------------------------
  // Linea separadora de <<ancho>> caracteres repetidos, emitida en la
  // posicion actual del cursor.
  // -------------------------------------------------------------------------
  void Separador(short ancho, char car) {
    for (short i = 0; i < ancho; i++)
      cout << car;
  } // Separador

  // -------------------------------------------------------------------------
  // Demora visible en pantalla.
  // -------------------------------------------------------------------------
  void Espera(long milisegundos) {
    if (not modoTextoPlano)
      Demorar(milisegundos);
  } // Espera

  // -------------------------------------------------------------------------
  // Lee una tecla y la traduce a los codigos TECLA_*. Devuelve el codigo del
  // caracter cuando no es una tecla especial.
  // -------------------------------------------------------------------------
  short LeerTecla() {
    int   codigos[3] = {0, 0, 0};
    short cant = LeerSecuenciaTecla(codigos);

    if (cant == 0)
      return TECLA_FIN;
#ifdef _WIN32
    // En Windows las teclas extendidas llegan como un prefijo 0 o 224 seguido
    // del codigo de la tecla.
    if (cant == 2 and (codigos[0] == 0 or codigos[0] == 224)) {
      if (codigos[1] == 72) return TECLA_ARRIBA;
      if (codigos[1] == 80) return TECLA_ABAJO;
      return 0;
    }
#else
    // En macOS/Linux llegan como la secuencia ESC [ A  /  ESC [ B.
    if (cant == 3 and codigos[0] == 27 and codigos[1] == '[') {
      if (codigos[2] == 'A') return TECLA_ARRIBA;
      if (codigos[2] == 'B') return TECLA_ABAJO;
      return 0;
    }
#endif
    if (codigos[0] == 27) return TECLA_ESCAPE;
    if (codigos[0] == 13 or codigos[0] == 10) return TECLA_ENTER;
    return (short) codigos[0];
  } // LeerTecla

  // -------------------------------------------------------------------------
  // Menu dinamico. Presenta las <<cantOpc>> opciones de aMenu a partir de la
  // linea linIni, columna colIni, y devuelve el indice (base 0) de la opcion
  // elegida, o NO_ENCONTRADO si se cancela con ESCAPE.
  //
  //   * En consola: se navega con las flechas ARRIBA / ABAJO, se confirma con
  //     ENTER y se puede saltar a una opcion tecleando su numero.
  //   * Fuera de consola (entrada redirigida): se lee el numero de opcion.
  // -------------------------------------------------------------------------
  short MenuNavegar(str40Opc aMenu[], short cantOpc, short linIni,
                    short colIni) {
    char rotulo[LARGO_OPCION + 12];
    short opcion = 0;

    // ----- Entrada no interactiva: se lee el numero de la opcion -----
    if (not EntradaEsConsola()) {
      for (short i = 0; i < cantOpc; i++) {
        snprintf(rotulo, sizeof rotulo, "%2d. %s", i + 1, aMenu[i]);
        MnsgBox(colIni, linIni + i, COL_MENU, rotulo);
      }
      _gotoxy(colIni, linIni + cantOpc + 1);
      cout << "Opcion: ";
      cout.flush();

      char  linea[80];
      char *resto;
      long  leido;

      cin.getline(linea, sizeof linea);
      if (cin.eof() and linea[0] == '\0') {    // se agoto la entrada
        entradaAgotada = true;
        return NO_ENCONTRADO;
      }
      if (cin.fail() and not cin.eof()) {      // linea mas larga que el buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
      if (cin.eof())
        entradaAgotada = true;
      leido = strtol(linea, &resto, 10);
      if (leido < 1 or leido > cantOpc)
        return NO_ENCONTRADO;                  // se interpreta como <<volver>>
      cout << leido << endl;
      return (short)(leido - 1);
    }

    // ----- Consola interactiva: navegacion con flechas -----
    OcultarCursor();
    MnsgBox(colIni, linIni + cantOpc + 1, COL_AVISO,
            "Flechas ↑ ↓ o el numero, ENTER confirma, ESC vuelve");

    short tecla = 0;
    bool  elegido = false;

    while (not elegido) {
      // Se redibuja solo la opcion resaltada y la anterior.
      for (short i = 0; i < cantOpc; i++) {
        snprintf(rotulo, sizeof rotulo, "%c%2d. %s",
                 (i == opcion ? '>' : ' '), i + 1, aMenu[i]);
        _textcolor(i == opcion ? AMARILLO : COL_MENU);
        MnsgBox(colIni, linIni + i, rotulo, 'i', LARGO_OPCION + 5);
      }
      tecla = LeerTecla();
      if (tecla == TECLA_ARRIBA)
        opcion = (opcion == 0 ? cantOpc - 1 : opcion - 1);
      else if (tecla == TECLA_ABAJO)
        opcion = (opcion == cantOpc - 1 ? 0 : opcion + 1);
      else if (tecla == TECLA_ENTER)
        elegido = true;
      else if (tecla == TECLA_ESCAPE or tecla == TECLA_FIN) {
        MostrarCursor();
        return NO_ENCONTRADO;
      }
      else if (tecla >= '1' and tecla <= '9' and tecla - '0' <= cantOpc)
        opcion = (short)(tecla - '1');
    }
    MostrarCursor();
    ColorNormal();
    return opcion;
  } // MenuNavegar

} // namespace Screen

#endif // CONSOLA_HPP
