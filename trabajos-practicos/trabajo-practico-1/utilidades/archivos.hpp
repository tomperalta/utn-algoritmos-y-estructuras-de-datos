// ---------------------------------------------------------------------------
// archivos.hpp
// Acceso a los archivos de texto de movimientos.
//
//   MovimientosCA.Txt  -> Caja de Ahorro     (58 bytes + salto de linea)
//   MovimientosTD.Txt  -> Tarjeta de Debito  (56 bytes + salto de linea)
//   MovimientosTC.Txt  -> Tarjeta de Credito (71 bytes + salto de linea)
//
// Formato tabular: cada campo ocupa una cantidad fija de columnas; los
// numeros se ajustan a la derecha y las cadenas a la izquierda rellenando con
// espacios en blanco. Los archivos NO llevan encabezados: contienen solamente
// los datos.
//
// Como todos los registros tienen el mismo largo en bytes, el archivo admite
// las dos formas de acceso que pide el enunciado:
//
//   * ACCESO SECUENCIAL: se recorre el archivo de principio a fin leyendo
//     linea por linea (LeerMovimientos*), y se agregan nuevas componentes al
//     final (AgregarMovimiento*).
//   * ACCESO ALEATORIO : se calcula el desplazamiento del registro que se
//     quiere leer o actualizar (nroReg * largoRegistro) y se posiciona el
//     puntero del archivo con seekg()/seekp() (LeerRegistro*, ActualizarRegistro*).
//
// Los archivos se abren siempre en modo binario y el salto de linea se escribe
// en forma explicita, de manera que el largo del registro sea el mismo en
// Windows y en macOS/Linux. Al leer se tolera igualmente el par CR+LF.
// ---------------------------------------------------------------------------
#ifndef ARCHIVOS_HPP
#define ARCHIVOS_HPP

namespace Archivo {

  const short LARGO_LINEA = 256; // buffer de lectura de una linea

  // ----- Desplazamiento (base 0) y ancho de cada campo dentro del registro --
  // Caja de Ahorro
  const short CA_POS_FECHA   =  0, CA_ANC_FECHA   = ANCHO_FECHA;
  const short CA_POS_TIPO    = 11, CA_ANC_TIPO    = ANCHO_TIPOMOV;
  const short CA_POS_DETALLE = 13, CA_ANC_DETALLE = ANCHO_COLDET;
  const short CA_POS_IMPORTE = 44, CA_ANC_IMPORTE = ANCHO_IMPORTE;
  const short CA_POS_USUARIO = 57, CA_ANC_USUARIO = ANCHO_USUARIO;
  // Tarjeta de Debito
  const short TD_POS_FECHA   =  0, TD_ANC_FECHA   = ANCHO_FECHA;
  const short TD_POS_DETALLE = 11, TD_ANC_DETALLE = ANCHO_COLDET;
  const short TD_POS_IMPORTE = 42, TD_ANC_IMPORTE = ANCHO_IMPORTE;
  const short TD_POS_USUARIO = 55, TD_ANC_USUARIO = ANCHO_USUARIO;
  // Tarjeta de Credito
  const short TC_POS_FECHA   =  0, TC_ANC_FECHA   = ANCHO_FECHA;
  const short TC_POS_DETALLE = 11, TC_ANC_DETALLE = ANCHO_COLDET;
  const short TC_POS_CUOTAS  = 42, TC_ANC_CUOTAS  = ANCHO_CUOTA;
  const short TC_POS_CPBTE   = 48, TC_ANC_CPBTE   = ANCHO_CPBTE;
  const short TC_POS_IMPORTE = 57, TC_ANC_IMPORTE = ANCHO_IMPORTE;
  const short TC_POS_USUARIO = 70, TC_ANC_USUARIO = ANCHO_USUARIO;

  // Importe maximo representable en el formato 9(7).99
  const double MAX_IMPORTE = 9999999.99;

  // -------------------------------------------------------------------------
  // Indica si el archivo existe y se puede abrir para lectura.
  // -------------------------------------------------------------------------
  bool Existe(const char nombreArch[]) {
    ifstream arch(nombreArch, ios::in | ios::binary);
    bool     abierto = arch.is_open();

    arch.close();
    return abierto;
  } // Existe

  // -------------------------------------------------------------------------
  // Largo real de un registro del archivo, EN BYTES, incluido el salto de
  // linea. Se determina leyendo hasta el primer '\n', de modo que funciona
  // tanto si el archivo usa LF (macOS/Linux) como CR+LF (Windows).
  // Devuelve 0 si el archivo no existe o esta vacio.
  // -------------------------------------------------------------------------
  // -------------------------------------------------------------------------
  // Largo en bytes que deben tener los DATOS de un registro de cada archivo,
  // sin contar el salto de linea.
  // -------------------------------------------------------------------------
  short LargoDatosDe(const char nombreArch[]) {
    if (strcmp(nombreArch, ARCH_MOV_CA) == 0) return LARGO_REG_CA;
    if (strcmp(nombreArch, ARCH_MOV_TD) == 0) return LARGO_REG_TD;
    if (strcmp(nombreArch, ARCH_MOV_TC) == 0) return LARGO_REG_TC;
    return 0;
  } // LargoDatosDe

  long LargoRegistro(const char nombreArch[]) {
    ifstream arch(nombreArch, ios::in | ios::binary);

    if (not arch.is_open())
      return 0;

    long  largo = 0;
    char  car;
    bool  hallado = false;

    while (not hallado and arch.get(car)) {
      largo++;
      hallado = (car == '\n');
    }
    arch.close();
    if (not hallado)
      return 0;

    // El largo se toma de la PRIMERA linea, de modo que hay que comprobar que
    // sea el que corresponde: si el archivo empezara con un renglon en blanco
    // o con una linea de largo distinto, todo el acceso aleatorio apuntaria a
    // lugares equivocados. Se admiten las dos terminaciones de linea posibles:
    // LF (un byte) y CR+LF (dos bytes).
    short esperado = LargoDatosDe(nombreArch);
    if (esperado != 0 and largo != esperado + 1 and largo != esperado + 2)
      return 0;                        // formato inesperado: sin acceso aleatorio
    return largo;
  } // LargoRegistro

  // -------------------------------------------------------------------------
  // Terminador de linea con el que fue escrito el archivo: CR+LF si se lo creo
  // con un editor de Windows, LF en cualquier otro caso. Las componentes
  // nuevas se agregan con el MISMO terminador, para que todos los registros
  // sigan ocupando la misma cantidad de bytes.
  // -------------------------------------------------------------------------
  void TerminadorDeLinea(const char nombreArch[], char terminador[]) {
    ifstream arch(nombreArch, ios::in | ios::binary);
    char     anterior = '\0', car;

    strcpy(terminador, "\n");
    if (not arch.is_open())
      return;
    while (arch.get(car)) {
      if (car == '\n') {
        if (anterior == '\r')
          strcpy(terminador, "\r\n");
        break;
      }
      anterior = car;
    }
    arch.close();
  } // TerminadorDeLinea

  // -------------------------------------------------------------------------
  // Se asegura de que el archivo termine en salto de linea antes de anexarle
  // una componente nueva. Si la ultima linea quedo sin terminador -algo
  // habitual cuando el archivo se preparo con un editor de texto- el registro
  // nuevo se pegaria al anterior y los dos se perderian.
  // -------------------------------------------------------------------------
  void AsegurarFinDeLinea(const char nombreArch[], const char terminador[]) {
    ifstream arch(nombreArch, ios::in | ios::binary | ios::ate);

    if (not arch.is_open())
      return;

    long tamanio = (long) arch.tellg();
    char ultimo = '\n';

    if (tamanio > 0) {
      arch.seekg(tamanio - 1, ios::beg);
      arch.get(ultimo);
    }
    arch.close();
    if (tamanio > 0 and ultimo != '\n') {
      ofstream salida(nombreArch, ios::out | ios::app | ios::binary);
      if (salida.is_open()) {
        salida << terminador;
        salida.close();
      }
    }
  } // AsegurarFinDeLinea

  // -------------------------------------------------------------------------
  // Cantidad de registros del archivo = tamanio en bytes / largo del registro.
  // -------------------------------------------------------------------------
  long CantidadRegistros(const char nombreArch[]) {
    long largoReg = LargoRegistro(nombreArch);

    if (largoReg == 0)
      return 0;

    ifstream arch(nombreArch, ios::in | ios::binary | ios::ate);
    if (not arch.is_open())
      return 0;
    long tamanio = (long) arch.tellg();
    arch.close();

    // Si la ultima linea quedo sin salto de linea, el resto de la division no
    // da cero: ese registro existe igual y hay que contarlo.
    return tamanio / largoReg + (tamanio % largoReg != 0 ? 1 : 0);
  } // CantidadRegistros

  // -------------------------------------------------------------------------
  // Extrae el campo que comienza en la columna <<desde>> (base 0) y ocupa
  // <<ancho>> columnas, y lo deja recortado en <<dest>>.
  // -------------------------------------------------------------------------
  void ExtraerCampo(const char linea[], short desde, short ancho,
                    char dest[]) {
    strncpy(dest, linea + desde, ancho);
    dest[ancho] = '\0';
    Validar::Recortar(dest);
  } // ExtraerCampo

  // -------------------------------------------------------------------------
  // Interpreta un campo de fecha con formato 99-99-9999.
  // -------------------------------------------------------------------------
  bool ExtraerFecha(const char linea[], short desde, Fecha &f) {
    char campo[ANCHO_FECHA + 1];
    int  dia = 0, mes = 0, anio = 0;

    ExtraerCampo(linea, desde, ANCHO_FECHA, campo);
    if (sscanf(campo, "%d-%d-%d", &dia, &mes, &anio) != 3)
      return false;
    f.dia  = (short) dia;
    f.mes  = (short) mes;
    f.anio = (short) anio;
    return FechaHora::FechaValida(f);
  } // ExtraerFecha

  double ExtraerReal(const char linea[], short desde, short ancho) {
    char campo[ANCHO_IMPORTE + 1];

    ExtraerCampo(linea, desde, ancho, campo);
    return atof(campo);
  } // ExtraerReal

  long ExtraerEntero(const char linea[], short desde, short ancho) {
    char campo[ANCHO_CPBTE + 1];

    ExtraerCampo(linea, desde, ancho, campo);
    return atol(campo);
  } // ExtraerEntero

  // -------------------------------------------------------------------------
  // Quita el salto de linea (LF o CR+LF) del final de la linea leida.
  // -------------------------------------------------------------------------
  void QuitarSaltoDeLinea(char linea[]) {
    short largo = (short) strlen(linea);

    while (largo > 0 and (linea[largo - 1] == '\n' or linea[largo - 1] == '\r'))
      linea[--largo] = '\0';
  } // QuitarSaltoDeLinea

  // =========================================================================
  //                        CAJA DE AHORRO  (MovimientosCA.Txt)
  // =========================================================================

  // -------------------------------------------------------------------------
  // Arma la linea de texto de un movimiento de Caja de Ahorro. Devuelve false
  // si el registro no respeta el largo previsto (importe fuera de formato).
  // -------------------------------------------------------------------------
  bool ArmarLineaCA(MovCA m, char linea[]) {
    if (m.importe < 0 or m.importe > MAX_IMPORTE)
      return false;
    snprintf(linea, LARGO_LINEA, "%2d-%2d-%4d %c %-*s %*.*f %*d",
             m.fecha.dia, m.fecha.mes, m.fecha.anio, m.tipoMov,
             ANCHO_COLDET, m.detalle,
             ANCHO_IMPORTE, DECIMALES, m.importe,
             ANCHO_USUARIO, m.nroUsuario);
    return (short) strlen(linea) == LARGO_REG_CA;
  } // ArmarLineaCA

  // -------------------------------------------------------------------------
  // Interpreta una linea del archivo de Caja de Ahorro.
  // -------------------------------------------------------------------------
  bool DesarmarLineaCA(const char linea[], MovCA &m) {
    char campo[ANCHO_COLDET + 1];

    if ((short) strlen(linea) < LARGO_REG_CA)
      return false;
    if (not ExtraerFecha(linea, CA_POS_FECHA, m.fecha))
      return false;
    m.tipoMov = linea[CA_POS_TIPO];
    if (m.tipoMov != MOV_ACREDITA and m.tipoMov != MOV_DEBITA)
      return false;
    ExtraerCampo(linea, CA_POS_DETALLE, CA_ANC_DETALLE, campo);
    Validar::CopiarCadena(m.detalle, campo, LARGO_DETALLE);
    m.importe    = ExtraerReal(linea, CA_POS_IMPORTE, CA_ANC_IMPORTE);
    m.nroUsuario = (short) ExtraerEntero(linea, CA_POS_USUARIO, CA_ANC_USUARIO);
    m.saldo      = 0;
    m.nroReg     = 0;
    return m.nroUsuario >= 1 and m.nroUsuario <= MAX_USUARIOS;
  } // DesarmarLineaCA

  // -------------------------------------------------------------------------
  // ACCESO SECUENCIAL: lee todos los movimientos del usuario indicado. Si
  // nroUsuario es 0 lee los de todos los usuarios. Devuelve la cantidad leida
  // y deja en cada componente el numero de registro dentro del archivo.
  // -------------------------------------------------------------------------
  short LeerMovimientosCA(MovCA a[], short nroUsuario, short maximo) {
    ifstream arch(ARCH_MOV_CA, ios::in | ios::binary);
    char     linea[LARGO_LINEA];
    MovCA    mov;
    short    cant = 0, nroReg = 0;

    if (not arch.is_open())
      return 0;
    while (cant < maximo and arch.getline(linea, LARGO_LINEA)) {
      QuitarSaltoDeLinea(linea);
      // nroReg lleva la posicion FISICA de la linea dentro del archivo, y por
      // eso avanza tambien cuando la linea no se pudo interpretar. Si solo
      // contara las validas, los numeros de registro quedarian corridos y el
      // acceso aleatorio leeria o actualizaria la componente equivocada.
      if (DesarmarLineaCA(linea, mov) and
          (nroUsuario == 0 or mov.nroUsuario == nroUsuario)) {
        mov.nroReg = nroReg;
        a[cant++] = mov;
      }
      nroReg++;
    }
    arch.close();
    return cant;
  } // LeerMovimientosCA

  // -------------------------------------------------------------------------
  // Recorre TODO el archivo de Caja de Ahorro acumulando, para el usuario
  // indicado, los importes que acreditan y los que debitan. Devuelve la
  // cantidad de movimientos considerados.
  //
  // No usa un array intermedio a proposito: el saldo tiene que salir bien por
  // muchos movimientos que llegue a tener el archivo, y un array siempre tiene
  // un tope. Si se calculara el saldo sobre un array de MAX_MOV_CA
  // componentes, en cuanto el usuario superara esa cantidad de movimientos el
  // saldo quedaria mal calculado en silencio.
  // -------------------------------------------------------------------------
  long AcumularMovimientosCA(short nroUsuario, double &acredita,
                             double &debita) {
    ifstream arch(ARCH_MOV_CA, ios::in | ios::binary);
    char     linea[LARGO_LINEA];
    MovCA    mov;
    long     cant = 0;

    acredita = 0;
    debita   = 0;
    if (not arch.is_open())
      return 0;
    while (arch.getline(linea, LARGO_LINEA)) {
      QuitarSaltoDeLinea(linea);
      if (DesarmarLineaCA(linea, mov) and
          (nroUsuario == 0 or mov.nroUsuario == nroUsuario)) {
        if (mov.tipoMov == MOV_ACREDITA)
          acredita += mov.importe;
        else
          debita += mov.importe;
        cant++;
      }
    }
    arch.close();
    return cant;
  } // AcumularMovimientosCA

  // -------------------------------------------------------------------------
  // Cantidad de movimientos que el usuario tiene en el archivo de Caja de
  // Ahorro, sin el tope de ningun array.
  // -------------------------------------------------------------------------
  long ContarMovimientosCA(short nroUsuario) {
    double acredita, debita;

    return AcumularMovimientosCA(nroUsuario, acredita, debita);
  } // ContarMovimientosCA

  // -------------------------------------------------------------------------
  // Acumula los importes que DEBITAN la Caja de Ahorro del usuario en una
  // fecha determinada y cuyo detalle empieza con el prefijo indicado.
  // Se usa para controlar el tope diario de transferencias.
  // -------------------------------------------------------------------------
  double AcumularDebitosDelDia(short nroUsuario, Fecha fecha,
                               const char prefijo[]) {
    ifstream arch(ARCH_MOV_CA, ios::in | ios::binary);
    char     linea[LARGO_LINEA];
    MovCA    mov;
    double   total = 0;
    short    largoPrefijo = (short) strlen(prefijo);

    if (not arch.is_open())
      return 0;
    while (arch.getline(linea, LARGO_LINEA)) {
      QuitarSaltoDeLinea(linea);
      if (DesarmarLineaCA(linea, mov) and
          mov.nroUsuario == nroUsuario and
          mov.tipoMov == MOV_DEBITA and
          FechaHora::FechaLarga(mov.fecha) == FechaHora::FechaLarga(fecha) and
          strncmp(mov.detalle, prefijo, largoPrefijo) == 0)
        total += mov.importe;
    }
    arch.close();
    return total;
  } // AcumularDebitosDelDia

  // -------------------------------------------------------------------------
  // Graba una NUEVA componente al final del archivo (acceso secuencial de
  // salida, sin destruir lo ya existente).
  // -------------------------------------------------------------------------
  bool AgregarMovimientoCA(MovCA m) {
    char linea[LARGO_LINEA];

    if (not ArmarLineaCA(m, linea))
      return false;

    char terminador[3];

    TerminadorDeLinea(ARCH_MOV_CA, terminador);
    AsegurarFinDeLinea(ARCH_MOV_CA, terminador);

    ofstream arch(ARCH_MOV_CA, ios::out | ios::app | ios::binary);
    if (not arch.is_open())
      return false;
    arch << linea << terminador;
    arch.close();
    return true;
  } // AgregarMovimientoCA

  // -------------------------------------------------------------------------
  // ACCESO ALEATORIO: lee el registro numero <<nroReg>> (base 0) sin recorrer
  // los anteriores.
  // -------------------------------------------------------------------------
  bool LeerRegistroCA(long nroReg, MovCA &m) {
    long largoReg = LargoRegistro(ARCH_MOV_CA);

    if (largoReg == 0 or nroReg < 0 or nroReg >= CantidadRegistros(ARCH_MOV_CA))
      return false;

    ifstream arch(ARCH_MOV_CA, ios::in | ios::binary);
    char     linea[LARGO_LINEA];

    if (not arch.is_open())
      return false;
    arch.seekg(nroReg * largoReg, ios::beg);
    arch.getline(linea, LARGO_LINEA);
    arch.close();
    QuitarSaltoDeLinea(linea);
    if (not DesarmarLineaCA(linea, m))
      return false;
    m.nroReg = (short) nroReg;
    return true;
  } // LeerRegistroCA

  // -------------------------------------------------------------------------
  // ACCESO ALEATORIO: actualiza (sobrescribe) el registro numero <<nroReg>>.
  // Solo es posible porque todos los registros tienen el mismo largo.
  // -------------------------------------------------------------------------
  bool ActualizarRegistroCA(long nroReg, MovCA m) {
    long largoReg = LargoRegistro(ARCH_MOV_CA);
    char linea[LARGO_LINEA];

    if (largoReg == 0 or nroReg < 0 or nroReg >= CantidadRegistros(ARCH_MOV_CA))
      return false;
    if (not ArmarLineaCA(m, linea))
      return false;

    fstream arch(ARCH_MOV_CA, ios::in | ios::out | ios::binary);
    if (not arch.is_open())
      return false;
    arch.seekp(nroReg * largoReg, ios::beg);
    arch.write(linea, LARGO_REG_CA);
    arch.close();
    return true;
  } // ActualizarRegistroCA

  // =========================================================================
  //                     TARJETA DE DEBITO  (MovimientosTD.Txt)
  // =========================================================================

  bool ArmarLineaTD(MovTD m, char linea[]) {
    if (m.importe < 0 or m.importe > MAX_IMPORTE)
      return false;
    snprintf(linea, LARGO_LINEA, "%2d-%2d-%4d %-*s %*.*f %*d",
             m.fecha.dia, m.fecha.mes, m.fecha.anio,
             ANCHO_COLDET, m.detalle,
             ANCHO_IMPORTE, DECIMALES, m.importe,
             ANCHO_USUARIO, m.nroUsuario);
    return (short) strlen(linea) == LARGO_REG_TD;
  } // ArmarLineaTD

  bool DesarmarLineaTD(const char linea[], MovTD &m) {
    char campo[ANCHO_COLDET + 1];

    if ((short) strlen(linea) < LARGO_REG_TD)
      return false;
    if (not ExtraerFecha(linea, TD_POS_FECHA, m.fecha))
      return false;
    ExtraerCampo(linea, TD_POS_DETALLE, TD_ANC_DETALLE, campo);
    Validar::CopiarCadena(m.detalle, campo, LARGO_DETALLE);
    m.importe    = ExtraerReal(linea, TD_POS_IMPORTE, TD_ANC_IMPORTE);
    m.nroUsuario = (short) ExtraerEntero(linea, TD_POS_USUARIO, TD_ANC_USUARIO);
    m.nroReg     = 0;
    return m.nroUsuario >= 1 and m.nroUsuario <= MAX_USUARIOS;
  } // DesarmarLineaTD

  short LeerMovimientosTD(MovTD a[], short nroUsuario, short maximo) {
    ifstream arch(ARCH_MOV_TD, ios::in | ios::binary);
    char     linea[LARGO_LINEA];
    MovTD    mov;
    short    cant = 0, nroReg = 0;

    if (not arch.is_open())
      return 0;
    while (cant < maximo and arch.getline(linea, LARGO_LINEA)) {
      QuitarSaltoDeLinea(linea);
      // nroReg lleva la posicion FISICA de la linea dentro del archivo, y por
      // eso avanza tambien cuando la linea no se pudo interpretar. Si solo
      // contara las validas, los numeros de registro quedarian corridos y el
      // acceso aleatorio leeria o actualizaria la componente equivocada.
      if (DesarmarLineaTD(linea, mov) and
          (nroUsuario == 0 or mov.nroUsuario == nroUsuario)) {
        mov.nroReg = nroReg;
        a[cant++] = mov;
      }
      nroReg++;
    }
    arch.close();
    return cant;
  } // LeerMovimientosTD

  bool AgregarMovimientoTD(MovTD m) {
    char linea[LARGO_LINEA];

    if (not ArmarLineaTD(m, linea))
      return false;

    char terminador[3];

    TerminadorDeLinea(ARCH_MOV_TD, terminador);
    AsegurarFinDeLinea(ARCH_MOV_TD, terminador);

    ofstream arch(ARCH_MOV_TD, ios::out | ios::app | ios::binary);
    if (not arch.is_open())
      return false;
    arch << linea << terminador;
    arch.close();
    return true;
  } // AgregarMovimientoTD

  bool LeerRegistroTD(long nroReg, MovTD &m) {
    long largoReg = LargoRegistro(ARCH_MOV_TD);

    if (largoReg == 0 or nroReg < 0 or nroReg >= CantidadRegistros(ARCH_MOV_TD))
      return false;

    ifstream arch(ARCH_MOV_TD, ios::in | ios::binary);
    char     linea[LARGO_LINEA];

    if (not arch.is_open())
      return false;
    arch.seekg(nroReg * largoReg, ios::beg);
    arch.getline(linea, LARGO_LINEA);
    arch.close();
    QuitarSaltoDeLinea(linea);
    if (not DesarmarLineaTD(linea, m))
      return false;
    m.nroReg = (short) nroReg;
    return true;
  } // LeerRegistroTD

  // =========================================================================
  //                    TARJETA DE CREDITO  (MovimientosTC.Txt)
  // =========================================================================

  // Mayor numero de comprobante que entra en la columna de ANCHO_CPBTE
  // digitos. Un valor mayor se truncaria en silencio, de modo que se rechaza.
  const long MAX_CPBTE = 99999999L;

  bool ArmarLineaTC(MovTC m, char linea[]) {
    char cpbte[ANCHO_CPBTE + 1];

    if (m.importe < 0 or m.importe > MAX_IMPORTE)
      return false;
    if (m.nroCpbte < 0 or m.nroCpbte > MAX_CPBTE)
      return false;
    // Un comprobante en cero significa <<sin comprobante>>: se deja en blanco.
    if (m.nroCpbte == 0)
      snprintf(cpbte, sizeof cpbte, "%*s", ANCHO_CPBTE, "");
    else
      snprintf(cpbte, sizeof cpbte, "%*ld", ANCHO_CPBTE, m.nroCpbte);

    snprintf(linea, LARGO_LINEA, "%2d-%2d-%4d %-*s %*s %s %*.*f %*d",
             m.fecha.dia, m.fecha.mes, m.fecha.anio,
             ANCHO_COLDET, m.detalle,
             ANCHO_CUOTA, m.cuotas,
             cpbte,
             ANCHO_IMPORTE, DECIMALES, m.importe,
             ANCHO_USUARIO, m.nroUsuario);
    return (short) strlen(linea) == LARGO_REG_TC;
  } // ArmarLineaTC

  bool DesarmarLineaTC(const char linea[], MovTC &m) {
    char campo[ANCHO_COLDET + 1];

    if ((short) strlen(linea) < LARGO_REG_TC)
      return false;
    if (not ExtraerFecha(linea, TC_POS_FECHA, m.fecha))
      return false;
    ExtraerCampo(linea, TC_POS_DETALLE, TC_ANC_DETALLE, campo);
    Validar::CopiarCadena(m.detalle, campo, LARGO_DETALLE);
    ExtraerCampo(linea, TC_POS_CUOTAS, TC_ANC_CUOTAS, campo);
    Validar::CopiarCadena(m.cuotas, campo, LARGO_CUOTAS);
    m.nroCpbte   = ExtraerEntero(linea, TC_POS_CPBTE, TC_ANC_CPBTE);
    m.importe    = ExtraerReal(linea, TC_POS_IMPORTE, TC_ANC_IMPORTE);
    m.nroUsuario = (short) ExtraerEntero(linea, TC_POS_USUARIO, TC_ANC_USUARIO);
    m.nroReg     = 0;
    return m.nroUsuario >= 1 and m.nroUsuario <= MAX_USUARIOS;
  } // DesarmarLineaTC

  short LeerMovimientosTC(MovTC a[], short nroUsuario, short maximo) {
    ifstream arch(ARCH_MOV_TC, ios::in | ios::binary);
    char     linea[LARGO_LINEA];
    MovTC    mov;
    short    cant = 0, nroReg = 0;

    if (not arch.is_open())
      return 0;
    while (cant < maximo and arch.getline(linea, LARGO_LINEA)) {
      QuitarSaltoDeLinea(linea);
      // nroReg lleva la posicion FISICA de la linea dentro del archivo, y por
      // eso avanza tambien cuando la linea no se pudo interpretar. Si solo
      // contara las validas, los numeros de registro quedarian corridos y el
      // acceso aleatorio leeria o actualizaria la componente equivocada.
      if (DesarmarLineaTC(linea, mov) and
          (nroUsuario == 0 or mov.nroUsuario == nroUsuario)) {
        mov.nroReg = nroReg;
        a[cant++] = mov;
      }
      nroReg++;
    }
    arch.close();
    return cant;
  } // LeerMovimientosTC

  // -------------------------------------------------------------------------
  // Acumula el total consumido con la Tarjeta de Credito por el usuario,
  // recorriendo el archivo completo (sin el tope de ningun array). Se usa para
  // controlar el limite de compra de la tarjeta.
  // -------------------------------------------------------------------------
  double AcumularMovimientosTC(short nroUsuario) {
    ifstream arch(ARCH_MOV_TC, ios::in | ios::binary);
    char     linea[LARGO_LINEA];
    MovTC    mov;
    double   total = 0;

    if (not arch.is_open())
      return 0;
    while (arch.getline(linea, LARGO_LINEA)) {
      QuitarSaltoDeLinea(linea);
      if (DesarmarLineaTC(linea, mov) and
          (nroUsuario == 0 or mov.nroUsuario == nroUsuario))
        total += mov.importe;
    }
    arch.close();
    return total;
  } // AcumularMovimientosTC

  bool AgregarMovimientoTC(MovTC m) {
    char linea[LARGO_LINEA];

    if (not ArmarLineaTC(m, linea))
      return false;

    char terminador[3];

    TerminadorDeLinea(ARCH_MOV_TC, terminador);
    AsegurarFinDeLinea(ARCH_MOV_TC, terminador);

    ofstream arch(ARCH_MOV_TC, ios::out | ios::app | ios::binary);
    if (not arch.is_open())
      return false;
    arch << linea << terminador;
    arch.close();
    return true;
  } // AgregarMovimientoTC

  bool LeerRegistroTC(long nroReg, MovTC &m) {
    long largoReg = LargoRegistro(ARCH_MOV_TC);

    if (largoReg == 0 or nroReg < 0 or nroReg >= CantidadRegistros(ARCH_MOV_TC))
      return false;

    ifstream arch(ARCH_MOV_TC, ios::in | ios::binary);
    char     linea[LARGO_LINEA];

    if (not arch.is_open())
      return false;
    arch.seekg(nroReg * largoReg, ios::beg);
    arch.getline(linea, LARGO_LINEA);
    arch.close();
    QuitarSaltoDeLinea(linea);
    if (not DesarmarLineaTC(linea, m))
      return false;
    m.nroReg = (short) nroReg;
    return true;
  } // LeerRegistroTC

  bool ActualizarRegistroTC(long nroReg, MovTC m) {
    long largoReg = LargoRegistro(ARCH_MOV_TC);
    char linea[LARGO_LINEA];

    if (largoReg == 0 or nroReg < 0 or nroReg >= CantidadRegistros(ARCH_MOV_TC))
      return false;
    if (not ArmarLineaTC(m, linea))
      return false;

    fstream arch(ARCH_MOV_TC, ios::in | ios::out | ios::binary);
    if (not arch.is_open())
      return false;
    arch.seekp(nroReg * largoReg, ios::beg);
    arch.write(linea, LARGO_REG_TC);
    arch.close();
    return true;
  } // ActualizarRegistroTC

} // namespace Archivo

#endif // ARCHIVOS_HPP
