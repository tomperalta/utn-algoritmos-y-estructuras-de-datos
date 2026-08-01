// ---------------------------------------------------------------------------
// MenuPpal.hpp
// Nivel 1 - Menu principal del Home Banking.
//
// Presenta las operaciones y consultas disponibles y devuelve la opcion
// elegida. SistemaHomeBanking() usa ese valor para despachar, por seleccion
// multiple, el modulo de nivel 2 correspondiente.
//
// El menu se construye con un ciclo 1-x: se repite hasta obtener una opcion
// valida.
// ---------------------------------------------------------------------------
#ifndef MENUPPAL_HPP
#define MENUPPAL_HPP

// ---------------------------------------------------------------------------
// Devuelve la opcion elegida (una de las constantes OPC_*). Si el usuario
// cancela con ESCAPE o se agota la entrada, devuelve OPC_LOGOUT.
// ---------------------------------------------------------------------------
short MenuPpal(DatosPers aDatosPers[], short nroUsu) {
  str40Opc aMenu[CANT_OPC_PPAL] = {
    "Mis Cuentas",
    "Movimientos de cuentas y tarjetas",
    "Transferir dinero",
    "Deposito",
    "Compras",
    "Tarjetas",
    "Plazo Fijo e Inversiones",
    "Compra / Venta de dolares",
    "Recargas",
    "Generar Token",
    "Mostrar mi CBU",
    "Agenda de CBU",
    "Datos Personales",
    "Modificar Contrasenia",
    "Crear nueva cuenta",
    "Emitir listados a ListadosHB.Txt",
    "Cerrar sesion (Logout)"
  };
  char  rotulo[100];
  short linMenu, opcion;

  linMenu = Operacion::PantallaMenu("MENU PRINCIPAL - HOME BANKING",
                                    aDatosPers, nroUsu);
  snprintf(rotulo, sizeof rotulo, "Saldo Caja de Ahorro $ %.2f",
           Operacion::SaldoCA(aDatosPers, nroUsu));
  Screen::MnsgBox(Operacion::COL_IZQ, linMenu - 1, COL_EXITO, rotulo);

  opcion = Screen::MenuNavegar(aMenu, CANT_OPC_PPAL, linMenu,
                               Operacion::COL_IZQ);
  if (opcion == NO_ENCONTRADO)
    return OPC_LOGOUT;
  return (short)(opcion + 1);
} // MenuPpal

#endif // MENUPPAL_HPP
