// ---------------------------------------------------------------------------
// Menu_DatosPers.hpp
// Nivel 2 - Mis datos personales.
//
// Muestra, con rotulos alineados, TODOS los campos del registro DatosPers del
// usuario que inicio la sesion: documento, apellido y nombre, fecha de
// nacimiento (en letras), edad calculada al dia de hoy, usuario, contrasenia
// enmascarada, celular, correo electronico, domicilio, numero de cuenta de la
// Caja de Ahorro y CBU, mas la ocupacion de sus arrays de cuentas y de CBU
// agendados.
//
// La contrasenia NUNCA se emite en claro: se la reemplaza por un asterisco por
// caracter, de modo que en pantalla solo se ve su longitud.
//
// Debajo del detalle se ofrece actualizar los DATOS DE CONTACTO (celular,
// correo electronico y domicilio) mediante un pequenio menu. Cada dato se pide
// con un ciclo 1-x hasta que resulta valido y se pide una confirmacion antes
// de tocar el registro.
//
// Los datos personales estan registrados internamente en el codigo (ver
// Datos::CargarDatosPersonales) y no en un archivo, porque el enunciado
// autoriza a escribir en disco unicamente los tres archivos de movimientos.
// Por eso las modificaciones valen para la SESION EN CURSO.
// ---------------------------------------------------------------------------
#ifndef MENU_DATOSPERS_HPP
#define MENU_DATOSPERS_HPP

// ----- Ancho de la columna de rotulos de esta pantalla -----
const short DPER_ANCHO_ROTULO = 24;

// ----- Opciones del menu de actualizacion de datos de contacto -----
const short DPER_OPC_CELULAR   = 1;
const short DPER_OPC_EMAIL     = 2;
const short DPER_OPC_DOMICILIO = 3;
const short DPER_OPC_VOLVER    = 4;

// ---------------------------------------------------------------------------
// Emite un rotulo relleno con puntos, para que todos los datos comiencen en la
// misma columna.
// ---------------------------------------------------------------------------
static void DatosPers_Rotulo(const char texto[]) {
  short relleno = (short)(DPER_ANCHO_ROTULO - Screen::LargoVisible(texto));

  Screen::_textcolor(COL_ROTULO);
  cout << "  " << texto;
  if (relleno > 0)
    Screen::Separador(relleno, '.');
  cout << ": ";
  Screen::_textcolor(COL_DATO);
} // DatosPers_Rotulo

// ---------------------------------------------------------------------------
// Deja en <<dest>> tantos asteriscos como caracteres tenga la clave. Se copia
// solamente la LONGITUD: el contenido no sale nunca a la pantalla.
// ---------------------------------------------------------------------------
static void DatosPers_Enmascarar(const char clave[], char dest[],
                                 short largoMax) {
  short largo = (short) strlen(clave);
  short i;

  if (largo > largoMax)
    largo = largoMax;
  for (i = 0; i < largo; i++)
    dest[i] = '*';
  dest[i] = '\0';
} // DatosPers_Enmascarar

// ---------------------------------------------------------------------------
// Edad cumplida al dia de hoy. Se resta un anio cuando el cumpleanios de este
// anio todavia no ocurrio.
// ---------------------------------------------------------------------------
static short DatosPers_EdadAlDiaDeHoy(Fecha fechaNac) {
  Fecha hoy;
  short edad;

  FechaHora::FechaDeHoy(hoy);
  edad = (short)(hoy.anio - fechaNac.anio);
  if (hoy.mes < fechaNac.mes or
      (hoy.mes == fechaNac.mes and hoy.dia < fechaNac.dia))
    edad--;
  return edad;
} // DatosPers_EdadAlDiaDeHoy

// ---------------------------------------------------------------------------
// Dibuja la pantalla completa con el detalle de los datos personales.
// ---------------------------------------------------------------------------
static void DatosPers_Mostrar(DatosPers aDatosPers[], short nroUsu) {
  const short LARGO_FECHA_LETRAS = 30; // "14 de Setiembre de 1983"
  char fechaNac[LARGO_FECHA_LETRAS + 1];
  char claveOculta[LARGO_CLAVE + 1];

  Operacion::PantallaOperacion("MIS DATOS PERSONALES", aDatosPers, nroUsu);

  FechaHora::FormatearFechaLetras(aDatosPers[nroUsu].fechaNac, fechaNac,
                                  LARGO_FECHA_LETRAS + 1);
  DatosPers_Enmascarar(aDatosPers[nroUsu].clave, claveOculta, LARGO_CLAVE);

  DatosPers_Rotulo("DNI");
  cout << aDatosPers[nroUsu].dni << endl;
  DatosPers_Rotulo("Apellido y nombre");
  cout << aDatosPers[nroUsu].apellidoNombre << endl;
  DatosPers_Rotulo("Fecha de nacimiento");
  cout << fechaNac << endl;
  DatosPers_Rotulo("Edad al dia de hoy");
  cout << DatosPers_EdadAlDiaDeHoy(aDatosPers[nroUsu].fechaNac) << " anios"
       << endl;
  DatosPers_Rotulo("Usuario");
  cout << aDatosPers[nroUsu].usuario << endl;
  DatosPers_Rotulo("Contrasenia");
  cout << claveOculta << "   (oculta por seguridad)" << endl;
  DatosPers_Rotulo("Celular");
  cout << aDatosPers[nroUsu].celular << endl;
  DatosPers_Rotulo("Correo electronico");
  cout << aDatosPers[nroUsu].email << endl;
  DatosPers_Rotulo("Domicilio");
  cout << aDatosPers[nroUsu].domicilio << endl;
  DatosPers_Rotulo("Numero de cuenta (CA)");
  cout << aDatosPers[nroUsu].nroCtaCA << endl;
  DatosPers_Rotulo("CBU");
  cout << aDatosPers[nroUsu].cbu << endl;
  DatosPers_Rotulo("Nro. interno de usuario");
  cout << aDatosPers[nroUsu].nroUsuario << endl;
  DatosPers_Rotulo("Cuentas registradas");
  cout << aDatosPers[nroUsu].cantCuentas << " de " << MAX_CUENTAS << endl;
  DatosPers_Rotulo("CBU agendados");
  cout << aDatosPers[nroUsu].cantAgenda << " de " << MAX_AGENDA << endl;
  Screen::ColorNormal();

  Operacion::Aviso("Los cambios valen para la sesion en curso: los datos "
                   "personales");
  cout << "  estan registrados internamente en el codigo y no en un archivo,"
       << endl;
  cout << "  porque en disco solo se graban los movimientos de CA, TD y TC."
       << endl;
} // DatosPers_Mostrar

// ---------------------------------------------------------------------------
// Actualiza el numero de celular. Ciclo 1-x: se insiste hasta que el numero
// sea valido (de 10 a 15 digitos, sin espacios ni guiones).
// ---------------------------------------------------------------------------
static void DatosPers_ActualizarCelular(DatosPers aDatosPers[], short nroUsu) {
  str15 nuevo;
  bool  valido = false;

  cout << endl << "  Celular actual: " << aDatosPers[nroUsu].celular << endl;
  do {
    Validar::LeerCadena("  Nuevo celular (solo digitos): ", nuevo,
                        LARGO_CELULAR, false);
    if (Validar::EntradaAgotada())
      return;
    valido = Validar::CelularValido(nuevo);
    if (not valido)
      Operacion::Error("El celular debe tener de 10 a 15 digitos, sin "
                       "espacios ni guiones.");
  } while (not valido and not Validar::EntradaAgotada());

  if (valido and Validar::LeerSiNo("  Confirma el cambio (S/N)?: ") and
      not Validar::EntradaAgotada()) {
    Validar::CopiarCadena(aDatosPers[nroUsu].celular, nuevo, LARGO_CELULAR);
    Operacion::Exito("Numero de celular actualizado.");
  }
  else
    Operacion::Aviso("No se modifico ningun dato.");
  Operacion::Continuar();
} // DatosPers_ActualizarCelular

// ---------------------------------------------------------------------------
// Actualiza el correo electronico. Ciclo 1-x hasta que la direccion tenga una
// sola arroba, texto antes y un punto despues.
// ---------------------------------------------------------------------------
static void DatosPers_ActualizarEmail(DatosPers aDatosPers[], short nroUsu) {
  str40 nuevo;
  bool  valido = false;

  cout << endl << "  Correo actual: " << aDatosPers[nroUsu].email << endl;
  do {
    Validar::LeerCadena("  Nuevo correo electronico: ", nuevo, LARGO_NOMBRE,
                        false);
    if (Validar::EntradaAgotada())
      return;
    valido = Validar::EmailValido(nuevo);
    if (not valido)
      Operacion::Error("La direccion debe tener el formato "
                       "nombre@servidor.com");
  } while (not valido and not Validar::EntradaAgotada());

  if (valido and Validar::LeerSiNo("  Confirma el cambio (S/N)?: ") and
      not Validar::EntradaAgotada()) {
    Validar::CopiarCadena(aDatosPers[nroUsu].email, nuevo, LARGO_NOMBRE);
    Operacion::Exito("Correo electronico actualizado.");
  }
  else
    Operacion::Aviso("No se modifico ningun dato.");
  Operacion::Continuar();
} // DatosPers_ActualizarEmail

// ---------------------------------------------------------------------------
// Actualiza el domicilio. Se normaliza a ASCII para que el dato ocupe un byte
// por caracter y no altere el largo fijo del registro en el archivo.
// ---------------------------------------------------------------------------
static void DatosPers_ActualizarDomicilio(DatosPers aDatosPers[],
                                          short nroUsu) {
  const short LARGO_DOM_MINIMO = 5;
  str40 nuevo;
  bool  valido = false;

  cout << endl << "  Domicilio actual: " << aDatosPers[nroUsu].domicilio
       << endl;
  do {
    Validar::LeerCadena("  Nuevo domicilio (calle, numero y ciudad): ", nuevo,
                        LARGO_NOMBRE, true);
    if (Validar::EntradaAgotada())
      return;
    valido = (short) strlen(nuevo) >= LARGO_DOM_MINIMO;
    if (not valido)
      Operacion::Error("El domicilio es demasiado corto. Indique calle, "
                       "numero y ciudad.");
  } while (not valido and not Validar::EntradaAgotada());

  if (valido and Validar::LeerSiNo("  Confirma el cambio (S/N)?: ") and
      not Validar::EntradaAgotada()) {
    Validar::CopiarCadena(aDatosPers[nroUsu].domicilio, nuevo, LARGO_NOMBRE);
    Operacion::Exito("Domicilio actualizado.");
  }
  else
    Operacion::Aviso("No se modifico ningun dato.");
  Operacion::Continuar();
} // DatosPers_ActualizarDomicilio

// ---------------------------------------------------------------------------
// Menu de nivel 3: que dato de contacto se quiere actualizar. Se construye con
// un ciclo 1-x, como todo menu, y se despacha por seleccion multiple.
// ---------------------------------------------------------------------------
static void DatosPers_MenuActualizar(DatosPers aDatosPers[], short nroUsu) {
  short opcion;
  bool  volver = false;

  do {
    Operacion::PantallaOperacion("ACTUALIZAR DATOS DE CONTACTO", aDatosPers,
                                 nroUsu);
    Screen::_textcolor(COL_MENU);
    cout << "   " << DPER_OPC_CELULAR   << ". Numero de celular   ("
         << aDatosPers[nroUsu].celular << ")" << endl;
    cout << "   " << DPER_OPC_EMAIL     << ". Correo electronico  ("
         << aDatosPers[nroUsu].email << ")" << endl;
    cout << "   " << DPER_OPC_DOMICILIO << ". Domicilio           ("
         << aDatosPers[nroUsu].domicilio << ")" << endl;
    cout << "   " << DPER_OPC_VOLVER    << ". Volver" << endl;
    Screen::ColorNormal();
    cout << endl;

    opcion = (short) Validar::LeerEntero("  Opcion: ", DPER_OPC_CELULAR,
                                         DPER_OPC_VOLVER);
    if (Validar::EntradaAgotada())
      volver = true;
    else
      switch (opcion) {                     // seleccion multiple
        case DPER_OPC_CELULAR   : DatosPers_ActualizarCelular(aDatosPers,
                                                            nroUsu);   break;
        case DPER_OPC_EMAIL     : DatosPers_ActualizarEmail(aDatosPers,
                                                          nroUsu);     break;
        case DPER_OPC_DOMICILIO : DatosPers_ActualizarDomicilio(aDatosPers,
                                                              nroUsu); break;
        default               : volver = true;
      }
  } while (not volver and not Validar::EntradaAgotada());
} // DatosPers_MenuActualizar

// ---------------------------------------------------------------------------
// Pantalla de operacion: detalle de los datos personales y actualizacion de
// los datos de contacto. El ciclo vuelve a mostrar el detalle despues de cada
// actualizacion, para que el usuario vea el dato ya modificado.
// ---------------------------------------------------------------------------
void Menu_DatosPers(DatosPers aDatosPers[], short nroUsu) {
  bool volver = false;

  do {
    DatosPers_Mostrar(aDatosPers, nroUsu);
    if (Validar::EntradaAgotada())
      return;
    // LeerSiNo() devuelve el valor por omision cuando la entrada se agota:
    // por eso se vuelve a consultar EntradaAgotada() antes de seguir.
    if (Validar::LeerSiNo("  Desea actualizar sus datos de contacto (S/N)?: ")
        and not Validar::EntradaAgotada())
      DatosPers_MenuActualizar(aDatosPers, nroUsu);
    else
      volver = true;
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_DatosPers

#endif // MENU_DATOSPERS_HPP
