// TP1-2026: HOME BANKING.

// Módulos para manejo de la Consola Pantalla y Teclado.
// Módulos para obtener la Fecha y Hora del Sistema.

// Estos módulos deberán ser utilizados como herramientas productivas para el
// TP1-HomeBanking.
// El proceso es <<Interactivo>>, por lo que el código se duplica con respecto a
// un proceso en <<Batch>> o por Lotes, debido a que en el proceso interactivo
// además de contar con el código que resuelve el problema lógico, se requiere del
// código de la interface con el usuario.
// Los módulos se encuentran dentro del del namespace o espacio de nombres que
// correspondan a los módulos relacionados con la pantalla y/o teclado es decir,
// con la Consola. Así tenemos en esta entrega los namespace de <<Screen>> y
// <<FechaHora>>. Cada grupo de trabajo podrá optar por utilizar otros namespace.
// Podemos copiar el <<namespace Screen>> completo con todos los módulos allí
// definidos; como así también el <<namespace FechaHora>>.
// Los módulos dentro de un namespace que utilicemos deberán estar acompañados
// por el ámbito al que pertenece. Por ejemplo: Screen::_gotoxy(x,y);
// Screen::_textcolor(n); Screen::_clrscr(); Screen::MnsgBox(x,y,Mensaje);
// Existe una forma de NO indicar el ámbito a la función definida, si utilizamos
// la siguiente sentencia: <<using namespace Screen>>, entonces luego de esto
// podemos utilizar esas funciones como se indica a continuación:
// _gotoxy(x,y); _textcolor(n); _clrscr(); MnsgBox(x,y,Mensaje);
// Los namespace tienen la siguiente sintaxis:

// namespace NomEspacioNombre {
//   Cabecera Mod1() {
//     Cuerpo del Mod1;
//   } // Fin Mod1
//
//   Cabecera Mod2() {
//     Cuerpo del Mod2;
//   } // Fin Mod2
//   ...
//   ...
// } // Fin namespace Screen

// Los nombres de módulos que inician con el caracter underscore o guión bajo o
// subrayado, tienen el mismo nombre de los módulos de Borland, por lo que el
// underscore indican que no son los módulos de Borland. El compilador a usar
// NO es el de Borland sino el TDM-GCC-64 GNU-GCC Compiler
// C++ Compiler x86_64-w64-mingw32-g++.exe

namespace Screen {

  void setConsoleColor(WORD colText, WORD colBack) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colText + 16*colBack);
  } // setConsoleColor

  void _window(short izq, short sup, short der, short inf) { // Clon de window()
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SMALL_RECT window = csbi.srWindow;

    window.Left = izq - 1;
    window.Top = sup - 1;
    window.Right = der - 1;
    window.Bottom = inf - 1;
  } // _window

  void _gotoxy(short x, short y) { // Clon de gotoxy()
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SMALL_RECT window = csbi.srWindow;
    COORD coord;

    coord.X = window.Left + x;
    coord.Y = window.Top + y;
    if (coord.X <= window.Right && coord.Y <= window.Bottom)
        SetConsoleCursorPosition(hConsole, coord);
  } // _gotoxy

  int _wherex() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return csbi.dwCursorPosition.X;
    return -1;
  } // _wherex

  int _wherey() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return csbi.dwCursorPosition.Y;
    return -1;
  } // _wherey

  void ActualizaColores(WORD colTextAct, WORD colBackAc) {
  	                         // Clon de textcolor() y textbackground() combinados.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // En Windows, el fondo se desplaza 4 bits a la izq. (se multiplica por 16)
    WORD atributo = colTextAct | (colBackAc << 4);
    SetConsoleTextAttribute(hConsole, atributo);
  } // ActualizaColores

  WORD ObtenerColorTextoActual() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        // Aplica una máscara para quedarse únicamente con el color del texto
        return csbi.wAttributes & 0x000F;
    }
    return 0; // Retorna 0 (negro) si hubo un error
  } // ObtenerColorTextoActual

  WORD ObtenerColorFondo() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        // El color de fondo está en los primeros 4 bits de la parte alta
        // Por lo tanto, dividimos entre 16 para extraerlo
        return (csbi.wAttributes >> 4) & 0x0F;
    }
    return -1; // Retorna -1 si ocurre un error
  } // ObtenerColorFondo

  void _textcolor(WORD color) {
    ActualizaColores(color,ObtenerColorFondo());
  } // _textcolor

  void _textbackground(WORD color) {
    ActualizaColores(ObtenerColorTextoActual(),color);
  } // _textbackground

  void _clrscr() { //Clon de _clrscr() LOCALIZADO (Solo limpia área de la ventana)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SMALL_RECT window = csbi.srWindow;
    DWORD escritos;
    WORD atributo = ObtenerColorTextoActual() | (ObtenerColorFondo() << 4);
    // Calcular ancho y alto de nuestra ventana
    int ancho = window.Right-3 - window.Left;

    window.Left += 2;
    // Limpiar fila por fila dentro de los límites coordenados
    for (short y = window.Top+2; y <= window.Bottom-1; ++y) {
        COORD inicio_fila = { window.Left, y };
        // Llena la fila actual con espacios en blanco
        FillConsoleOutputCharacter(hConsole, ' ', ancho, inicio_fila, &escritos);
        // Aplica el color de fondo actual a esos espacios en blanco
        FillConsoleOutputAttribute(hConsole,atributo,ancho,inicio_fila,&escritos);
    }
    _gotoxy(1, 1); // Igual a Borland, al limpiar, regresa a (1,1) relativo.
  } // _clrscr

  void _clreol() {
    COORD coord;
    DWORD escrito;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info; // Obtener la posición actual del cursor
    GetConsoleScreenBufferInfo(hStdOut, &info);

    coord = info.dwCursorPosition;
    // Calcular cuantos caracteres faltan para el final de la línea
    int longitud = info.dwSize.X - coord.X;
    // Sobrescribir con espacios y restaurar posición del cursor
    FillConsoleOutputCharacter(hStdOut, ' ', longitud, coord, &escrito);
    SetConsoleCursorPosition(hStdOut, coord);
  } // _clreol

  void MnsgBox(short x, short y, string mensaje, char alinea, short ancho = 0) {
    _gotoxy(x,y);
    if (alinea == 'i')
      cout << setw(ancho) << left << mensaje;
  	else
      cout << setw(ancho) << right << mensaje;
  } // MnsgBox

  void MnsgBox(short x, short y, string mensaje) {
    _gotoxy(x,y);
    cout << mensaje;
  } // MnsgBox

  void MnsgBox(short x, short y, short colorText, string mensaje) {
    _gotoxy(x,y);
    _textcolor(colorText);
    cout << mensaje;
  } // MnsgBox

  void Marco(short x1, short y1, short x2, short y2, short colorTexto) {
  	_textcolor(colorTexto);
    MnsgBox(x1,y1,"╔");
    MnsgBox(x2-1,y1,"╗");
    for (short i = 1; i < x2-x1-1; i++)
      MnsgBox(x1+i,y1,"═");
    for (short i = 1; i < y2-y1-5; i++) {
      MnsgBox(x1,y1+i,"║");
      MnsgBox(x2-1,y1+i,"║");
    }
    MnsgBox(x1,y2-5,"╚");
    MnsgBox(x2-1,y2-5,"╝");
    for (short i = 1; i < x2-x1-1; i++)
      MnsgBox(x1+i,y2-5,"═");
  } // Marco

  string Separador(int ancho = 60, char car = '-') {
    return string(ancho, car);
  } // Separador

  void OcultarCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(consoleHandle, &info);
    info.bVisible = FALSE; // Oculta el cursor
    SetConsoleCursorInfo(consoleHandle, &info);
  } // OcultarCursor

  void MostrarCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(consoleHandle, &info);
    info.bVisible = TRUE; // Muestra el cursor
    SetConsoleCursorInfo(consoleHandle, &info);
  } //MostrarCursor

	void Espera(short tiempo) {
		_textcolor(7);
  	MnsgBox(5,20,"Espere "+to_string(tiempo/1000)+" segundos");
		_textcolor(15);
    Sleep(tiempo);
    _gotoxy(5,20);
    _clreol();
  } // Espera

  void BarraTitulo() {
    SetConsoleTitleA("Home Banking Mesopotamia");
  } // BarraTitulo

  void Pausa(string mensg="") {
	  short i = 1;

    MnsgBox(12,8,mensg);
    GetAsyncKeyState(VK_SPACE);
    while(not GetAsyncKeyState(VK_SPACE)) {
			_textbackground(8);
  	  MnsgBox(5,17,i,"Oprima la tecla");
  	  setConsoleColor(AZUL,VERDE);
	    MnsgBox(21,17,AMARILLO+16*VERDE,"ESPACIO");
		  _textbackground(8);
	    MnsgBox(29,17,i++,"para continuar...");
  	  Sleep(1000);
	    if (i == 16)
		  	i = 1;
    }
  } // Pausa

void BloquearCambioTamaño() {
    HWND hwnd = GetConsoleWindow(); // Obtiene identificador de la ventana.
    // Obtiene los estilos actuales de la ventana
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX; // Quita botones de maximizar y redimensionar.
    style &= ~WS_SIZEBOX;
    SetWindowLong(hwnd, GWL_STYLE, style); // Aplica los nuevos estilos
} // BloquearCambioTamaño

  string Left(str25 cad, short cant) {
    return string(cad,cant);
  } // Left

  int MenuNavegar(char aMenu[][25],short lIni, short lFin, short cIni) {
    int Menu = lIni;

    OcultarCursor();
    MnsgBox(6,20,"Flecha ↑ ↓ para moverse sobre el menú");
    MnsgBox(3,21,"Tecla ESCAPE seleccionar la opción del menú");
    //GoToXY(cIni,lIni);
    _gotoxy(cIni,lIni);
    while(not GetAsyncKeyState(VK_ESCAPE)) {
      Sleep(200);
      if (GetAsyncKeyState(VK_UP)) {
        if (Menu == lIni) {
          _textcolor(15);
          _gotoxy(cIni,lIni);
          cout << aMenu[Menu-lIni];
          Menu = lFin;
          _textcolor(14);
          _gotoxy(cIni,lFin);
          cout << aMenu[Menu-lIni];
        }
        else {
          _textcolor(15);
          _gotoxy(cIni,_wherey());
          cout << aMenu[Menu-lIni];
          Menu--;
          _gotoxy(cIni,_wherey()-1);
          _textcolor(14);
          cout << aMenu[Menu-lIni];
        }
      }
      else if (GetAsyncKeyState(VK_DOWN)) {
        if (Menu == lFin) {
          _textcolor(15);
          _gotoxy(cIni,lFin);
          cout << aMenu[Menu-lIni];
          Menu = lIni;
          _gotoxy(cIni,lIni);
          _textcolor(14);
          cout << aMenu[Menu-lIni];
        }
        else {
          _textcolor(15);
          _gotoxy(cIni,_wherey());
          cout << aMenu[Menu-lIni];
          Menu++;
          _gotoxy(cIni,_wherey()+1);
          _textcolor(14);
          cout << aMenu[Menu-lIni];
        }
      }
    } // Fin While
    MostrarCursor();
    return Menu - lIni;
  } // MenuNavegar

} // Screen

namespace FechaHora {
  long GetTime(int &hh, int &mm, int &ss) {
    time_t     rawtime;
    record tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    hh = timeinfo->tm_hour;
    mm = timeinfo->tm_min;
		ss = timeinfo->tm_sec;
    return timeinfo->tm_hour * 10000 + timeinfo->tm_min * 100 + timeinfo->tm_sec;
  } // GetTime

  long GetDate(int &year, int &mes, int &dia, int &ds) {
    time_t     rawtime;
    record tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    year = 1900 + timeinfo->tm_year;
    mes  = 1 + timeinfo->tm_mon;
    dia  = timeinfo->tm_mday;
    ds   = 1 + timeinfo->tm_wday;
    return (1900 + timeinfo->tm_year) * 10000 + (1 + timeinfo->tm_mon) * 100
            + timeinfo->tm_mday;
  } // GetDate

  int FechaHoy() {
    int año, mes, dia, dsem;

    GetDate(año,mes,dia,dsem);
    cout << "Dia: " << setw(2) << dia << " Mes: " << setw(2) << mes
         << " Año:" << ' ' << setw(2) << año << endl;
	  return dia * 10000 + mes * 100 + año;
  } // FechaHoy

} // FechaHora
