// ---------------------------------------------------------------------------
// SistemaHomeBanking.hpp
// Nivel 1 - Modulo de mas alto nivel del sistema, unico invocado desde el
// bloque principal.
//
// Estrategia
// ----------
//   1. Prepara la consola y la muestra de datos:
//        - registra internamente los datos personales de los cinco usuarios,
//        - crea los archivos de movimientos si todavia no existen.
//   2. Invoca a MenuLogin(aDatosPers, usuario), que devuelve el usuario que
//      ingreso al sitio.
//   3. Invoca a MenuPpal(), que devuelve la opcion elegida, y con esa opcion
//      despacha al modulo correspondiente mediante SELECCION MULTIPLE.
//   4. Al cerrar la sesion ofrece iniciar otra, hasta que se decida salir.
// ---------------------------------------------------------------------------
#ifndef SISTEMAHOMEBANKING_HPP
#define SISTEMAHOMEBANKING_HPP

// ---------------------------------------------------------------------------
// Pantalla de presentacion del sitio.
// ---------------------------------------------------------------------------
void PantallaBienvenida() {
  char fecha[ANCHO_FECHA + 1];
  char hora[9];
  Fecha hoy;

  FechaHora::FechaDeHoy(hoy);
  FechaHora::FormatearFechaCeros(hoy, fecha);
  FechaHora::HoraDeHoy(hora);

  Screen::_clrscr();
  Screen::_textcolor(COL_TITULO);
  cout << endl;
  Screen::Separador(74, '*');
  cout << endl;
  cout << "            H O M E   B A N K I N G   M E S O P O T A M I A"
       << endl;
  Screen::Separador(74, '*');
  Screen::_textcolor(GRIS_CLARO);
  cout << endl << endl;
  cout << "  " << FechaHora::DiaSemanaDeHoy() << " " << fecha << "   Hora: "
       << hora << endl;
  cout << "  Sucursal 009 - Banca por Internet" << endl;
  Screen::ColorNormal();
  Screen::Espera(1200);
} // PantallaBienvenida

// ---------------------------------------------------------------------------
// Sesion de un usuario: menu principal y despacho de las opciones.
// ---------------------------------------------------------------------------
void SesionUsuario(DatosPers aDatosPers[], short nroUsu) {
  short opcion;
  bool  cerrar = false;

  // El saldo de la Caja de Ahorro se recalcula al ingresar, leyendo el
  // archivo de movimientos.
  Operacion::RecalcularSaldoCA(aDatosPers, nroUsu);

  do {
    opcion = MenuPpal(aDatosPers, nroUsu);

    switch (opcion) {                       // seleccion multiple
      case OPC_MIS_CUENTAS  : Menu_MisCuentas(aDatosPers, nroUsu);          break;
      case OPC_MOVIMIENTOS  : Menu_Movimientos(aDatosPers, nroUsu);         break;
      case OPC_TRANSFERIR   : Menu_Transferir(aDatosPers, nroUsu);          break;
      case OPC_DEPOSITO     : Menu_Deposito(aDatosPers, nroUsu);            break;
      case OPC_COMPRAS      : Menu_Compras(aDatosPers, nroUsu);             break;
      case OPC_TARJETAS     : Menu_Tarjetas(aDatosPers, nroUsu);            break;
      case OPC_PLAZO_FIJO   : Menu_PlazoFijo(aDatosPers, nroUsu);           break;
      case OPC_DOLARES      : Menu_CompraVentaDolares(aDatosPers, nroUsu);  break;
      case OPC_RECARGAS     : Menu_Recargar(aDatosPers, nroUsu);            break;
      case OPC_TOKEN        : Menu_GenToken(aDatosPers, nroUsu);            break;
      case OPC_CBU          : Menu_MostrarCBU(aDatosPers, nroUsu);          break;
      case OPC_AGENDA       : Menu_AgendaCBU(aDatosPers, nroUsu);           break;
      case OPC_DATOS_PERS   : Menu_DatosPers(aDatosPers, nroUsu);           break;
      case OPC_MODIF_CLAVE  : Menu_ModifClave(aDatosPers, nroUsu);          break;
      case OPC_NUEVA_CUENTA : Menu_NuevaCuenta(aDatosPers, nroUsu);         break;
      case OPC_LISTADOS     : Menu_Listados(aDatosPers, nroUsu);            break;
      case OPC_LOGOUT       : cerrar = Menu_Logout(aDatosPers, nroUsu);     break;
      default               : Operacion::Error("Opcion inexistente.");
                              Operacion::Continuar();
    }
  } while (not cerrar and not Validar::EntradaAgotada());
} // SesionUsuario

// ---------------------------------------------------------------------------
// Modulo de nivel 1 invocado desde el bloque principal.
// ---------------------------------------------------------------------------
void SistemaHomeBanking() {
  DatosPers aDatosPers[MAX_USUARIOS];   // array de registros: los 5 usuarios
  char      avisoMuestra[LARGO_NOMBRE * 3];
  short     usuario;
  bool      salir = false;

  srand((unsigned) time(NULL));         // semilla para los tokens al azar
  PrepararConsola();
  // El modo texto plano se establece ANTES de cualquier emision, para que no
  // se escape ninguna secuencia de escape cuando la salida no es una consola.
  Screen::ModoTextoPlano(not SalidaEsConsola());
  Screen::BarraTitulo("Home Banking Mesopotamia");

  Datos::CargarDatosPersonales(aDatosPers);
  Datos::GenerarMuestraDatos();

  // El enunciado exige no menos de MIN_REG_ARCH registros por archivo: si la
  // muestra de datos quedo corta se avisa, en lugar de seguir en silencio.
  if (Datos::VerificarMuestraDatos(avisoMuestra, sizeof avisoMuestra) > 0) {
    Operacion::Aviso("La muestra de datos no alcanza el minimo de registros:");
    cout << "  " << avisoMuestra << endl;
    Operacion::Continuar();
  }

  PantallaBienvenida();

  do {
    MenuLogin(aDatosPers, usuario);
    if (usuario != NO_ENCONTRADO) {
      SesionUsuario(aDatosPers, usuario);
      if (not Validar::EntradaAgotada()) {
        Operacion::PantallaOperacion("SESION FINALIZADA", aDatosPers, usuario);
        salir = not Validar::LeerSiNo("  Desea iniciar otra sesion (S/N)?: ");
      }
      else
        salir = true;
    }
    else
      salir = true;
  } while (not salir and not Validar::EntradaAgotada());

  Screen::_clrscr();
  Screen::_textcolor(COL_TITULO);
  cout << endl << "  Gracias por utilizar Home Banking Mesopotamia." << endl;
  Screen::ColorNormal();
  Screen::MostrarCursor();
  RestaurarConsola();
} // SistemaHomeBanking

#endif // SISTEMAHOMEBANKING_HPP
