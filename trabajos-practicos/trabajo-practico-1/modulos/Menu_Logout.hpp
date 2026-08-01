// ---------------------------------------------------------------------------
// Menu_Logout.hpp
// Nivel 2 - Opcion <<Cerrar sesion (Logout)>> del menu principal.
//
// Muestra un resumen del estado en que queda la cuenta del usuario (saldo de
// la Caja de Ahorro en pesos, saldo en dolares y hora de cierre) y pide
// confirmacion antes de abandonar la sesion, para que un ENTER de mas no deje
// al usuario fuera del sitio.
//
// Devuelve:
//   true  -> la sesion se cierra (el usuario confirmo, o se agoto la entrada)
//   false -> el usuario se arrepintio y la sesion continua abierta
//
// Cuando la entrada se agota (EOF, tipico al ejecutar el programa con la
// entrada redirigida desde un archivo de prueba) se devuelve true, de manera
// que SesionUsuario() pueda terminar en forma ordenada en lugar de quedar
// girando en el menu principal.
// ---------------------------------------------------------------------------
#ifndef MENU_LOGOUT_HPP
#define MENU_LOGOUT_HPP

// ---------------------------------------------------------------------------
// Pantalla de operacion de cierre de sesion.
// ---------------------------------------------------------------------------
bool Menu_Logout(DatosPers aDatosPers[], short nroUsu) {
  const short LARGO_HORA  =  8;   // "hh:mm:ss"
  const short ANCHO_SALDO = 16;
  char hora[LARGO_HORA + 1];
  bool confirma;

  FechaHora::HoraDeHoy(hora);
  Operacion::PantallaOperacion("CERRAR SESION", aDatosPers, nroUsu);

  cout << "  Resumen de la sesion" << endl << endl;
  cout << "    Usuario................: "
       << aDatosPers[nroUsu].apellidoNombre << endl;
  cout << "    Cuenta Caja de Ahorro..: " << aDatosPers[nroUsu].nroCtaCA
       << endl;
  cout << "    Saldo final $..........:";
  Operacion::Importe(Operacion::SaldoCA(aDatosPers, nroUsu), ANCHO_SALDO);
  cout << endl;
  cout << "    Saldo final u$s........:";
  Operacion::Importe(Operacion::SaldoDolares(aDatosPers, nroUsu), ANCHO_SALDO);
  cout << endl;
  cout << "    Hora de cierre.........: " << hora << " hs." << endl;

  // Si la entrada ya se agoto no se puede pedir la confirmacion: se cierra la
  // sesion para que el programa pueda terminar.
  if (Validar::EntradaAgotada())
    return true;

  confirma = Validar::LeerSiNo("  Confirma cerrar la sesion (S/N)?: ");
  if (Validar::EntradaAgotada())
    return true;

  if (not confirma) {
    Operacion::Aviso("La sesion continua abierta.");
    Operacion::Continuar();
    return false;
  }

  Operacion::Exito("La sesion se cerro correctamente.");
  cout << "  Hasta pronto, " << aDatosPers[nroUsu].apellidoNombre << "."
       << endl;
  cout << "  Recuerde cerrar el navegador por su seguridad." << endl;
  Operacion::Continuar();
  return true;
} // Menu_Logout

#endif // MENU_LOGOUT_HPP
