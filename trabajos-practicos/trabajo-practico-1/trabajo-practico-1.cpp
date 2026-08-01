// ===========================================================================
// Nombre del programa : TP1V1_K____G__ - Home Banking Mesopotamia
// Trabajo Practico    : TP #1 - Metodologia-DP-ECP-Modulos-ED:
//                       Registro, Arreglo y Archivo de texto
// Version             : V1
// Fecha de entrega    : <<COMPLETAR>>
// Materia             : Algoritmos y Estructuras de Datos - UTN
// Curso / Dia / Turno : <<COMPLETAR>>
// Nro. de grupo       : <<COMPLETAR>>
// Integrantes         : <<Nro.Legajo - Apellido, Nombre>>
//                       <<Nro.Legajo - Apellido, Nombre>>
//                       <<Nro.Legajo - Apellido, Nombre>>
//
// Objetivo
// --------
// Simular un sitio de Home Banking de consola. El sistema registra
// internamente CINCO usuarios (array de registros), valida el ingreso con DNI,
// usuario y clave, y ofrece un menu de nivel 1 desde el cual se despachan, por
// seleccion multiple, los modulos de nivel 2: consulta de cuentas y
// movimientos, transferencias a CBU/alias y a celular, depositos, compras con
// tarjeta de debito o credito, simulacion e inversion en plazo fijo, fondo
// comun de inversion, compra y venta de dolares, recargas de celular y SUBE,
// generacion de token, agenda de CBU, modificacion de la clave, creacion de
// cuentas nuevas y emision de listados.
//
// Los movimientos se conservan en tres archivos de texto de formato tabular y
// registros de largo fijo:
//     MovimientosCA.Txt  - Caja de Ahorro     (58 bytes + salto de linea)
//     MovimientosTD.Txt  - Tarjeta de Debito  (56 bytes + salto de linea)
//     MovimientosTC.Txt  - Tarjeta de Credito (71 bytes + salto de linea)
// y la salida de los listados se dirige con freopen() a  ListadosHB.Txt
//
// Paradigma: imperativo procedural, programacion estructurada y modular.
// Memoria dinamica utilizada: 0 bytes (solo arrays y registros estaticos).
//
// Compiladores verificados
// ------------------------
//   Windows : GNU-GCC TDM-GCC-64  x86_64-w64-mingw32-g++.exe  (Code::Blocks)
//   macOS   : clang++ -std=c++17
//
// Organizacion del codigo fuente
// ------------------------------
//   trabajo-practico-1.cpp   bloque principal (este archivo)
//   utilidades/              constantes, tipos, consola, fechas, validaciones,
//                            ordenamiento, archivos, muestra de datos y
//                            operaciones comunes
//   modulos/                 un archivo por cada modulo Menu_opc()
//   lib/lib.cpp              ModulosHB.cpp original de la catedra (referencia)
// ===========================================================================

// ----- Utilidades -----
#include "utilidades/compatibilidad.hpp"
#include "utilidades/constantes.hpp"
#include "utilidades/tipos.hpp"
#include "utilidades/prototipos.hpp"
#include "utilidades/consola.hpp"
#include "utilidades/fechas.hpp"
#include "utilidades/validaciones.hpp"
#include "utilidades/ordenamiento.hpp"
#include "utilidades/archivos.hpp"
#include "utilidades/datos.hpp"
#include "utilidades/operaciones.hpp"

// ----- Modulos del Home Banking -----
#include "modulos/MenuLogin.hpp"
#include "modulos/MenuPpal.hpp"
#include "modulos/Menu_MisCuentas.hpp"
#include "modulos/Menu_Movimientos.hpp"
#include "modulos/Menu_ActualizarMov.hpp"
#include "modulos/Menu_Transferir.hpp"
#include "modulos/Menu_Deposito.hpp"
#include "modulos/Menu_Compras.hpp"
#include "modulos/Menu_Tarjetas.hpp"
#include "modulos/Menu_PlazoFijo.hpp"
#include "modulos/Menu_CompraVentaDolares.hpp"
#include "modulos/Menu_Recargar.hpp"
#include "modulos/Menu_GenToken.hpp"
#include "modulos/Menu_MostrarCBU.hpp"
#include "modulos/Menu_AgendaCBU.hpp"
#include "modulos/Menu_DatosPers.hpp"
#include "modulos/Menu_ModifClave.hpp"
#include "modulos/Menu_NuevaCuenta.hpp"
#include "modulos/Menu_Listados.hpp"
#include "modulos/Menu_Logout.hpp"
#include "modulos/SistemaHomeBanking.hpp"

// ===========================================================================
//                             BLOQUE PRINCIPAL
//    Solo contiene la invocacion al modulo de nivel 1 del sistema.
// ===========================================================================
int main() {
  SistemaHomeBanking();
  return 0;
} // main
