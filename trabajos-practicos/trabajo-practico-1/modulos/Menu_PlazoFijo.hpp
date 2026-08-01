// ---------------------------------------------------------------------------
// Menu_PlazoFijo.hpp
// Nivel 2 - Plazo Fijo e Inversiones.
//
// Reune las tres operaciones de inversion que ofrece el sitio:
//
//   * Menu_SimulacionPF()   simula un plazo fijo SIN tocar ningun saldo,
//   * Menu_InversionPF()    constituye el plazo fijo debitando la Caja de
//                           Ahorro y emite el certificado,
//   * Menu_FondoInversion() suscribe un Fondo Comun de Inversion (FCI) y
//                           estima su rendimiento a 30 dias.
//
// El interes se calcula siempre con la formula del INTERES SIMPLE
//
//        interes = capital * (TNA / 100) * dias / DIAS_DEL_ANIO
//
// que es la que aplican los bancos a los plazos fijos tradicionales de menos
// de un anio: durante el plazo los intereses NO se capitalizan.
//
// La simulacion y la inversion piden exactamente los mismos datos; la unica
// diferencia es que la inversion valida el saldo disponible, exige
// confirmacion y token, y graba el movimiento en MovimientosCA.Txt.
// ---------------------------------------------------------------------------
#ifndef MENU_PLAZOFIJO_HPP
#define MENU_PLAZOFIJO_HPP

// ----- Geometria de las pantallas de este modulo -----
const short PF_ANCHO_ROTULO = 29; // columna de los rotulos
const short PF_ANCHO_MONTO  = 16; // columna de los importes

// Divisor que convierte la Tasa Nominal Anual (expresada en %) a tanto por uno
const double PF_PORCENTAJE = 100.00;

// Plazos del cuadro comparativo: se recorren con una repeticion exacta, porque
// la cantidad de plazos a comparar se conoce de antemano.
const short PF_CANT_PLAZOS = 5;
const short PF_PLAZOS[PF_CANT_PLAZOS] = {30, 60, 90, 180, 365};

// Plazo con el que se estima el rendimiento del Fondo Comun de Inversion
const short PF_DIAS_FCI = 30;

// Rango del numero de certificado del plazo fijo (se genera al azar)
const long PF_CERT_MINIMO = 100000;
const long PF_CERT_MAXIMO = 999999;

// Anchos de las columnas del cuadro comparativo
const short PF_ANC_PLAZO   =  8;
const short PF_ANC_VTO     = 14;
const short PF_ANC_INTERES = 18;
const short PF_ANC_TOTAL   = 20;

// ---------------------------------------------------------------------------
// Interes SIMPLE que devenga <<capital>> a la tasa nominal anual <<tasaAnual>>
// durante <<dias>> dias. La multiplicacion por el capital (double) fuerza la
// aritmetica real, de modo que la division por DIAS_DEL_ANIO no se trunca.
// ---------------------------------------------------------------------------
static double PF_InteresSimple(double capital, double tasaAnual, short dias) {
  return capital * (tasaAnual / PF_PORCENTAJE) * dias / DIAS_DEL_ANIO;
} // PF_InteresSimple

// ---------------------------------------------------------------------------
// Emite una linea <<rotulo + importe>> con la moneda indicada.
// ---------------------------------------------------------------------------
static void PF_LineaImporte(const char rotulo[], const char moneda[],
                            double importe) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, PF_ANCHO_ROTULO, 'i');
  Screen::_textcolor(COL_DATO);
  cout << moneda;
  Operacion::Importe(importe, PF_ANCHO_MONTO);
  cout << endl;
  Screen::ColorNormal();
} // PF_LineaImporte

// ---------------------------------------------------------------------------
// Emite una linea <<rotulo + valor alfanumerico>>.
// ---------------------------------------------------------------------------
static void PF_LineaTexto(const char rotulo[], const char valor[]) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, PF_ANCHO_ROTULO, 'i');
  Screen::_textcolor(COL_DATO);
  cout << valor << endl;
  Screen::ColorNormal();
} // PF_LineaTexto

// ---------------------------------------------------------------------------
// Cuerpo del detalle de un plazo fijo, comun a la simulacion y al certificado
// de la inversion: capital, tasa, plazo, vencimiento, interes y total.
// ---------------------------------------------------------------------------
static void PF_Detalle(double capital, double tasaAnual, short dias, Fecha vto,
                       double interes, double total) {
  char texto[LARGO_NOMBRE + 1];
  char fechaVto[ANCHO_FECHA + 1];

  PF_LineaImporte("Capital invertido.........:", " $ ", capital);
  snprintf(texto, sizeof texto, "%.*f %%", DECIMALES, tasaAnual);
  PF_LineaTexto("Tasa Nominal Anual (TNA)..:", texto);
  snprintf(texto, sizeof texto, "%d dias", dias);
  PF_LineaTexto("Plazo.....................:", texto);
  FechaHora::FormatearFechaCeros(vto, fechaVto);
  PF_LineaTexto("Fecha de vencimiento......:", fechaVto);
  PF_LineaImporte("Interes a cobrar..........:", " $ ", interes);
  PF_LineaImporte("Total a cobrar............:", " $ ", total);
} // PF_Detalle

// ---------------------------------------------------------------------------
// Cuadro comparativo del mismo capital colocado a los distintos plazos de
// PF_PLAZOS. Se recorre el array con un for (repeticion exacta).
// ---------------------------------------------------------------------------
static void PF_Comparativo(double capital, double tasaAnual) {
  char  plazo[PF_ANC_PLAZO + 1];
  char  fechaVto[ANCHO_FECHA + 1];
  Fecha hoy, vto;
  double interes, total;

  FechaHora::FechaDeHoy(hoy);

  Screen::_textcolor(COL_TITULO);
  cout << endl << "  El mismo capital a distintos plazos:" << endl << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto("Plazo", PF_ANC_PLAZO, 'd');
  Screen::Texto("Vencimiento", PF_ANC_VTO, 'd');
  Screen::Texto("Interes", PF_ANC_INTERES, 'd');
  Screen::Texto("Total a cobrar", PF_ANC_TOTAL, 'd');
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(PF_ANC_PLAZO + PF_ANC_VTO + PF_ANC_INTERES + PF_ANC_TOTAL,
                    '-');
  cout << endl;
  Screen::ColorNormal();

  for (short i = 0; i < PF_CANT_PLAZOS; i++) {
    interes = PF_InteresSimple(capital, tasaAnual, PF_PLAZOS[i]);
    total   = capital + interes;
    FechaHora::SumarDias(hoy, PF_PLAZOS[i], vto);
    FechaHora::FormatearFechaCeros(vto, fechaVto);
    snprintf(plazo, sizeof plazo, "%d", PF_PLAZOS[i]);
    cout << "  ";
    Screen::Texto(plazo, PF_ANC_PLAZO, 'd');
    Screen::Texto(fechaVto, PF_ANC_VTO, 'd');
    Operacion::Importe(interes, PF_ANC_INTERES);
    Operacion::Importe(total, PF_ANC_TOTAL);
    cout << endl;
  }

  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(PF_ANC_PLAZO + PF_ANC_VTO + PF_ANC_INTERES + PF_ANC_TOTAL,
                    '-');
  cout << endl;
  Screen::ColorNormal();
} // PF_Comparativo

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de Plazo Fijo e Inversiones.
//
// El ciclo es 1-x porque se trata de la construccion de un menu: se repite
// hasta que se elija Volver, se cancele con ESCAPE o se agote la entrada.
// ---------------------------------------------------------------------------
void Menu_PlazoFijo(DatosPers aDatosPers[], short nroUsu) {
  const short CANT_OPC = 4;
  str40Opc aMenu[CANT_OPC] = {"Simulacion de Plazo Fijo",
                              "Constituir un Plazo Fijo",
                              "Fondo Comun de Inversion",
                              "Volver al menu principal"};
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("PLAZO FIJO E INVERSIONES", aDatosPers,
                                      nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {                        // seleccion multiple
      case 0 : Menu_SimulacionPF(aDatosPers, nroUsu);    break;
      case 1 : Menu_InversionPF(aDatosPers, nroUsu);     break;
      case 2 : Menu_FondoInversion(aDatosPers, nroUsu);  break;
      default: volver = true;                // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_PlazoFijo

// ---------------------------------------------------------------------------
// Nivel 3 - Simulacion de un plazo fijo.
//
// Es una consulta: NO genera movimientos ni modifica saldo alguno, de modo que
// no pide confirmacion ni token.
// ---------------------------------------------------------------------------
void Menu_SimulacionPF(DatosPers aDatosPers[], short nroUsu) {
  char  texto[LARGO_NOMBRE + 1];
  char  fechaHoy[ANCHO_FECHA + 1];
  Fecha hoy, vencimiento;
  double capital, interes, total, tasa;
  short dias;

  Operacion::PantallaOperacion("SIMULACION DE PLAZO FIJO", aDatosPers, nroUsu);

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fechaHoy);
  snprintf(texto, sizeof texto, "%.*f %%", DECIMALES, TASA_PF_ANUAL);
  PF_LineaTexto("Tasa vigente del banco....:", texto);
  PF_LineaTexto("Fecha de la simulacion....:", fechaHoy);
  PF_LineaImporte("Capital minimo exigido....:", " $ ", MIN_PLAZO_FIJO);
  cout << endl;

  // Todo dato ingresado pasa por el modulo Validar, que insiste con un ciclo
  // 1-x hasta obtener un valor dentro del rango permitido.
  capital = Validar::LeerReal("  Capital a invertir $.....: ", MIN_PLAZO_FIJO,
                              Archivo::MAX_IMPORTE);
  if (Validar::EntradaAgotada())
    return;
  dias = (short) Validar::LeerEntero("  Plazo en dias (30 a 365).: ",
                                     DIAS_PF_MINIMO, DIAS_PF_MAXIMO);
  if (Validar::EntradaAgotada())
    return;

  // El enunciado pide que en la simulacion se ingresen el capital, el tiempo
  // Y LA TASA. Se ofrece la tasa vigente del banco como valor por omision, y
  // se admite simular con otra para poder comparar escenarios.
  if (Validar::LeerSiNo("  Simular con la tasa vigente del banco (S/N)?: "))
    tasa = TASA_PF_ANUAL;
  else
    tasa = Validar::LeerReal("  Tasa Nominal Anual % .....: ", 0.01, 999.99);
  if (Validar::EntradaAgotada())
    return;

  FechaHora::SumarDias(hoy, dias, vencimiento);
  interes = PF_InteresSimple(capital, tasa, dias);
  total   = capital + interes;

  Screen::_textcolor(COL_TITULO);
  cout << endl << "  RESULTADO DE LA SIMULACION" << endl << endl;
  Screen::ColorNormal();
  PF_LineaTexto("Fecha de constitucion.....:", fechaHoy);
  PF_Detalle(capital, tasa, dias, vencimiento, interes, total);

  PF_Comparativo(capital, tasa);   // con la MISMA tasa de la simulacion

  Operacion::Aviso("Simulacion informativa: no modifica el saldo de sus "
                   "cuentas.");
  Operacion::Continuar();
} // Menu_SimulacionPF

// ---------------------------------------------------------------------------
// Nivel 3 - Constitucion de un plazo fijo.
//
// Debita el capital de la Caja de Ahorro en pesos generando un nuevo
// movimiento en MovimientosCA.Txt, y emite el certificado de la colocacion.
// ---------------------------------------------------------------------------
void Menu_InversionPF(DatosPers aDatosPers[], short nroUsu) {
  char  texto[LARGO_NOMBRE + 1];
  char  fechaHoy[ANCHO_FECHA + 1];
  Fecha hoy, vencimiento;
  double saldo, maximo, capital, interes, total;
  long  nroCertificado;
  short dias;
  bool  confirma;

  Operacion::PantallaOperacion("CONSTITUIR UN PLAZO FIJO", aDatosPers, nroUsu);

  saldo = Operacion::SaldoCA(aDatosPers, nroUsu);
  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fechaHoy);
  PF_LineaImporte("Saldo Caja de Ahorro......:", " $ ", saldo);
  snprintf(texto, sizeof texto, "%.*f %%", DECIMALES, TASA_PF_ANUAL);
  PF_LineaTexto("Tasa vigente del banco....:", texto);
  PF_LineaImporte("Capital minimo exigido....:", " $ ", MIN_PLAZO_FIJO);
  cout << endl;

  // Se corta ANTES de pedir el capital: si el saldo no alcanza el minimo, el
  // rango de validacion quedaria vacio y no habria importe posible de aceptar.
  if (saldo < MIN_PLAZO_FIJO) {
    Operacion::Error("Saldo insuficiente para constituir un plazo fijo.");
    Operacion::Continuar();
    return;
  }

  // El capital se limita al saldo disponible y, ademas, al maximo que
  // admite el formato 9(7).99 con el que se graba el importe en el archivo.
  maximo = saldo < Archivo::MAX_IMPORTE ? saldo : Archivo::MAX_IMPORTE;
  capital = Validar::LeerReal("  Capital a invertir $.....: ", MIN_PLAZO_FIJO,
                              maximo);
  if (Validar::EntradaAgotada())
    return;
  dias = (short) Validar::LeerEntero("  Plazo en dias (30 a 365).: ",
                                     DIAS_PF_MINIMO, DIAS_PF_MAXIMO);
  if (Validar::EntradaAgotada())
    return;

  // Resguardo: el saldo pudo quedar tomado antes de leer el capital.
  if (capital > saldo) {
    Operacion::Error("El capital supera el saldo de la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  FechaHora::SumarDias(hoy, dias, vencimiento);
  interes = PF_InteresSimple(capital, TASA_PF_ANUAL, dias);
  total   = capital + interes;

  Screen::_textcolor(COL_TITULO);
  cout << endl << "  CONDICIONES DE LA COLOCACION" << endl << endl;
  Screen::ColorNormal();
  PF_LineaTexto("Fecha de constitucion.....:", fechaHoy);
  PF_Detalle(capital, TASA_PF_ANUAL, dias, vencimiento, interes, total);
  cout << endl;

  confirma = Validar::LeerSiNo("  Confirma la operacion (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("Operacion cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  // Pide token solamente cuando el capital supera TOPE_SIN_TOKEN.
  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, capital)) {
    Operacion::Continuar();
    return;
  }

  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA,
                                           "Inversion Plazo Fijo", capital)) {
    Operacion::Error("No se pudo registrar el debito en la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  nroCertificado = rand() % (PF_CERT_MAXIMO - PF_CERT_MINIMO + 1) +
                   PF_CERT_MINIMO;

  Operacion::Exito("Plazo fijo constituido correctamente.");
  Screen::_textcolor(COL_TITULO);
  cout << endl << "  CERTIFICADO DE PLAZO FIJO" << endl << endl;
  Screen::ColorNormal();
  snprintf(texto, sizeof texto, "%ld", nroCertificado);
  PF_LineaTexto("Nro. de certificado.......:", texto);
  PF_LineaTexto("Titular...................:",
                aDatosPers[nroUsu].apellidoNombre);
  PF_LineaTexto("Cuenta debitada...........:", aDatosPers[nroUsu].nroCtaCA);
  PF_LineaTexto("Fecha de constitucion.....:", fechaHoy);
  PF_Detalle(capital, TASA_PF_ANUAL, dias, vencimiento, interes, total);
  PF_LineaImporte("Nuevo saldo Caja de Ahorro:", " $ ",
                  Operacion::SaldoCA(aDatosPers, nroUsu));
  Operacion::Aviso("El total se acreditara en su cuenta al vencimiento.");
  Operacion::Continuar();
} // Menu_InversionPF

// ---------------------------------------------------------------------------
// Nivel 3 - Suscripcion a un Fondo Comun de Inversion.
//
// Debita el capital suscripto de la Caja de Ahorro y muestra el rendimiento
// ESTIMADO a PF_DIAS_FCI dias con la tasa TASA_FCI_ANUAL. A diferencia del
// plazo fijo, el rendimiento de un FCI no esta asegurado: la cifra que se
// informa es una proyeccion, no un compromiso del banco.
// ---------------------------------------------------------------------------
void Menu_FondoInversion(DatosPers aDatosPers[], short nroUsu) {
  char  texto[LARGO_NOMBRE + 1];
  char  fechaHoy[ANCHO_FECHA + 1];
  char  fechaEst[ANCHO_FECHA + 1];
  Fecha hoy, estimada;
  double saldo, maximo, capital, rendimiento, total;
  bool  confirma;

  Operacion::PantallaOperacion("FONDO COMUN DE INVERSION", aDatosPers, nroUsu);

  saldo = Operacion::SaldoCA(aDatosPers, nroUsu);
  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fechaHoy);
  FechaHora::SumarDias(hoy, PF_DIAS_FCI, estimada);
  FechaHora::FormatearFechaCeros(estimada, fechaEst);

  PF_LineaImporte("Saldo Caja de Ahorro......:", " $ ", saldo);
  snprintf(texto, sizeof texto, "%.*f %%", DECIMALES, TASA_FCI_ANUAL);
  PF_LineaTexto("TNA estimada del fondo....:", texto);
  PF_LineaImporte("Suscripcion minima........:", " $ ", MIN_FONDO_INV);
  cout << endl;

  // Igual que en el plazo fijo, si el saldo no alcanza la suscripcion minima
  // no se pide el importe: no existiria valor valido para ingresar.
  if (saldo < MIN_FONDO_INV) {
    Operacion::Error("Saldo insuficiente para suscribir el fondo.");
    Operacion::Continuar();
    return;
  }

  maximo = saldo < Archivo::MAX_IMPORTE ? saldo : Archivo::MAX_IMPORTE;
  capital = Validar::LeerReal("  Capital a invertir $.....: ", MIN_FONDO_INV,
                              maximo);
  if (Validar::EntradaAgotada())
    return;
  if (capital > saldo) {
    Operacion::Error("El capital supera el saldo de la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  rendimiento = PF_InteresSimple(capital, TASA_FCI_ANUAL, PF_DIAS_FCI);
  total       = capital + rendimiento;

  cout << endl;
  confirma = Validar::LeerSiNo("  Confirma la suscripcion (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("Operacion cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  // Autorizacion por token, con el mismo criterio que el plazo fijo: por
  // encima de TOPE_SIN_TOKEN toda operacion que mueve dinero lo exige.
  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, capital)) {
    Operacion::Continuar();
    return;
  }

  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA,
                                           "Suscripcion FCI", capital)) {
    Operacion::Error("No se pudo registrar el debito en la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  Operacion::Exito("Suscripcion al Fondo Comun de Inversion registrada.");
  Screen::_textcolor(COL_TITULO);
  cout << endl << "  COMPROBANTE DE SUSCRIPCION" << endl << endl;
  Screen::ColorNormal();
  PF_LineaTexto("Titular...................:",
                aDatosPers[nroUsu].apellidoNombre);
  PF_LineaTexto("Cuenta debitada...........:", aDatosPers[nroUsu].nroCtaCA);
  PF_LineaTexto("Fecha de suscripcion......:", fechaHoy);
  PF_LineaImporte("Capital suscripto.........:", " $ ", capital);
  snprintf(texto, sizeof texto, "%.*f %%", DECIMALES, TASA_FCI_ANUAL);
  PF_LineaTexto("TNA estimada del fondo....:", texto);
  snprintf(texto, sizeof texto, "%d dias", PF_DIAS_FCI);
  PF_LineaTexto("Plazo de la estimacion....:", texto);
  PF_LineaTexto("Fecha de la estimacion....:", fechaEst);
  PF_LineaImporte("Rendimiento estimado......:", " $ ", rendimiento);
  PF_LineaImporte("Total estimado............:", " $ ", total);
  PF_LineaImporte("Nuevo saldo Caja de Ahorro:", " $ ",
                  Operacion::SaldoCA(aDatosPers, nroUsu));
  Operacion::Aviso("Rendimiento ESTIMADO: los fondos comunes de inversion no "
                   "aseguran resultados.");
  Operacion::Continuar();
} // Menu_FondoInversion

#endif // MENU_PLAZOFIJO_HPP
