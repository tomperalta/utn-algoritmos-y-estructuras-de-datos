// ---------------------------------------------------------------------------
// Menu_Recargar.hpp
// Nivel 2 - Recargas.
//
//   * Menu_RecargaCelular() carga credito en una linea de telefono celular,
//     que puede ser la del propio usuario o cualquier otra,
//   * Menu_RecargaSube()    carga saldo en una tarjeta SUBE de 16 digitos.
//
// Las dos operaciones son pagos: debitan la Caja de Ahorro en pesos generando
// una nueva componente en MovimientosCA.Txt a traves de
// Operacion::RegistrarMovimientoCA().
//
// Los numeros (celular y tarjeta) se leen en un buffer holgado y recien
// despues se validan. Si se los leyera en un buffer del largo exacto, una
// entrada mas larga quedaria TRUNCADA y pasaria la validacion siendo
// incorrecta.
// ---------------------------------------------------------------------------
#ifndef MENU_RECARGAR_HPP
#define MENU_RECARGAR_HPP

// ----- Geometria de las pantallas de este modulo -----
const short REC_ANCHO_ROTULO = 30; // columna de los rotulos
const short REC_ANCHO_MONTO  = 16; // columna de los importes

// La tarjeta SUBE se identifica con 16 digitos, sin espacios ni guiones.
const short REC_LARGO_SUBE = 16;

// Rango del numero de comprobante que se emite al azar
const long REC_CPBTE_MINIMO = 10000000;
const long REC_CPBTE_MAXIMO = 99999999;

// ---------------------------------------------------------------------------
// Emite una linea <<rotulo + importe>> en pesos.
// ---------------------------------------------------------------------------
static void REC_LineaImporte(const char rotulo[], double importe) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, REC_ANCHO_ROTULO, 'i');
  Screen::_textcolor(COL_DATO);
  cout << " $ ";
  Operacion::Importe(importe, REC_ANCHO_MONTO);
  cout << endl;
  Screen::ColorNormal();
} // REC_LineaImporte

// ---------------------------------------------------------------------------
// Emite una linea <<rotulo + valor alfanumerico>>.
// ---------------------------------------------------------------------------
static void REC_LineaTexto(const char rotulo[], const char valor[]) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, REC_ANCHO_ROTULO, 'i');
  Screen::_textcolor(COL_DATO);
  cout << " " << valor << endl;
  Screen::ColorNormal();
} // REC_LineaTexto

// ---------------------------------------------------------------------------
// Encabezado del comprobante que emiten las dos recargas: titular, cuenta
// debitada, fecha, hora y numero de comprobante.
// ---------------------------------------------------------------------------
static void REC_Comprobante(DatosPers aDatosPers[], short nroUsu,
                            const char titulo[]) {
  char fecha[ANCHO_FECHA + 1];
  char hora[9];
  char texto[LARGO_NOMBRE + 1];
  Fecha hoy;
  long  nroCpbte;

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);
  nroCpbte = rand() % (REC_CPBTE_MAXIMO - REC_CPBTE_MINIMO + 1) +
             REC_CPBTE_MINIMO;

  Screen::_textcolor(COL_TITULO);
  cout << endl << "  " << titulo << endl << endl;
  Screen::ColorNormal();
  snprintf(texto, sizeof texto, "%ld", nroCpbte);
  REC_LineaTexto("Nro. de comprobante.........:", texto);
  REC_LineaTexto("Titular.....................:",
                 aDatosPers[nroUsu].apellidoNombre);
  REC_LineaTexto("Cuenta debitada.............:", aDatosPers[nroUsu].nroCtaCA);
  REC_LineaTexto("Fecha.......................:", fecha);
  REC_LineaTexto("Hora........................:", hora);
} // REC_Comprobante

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de recargas.
//
// Ciclo 1-x de construccion de menu: se repite hasta elegir Volver, cancelar
// con ESCAPE o agotarse la entrada.
// ---------------------------------------------------------------------------
void Menu_Recargar(DatosPers aDatosPers[], short nroUsu) {
  const short CANT_OPC = 3;
  str40Opc aMenu[CANT_OPC] = {"Recargar credito de celular",
                              "Recargar tarjeta SUBE",
                              "Volver al menu principal"};
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("RECARGAS", aDatosPers, nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {                        // seleccion multiple
      case 0 : Menu_RecargaCelular(aDatosPers, nroUsu);  break;
      case 1 : Menu_RecargaSube(aDatosPers, nroUsu);     break;
      default: volver = true;                // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_Recargar

// ---------------------------------------------------------------------------
// Nivel 3 - Recarga de credito de celular.
//
// Se ofrece recargar la linea registrada del usuario o cualquier otra. El
// numero ajeno se valida con un ciclo 1-x, que corta tambien cuando se agota
// la entrada para no quedar en un ciclo infinito.
// ---------------------------------------------------------------------------
void Menu_RecargaCelular(DatosPers aDatosPers[], short nroUsu) {
  str25 detalle;
  str40 celular;
  double saldo, importe;
  bool  propio, valido = false, confirma;

  Operacion::PantallaOperacion("RECARGA DE CREDITO DE CELULAR", aDatosPers,
                               nroUsu);

  saldo = Operacion::SaldoCA(aDatosPers, nroUsu);
  REC_LineaImporte("Saldo Caja de Ahorro........:", saldo);
  REC_LineaTexto("Su linea registrada.........:", aDatosPers[nroUsu].celular);
  REC_LineaImporte("Recarga minima..............:", MIN_RECARGA_CEL);
  REC_LineaImporte("Recarga maxima..............:", MAX_RECARGA_CEL);
  cout << endl;

  propio = Validar::LeerSiNo("  Recargar su propia linea (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;

  if (propio)
    Validar::CopiarCadena(celular, aDatosPers[nroUsu].celular, LARGO_NOMBRE);
  else {
    // Ciclo 1-x de validacion de dato: insiste hasta obtener un celular
    // valido (de 10 a 15 digitos) o hasta que se agote la entrada.
    do {
      Validar::LeerCadena("  Numero de celular a recargar: ", celular,
                          LARGO_NOMBRE, false);
      valido = Validar::CelularValido(celular);
      if (not valido and not Validar::EntradaAgotada())
        Operacion::Error("El celular debe tener de 10 a 15 digitos, sin "
                         "espacios ni guiones.");
    } while (not valido and not Validar::EntradaAgotada());
    if (not valido)
      return;
  }

  importe = Validar::LeerReal("  Importe a recargar $.......: ",
                              MIN_RECARGA_CEL, MAX_RECARGA_CEL);
  if (Validar::EntradaAgotada())
    return;
  if (importe > saldo) {
    Operacion::Error("Saldo insuficiente en la Caja de Ahorro.");
    REC_LineaImporte("Importe solicitado..........:", importe);
    REC_LineaImporte("Saldo disponible............:", saldo);
    Operacion::Continuar();
    return;
  }

  cout << endl;
  REC_LineaTexto("Linea a recargar............:", celular);
  REC_LineaImporte("Importe de la recarga.......:", importe);
  cout << endl;
  confirma = Validar::LeerSiNo("  Confirma la recarga (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("Operacion cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  // El detalle que se graba en el archivo ocupa a lo sumo LARGO_DETALLE
  // caracteres: snprintf recorta solo el sobrante del numero de linea.
  // "Recarga Cel " ocupa 12 de los 25 caracteres del detalle: un numero de
  // mas de 13 digitos se truncaria justo por donde identifica a la linea. Se
  // graban entonces sus ultimos digitos.
  if ((short) strlen(celular) <= LARGO_DETALLE - 12)
    snprintf(detalle, sizeof detalle, "Recarga Cel %s", celular);
  else
    snprintf(detalle, sizeof detalle, "Recarga Cel ..%s",
             celular + strlen(celular) - (LARGO_DETALLE - 14));

  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA,
                                           detalle, importe)) {
    Operacion::Error("No se pudo registrar el debito en la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  Operacion::Exito("Recarga de celular realizada.");
  REC_Comprobante(aDatosPers, nroUsu, "COMPROBANTE DE RECARGA DE CELULAR");
  REC_LineaTexto("Linea recargada.............:", celular);
  REC_LineaTexto("Detalle del movimiento......:", detalle);
  REC_LineaImporte("Importe debitado............:", importe);
  REC_LineaImporte("Nuevo saldo Caja de Ahorro..:",
                   Operacion::SaldoCA(aDatosPers, nroUsu));
  Operacion::Aviso("El credito se acredita en la linea en pocos minutos.");
  Operacion::Continuar();
} // Menu_RecargaCelular

// ---------------------------------------------------------------------------
// Nivel 3 - Recarga de tarjeta SUBE.
//
// El numero de la tarjeta debe tener exactamente REC_LARGO_SUBE digitos: se
// valida con un ciclo 1-x que corta tambien al agotarse la entrada.
// ---------------------------------------------------------------------------
void Menu_RecargaSube(DatosPers aDatosPers[], short nroUsu) {
  str40 tarjeta;
  double saldo, importe;
  bool  valido = false, confirma;

  Operacion::PantallaOperacion("RECARGA DE TARJETA SUBE", aDatosPers, nroUsu);

  saldo = Operacion::SaldoCA(aDatosPers, nroUsu);
  REC_LineaImporte("Saldo Caja de Ahorro........:", saldo);
  REC_LineaImporte("Recarga minima..............:", MIN_RECARGA_SUBE);
  REC_LineaImporte("Recarga maxima..............:", MAX_RECARGA_SUBE);
  cout << endl;

  do {
    Validar::LeerCadena("  Nro. de tarjeta SUBE (16 digitos): ", tarjeta,
                        LARGO_NOMBRE, false);
    valido = (short) strlen(tarjeta) == REC_LARGO_SUBE and
             Validar::SoloDigitos(tarjeta);
    if (not valido and not Validar::EntradaAgotada())
      Operacion::Error("La tarjeta SUBE debe tener 16 digitos, sin espacios "
                       "ni guiones.");
  } while (not valido and not Validar::EntradaAgotada());
  if (not valido)
    return;

  importe = Validar::LeerReal("  Importe a recargar $.......: ",
                              MIN_RECARGA_SUBE, MAX_RECARGA_SUBE);
  if (Validar::EntradaAgotada())
    return;
  if (importe > saldo) {
    Operacion::Error("Saldo insuficiente en la Caja de Ahorro.");
    REC_LineaImporte("Importe solicitado..........:", importe);
    REC_LineaImporte("Saldo disponible............:", saldo);
    Operacion::Continuar();
    return;
  }

  cout << endl;
  REC_LineaTexto("Tarjeta SUBE a recargar.....:", tarjeta);
  REC_LineaImporte("Importe de la recarga.......:", importe);
  cout << endl;
  confirma = Validar::LeerSiNo("  Confirma la recarga (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("Operacion cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA,
                                           "Recarga SUBE", importe)) {
    Operacion::Error("No se pudo registrar el debito en la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  Operacion::Exito("Recarga de tarjeta SUBE realizada.");
  REC_Comprobante(aDatosPers, nroUsu, "COMPROBANTE DE RECARGA SUBE");
  REC_LineaTexto("Tarjeta SUBE recargada......:", tarjeta);
  REC_LineaTexto("Detalle del movimiento......:", "Recarga SUBE");
  REC_LineaImporte("Importe debitado............:", importe);
  REC_LineaImporte("Nuevo saldo Caja de Ahorro..:",
                   Operacion::SaldoCA(aDatosPers, nroUsu));
  Operacion::Aviso("Acredite la carga en una terminal SUBE o con NFC.");
  Operacion::Continuar();
} // Menu_RecargaSube

#endif // MENU_RECARGAR_HPP
