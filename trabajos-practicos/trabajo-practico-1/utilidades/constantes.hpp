// ---------------------------------------------------------------------------
// constantes.hpp
// Constantes con nombre de todo el sistema: dimensiones de las estructuras de
// datos, largos de cadenas, anchos de columna de los archivos de texto,
// colores de consola y parametros del negocio bancario.
//
// No se utilizan literales numericos sueltos en el resto del programa: toda
// cantidad se expresa con una de estas constantes.
// ---------------------------------------------------------------------------
#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

// La catedra escribe los registros con la palabra reservada <<record>>
// (dialecto de los apuntes del prof. Hugo Cuello). Se conserva la convencion
// para que el codigo sea consistente con el material de la materia.
#define record struct

// ---------------------------------------------------------------------------
// Dimensiones de las estructuras de datos (arrays)
// ---------------------------------------------------------------------------
const short MAX_USUARIOS  =   5; // usuarios registrados en el sistema
const short MAX_CUENTAS   =   8; // cuentas por usuario (CA/CC en $ y u$s)
const short MAX_AGENDA    =  12; // CBU agendados por usuario
const short MAX_MOV_CA    = 300; // movimientos de Caja de Ahorro en memoria
const short MAX_MOV_TD    = 150; // movimientos de Tarjeta de Debito
const short MAX_MOV_TC    = 150; // movimientos de Tarjeta de Credito
const short MIN_REG_ARCH  =  15; // minimo de registros por archivo (enunciado)

// ---------------------------------------------------------------------------
// Largos de las cadenas (sin contar el terminador nulo)
// ---------------------------------------------------------------------------
const short LARGO_CUOTAS  =  5; // "12/18"
const short LARGO_CLAVE   = 15;
const short LARGO_USUARIO = 15;
const short LARGO_CELULAR = 15;
const short LARGO_DETALLE = 25; // str25 exigido por el enunciado
const short LARGO_CBU     = 22; // el CBU tiene exactamente 22 digitos
const short LARGO_NOMBRE  = 40;
const short LARGO_CTA     = 20; // "106-4235689707-3"
const short LARGO_OPCION  = 40; // texto de una opcion de menu

// ---------------------------------------------------------------------------
// Formato tabular de los archivos de texto
// Los enteros y reales se ajustan a la derecha; las cadenas a la izquierda,
// rellenando con espacios en blanco. Cada registro ocupa SIEMPRE la misma
// cantidad de bytes, lo que habilita el acceso aleatorio por numero de
// registro (seekg / seekp).
// ---------------------------------------------------------------------------
const short ANCHO_FECHA   = 10; // 99-99-9999
const short ANCHO_TIPOMOV =  1; // 'D' | 'H'
const short ANCHO_COLDET  = 30; // X(30)
const short ANCHO_IMPORTE = 12; // 9(7).99 ajustado a derecha
const short ANCHO_CUOTA   =  5; // 99/99
const short ANCHO_CPBTE   =  8; // numero de comprobante
const short ANCHO_USUARIO =  1; // numero de usuario duenio del movimiento
const short DECIMALES     =  2;

// Largo en bytes de cada registro, SIN el salto de linea:
//   CA: fecha(10) + ' ' + tipo(1) + ' ' + detalle(30) + ' ' + importe(12) +
//       ' ' + usuario(1)                                             = 58
//   TD: fecha(10) + ' ' + detalle(30) + ' ' + importe(12) + ' ' +
//       usuario(1)                                                   = 56
//   TC: fecha(10) + ' ' + detalle(30) + ' ' + cuotas(5) + ' ' +
//       cpbte(8) + ' ' + importe(12) + ' ' + usuario(1)               = 71
const short LARGO_REG_CA  = 58;
const short LARGO_REG_TD  = 56;
const short LARGO_REG_TC  = 71;

// ---------------------------------------------------------------------------
// Nombres de los archivos de datos y del archivo de salida
// ---------------------------------------------------------------------------
const char ARCH_MOV_CA[]   = "MovimientosCA.Txt";
const char ARCH_MOV_TD[]   = "MovimientosTD.Txt";
const char ARCH_MOV_TC[]   = "MovimientosTC.Txt";
const char ARCH_LISTADOS[] = "ListadosHB.Txt";

// ---------------------------------------------------------------------------
// Colores de la consola (codigos de 4 bits de la API de Windows).
// Se define la paleta completa aunque el sistema no use todos los colores:
// son los 16 valores que admiten las funciones de consola de la catedra.
// ---------------------------------------------------------------------------
const short NEGRO         =  0;
const short AZUL          =  1;
const short VERDE         =  2;
const short CIAN          =  3;
const short ROJO          =  4;
const short MAGENTA       =  5;
const short MARRON        =  6;
const short GRIS_CLARO    =  7;
const short GRIS_OSCURO   =  8;
const short AZUL_CLARO    =  9;
const short VERDE_CLARO   = 10;
const short CIAN_CLARO    = 11;
const short ROJO_CLARO    = 12;
const short MAGENTA_CLARO = 13;
const short AMARILLO      = 14;
const short BLANCO        = 15;

// Colores asignados a cada rol de la interfaz
const short COL_TITULO    = CIAN_CLARO;
const short COL_ROTULO    = AMARILLO;
const short COL_DATO      = BLANCO;
const short COL_MARCO     = AZUL_CLARO;
const short COL_ERROR     = ROJO_CLARO;
const short COL_EXITO     = VERDE_CLARO;
const short COL_AVISO     = MARRON;
const short COL_MENU      = GRIS_CLARO;

// ---------------------------------------------------------------------------
// Geometria de la pantalla
// ---------------------------------------------------------------------------
const short ANCHO_PANT    = 80;
const short ALTO_PANT     = 25;

// ---------------------------------------------------------------------------
// Tipos de movimiento de la Caja de Ahorro
// En el formato de la catedra la columna 'D' (Debe) INCREMENTA el saldo del
// titular (acreditaciones) y la columna 'H' (Haber) lo DISMINUYE (extracciones
// y pagos), tal como figura en el ejemplo del enunciado.
// ---------------------------------------------------------------------------
const char MOV_ACREDITA = 'D';
const char MOV_DEBITA   = 'H';

// ---------------------------------------------------------------------------
// Parametros del negocio bancario
//
// Los importes se manejan en <<double>> y no en <<float>>. El formato de los
// archivos es 9(7).99, o sea hasta NUEVE digitos significativos, y un float
// solo garantiza SIETE: un importe como 1542367.21 se guardaria como
// 1542367.25, y el error se arrastraria en cada acumulacion de saldos. El
// double garantiza quince digitos significativos, con lo que todos los
// importes del sistema se representan y se acumulan en forma exacta.
// ---------------------------------------------------------------------------
const double TASA_PF_ANUAL      =   37.50; // TNA del plazo fijo tradicional
const double TASA_PF_UVA        =   28.00; // TNA del plazo fijo UVA
const double TASA_FCI_ANUAL     =   32.75; // TNA estimada del fondo comun
const double COTIZ_DOLAR_COMPRA = 1010.50; // el banco vende a este precio
const double COTIZ_DOLAR_VENTA  =  975.25; // el banco compra a este precio
const double MIN_PLAZO_FIJO     = 10000.00;
const double MIN_FONDO_INV      =  5000.00;
const double TOPE_SIN_TOKEN     = 100000.00; // arriba de este monto pide token
const double TOPE_TRANSF_DIARIO = 5000000.00;
const double MIN_RECARGA_CEL    =    500.00;
const double MAX_RECARGA_CEL    =  50000.00;
const double MIN_RECARGA_SUBE   =    300.00;
const double MAX_RECARGA_SUBE   =  30000.00;
const short DIAS_PF_MINIMO     =  30;
const short DIAS_PF_MAXIMO     = 365;
const short DIAS_DEL_ANIO      = 365;
const short MAX_CUOTAS_TC      =  18;
const double LIMITE_TARJ_CREDITO = 1500000.00; // limite de compra de la TC

// Token de seguridad: 6 digitos
const long  TOKEN_MINIMO  = 100000;
const long  TOKEN_MAXIMO  = 999999;
const short INTENTOS_MAX  = 3; // intentos de login y de validacion de token

// Rango de anios aceptado al validar una fecha ingresada
const short ANIO_MINIMO = 1900;
const short ANIO_MAXIMO = 2100;

// Valor devuelto cuando una busqueda no encuentra el elemento buscado
const short NO_ENCONTRADO = -1;

#endif // CONSTANTES_HPP
