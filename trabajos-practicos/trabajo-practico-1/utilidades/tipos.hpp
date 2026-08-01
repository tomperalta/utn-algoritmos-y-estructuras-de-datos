// ---------------------------------------------------------------------------
// tipos.hpp
// Definicion de las estructuras de datos del sistema.
//
// Se utilizan UNICAMENTE arrays estaticos y registros: no se reserva memoria
// dinamica en ningun punto del programa (restriccion del enunciado: espacio en
// memoria dinamica = 0 bytes). Las cadenas son arrays de char de largo fijo.
//
// Estructuras combinadas empleadas:
//   * array de registros            -> aDatosPers[MAX_USUARIOS]
//   * campo de registro que es array de registros -> DatosPers.aCuentas
//                                                    DatosPers.aAgenda
//   * registro dentro de registro   -> DatosPers.fechaNac (Fecha)
// ---------------------------------------------------------------------------
#ifndef TIPOS_HPP
#define TIPOS_HPP

// ---------------------------------------------------------------------------
// Tipos cadena (el +1 reserva el lugar del terminador nulo)
// ---------------------------------------------------------------------------
typedef char str3[3 + 1];                   // "CA" | "CC"
typedef char str4[3 + 1];                   // "$"  | "u$s"
typedef char str5[LARGO_CUOTAS + 1];        // "12/18"
typedef char str15[LARGO_USUARIO + 1];      // usuario, clave, celular
typedef char str20[LARGO_CTA + 1];          // numero de cuenta
typedef char str22[LARGO_CBU + 1];          // CBU de 22 digitos
typedef char str25[LARGO_DETALLE + 1];      // detalle de un movimiento
typedef char str40[LARGO_NOMBRE + 1];       // nombre, e-mail, domicilio
typedef char str40Opc[LARGO_OPCION + 1];    // texto de una opcion de menu

// ---------------------------------------------------------------------------
// Fecha
//   dia  : 1..31        2 bytes
//   mes  : 1..12        2 bytes
//   anio : 1900..2100   2 bytes
//                       -------
//                       6 bytes
// ---------------------------------------------------------------------------
record Fecha {
  short dia;
  short mes;
  short anio;
};

// ---------------------------------------------------------------------------
// Cuenta bancaria del usuario                        56 bytes en total
//   nroCta      : "999-9999999999-9"        21 bytes
//   tipo        : "CA" | "CC"                4 bytes
//   moneda      : "$"  | "u$s"               4 bytes
//   (relleno de alineacion)                  3 bytes
//   saldoInicial: saldo previo a los         8 bytes
//                 movimientos del archivo
//   saldo       : saldo actual               8 bytes
//   estado      : 'A' activa | 'I' inactiva  1 byte
//   (relleno de alineacion)                  7 bytes
//                                           --------
//                                           56 bytes
//
// Para la Caja de Ahorro en pesos (cuenta principal) el saldo NO se almacena:
// se recalcula como saldoInicial + suma de los movimientos leidos del archivo
// MovimientosCA.Txt, de modo que archivo y pantalla nunca queden desfasados.
// ---------------------------------------------------------------------------
record Cuenta {
  str20  nroCta;
  str3   tipo;
  str4   moneda;
  double saldoInicial;
  double saldo;
  char   estado;
};

// ---------------------------------------------------------------------------
// Cuenta agendada para transferir dinero            132 bytes en total
//   alias          : alias del CBU        26 bytes
//   apellidoNombre : titular              41 bytes
//   cbu            : 22 digitos           23 bytes
//   celular        : para transferir      16 bytes
//   banco          : banco de destino     26 bytes
//                                        ----------
//                                        132 bytes
// ---------------------------------------------------------------------------
record Contacto {
  str25 alias;
  str40 apellidoNombre;
  str22 cbu;
  str15 celular;
  str25 banco;
};

// ---------------------------------------------------------------------------
// Datos personales de un usuario (registro principal del sistema)
//                                              2272 bytes en total
//   dni            : numero de documento          8 bytes (long)
//   apellidoNombre :                             41 bytes
//   fechaNac       : registro Fecha               6 bytes
//   usuario        :                             16 bytes
//   clave          :                             16 bytes
//   celular        :                             16 bytes
//   email          :                             41 bytes
//   domicilio      :                             41 bytes
//   nroCtaCA       : cuenta principal            21 bytes
//   cbu            : CBU de la cuenta principal  23 bytes
//   aCuentas       : 8 registros Cuenta         448 bytes  (8 x 56)
//   cantCuentas    : ocupacion de aCuentas        2 bytes
//   aAgenda        : 12 registros Contacto      1584 bytes (12 x 132)
//   cantAgenda     : ocupacion de aAgenda         2 bytes
//   nroUsuario     : 1..5, identifica al usuario  2 bytes
//                    dentro de los archivos
//   (relleno de alineacion)                       5 bytes
//                                             ------------
//                                             2272 bytes
//
// El array del sistema es  DatosPers aDatosPers[MAX_USUARIOS]
//                          = 5 x 2272 = 11360 bytes.
// ---------------------------------------------------------------------------
record DatosPers {
  long     dni;
  str40    apellidoNombre;
  Fecha    fechaNac;
  str15    usuario;
  str15    clave;
  str15    celular;
  str40    email;
  str40    domicilio;
  str20    nroCtaCA;
  str22    cbu;
  Cuenta   aCuentas[MAX_CUENTAS];
  short    cantCuentas;
  Contacto aAgenda[MAX_AGENDA];
  short    cantAgenda;
  short    nroUsuario;
};

// ---------------------------------------------------------------------------
// Registro del archivo MovimientosCA.Txt
//   En el ARCHIVO ocupa 58 bytes + el salto de linea (registro de largo fijo).
//   En MEMORIA el registro ocupa 72 bytes, porque incluye dos campos derivados
//   y el compilador alinea los double.
//
//   fecha      : dia, mes, anio                     10 columnas
//   tipoMov    : 'D' acredita | 'H' debita           1 columna
//   detalle    : str25 escrito en columna de 30     30 columnas
//   importe    : 9(7).99 ajustado a derecha         12 columnas
//   nroUsuario : 1..5                                1 columna
//
// Campos derivados (NO se graban, se calculan al leer):
//   saldo  : saldo acumulado hasta este movimiento
//   nroReg : posicion del registro dentro del archivo (base 0), usada para el
//            acceso aleatorio y para actualizar la componente.
// ---------------------------------------------------------------------------
record MovCA {
  Fecha  fecha;
  char   tipoMov;
  str25  detalle;
  double importe;
  short  nroUsuario;
  double saldo;
  short  nroReg;
};

// ---------------------------------------------------------------------------
// Registro del archivo MovimientosTD.Txt
//   En el ARCHIVO ocupa 56 bytes + el salto de linea; en MEMORIA, 48 bytes.
//   fecha(10) + detalle en columna de 30 + importe 9(7).99 + nroUsuario
// ---------------------------------------------------------------------------
record MovTD {
  Fecha  fecha;
  str25  detalle;
  double importe;
  short  nroUsuario;
  short  nroReg;
};

// ---------------------------------------------------------------------------
// Registro del archivo MovimientosTC.Txt
//   En el ARCHIVO ocupa 71 bytes + el salto de linea; en MEMORIA, 64 bytes.
//   fecha(10) + detalle en columna de 30 + cuotas "x/y" (5) + nroCpbte (8)
//   + importe 9(7).99 (12) + nroUsuario (1)
// ---------------------------------------------------------------------------
record MovTC {
  Fecha  fecha;
  str25  detalle;
  str5   cuotas;
  long   nroCpbte;
  double importe;
  short  nroUsuario;
  short  nroReg;
};

#endif // TIPOS_HPP
