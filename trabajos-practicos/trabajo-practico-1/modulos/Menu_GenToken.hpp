// ---------------------------------------------------------------------------
// Menu_GenToken.hpp
// Nivel 2 - Generar Token de seguridad.
//
// El token es una clave de UN SOLO USO de seis digitos que el banco exige para
// autorizar las operaciones de importe elevado. Aca se lo genera a pedido del
// usuario (opcion <<Generar Token>> del menu principal) y se lo muestra
// destacado en pantalla junto con la hora de emision y la hora de vencimiento.
//
// La generacion se delega en Operacion::GenerarToken(), que sortea el numero
// con rand() dentro del rango TOKEN_MINIMO..TOKEN_MAXIMO. De esa forma el
// mismo mecanismo se usa aca y en Operacion::ValidarToken(), que es la que
// pide el token cuando una operacion supera TOPE_SIN_TOKEN.
//
// El vencimiento se simula sumando VALIDEZ_MINUTOS a la hora del sistema: no
// se controla el paso del tiempo, porque el token se valida en el mismo
// instante en que se lo emite.
// ---------------------------------------------------------------------------
#ifndef MENU_GENTOKEN_HPP
#define MENU_GENTOKEN_HPP

// ---------------------------------------------------------------------------
// Pantalla de operacion. Permite emitir tantos tokens como se quiera: el ciclo
// es 0-x (while) sobre la respuesta S/N, y corta tambien cuando la entrada se
// agota, porque en ese caso Validar::LeerSiNo() devuelve el valor por omision
// y el ciclo quedaria girando para siempre.
// ---------------------------------------------------------------------------
void Menu_GenToken(DatosPers aDatosPers[], short nroUsu) {
  const short VALIDEZ_MINUTOS  =   2; // vigencia simulada del token
  const short MINUTOS_POR_HORA =  60;
  const short HORAS_POR_DIA    =  24;
  const short LARGO_HORA       =   8; // "hh:mm:ss"
  const short ANCHO_RECUADRO   =  46; // ancho del cartel que destaca el token
  const short LARGO_MENSAJE    = 100;
  char  hora[LARGO_HORA + 1], vence[LARGO_HORA + 1];
  char  recuadro[ANCHO_RECUADRO + 1];
  char  mensaje[LARGO_MENSAJE + 1];
  int   hh, mm, ss;
  long  token;
  short largo, relleno;
  bool  otro = false;

  do {
    Operacion::PantallaOperacion("GENERAR TOKEN DE SEGURIDAD", aDatosPers,
                                 nroUsu);

    // ----- Emision del token -----
    token = Operacion::GenerarToken();
    FechaHora::HoraDeHoy(hora);
    FechaHora::GetTime(hh, mm, ss);
    mm = mm + VALIDEZ_MINUTOS;
    hh = (hh + mm / MINUTOS_POR_HORA) % HORAS_POR_DIA; // pasa de las 23:59
    mm = mm % MINUTOS_POR_HORA;
    snprintf(vence, sizeof vence, "%02d:%02d:%02d", hh, mm, ss);

    cout << "  Titular......: " << aDatosPers[nroUsu].apellidoNombre << endl;
    cout << "  Celular......: " << aDatosPers[nroUsu].celular << endl;
    cout << endl;

    // ----- Cartel con el token, centrado dentro del recuadro -----
    snprintf(recuadro, sizeof recuadro, "T O K E N :   %ld", token);
    largo   = Screen::LargoVisible(recuadro);
    relleno = (short)((ANCHO_RECUADRO - 2 - largo) / 2);
    Screen::_textcolor(COL_MARCO);
    cout << "  ";
    Screen::Separador(ANCHO_RECUADRO, '*');
    cout << endl << "  *";
    Screen::_textcolor(COL_EXITO);
    Operacion::Blancos(relleno);
    cout << recuadro;
    Operacion::Blancos((short)(ANCHO_RECUADRO - 2 - largo - relleno));
    Screen::_textcolor(COL_MARCO);
    cout << '*' << endl << "  ";
    Screen::Separador(ANCHO_RECUADRO, '*');
    cout << endl;
    Screen::ColorNormal();

    cout << endl;
    cout << "  Emitido a las..: " << hora << endl;
    cout << "  Valido hasta...: " << vence << "   (" << VALIDEZ_MINUTOS
         << " minutos de vigencia)" << endl;
    cout << "  Uso............: unico. Al usarlo o al vencer queda anulado."
         << endl;

    snprintf(mensaje, sizeof mensaje,
             "Se pide token en forma automatica cuando la operacion supera $ "
             "%.2f", TOPE_SIN_TOKEN);
    Operacion::Aviso(mensaje);
    cout << "  Tambien puede emitirlo desde aca y tenerlo a mano antes de"
         << endl;
    cout << "  iniciar una transferencia o un pago de importe elevado." << endl;
    cout << "  El banco NUNCA le va a pedir el token por telefono ni por mail."
         << endl;
    cout << endl;

    // Validar::LeerSiNo() devuelve el valor por omision si la entrada se
    // agoto: por eso se pregunta solo cuando todavia hay datos para leer.
    otro = false;
    if (not Validar::EntradaAgotada())
      otro = Validar::LeerSiNo("  Desea generar otro token (S/N)?: ");
  } while (otro and not Validar::EntradaAgotada());
} // Menu_GenToken

#endif // MENU_GENTOKEN_HPP
