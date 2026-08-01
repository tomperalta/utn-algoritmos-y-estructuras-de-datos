// ---------------------------------------------------------------------------
// Menu_Listados.hpp
// Nivel 2 - Opcion <<Emitir listados a ListadosHB.Txt>> del menu principal.
//
// Emite en un unico archivo de texto TODOS los listados del usuario que inicio
// la sesion. Tal como pide el enunciado, la salida se dirige al archivo
// ListadosHB.Txt redirigiendo la salida estandar con freopen(), de manera que
// los mismos modulos que dibujan las tablas en la pantalla (Operacion::TablaCA,
// TablaTD, TablaTC, ListarCuentas y ListarAgenda) sirvan tambien para armar el
// archivo, sin duplicar una linea de codigo.
//
// Secuencia de la redireccion
// ---------------------------
//   1. confirmacion del usuario (el archivo se sobrescribe por completo)
//   2. se guarda el estado del modo texto plano
//   3. cout.flush()                 vacia lo pendiente en la pantalla
//   4. freopen(ARCH_LISTADOS, "w", stdout)
//   5. Screen::ModoTextoPlano(true) sin colores ni posicionamiento absoluto,
//                                   que ensuciarian el archivo
//   6. emision del contenido
//   7. cout.flush()                 vacia lo pendiente en el archivo
//   8. freopen(DISPOSITIVO_CONSOLA, "w", stdout)
//   9. Screen::ModoTextoPlano(estado anterior)
//  10. mensaje de resultado en la pantalla
//
// Los pasos 8 y 9 se ejecutan SIEMPRE: dentro del bloque redirigido no hay
// ningun return, porque volver antes de restaurar dejaria a todo el programa
// escribiendo en el archivo en lugar de la pantalla.
//
// Los movimientos se leen y se ordenan ANTES de redirigir, de modo que un
// eventual problema con los archivos de datos se informe en la pantalla.
// ---------------------------------------------------------------------------
#ifndef MENU_LISTADOS_HPP
#define MENU_LISTADOS_HPP

// ----- Medidas y textos propios de este modulo (prefijo LST_) --------------
const short LST_ANCHO        = 74;  // ancho de las lineas separadoras
const short LST_COL_ROTULO   = 26;  // columna donde se alinean los valores
const short LST_ANCHO_IMPORTE = 16;
const short LST_LARGO_HORA   =  8;  // "hh:mm:ss"
const short LST_LARGO_MENSAJE = 60;
const char  LST_BANCO[] = "HOME BANKING MESOPOTAMIA  -  Sucursal 009";

// ----- Numeracion de los listados que componen el archivo -----------------
const short LST_NRO_DATOS   = 1;
const short LST_NRO_CUENTAS = 2;
const short LST_NRO_AGENDA  = 3;
const short LST_NRO_CA      = 4;
const short LST_NRO_TD      = 5;
const short LST_NRO_TC      = 6;
const short LST_NRO_RESUMEN = 7;

// ---------------------------------------------------------------------------
// Emite el rotulo de una linea de datos rellenando con puntos hasta una
// columna fija, para que todos los valores del listado queden alineados.
// ---------------------------------------------------------------------------
static void MenuListados_Rotulo(const char texto[]) {
  short relleno = (short)(LST_COL_ROTULO - Screen::LargoVisible(texto));

  cout << "  " << texto;
  for (short i = 0; i < relleno; i++)
    cout << '.';
  cout << ": ";
} // MenuListados_Rotulo

// ---------------------------------------------------------------------------
// Encabezado general del archivo: nombre del banco, fecha y hora de emision y
// usuario al que corresponden los listados.
// ---------------------------------------------------------------------------
static void MenuListados_Portada(DatosPers aDatosPers[], short nroUsu) {
  char  fecha[ANCHO_FECHA + 1];
  char  hora[LST_LARGO_HORA + 1];
  Fecha hoy;

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);

  Screen::Separador(LST_ANCHO, '=');
  cout << endl;
  cout << "  " << LST_BANCO << endl;
  cout << "  LISTADOS DEL USUARIO" << endl;
  cout << "  Emitido el " << FechaHora::DiaSemanaDeHoy() << " " << fecha
       << " a las " << hora << " hs." << endl;
  cout << "  Usuario: " << aDatosPers[nroUsu].apellidoNombre
       << "   (nro. " << aDatosPers[nroUsu].nroUsuario << ")" << endl;
  cout << "  Archivo: " << ARCH_LISTADOS << endl;
  Screen::Separador(LST_ANCHO, '=');
  cout << endl;
} // MenuListados_Portada

// ---------------------------------------------------------------------------
// Titulo de cada uno de los listados que componen el archivo.
// ---------------------------------------------------------------------------
static void MenuListados_Titulo(short nro, const char texto[]) {
  cout << endl;
  Screen::Separador(LST_ANCHO, '-');
  cout << endl << "  " << nro << ". " << texto << endl;
  Screen::Separador(LST_ANCHO, '-');
  cout << endl << endl;
} // MenuListados_Titulo

// ---------------------------------------------------------------------------
// Listado 1: datos personales del usuario.
// La CLAVE no se emite: es un dato secreto y no debe quedar en un archivo de
// texto que despues se imprime.
// ---------------------------------------------------------------------------
static void MenuListados_DatosPersonales(DatosPers aDatosPers[], short nroUsu) {
  char fechaNac[ANCHO_FECHA + 1];

  FechaHora::FormatearFechaCeros(aDatosPers[nroUsu].fechaNac, fechaNac);

  MenuListados_Rotulo("Nro. de usuario");
  cout << aDatosPers[nroUsu].nroUsuario << endl;
  MenuListados_Rotulo("DNI");
  cout << aDatosPers[nroUsu].dni << endl;
  MenuListados_Rotulo("Apellido y Nombre");
  cout << aDatosPers[nroUsu].apellidoNombre << endl;
  MenuListados_Rotulo("Fecha de nacimiento");
  cout << fechaNac << endl;
  MenuListados_Rotulo("Usuario");
  cout << aDatosPers[nroUsu].usuario << endl;
  MenuListados_Rotulo("Nro. de celular");
  cout << aDatosPers[nroUsu].celular << endl;
  MenuListados_Rotulo("Correo electronico");
  cout << aDatosPers[nroUsu].email << endl;
  MenuListados_Rotulo("Domicilio");
  cout << aDatosPers[nroUsu].domicilio << endl;
  MenuListados_Rotulo("Nro. Cta. Caja Ahorro");
  cout << aDatosPers[nroUsu].nroCtaCA << endl;
  MenuListados_Rotulo("CBU");
  cout << aDatosPers[nroUsu].cbu << endl;
} // MenuListados_DatosPersonales

// ---------------------------------------------------------------------------
// Listado 7: resumen final de saldos y de consumos con las tarjetas.
// ---------------------------------------------------------------------------
static void MenuListados_Resumen(DatosPers aDatosPers[], short nroUsu,
                                 double totalTD, double totalTC) {
  MenuListados_Rotulo("Saldo Caja de Ahorro $");
  Operacion::Importe(Operacion::SaldoCA(aDatosPers, nroUsu),
                     LST_ANCHO_IMPORTE);
  cout << endl;
  MenuListados_Rotulo("Saldo en dolares u$s");
  Operacion::Importe(Operacion::SaldoDolares(aDatosPers, nroUsu),
                     LST_ANCHO_IMPORTE);
  cout << endl;
  MenuListados_Rotulo("Consumido Tarj. Debito");
  Operacion::Importe(totalTD, LST_ANCHO_IMPORTE);
  cout << endl;
  MenuListados_Rotulo("Consumido Tarj. Credito");
  Operacion::Importe(totalTC, LST_ANCHO_IMPORTE);
  cout << endl;
  MenuListados_Rotulo("TOTAL consumido tarjetas");
  Operacion::Importe(totalTD + totalTC, LST_ANCHO_IMPORTE);
  cout << endl;
} // MenuListados_Resumen

// ---------------------------------------------------------------------------
// Pie del archivo, con la cantidad de registros emitidos.
// ---------------------------------------------------------------------------
static void MenuListados_Pie(short cantRegistros) {
  cout << endl;
  Screen::Separador(LST_ANCHO, '=');
  cout << endl;
  cout << "  Registros emitidos en este listado: " << cantRegistros << endl;
  cout << "  FIN DE LOS LISTADOS" << endl;
  Screen::Separador(LST_ANCHO, '=');
  cout << endl;
} // MenuListados_Pie

// ---------------------------------------------------------------------------
// Modulo de nivel 2: emite todos los listados del usuario a ListadosHB.Txt.
// ---------------------------------------------------------------------------
void Menu_Listados(DatosPers aDatosPers[], short nroUsu) {
  // Los tres arrays de movimientos son grandes: se declaran <<static>> para no
  // sobrecargar la pila. Siguen siendo memoria estatica, no dinamica.
  static MovCA aMovCA[MAX_MOV_CA];
  static MovTD aMovTD[MAX_MOV_TD];
  static MovTC aMovTC[MAX_MOV_TC];
  char  mensaje[LST_LARGO_MENSAJE + 1];
  short cantCA, cantTD, cantTC, cantRegistros, iCtaCA;
  double saldoInicialCA, totalTD = 0, totalTC = 0;
  bool  modoAnterior, confirma;

  Operacion::PantallaOperacion("EMITIR LISTADOS A ListadosHB.Txt", aDatosPers,
                               nroUsu);
  cout << "  Se emitiran todos sus listados al archivo de texto "
       << ARCH_LISTADOS << "." << endl;
  Operacion::Aviso("Si el archivo ya existe, se sobrescribe por completo.");

  confirma = Validar::LeerSiNo(
      "  Confirma la emision de los listados (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("No se emitio ningun listado.");
    Operacion::Continuar();
    return;
  }

  // El saldo se recalcula desde el archivo para que el listado y los
  // movimientos que se emiten mas abajo no queden desfasados.
  Operacion::RecalcularSaldoCA(aDatosPers, nroUsu);

  // ACCESO SECUENCIAL a los tres archivos de movimientos. Se pasa
  // nroUsuario (1..5) y no el indice del array.
  cantCA = Archivo::LeerMovimientosCA(aMovCA, aDatosPers[nroUsu].nroUsuario,
                                      MAX_MOV_CA);
  cantTD = Archivo::LeerMovimientosTD(aMovTD, aDatosPers[nroUsu].nroUsuario,
                                      MAX_MOV_TD);
  cantTC = Archivo::LeerMovimientosTC(aMovTC, aDatosPers[nroUsu].nroUsuario,
                                      MAX_MOV_TC);

  // Los movimientos se graban en el archivo en el orden en que se producen:
  // para el listado hay que ordenarlos cronologicamente.
  Ordenar::MovCAporFecha(aMovCA, cantCA);
  Ordenar::MovTDporFecha(aMovTD, cantTD);
  Ordenar::MovTCporFecha(aMovTC, cantTC);

  // Totales de las tarjetas: repeticion exacta, se conoce la cantidad.
  for (short i = 0; i < cantTD; i++)
    totalTD += aMovTD[i].importe;
  for (short i = 0; i < cantTC; i++)
    totalTC += aMovTC[i].importe;

  iCtaCA = Operacion::IndiceCuentaCA(aDatosPers, nroUsu);
  saldoInicialCA = iCtaCA == NO_ENCONTRADO
                 ? 0 : aDatosPers[nroUsu].aCuentas[iCtaCA].saldoInicial;
  cantRegistros = (short)(aDatosPers[nroUsu].cantCuentas +
                          aDatosPers[nroUsu].cantAgenda +
                          cantCA + cantTD + cantTC);

  modoAnterior = Screen::EnModoTextoPlano();   // paso 2
  cout.flush();                                // paso 3
  GuardarSalidaEstandar();  // copia del descriptor, para poder volver siempre

  if (freopen(ARCH_LISTADOS, "w", stdout) == NULL) {   // paso 4
    // Cuando freopen() falla deja la salida estandar CERRADA: si se volviera
    // sin mas, el programa seguiria andando a ciegas y el usuario no veria
    // ningun mensaje. Por eso primero se restituye la pantalla y recien
    // despues se informa el error.
    RestaurarSalidaEstandar();
    Screen::ModoTextoPlano(modoAnterior);
    Operacion::Error("No se pudo crear el archivo de listados.");
    cout << "  Verifique que la carpeta admita escritura." << endl;
    Operacion::Continuar();
    return;
  }
  Screen::ModoTextoPlano(true);                // paso 5

  // ======================= paso 6: EMISION ==================================
  // A partir de aqui todo lo que se emita va al archivo. NO hay ningun return
  // dentro de este bloque: la salida estandar debe restaurarse siempre.
  MenuListados_Portada(aDatosPers, nroUsu);

  MenuListados_Titulo(LST_NRO_DATOS, "LISTADO DE DATOS PERSONALES");
  MenuListados_DatosPersonales(aDatosPers, nroUsu);

  MenuListados_Titulo(LST_NRO_CUENTAS, "LISTADO DE CUENTAS");
  Operacion::ListarCuentas(aDatosPers, nroUsu);

  MenuListados_Titulo(LST_NRO_AGENDA,
                      "LISTADO DE CUENTAS AGENDADAS (por alias)");
  Operacion::ListarAgenda(aDatosPers, nroUsu);

  MenuListados_Titulo(LST_NRO_CA,
                      "MOVIMIENTOS DE LA CAJA DE AHORRO (por fecha)");
  Operacion::TablaCA(aMovCA, cantCA, saldoInicialCA,
                     aDatosPers[nroUsu].nroCtaCA, "$");

  MenuListados_Titulo(LST_NRO_TD,
                      "MOVIMIENTOS DE TARJETA DE DEBITO (por fecha)");
  Operacion::TablaTD(aMovTD, cantTD);

  MenuListados_Titulo(LST_NRO_TC,
                      "MOVIMIENTOS DE TARJETA DE CREDITO (por fecha)");
  Operacion::TablaTC(aMovTC, cantTC);

  MenuListados_Titulo(LST_NRO_RESUMEN, "RESUMEN FINAL");
  MenuListados_Resumen(aDatosPers, nroUsu, totalTD, totalTC);

  MenuListados_Pie(cantRegistros);
  // ====================== fin de la emision =================================

  cout.flush();                                    // paso 7
  RestaurarSalidaEstandar();                       // paso 8
  Screen::ModoTextoPlano(modoAnterior);            // paso 9

  // ----- paso 10: resultado en la pantalla -----
  Operacion::PantallaOperacion("EMITIR LISTADOS A ListadosHB.Txt", aDatosPers,
                               nroUsu);
  snprintf(mensaje, sizeof mensaje, "Listados emitidos en el archivo %s",
           ARCH_LISTADOS);
  Operacion::Exito(mensaje);
  cout << endl << "  Registros emitidos: " << cantRegistros << endl;
  cout << "    Cuentas del usuario............: "
       << aDatosPers[nroUsu].cantCuentas << endl;
  cout << "    Cuentas agendadas..............: "
       << aDatosPers[nroUsu].cantAgenda << endl;
  cout << "    Movimientos de Caja de Ahorro..: " << cantCA << endl;
  cout << "    Movimientos de Tarj. de Debito.: " << cantTD << endl;
  cout << "    Movimientos de Tarj. de Credito: " << cantTC << endl;
  Operacion::Continuar();
} // Menu_Listados

#endif // MENU_LISTADOS_HPP
