// ---------------------------------------------------------------------------
// prototipos.hpp
// Cabeceras de todos los modulos del Home Banking.
//
// Al declararlos todos aqui, cualquier modulo puede invocar a otro sin que
// importe el orden en que estan definidos mas abajo (por ejemplo, el menu de
// Transferencias invoca al de la Agenda de CBU).
//
// JERARQUIA DE MODULOS
// --------------------
//   Nivel 0 : main()
//   Nivel 1 : SistemaHomeBanking()  ->  MenuLogin(), MenuPpal()
//   Nivel 2 : Menu_opc()            ->  una por cada opcion del menu principal
//   Nivel 3 : submenues y pasos internos de cada opcion
// ---------------------------------------------------------------------------
#ifndef PROTOTIPOS_HPP
#define PROTOTIPOS_HPP

// ----- Opciones del menu principal (nivel 1) -----
const short OPC_MIS_CUENTAS   =  1;
const short OPC_MOVIMIENTOS   =  2;
const short OPC_TRANSFERIR    =  3;
const short OPC_DEPOSITO      =  4;
const short OPC_COMPRAS       =  5;
const short OPC_TARJETAS      =  6;
const short OPC_PLAZO_FIJO    =  7;
const short OPC_DOLARES       =  8;
const short OPC_RECARGAS      =  9;
const short OPC_TOKEN         = 10;
const short OPC_CBU           = 11;
const short OPC_AGENDA        = 12;
const short OPC_DATOS_PERS    = 13;
const short OPC_MODIF_CLAVE   = 14;
const short OPC_NUEVA_CUENTA  = 15;
const short OPC_LISTADOS      = 16;
const short OPC_LOGOUT        = 17;
const short CANT_OPC_PPAL     = 17;

// ---------------------------------------------------------------------------
// Nivel 1: sistema, login y menu principal
// ---------------------------------------------------------------------------
void  SistemaHomeBanking();
short MenuLogin(DatosPers aDatosPers[], short &usuario);
short MenuPpal(DatosPers aDatosPers[], short nroUsu);

// ---------------------------------------------------------------------------
// Nivel 2: una funcion por cada opcion del menu principal
// ---------------------------------------------------------------------------
void Menu_MisCuentas(DatosPers aDatosPers[], short nroUsu);
void Menu_Movimientos(DatosPers aDatosPers[], short nroUsu);
void Menu_Transferir(DatosPers aDatosPers[], short nroUsu);
void Menu_Deposito(DatosPers aDatosPers[], short nroUsu);
void Menu_Compras(DatosPers aDatosPers[], short nroUsu);
void Menu_Tarjetas(DatosPers aDatosPers[], short nroUsu);
void Menu_PlazoFijo(DatosPers aDatosPers[], short nroUsu);
void Menu_CompraVentaDolares(DatosPers aDatosPers[], short nroUsu);
void Menu_Recargar(DatosPers aDatosPers[], short nroUsu);
void Menu_GenToken(DatosPers aDatosPers[], short nroUsu);
void Menu_MostrarCBU(DatosPers aDatosPers[], short nroUsu);
void Menu_AgendaCBU(DatosPers aDatosPers[], short nroUsu);
void Menu_DatosPers(DatosPers aDatosPers[], short nroUsu);
void Menu_ModifClave(DatosPers aDatosPers[], short nroUsu);
void Menu_NuevaCuenta(DatosPers aDatosPers[], short nroUsu);
void Menu_Listados(DatosPers aDatosPers[], short nroUsu);
bool Menu_Logout(DatosPers aDatosPers[], short nroUsu);

// ---------------------------------------------------------------------------
// Nivel 3: submenues y operaciones internas
// ---------------------------------------------------------------------------
void Menu_MovimientosCA(DatosPers aDatosPers[], short nroUsu);
void Menu_MovimientosTD(DatosPers aDatosPers[], short nroUsu);
void Menu_MovimientosTC(DatosPers aDatosPers[], short nroUsu);
void Menu_BuscarMovimientoCA(DatosPers aDatosPers[], short nroUsu);
void Menu_EditarDetalleCA(DatosPers aDatosPers[], short nroUsu);
void Menu_PagarCuotaTC(DatosPers aDatosPers[], short nroUsu);
void Menu_TransferirCBU(DatosPers aDatosPers[], short nroUsu);
void Menu_TransferirCelular(DatosPers aDatosPers[], short nroUsu);
void Menu_AgendarCBU(DatosPers aDatosPers[], short nroUsu);
void Menu_BuscarAlias(DatosPers aDatosPers[], short nroUsu);
void Menu_EliminarCBU(DatosPers aDatosPers[], short nroUsu);
void Menu_SimulacionPF(DatosPers aDatosPers[], short nroUsu);
void Menu_InversionPF(DatosPers aDatosPers[], short nroUsu);
void Menu_FondoInversion(DatosPers aDatosPers[], short nroUsu);
void Menu_RecargaCelular(DatosPers aDatosPers[], short nroUsu);
void Menu_RecargaSube(DatosPers aDatosPers[], short nroUsu);
void Menu_CuentasEnPesos(DatosPers aDatosPers[], short nroUsu);
void Menu_CuentasEnDolares(DatosPers aDatosPers[], short nroUsu);
void Menu_ComprarDolares(DatosPers aDatosPers[], short nroUsu);
void Menu_VenderDolares(DatosPers aDatosPers[], short nroUsu);
void Menu_ResumenTarjetas(DatosPers aDatosPers[], short nroUsu);

#endif // PROTOTIPOS_HPP
