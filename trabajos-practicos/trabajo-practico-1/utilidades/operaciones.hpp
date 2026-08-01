// ---------------------------------------------------------------------------
// operaciones.hpp
// Operaciones comunes a varios modulos del Home Banking:
//
//   * presentacion de pantallas y de las tablas de movimientos,
//   * calculo del saldo de la Caja de Ahorro a partir del archivo,
//   * registracion de nuevos movimientos en CA, TD y TC,
//   * generacion y validacion del token de seguridad,
//   * numeracion correlativa de las cuentas nuevas.
//
// Se agrupan aca para que ningun modulo Menu_*() repita codigo.
// ---------------------------------------------------------------------------
#ifndef OPERACIONES_HPP
#define OPERACIONES_HPP

namespace Operacion {

  // ----- Lineas fijas de las pantallas de menu -----
  const short LIN_TITULO  =  2;
  const short LIN_USUARIO =  3;
  const short LIN_LINEA   =  4;
  const short LIN_MENU    =  6;
  const short COL_IZQ     =  3;

  // ----- Anchos de las columnas de las tablas de movimientos -----
  const short TAB_DETALLE = 25;
  const short TAB_MONTO   = 13;
  const short TAB_SALDO   = 14;
  const short TAB_COMERCIO = 30;
  const short TAB_IMPORTE = 14;
  const short TAB_CUOTAS  =  6;
  const short TAB_CPBTE   = 10;

  // =========================================================================
  //                              PRESENTACION
  // =========================================================================

  // -------------------------------------------------------------------------
  // Emite un importe con dos decimales, ajustado a la derecha.
  // -------------------------------------------------------------------------
  void Importe(double valor, short ancho) {
    cout << fixed << setprecision(DECIMALES) << setw(ancho) << valor;
  } // Importe

  // -------------------------------------------------------------------------
  // Emite un espacio en blanco de <<ancho>> columnas (columna sin dato).
  // -------------------------------------------------------------------------
  void Blancos(short ancho) {
    for (short i = 0; i < ancho; i++)
      cout << ' ';
  } // Blancos

  // -------------------------------------------------------------------------
  // Encabezado de una pantalla de MENU: usa posicionamiento absoluto porque
  // debajo se dibuja el menu navegable.
  // Devuelve la linea a partir de la cual se pueden ubicar las opciones.
  // -------------------------------------------------------------------------
  short PantallaMenu(const char titulo[], DatosPers aDatosPers[],
                     short nroUsu) {
    char rotulo[100];
    char fecha[ANCHO_FECHA + 1];
    Fecha hoy;

    Screen::_clrscr();
    Screen::MnsgBox(COL_IZQ, LIN_TITULO, COL_TITULO, titulo);
    if (nroUsu != NO_ENCONTRADO) {
      FechaHora::FechaDeHoy(hoy);
      FechaHora::FormatearFechaCeros(hoy, fecha);
      snprintf(rotulo, sizeof rotulo, "Usuario: %s   |   %s",
               aDatosPers[nroUsu].apellidoNombre, fecha);
      Screen::MnsgBox(COL_IZQ, LIN_USUARIO, GRIS_CLARO, rotulo);
    }
    Screen::_gotoxy(COL_IZQ, LIN_LINEA);
    Screen::_textcolor(COL_MARCO);
    Screen::Separador(74, '=');
    Screen::ColorNormal();
    return LIN_MENU;
  } // PantallaMenu

  // -------------------------------------------------------------------------
  // Encabezado de una pantalla de OPERACION o de LISTADO: se emite en forma
  // secuencial, de modo que lo que se imprima a continuacion siga el flujo
  // normal de la pantalla (y del archivo ListadosHB.Txt).
  // -------------------------------------------------------------------------
  void PantallaOperacion(const char titulo[], DatosPers aDatosPers[],
                         short nroUsu) {
    char fecha[ANCHO_FECHA + 1];
    Fecha hoy;

    Screen::_clrscr();
    Screen::_textcolor(COL_TITULO);
    cout << endl;
    Screen::Separador(74, '=');
    cout << endl << "  " << titulo << endl;
    if (nroUsu != NO_ENCONTRADO) {
      FechaHora::FechaDeHoy(hoy);
      FechaHora::FormatearFechaCeros(hoy, fecha);
      Screen::_textcolor(GRIS_CLARO);
      cout << "  Usuario: " << aDatosPers[nroUsu].apellidoNombre
           << "   |   Cta.CA: " << aDatosPers[nroUsu].nroCtaCA
           << "   |   " << fecha << endl;
    }
    Screen::_textcolor(COL_MARCO);
    Screen::Separador(74, '=');
    Screen::ColorNormal();
    cout << endl << endl;
  } // PantallaOperacion

  // -------------------------------------------------------------------------
  // Mensajes de resultado.
  // -------------------------------------------------------------------------
  void Exito(const char texto[]) {
    Screen::_textcolor(COL_EXITO);
    cout << endl << "  [OK] " << texto << endl;
    Screen::ColorNormal();
  } // Exito

  void Error(const char texto[]) {
    Screen::_textcolor(COL_ERROR);
    cout << endl << "  [ERROR] " << texto << endl;
    Screen::ColorNormal();
  } // Error

  void Aviso(const char texto[]) {
    Screen::_textcolor(COL_AVISO);
    cout << endl << "  [AVISO] " << texto << endl;
    Screen::ColorNormal();
  } // Aviso

  // -------------------------------------------------------------------------
  // Espera que el usuario oprima ENTER, en forma secuencial.
  // -------------------------------------------------------------------------
  void Continuar() {
    char basura[Validar::LARGO_BUFFER];

    if (Validar::EntradaAgotada())
      return;
    cout << endl;
    Screen::_textcolor(COL_AVISO);
    cout << "  Oprima ENTER para continuar...";
    Screen::ColorNormal();
    cout.flush();
    Validar::LeerLinea(basura, Validar::LARGO_BUFFER - 1);
  } // Continuar

  // =========================================================================
  //                     TABLAS DE MOVIMIENTOS (con rotulos)
  // =========================================================================

  // -------------------------------------------------------------------------
  // Movimientos de la Caja de Ahorro, con las columnas Debe, Haber y el saldo
  // acumulado movimiento a movimiento.
  //   'D' (Debe)  -> acredita: incrementa el saldo
  //   'H' (Haber) -> debita  : disminuye el saldo
  // -------------------------------------------------------------------------
  void TablaCA(MovCA aMov[], short cant, double saldoInicial,
               const char nroCta[], const char moneda[]) {
    char  fecha[ANCHO_FECHA + 1];
    double saldo = saldoInicial;
    double totalDebe = 0, totalHaber = 0;

    cout << "  Cta.CA:  " << nroCta << endl;
    cout << "  Saldo inicial " << moneda;
    Importe(saldoInicial, TAB_SALDO);
    cout << endl << endl;

    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("Fecha", ANCHO_FECHA, 'i');
    cout << " T ";
    Screen::Texto("Detalle", TAB_DETALLE, 'i');
    Screen::Texto("Debe", TAB_MONTO, 'd');
    Screen::Texto("Haber", TAB_MONTO, 'd');
    Screen::Texto("Saldo", TAB_SALDO, 'd');
    cout << endl;
    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_FECHA + 3 + TAB_DETALLE + 2 * TAB_MONTO +
                      TAB_SALDO, '-');
    cout << endl;
    Screen::ColorNormal();

    for (short i = 0; i < cant; i++) {
      FechaHora::FormatearFecha(aMov[i].fecha, fecha);
      if (aMov[i].tipoMov == MOV_ACREDITA) {
        saldo += aMov[i].importe;
        totalDebe += aMov[i].importe;
      }
      else {
        saldo -= aMov[i].importe;
        totalHaber += aMov[i].importe;
      }
      cout << "  ";
      Screen::Texto(fecha, ANCHO_FECHA, 'i');
      cout << ' ' << aMov[i].tipoMov << ' ';
      Screen::Texto(aMov[i].detalle, TAB_DETALLE, 'i');
      if (aMov[i].tipoMov == MOV_ACREDITA) {
        Importe(aMov[i].importe, TAB_MONTO);
        Blancos(TAB_MONTO);
      }
      else {
        Blancos(TAB_MONTO);
        Importe(aMov[i].importe, TAB_MONTO);
      }
      Importe(saldo, TAB_SALDO);
      cout << endl;
    }

    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_FECHA + 3 + TAB_DETALLE + 2 * TAB_MONTO +
                      TAB_SALDO, '-');
    cout << endl;
    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("", ANCHO_FECHA + 3, 'i');
    Screen::Texto("Totales", TAB_DETALLE, 'i');
    Importe(totalDebe, TAB_MONTO);
    Importe(totalHaber, TAB_MONTO);
    Importe(saldo, TAB_SALDO);
    cout << endl;
    Screen::ColorNormal();
    cout << endl << "  Cantidad de movimientos: " << cant << endl;
  } // TablaCA

  // -------------------------------------------------------------------------
  // Movimientos de la Tarjeta de Debito.
  // -------------------------------------------------------------------------
  void TablaTD(MovTD aMov[], short cant) {
    char  fecha[ANCHO_FECHA + 1];
    double total = 0;

    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("Fecha", ANCHO_FECHA, 'i');
    cout << ' ';
    Screen::Texto("Comercio", TAB_COMERCIO, 'i');
    Screen::Texto("Importe", TAB_IMPORTE, 'd');
    cout << endl;
    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_FECHA + 1 + TAB_COMERCIO + TAB_IMPORTE, '-');
    cout << endl;
    Screen::ColorNormal();

    for (short i = 0; i < cant; i++) {
      FechaHora::FormatearFecha(aMov[i].fecha, fecha);
      total += aMov[i].importe;
      cout << "  ";
      Screen::Texto(fecha, ANCHO_FECHA, 'i');
      cout << ' ';
      Screen::Texto(aMov[i].detalle, TAB_COMERCIO, 'i');
      Importe(aMov[i].importe, TAB_IMPORTE);
      cout << endl;
    }

    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_FECHA + 1 + TAB_COMERCIO + TAB_IMPORTE, '-');
    cout << endl;
    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("", ANCHO_FECHA + 1, 'i');
    Screen::Texto("Total", TAB_COMERCIO, 'd');
    Importe(total, TAB_IMPORTE);
    cout << endl;
    Screen::ColorNormal();
    cout << endl << "  Cantidad de consumos: " << cant << endl;
  } // TablaTD

  // -------------------------------------------------------------------------
  // Movimientos de la Tarjeta de Credito.
  // -------------------------------------------------------------------------
  void TablaTC(MovTC aMov[], short cant) {
    char  fecha[ANCHO_FECHA + 1];
    char  cpbte[ANCHO_CPBTE + 1];
    double total = 0;

    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("Fecha", ANCHO_FECHA, 'i');
    cout << ' ';
    Screen::Texto("Comercio", TAB_COMERCIO, 'i');
    cout << ' ';
    Screen::Texto("Cuotas", TAB_CUOTAS, 'd');
    cout << ' ';
    Screen::Texto("Nro.Cpbte.", TAB_CPBTE, 'd');
    Screen::Texto("Importe", TAB_IMPORTE, 'd');
    cout << endl;
    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_FECHA + 1 + TAB_COMERCIO + 1 + TAB_CUOTAS + 1 +
                      TAB_CPBTE + TAB_IMPORTE, '-');
    cout << endl;
    Screen::ColorNormal();

    for (short i = 0; i < cant; i++) {
      FechaHora::FormatearFecha(aMov[i].fecha, fecha);
      total += aMov[i].importe;
      if (aMov[i].nroCpbte == 0)
        cpbte[0] = '\0';
      else
        snprintf(cpbte, sizeof cpbte, "%ld", aMov[i].nroCpbte);
      cout << "  ";
      Screen::Texto(fecha, ANCHO_FECHA, 'i');
      cout << ' ';
      Screen::Texto(aMov[i].detalle, TAB_COMERCIO, 'i');
      cout << ' ';
      Screen::Texto(aMov[i].cuotas, TAB_CUOTAS, 'd');
      cout << ' ';
      Screen::Texto(cpbte, TAB_CPBTE, 'd');
      Importe(aMov[i].importe, TAB_IMPORTE);
      cout << endl;
    }

    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_FECHA + 1 + TAB_COMERCIO + 1 + TAB_CUOTAS + 1 +
                      TAB_CPBTE + TAB_IMPORTE, '-');
    cout << endl;
    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("", ANCHO_FECHA + 1, 'i');
    Screen::Texto("Total", TAB_COMERCIO + 1 + TAB_CUOTAS + 1 + TAB_CPBTE, 'd');
    Importe(total, TAB_IMPORTE);
    cout << endl;
    Screen::ColorNormal();
    cout << endl << "  Cantidad de consumos: " << cant << endl;
  } // TablaTC

  // =========================================================================
  //                        CUENTAS Y SALDOS DEL USUARIO
  // =========================================================================

  // -------------------------------------------------------------------------
  // Posicion de la Caja de Ahorro en pesos (cuenta principal) del usuario.
  // -------------------------------------------------------------------------
  short IndiceCuentaCA(DatosPers aDatosPers[], short nroUsu) {
    return Ordenar::SecuencialCuentaPorTipo(aDatosPers[nroUsu].aCuentas,
                                            aDatosPers[nroUsu].cantCuentas,
                                            "CA", "$");
  } // IndiceCuentaCA

  // -------------------------------------------------------------------------
  // Posicion de la Caja de Ahorro en dolares del usuario.
  // -------------------------------------------------------------------------
  short IndiceCuentaDolares(DatosPers aDatosPers[], short nroUsu) {
    return Ordenar::SecuencialCuentaPorTipo(aDatosPers[nroUsu].aCuentas,
                                            aDatosPers[nroUsu].cantCuentas,
                                            "CA", "u$s");
  } // IndiceCuentaDolares

  // -------------------------------------------------------------------------
  // Recalcula el saldo de la Caja de Ahorro en pesos leyendo el archivo de
  // movimientos (acceso secuencial) y lo deja actualizado en la cuenta.
  // Devuelve el saldo resultante.
  // -------------------------------------------------------------------------
  double RecalcularSaldoCA(DatosPers aDatosPers[], short nroUsu) {
    double acredita, debita;
    short  iCta = IndiceCuentaCA(aDatosPers, nroUsu);

    if (iCta == NO_ENCONTRADO)
      return 0;

    Cuenta &cta = aDatosPers[nroUsu].aCuentas[iCta];

    // Se acumula recorriendo el archivo completo, y NO sobre un array de
    // movimientos: asi el saldo sigue siendo exacto por muchas operaciones que
    // el usuario llegue a realizar.
    Archivo::AcumularMovimientosCA(aDatosPers[nroUsu].nroUsuario, acredita,
                                   debita);
    cta.saldo = cta.saldoInicial + acredita - debita;
    return cta.saldo;
  } // RecalcularSaldoCA

  double SaldoCA(DatosPers aDatosPers[], short nroUsu) {
    short iCta = IndiceCuentaCA(aDatosPers, nroUsu);

    return iCta == NO_ENCONTRADO ? 0 : aDatosPers[nroUsu].aCuentas[iCta].saldo;
  } // SaldoCA

  double SaldoDolares(DatosPers aDatosPers[], short nroUsu) {
    short iCta = IndiceCuentaDolares(aDatosPers, nroUsu);

    return iCta == NO_ENCONTRADO ? 0 : aDatosPers[nroUsu].aCuentas[iCta].saldo;
  } // SaldoDolares

  // =========================================================================
  //                      REGISTRACION DE NUEVOS MOVIMIENTOS
  // =========================================================================

  // -------------------------------------------------------------------------
  // Genera un nuevo movimiento en la Caja de Ahorro con la fecha indicada:
  // valida el saldo cuando la operacion debita, agrega la componente al
  // archivo y recalcula el saldo de la cuenta.
  // -------------------------------------------------------------------------
  bool RegistrarMovimientoCA(DatosPers aDatosPers[], short nroUsu,
                             Fecha fecha, char tipoMov, const char detalle[],
                             double importe) {
    MovCA mov;

    if (importe <= 0 or importe > Archivo::MAX_IMPORTE)
      return false;
    if (tipoMov == MOV_DEBITA and importe > SaldoCA(aDatosPers, nroUsu))
      return false;

    mov.fecha   = fecha;
    mov.tipoMov = tipoMov;
    Validar::CopiarCadena(mov.detalle, detalle, LARGO_DETALLE);
    Validar::NormalizarASCII(mov.detalle);
    mov.importe    = importe;
    mov.nroUsuario = aDatosPers[nroUsu].nroUsuario;
    mov.saldo      = 0;
    mov.nroReg     = 0;
    if (not Archivo::AgregarMovimientoCA(mov))
      return false;
    RecalcularSaldoCA(aDatosPers, nroUsu);
    return true;
  } // RegistrarMovimientoCA

  // -------------------------------------------------------------------------
  // Igual que la anterior, pero con la fecha del dia.
  // -------------------------------------------------------------------------
  bool RegistrarMovimientoCA(DatosPers aDatosPers[], short nroUsu,
                             char tipoMov, const char detalle[],
                             double importe) {
    Fecha hoy;

    FechaHora::FechaDeHoy(hoy);
    return RegistrarMovimientoCA(aDatosPers, nroUsu, hoy, tipoMov, detalle,
                                 importe);
  } // RegistrarMovimientoCA

  // -------------------------------------------------------------------------
  // Genera un nuevo movimiento en la Tarjeta de Debito.
  // -------------------------------------------------------------------------
  bool RegistrarMovimientoTD(DatosPers aDatosPers[], short nroUsu,
                             Fecha fecha, const char detalle[],
                             double importe) {
    MovTD mov;

    if (importe <= 0 or importe > Archivo::MAX_IMPORTE)
      return false;
    mov.fecha = fecha;
    Validar::CopiarCadena(mov.detalle, detalle, LARGO_DETALLE);
    Validar::NormalizarASCII(mov.detalle);
    mov.importe    = importe;
    mov.nroUsuario = aDatosPers[nroUsu].nroUsuario;
    mov.nroReg     = 0;
    return Archivo::AgregarMovimientoTD(mov);
  } // RegistrarMovimientoTD

  // -------------------------------------------------------------------------
  // Genera un nuevo movimiento en la Tarjeta de Credito.
  // -------------------------------------------------------------------------
  bool RegistrarMovimientoTC(DatosPers aDatosPers[], short nroUsu,
                             Fecha fecha, const char detalle[],
                             const char cuotas[], long nroCpbte,
                             double importe) {
    MovTC mov;

    if (importe <= 0 or importe > Archivo::MAX_IMPORTE)
      return false;
    mov.fecha = fecha;
    Validar::CopiarCadena(mov.detalle, detalle, LARGO_DETALLE);
    Validar::NormalizarASCII(mov.detalle);
    Validar::CopiarCadena(mov.cuotas, cuotas, LARGO_CUOTAS);
    mov.nroCpbte   = nroCpbte;
    mov.importe    = importe;
    mov.nroUsuario = aDatosPers[nroUsu].nroUsuario;
    mov.nroReg     = 0;
    return Archivo::AgregarMovimientoTC(mov);
  } // RegistrarMovimientoTC

  // =========================================================================
  //                          TOKEN DE SEGURIDAD
  // =========================================================================

  // -------------------------------------------------------------------------
  // Genera un token al azar de 6 digitos.
  // -------------------------------------------------------------------------
  long GenerarToken() {
    return rand() % (TOKEN_MAXIMO - TOKEN_MINIMO + 1) + TOKEN_MINIMO;
  } // GenerarToken

  // -------------------------------------------------------------------------
  // Genera un token, lo muestra simulando el envio al celular del usuario y
  // pide que se lo ingrese. Devuelve true si se valido correctamente dentro de
  // los INTENTOS_MAX permitidos.
  // -------------------------------------------------------------------------
  bool ValidarToken(DatosPers aDatosPers[], short nroUsu) {
    long  token = GenerarToken();
    long  ingresado;
    short intentos = 0;

    Screen::_textcolor(COL_AVISO);
    cout << endl << "  Por seguridad esta operacion requiere TOKEN." << endl;
    cout << "  Se envio un codigo al celular " << aDatosPers[nroUsu].celular
         << endl;
    Screen::_textcolor(COL_TITULO);
    cout << "  [SMS simulado] Su token es: " << token << endl;
    Screen::ColorNormal();

    do {
      ingresado = Validar::LeerEntero("  Ingrese el token de 6 digitos: ",
                                      TOKEN_MINIMO, TOKEN_MAXIMO);
      intentos++;
      if (ingresado != token and intentos < INTENTOS_MAX and
          not Validar::EntradaAgotada())
        Error("Token incorrecto. Reintente.");
    } while (ingresado != token and intentos < INTENTOS_MAX and
             not Validar::EntradaAgotada());

    if (ingresado != token) {
      Error("Token invalido. La operacion fue cancelada.");
      return false;
    }
    return true;
  } // ValidarToken

  // -------------------------------------------------------------------------
  // Pide token unicamente cuando el importe supera el tope establecido.
  // -------------------------------------------------------------------------
  bool AutorizarOperacion(DatosPers aDatosPers[], short nroUsu,
                          double importe) {
    if (importe <= TOPE_SIN_TOKEN)
      return true;
    return ValidarToken(aDatosPers, nroUsu);
  } // AutorizarOperacion

  // =========================================================================
  //                    NUMERACION CORRELATIVA DE CUENTAS
  // =========================================================================

  // -------------------------------------------------------------------------
  // Arma el numero de la proxima cuenta a crear con formato 999-9999999, donde
  // el segundo grupo es el correlativo respecto de las cuentas ya existentes
  // en todo el sistema.
  // -------------------------------------------------------------------------
  void NuevoNroCuenta(DatosPers aDatosPers[], short cantUsuarios,
                      char dest[]) {
    const short SUCURSAL = 9;
    short       correlativo = 0;

    for (short u = 0; u < cantUsuarios; u++)
      correlativo = (short)(correlativo + aDatosPers[u].cantCuentas);
    correlativo++;
    snprintf(dest, LARGO_CTA + 1, "%03d-%07d", SUCURSAL, correlativo);
  } // NuevoNroCuenta

  // -------------------------------------------------------------------------
  // Muestra el detalle de todas las cuentas del usuario.
  // -------------------------------------------------------------------------
  void ListarCuentas(DatosPers aDatosPers[], short nroUsu) {
    const short ANC_NRO = 20, ANC_TIPO = 6, ANC_MON = 8, ANC_SALDO = 16,
                ANC_EST = 10;

    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("Nro. Cuenta", ANC_NRO, 'i');
    Screen::Texto("Tipo", ANC_TIPO, 'i');
    Screen::Texto("Moneda", ANC_MON, 'i');
    Screen::Texto("Saldo", ANC_SALDO, 'd');
    cout << "  ";
    Screen::Texto("Estado", ANC_EST, 'i');
    cout << endl;
    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANC_NRO + ANC_TIPO + ANC_MON + ANC_SALDO + 2 + ANC_EST,
                      '-');
    cout << endl;
    Screen::ColorNormal();

    for (short i = 0; i < aDatosPers[nroUsu].cantCuentas; i++) {
      Cuenta &cta = aDatosPers[nroUsu].aCuentas[i];
      cout << "  ";
      Screen::Texto(cta.nroCta, ANC_NRO, 'i');
      Screen::Texto(cta.tipo,   ANC_TIPO, 'i');
      Screen::Texto(cta.moneda, ANC_MON,  'i');
      Importe(cta.saldo, ANC_SALDO);
      cout << "  ";
      Screen::Texto(cta.estado == 'A' ? "Activa" : "Inactiva", ANC_EST, 'i');
      cout << endl;
    }
    cout << endl << "  Cantidad de cuentas: " << aDatosPers[nroUsu].cantCuentas
         << endl;
  } // ListarCuentas

  // -------------------------------------------------------------------------
  // Muestra la agenda de CBU del usuario, ordenada por alias.
  // -------------------------------------------------------------------------
  void ListarAgenda(DatosPers aDatosPers[], short nroUsu) {
    const short ANC_NRO = 5, ANC_ALIAS = 20, ANC_TIT = 28, ANC_CBU = 24;
    char        nro[ANC_NRO + 1];

    if (aDatosPers[nroUsu].cantAgenda == 0) {
      Aviso("La agenda de CBU esta vacia.");
      return;
    }
    Screen::_textcolor(COL_ROTULO);
    cout << "  ";
    Screen::Texto("Nro.", ANC_NRO, 'd');
    cout << "  ";
    Screen::Texto("Alias", ANC_ALIAS, 'i');
    Screen::Texto("Titular", ANC_TIT, 'i');
    Screen::Texto("CBU", ANC_CBU, 'i');
    cout << endl;
    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANC_NRO + 2 + ANC_ALIAS + ANC_TIT + ANC_CBU, '-');
    cout << endl;
    Screen::ColorNormal();

    for (short i = 0; i < aDatosPers[nroUsu].cantAgenda; i++) {
      Contacto &con = aDatosPers[nroUsu].aAgenda[i];
      snprintf(nro, sizeof nro, "%d", i + 1);
      cout << "  ";
      Screen::Texto(nro, ANC_NRO, 'd');
      cout << "  ";
      Screen::Texto(con.alias, ANC_ALIAS, 'i');
      Screen::Texto(con.apellidoNombre, ANC_TIT, 'i');
      Screen::Texto(con.cbu, ANC_CBU, 'i');
      cout << endl;
    }
    cout << endl << "  Cantidad de cuentas agendadas: "
         << aDatosPers[nroUsu].cantAgenda << endl;
  } // ListarAgenda

} // namespace Operacion

#endif // OPERACIONES_HPP
