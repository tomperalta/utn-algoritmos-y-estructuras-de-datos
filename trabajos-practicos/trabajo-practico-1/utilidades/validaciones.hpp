// ---------------------------------------------------------------------------
// validaciones.hpp
// Ingreso validado de datos por teclado y utilitarios de cadenas.
//
// Todo dato que ingresa el usuario pasa por una de estas funciones, que
// repiten el pedido con un ciclo 1-x (do..while) hasta obtener un valor
// correcto, tal como exige el enunciado. No se emplea memoria dinamica: las
// lecturas se hacen sobre arrays de char con cin.getline().
//
// Fin de la entrada
// -----------------
// Si la entrada se agota (EOF, tipico cuando el programa se ejecuta con la
// entrada redirigida desde un archivo de prueba), se enciende la bandera
// <<entradaAgotada>> y las funciones dejan de insistir devolviendo un valor
// neutro. Los menues consultan EntradaAgotada() para poder cerrar la sesion en
// forma ordenada en lugar de quedar en un ciclo infinito.
// ---------------------------------------------------------------------------
#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

namespace Validar {

  const short LARGO_BUFFER = 200;

  // La bandera <<entradaAgotada>> se define en compatibilidad.hpp, porque
  // tambien la necesita el modulo de consola.
  bool EntradaAgotada() {
    return entradaAgotada;
  } // EntradaAgotada

  // -------------------------------------------------------------------------
  // Descarta lo que quede en el buffer de entrada hasta el fin de linea.
  // -------------------------------------------------------------------------
  void VaciarEntrada() {
    if (cin.eof()) {
      entradaAgotada = true;
      return;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  } // VaciarEntrada

  // -------------------------------------------------------------------------
  // Elimina los espacios en blanco del comienzo y del final de la cadena.
  // -------------------------------------------------------------------------
  void Recortar(char cad[]) {
    short ini = 0, fin = (short) strlen(cad) - 1;

    while (cad[ini] == ' ' or cad[ini] == '\t')
      ini++;
    while (fin >= ini and (cad[fin] == ' ' or cad[fin] == '\t' or
                           cad[fin] == '\r' or cad[fin] == '\n'))
      fin--;
    short j = 0;
    for (short i = ini; i <= fin; i++)
      cad[j++] = cad[i];
    cad[j] = '\0';
  } // Recortar

  // -------------------------------------------------------------------------
  // Copia <<origen>> en <<dest>> truncando a lo sumo a <<largoMax>>
  // caracteres y asegurando el terminador nulo.
  // -------------------------------------------------------------------------
  void CopiarCadena(char dest[], const char origen[], short largoMax) {
    strncpy(dest, origen, largoMax);
    dest[largoMax] = '\0';
  } // CopiarCadena

  // -------------------------------------------------------------------------
  // Reemplaza las letras acentuadas de UTF-8 por su equivalente ASCII.
  //
  // Los registros de los archivos de texto deben ocupar SIEMPRE la misma
  // cantidad de bytes para poder acceder a ellos en forma aleatoria. Como en
  // UTF-8 una letra acentuada ocupa dos bytes, los detalles que se graban se
  // normalizan a ASCII: asi un caracter equivale a una columna y el formato
  // tabular queda perfectamente alineado.
  // -------------------------------------------------------------------------
  void NormalizarASCII(char cad[]) {
    const char *ACENTUADAS[] = {"á","é","í","ó","ú","ñ","ü",
                                "Á","É","Í","Ó","Ú","Ñ","Ü","°","º","ª"};
    const char  SIMPLES[]    =  {'a','e','i','o','u','n','u',
                                 'A','E','I','O','U','N','U','o','o','a'};
    const short CANT_ACENTOS = 17;
    char  salida[LARGO_BUFFER];
    short i = 0, j = 0;

    while (cad[i] != '\0' and j < LARGO_BUFFER - 1) {
      bool reemplazada = false;
      if ((cad[i] & 0x80) != 0) {                 // byte no ASCII
        for (short k = 0; k < CANT_ACENTOS and not reemplazada; k++) {
          short largo = (short) strlen(ACENTUADAS[k]);
          if (strncmp(cad + i, ACENTUADAS[k], largo) == 0) {
            salida[j++] = SIMPLES[k];
            i = (short)(i + largo);
            reemplazada = true;
          }
        }
        if (not reemplazada) {                    // otro caracter multibyte
          salida[j++] = '.';
          i++;
          while ((cad[i] & 0xC0) == 0x80)         // saltea continuaciones
            i++;
        }
      }
      else {
        salida[j++] = cad[i++];
      }
    }
    salida[j] = '\0';
    strcpy(cad, salida);
  } // NormalizarASCII

  bool SoloDigitos(const char cad[]) {
    if (cad[0] == '\0')
      return false;
    for (short i = 0; cad[i] != '\0'; i++)
      if (cad[i] < '0' or cad[i] > '9')
        return false;
    return true;
  } // SoloDigitos

  bool CBUValido(const char cad[]) {
    return (short) strlen(cad) == LARGO_CBU and SoloDigitos(cad);
  } // CBUValido

  bool CelularValido(const char cad[]) {
    short largo = (short) strlen(cad);

    return largo >= 10 and largo <= LARGO_CELULAR and SoloDigitos(cad);
  } // CelularValido

  // Validacion elemental de un correo: una sola arroba, con texto antes y un
  // punto despues.
  bool EmailValido(const char cad[]) {
    const char *arroba = strchr(cad, '@');

    if (arroba == NULL or arroba == cad)
      return false;
    if (strchr(arroba + 1, '@') != NULL)
      return false;
    const char *punto = strchr(arroba + 1, '.');
    return punto != NULL and punto[1] != '\0';
  } // EmailValido

  // -------------------------------------------------------------------------
  // Lee una linea completa de la entrada y la deja recortada en <<dest>>.
  // Devuelve false si la entrada se agoto.
  // -------------------------------------------------------------------------
  bool LeerLinea(char dest[], short largoMax) {
    char buffer[LARGO_BUFFER];

    dest[0] = '\0';
    if (entradaAgotada)
      return false;
    cin.getline(buffer, LARGO_BUFFER);
    if (cin.eof()) {
      entradaAgotada = true;
      if (buffer[0] == '\0')
        return false;
    }
    else if (cin.fail()) {          // la linea excedia el buffer
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    Recortar(buffer);
    CopiarCadena(dest, buffer, largoMax);
    return true;
  } // LeerLinea

  // -------------------------------------------------------------------------
  // Eco del dato leido.
  //
  // Cuando la entrada NO proviene de una consola (por ejemplo cuando se la
  // toma de un archivo de prueba) el dato que se lee no aparece en pantalla,
  // y los rotulos quedarian todos pegados uno detras del otro. En ese caso se
  // lo emite a continuacion del rotulo, de modo que la transcripcion de la
  // corrida se lea igual que la sesion interactiva.
  // -------------------------------------------------------------------------
  void EcoEntrada(const char texto[]) {
    if (not EntradaEsConsola())
      cout << texto << endl;
  } // EcoEntrada

  // -------------------------------------------------------------------------
  // Pide una cadena no vacia. Ciclo 1-x: insiste hasta que sea valida.
  //   normalizar = true -> se convierte a ASCII (para grabar en los archivos)
  // -------------------------------------------------------------------------
  void LeerCadena(const char rotulo[], char dest[], short largoMax,
                  bool normalizar) {
    bool valida = false;

    do {
      Screen::_textcolor(COL_ROTULO);
      cout << rotulo;
      Screen::_textcolor(COL_DATO);
      cout.flush();
      if (not LeerLinea(dest, largoMax)) {
        dest[0] = '\0';
        return;
      }
      if (normalizar) {
        NormalizarASCII(dest);
        dest[largoMax] = '\0';
      }
      EcoEntrada(dest);
      valida = dest[0] != '\0';
      if (not valida) {
        Screen::_textcolor(COL_ERROR);
        cout << "  El dato no puede quedar vacio. Reintente." << endl;
      }
    } while (not valida and not entradaAgotada);
    Screen::ColorNormal();
  } // LeerCadena

  // -------------------------------------------------------------------------
  // Pide un entero comprendido entre minimo y maximo (ciclo 1-x).
  // -------------------------------------------------------------------------
  long LeerEntero(const char rotulo[], long minimo, long maximo) {
    char  texto[LARGO_BUFFER];
    long  valor = minimo;
    bool  valido = false;

    do {
      Screen::_textcolor(COL_ROTULO);
      cout << rotulo;
      Screen::_textcolor(COL_DATO);
      cout.flush();
      if (not LeerLinea(texto, LARGO_BUFFER - 1))
        return minimo;
      EcoEntrada(texto);
      char *resto;
      valor = strtol(texto, &resto, 10);
      valido = texto[0] != '\0' and *resto == '\0' and
               valor >= minimo and valor <= maximo;
      if (not valido) {
        Screen::_textcolor(COL_ERROR);
        cout << "  Debe ingresar un numero entero entre " << minimo << " y "
             << maximo << ". Reintente." << endl;
      }
    } while (not valido and not entradaAgotada);
    Screen::ColorNormal();
    return valor;
  } // LeerEntero

  // -------------------------------------------------------------------------
  // Pide un importe real comprendido entre minimo y maximo (ciclo 1-x).
  // -------------------------------------------------------------------------
  double LeerReal(const char rotulo[], double minimo, double maximo) {
    char  texto[LARGO_BUFFER];
    double valor = minimo;
    bool  valido = false;

    do {
      Screen::_textcolor(COL_ROTULO);
      cout << rotulo;
      Screen::_textcolor(COL_DATO);
      cout.flush();
      if (not LeerLinea(texto, LARGO_BUFFER - 1))
        return minimo;
      EcoEntrada(texto);
      // Se admite la coma como separador decimal.
      for (short i = 0; texto[i] != '\0'; i++)
        if (texto[i] == ',')
          texto[i] = '.';
      char *resto;
      valor = strtod(texto, &resto);
      valido = texto[0] != '\0' and *resto == '\0' and
               valor >= minimo and valor <= maximo;
      if (not valido) {
        Screen::_textcolor(COL_ERROR);
        cout << fixed << setprecision(DECIMALES)
             << "  Debe ingresar un importe entre " << minimo << " y "
             << maximo << ". Reintente." << endl;
      }
    } while (not valido and not entradaAgotada);
    Screen::ColorNormal();
    return valor;
  } // LeerReal

  // -------------------------------------------------------------------------
  // Pide un caracter que pertenezca al conjunto <<validos>> (ciclo 1-x).
  // Devuelve el caracter en mayuscula.
  // -------------------------------------------------------------------------
  char LeerCaracter(const char rotulo[], const char validos[]) {
    char texto[LARGO_BUFFER];
    char car = validos[0];
    bool valido = false;

    do {
      Screen::_textcolor(COL_ROTULO);
      cout << rotulo;
      Screen::_textcolor(COL_DATO);
      cout.flush();
      if (not LeerLinea(texto, LARGO_BUFFER - 1))
        return validos[0];
      EcoEntrada(texto);
      car = (char) toupper(texto[0]);
      valido = strlen(texto) == 1 and strchr(validos, car) != NULL;
      if (not valido) {
        Screen::_textcolor(COL_ERROR);
        cout << "  Solo se admite uno de estos caracteres: " << validos
             << ". Reintente." << endl;
      }
    } while (not valido and not entradaAgotada);
    Screen::ColorNormal();
    return car;
  } // LeerCaracter

  // -------------------------------------------------------------------------
  // Pregunta por SI o por NO.
  //
  // Si la entrada se agoto NO hay respuesta del usuario, y en ese caso se
  // devuelve false: un fin de archivo nunca debe confirmar por accidente una
  // operacion irreversible (una transferencia, una baja de la agenda, la
  // sobreescritura del archivo de listados).
  // -------------------------------------------------------------------------
  bool LeerSiNo(const char rotulo[]) {
    char respuesta = LeerCaracter(rotulo, "SN");

    return not entradaAgotada and respuesta == 'S';
  } // LeerSiNo

  // -------------------------------------------------------------------------
  // Pide una fecha valida campo por campo (ciclo 1-x sobre la fecha completa).
  // -------------------------------------------------------------------------
  void LeerFecha(const char rotulo[], Fecha &f) {
    Screen::_textcolor(COL_TITULO);
    cout << rotulo << endl;
    Screen::ColorNormal();
    do {
      f.anio = (short) LeerEntero("    Anio (aaaa)....: ", ANIO_MINIMO,
                                  ANIO_MAXIMO);
      f.mes  = (short) LeerEntero("    Mes  (1-12)....: ", 1, 12);
      f.dia  = (short) LeerEntero("    Dia  (1-31)....: ", 1,
                                  FechaHora::DiasDelMes(f.mes, f.anio));
      if (not FechaHora::FechaValida(f) and not entradaAgotada) {
        Screen::_textcolor(COL_ERROR);
        cout << "  La fecha ingresada no existe. Reintente." << endl;
      }
    } while (not FechaHora::FechaValida(f) and not entradaAgotada);
  } // LeerFecha

  // -------------------------------------------------------------------------
  // Pide una clave mostrando un asterisco por caracter. Si la entrada no es
  // una consola, la lee en claro (modo prueba).
  // -------------------------------------------------------------------------
  void LeerClave(const char rotulo[], char dest[], short largoMax) {
    Screen::_textcolor(COL_ROTULO);
    cout << rotulo;
    Screen::ColorNormal();
    cout.flush();

    if (not EntradaEsConsola()) {
      LeerLinea(dest, largoMax);
      // Ni siquiera en la transcripcion de una corrida se ve la clave.
      for (short i = 0; dest[i] != '\0'; i++)
        cout << '*';
      cout << endl;
      return;
    }

    short cant = 0;
    int   car;

    do {
      car = LeerCaracterOculto();
      if (car == EOF) {
        entradaAgotada = true;
        break;
      }
      if (car == '\b' or car == 127) {          // retroceso
        if (cant > 0) {
          cant--;
          cout << "\b \b";
          cout.flush();
        }
      }
      else if (car != '\r' and car != '\n' and cant < largoMax) {
        dest[cant++] = (char) car;
        cout << '*';
        cout.flush();
      }
    } while (car != '\r' and car != '\n');
    dest[cant] = '\0';
    cout << endl;
  } // LeerClave

  // -------------------------------------------------------------------------
  // Espera que el usuario oprima ENTER para continuar.
  // -------------------------------------------------------------------------
  void Pausa(short x, short y) {
    char basura[LARGO_BUFFER];

    if (entradaAgotada)
      return;
    Screen::MnsgBox(x, y, COL_AVISO, "Oprima ENTER para continuar...");
    Screen::ColorNormal();
    cout.flush();
    LeerLinea(basura, LARGO_BUFFER - 1);
    if (not EntradaEsConsola())
      cout << endl;
  } // Pausa

} // namespace Validar

#endif // VALIDACIONES_HPP
