// ---------------------------------------------------------------------------
// Menu_ActualizarMov.hpp
// Nivel 3 - ACTUALIZACION DE COMPONENTES DE LOS ARCHIVOS.
//
// El enunciado incluye, entre las operaciones a llevar a cabo, la de
// <<actualizar componentes de archivo>>. Estos dos modulos son los que la
// ejercitan: no agregan ni eliminan registros, sino que REESCRIBEN EN EL LUGAR
// una componente que ya existe.
//
// Eso solo es posible porque todos los registros ocupan exactamente la misma
// cantidad de bytes: se calcula el desplazamiento del registro
// (nroReg x largoRegistro), se posiciona el puntero del archivo con seekp() y
// se sobrescriben esos bytes. Si los registros fueran de largo variable habria
// que reescribir el archivo completo.
//
//   Menu_EditarDetalleCA()  cambia la descripcion de un movimiento de la Caja
//                           de Ahorro, sin tocar la fecha ni el importe (de
//                           modo que el saldo NO se altera).
//   Menu_PagarCuotaTC()     avanza el contador de cuotas de una compra con
//                           tarjeta de credito: "2/6" pasa a "3/6".
//
// En los dos casos se verifica que la componente que se va a modificar
// pertenezca al usuario que inicio la sesion: nadie puede tocar los
// movimientos de otro.
// ---------------------------------------------------------------------------
#ifndef MENU_ACTUALIZARMOV_HPP
#define MENU_ACTUALIZARMOV_HPP

const short ACTMOV_ANC_ROTULO = 28;
const short ACTMOV_ANC_IMP    = 14;

// ---------------------------------------------------------------------------
// Rotulo alineado en columna.
// ---------------------------------------------------------------------------
static void ActMov_Rotulo(const char rotulo[]) {
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto(rotulo, ACTMOV_ANC_ROTULO, 'i');
  cout << ' ';
  Screen::_textcolor(COL_DATO);
} // ActMov_Rotulo

// ---------------------------------------------------------------------------
// Lista los movimientos de Caja de Ahorro del usuario, numerados, indicando el
// numero de registro que ocupa cada uno DENTRO DEL ARCHIVO. Ese numero es el
// que despues se usa para el acceso aleatorio.
// ---------------------------------------------------------------------------
static void ActMov_ListarCA(MovCA aMov[], short cant) {
  const short ANC_NRO = 6, ANC_REG = 10, ANC_DET = 27;
  char        fecha[ANCHO_FECHA + 1];
  char        numero[ANC_NRO + 1];
  char        registro[ANC_REG + 1];

  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto("Nro.", ANC_NRO, 'i');
  Screen::Texto("Registro", ANC_REG, 'i');
  Screen::Texto("Fecha", ANCHO_FECHA, 'i');
  cout << " T ";
  Screen::Texto("Detalle", ANC_DET, 'i');
  Screen::Texto("Importe", ACTMOV_ANC_IMP, 'd');
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_NRO + ANC_REG + ANCHO_FECHA + 3 + ANC_DET +
                    ACTMOV_ANC_IMP, '-');
  cout << endl;
  Screen::ColorNormal();

  for (short i = 0; i < cant; i++) {
    FechaHora::FormatearFecha(aMov[i].fecha, fecha);
    snprintf(numero,   sizeof numero,   "%d", i + 1);
    snprintf(registro, sizeof registro, "%d", aMov[i].nroReg + 1);
    cout << "  ";
    Screen::Texto(numero,   ANC_NRO, 'i');
    Screen::Texto(registro, ANC_REG, 'i');
    Screen::Texto(fecha,    ANCHO_FECHA, 'i');
    cout << ' ' << aMov[i].tipoMov << ' ';
    Screen::Texto(aMov[i].detalle, ANC_DET, 'i');
    Operacion::Importe(aMov[i].importe, ACTMOV_ANC_IMP);
    cout << endl;
  }
  cout << endl << "  Cantidad de movimientos: " << cant << endl;
} // ActMov_ListarCA

// ---------------------------------------------------------------------------
// Nivel 3 - Corrige la descripcion de un movimiento de la Caja de Ahorro.
// ---------------------------------------------------------------------------
void Menu_EditarDetalleCA(DatosPers aDatosPers[], short nroUsu) {
  static MovCA aMov[MAX_MOV_CA];
  MovCA        movArchivo;
  str25        detalleNuevo;
  char         mensaje[2 * LARGO_DETALLE + LARGO_NOMBRE + 1];
  short        cant, elegido;
  bool         confirma;

  Operacion::PantallaOperacion("CORREGIR LA DESCRIPCION DE UN MOVIMIENTO",
                               aDatosPers, nroUsu);

  // ACCESO SECUENCIAL: se leen los movimientos del usuario y, de paso, queda
  // registrado en cada componente el numero de registro que ocupa en el
  // archivo (campo nroReg).
  cant = Archivo::LeerMovimientosCA(aMov, aDatosPers[nroUsu].nroUsuario,
                                    MAX_MOV_CA);
  if (cant == 0) {
    Operacion::Aviso("Todavia no tiene movimientos en la Caja de Ahorro.");
    Operacion::Continuar();
    return;
  }

  cout << "  Se cambia solamente el texto de la descripcion: la fecha, el tipo"
       << endl;
  cout << "  y el importe no se tocan, de modo que el saldo no se altera."
       << endl << endl;
  ActMov_ListarCA(aMov, cant);
  cout << endl;

  elegido = (short) Validar::LeerEntero(
      "  Nro. de movimiento a corregir (0 = cancelar): ", 0, cant);
  if (Validar::EntradaAgotada())
    return;
  if (elegido == 0) {
    Operacion::Aviso("No se modifico ningun movimiento.");
    Operacion::Continuar();
    return;
  }

  // ACCESO ALEATORIO: se relee del archivo justo la componente elegida, sin
  // recorrer las anteriores, para trabajar sobre el dato tal como esta grabado.
  if (not Archivo::LeerRegistroCA(aMov[elegido - 1].nroReg, movArchivo)) {
    Operacion::Error("No se pudo leer el registro en el archivo.");
    Operacion::Continuar();
    return;
  }

  // Resguardo: la componente tiene que ser del usuario de la sesion.
  if (movArchivo.nroUsuario != aDatosPers[nroUsu].nroUsuario) {
    Operacion::Error("Ese movimiento no le pertenece.");
    Operacion::Continuar();
    return;
  }

  cout << endl;
  ActMov_Rotulo("Nro. de registro:");
  cout << movArchivo.nroReg + 1 << endl;
  ActMov_Rotulo("Descripcion actual:");
  cout << movArchivo.detalle << endl;
  cout << endl;

  Validar::LeerCadena("  Descripcion nueva.........: ", detalleNuevo,
                      LARGO_DETALLE, true);
  if (Validar::EntradaAgotada())
    return;
  if (strcmp(detalleNuevo, movArchivo.detalle) == 0) {
    Operacion::Aviso("La descripcion nueva es igual a la anterior.");
    Operacion::Continuar();
    return;
  }

  snprintf(mensaje, sizeof mensaje, "  Cambiar \"%s\" por \"%s\" (S/N)?: ",
           movArchivo.detalle, detalleNuevo);
  confirma = Validar::LeerSiNo(mensaje);
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("No se modifico ningun movimiento.");
    Operacion::Continuar();
    return;
  }

  // ACTUALIZACION DE LA COMPONENTE: se sobrescribe el registro en el lugar que
  // ya ocupa dentro del archivo.
  Validar::CopiarCadena(movArchivo.detalle, detalleNuevo, LARGO_DETALLE);
  if (Archivo::ActualizarRegistroCA(movArchivo.nroReg, movArchivo)) {
    Operacion::Exito("El movimiento se actualizo en MovimientosCA.Txt.");
    ActMov_Rotulo("Descripcion nueva:");
    cout << movArchivo.detalle << endl;
    ActMov_Rotulo("Saldo (sin cambios):");
    cout << "$";
    Operacion::Importe(Operacion::RecalcularSaldoCA(aDatosPers, nroUsu),
                       ACTMOV_ANC_IMP);
    cout << endl;
  }
  else
    Operacion::Error("No se pudo actualizar el registro en el archivo.");

  Operacion::Continuar();
} // Menu_EditarDetalleCA

// ---------------------------------------------------------------------------
// Interpreta unas cuotas con formato "x/y". Devuelve false si el campo esta
// vacio o no tiene ese formato (compra en un solo pago).
// ---------------------------------------------------------------------------
static bool ActMov_LeerCuotas(const char cuotas[], short &pagada,
                              short &total) {
  int x = 0, y = 0;

  if (cuotas[0] == '\0')
    return false;
  if (sscanf(cuotas, "%d/%d", &x, &y) != 2)
    return false;
  if (x < 1 or y < 1 or x > y or y > MAX_CUOTAS_TC)
    return false;
  pagada = (short) x;
  total  = (short) y;
  return true;
} // ActMov_LeerCuotas

// ---------------------------------------------------------------------------
// Lista los consumos en cuotas del usuario y devuelve la cantidad listada.
// En <<aIndice>> deja, por cada renglon emitido, la posicion que ocupa dentro
// de aMov, para poder resolver despues cual eligio el usuario.
// ---------------------------------------------------------------------------
static short ActMov_ListarCuotas(MovTC aMov[], short cant, short aIndice[]) {
  const short ANC_NRO = 6, ANC_REG = 10, ANC_DET = 27, ANC_CUO = 8;
  char        fecha[ANCHO_FECHA + 1];
  char        numero[ANC_NRO + 1];
  char        registro[ANC_REG + 1];
  short       pagada, total, listados = 0;

  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto("Nro.", ANC_NRO, 'i');
  Screen::Texto("Registro", ANC_REG, 'i');
  Screen::Texto("Fecha", ANCHO_FECHA, 'i');
  cout << ' ';
  Screen::Texto("Comercio", ANC_DET, 'i');
  Screen::Texto("Cuotas", ANC_CUO, 'd');
  Screen::Texto("Importe", ACTMOV_ANC_IMP, 'd');
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(ANC_NRO + ANC_REG + ANCHO_FECHA + 1 + ANC_DET + ANC_CUO +
                    ACTMOV_ANC_IMP, '-');
  cout << endl;
  Screen::ColorNormal();

  for (short i = 0; i < cant; i++)
    if (ActMov_LeerCuotas(aMov[i].cuotas, pagada, total)) {
      aIndice[listados] = i;
      listados++;
      FechaHora::FormatearFecha(aMov[i].fecha, fecha);
      snprintf(numero,   sizeof numero,   "%d", listados);
      snprintf(registro, sizeof registro, "%d", aMov[i].nroReg + 1);
      cout << "  ";
      Screen::Texto(numero,   ANC_NRO, 'i');
      Screen::Texto(registro, ANC_REG, 'i');
      Screen::Texto(fecha,    ANCHO_FECHA, 'i');
      cout << ' ';
      Screen::Texto(aMov[i].detalle, ANC_DET, 'i');
      Screen::Texto(aMov[i].cuotas,  ANC_CUO, 'd');
      Operacion::Importe(aMov[i].importe, ACTMOV_ANC_IMP);
      cout << endl;
    }
  cout << endl << "  Compras en cuotas: " << listados << endl;
  return listados;
} // ActMov_ListarCuotas

// ---------------------------------------------------------------------------
// Nivel 3 - Registra el pago de una cuota de una compra con tarjeta de credito.
// ---------------------------------------------------------------------------
void Menu_PagarCuotaTC(DatosPers aDatosPers[], short nroUsu) {
  static MovTC aMov[MAX_MOV_TC];
  static short aIndice[MAX_MOV_TC];
  MovTC        movArchivo;
  str5         cuotasNuevas;
  short        cant, enCuotas, elegido, pagada, total;
  bool         confirma;

  Operacion::PantallaOperacion("REGISTRAR EL PAGO DE UNA CUOTA", aDatosPers,
                               nroUsu);

  cant = Archivo::LeerMovimientosTC(aMov, aDatosPers[nroUsu].nroUsuario,
                                    MAX_MOV_TC);
  if (cant == 0) {
    Operacion::Aviso("No tiene consumos con tarjeta de credito.");
    Operacion::Continuar();
    return;
  }

  cout << "  Al pagar una cuota se actualiza el contador del consumo: una"
       << endl;
  cout << "  compra en \"2/6\" pasa a \"3/6\". El importe no se modifica."
       << endl << endl;

  enCuotas = ActMov_ListarCuotas(aMov, cant, aIndice);
  if (enCuotas == 0) {
    Operacion::Aviso("Ninguno de sus consumos esta financiado en cuotas.");
    Operacion::Continuar();
    return;
  }
  cout << endl;

  elegido = (short) Validar::LeerEntero(
      "  Nro. de consumo a actualizar (0 = cancelar): ", 0, enCuotas);
  if (Validar::EntradaAgotada())
    return;
  if (elegido == 0) {
    Operacion::Aviso("No se actualizo ningun consumo.");
    Operacion::Continuar();
    return;
  }

  // ACCESO ALEATORIO a la componente elegida.
  if (not Archivo::LeerRegistroTC(aMov[aIndice[elegido - 1]].nroReg,
                                  movArchivo)) {
    Operacion::Error("No se pudo leer el registro en el archivo.");
    Operacion::Continuar();
    return;
  }
  if (movArchivo.nroUsuario != aDatosPers[nroUsu].nroUsuario) {
    Operacion::Error("Ese consumo no le pertenece.");
    Operacion::Continuar();
    return;
  }
  if (not ActMov_LeerCuotas(movArchivo.cuotas, pagada, total)) {
    Operacion::Error("El consumo no esta financiado en cuotas.");
    Operacion::Continuar();
    return;
  }

  cout << endl;
  ActMov_Rotulo("Comercio:");
  cout << movArchivo.detalle << endl;
  ActMov_Rotulo("Cuotas pagadas:");
  cout << pagada << " de " << total << endl;

  if (pagada >= total) {
    Operacion::Aviso("Esta compra ya esta totalmente paga.");
    Operacion::Continuar();
    return;
  }

  ActMov_Rotulo("Proxima cuota:");
  cout << pagada + 1 << " de " << total << endl;
  ActMov_Rotulo("Valor de la cuota:");
  cout << "$";
  Operacion::Importe(movArchivo.importe / total, ACTMOV_ANC_IMP);
  cout << endl << endl;

  confirma = Validar::LeerSiNo("  Confirma el pago de la cuota (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("No se actualizo ningun consumo.");
    Operacion::Continuar();
    return;
  }

  // ACTUALIZACION DE LA COMPONENTE en el archivo de Tarjeta de Credito.
  snprintf(cuotasNuevas, sizeof cuotasNuevas, "%d/%d", pagada + 1, total);
  Validar::CopiarCadena(movArchivo.cuotas, cuotasNuevas, LARGO_CUOTAS);
  if (Archivo::ActualizarRegistroTC(movArchivo.nroReg, movArchivo)) {
    Operacion::Exito("El consumo se actualizo en MovimientosTC.Txt.");
    ActMov_Rotulo("Cuotas ahora:");
    cout << movArchivo.cuotas << endl;
    if (pagada + 1 == total)
      Operacion::Aviso("Con esta cuota la compra queda totalmente paga.");
  }
  else
    Operacion::Error("No se pudo actualizar el registro en el archivo.");

  Operacion::Continuar();
} // Menu_PagarCuotaTC

#endif // MENU_ACTUALIZARMOV_HPP
