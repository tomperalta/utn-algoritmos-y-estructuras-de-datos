// ---------------------------------------------------------------------------
// MenuLogin.hpp
// Nivel 1 - Ingreso al sitio.
//
// Solicita DNI, usuario y clave, y los verifica contra el array de registros
// de datos personales. Se admiten hasta INTENTOS_MAX intentos; agotados los
// intentos el ingreso queda rechazado.
//
// La busqueda del DNI dentro del array es SECUENCIAL, porque el array de
// usuarios se conserva en el orden en que fueron dados de alta y no esta
// ordenado por documento.
// ---------------------------------------------------------------------------
#ifndef MENULOGIN_HPP
#define MENULOGIN_HPP

// ---------------------------------------------------------------------------
// Devuelve por parametro y como resultado la posicion del usuario que ingreso
// al sitio, o NO_ENCONTRADO si el ingreso fue rechazado o cancelado.
// ---------------------------------------------------------------------------
short MenuLogin(DatosPers aDatosPers[], short &usuario) {
  const long DNI_MINIMO = 1000000, DNI_MAXIMO = 99999999;
  char  usuIngresado[LARGO_USUARIO + 1];
  char  claveIngresada[LARGO_CLAVE + 1];
  long  dni;
  short pos, intentos = 0;
  bool  ingresoOk = false;

  usuario = NO_ENCONTRADO;
  do {
    Operacion::PantallaOperacion("INGRESO AL HOME BANKING", aDatosPers,
                                 NO_ENCONTRADO);
    cout << "  Intento " << intentos + 1 << " de " << INTENTOS_MAX << endl;
    cout << "  (ingrese 0 en el DNI para salir del sistema)" << endl << endl;

    dni = Validar::LeerEntero("  DNI.............: ", 0, DNI_MAXIMO);
    if (dni == 0 or Validar::EntradaAgotada())
      return NO_ENCONTRADO;
    if (dni < DNI_MINIMO) {
      Operacion::Error("El DNI debe tener entre 7 y 8 digitos.");
      intentos++;
      Operacion::Continuar();
      continue;
    }

    Validar::LeerCadena("  Usuario.........: ", usuIngresado, LARGO_USUARIO,
                        false);
    Validar::LeerClave("  Clave...........: ", claveIngresada, LARGO_CLAVE);

    // Busqueda secuencial del DNI dentro del array de usuarios.
    pos = Ordenar::SecuencialDni(aDatosPers, MAX_USUARIOS, dni);
    ingresoOk = pos != NO_ENCONTRADO and
                strcmp(aDatosPers[pos].usuario, usuIngresado) == 0 and
                strcmp(aDatosPers[pos].clave, claveIngresada) == 0;
    intentos++;

    if (ingresoOk) {
      usuario = pos;
      Operacion::Exito("Ingreso correcto.");
      cout << "  Bienvenido/a " << aDatosPers[pos].apellidoNombre << endl;
      Operacion::Continuar();
    }
    else {
      Operacion::Error("DNI, usuario o clave incorrectos.");
      if (intentos < INTENTOS_MAX)
        cout << "  Le quedan " << INTENTOS_MAX - intentos << " intento(s)."
             << endl;
      Operacion::Continuar();
    }
  } while (not ingresoOk and intentos < INTENTOS_MAX and
           not Validar::EntradaAgotada());

  if (not ingresoOk and intentos >= INTENTOS_MAX) {
    Operacion::PantallaOperacion("INGRESO RECHAZADO", aDatosPers,
                                 NO_ENCONTRADO);
    Operacion::Error("Se agotaron los intentos permitidos.");
    cout << "  Por seguridad el acceso quedo bloqueado." << endl;
    Operacion::Continuar();
  }
  return usuario;
} // MenuLogin

#endif // MENULOGIN_HPP
