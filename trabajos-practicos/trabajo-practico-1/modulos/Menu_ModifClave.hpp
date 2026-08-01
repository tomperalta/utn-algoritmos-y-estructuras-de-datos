// ---------------------------------------------------------------------------
// Menu_ModifClave.hpp
// Nivel 2 - Modificar la contrasenia del usuario.
//
// Segun el enunciado la operacion pide TRES claves: la actual, la nueva y la
// confirmacion de la nueva, y debe dar un mensaje apropiado en cada situacion:
//
//   * la contrasenia actual NO EXISTE (no coincide con la registrada) -> la
//     operacion no se realiza;
//   * la confirmacion NO COINCIDE con la contrasenia nueva;
//   * la nueva no puede ser igual a la actual (no seria un cambio);
//   * la nueva debe tener de LARGO_MINIMO a LARGO_CLAVE caracteres y combinar
//     al menos una letra y al menos un digito.
//
// Las tres claves se piden con Validar::LeerClave(), que muestra un asterisco
// por caracter: en ningun momento la contrasenia aparece en claro.
//
// Se admiten hasta INTENTOS_MAX intentos (ciclo 1-x), igual que en el login, y
// antes de aplicar el cambio se pide una confirmacion final.
//
// El cambio se aplica sobre aDatosPers[nroUsu].clave y vale para la sesion en
// curso: los datos personales estan registrados internamente en el codigo,
// porque el enunciado autoriza a grabar en disco unicamente los tres archivos
// de movimientos.
// ---------------------------------------------------------------------------
#ifndef MENU_MODIFCLAVE_HPP
#define MENU_MODIFCLAVE_HPP

// Largo minimo exigido a la contrasenia nueva.
const short MCLV_LARGO_MINIMO  = 6;
const short MCLV_LARGO_MENSAJE = 90;

// ---------------------------------------------------------------------------
// Indica si la cadena contiene por lo menos una letra. Se comparan los rangos
// ASCII en lugar de usar isalpha() para que el resultado no dependa de la
// configuracion regional de la consola.
// ---------------------------------------------------------------------------
static bool ModifClave_TieneLetra(const char clave[]) {
  bool hay = false;

  for (short i = 0; clave[i] != '\0' and not hay; i++)
    hay = (clave[i] >= 'a' and clave[i] <= 'z') or
          (clave[i] >= 'A' and clave[i] <= 'Z');
  return hay;
} // ModifClave_TieneLetra

// ---------------------------------------------------------------------------
// Indica si la cadena contiene por lo menos un digito.
// ---------------------------------------------------------------------------
static bool ModifClave_TieneDigito(const char clave[]) {
  bool hay = false;

  for (short i = 0; clave[i] != '\0' and not hay; i++)
    hay = clave[i] >= '0' and clave[i] <= '9';
  return hay;
} // ModifClave_TieneDigito

// ---------------------------------------------------------------------------
// Controla la contrasenia nueva contra todas las reglas y emite el mensaje que
// corresponde a la primera regla incumplida. Devuelve true si es aceptable.
// ---------------------------------------------------------------------------
static bool ModifClave_NuevaAceptable(const char nueva[],
                                      const char actual[]) {
  char  mensaje[MCLV_LARGO_MENSAJE + 1];
  short largo = (short) strlen(nueva);
  bool  aceptable = false;

  if (largo < MCLV_LARGO_MINIMO) {
    snprintf(mensaje, sizeof mensaje,
             "La contrasenia nueva debe tener al menos %d caracteres.",
             MCLV_LARGO_MINIMO);
    Operacion::Error(mensaje);
  }
  else if (largo > LARGO_CLAVE) {
    snprintf(mensaje, sizeof mensaje,
             "La contrasenia nueva no puede superar los %d caracteres.",
             LARGO_CLAVE);
    Operacion::Error(mensaje);
  }
  else if (not ModifClave_TieneLetra(nueva))
    Operacion::Error("La contrasenia nueva debe contener al menos una letra.");
  else if (not ModifClave_TieneDigito(nueva))
    Operacion::Error("La contrasenia nueva debe contener al menos un digito.");
  else if (strcmp(nueva, actual) == 0)
    Operacion::Error("La contrasenia nueva no puede ser igual a la actual.");
  else
    aceptable = true;
  return aceptable;
} // ModifClave_NuevaAceptable

// ---------------------------------------------------------------------------
// Pantalla de operacion. Cada pasada del ciclo es un intento completo: clave
// actual, clave nueva y confirmacion. El ciclo corta cuando el cambio queda
// resuelto, cuando se agotan los INTENTOS_MAX o cuando se agota la entrada.
// ---------------------------------------------------------------------------
void Menu_ModifClave(DatosPers aDatosPers[], short nroUsu) {
  str15 claveActual, claveNueva, claveConfirma;
  short intentos = 0;
  bool  datosOk, finalizado = false;

  do {
    Operacion::PantallaOperacion("MODIFICAR CONTRASENIA", aDatosPers, nroUsu);
    cout << "  Intento " << intentos + 1 << " de " << INTENTOS_MAX << endl;
    cout << "  La contrasenia nueva debe tener de " << MCLV_LARGO_MINIMO
         << " a " << LARGO_CLAVE << " caracteres," << endl;
    cout << "  con al menos una letra y al menos un digito." << endl << endl;

    // ----- 1) Contrasenia actual -----
    Validar::LeerClave("  Contrasenia actual.....: ", claveActual,
                       LARGO_CLAVE);
    if (Validar::EntradaAgotada())
      return;
    datosOk = strcmp(claveActual, aDatosPers[nroUsu].clave) == 0;
    if (not datosOk)
      Operacion::Error("La contrasenia actual NO EXISTE o es incorrecta: la "
                       "operacion no se realiza.");

    // ----- 2) Contrasenia nueva -----
    if (datosOk) {
      Validar::LeerClave("  Contrasenia nueva......: ", claveNueva,
                         LARGO_CLAVE);
      if (Validar::EntradaAgotada())
        return;
      datosOk = ModifClave_NuevaAceptable(claveNueva, claveActual);
    }

    // ----- 3) Confirmacion de la contrasenia nueva -----
    if (datosOk) {
      Validar::LeerClave("  Confirmar la nueva.....: ", claveConfirma,
                         LARGO_CLAVE);
      if (Validar::EntradaAgotada())
        return;
      datosOk = strcmp(claveNueva, claveConfirma) == 0;
      if (not datosOk)
        Operacion::Error("La confirmacion NO COINCIDE con la contrasenia "
                         "nueva.");
    }

    // ----- 4) Confirmacion final y actualizacion del registro -----
    if (datosOk) {
      finalizado = true;                 // el intento llego hasta el final
      if (Validar::LeerSiNo("  Confirma el cambio de contrasenia (S/N)?: ")
          and not Validar::EntradaAgotada()) {
        Validar::CopiarCadena(aDatosPers[nroUsu].clave, claveNueva,
                              LARGO_CLAVE);
        Operacion::Exito("La contrasenia fue modificada correctamente.");
        cout << "  Usela en su proximo ingreso al Home Banking." << endl;
      }
      else
        Operacion::Aviso("El cambio fue cancelado: la contrasenia no se "
                         "modifico.");
    }
    else {
      intentos++;
      if (intentos < INTENTOS_MAX)
        cout << "  Le quedan " << INTENTOS_MAX - intentos << " intento(s)."
             << endl;
    }
    Operacion::Continuar();
  } while (not finalizado and intentos < INTENTOS_MAX and
           not Validar::EntradaAgotada());

  if (not finalizado and intentos >= INTENTOS_MAX) {
    Operacion::PantallaOperacion("MODIFICAR CONTRASENIA", aDatosPers, nroUsu);
    Operacion::Error("Se agotaron los intentos permitidos.");
    cout << "  La contrasenia sigue siendo la anterior." << endl;
    Operacion::Continuar();
  }
} // Menu_ModifClave

#endif // MENU_MODIFCLAVE_HPP
