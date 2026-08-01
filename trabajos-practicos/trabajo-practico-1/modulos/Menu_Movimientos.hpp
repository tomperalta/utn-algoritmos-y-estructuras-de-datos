// ---------------------------------------------------------------------------
// Menu_Movimientos.hpp
// Nivel 2 - Opcion <<Movimientos de cuentas y tarjetas>> del menu principal.
//
// Submenu de consulta de los tres archivos de movimientos:
//
//   1. Movimientos de Caja de Ahorro    -> Menu_MovimientosCA()
//   2. Movimientos de Tarjeta de Debito -> Menu_MovimientosTD()
//   3. Movimientos de Tarjeta de Credito-> Menu_MovimientosTC()
//   4. Buscar un movimiento de CA por numero de registro
//                                       -> Menu_BuscarMovimientoCA()
//   5. Volver al menu principal
//
// Las dos formas de acceso a los archivos que pide el enunciado quedan a la
// vista en este modulo:
//
//   * ACCESO SECUENCIAL: las tres primeras opciones recorren el archivo de
//     principio a fin con Archivo::LeerMovimientos*(), quedandose con los
//     movimientos del usuario que inicio sesion. Es lo que corresponde porque
//     se necesitan TODAS las componentes del usuario.
//   * ACCESO ALEATORIO : la cuarta opcion lee UNA sola componente cualquiera,
//     sin recorrer las anteriores, calculando su desplazamiento dentro del
//     archivo (nroReg * largo del registro). Solo es posible porque todos los
//     registros ocupan la misma cantidad de bytes.
//
// Los movimientos leidos se ordenan por fecha con el metodo de la BURBUJA
// OPTIMIZADA (Ordenar::Mov*porFecha): el archivo se graba en el orden en que
// se producen las operaciones, que no es necesariamente el cronologico, porque
// cada usuario agrega sus componentes al final del archivo comun.
// ---------------------------------------------------------------------------
#ifndef MENU_MOVIMIENTOS_HPP
#define MENU_MOVIMIENTOS_HPP

// ---------------------------------------------------------------------------
// Emite el rotulo de un dato y deja el color preparado para el valor.
// Auxiliar propia de este modulo.
// ---------------------------------------------------------------------------
static void Movimientos_Rotulo(const char rotulo[]) {
  const short ANC_ROTULO = 28;

  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, ANC_ROTULO, 'i');
  cout << ' ';
  Screen::_textcolor(COL_DATO);
} // Movimientos_Rotulo

static void Movimientos_DatoTexto(const char rotulo[], const char valor[]) {
  Movimientos_Rotulo(rotulo);
  cout << valor << endl;
  Screen::ColorNormal();
} // Movimientos_DatoTexto

static void Movimientos_DatoEntero(const char rotulo[], long valor) {
  Movimientos_Rotulo(rotulo);
  cout << valor << endl;
  Screen::ColorNormal();
} // Movimientos_DatoEntero

static void Movimientos_DatoImporte(const char rotulo[], double valor) {
  const short ANC_IMPORTE = 16;

  Movimientos_Rotulo(rotulo);
  cout << "$";
  Operacion::Importe(valor, ANC_IMPORTE);
  cout << endl;
  Screen::ColorNormal();
} // Movimientos_DatoImporte

// ---------------------------------------------------------------------------
// Muestra, con rotulos, todos los campos de un movimiento de Caja de Ahorro
// leido del archivo, incluido el usuario al que pertenece.
// ---------------------------------------------------------------------------
static void Movimientos_MostrarRegistroCA(DatosPers aDatosPers[], MovCA mov) {
  char fecha[ANCHO_FECHA + 1];
  char fechaLetras[Validar::LARGO_BUFFER];
  char tipo[Validar::LARGO_BUFFER];
  char titular[LARGO_NOMBRE + 1];

  FechaHora::FormatearFechaCeros(mov.fecha, fecha);
  FechaHora::FormatearFechaLetras(mov.fecha, fechaLetras,
                                  Validar::LARGO_BUFFER);
  snprintf(tipo, sizeof tipo, "%c   (%s)", mov.tipoMov,
           mov.tipoMov == MOV_ACREDITA ? "Debe: acredita el saldo"
                                       : "Haber: debita el saldo");
  // El numero de usuario grabado en el archivo va de 1 a MAX_USUARIOS, de modo
  // que su posicion dentro del array de usuarios es nroUsuario - 1.
  if (mov.nroUsuario >= 1 and mov.nroUsuario <= MAX_USUARIOS)
    Validar::CopiarCadena(titular, aDatosPers[mov.nroUsuario - 1].apellidoNombre,
                          LARGO_NOMBRE);
  else
    Validar::CopiarCadena(titular, "(usuario inexistente)", LARGO_NOMBRE);

  Movimientos_DatoEntero("Nro. de registro (base 0)..:", mov.nroReg);
  Movimientos_DatoTexto ("Fecha......................:", fecha);
  Movimientos_DatoTexto ("Fecha en letras............:", fechaLetras);
  Movimientos_DatoTexto ("Tipo de movimiento.........:", tipo);
  Movimientos_DatoTexto ("Detalle....................:", mov.detalle);
  Movimientos_DatoImporte("Importe....................:", mov.importe);
  Movimientos_DatoEntero("Nro. de usuario duenio.....:", mov.nroUsuario);
  Movimientos_DatoTexto ("Titular....................:", titular);
} // Movimientos_MostrarRegistroCA

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de movimientos.
// Ciclo de construccion de menu (1-x): se repite hasta elegir Volver, cancelar
// con ESCAPE o agotarse la entrada.
// ---------------------------------------------------------------------------
void Menu_Movimientos(DatosPers aDatosPers[], short nroUsu) {
  const short CANT_OPC = 6;
  str40Opc aMenu[CANT_OPC] = {"Movimientos de Caja de Ahorro",
                              "Movimientos de Tarjeta de Debito",
                              "Movimientos de Tarjeta de Credito",
                              "Buscar movimiento de CA por registro",
                              "Corregir descripcion de un movimiento",
                              "Volver al menu principal"};
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("MOVIMIENTOS DE CUENTAS Y TARJETAS",
                                      aDatosPers, nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {                          // seleccion multiple
      case 0 : Menu_MovimientosCA(aDatosPers, nroUsu);        break;
      case 1 : Menu_MovimientosTD(aDatosPers, nroUsu);        break;
      case 2 : Menu_MovimientosTC(aDatosPers, nroUsu);        break;
      case 3 : Menu_BuscarMovimientoCA(aDatosPers, nroUsu);   break;
      case 4 : Menu_EditarDetalleCA(aDatosPers, nroUsu);      break;
      default: volver = true;                  // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_Movimientos

// ---------------------------------------------------------------------------
// Nivel 3 - Movimientos de la Caja de Ahorro en pesos.
// Lectura SECUENCIAL del archivo, ordenamiento por fecha con burbuja
// optimizada y emision de la tabla con el saldo acumulado movimiento a
// movimiento a partir del saldo inicial de la cuenta.
// ---------------------------------------------------------------------------
void Menu_MovimientosCA(DatosPers aDatosPers[], short nroUsu) {
  MovCA aMov[MAX_MOV_CA];
  short cant, iCta;
  double saldoInicial = 0;

  Operacion::PantallaOperacion("MOVIMIENTOS DE CAJA DE AHORRO", aDatosPers,
                               nroUsu);

  iCta = Operacion::IndiceCuentaCA(aDatosPers, nroUsu);
  if (iCta != NO_ENCONTRADO)
    saldoInicial = aDatosPers[nroUsu].aCuentas[iCta].saldoInicial;

  // El segundo argumento es el nro. de usuario del archivo (1..5), no el
  // indice del array de usuarios.
  cant = Archivo::LeerMovimientosCA(aMov, aDatosPers[nroUsu].nroUsuario,
                                    MAX_MOV_CA);
  if (cant == 0)
    Operacion::Aviso("La Caja de Ahorro no registra movimientos.");
  else {
    Ordenar::MovCAporFecha(aMov, cant);        // burbuja optimizada
    Operacion::TablaCA(aMov, cant, saldoInicial, aDatosPers[nroUsu].nroCtaCA,
                       "$");
  }
  Operacion::Continuar();
} // Menu_MovimientosCA

// ---------------------------------------------------------------------------
// Nivel 3 - Movimientos de la Tarjeta de Debito.
// ---------------------------------------------------------------------------
void Menu_MovimientosTD(DatosPers aDatosPers[], short nroUsu) {
  MovTD aMov[MAX_MOV_TD];
  short cant;

  Operacion::PantallaOperacion("MOVIMIENTOS DE TARJETA DE DEBITO", aDatosPers,
                               nroUsu);

  cant = Archivo::LeerMovimientosTD(aMov, aDatosPers[nroUsu].nroUsuario,
                                    MAX_MOV_TD);
  if (cant == 0)
    Operacion::Aviso("La Tarjeta de Debito no registra consumos.");
  else {
    Ordenar::MovTDporFecha(aMov, cant);        // burbuja optimizada
    Operacion::TablaTD(aMov, cant);
  }
  Operacion::Continuar();
} // Menu_MovimientosTD

// ---------------------------------------------------------------------------
// Nivel 3 - Movimientos de la Tarjeta de Credito.
// ---------------------------------------------------------------------------
void Menu_MovimientosTC(DatosPers aDatosPers[], short nroUsu) {
  MovTC aMov[MAX_MOV_TC];
  short cant;

  Operacion::PantallaOperacion("MOVIMIENTOS DE TARJETA DE CREDITO", aDatosPers,
                               nroUsu);

  cant = Archivo::LeerMovimientosTC(aMov, aDatosPers[nroUsu].nroUsuario,
                                    MAX_MOV_TC);
  if (cant == 0)
    Operacion::Aviso("La Tarjeta de Credito no registra consumos.");
  else {
    Ordenar::MovTCporFecha(aMov, cant);        // burbuja optimizada
    Operacion::TablaTC(aMov, cant);
  }
  Operacion::Continuar();
} // Menu_MovimientosTC

// ---------------------------------------------------------------------------
// Nivel 3 - Consulta de UNA componente del archivo de Caja de Ahorro por su
// numero de registro: es la demostracion del ACCESO ALEATORIO.
//
// Se informa la cantidad de registros y el largo en bytes de cada uno, que es
// el dato que hace posible el calculo del desplazamiento, y se lee el registro
// pedido con Archivo::LeerRegistroCA() sin recorrer los anteriores.
//
// Como el archivo es comun a los cinco usuarios, el registro consultado puede
// pertenecer a otro titular: eso no es un error, sino un dato del archivo, y
// se informa con un aviso.
//
// El ciclo es 1-x y corta por decision del usuario o por fin de la entrada.
// ---------------------------------------------------------------------------
void Menu_BuscarMovimientoCA(DatosPers aDatosPers[], short nroUsu) {
  MovCA mov;
  long  cantReg, largoReg, nroReg;
  bool  otra = false;

  do {
    Operacion::PantallaOperacion("BUSCAR MOVIMIENTO DE CA (ACCESO ALEATORIO)",
                                 aDatosPers, nroUsu);

    cantReg  = Archivo::CantidadRegistros(ARCH_MOV_CA);
    largoReg = Archivo::LargoRegistro(ARCH_MOV_CA);

    Movimientos_DatoTexto ("Archivo....................:", ARCH_MOV_CA);
    Movimientos_DatoEntero("Cantidad de registros......:", cantReg);
    Movimientos_DatoEntero("Largo del registro (bytes).:", largoReg);
    Movimientos_DatoEntero("Su nro. de usuario.........:",
                           aDatosPers[nroUsu].nroUsuario);
    cout << endl;

    if (cantReg == 0) {
      Operacion::Aviso("El archivo de movimientos esta vacio.");
      Operacion::Continuar();
      return;
    }

    nroReg = Validar::LeerEntero("  Nro. de registro a consultar: ", 1,
                                 cantReg);
    if (Validar::EntradaAgotada())
      return;

    cout << endl;
    // Se resta 1 porque el usuario numera los registros desde 1 y el archivo
    // los ubica desde 0.
    if (not Archivo::LeerRegistroCA(nroReg - 1, mov))
      Operacion::Error("No se pudo leer el registro solicitado.");
    else {
      Movimientos_DatoEntero("Nro. de registro pedido....:", nroReg);
      Movimientos_MostrarRegistroCA(aDatosPers, mov);
      if (mov.nroUsuario == aDatosPers[nroUsu].nroUsuario)
        Operacion::Exito("El movimiento pertenece a su cuenta.");
      else
        Operacion::Aviso("El movimiento pertenece a otro usuario del banco.");
    }

    otra = Validar::LeerSiNo("  Desea consultar otro registro (S/N)?: ");
  } while (otra and not Validar::EntradaAgotada());
} // Menu_BuscarMovimientoCA

#endif // MENU_MOVIMIENTOS_HPP
