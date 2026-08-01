// ---------------------------------------------------------------------------
// Menu_Deposito.hpp
// Nivel 2 - Opcion <<Deposito>> del menu principal.
//
// Registra un deposito en la Caja de Ahorro en pesos del usuario. Segun el
// enunciado el deposito se compone de la fecha (dia, mes y anio), la
// descripcion y el importe.
//
// El movimiento se graba en MovimientosCA.Txt con el tipo 'D' (Debe), que en
// el formato de la catedra es el que INCREMENTA el saldo del titular. El saldo
// no se actualiza a mano: Operacion::RegistrarMovimientoCA() lo recalcula
// releyendo el archivo, de modo que pantalla y archivo nunca queden
// desfasados.
// ---------------------------------------------------------------------------
#ifndef MENU_DEPOSITO_HPP
#define MENU_DEPOSITO_HPP

// ---------------------------------------------------------------------------
// Pide la fecha del deposito y la devuelve por parametro.
//
// Primero ofrece la fecha de hoy (el caso mas frecuente, asi el usuario no
// tiene que tipear tres numeros); si la rechaza, la pide campo por campo.
// Un deposito no puede tener fecha futura: la validacion se hace comparando
// las dos fechas convertidas a un unico entero aaaammdd con FechaLarga(), que
// es la forma mas simple de compararlas cronologicamente. Ciclo 1-x: se
// insiste hasta obtener una fecha aceptable o hasta que se agote la entrada.
// ---------------------------------------------------------------------------
static void Deposito_LeerFecha(Fecha &fecha) {
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
      Validar::LeerFecha("  Fecha del deposito:", fecha);
    aceptada = FechaHora::FechaLarga(fecha) <= FechaHora::FechaLarga(hoy);
    if (not aceptada and not Validar::EntradaAgotada())
      Operacion::Error("La fecha no puede ser posterior a la de hoy.");
  } while (not aceptada and not Validar::EntradaAgotada());
} // Deposito_LeerFecha

// ---------------------------------------------------------------------------
// Pantalla de operacion del deposito.
// ---------------------------------------------------------------------------
void Menu_Deposito(DatosPers aDatosPers[], short nroUsu) {
  const double MIN_DEPOSITO = 1.00; // no tiene sentido depositar menos de $1
  const short ANC_IMP      = 14;    // ancho de columna de los importes
  const short ANC_LINEA    = 60;    // ancho de las lineas del comprobante
  str25 descripcion;
  char  fechaTexto[ANCHO_FECHA + 1];
  Fecha fecha;
  double importe, saldoAnterior, saldoActual;
  bool  confirma;

  Operacion::PantallaOperacion("DEPOSITO EN CAJA DE AHORRO", aDatosPers,
                               nroUsu);

  // ----- Situacion de la cuenta antes de la operacion -----
  saldoAnterior = Operacion::SaldoCA(aDatosPers, nroUsu);
  Screen::_textcolor(COL_ROTULO);
  cout << "  Cuenta de destino..: ";
  Screen::_textcolor(COL_DATO);
  cout << aDatosPers[nroUsu].nroCtaCA << endl;
  Screen::_textcolor(COL_ROTULO);
  cout << "  Saldo actual.......: $";
  Screen::_textcolor(COL_DATO);
  Operacion::Importe(saldoAnterior, ANC_IMP);
  Screen::ColorNormal();
  cout << endl << endl;

  // ----- Datos del deposito -----
  Deposito_LeerFecha(fecha);
  if (Validar::EntradaAgotada())
    return;

  // La descripcion se normaliza a ASCII porque se graba en un archivo de
  // formato tabular: una letra acentuada ocuparia dos bytes y correria las
  // columnas del registro.
  Validar::LeerCadena("  Descripcion........: ", descripcion, LARGO_DETALLE,
                      true);
  importe = Validar::LeerReal("  Importe a depositar $: ", MIN_DEPOSITO,
                              Archivo::MAX_IMPORTE);
  if (Validar::EntradaAgotada())
    return;

  // ----- Confirmacion -----
  FechaHora::FormatearFechaCeros(fecha, fechaTexto);
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_LINEA, '-');
  cout << endl;
  Screen::ColorNormal();
  cout << "  Se acreditara en la cuenta " << aDatosPers[nroUsu].nroCtaCA
       << endl;
  cout << "  " << fechaTexto << "  " << descripcion << "   $";
  Operacion::Importe(importe, ANC_IMP);
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_LINEA, '-');
  cout << endl;
  Screen::ColorNormal();

  confirma = Validar::LeerSiNo("  Confirma el deposito (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("El deposito fue cancelado por el usuario.");
    Operacion::Continuar();
    return;
  }

  // ----- Impacto en la Caja de Ahorro -----
  if (not Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, fecha,
                                           MOV_ACREDITA, descripcion,
                                           importe)) {
    Operacion::Error("No se pudo grabar el movimiento en MovimientosCA.Txt.");
    cout << "  Verifique que el archivo exista y que no este protegido contra"
         << " escritura." << endl;
    cout << "  El saldo de la cuenta NO fue modificado." << endl;
    Operacion::Continuar();
    return;
  }

  // ----- Comprobante -----
  saldoActual = Operacion::SaldoCA(aDatosPers, nroUsu);
  Operacion::Exito("El deposito se acredito en la Caja de Ahorro.");
  cout << endl;
  Screen::_textcolor(COL_TITULO);
  cout << "  COMPROBANTE DE DEPOSITO" << endl;
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
  cout << "  Importe depositado.: $";
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
} // Menu_Deposito

#endif // MENU_DEPOSITO_HPP
