// ---------------------------------------------------------------------------
// Menu_MostrarCBU.hpp
// Nivel 2 - Mostrar mi CBU.
//
// Presenta los datos que el usuario necesita entregar para RECIBIR una
// transferencia: titular, documento, numero de cuenta, tipo, moneda y el CBU
// de 22 digitos de su Caja de Ahorro en pesos.
//
// El CBU se emite de dos maneras:
//   * completo, tal como esta registrado, para copiarlo de una sola vez;
//   * separado en sus dos bloques normalizados, de 8 y de 14 digitos, que es
//     como lo muestran los bancos para que se pueda leer y dictar sin errores.
//         bloque 1 -> entidad (3) + sucursal (4) + digito verificador (1)
//         bloque 2 -> cuenta  (13) + digito verificador (1)
//
// Antes de mostrarlo se lo verifica con Validar::CBUValido(), que controla que
// tenga exactamente LARGO_CBU digitos: si el dato registrado estuviera mal
// cargado, se emite un error en lugar de mostrar un CBU incompleto.
//
// Ademas se arma con snprintf() un ALIAS derivado del apellido del titular,
// que es la forma abreviada con la que tambien se puede recibir dinero.
// ---------------------------------------------------------------------------
#ifndef MENU_MOSTRARCBU_HPP
#define MENU_MOSTRARCBU_HPP

// Ancho de la columna de rotulos de esta pantalla.
const short MCBU_ANCHO_ROTULO = 24;

// ---------------------------------------------------------------------------
// Emite un rotulo relleno con puntos hasta la columna del dato, de modo que
// todos los valores queden alineados en la misma columna.
// ---------------------------------------------------------------------------
static void MostrarCBU_Rotulo(const char texto[]) {
  short relleno = (short)(MCBU_ANCHO_ROTULO - Screen::LargoVisible(texto));

  Screen::_textcolor(COL_ROTULO);
  cout << "  " << texto;
  if (relleno > 0)
    Screen::Separador(relleno, '.');
  cout << ": ";
  Screen::_textcolor(COL_DATO);
} // MostrarCBU_Rotulo

// ---------------------------------------------------------------------------
// Arma el alias tomando la PRIMERA palabra de <<apellidoNombre>> (el apellido)
// y pasandola a minusculas. Se copian solamente las letras: los alias de CBU
// no admiten espacios ni signos de puntuacion.
// ---------------------------------------------------------------------------
static void MostrarCBU_ApellidoEnMinusculas(const char apellidoNombre[],
                                            char dest[], short largoMax) {
  short i = 0, j = 0;

  while (apellidoNombre[i] != '\0' and apellidoNombre[i] != ' ' and
         j < largoMax) {
    if (apellidoNombre[i] >= 'A' and apellidoNombre[i] <= 'Z')
      dest[j++] = (char)(apellidoNombre[i] - 'A' + 'a');
    else if (apellidoNombre[i] >= 'a' and apellidoNombre[i] <= 'z')
      dest[j++] = apellidoNombre[i];
    i++;
  }
  dest[j] = '\0';
} // MostrarCBU_ApellidoEnMinusculas

// ---------------------------------------------------------------------------
// Pantalla de operacion: solo consulta, no modifica ningun dato.
// ---------------------------------------------------------------------------
void Menu_MostrarCBU(DatosPers aDatosPers[], short nroUsu) {
  const short LARGO_BLOQUE_ENT = 8;  // entidad + sucursal + verificador
  const short LARGO_BLOQUE_CTA = 14; // cuenta + verificador
  const short LARGO_APELLIDO   = 12; // recorte del apellido para el alias
  char  bloqueEntidad[LARGO_BLOQUE_ENT + 1];
  char  bloqueCuenta[LARGO_BLOQUE_CTA + 1];
  char  apellido[LARGO_APELLIDO + 1];
  str25 alias;
  short iCta;

  Operacion::PantallaOperacion("MI CBU Y DATOS PARA RECIBIR TRANSFERENCIAS",
                               aDatosPers, nroUsu);

  // Busqueda secuencial de la Caja de Ahorro en pesos dentro del array de
  // cuentas del usuario: son pocas componentes y no estan ordenadas.
  iCta = Operacion::IndiceCuentaCA(aDatosPers, nroUsu);

  MostrarCBU_Rotulo("Titular");
  cout << aDatosPers[nroUsu].apellidoNombre << endl;
  MostrarCBU_Rotulo("DNI");
  cout << aDatosPers[nroUsu].dni << endl;
  MostrarCBU_Rotulo("Numero de cuenta (CA)");
  cout << aDatosPers[nroUsu].nroCtaCA << endl;
  MostrarCBU_Rotulo("Tipo de cuenta");
  if (iCta == NO_ENCONTRADO)
    cout << "CA (Caja de Ahorro)" << endl;
  else
    cout << aDatosPers[nroUsu].aCuentas[iCta].tipo << " (Caja de Ahorro)"
         << endl;
  MostrarCBU_Rotulo("Moneda");
  if (iCta == NO_ENCONTRADO)
    cout << "$ (pesos)" << endl;
  else
    cout << aDatosPers[nroUsu].aCuentas[iCta].moneda << " (pesos)" << endl;
  Screen::ColorNormal();

  cout << endl;

  // ----- El CBU, en sus dos formatos -----
  if (not Validar::CBUValido(aDatosPers[nroUsu].cbu)) {
    Operacion::Error("El CBU registrado no tiene los 22 digitos requeridos.");
    cout << "  Dato registrado: " << aDatosPers[nroUsu].cbu << endl;
    cout << "  Comuniquese con su sucursal para regularizar la situacion."
         << endl;
  }
  else {
    Validar::CopiarCadena(bloqueEntidad, aDatosPers[nroUsu].cbu,
                          LARGO_BLOQUE_ENT);
    Validar::CopiarCadena(bloqueCuenta,
                          aDatosPers[nroUsu].cbu + LARGO_BLOQUE_ENT,
                          LARGO_BLOQUE_CTA);

    MostrarCBU_Rotulo("CBU completo");
    Screen::_textcolor(COL_EXITO);
    cout << aDatosPers[nroUsu].cbu << endl;
    Screen::ColorNormal();
    MostrarCBU_Rotulo("Bloque 1 (8 digitos)");
    cout << bloqueEntidad << "   entidad, sucursal y verificador" << endl;
    MostrarCBU_Rotulo("Bloque 2 (14 digitos)");
    cout << bloqueCuenta << "   cuenta y verificador" << endl;
    Screen::ColorNormal();
  }

  // ----- Alias derivado del apellido -----
  MostrarCBU_ApellidoEnMinusculas(aDatosPers[nroUsu].apellidoNombre, apellido,
                                  LARGO_APELLIDO);
  snprintf(alias, sizeof alias, "%s.mesopotamia", apellido);
  cout << endl;
  MostrarCBU_Rotulo("Alias del CBU");
  Screen::_textcolor(COL_EXITO);
  cout << alias << endl;
  Screen::ColorNormal();

  Operacion::Aviso("Con el CBU o con el alias cualquier banco puede "
                   "transferirle dinero.");
  cout << "  Los datos son publicos para recibir fondos: con ellos NO se puede"
       << endl;
  cout << "  extraer dinero de su cuenta." << endl;

  Operacion::Continuar();
} // Menu_MostrarCBU

#endif // MENU_MOSTRARCBU_HPP
