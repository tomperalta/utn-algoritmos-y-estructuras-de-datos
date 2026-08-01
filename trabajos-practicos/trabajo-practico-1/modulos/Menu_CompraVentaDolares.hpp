// ---------------------------------------------------------------------------
// Menu_CompraVentaDolares.hpp
// Nivel 2 - Compra y venta de dolares.
//
//   * Menu_ComprarDolares() debita pesos de la Caja de Ahorro y acredita los
//     dolares en la cuenta en moneda extranjera del usuario,
//   * Menu_VenderDolares()  hace exactamente lo inverso,
//   * la opcion "Ver cotizaciones del dia" es solo una consulta.
//
// Criterio de las cotizaciones (siempre desde el punto de vista del BANCO):
//   COTIZ_DOLAR_COMPRA -> precio al que el banco VENDE el dolar, es decir el
//                         que paga el cliente cuando COMPRA divisas.
//   COTIZ_DOLAR_VENTA  -> precio al que el banco COMPRA el dolar, es decir el
//                         que recibe el cliente cuando VENDE divisas.
// Los rotulos de pantalla explicitan las dos lecturas para que no quede lugar
// a confusion.
//
// La Caja de Ahorro en pesos se mueve siempre a traves de
// Operacion::RegistrarMovimientoCA(), que graba la componente en
// MovimientosCA.Txt y recalcula el saldo. La cuenta en dolares no tiene
// archivo de movimientos: su saldo se ajusta en memoria, sobre el array de
// cuentas del usuario.
// ---------------------------------------------------------------------------
#ifndef MENU_COMPRAVENTADOLARES_HPP
#define MENU_COMPRAVENTADOLARES_HPP

// ---------------------------------------------------------------------------
// ALCANCE DE LA PERSISTENCIA (limitacion conocida y deliberada)
//
// El enunciado autoriza a escribir en disco UNICAMENTE los tres archivos de
// movimientos: MovimientosCA.Txt, MovimientosTD.Txt y MovimientosTC.Txt. No
// hay entonces ningun archivo donde persistir el saldo de la Caja de Ahorro en
// dolares, que vive solamente en el array de cuentas, en memoria.
//
// Consecuencia: el movimiento en PESOS de una compra o de una venta de dolares
// si queda grabado en MovimientosCA.Txt y se recupera en la proxima ejecucion,
// mientras que el saldo en DOLARES vuelve al valor inicial de la muestra de
// datos. Los dos saldos son coherentes durante toda la sesion; dejan de serlo
// al reiniciar el programa.
//
// Se avisa al usuario en pantalla, para no dar por persistido algo que no lo
// esta. Lo mismo ocurre con la modificacion de la clave y con la creacion de
// cuentas nuevas, que tambien viven en el array de datos personales.
// ---------------------------------------------------------------------------

// ----- Geometria de las pantallas de este modulo -----
const short DOL_ANCHO_ROTULO = 37; // columna de los rotulos
const short DOL_ANCHO_MONTO  = 16; // columna de los importes

// Monto minimo negociable en una operacion de cambio: un dolar.
const double DOL_MIN_OPERACION = 1.00;

// ---------------------------------------------------------------------------
// Emite una linea <<rotulo + importe>> con la moneda indicada.
// ---------------------------------------------------------------------------
static void DOL_LineaImporte(const char rotulo[], const char moneda[],
                             double importe) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, DOL_ANCHO_ROTULO, 'i');
  Screen::_textcolor(COL_DATO);
  cout << moneda;
  Operacion::Importe(importe, DOL_ANCHO_MONTO);
  cout << endl;
  Screen::ColorNormal();
} // DOL_LineaImporte

// ---------------------------------------------------------------------------
// Emite una linea <<rotulo + valor alfanumerico>>.
// ---------------------------------------------------------------------------
static void DOL_LineaTexto(const char rotulo[], const char valor[]) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, DOL_ANCHO_ROTULO, 'i');
  Screen::_textcolor(COL_DATO);
  cout << valor << endl;
  Screen::ColorNormal();
} // DOL_LineaTexto

// ---------------------------------------------------------------------------
// Muestra las dos cotizaciones vigentes con rotulos que indican quien compra y
// quien vende en cada caso.
// ---------------------------------------------------------------------------
static void DOL_MostrarCotizaciones() {
  DOL_LineaImporte("Usted COMPRA dolares a (banco vende)", " $   ",
                   COTIZ_DOLAR_COMPRA);
  DOL_LineaImporte("Usted VENDE dolares a (banco compra)", " $   ",
                   COTIZ_DOLAR_VENTA);
} // DOL_MostrarCotizaciones

// ---------------------------------------------------------------------------
// Muestra los saldos de las dos cuentas involucradas en la operacion.
// ---------------------------------------------------------------------------
static void DOL_MostrarSaldos(DatosPers aDatosPers[], short nroUsu) {
  DOL_LineaImporte("Saldo Caja de Ahorro en pesos", " $   ",
                   Operacion::SaldoCA(aDatosPers, nroUsu));
  DOL_LineaImporte("Saldo Caja de Ahorro en dolares", " u$s ",
                   Operacion::SaldoDolares(aDatosPers, nroUsu));
} // DOL_MostrarSaldos

// ---------------------------------------------------------------------------
// Pantalla de consulta de las cotizaciones del dia, con la fecha y la hora en
// que fueron tomadas.
// ---------------------------------------------------------------------------
static void DOL_VerCotizaciones(DatosPers aDatosPers[], short nroUsu) {
  char  fecha[ANCHO_FECHA + 1];
  char  hora[9];
  Fecha hoy;

  Operacion::PantallaOperacion("COTIZACIONES DEL DIA", aDatosPers, nroUsu);

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);

  DOL_LineaTexto("Fecha de la cotizacion", fecha);
  DOL_LineaTexto("Hora de la cotizacion", hora);
  cout << endl;
  DOL_MostrarCotizaciones();
  cout << endl;
  DOL_MostrarSaldos(aDatosPers, nroUsu);

  Operacion::Aviso("Cotizaciones de referencia del Banco Mesopotamia.");
  Operacion::Continuar();
} // DOL_VerCotizaciones

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de compra y venta de dolares.
//
// Ciclo 1-x de construccion de menu: se repite hasta elegir Volver, cancelar
// con ESCAPE o agotarse la entrada.
// ---------------------------------------------------------------------------
void Menu_CompraVentaDolares(DatosPers aDatosPers[], short nroUsu) {
  const short CANT_OPC = 4;
  str40Opc aMenu[CANT_OPC] = {"Comprar dolares",
                              "Vender dolares",
                              "Ver cotizaciones del dia",
                              "Volver al menu principal"};
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("COMPRA / VENTA DE DOLARES", aDatosPers,
                                      nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {                        // seleccion multiple
      case 0 : Menu_ComprarDolares(aDatosPers, nroUsu);   break;
      case 1 : Menu_VenderDolares(aDatosPers, nroUsu);    break;
      case 2 : DOL_VerCotizaciones(aDatosPers, nroUsu);   break;
      default: volver = true;                // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_CompraVentaDolares

// ---------------------------------------------------------------------------
// Nivel 3 - Compra de dolares.
//
// El cliente indica cuantos dolares quiere comprar; el costo en pesos se
// debita de la Caja de Ahorro y los dolares se acreditan en la cuenta en
// moneda extranjera.
// ---------------------------------------------------------------------------
void Menu_ComprarDolares(DatosPers aDatosPers[], short nroUsu) {
  char  fecha[ANCHO_FECHA + 1];
  char  hora[9];
  Fecha hoy;
  double saldoPesos, maximo, monto, costo, posible;
  short iDol;
  bool  confirma;

  Operacion::PantallaOperacion("COMPRA DE DOLARES", aDatosPers, nroUsu);

  // La cuenta en dolares es el destino de la compra: sin ella no hay
  // operacion posible.
  iDol = Operacion::IndiceCuentaDolares(aDatosPers, nroUsu);
  if (iDol == NO_ENCONTRADO) {
    Operacion::Aviso("No posee una Caja de Ahorro en dolares.");
    cout << "  Puede crearla desde la opcion <<Crear nueva cuenta>>." << endl;
    Operacion::Continuar();
    return;
  }

  saldoPesos = Operacion::SaldoCA(aDatosPers, nroUsu);
  DOL_MostrarSaldos(aDatosPers, nroUsu);
  cout << endl;
  DOL_MostrarCotizaciones();
  cout << endl;

  // Con menos pesos que el valor de un dolar no hay compra posible: se corta
  // antes de pedir el monto, porque no existiria valor valido.
  if (saldoPesos < DOL_MIN_OPERACION * COTIZ_DOLAR_COMPRA) {
    Operacion::Error("Saldo en pesos insuficiente para comprar dolares.");
    Operacion::Continuar();
    return;
  }

  // El costo en pesos se graba en el archivo con formato 9(7).99: el monto en
  // dolares se limita para que el debito nunca exceda ese formato.
  maximo = Archivo::MAX_IMPORTE / COTIZ_DOLAR_COMPRA;
  monto  = Validar::LeerReal("  Monto a comprar u$s.: ", DOL_MIN_OPERACION,
                             maximo);
  if (Validar::EntradaAgotada())
    return;

  costo = monto * COTIZ_DOLAR_COMPRA;
  if (costo > saldoPesos) {
    posible = saldoPesos / COTIZ_DOLAR_COMPRA;
    Operacion::Error("Saldo en pesos insuficiente para esa compra.");
    DOL_LineaImporte("Costo de la compra", " $   ", costo);
    DOL_LineaImporte("Saldo disponible", " $   ", saldoPesos);
    DOL_LineaImporte("Con su saldo podria comprar hasta", " u$s ", posible);
    Operacion::Continuar();
    return;
  }

  DOL_LineaImporte("Monto a comprar", " u$s ", monto);
  DOL_LineaImporte("Cotizacion aplicada", " $   ", COTIZ_DOLAR_COMPRA);
  DOL_LineaImporte("Costo total a debitar", " $   ", costo);
  cout << endl;

  confirma = Validar::LeerSiNo("  Confirma la compra (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("Operacion cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  // Se autoriza por el importe en PESOS, que es el que sale de la cuenta.
  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, costo)) {
    Operacion::Continuar();
    return;
  }

  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA,
                                           "Compra de dolares", costo)) {
    Operacion::Error("No se pudo registrar el debito en la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }
  // La cuenta en dolares no tiene archivo de movimientos: se acredita en el
  // array de cuentas del usuario, y recien despues de grabado el debito.
  // El saldo en dolares se actualiza SOLO en memoria: no hay archivo donde
  // persistirlo (ver la nota del encabezado de este modulo).
  aDatosPers[nroUsu].aCuentas[iDol].saldo += monto;

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);

  Operacion::Exito("Compra de dolares realizada.");
  Screen::_textcolor(COL_TITULO);
  cout << endl << "  COMPROBANTE DE COMPRA DE DOLARES" << endl << endl;
  Screen::ColorNormal();
  DOL_LineaTexto("Titular", aDatosPers[nroUsu].apellidoNombre);
  DOL_LineaTexto("Fecha de la operacion", fecha);
  DOL_LineaTexto("Hora de la operacion", hora);
  DOL_LineaTexto("Cuenta debitada", aDatosPers[nroUsu].nroCtaCA);
  DOL_LineaTexto("Cuenta acreditada",
                 aDatosPers[nroUsu].aCuentas[iDol].nroCta);
  DOL_LineaImporte("Dolares comprados", " u$s ", monto);
  DOL_LineaImporte("Cotizacion aplicada", " $   ", COTIZ_DOLAR_COMPRA);
  DOL_LineaImporte("Total debitado", " $   ", costo);
  cout << endl;
  DOL_MostrarSaldos(aDatosPers, nroUsu);
  Operacion::Aviso("El saldo en dolares se conserva durante esta sesion: el enunciado\n          solo admite grabar en disco los archivos de movimientos.");
  Operacion::Continuar();
} // Menu_ComprarDolares

// ---------------------------------------------------------------------------
// Nivel 3 - Venta de dolares.
//
// Operacion inversa a la anterior: se descuentan los dolares de la cuenta en
// moneda extranjera y se acredita el producido en pesos en la Caja de Ahorro.
// ---------------------------------------------------------------------------
void Menu_VenderDolares(DatosPers aDatosPers[], short nroUsu) {
  char  fecha[ANCHO_FECHA + 1];
  char  hora[9];
  Fecha hoy;
  double saldoDolares, maximo, tope, monto, ingreso;
  short iDol;
  bool  confirma;

  Operacion::PantallaOperacion("VENTA DE DOLARES", aDatosPers, nroUsu);

  iDol = Operacion::IndiceCuentaDolares(aDatosPers, nroUsu);
  if (iDol == NO_ENCONTRADO) {
    Operacion::Aviso("No posee una Caja de Ahorro en dolares.");
    cout << "  Puede crearla desde la opcion <<Crear nueva cuenta>>." << endl;
    Operacion::Continuar();
    return;
  }

  saldoDolares = Operacion::SaldoDolares(aDatosPers, nroUsu);
  DOL_MostrarSaldos(aDatosPers, nroUsu);
  cout << endl;
  DOL_MostrarCotizaciones();
  cout << endl;

  if (saldoDolares < DOL_MIN_OPERACION) {
    Operacion::Error("No tiene dolares suficientes para vender.");
    Operacion::Continuar();
    return;
  }

  // Se vende como maximo lo que hay en la cuenta y, ademas, lo que permita el
  // formato 9(7).99 con el que se graba la acreditacion en pesos.
  tope   = Archivo::MAX_IMPORTE / COTIZ_DOLAR_VENTA;
  maximo = saldoDolares < tope ? saldoDolares : tope;
  monto  = Validar::LeerReal("  Monto a vender u$s..: ", DOL_MIN_OPERACION,
                             maximo);
  if (Validar::EntradaAgotada())
    return;

  // Resguardo equivalente al de la compra: nunca se vende mas de lo que hay.
  if (monto > saldoDolares) {
    Operacion::Error("El monto supera el saldo de la cuenta en dolares.");
    Operacion::Continuar();
    return;
  }

  ingreso = monto * COTIZ_DOLAR_VENTA;
  DOL_LineaImporte("Monto a vender", " u$s ", monto);
  DOL_LineaImporte("Cotizacion aplicada", " $   ", COTIZ_DOLAR_VENTA);
  DOL_LineaImporte("Total a acreditar", " $   ", ingreso);
  cout << endl;

  confirma = Validar::LeerSiNo("  Confirma la venta (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("Operacion cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, ingreso)) {
    Operacion::Continuar();
    return;
  }

  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_ACREDITA,
                                           "Venta de dolares", ingreso)) {
    Operacion::Error("No se pudo registrar la acreditacion en pesos.");
    Operacion::Continuar();
    return;
  }
  // Igual que en la compra: el saldo en dolares solo vive en memoria.
  aDatosPers[nroUsu].aCuentas[iDol].saldo -= monto;

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);

  Operacion::Exito("Venta de dolares realizada.");
  Screen::_textcolor(COL_TITULO);
  cout << endl << "  COMPROBANTE DE VENTA DE DOLARES" << endl << endl;
  Screen::ColorNormal();
  DOL_LineaTexto("Titular", aDatosPers[nroUsu].apellidoNombre);
  DOL_LineaTexto("Fecha de la operacion", fecha);
  DOL_LineaTexto("Hora de la operacion", hora);
  DOL_LineaTexto("Cuenta debitada",
                 aDatosPers[nroUsu].aCuentas[iDol].nroCta);
  DOL_LineaTexto("Cuenta acreditada", aDatosPers[nroUsu].nroCtaCA);
  DOL_LineaImporte("Dolares vendidos", " u$s ", monto);
  DOL_LineaImporte("Cotizacion aplicada", " $   ", COTIZ_DOLAR_VENTA);
  DOL_LineaImporte("Total acreditado", " $   ", ingreso);
  cout << endl;
  DOL_MostrarSaldos(aDatosPers, nroUsu);
  Operacion::Aviso("El saldo en dolares se conserva durante esta sesion: el enunciado\n          solo admite grabar en disco los archivos de movimientos.");
  Operacion::Continuar();
} // Menu_VenderDolares

#endif // MENU_COMPRAVENTADOLARES_HPP
