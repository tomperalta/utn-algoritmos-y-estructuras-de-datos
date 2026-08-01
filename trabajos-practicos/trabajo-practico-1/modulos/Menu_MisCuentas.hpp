// ---------------------------------------------------------------------------
// Menu_MisCuentas.hpp
// Nivel 2 - Opcion <<Mis Cuentas>> del menu principal.
//
// Submenu de consulta de las cuentas del usuario que inicio sesion:
//
//   1. Ver todas mis cuentas      -> listado completo (Operacion::ListarCuentas)
//   2. Caja de Ahorro en pesos    -> Menu_CuentasEnPesos()
//   3. Caja de Ahorro en dolares  -> Menu_CuentasEnDolares()
//   4. Volver al menu principal
//
// Dos decisiones que conviene justificar:
//
//   * El saldo de la Caja de Ahorro en pesos NO se conserva grabado en ningun
//     lado: se recalcula con Operacion::RecalcularSaldoCA(), que recorre el
//     archivo MovimientosCA.Txt en forma SECUENCIAL y le suma/resta cada
//     movimiento al saldo inicial. Asi la pantalla y el archivo nunca quedan
//     desfasados, aunque la sesion haya generado movimientos nuevos.
//   * La cuenta en pesos y la cuenta en dolares se ubican dentro del array de
//     cuentas del usuario con una BUSQUEDA SECUENCIAL por tipo y moneda
//     (Operacion::IndiceCuentaCA / IndiceCuentaDolares). Corresponde la
//     secuencial y no la binaria porque el array de cuentas es muy chico
//     (MAX_CUENTAS componentes) y no esta ordenado por esas claves, sino en el
//     orden en que las cuentas fueron abiertas.
// ---------------------------------------------------------------------------
#ifndef MENU_MISCUENTAS_HPP
#define MENU_MISCUENTAS_HPP

// ---------------------------------------------------------------------------
// Emite el rotulo de un dato y deja el color preparado para el valor.
// Auxiliar propia de este modulo: unifica el ancho de todos los rotulos para
// que los valores queden alineados en columna.
// ---------------------------------------------------------------------------
static void MisCuentas_Rotulo(const char rotulo[]) {
  const short ANC_ROTULO = 26;

  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, ANC_ROTULO, 'i');
  cout << ' ';
  Screen::_textcolor(COL_DATO);
} // MisCuentas_Rotulo

// ---------------------------------------------------------------------------
// Rotulo + valor de tipo cadena.
// ---------------------------------------------------------------------------
static void MisCuentas_DatoTexto(const char rotulo[], const char valor[]) {
  MisCuentas_Rotulo(rotulo);
  cout << valor << endl;
  Screen::ColorNormal();
} // MisCuentas_DatoTexto

// ---------------------------------------------------------------------------
// Rotulo + valor entero (por ejemplo, la cantidad de movimientos).
// ---------------------------------------------------------------------------
static void MisCuentas_DatoEntero(const char rotulo[], long valor) {
  MisCuentas_Rotulo(rotulo);
  cout << valor << endl;
  Screen::ColorNormal();
} // MisCuentas_DatoEntero

// ---------------------------------------------------------------------------
// Rotulo + importe con su moneda, ajustado a la derecha.
// ---------------------------------------------------------------------------
static void MisCuentas_DatoImporte(const char rotulo[], const char moneda[],
                                   double valor) {
  const short ANC_MONEDA = 4, ANC_IMPORTE = 16;

  MisCuentas_Rotulo(rotulo);
  Screen::Texto(moneda, ANC_MONEDA, 'i');
  Operacion::Importe(valor, ANC_IMPORTE);
  cout << endl;
  Screen::ColorNormal();
} // MisCuentas_DatoImporte

// ---------------------------------------------------------------------------
// Opcion 1 del submenu: todas las cuentas del usuario, con sus saldos al dia.
// ---------------------------------------------------------------------------
static void MisCuentas_VerTodas(DatosPers aDatosPers[], short nroUsu) {
  Operacion::PantallaOperacion("MIS CUENTAS - DETALLE DE TODAS LAS CUENTAS",
                               aDatosPers, nroUsu);
  // Antes de listar se pone al dia el saldo de la CA en pesos leyendo el
  // archivo: el resto de las cuentas conserva el saldo que tiene en memoria.
  Operacion::RecalcularSaldoCA(aDatosPers, nroUsu);
  Operacion::ListarCuentas(aDatosPers, nroUsu);
  MisCuentas_DatoTexto("Titular...................:",
                       aDatosPers[nroUsu].apellidoNombre);
  MisCuentas_DatoTexto("CBU de la cuenta principal:", aDatosPers[nroUsu].cbu);
  Operacion::Continuar();
} // MisCuentas_VerTodas

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de <<Mis Cuentas>>.
// El ciclo do..while es de construccion de menu (1-x): se repite hasta que se
// elija Volver, se cancele con ESCAPE o se agote la entrada.
// ---------------------------------------------------------------------------
void Menu_MisCuentas(DatosPers aDatosPers[], short nroUsu) {
  const short CANT_OPC = 4;
  str40Opc aMenu[CANT_OPC] = {"Ver todas mis cuentas",
                              "Caja de Ahorro en pesos",
                              "Caja de Ahorro en dolares",
                              "Volver al menu principal"};
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("MIS CUENTAS", aDatosPers, nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {                          // seleccion multiple
      case 0 : MisCuentas_VerTodas(aDatosPers, nroUsu);      break;
      case 1 : Menu_CuentasEnPesos(aDatosPers, nroUsu);      break;
      case 2 : Menu_CuentasEnDolares(aDatosPers, nroUsu);    break;
      default: volver = true;                  // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_MisCuentas

// ---------------------------------------------------------------------------
// Nivel 3 - Detalle de la Caja de Ahorro en pesos (cuenta principal).
//
// Muestra los datos bancarios que pide el enunciado (Nro.Cta, tipo, moneda,
// saldo, estado), el CBU del usuario, el saldo inicial, el saldo actual
// recalculado desde el archivo y la cantidad de movimientos que el archivo
// tiene registrados para este usuario.
// ---------------------------------------------------------------------------
void Menu_CuentasEnPesos(DatosPers aDatosPers[], short nroUsu) {
  MovCA aMov[MAX_MOV_CA];
  short iCta, cantMov;
  double saldoInicial, saldoActual;

  Operacion::PantallaOperacion("CAJA DE AHORRO EN PESOS", aDatosPers, nroUsu);

  // Busqueda secuencial de la cuenta "CA" en moneda "$" dentro del usuario.
  iCta = Operacion::IndiceCuentaCA(aDatosPers, nroUsu);
  if (iCta == NO_ENCONTRADO) {
    Operacion::Aviso("El usuario no posee Caja de Ahorro en pesos.");
    Operacion::Continuar();
    return;
  }

  saldoInicial = aDatosPers[nroUsu].aCuentas[iCta].saldoInicial;
  saldoActual  = Operacion::RecalcularSaldoCA(aDatosPers, nroUsu);
  // ACCESO SECUENCIAL al archivo. El filtro es aDatosPers[nroUsu].nroUsuario
  // (1..5), que identifica al duenio del movimiento dentro del archivo, y no
  // el indice nroUsu (0..4) del array de usuarios.
  cantMov = Archivo::LeerMovimientosCA(aMov, aDatosPers[nroUsu].nroUsuario,
                                       MAX_MOV_CA);

  MisCuentas_DatoTexto("Numero de cuenta..........:",
                       aDatosPers[nroUsu].aCuentas[iCta].nroCta);
  MisCuentas_DatoTexto("Tipo de cuenta............:",
                       aDatosPers[nroUsu].aCuentas[iCta].tipo);
  MisCuentas_DatoTexto("Moneda....................:",
                       aDatosPers[nroUsu].aCuentas[iCta].moneda);
  MisCuentas_DatoTexto("CBU.......................:", aDatosPers[nroUsu].cbu);
  MisCuentas_DatoTexto("Estado....................:",
                       aDatosPers[nroUsu].aCuentas[iCta].estado == 'A'
                       ? "Activa" : "Inactiva");
  cout << endl;
  MisCuentas_DatoImporte("Saldo inicial.............:",
                         aDatosPers[nroUsu].aCuentas[iCta].moneda,
                         saldoInicial);
  MisCuentas_DatoImporte("Saldo actual..............:",
                         aDatosPers[nroUsu].aCuentas[iCta].moneda,
                         saldoActual);
  MisCuentas_DatoEntero("Movimientos en el archivo.:", cantMov);

  if (cantMov == 0)
    Operacion::Aviso("La cuenta todavia no registra movimientos.");
  Operacion::Continuar();
} // Menu_CuentasEnPesos

// ---------------------------------------------------------------------------
// Nivel 3 - Detalle de la Caja de Ahorro en dolares.
//
// Ademas del saldo en u$s se informa su equivalente en pesos, calculado con
// COTIZ_DOLAR_VENTA, que es el precio al que el banco COMPRA los dolares: es
// el importe que recibiria el usuario si vendiera todo su saldo.
// ---------------------------------------------------------------------------
void Menu_CuentasEnDolares(DatosPers aDatosPers[], short nroUsu) {
  short iCta;
  double saldoDolares, equivalentePesos;

  Operacion::PantallaOperacion("CAJA DE AHORRO EN DOLARES", aDatosPers,
                               nroUsu);

  // Busqueda secuencial de la cuenta "CA" en moneda "u$s" dentro del usuario.
  iCta = Operacion::IndiceCuentaDolares(aDatosPers, nroUsu);
  if (iCta == NO_ENCONTRADO) {
    Operacion::Aviso("El usuario no posee Caja de Ahorro en dolares.");
    cout << "  Puede abrir una desde la opcion Crear nueva cuenta del menu "
         << "principal." << endl;
    Operacion::Continuar();
    return;
  }

  saldoDolares     = Operacion::SaldoDolares(aDatosPers, nroUsu);
  equivalentePesos = saldoDolares * COTIZ_DOLAR_VENTA;

  MisCuentas_DatoTexto("Numero de cuenta..........:",
                       aDatosPers[nroUsu].aCuentas[iCta].nroCta);
  MisCuentas_DatoTexto("Tipo de cuenta............:",
                       aDatosPers[nroUsu].aCuentas[iCta].tipo);
  MisCuentas_DatoTexto("Moneda....................:",
                       aDatosPers[nroUsu].aCuentas[iCta].moneda);
  MisCuentas_DatoTexto("Cuenta asociada en pesos..:",
                       aDatosPers[nroUsu].nroCtaCA);
  MisCuentas_DatoTexto("Estado....................:",
                       aDatosPers[nroUsu].aCuentas[iCta].estado == 'A'
                       ? "Activa" : "Inactiva");
  cout << endl;
  MisCuentas_DatoImporte("Saldo en dolares..........:",
                         aDatosPers[nroUsu].aCuentas[iCta].moneda,
                         saldoDolares);
  MisCuentas_DatoImporte("Cotizacion (banco compra).:", "$",
                         COTIZ_DOLAR_VENTA);
  MisCuentas_DatoImporte("Equivalente en pesos......:", "$",
                         equivalentePesos);

  if (saldoDolares == 0)
    Operacion::Aviso("La cuenta en dolares no tiene saldo disponible.");
  Operacion::Continuar();
} // Menu_CuentasEnDolares

#endif // MENU_MISCUENTAS_HPP
