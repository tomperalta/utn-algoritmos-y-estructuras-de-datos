// ---------------------------------------------------------------------------
// Menu_NuevaCuenta.hpp
// Nivel 2 - Opcion <<Crear nueva cuenta>> del menu principal.
//
// Da de alta una cuenta bancaria nueva para el usuario que inicio la sesion.
// Antes de crearla se verifica:
//   * que quede lugar libre en el array de cuentas del usuario (MAX_CUENTAS
//     componentes), y
//   * que el usuario NO tenga ya una cuenta del mismo tipo y de la misma
//     moneda, porque el banco admite una sola de cada combinacion.
//
// El numero de la cuenta lo arma Operacion::NuevoNroCuenta(), que lo genera
// CORRELATIVO respecto de todas las cuentas ya creadas en el sistema, y el
// alta se informa con el mensaje que pide el enunciado:
//
//     Cuenta creada nro. 009-0000011
//
// La cuenta nace con saldo cero y en estado 'A' (activa). El alta se refleja
// unicamente en el array de registros que esta en memoria: los archivos de
// texto conservan solamente los movimientos, no las cuentas.
// ---------------------------------------------------------------------------
#ifndef MENU_NUEVACUENTA_HPP
#define MENU_NUEVACUENTA_HPP

// ---------------------------------------------------------------------------
// Pantalla de operacion: muestra las cuentas actuales, pide tipo y moneda,
// confirma y agrega la cuenta al array del usuario.
// ---------------------------------------------------------------------------
void Menu_NuevaCuenta(DatosPers aDatosPers[], short nroUsu) {
  const short LARGO_TIPO_MON  =  3;      // "CA" | "CC"  y  "$" | "u$s"
  const short LARGO_MENSAJE   = 60;      // texto del mensaje de exito
  const short ANCHO_SALDO     = 14;
  const double SALDO_INICIAL   =  0.00;  // toda cuenta nueva nace en cero
  const char  ESTADO_ACTIVA   = 'A';
  str3  tipoCta;
  str4  moneda;
  str20 nroCtaNueva;
  char  mensaje[LARGO_MENSAJE + 1];
  char  tipoElegido, monedaElegida;
  short posRepetida, posNueva;
  bool  confirma;

  Operacion::PantallaOperacion("CREAR UNA NUEVA CUENTA", aDatosPers, nroUsu);

  // Se listan primero las cuentas que el usuario ya tiene, para que pueda ver
  // cual le falta antes de elegir el tipo y la moneda de la nueva.
  cout << "  Cuentas que usted posee actualmente:" << endl << endl;
  Operacion::ListarCuentas(aDatosPers, nroUsu);

  // El lugar en el array se controla ANTES de pedir los datos: si el array
  // esta completo no tiene sentido continuar con la operacion.
  if (aDatosPers[nroUsu].cantCuentas >= MAX_CUENTAS) {
    Operacion::Error("No hay lugar para mas cuentas.");
    cout << "  Cada usuario admite hasta " << MAX_CUENTAS << " cuentas."
         << endl;
    Operacion::Continuar();
    return;
  }

  cout << endl;
  tipoElegido = Validar::LeerCaracter(
      "  Tipo de cuenta (A=Caja de Ahorro / C=Cuenta Corriente): ", "AC");
  if (Validar::EntradaAgotada())
    return;
  Validar::CopiarCadena(tipoCta, tipoElegido == 'A' ? "CA" : "CC",
                        LARGO_TIPO_MON);

  monedaElegida = Validar::LeerCaracter("  Moneda (P=Pesos / D=Dolares): ",
                                        "PD");
  if (Validar::EntradaAgotada())
    return;
  Validar::CopiarCadena(moneda, monedaElegida == 'P' ? "$" : "u$s",
                        LARGO_TIPO_MON);

  // BUSQUEDA SECUENCIAL: el array de cuentas de un usuario tiene a lo sumo
  // MAX_CUENTAS componentes y no esta ordenado por tipo ni por moneda, de modo
  // que no se justifica una busqueda binaria.
  posRepetida = Ordenar::SecuencialCuentaPorTipo(
      aDatosPers[nroUsu].aCuentas, aDatosPers[nroUsu].cantCuentas, tipoCta,
      moneda);
  if (posRepetida != NO_ENCONTRADO) {
    Operacion::Error("Ya posee una cuenta de ese tipo y en esa moneda.");
    cout << "  Cuenta existente: "
         << aDatosPers[nroUsu].aCuentas[posRepetida].nroCta << "  ("
         << aDatosPers[nroUsu].aCuentas[posRepetida].tipo << " "
         << aDatosPers[nroUsu].aCuentas[posRepetida].moneda << ")" << endl;
    Operacion::Continuar();
    return;
  }

  // El numero se arma ANTES de incrementar la cantidad de cuentas, porque el
  // correlativo se calcula sobre las cuentas ya existentes en el sistema.
  Operacion::NuevoNroCuenta(aDatosPers, MAX_USUARIOS, nroCtaNueva);

  cout << endl << "  Datos de la cuenta a crear:" << endl << endl;
  cout << "    Numero de cuenta..: " << nroCtaNueva << endl;
  cout << "    Tipo..............: " << tipoCta << "  ("
       << (tipoElegido == 'A' ? "Caja de Ahorro" : "Cuenta Corriente") << ")"
       << endl;
  cout << "    Moneda............: " << moneda << "  ("
       << (monedaElegida == 'P' ? "Pesos" : "Dolares") << ")" << endl;
  cout << "    Saldo inicial.....:";
  Operacion::Importe(SALDO_INICIAL, ANCHO_SALDO);
  cout << endl;
  cout << "    Estado............: Activa" << endl;

  confirma = Validar::LeerSiNo("  Confirma la creacion de la cuenta (S/N)?: ");
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("La creacion de la cuenta fue cancelada.");
    Operacion::Continuar();
    return;
  }

  // ----- Alta de la cuenta en la ultima posicion libre del array -----
  posNueva = aDatosPers[nroUsu].cantCuentas;
  Validar::CopiarCadena(aDatosPers[nroUsu].aCuentas[posNueva].nroCta,
                        nroCtaNueva, LARGO_CTA);
  Validar::CopiarCadena(aDatosPers[nroUsu].aCuentas[posNueva].tipo, tipoCta,
                        LARGO_TIPO_MON);
  Validar::CopiarCadena(aDatosPers[nroUsu].aCuentas[posNueva].moneda, moneda,
                        LARGO_TIPO_MON);
  aDatosPers[nroUsu].aCuentas[posNueva].saldoInicial = SALDO_INICIAL;
  aDatosPers[nroUsu].aCuentas[posNueva].saldo        = SALDO_INICIAL;
  aDatosPers[nroUsu].aCuentas[posNueva].estado       = ESTADO_ACTIVA;
  aDatosPers[nroUsu].cantCuentas++;

  // Mensaje con la forma exacta que pide el enunciado.
  snprintf(mensaje, sizeof mensaje, "Cuenta creada nro. %s", nroCtaNueva);
  Operacion::Exito(mensaje);

  // Se vuelve a listar para que se vea la cuenta recien agregada.
  cout << endl << "  Cuentas del usuario luego del alta:" << endl << endl;
  Operacion::ListarCuentas(aDatosPers, nroUsu);
  Operacion::Continuar();
} // Menu_NuevaCuenta

#endif // MENU_NUEVACUENTA_HPP
