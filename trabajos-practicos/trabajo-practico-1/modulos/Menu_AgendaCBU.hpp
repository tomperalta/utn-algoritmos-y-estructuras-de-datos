// ---------------------------------------------------------------------------
// Menu_AgendaCBU.hpp
// Nivel 2 - Agenda de cuentas (CBU) del usuario.
//
// Cada usuario tiene su propia agenda: aDatosPers[nroUsu].aAgenda, un array de
// MAX_AGENDA registros Contacto con su ocupacion en cantAgenda (campo de un
// registro que es, a su vez, array de registros).
//
// INVARIANTE DEL MODULO
// ---------------------
// La agenda se conserva SIEMPRE ordenada por alias:
//
//   * el alta usa Ordenar::InsertarContactoEnOrden(), que ubica el contacto en
//     el lugar que le corresponde desplazando los siguientes;
//   * la baja usa Ordenar::EliminarContacto(), que compacta el array sin dejar
//     huecos y sin alterar el orden relativo.
//
// Mantener ese invariante es lo que permite buscar por alias con BUSQUEDA
// BINARIA (log2 n comparaciones). Las claves que NO son de ordenamiento -el
// CBU y el numero de celular- se buscan en forma SECUENCIAL.
//
// Operaciones del submenu: listar, agendar, buscar por alias y eliminar.
// ---------------------------------------------------------------------------
#ifndef MENU_AGENDACBU_HPP
#define MENU_AGENDACBU_HPP

// ----- Parametros propios del modulo -----
const short AGENDA_CANT_OPC = 5;  // opciones del submenu
const short AGENDA_CANCELA  = 0;  // numero que cancela la eliminacion

// ---------------------------------------------------------------------------
// Muestra con rotulos todos los datos de un contacto de la agenda.
// ---------------------------------------------------------------------------
static void AgendaMostrarContacto(Contacto con) {
  cout << "  Alias............: " << con.alias << endl;
  cout << "  Titular..........: " << con.apellidoNombre << endl;
  cout << "  CBU..............: " << con.cbu << endl;
  cout << "  Celular..........: "
       << (con.celular[0] == '\0' ? "(sin dato)" : con.celular) << endl;
  cout << "  Banco............: " << con.banco << endl;
} // AgendaMostrarContacto

// ---------------------------------------------------------------------------
// Nivel 3 - Alta de una cuenta en la agenda.
// ---------------------------------------------------------------------------
void Menu_AgendarCBU(DatosPers aDatosPers[], short nroUsu) {
  Contacto nuevo;
  char     cbuIngresado[LARGO_DETALLE + 1];
  short    pos;
  bool     valido;

  Operacion::PantallaOperacion("AGENDAR UNA NUEVA CUENTA CBU", aDatosPers,
                               nroUsu);

  // La agenda es un array de tamanio fijo: si esta completa se avisa antes de
  // pedir los datos, para no hacer trabajar al usuario en vano.
  if (aDatosPers[nroUsu].cantAgenda >= MAX_AGENDA) {
    Operacion::Error("La agenda esta completa.");
    cout << "  Solo se admiten " << MAX_AGENDA
         << " cuentas agendadas: elimine alguna antes de agregar otra." << endl;
    Operacion::Continuar();
    return;
  }
  cout << "  Cuentas agendadas: " << aDatosPers[nroUsu].cantAgenda << " de "
       << MAX_AGENDA << endl << endl;

  Validar::LeerCadena("  Alias...............: ", nuevo.alias, LARGO_DETALLE,
                      true);
  if (Validar::EntradaAgotada())
    return;

  // BUSQUEDA BINARIA: el alias es la clave por la que esta ordenado el array.
  if (Ordenar::BinariaAlias(aDatosPers[nroUsu].aAgenda,
                            aDatosPers[nroUsu].cantAgenda,
                            nuevo.alias) != NO_ENCONTRADO) {
    Operacion::Error("Ya existe una cuenta agendada con ese alias.");
    cout << "  Elija otro alias o elimine primero la cuenta existente."
         << endl;
    Operacion::Continuar();
    return;
  }

  Validar::LeerCadena("  Apellido y nombre...: ", nuevo.apellidoNombre,
                      LARGO_NOMBRE, true);
  if (Validar::EntradaAgotada())
    return;

  // Ciclo 1-x: el CBU se pide hasta que tenga los 22 digitos exigidos. Se lee
  // en un buffer mas ancho que el campo para poder rechazar los que se pasan
  // de largo en lugar de truncarlos en silencio.
  do {
    Validar::LeerCadena("  CBU (22 digitos)....: ", cbuIngresado,
                        LARGO_DETALLE, false);
    valido = Validar::CBUValido(cbuIngresado);
    if (not valido and not Validar::EntradaAgotada())
      Operacion::Error("El CBU debe tener exactamente 22 digitos, sin "
                       "espacios ni guiones.");
  } while (not valido and not Validar::EntradaAgotada());
  if (Validar::EntradaAgotada())
    return;
  Validar::CopiarCadena(nuevo.cbu, cbuIngresado, LARGO_CBU);

  // BUSQUEDA SECUENCIAL: el CBU no es la clave de ordenamiento de la agenda,
  // asi que se recorre el array de principio a fin con corte.
  pos = Ordenar::SecuencialCBU(aDatosPers[nroUsu].aAgenda,
                               aDatosPers[nroUsu].cantAgenda, nuevo.cbu);
  if (pos != NO_ENCONTRADO) {
    Operacion::Error("Ese CBU ya figura en su agenda.");
    cout << "  Esta agendado con el alias: "
         << aDatosPers[nroUsu].aAgenda[pos].alias << endl;
    Operacion::Continuar();
    return;
  }

  // El celular es OPCIONAL: si se deja vacio el contacto queda sin celular y
  // no se lo podra alcanzar desde <Transferir a un celular>. Por ser opcional
  // se lee con Validar::LeerLinea(), que admite la linea vacia.
  do {
    Screen::_textcolor(COL_ROTULO);
    cout << "  Celular (opcional)..: ";
    Screen::ColorNormal();
    cout.flush();
    Validar::LeerLinea(nuevo.celular, LARGO_CELULAR);
    valido = nuevo.celular[0] == '\0' or
             Validar::CelularValido(nuevo.celular);
    if (not valido and not Validar::EntradaAgotada())
      Operacion::Error("El celular se ingresa solo con digitos, o se deja "
                       "vacio.");
  } while (not valido and not Validar::EntradaAgotada());
  if (Validar::EntradaAgotada())
    return;

  Validar::LeerCadena("  Banco...............: ", nuevo.banco, LARGO_DETALLE,
                      true);
  if (Validar::EntradaAgotada())
    return;

  // La insercion ubica el contacto en el lugar que le corresponde por alias
  // desplazando los posteriores: la agenda NUNCA deja de estar ordenada, y por
  // eso la busqueda por alias puede seguir siendo BINARIA (log2 n
  // comparaciones) en lugar de secuencial (n comparaciones).
  if (Ordenar::InsertarContactoEnOrden(aDatosPers[nroUsu].aAgenda,
                                       aDatosPers[nroUsu].cantAgenda, nuevo)) {
    Operacion::Exito("La cuenta quedo agendada.");
    cout << endl;
    AgendaMostrarContacto(nuevo);
    cout << endl << "  Cuentas agendadas: " << aDatosPers[nroUsu].cantAgenda
         << " de " << MAX_AGENDA << endl;
  }
  else if (aDatosPers[nroUsu].cantAgenda >= MAX_AGENDA)
    Operacion::Error("La agenda esta completa: no se pudo agendar la cuenta.");
  else
    Operacion::Error("Ya existe una cuenta agendada con ese alias.");

  Operacion::Continuar();
} // Menu_AgendarCBU

// ---------------------------------------------------------------------------
// Nivel 3 - Busqueda de una cuenta agendada por su alias.
// ---------------------------------------------------------------------------
void Menu_BuscarAlias(DatosPers aDatosPers[], short nroUsu) {
  char  alias[LARGO_DETALLE + 1];
  short pos, maxComp = 0, restantes;

  Operacion::PantallaOperacion("BUSCAR UNA CUENTA POR ALIAS", aDatosPers,
                               nroUsu);

  if (aDatosPers[nroUsu].cantAgenda == 0) {
    Operacion::Aviso("La agenda de CBU esta vacia: no hay nada para buscar.");
    Operacion::Continuar();
    return;
  }

  // Cantidad maxima de comparaciones de una busqueda binaria sobre n
  // elementos: floor(log2(n)) + 1. Se calcula dividiendo el intervalo por dos
  // hasta agotarlo, sin recurrir a la biblioteca matematica.
  restantes = aDatosPers[nroUsu].cantAgenda;
  while (restantes > 0) {
    maxComp++;
    restantes = (short)(restantes / 2);
  }

  cout << "  Cuentas agendadas..........................: "
       << aDatosPers[nroUsu].cantAgenda << endl;
  cout << "  Comparaciones maximas (busqueda binaria)...: " << maxComp << endl;
  cout << "  Comparaciones maximas (busqueda secuencial): "
       << aDatosPers[nroUsu].cantAgenda << endl << endl;

  Validar::LeerCadena("  Alias a buscar: ", alias, LARGO_DETALLE, true);
  if (Validar::EntradaAgotada())
    return;

  // BUSQUEDA BINARIA: es aplicable porque la agenda se conserva ordenada por
  // alias tanto en las altas como en las bajas.
  pos = Ordenar::BinariaAlias(aDatosPers[nroUsu].aAgenda,
                              aDatosPers[nroUsu].cantAgenda, alias);
  if (pos == NO_ENCONTRADO) {
    Operacion::Aviso("No hay ninguna cuenta agendada con ese alias.");
    cout << "  Tenga en cuenta que el alias distingue mayusculas de "
         << "minusculas." << endl;
  }
  else {
    Operacion::Exito("Cuenta encontrada.");
    cout << endl;
    AgendaMostrarContacto(aDatosPers[nroUsu].aAgenda[pos]);
    cout << endl << "  Posicion en la agenda: " << pos + 1 << " de "
         << aDatosPers[nroUsu].cantAgenda << endl;
  }
  Operacion::Continuar();
} // Menu_BuscarAlias

// ---------------------------------------------------------------------------
// Nivel 3 - Baja de una cuenta de la agenda.
// ---------------------------------------------------------------------------
void Menu_EliminarCBU(DatosPers aDatosPers[], short nroUsu) {
  short nro, pos;
  bool  confirma;

  Operacion::PantallaOperacion("ELIMINAR UNA CUENTA AGENDADA", aDatosPers,
                               nroUsu);

  if (aDatosPers[nroUsu].cantAgenda == 0) {
    Operacion::Aviso("La agenda de CBU esta vacia: no hay nada para eliminar.");
    Operacion::Continuar();
    return;
  }

  Operacion::ListarAgenda(aDatosPers, nroUsu);
  cout << endl;

  nro = (short) Validar::LeerEntero("  Numero a eliminar (0 cancela): ",
                                    AGENDA_CANCELA,
                                    aDatosPers[nroUsu].cantAgenda);
  if (Validar::EntradaAgotada())
    return;
  if (nro == AGENDA_CANCELA) {
    Operacion::Aviso("No se elimino ninguna cuenta.");
    Operacion::Continuar();
    return;
  }

  pos = (short)(nro - 1);      // Operacion::ListarAgenda numera desde 1
  cout << endl;
  AgendaMostrarContacto(aDatosPers[nroUsu].aAgenda[pos]);
  cout << endl;

  confirma = Validar::LeerSiNo("  Confirma la eliminacion (S/N)?: ");
  // Se consulta el fin de la entrada ANTES de usar la respuesta: al agotarse
  // la entrada LeerSiNo() devuelve un valor neutro que no expresa la voluntad
  // del usuario, y una baja no debe ejecutarse sin confirmacion real.
  if (Validar::EntradaAgotada())
    return;
  if (not confirma) {
    Operacion::Aviso("No se elimino ninguna cuenta.");
    Operacion::Continuar();
    return;
  }

  // La baja compacta el array desplazando los contactos siguientes: la agenda
  // queda sin huecos y sigue ordenada por alias.
  if (Ordenar::EliminarContacto(aDatosPers[nroUsu].aAgenda,
                                aDatosPers[nroUsu].cantAgenda, pos)) {
    Operacion::Exito("La cuenta fue eliminada de la agenda.");
    cout << "  Cuentas agendadas: " << aDatosPers[nroUsu].cantAgenda << " de "
         << MAX_AGENDA << endl;
  }
  else
    Operacion::Error("No fue posible eliminar la cuenta indicada.");

  Operacion::Continuar();
} // Menu_EliminarCBU

// ---------------------------------------------------------------------------
// Nivel 2 - Submenu de la agenda de CBU.
// ---------------------------------------------------------------------------
void Menu_AgendaCBU(DatosPers aDatosPers[], short nroUsu) {
  str40Opc aMenu[AGENDA_CANT_OPC] = {
    "Ver la agenda completa (por alias)",
    "Agendar una nueva cuenta CBU",
    "Buscar por alias (busqueda binaria)",
    "Eliminar una cuenta agendada",
    "Volver al menu principal"
  };
  short linMenu, opcion;
  bool  volver = false;

  do {
    linMenu = Operacion::PantallaMenu("AGENDA DE CBU", aDatosPers, nroUsu);
    opcion  = Screen::MenuNavegar(aMenu, AGENDA_CANT_OPC, linMenu,
                                  Operacion::COL_IZQ);
    switch (opcion) {
      case 0 : Operacion::PantallaOperacion("AGENDA DE CBU - CUENTAS "
                                            "AGENDADAS", aDatosPers, nroUsu);
               Operacion::ListarAgenda(aDatosPers, nroUsu);
               Operacion::Continuar();                            break;
      case 1 : Menu_AgendarCBU(aDatosPers, nroUsu);               break;
      case 2 : Menu_BuscarAlias(aDatosPers, nroUsu);              break;
      case 3 : Menu_EliminarCBU(aDatosPers, nroUsu);              break;
      default: volver = true;                 // opcion Volver o ESCAPE
    }
  } while (not volver and not Validar::EntradaAgotada());
} // Menu_AgendaCBU

#endif // MENU_AGENDACBU_HPP
