// ---------------------------------------------------------------------------
// Menu_Transferir.hpp
// Nivel 2 - Transferencias de dinero desde la Caja de Ahorro en pesos.
//
// El submenu ofrece las dos formas de transferir que pide el enunciado y la
// consulta de las cuentas agendadas:
//
//   * a CBU o alias  -> Menu_TransferirCBU()
//   * a un celular   -> Menu_TransferirCelular()
//
// En los dos casos la operacion genera un nuevo movimiento en la Caja de
// Ahorro con tipo MOV_DEBITA ('H'), que disminuye el saldo, y se agrega al
// archivo MovimientosCA.Txt a traves de Operacion::RegistrarMovimientoCA().
//
// Busquedas empleadas
// -------------------
// La agenda de CBU de cada usuario se conserva ORDENADA POR ALIAS, porque las
// altas se hacen con Ordenar::InsertarContactoEnOrden() y las bajas compactan
// el array. Por eso:
//
//   * si el destino ingresado es un ALIAS, se lo ubica con BUSQUEDA BINARIA,
//     que resuelve en log2(n) comparaciones;
//   * si el destino ingresado es un CBU (22 digitos) o un numero de CELULAR,
//     se lo ubica con BUSQUEDA SECUENCIAL, porque ninguna de esas dos claves
//     es la clave de ordenamiento del array.
//
// Seguridad: por encima de TOPE_SIN_TOKEN la operacion exige token, tarea que
// resuelve Operacion::AutorizarOperacion().
// ---------------------------------------------------------------------------
#ifndef MENU_TRANSFERIR_HPP
#define MENU_TRANSFERIR_HPP

// ----- Parametros propios del modulo -----
const short TRANSF_CANT_OPC    =  4;    // opciones del submenu
const short TRANSF_ANC_MARCO   = 60;    // ancho de las lineas del comprobante
const short TRANSF_ANC_IMPORTE = 14;    // columnas del importe
const short TRANSF_ANC_HORA    =  8;    // "hh:mm:ss"
const short TRANSF_ANC_ALIAS   = 18;    // columna Alias del listado de celulares
const short TRANSF_ANC_TIT     = 28;    // columna Titular
const short TRANSF_ANC_CEL     = 16;    // columna Celular
const double TRANSF_MIN_IMPORTE = 1.00; // importe minimo de una transferencia

// ---------------------------------------------------------------------------
// Emite el comprobante de la transferencia ya realizada. El numero de
// operacion se toma del generador de numeros al azar de seis digitos que el
// sistema usa para los tokens (Operacion::GenerarToken).
// ---------------------------------------------------------------------------
static void TransferirComprobante(const char destino[], const char titular[],
                                  const char cbu[], double importe,
                                  double saldoFinal) {
  char  fecha[ANCHO_FECHA + 1];
  char  hora[TRANSF_ANC_HORA + 1];
  Fecha hoy;
  long  nroOperacion = Operacion::GenerarToken();

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);

  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(TRANSF_ANC_MARCO, '=');
  cout << endl;
  Screen::_textcolor(COL_TITULO);
  cout << "  COMPROBANTE DE TRANSFERENCIA" << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(TRANSF_ANC_MARCO, '-');
  cout << endl;
  Screen::ColorNormal();
  cout << "  Fecha............: " << fecha << "    Hora: " << hora << endl;
  cout << "  Destino..........: " << destino << endl;
  cout << "  Titular..........: " << titular << endl;
  cout << "  CBU..............: " << cbu << endl;
  cout << "  Importe..........: $";
  Operacion::Importe(importe, TRANSF_ANC_IMPORTE);
  cout << endl;
  cout << "  Saldo resultante.: $";
  Operacion::Importe(saldoFinal, TRANSF_ANC_IMPORTE);
  cout << endl;
  cout << "  Nro. de operacion: " << nroOperacion << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(TRANSF_ANC_MARCO, '=');
  cout << endl;
  Screen::ColorNormal();
} // TransferirComprobante

// ---------------------------------------------------------------------------
// Muestra la agenda del usuario con la columna Celular, que es el dato que
// hace falta para transferir a un telefono (Operacion::ListarAgenda muestra el
// CBU en su lugar).
// ---------------------------------------------------------------------------
static void TransferirListarCelulares(DatosPers aDatosPers[], short nroUsu) {
  short i;

  if (aDatosPers[nroUsu].cantAgenda == 0) {
    Operacion::Aviso("La agenda de CBU esta vacia.");
    return;
  }
  Screen::_textcolor(COL_ROTULO);
  cout << "  ";
  Screen::Texto("Alias", TRANSF_ANC_ALIAS, 'i');
  Screen::Texto("Titular", TRANSF_ANC_TIT, 'i');
  Screen::Texto("Celular", TRANSF_ANC_CEL, 'i');
  cout << endl;
  Screen::_textcolor(COL_MARCO);
  cout << "  ";
  Screen::Separador(TRANSF_ANC_ALIAS + TRANSF_ANC_TIT + TRANSF_ANC_CEL, '-');
  cout << endl;
  Screen::ColorNormal();

  for (i = 0; i < aDatosPers[nroUsu].cantAgenda; i++) {
    cout << "  ";
    Screen::Texto(aDatosPers[nroUsu].aAgenda[i].alias, TRANSF_ANC_ALIAS, 'i');
    Screen::Texto(aDatosPers[nroUsu].aAgenda[i].apellidoNombre,
                  TRANSF_ANC_TIT, 'i');
    Screen::Texto(aDatosPers[nroUsu].aAgenda[i].celular[0] == '\0'
                    ? "(sin dato)" : aDatosPers[nroUsu].aAgenda[i].celular,
                  TRANSF_ANC_CEL, 'i');
    cout << endl;
  }
  cout << endl << "  Cantidad de cuentas agendadas: "
       << aDatosPers[nroUsu].cantAgenda << endl;
} // TransferirListarCelulares

// ---------------------------------------------------------------------------
// Al terminar una transferencia a un CBU que no estaba agendado, ofrece
// incorporarlo a la agenda para no tener que volver a tipearlo.
// ---------------------------------------------------------------------------
static void TransferirOfrecerAgenda(DatosPers aDatosPers[], short nroUsu,
                                    const char cbu[]) {
  Contacto nuevo;
  bool     confirma;

  if (Validar::EntradaAgotada())
    return;
  if (aDatosPers[nroUsu].cantAgenda >= MAX_AGENDA) {
    Operacion::Aviso("La agenda esta completa: no es posible agendar el CBU.");
    return;
  }

  cout << endl;
  confirma = Validar::LeerSiNo("  Desea agendar este CBU (S/N)?: ");
  // Se consulta el fin de la entrada ANTES de usar la respuesta, porque al
  // agotarse la entrada LeerSiNo() devuelve un valor neutro que no expresa la
  // voluntad del usuario.
  if (Validar::EntradaAgotada() or not confirma)
    return;

  Validar::LeerCadena("  Alias para la agenda: ", nuevo.alias, LARGO_DETALLE,
                      true);
  Validar::LeerCadena("  Apellido y nombre...: ", nuevo.apellidoNombre,
                      LARGO_NOMBRE, true);
  Validar::LeerCadena("  Banco...............: ", nuevo.banco, LARGO_DETALLE,
                      true);
  Validar::CopiarCadena(nuevo.cbu, cbu, LARGO_CBU);
  nuevo.celular[0] = '\0';    // en una transferencia por CBU no se conoce
  if (Validar::EntradaAgotada())
    return;

  // La insercion en orden mantiene la agenda ordenada por alias, de modo que
  // la busqueda binaria por alias sigue siendo aplicable.
  if (Ordenar::InsertarContactoEnOrden(aDatosPers[nroUsu].aAgenda,
                                       aDatosPers[nroUsu].cantAgenda, nuevo))
    Operacion::Exito("La cuenta quedo agendada.");
  else
    Operacion::Error("No se pudo agendar: el alias ya existe.");
} // TransferirOfrecerAgenda

// ---------------------------------------------------------------------------
// Nivel 3 - Transferencia a un CBU o a un alias de la agenda.
// ---------------------------------------------------------------------------
void Menu_TransferirCBU(DatosPers aDatosPers[], short nroUsu) {
  char  destino[LARGO_DETALLE + 1];
  str22 cbuDestino;
  str40 titular;
  str25 detalle;
  double saldo, tope, importe, transferidoHoy, restanteHoy;
  Fecha  hoy;
  short  pos;
  bool  agendado, confirma;

  Operacion::PantallaOperacion("TRANSFERIR A CBU O ALIAS", aDatosPers, nroUsu);

  saldo = Operacion::SaldoCA(aDatosPers, nroUsu);
  cout << "  Saldo disponible en la Caja de Ahorro $";
  Operacion::Importe(saldo, TRANSF_ANC_IMPORTE);
  cout << endl << endl;

  if (saldo < TRANSF_MIN_IMPORTE) {
    Operacion::Aviso("No tiene saldo disponible para transferir.");
    Operacion::Continuar();
    return;
  }

  Operacion::ListarAgenda(aDatosPers, nroUsu);
  cout << endl;

  Validar::LeerCadena("  CBU (22 digitos) o alias de destino: ", destino,
                      LARGO_DETALLE, true);
  if (Validar::EntradaAgotada())
    return;

  if (Validar::CBUValido(destino)) {
    // La agenda esta ordenada por ALIAS y no por CBU: para ubicar un CBU no
    // queda mas remedio que recorrerla en forma SECUENCIAL con corte.
    pos = Ordenar::SecuencialCBU(aDatosPers[nroUsu].aAgenda,
                                 aDatosPers[nroUsu].cantAgenda, destino);
    agendado = pos != NO_ENCONTRADO;
    if (not agendado) {
      Operacion::Aviso("El destinatario no esta agendado.");
      cout << "  Verifique el CBU antes de confirmar: la transferencia no "
           << "se puede deshacer." << endl;
      Validar::CopiarCadena(cbuDestino, destino, LARGO_CBU);
      Validar::CopiarCadena(titular, "(no agendado)", LARGO_NOMBRE);
    }
  }
  else {
    // El array SI esta ordenado por alias, de modo que corresponde la
    // BUSQUEDA BINARIA: log2(n) comparaciones en lugar de n.
    pos = Ordenar::BinariaAlias(aDatosPers[nroUsu].aAgenda,
                                aDatosPers[nroUsu].cantAgenda, destino);
    agendado = pos != NO_ENCONTRADO;
    if (not agendado) {
      Operacion::Error("El alias no existe en su agenda de CBU.");
      cout << "  Agendelo primero desde la opcion Agenda de CBU, o ingrese "
           << "el CBU completo." << endl;
      Operacion::Continuar();
      return;
    }
  }

  if (agendado) {
    Validar::CopiarCadena(cbuDestino, aDatosPers[nroUsu].aAgenda[pos].cbu,
                          LARGO_CBU);
    Validar::CopiarCadena(titular,
                          aDatosPers[nroUsu].aAgenda[pos].apellidoNombre,
                          LARGO_NOMBRE);
  }

  cout << endl;
  cout << "  Destino..........: " << destino << endl;
  cout << "  Titular..........: " << titular << endl;
  cout << "  CBU..............: " << cbuDestino << endl;
  if (agendado)
    cout << "  Banco............: " << aDatosPers[nroUsu].aAgenda[pos].banco
         << endl;
  cout << endl;

  // El importe no puede superar ni el saldo de la cuenta ni lo que queda del
  // TOPE DIARIO. El tope es POR DIA y no por operacion, de modo que hay que
  // descontarle lo ya transferido hoy, que se obtiene del propio archivo de
  // movimientos.
  FechaHora::FechaDeHoy(hoy);
  transferidoHoy = Archivo::AcumularDebitosDelDia(
                       aDatosPers[nroUsu].nroUsuario, hoy, "Transf.");
  restanteHoy = TOPE_TRANSF_DIARIO - transferidoHoy;
  if (restanteHoy < TRANSF_MIN_IMPORTE) {
    Operacion::Error("Alcanzo el tope diario de transferencias.");
    cout << "  Tope diario      $";
    Operacion::Importe(TOPE_TRANSF_DIARIO, TRANSF_ANC_IMPORTE);
    cout << endl << "  Transferido hoy  $";
    Operacion::Importe(transferidoHoy, TRANSF_ANC_IMPORTE);
    cout << endl;
    Operacion::Continuar();
    return;
  }
  if (transferidoHoy > 0) {
    cout << "  Transferido hoy  $";
    Operacion::Importe(transferidoHoy, TRANSF_ANC_IMPORTE);
    cout << "   de un tope diario de $";
    Operacion::Importe(TOPE_TRANSF_DIARIO, TRANSF_ANC_IMPORTE);
    cout << endl;
  }
  tope = saldo < restanteHoy ? saldo : restanteHoy;
  importe = Validar::LeerReal("  Importe a transferir $: ", TRANSF_MIN_IMPORTE,
                              tope);
  if (Validar::EntradaAgotada())
    return;

  confirma = Validar::LeerSiNo("  Confirma la transferencia (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("La transferencia fue cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  // Por encima de TOPE_SIN_TOKEN la operacion exige el token de seguridad.
  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, importe)) {
    Operacion::Continuar();
    return;
  }

  // El detalle que se graba tiene a lo sumo LARGO_DETALLE caracteres. Un CBU
  // completo (22 digitos) no entra junto con el rotulo, y truncarlo por la
  // izquierda dejaria grabado el codigo del banco, que es lo que menos
  // identifica al destinatario: por eso se graban sus 8 ultimos digitos.
  if (agendado)
    snprintf(detalle, LARGO_DETALLE + 1, "Transf.a %s",
             aDatosPers[nroUsu].aAgenda[pos].alias);
  else
    snprintf(detalle, LARGO_DETALLE + 1, "Transf.CBU ..%s",
             cbuDestino + LARGO_CBU - 8);

  if (Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA, detalle,
                                       importe)) {
    Operacion::Exito("Transferencia realizada.");
    TransferirComprobante(destino, titular, cbuDestino, importe,
                          Operacion::SaldoCA(aDatosPers, nroUsu));
    if (not agendado)
      TransferirOfrecerAgenda(aDatosPers, nroUsu, cbuDestino);
  }
  else
    Operacion::Error("No fue posible registrar la transferencia: saldo "
                     "insuficiente o archivo no disponible.");

  Operacion::Continuar();
} // Menu_TransferirCBU

// ---------------------------------------------------------------------------
// Nivel 3 - Transferencia a un numero de celular ya agendado.
// ---------------------------------------------------------------------------
void Menu_TransferirCelular(DatosPers aDatosPers[], short nroUsu) {
  char  celular[LARGO_CELULAR + 1];
  str25 detalle;
  double saldo, tope, importe;
  short pos;
  bool  valido, confirma;

  Operacion::PantallaOperacion("TRANSFERIR A UN NUMERO DE CELULAR",
                               aDatosPers, nroUsu);

  saldo = Operacion::SaldoCA(aDatosPers, nroUsu);
  cout << "  Saldo disponible en la Caja de Ahorro $";
  Operacion::Importe(saldo, TRANSF_ANC_IMPORTE);
  cout << endl << endl;

  if (saldo < TRANSF_MIN_IMPORTE) {
    Operacion::Aviso("No tiene saldo disponible para transferir.");
    Operacion::Continuar();
    return;
  }

  TransferirListarCelulares(aDatosPers, nroUsu);
  cout << endl;

  // Ciclo 1-x: se insiste hasta que el numero tenga el formato esperado.
  do {
    Validar::LeerCadena("  Numero de celular (solo digitos): ", celular,
                        LARGO_CELULAR, false);
    valido = Validar::CelularValido(celular);
    if (not valido and not Validar::EntradaAgotada())
      Operacion::Error("El celular se ingresa solo con digitos, sin espacios "
                       "ni guiones.");
  } while (not valido and not Validar::EntradaAgotada());
  if (Validar::EntradaAgotada())
    return;

  // El celular no es la clave de ordenamiento de la agenda (lo es el alias),
  // de modo que la busqueda debe ser SECUENCIAL.
  pos = Ordenar::SecuencialCelular(aDatosPers[nroUsu].aAgenda,
                                   aDatosPers[nroUsu].cantAgenda, celular);
  if (pos == NO_ENCONTRADO) {
    Operacion::Error("Ningun contacto de su agenda tiene ese numero de "
                     "celular.");
    cout << "  Para transferir a un celular el destinatario debe estar "
         << "agendado con su numero." << endl;
    Operacion::Continuar();
    return;
  }

  cout << endl;
  cout << "  Celular..........: " << celular << endl;
  cout << "  Alias............: " << aDatosPers[nroUsu].aAgenda[pos].alias
       << endl;
  cout << "  Titular..........: "
       << aDatosPers[nroUsu].aAgenda[pos].apellidoNombre << endl;
  cout << "  CBU..............: " << aDatosPers[nroUsu].aAgenda[pos].cbu
       << endl;
  cout << "  Banco............: " << aDatosPers[nroUsu].aAgenda[pos].banco
       << endl << endl;

  tope = saldo < TOPE_TRANSF_DIARIO ? saldo : TOPE_TRANSF_DIARIO;
  importe = Validar::LeerReal("  Importe a transferir $: ", TRANSF_MIN_IMPORTE,
                              tope);
  if (Validar::EntradaAgotada())
    return;

  confirma = Validar::LeerSiNo("  Confirma la transferencia (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("La transferencia fue cancelada por el usuario.");
    Operacion::Continuar();
    return;
  }

  if (not Operacion::AutorizarOperacion(aDatosPers, nroUsu, importe)) {
    Operacion::Continuar();
    return;
  }

  snprintf(detalle, LARGO_DETALLE + 1, "Transf.Cel %s", celular);

  if (Operacion::RegistrarMovimientoCA(aDatosPers, nroUsu, MOV_DEBITA, detalle,
                                       importe)) {
    Operacion::Exito("Transferencia realizada.");
    TransferirComprobante(celular,
                          aDatosPers[nroUsu].aAgenda[pos].apellidoNombre,
                          aDatosPers[nroUsu].aAgenda[pos].cbu, importe,
                          Operacion::SaldoCA(aDatosPers, nroUsu));
  }
  else
    Operacion::Error("No fue posible registrar la transferencia: saldo "
                     "insuficiente o archivo no disponible.");

  Operacion::Continuar();
} // Menu_TransferirCelular

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de transferencias.
// ---------------------------------------------------------------------------
void Menu_Transferir(DatosPers aDatosPers[], short nroUsu) {
  str40Opc aMenu[TRANSF_CANT_OPC] = {
    "Transferir a CBU o Alias",
    "Transferir a un numero de celular",
    "Ver mis cuentas agendadas",
    "Volver al menu principal"
  };
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("TRANSFERIR DINERO", aDatosPers, nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, TRANSF_CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {
      case 0 : Menu_TransferirCBU(aDatosPers, nroUsu);              break;
      case 1 : Menu_TransferirCelular(aDatosPers, nroUsu);          break;
      case 2 : Operacion::PantallaOperacion("MIS CUENTAS AGENDADAS",
                                            aDatosPers, nroUsu);
               Operacion::ListarAgenda(aDatosPers, nroUsu);
               Operacion::Continuar();                              break;
      default: volver = true;                 // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_Transferir

#endif // MENU_TRANSFERIR_HPP
