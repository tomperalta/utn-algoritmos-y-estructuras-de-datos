// ---------------------------------------------------------------------------
// Menu_Tarjetas.hpp
// Nivel 2 - Opcion <<Tarjetas>> del menu principal.
//
// Submenu de las tarjetas asociadas a la cuenta del usuario:
//
//   1. Tarjeta de Debito   -> datos de la tarjeta y luego Menu_MovimientosTD()
//   2. Tarjeta de Credito  -> datos de la tarjeta (limite de compra, cierre y
//                             vencimiento del resumen) y luego
//                             Menu_MovimientosTC()
//   3. Resumen comparativo -> Menu_ResumenTarjetas()
//   4. Volver al menu principal
//
// Numeracion de las tarjetas
// --------------------------
// Los numeros de tarjeta no se almacenan en los datos personales: se ARMAN
// enmascarados a partir del DNI del usuario, mostrando solamente los cuatro
// ultimos digitos, tal como lo hacen los sitios reales de home banking
// ("4517 88** **** 5478"). De esa manera cada usuario tiene siempre el mismo
// numero de tarjeta sin necesidad de agregar campos al registro DatosPers.
//
// Fechas del resumen de la Tarjeta de Credito
// -------------------------------------------
// El cierre del resumen es el dia TARJ_DIA_CIERRE del mes en curso; si ese dia
// ya paso, el cierre pasa al mes siguiente. El vencimiento se obtiene sumando
// TARJ_DIAS_VTO dias al cierre con FechaHora::SumarDias(), que se encarga de
// los meses de distinta duracion y de los anios bisiestos.
// ---------------------------------------------------------------------------
#ifndef MENU_TARJETAS_HPP
#define MENU_TARJETAS_HPP

// ----- Parametros de las tarjetas (constantes con nombre de este modulo) ----
const short TARJ_LARGO_NRO   = 19;      // "4517 88** **** 5478"
const short TARJ_LARGO_VTO   =  5;      // "mm/aa"
const short TARJ_DIA_CIERRE  = 20;      // dia de cierre del resumen de la TC
const short TARJ_DIAS_VTO    = 10;      // dias entre el cierre y el vencimiento
const short TARJ_ANIOS_VIG   =  4;      // vigencia del plastico, en anios
const short TARJ_MESES_ANIO  = 12;
const short TARJ_SIGLO       = 100;     // para expresar el anio en dos digitos
const char  TARJ_PREFIJO_TD[] = "4517 88";   // Visa Debito
const char  TARJ_PREFIJO_TC[] = "5412 73";   // Mastercard Credito
const char  TARJ_MARCA_TD[]   = "Visa Debito";
const char  TARJ_MARCA_TC[]   = "Mastercard Credito";

// ---------------------------------------------------------------------------
// Arma el numero de tarjeta enmascarado a partir del DNI del usuario: se
// conservan a la vista el prefijo del emisor y los cuatro ultimos digitos del
// documento, y el resto se reemplaza por asteriscos.
// ---------------------------------------------------------------------------
static void Tarjetas_ArmarNumero(const char prefijo[], long dni, char dest[],
                                 short tam) {
  const long ULTIMOS_CUATRO = 10000;    // el resto da los 4 ultimos digitos

  snprintf(dest, tam, "%s** **** %04ld", prefijo, dni % ULTIMOS_CUATRO);
} // Tarjetas_ArmarNumero

// ---------------------------------------------------------------------------
// Emite el rotulo de un dato y deja el color preparado para el valor.
// ---------------------------------------------------------------------------
static void Tarjetas_Rotulo(const char rotulo[]) {
  const short ANC_ROTULO = 28;

  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, ANC_ROTULO, 'i');
  cout << ' ';
  Screen::_textcolor(COL_DATO);
} // Tarjetas_Rotulo

static void Tarjetas_DatoTexto(const char rotulo[], const char valor[]) {
  Tarjetas_Rotulo(rotulo);
  cout << valor << endl;
  Screen::ColorNormal();
} // Tarjetas_DatoTexto

static void Tarjetas_DatoEntero(const char rotulo[], long valor) {
  Tarjetas_Rotulo(rotulo);
  cout << valor << endl;
  Screen::ColorNormal();
} // Tarjetas_DatoEntero

static void Tarjetas_DatoImporte(const char rotulo[], double valor) {
  const short ANC_IMPORTE = 16;

  Tarjetas_Rotulo(rotulo);
  cout << "$";
  Operacion::Importe(valor, ANC_IMPORTE);
  cout << endl;
  Screen::ColorNormal();
} // Tarjetas_DatoImporte

// ---------------------------------------------------------------------------
// Opcion 1 - Datos de la Tarjeta de Debito y, a continuacion, sus consumos.
// ---------------------------------------------------------------------------
static void Tarjetas_Debito(DatosPers aDatosPers[], short nroUsu) {
  char  numero[TARJ_LARGO_NRO + 1];
  char  vence[TARJ_LARGO_VTO + 1];
  Fecha hoy;

  Operacion::PantallaOperacion("TARJETA DE DEBITO", aDatosPers, nroUsu);

  FechaHora::FechaDeHoy(hoy);
  Tarjetas_ArmarNumero(TARJ_PREFIJO_TD, aDatosPers[nroUsu].dni, numero,
                       TARJ_LARGO_NRO + 1);
  snprintf(vence, sizeof vence, "%02d/%02d", hoy.mes,
           (hoy.anio + TARJ_ANIOS_VIG) % TARJ_SIGLO);

  Tarjetas_DatoTexto ("Marca......................:", TARJ_MARCA_TD);
  Tarjetas_DatoTexto ("Numero de tarjeta..........:", numero);
  Tarjetas_DatoTexto ("Titular....................:",
                      aDatosPers[nroUsu].apellidoNombre);
  Tarjetas_DatoTexto ("Cuenta asociada............:",
                      aDatosPers[nroUsu].nroCtaCA);
  Tarjetas_DatoTexto ("Vencimiento del plastico...:", vence);
  Tarjetas_DatoTexto ("Estado.....................:", "Habilitada");
  Tarjetas_DatoImporte("Saldo disponible en la CA..:",
                       Operacion::SaldoCA(aDatosPers, nroUsu));
  cout << endl << "  Los consumos con Tarjeta de Debito debitan del saldo de "
       << "la Caja de Ahorro." << endl;

  Operacion::Continuar();
  if (Validar::EntradaAgotada())
    return;
  Menu_MovimientosTD(aDatosPers, nroUsu);
} // Tarjetas_Debito

// ---------------------------------------------------------------------------
// Opcion 2 - Datos de la Tarjeta de Credito y, a continuacion, sus consumos.
// ---------------------------------------------------------------------------
static void Tarjetas_Credito(DatosPers aDatosPers[], short nroUsu) {
  char  numero[TARJ_LARGO_NRO + 1];
  char  vence[TARJ_LARGO_VTO + 1];
  char  fechaCierre[Validar::LARGO_BUFFER];
  char  fechaVto[Validar::LARGO_BUFFER];
  Fecha hoy, cierre, vencimiento;

  Operacion::PantallaOperacion("TARJETA DE CREDITO", aDatosPers, nroUsu);

  FechaHora::FechaDeHoy(hoy);
  Tarjetas_ArmarNumero(TARJ_PREFIJO_TC, aDatosPers[nroUsu].dni, numero,
                       TARJ_LARGO_NRO + 1);
  snprintf(vence, sizeof vence, "%02d/%02d", hoy.mes,
           (hoy.anio + TARJ_ANIOS_VIG) % TARJ_SIGLO);

  // Cierre del resumen: dia fijo del mes en curso, o del mes siguiente si ese
  // dia ya paso. El dia de cierre existe en todos los meses, de manera que no
  // hace falta corregirlo.
  cierre.dia  = TARJ_DIA_CIERRE;
  cierre.mes  = hoy.mes;
  cierre.anio = hoy.anio;
  if (hoy.dia > TARJ_DIA_CIERRE) {
    cierre.mes  = (short)(hoy.mes == TARJ_MESES_ANIO ? 1 : hoy.mes + 1);
    cierre.anio = (short)(hoy.mes == TARJ_MESES_ANIO ? hoy.anio + 1 : hoy.anio);
  }
  FechaHora::SumarDias(cierre, TARJ_DIAS_VTO, vencimiento);
  FechaHora::FormatearFechaLetras(cierre, fechaCierre, Validar::LARGO_BUFFER);
  FechaHora::FormatearFechaLetras(vencimiento, fechaVto, Validar::LARGO_BUFFER);

  Tarjetas_DatoTexto ("Marca......................:", TARJ_MARCA_TC);
  Tarjetas_DatoTexto ("Numero de tarjeta..........:", numero);
  Tarjetas_DatoTexto ("Titular....................:",
                      aDatosPers[nroUsu].apellidoNombre);
  Tarjetas_DatoTexto ("Vencimiento del plastico...:", vence);
  Tarjetas_DatoTexto ("Estado.....................:", "Habilitada");
  cout << endl;
  Tarjetas_DatoImporte("Limite de compra...........:", LIMITE_TARJ_CREDITO);
  Tarjetas_DatoEntero("Maximo de cuotas...........:", MAX_CUOTAS_TC);
  Tarjetas_DatoTexto ("Cierre del resumen.........:", fechaCierre);
  Tarjetas_DatoTexto ("Vencimiento del resumen....:", fechaVto);

  Operacion::Continuar();
  if (Validar::EntradaAgotada())
    return;
  Menu_MovimientosTC(aDatosPers, nroUsu);
} // Tarjetas_Credito

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de Tarjetas.
// Ciclo de construccion de menu (1-x): se repite hasta elegir Volver, cancelar
// con ESCAPE o agotarse la entrada.
// ---------------------------------------------------------------------------
void Menu_Tarjetas(DatosPers aDatosPers[], short nroUsu) {
  const short CANT_OPC = 5;
  str40Opc aMenu[CANT_OPC] = {"Tarjeta de Debito",
                              "Tarjeta de Credito",
                              "Resumen comparativo de tarjetas",
                              "Registrar el pago de una cuota",
                              "Volver al menu principal"};
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("TARJETAS", aDatosPers, nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {                          // seleccion multiple
      case 0 : Tarjetas_Debito(aDatosPers, nroUsu);        break;
      case 1 : Tarjetas_Credito(aDatosPers, nroUsu);       break;
      case 2 : Menu_ResumenTarjetas(aDatosPers, nroUsu);   break;
      case 3 : Menu_PagarCuotaTC(aDatosPers, nroUsu);        break;
      default: volver = true;                  // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_Tarjetas

// ---------------------------------------------------------------------------
// Nivel 3 - Resumen comparativo de las dos tarjetas.
//
// Lee en forma SECUENCIAL los consumos del usuario en los archivos
// MovimientosTD.Txt y MovimientosTC.Txt y calcula, para cada tarjeta, la
// cantidad de consumos, el total, el consumo promedio y el mayor consumo.
//
// Para los TRES mayores consumos con Tarjeta de Debito se ordena el array por
// importe DESCENDENTE con burbuja optimizada (Ordenar::MovTDporImporteDesc):
// una vez ordenado, los mayores quedan al comienzo y basta con recorrer las
// primeras componentes, sin volver a buscar el maximo tantas veces como
// consumos se quieran listar.
// ---------------------------------------------------------------------------
void Menu_ResumenTarjetas(DatosPers aDatosPers[], short nroUsu) {
  const short TOP_CONSUMOS = 3;
  const short ANC_TARJETA = 22, ANC_CANT = 10, ANC_MONTO = 16;
  MovTD aMovTD[MAX_MOV_TD];
  MovTC aMovTC[MAX_MOV_TC];
  char  numeroTD[TARJ_LARGO_NRO + 1], numeroTC[TARJ_LARGO_NRO + 1];
  char  fecha[ANCHO_FECHA + 1];
  short cantTD, cantTC, tope, i;
  double totalTD = 0, totalTC = 0, mayorTD = 0, mayorTC = 0;
  double promedioTD = 0, promedioTC = 0, disponible;

  Operacion::PantallaOperacion("RESUMEN COMPARATIVO DE TARJETAS", aDatosPers,
                               nroUsu);

  Tarjetas_ArmarNumero(TARJ_PREFIJO_TD, aDatosPers[nroUsu].dni, numeroTD,
                       TARJ_LARGO_NRO + 1);
  Tarjetas_ArmarNumero(TARJ_PREFIJO_TC, aDatosPers[nroUsu].dni, numeroTC,
                       TARJ_LARGO_NRO + 1);
  Tarjetas_DatoTexto("Tarjeta de Debito..........:", numeroTD);
  Tarjetas_DatoTexto("Tarjeta de Credito.........:", numeroTC);
  cout << endl;

  cantTD = Archivo::LeerMovimientosTD(aMovTD, aDatosPers[nroUsu].nroUsuario,
                                      MAX_MOV_TD);
  cantTC = Archivo::LeerMovimientosTC(aMovTC, aDatosPers[nroUsu].nroUsuario,
                                      MAX_MOV_TC);

  // Repeticion exacta: la cantidad de consumos leidos se conoce de antemano.
  for (i = 0; i < cantTD; i++) {
    totalTD = totalTD + aMovTD[i].importe;
    if (aMovTD[i].importe > mayorTD)
      mayorTD = aMovTD[i].importe;
  }
  for (i = 0; i < cantTC; i++) {
    totalTC = totalTC + aMovTC[i].importe;
    if (aMovTC[i].importe > mayorTC)
      mayorTC = aMovTC[i].importe;
  }
  if (cantTD > 0)
    promedioTD = totalTD / cantTD;
  if (cantTC > 0)
    promedioTC = totalTC / cantTC;

  // ----- Tabla comparativa -----
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto("Tarjeta", ANC_TARJETA, 'i');
  Screen::Texto("Consumos", ANC_CANT, 'd');
  Screen::Texto("Total", ANC_MONTO, 'd');
  Screen::Texto("Promedio", ANC_MONTO, 'd');
  Screen::Texto("Mayor", ANC_MONTO, 'd');
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_TARJETA + ANC_CANT + 3 * ANC_MONTO, '-');
  cout << endl;
  Screen::ColorNormal();

  cout << "  ";
  Screen::Texto("Tarjeta de Debito", ANC_TARJETA, 'i');
  cout << setw(ANC_CANT) << cantTD;
  Operacion::Importe(totalTD, ANC_MONTO);
  Operacion::Importe(promedioTD, ANC_MONTO);
  Operacion::Importe(mayorTD, ANC_MONTO);
  cout << endl;

  cout << "  ";
  Screen::Texto("Tarjeta de Credito", ANC_TARJETA, 'i');
  cout << setw(ANC_CANT) << cantTC;
  Operacion::Importe(totalTC, ANC_MONTO);
  Operacion::Importe(promedioTC, ANC_MONTO);
  Operacion::Importe(mayorTC, ANC_MONTO);
  cout << endl;

  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_TARJETA + ANC_CANT + 3 * ANC_MONTO, '-');
  cout << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto("Totales", ANC_TARJETA, 'i');
  cout << setw(ANC_CANT) << cantTD + cantTC;
  Operacion::Importe(totalTD + totalTC, ANC_MONTO);
  Operacion::Blancos(ANC_MONTO);
  Operacion::Blancos(ANC_MONTO);
  cout << endl;
  Screen::ColorNormal();
  cout << endl;

  // ----- Uso del limite de compra de la Tarjeta de Credito -----
  disponible = LIMITE_TARJ_CREDITO - totalTC;
  if (disponible < 0)
    disponible = 0;
  Tarjetas_DatoImporte("Limite de compra de la TC..:", LIMITE_TARJ_CREDITO);
  Tarjetas_DatoImporte("Consumido con la TC........:", totalTC);
  Tarjetas_DatoImporte("Disponible de la TC........:", disponible);

  // ----- Los tres mayores consumos con Tarjeta de Debito -----
  if (cantTD == 0)
    Operacion::Aviso("La Tarjeta de Debito no registra consumos.");
  else {
    Ordenar::MovTDporImporteDesc(aMovTD, cantTD);   // burbuja optimizada
    tope = (short)(cantTD < TOP_CONSUMOS ? cantTD : TOP_CONSUMOS);
    cout << endl;
    Screen::_textcolor(COL_TITULO);
    cout << "  Mayores consumos con Tarjeta de Debito (hasta " << TOP_CONSUMOS
         << "):" << endl;
    Screen::ColorNormal();
    for (i = 0; i < tope; i++) {                    // repeticion exacta
      FechaHora::FormatearFechaCeros(aMovTD[i].fecha, fecha);
      cout << "   " << i + 1 << ". ";
      Screen::Texto(fecha, ANCHO_FECHA, 'i');
      cout << ' ';
      Screen::Texto(aMovTD[i].detalle, LARGO_DETALLE, 'i');
      Operacion::Importe(aMovTD[i].importe, ANC_MONTO);
      cout << endl;
    }
  }

  if (cantTC == 0)
    Operacion::Aviso("La Tarjeta de Credito no registra consumos.");
  Operacion::Continuar();
} // Menu_ResumenTarjetas

#endif // MENU_TARJETAS_HPP
