// ---------------------------------------------------------------------------
// ordenamiento.hpp
// Operaciones clasicas sobre los arrays del sistema.
//
//   * Ordenamiento por el metodo de la BURBUJA OPTIMIZADA: el ciclo externo es
//     1-x (do..while) y termina en cuanto una pasada no produce intercambios;
//     ademas el ciclo interno reduce su limite en cada pasada, porque la parte
//     final del array ya quedo ordenada. Se evitan asi las pasadas inutiles.
//   * BUSQUEDA BINARIA sobre el array de CBU agendados, que se mantiene
//     ordenado por alias.
//   * BUSQUEDA SECUENCIAL con corte cuando la clave no esta ordenada (DNI,
//     CBU, numero de celular, numero de cuenta).
//   * INSERCION EN ORDEN, que ubica el nuevo contacto en el lugar que le
//     corresponde desplazando los siguientes, de modo que el array nunca deja
//     de estar ordenado y la busqueda binaria siempre es aplicable.
// ---------------------------------------------------------------------------
#ifndef ORDENAMIENTO_HPP
#define ORDENAMIENTO_HPP

namespace Ordenar {

  // =========================================================================
  //                     ORDENAMIENTO: BURBUJA OPTIMIZADA
  // =========================================================================

  // -------------------------------------------------------------------------
  // Ordena los movimientos de Caja de Ahorro por fecha en forma ascendente.
  // -------------------------------------------------------------------------
  void MovCAporFecha(MovCA a[], short cant) {
    MovCA aux;
    short limite = (short)(cant - 1);
    bool  huboCambio;

    if (cant < 2)
      return;
    do {
      huboCambio = false;
      for (short i = 0; i < limite; i++)
        if (FechaHora::FechaLarga(a[i].fecha) >
            FechaHora::FechaLarga(a[i + 1].fecha)) {
          aux      = a[i];
          a[i]     = a[i + 1];
          a[i + 1] = aux;
          huboCambio = true;
        }
      limite--;
    } while (huboCambio and limite > 0);
  } // MovCAporFecha

  void MovTDporFecha(MovTD a[], short cant) {
    MovTD aux;
    short limite = (short)(cant - 1);
    bool  huboCambio;

    if (cant < 2)
      return;
    do {
      huboCambio = false;
      for (short i = 0; i < limite; i++)
        if (FechaHora::FechaLarga(a[i].fecha) >
            FechaHora::FechaLarga(a[i + 1].fecha)) {
          aux      = a[i];
          a[i]     = a[i + 1];
          a[i + 1] = aux;
          huboCambio = true;
        }
      limite--;
    } while (huboCambio and limite > 0);
  } // MovTDporFecha

  void MovTCporFecha(MovTC a[], short cant) {
    MovTC aux;
    short limite = (short)(cant - 1);
    bool  huboCambio;

    if (cant < 2)
      return;
    do {
      huboCambio = false;
      for (short i = 0; i < limite; i++)
        if (FechaHora::FechaLarga(a[i].fecha) >
            FechaHora::FechaLarga(a[i + 1].fecha)) {
          aux      = a[i];
          a[i]     = a[i + 1];
          a[i + 1] = aux;
          huboCambio = true;
        }
      limite--;
    } while (huboCambio and limite > 0);
  } // MovTCporFecha

  // -------------------------------------------------------------------------
  // Ordena los movimientos de Tarjeta de Debito por importe DESCENDENTE, para
  // el listado de los mayores consumos.
  // -------------------------------------------------------------------------
  void MovTDporImporteDesc(MovTD a[], short cant) {
    MovTD aux;
    short limite = (short)(cant - 1);
    bool  huboCambio;

    if (cant < 2)
      return;
    do {
      huboCambio = false;
      for (short i = 0; i < limite; i++)
        if (a[i].importe < a[i + 1].importe) {
          aux      = a[i];
          a[i]     = a[i + 1];
          a[i + 1] = aux;
          huboCambio = true;
        }
      limite--;
    } while (huboCambio and limite > 0);
  } // MovTDporImporteDesc

  // -------------------------------------------------------------------------
  // Ordena la agenda de CBU por alias en forma ascendente.
  // -------------------------------------------------------------------------
  void AgendaPorAlias(Contacto a[], short cant) {
    Contacto aux;
    short    limite = (short)(cant - 1);
    bool     huboCambio;

    if (cant < 2)
      return;
    do {
      huboCambio = false;
      for (short i = 0; i < limite; i++)
        if (strcmp(a[i].alias, a[i + 1].alias) > 0) {
          aux      = a[i];
          a[i]     = a[i + 1];
          a[i + 1] = aux;
          huboCambio = true;
        }
      limite--;
    } while (huboCambio and limite > 0);
  } // AgendaPorAlias

  // =========================================================================
  //                              BUSQUEDAS
  // =========================================================================

  // -------------------------------------------------------------------------
  // BUSQUEDA BINARIA del alias en la agenda (que esta ordenada por alias).
  // Devuelve la posicion o NO_ENCONTRADO.
  // -------------------------------------------------------------------------
  short BinariaAlias(Contacto a[], short cant, const char alias[]) {
    short primero = 0, ultimo = (short)(cant - 1), medio, comparacion;

    while (primero <= ultimo) {
      medio = (short)((primero + ultimo) / 2);
      comparacion = (short) strcmp(alias, a[medio].alias);
      if (comparacion == 0)
        return medio;
      if (comparacion < 0)
        ultimo = (short)(medio - 1);
      else
        primero = (short)(medio + 1);
    }
    return NO_ENCONTRADO;
  } // BinariaAlias

  // -------------------------------------------------------------------------
  // BUSQUEDA SECUENCIAL del CBU en la agenda. El array no esta ordenado por
  // CBU, de modo que corresponde el recorrido secuencial con corte.
  // -------------------------------------------------------------------------
  short SecuencialCBU(Contacto a[], short cant, const char cbu[]) {
    short i = 0;

    while (i < cant and strcmp(a[i].cbu, cbu) != 0)
      i++;
    return i < cant ? i : NO_ENCONTRADO;
  } // SecuencialCBU

  short SecuencialCelular(Contacto a[], short cant, const char celular[]) {
    short i = 0;

    while (i < cant and strcmp(a[i].celular, celular) != 0)
      i++;
    return i < cant ? i : NO_ENCONTRADO;
  } // SecuencialCelular

  // -------------------------------------------------------------------------
  // BUSQUEDA SECUENCIAL del usuario por DNI dentro del array de usuarios.
  // -------------------------------------------------------------------------
  short SecuencialDni(DatosPers a[], short cant, long dni) {
    short i = 0;

    while (i < cant and a[i].dni != dni)
      i++;
    return i < cant ? i : NO_ENCONTRADO;
  } // SecuencialDni

  // -------------------------------------------------------------------------
  // BUSQUEDA SECUENCIAL de la cuenta por numero, dentro de un usuario.
  // -------------------------------------------------------------------------
  short SecuencialCuenta(Cuenta a[], short cant, const char nroCta[]) {
    short i = 0;

    while (i < cant and strcmp(a[i].nroCta, nroCta) != 0)
      i++;
    return i < cant ? i : NO_ENCONTRADO;
  } // SecuencialCuenta

  // -------------------------------------------------------------------------
  // BUSQUEDA SECUENCIAL de la primera cuenta que coincide con el tipo y la
  // moneda pedidos ("CA"/"CC" y "$"/"u$s").
  // -------------------------------------------------------------------------
  short SecuencialCuentaPorTipo(Cuenta a[], short cant, const char tipo[],
                               const char moneda[]) {
    short i = 0;

    while (i < cant and (strcmp(a[i].tipo, tipo) != 0 or
                         strcmp(a[i].moneda, moneda) != 0))
      i++;
    return i < cant ? i : NO_ENCONTRADO;
  } // SecuencialCuentaPorTipo

  // =========================================================================
  //                          INSERCION EN ORDEN
  // =========================================================================

  // -------------------------------------------------------------------------
  // Inserta el contacto en la agenda manteniendola ordenada por alias.
  // Devuelve false si la agenda esta completa o si el alias ya existe.
  // -------------------------------------------------------------------------
  bool InsertarContactoEnOrden(Contacto a[], short &cant, Contacto nuevo) {
    if (cant >= MAX_AGENDA)
      return false;
    if (BinariaAlias(a, cant, nuevo.alias) != NO_ENCONTRADO)
      return false;

    short pos = cant;               // posicion donde debe quedar el nuevo

    while (pos > 0 and strcmp(a[pos - 1].alias, nuevo.alias) > 0) {
      a[pos] = a[pos - 1];          // desplaza para hacerle lugar
      pos--;
    }
    a[pos] = nuevo;
    cant++;
    return true;
  } // InsertarContactoEnOrden

  // -------------------------------------------------------------------------
  // Elimina el contacto de la posicion indicada, compactando el array para que
  // no queden huecos y siga ordenado.
  // -------------------------------------------------------------------------
  bool EliminarContacto(Contacto a[], short &cant, short pos) {
    if (pos < 0 or pos >= cant)
      return false;
    for (short i = pos; i < cant - 1; i++)
      a[i] = a[i + 1];
    cant--;
    return true;
  } // EliminarContacto

} // namespace Ordenar

#endif // ORDENAMIENTO_HPP
