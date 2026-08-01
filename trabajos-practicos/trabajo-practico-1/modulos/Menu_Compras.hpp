// ---------------------------------------------------------------------------
// Menu_Compras.hpp
// Nivel 2 - Opcion <<Compras>> del menu principal.
//
// Registra una compra con tarjeta. El enunciado pide los datos fecha (dia, mes
// y anio), descripcion, importe, comercio y modo de pago (TD o TC), y aclara
// que la operacion "debe modificar agregando la compra en CA, y TD o TC": es
// decir que la compra genera SIEMPRE dos componentes,
//
//     * una en la tarjeta empleada  -> MovimientosTD.Txt o MovimientosTC.Txt
//     * y otra en la Caja de Ahorro -> MovimientosCA.Txt, con tipo 'H' (Haber),
//       que es el que DISMINUYE el saldo del titular.
//
// Por eso el modulo controla el saldo de la Caja de Ahorro cualquiera sea el
// modo de pago: si no alcanza, no se registra absolutamente nada.
// ---------------------------------------------------------------------------
#ifndef MENU_COMPRAS_HPP
#define MENU_COMPRAS_HPP

// ---------------------------------------------------------------------------
// Pide la fecha de la compra y la devuelve por parametro.
//
// Mismo criterio que en el deposito: se ofrece la fecha de hoy y, si se la
// rechaza, se la pide campo por campo. No se admite una fecha futura, porque
// una compra que todavia no ocurrio no puede impactar en el saldo. La
// comparacion se hace con FechaLarga(), que convierte la fecha a un unico
// entero aaaammdd apto para comparar cronologicamente. Ciclo 1-x: se insiste
// hasta obtener una fecha aceptable o hasta que se agote la entrada.
// ---------------------------------------------------------------------------
static void Compras_LeerFecha(Fecha &fecha) {
  char  rotulo[80];
  char  fechaHoy[ANCHO_FECHA + 1];
  Fecha hoy;
  bool  aceptada = false;

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fechaHoy);
  snprintf(rotulo, sizeof rotulo, "  Usar la fecha de hoy %s (S/N)?: ",
           fechaHoy);

  do {
    fecha = hoy;
    if (not Validar::LeerSiNo(rotulo))
      Validar::LeerFecha("  Fecha de la compra:", fecha);
    aceptada = FechaHora::FechaLarga(fecha) <= FechaHora::FechaLarga(hoy);
    if (not aceptada and not Validar::EntradaAgotada())
      Operacion::Error("La fecha no puede ser posterior a la de hoy.");
  } while (not aceptada and not Validar::EntradaAgotada());
} // Compras_LeerFecha

// ---------------------------------------------------------------------------
// Pantalla de operacion de la compra.
// ---------------------------------------------------------------------------
void Menu_Compras(DatosPers aDatosPers[], short nroUsu) {
  const char  PAGO_DEBITO   = 'D';
  const char  PAGO_CREDITO  = 'C';
  const double MIN_COMPRA    = 1.00;  // importe minimo admitido
  const long  CPBTE_MINIMO  = 100000; // el comprobante tiene 6 digitos
  const long  CPBTE_MAXIMO  = 999999;
  const short ANC_IMP       = 14;     // ancho de columna de los importes
  const short ANC_LINEA     = 60;     // ancho de las lineas del comprobante
  str25 descripcion, comercio;
  str5  cuotas;
  char  fechaTexto[ANCHO_FECHA + 1];
  char  modoPago;
  Fecha fecha;
  double importe, saldoAnterior, saldoActual;
  long  nroCpbte = 0;
  short cantCuotas = 1;
  bool  okCuenta = false, okTarjeta = false, confirma;

  Operacion::PantallaOperacion("REGISTRAR UNA COMPRA", aDatosPers, nroUsu);

  // ----- Situacion de la cuenta antes de la operacion -----
  saldoAnterior = Operacion::SaldoCA(aDatosPers, nroUsu);
  Screen::_textcolor(COL_ROTULO);
  cout << "  Cuenta a debitar...: ";
  Screen::_textcolor(COL_DATO);
  cout << aDatosPers[nroUsu].nroCtaCA << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Saldo disponible...: $";
  Screen::_textcolor(COL_DATO);
  Operacion::Importe(saldoAnterior, ANC_IMP);
  Screen::ColorNormal();
  cout << endl << endl;

  // ----- Datos de la compra -----
  Compras_LeerFecha(fecha);
  if (Validar::EntradaAgotada())
    return;

  // La descripcion y el comercio se normalizan a ASCII porque se graban en
  // archivos de formato tabular: una letra acentuada ocupa dos bytes y
  // desalinearia las columnas del registro.
  Validar::LeerCadena("  Descripcion........: ", descripcion, LARGO_DETALLE,
                      true);
  importe = Validar::LeerReal("  Importe $..........: ", MIN_COMPRA,
                              Archivo::MAX_IMPORTE);
  Validar::LeerCadena("  Comercio...........: ", comercio, LARGO_DETALLE,
                      true);
  modoPago = Validar::LeerCaracter("  Modo de pago (D=Debito / C=Credito): ",
                                   "DC");
  if (Validar::EntradaAgotada())
    return;

  // ----- Datos propios del pago con tarjeta de credito -----
  if (modoPago == PAGO_CREDITO) {
    cantCuotas = (short) Validar::LeerEntero("  Cantidad de cuotas.: ", 1,
                                             MAX_CUOTAS_TC);
    if (Validar::EntradaAgotada())
      return;
    // La compra que se registra es la PRIMERA cuota del plan, de modo que la
    // cadena siempre arranca en 1: con una sola cuota queda "1/1".
    snprintf(cuotas, sizeof cuotas, "1/%d", cantCuotas);
    // Numero de comprobante simulado, al azar y de 6 digitos.
    nroCpbte = rand() % (CPBTE_MAXIMO - CPBTE_MINIMO + 1) + CPBTE_MINIMO;
  }
  else {
    cuotas[0] = '\0';
  }

  // ----- Control del saldo -----
  // Vale para los dos modos de pago: la compra impacta siempre en la Caja de
  // Ahorro, asi que si el saldo no alcanza no se registra nada, ni en la
  // tarjeta ni en la cuenta.
  if (importe > saldoAnterior) {
    Operacion::Error("El saldo de la Caja de Ahorro es insuficiente.");
    cout << "  Importe de la compra $";
    Operacion::Importe(importe, ANC_IMP);
    cout << endl;
    cout << "  Saldo disponible     $";
    Operacion::Importe(saldoAnterior, ANC_IMP);
    cout << endl;
    cout << "  La compra no fue registrada." << endl;
    Operacion::Continuar();
    return;
  }

  // ----- Confirmacion -----
  FechaHora::FormatearFechaCeros(fecha, fechaTexto);
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_LINEA, '-');
  cout << endl;
  Screen::ColorNormal();
  cout << "  " << fechaTexto << "  " << descripcion << "  en " << comercio
       << endl;
  cout << "  Modo de pago: "
       << (modoPago == PAGO_DEBITO ? "Tarjeta de Debito"
                                   : "Tarjeta de Credito");
  if (modoPago == PAGO_CREDITO)
    cout << "  Cuotas: " << cuotas;
  cout << "   $";
  Operacion::Importe(importe, ANC_IMP);
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_LINEA, '-');
  cout << endl;
  Screen::ColorNormal();

  confirma = Validar::LeerSiNo("  Confirma la compra (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("La compra fue cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  // ----- Control del limite de compra de la Tarjeta de Credito -----
  // El limite se mostraba en la pantalla de la tarjeta pero no se controlaba:
  // se verifica aca, sumando lo ya consumido en MovimientosTC.Txt.
  if (modoPago == PAGO_CREDITO) {
    double consumido  = Archivo::AcumularMovimientosTC(
                            aDatosPers[nroUsu].nroUsuario);
    double disponible = LIMITE_TARJ_CREDITO - consumido;

    if (importe > disponible) {
      Operacion::Error("La compra supera el limite de su Tarjeta de Credito.");
      cout << "  Limite de compra     $";
      Operacion::Importe(LIMITE_TARJ_CREDITO, ANC_IMP);
      cout << endl;
      cout << "  Ya consumido         $";
      Operacion::Importe(consumido, ANC_IMP);
      cout << endl;
      cout << "  Disponible           $";
      Operacion::Importe(disponible < 0 ? 0 : disponible, ANC_IMP);
      cout << endl;
      cout << "  La compra no fue registrada." << endl;
      Operacion::Continuar();
      return;
    }
  }

  // ----- Autorizacion por token -----
  // Igual que las transferencias y las inversiones: por encima de
  // TOPE_SIN_TOKEN la operacion exige el token de seguridad.
  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, importe)) {
    Operacion::Continuar();
    return;
  }

  // ----- Impacto de la compra -----
  // Primero se debita la Caja de Ahorro y recien despues se registra el
  // consumo en la tarjeta: si la cuenta no se pudiera debitar, no queda un
  // consumo grabado en la tarjeta que el saldo nunca reflejaria.
  okCuenta = Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, fecha,
                                              MOV_DEBITA, descripcion,
                                              importe);
  if (okCuenta) {
    if (modoPago == PAGO_DEBITO)
      okTarjeta = Operacion::RegistrarMovimientoTD(aDatosPers, nroUsu, fecha,
                                                   comercio, importe);
    else
      okTarjeta = Operacion::RegistrarMovimientoTC(aDatosPers, nroUsu, fecha,
                                                   comercio, cuotas, nroCpbte,
                                                   importe);
  }

  // ----- Resultado de las dos grabaciones -----
  // Se informa con precision cual de las dos fallo, para que el usuario no se
  // quede con informacion equivocada sobre lo que quedo registrado.
  if (not okCuenta) {
    Operacion::Error("No se pudo debitar la Caja de Ahorro"
                     " (MovimientosCA.Txt).");
    cout << "  No se registro ningun consumo en la tarjeta." << endl;
    cout << "  El saldo de la cuenta NO fue modificado." << endl;
    Operacion::Continuar();
    return;
  }
  if (not okTarjeta) {
    Operacion::Error(modoPago == PAGO_DEBITO
                     ? "No se pudo grabar el consumo en MovimientosTD.Txt."
                     : "No se pudo grabar el consumo en MovimientosTC.Txt.");
    cout << "  ATENCION: el debito en la Caja de Ahorro SI quedo registrado."
         << endl;
    cout << "  El consumo no figurara en el resumen de la tarjeta." << endl;
  }
  else {
    Operacion::Exito("La compra se registro en la cuenta y en la tarjeta.");
  }

  // ----- Comprobante -----
  saldoActual = Operacion::SaldoCA(aDatosPers, nroUsu);
  cout << endl;
  Screen::_textcolor(COL_TITULO);
  cout << "  COMPROBANTE DE COMPRA" << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_LINEA, '=');
  cout << endl;
  Screen::ColorNormal();

  Screen::_textcolor(COL_ROTULO);
  cout << "  Fecha..............: ";
  Screen::_textcolor(COL_DATO);
  cout << fechaTexto << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Descripcion........: ";
  Screen::_textcolor(COL_DATO);
  cout << descripcion << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Comercio...........: ";
  Screen::_textcolor(COL_DATO);
  cout << comercio << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Modo de pago.......: ";
  Screen::_textcolor(COL_DATO);
  cout << (modoPago == PAGO_DEBITO ? "Tarjeta de Debito"
                                   : "Tarjeta de Credito") << endl;
  if (modoPago == PAGO_CREDITO) {
    Screen::_textcolor(COL_ROTULO);
    cout << "  Cuotas.............: ";
    Screen::_textcolor(COL_DATO);
    cout << cuotas << endl;
    Screen::_textcolor(COL_ROTULO);
    cout << "  Nro. comprobante...: ";
    Screen::_textcolor(COL_DATO);
    cout << nroCpbte << endl;
  }
  Screen::_textcolor(COL_ROTULO);
  cout << "  Importe............: $";
  Screen::_textcolor(COL_DATO);
  Operacion::Importe(importe, ANC_IMP);
  cout << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Saldo anterior.....: $";
  Screen::_textcolor(COL_DATO);
  Operacion::Importe(saldoAnterior, ANC_IMP);
  cout << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Saldo actualizado..: $";
  Screen::_textcolor(COL_EXITO);
  Operacion::Importe(saldoActual, ANC_IMP);
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_LINEA, '=');
  cout << endl;
  Screen::ColorNormal();

  Operacion::Continuar();
} // Menu_Compras

#endif // MENU_COMPRAS_HPP
