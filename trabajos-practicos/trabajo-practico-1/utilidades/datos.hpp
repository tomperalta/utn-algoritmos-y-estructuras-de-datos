// ---------------------------------------------------------------------------
// datos.hpp
// Muestra de datos del sistema.
//
//   * CargarDatosPersonales() registra internamente en el codigo los CINCO
//     usuarios del Home Banking, con sus cuentas y su agenda de CBU. Es la
//     estructura combinada <<array de registros>> que pide el enunciado.
//   * GenerarMuestraDatos() crea los tres archivos de texto de movimientos con
//     la muestra de datos del grupo, unicamente si todavia no existen. De esa
//     forma el programa se puede ejecutar por primera vez sin datos y, a
//     partir de ahi, los archivos se conservan y solo se les agregan las
//     nuevas componentes que generan las operaciones.
//
// Los datos de las personas son de fantasia: no corresponden a personas reales.
// ---------------------------------------------------------------------------
#ifndef DATOS_HPP
#define DATOS_HPP

namespace Datos {

  // -------------------------------------------------------------------------
  // Registros auxiliares usados solamente para escribir la muestra de datos en
  // forma compacta y legible.
  // -------------------------------------------------------------------------
  record MuestraUsuario {
    long        dni;
    const char *apellidoNombre;
    short       diaNac, mesNac, anioNac;
    const char *usuario;
    const char *clave;
    const char *celular;
    const char *email;
    const char *domicilio;
    const char *nroCtaCA;
    const char *cbu;
    double      saldoInicialCA;   // saldo previo a los movimientos del archivo
    double      saldoDolares;     // saldo de la Caja de Ahorro en dolares
  };

  record MuestraContacto {
    short       usuario;
    const char *alias;
    const char *apellidoNombre;
    const char *cbu;
    const char *celular;
    const char *banco;
  };

  record MuestraMovCA {
    short       usuario;
    short       dia, mes, anio;
    char        tipoMov;
    const char *detalle;
    double      importe;
  };

  record MuestraMovTD {
    short       usuario;
    short       dia, mes, anio;
    const char *detalle;
    double      importe;
  };

  record MuestraMovTC {
    short       usuario;
    short       dia, mes, anio;
    const char *detalle;
    const char *cuotas;
    long        nroCpbte;
    double      importe;
  };

  // =========================================================================
  //                        LOS CINCO USUARIOS DEL SISTEMA
  // =========================================================================
  const short CANT_USUARIOS_MUESTRA = 5;

  const MuestraUsuario USUARIOS[CANT_USUARIOS_MUESTRA] = {
    {30125478, "ALVAREZ MARIA SOLEDAD",     14,  3, 1983,
     "malvarez",   "Sol1983",  "3414785296", "malvarez@correo.com",
     "Av. Pellegrini 1450 - Rosario",   "106-4235689707-3",
     "1060099920000423568970",  1250000.00,  4820.55},

    {27890456, "BENITEZ CARLOS ALBERTO",     2, 11, 1979,
     "cbenitez",   "Cba2024",  "3415236987", "cbenitez@correo.com",
     "San Martin 785 - Santa Fe",       "106-4235690112-8",
     "1060099920000423569011",   480000.00, 12750.00},

    {35478912, "CANTERO LUCIA BEATRIZ",     25,  7, 1990,
     "lcantero",   "Luc2590",  "3415874123", "lcantero@correo.com",
     "Mitre 342 - Parana",              "106-4235691345-5",
     "1060099920000423569134",    92500.00,   980.20},

    {41236789, "DOMINGUEZ JAVIER HORACIO",   9,  1, 1999,
     "jdominguez", "Jav0199",  "3416987452", "jdominguez@correo.com",
     "Bv. Orono 2130 - Rosario",        "106-4235692578-1",
     "1060099920000423569257",    35800.00,   150.00},

    {23456781, "ESPINDOLA ROBERTO DANIEL",  18,  5, 1974,
     "respindola", "Rob1874",  "3414123658", "respindola@correo.com",
     "Rioja 1290 - Rosario",            "106-4235693781-9",
     "1060099920000423569378",  2100000.00, 35600.40}
  };

  // =========================================================================
  //                    CBU YA AGENDADOS PARA TRANSFERENCIAS
  // =========================================================================
  const short CANT_CONTACTOS_MUESTRA = 16;

  const MuestraContacto CONTACTOS[CANT_CONTACTOS_MUESTRA] = {
    {1, "gas.litoral",     "LITORAL GAS S.A.",         "0170099940000012345678", "3415550001", "Banco Frances"},
    {1, "juan.perez",      "PEREZ JUAN MANUEL",        "0290099910000098765432", "3415551122", "Banco Nacion"},
    {1, "mama",            "ALVAREZ ELSA NOEMI",       "1910099930000045678912", "3415553344", "Banco Credicoop"},
    {1, "alquiler.depto",  "SOSA INMOBILIARIA SRL",    "0720099950000078912345", "3415555566", "Banco Santander"},

    {2, "ana.gomez",       "GOMEZ ANA CLARA",          "0110099960000032165498", "3425557788", "Banco Nacion"},
    {2, "seguro.auto",     "PROTEGER SEGUROS S.A.",    "0150099970000065498732", "3425559900", "Banco ICBC"},
    {2, "hermano",         "BENITEZ MARTIN JOSE",      "0340099980000014785236", "3425551234", "Banco Galicia"},

    {3, "escuela",         "COLEGIO SAN JOSE",         "0140099910000025836914", "3435554321", "Banco Provincia"},
    {3, "pablo.ruiz",      "RUIZ PABLO ESTEBAN",       "0270099920000036925814", "3435558765", "Banco Supervielle"},
    {3, "veterinaria",     "VETERINARIA HUELLAS SRL",  "0450099930000041236587", "3435552468", "Banco Macro"},

    {4, "expensas",        "CONSORCIO EDIFICIO ALTO",  "0160099940000052147896", "3445551357", "Banco BBVA"},
    {4, "sofia.luna",      "LUNA SOFIA BELEN",         "0280099950000063258741", "3445559513", "Banco Patagonia"},
    {4, "gimnasio",        "SPORT CLUB ROSARIO S.A.",  "0330099960000074185296", "3445557531", "Banco HSBC"},

    {5, "contador",        "MORALES ESTUDIO CONTABLE", "0170099970000085296314", "3455552580", "Banco Frances"},
    {5, "nieta",           "ESPINDOLA CAMILA ROCIO",   "0290099980000096325874", "3455554826", "Banco Nacion"},
    {5, "prepaga.salud",   "SALUD INTEGRAL S.A.",      "0200099910000017539514", "3455556248", "Banco Ciudad"}
  };

  // =========================================================================
  //          MUESTRA DE DATOS DEL ARCHIVO MovimientosCA.Txt  (26 registros)
  // =========================================================================
  const short CANT_MOV_CA_MUESTRA = 26;

  const MuestraMovCA MOV_CA[CANT_MOV_CA_MUESTRA] = {
    {1,  1, 4, 2026, 'D', "Sueldo Marzo 2026",        1542367.21},
    {1,  1, 4, 2026, 'H', "Combustible YPF",            35983.68},
    {1,  3, 4, 2026, 'H', "Kiosco Las 5 Esquinas",       8952.03},
    {1, 10, 4, 2026, 'D', "Transferencia por Serv.",    156236.40},
    {1, 11, 4, 2026, 'H', "Restaurant Siga la Vaca",     76591.31},
    {1, 15, 4, 2026, 'H', "Pago Tarjeta de Credito",    245680.55},

    {2,  2, 4, 2026, 'D', "Sueldo Marzo 2026",         987450.30},
    {2,  4, 4, 2026, 'H', "Supermercado Kotto",        125634.87},
    {2,  8, 4, 2026, 'H', "Recarga Celular",            12000.00},
    {2, 12, 4, 2026, 'D', "Plazo Fijo acreditado",     315780.44},
    {2, 20, 4, 2026, 'H', "Alquiler Departamento",     420000.00},

    {3,  3, 4, 2026, 'D', "Sueldo Marzo 2026",         743210.15},
    {3,  5, 4, 2026, 'H', "Farmacia Del Centro",        34567.20},
    {3,  9, 4, 2026, 'H', "Recarga SUBE",                5000.00},
    {3, 14, 4, 2026, 'H', "Libreria Alfa-Omega",         9782.67},
    {3, 22, 4, 2026, 'D', "Venta de dolares",          195050.00},

    {4,  1, 4, 2026, 'D', "Sueldo Marzo 2026",         612345.90},
    {4,  6, 4, 2026, 'H', "Pescaderia El Pez Cado",    106491.09},
    {4, 13, 4, 2026, 'H', "Casa de Pastas La Juvenil",  38412.25},
    {4, 18, 4, 2026, 'D', "Transferencia recibida",     87500.00},
    {4, 25, 4, 2026, 'H', "Compra Fondo Comun",        150000.00},

    {5,  2, 4, 2026, 'D', "Jubilacion Marzo 2026",     458900.75},
    {5,  7, 4, 2026, 'H', "Pago Servicios EPE",         62340.18},
    {5, 16, 4, 2026, 'H', "Panaderia Buen Dia",         15230.84},
    {5, 21, 4, 2026, 'D', "Plazo Fijo acreditado",    1102450.60},
    {5, 28, 4, 2026, 'H', "Ferreteria Black Decker",    85440.25}
  };

  // =========================================================================
  //          MUESTRA DE DATOS DEL ARCHIVO MovimientosTD.Txt  (17 registros)
  // =========================================================================
  const short CANT_MOV_TD_MUESTRA = 17;

  const MuestraMovTD MOV_TD[CANT_MOV_TD_MUESTRA] = {
    {1,  5, 3, 2026, "Casa de Pastas La Juvenil",  38412.25},
    {1, 12, 3, 2026, "Pescaderia El Pez Cado",     10649.09},
    {1, 15, 3, 2026, "Supermercado Kotto",         25632.94},
    {1, 29, 3, 2026, "Libreria Alfa-Omega",         9782.67},

    {2,  3, 3, 2026, "Farmacia Del Centro",         8450.30},
    {2, 11, 3, 2026, "Estacion Servicio Shell",    32780.55},
    {2, 22, 3, 2026, "Verduleria La Huerta",        6320.40},

    {3,  7, 3, 2026, "Perfumeria Rouge",           14990.00},
    {3, 18, 3, 2026, "Panaderia Buen Dia",          4780.25},
    {3, 27, 3, 2026, "Cine Monumental",            12500.00},

    {4,  2, 3, 2026, "Rotiseria Don Pepe",          7650.80},
    {4, 14, 3, 2026, "Kiosco Las 5 Esquinas",       3250.00},
    {4, 25, 3, 2026, "Heladeria Vittorio",          9100.45},

    {5,  6, 3, 2026, "Ferreteria Black Decker",    45320.90},
    {5, 16, 3, 2026, "Optica Vision Total",        78400.00},
    {5, 24, 3, 2026, "Vinoteca El Descorche",      23150.75},
    {5, 30, 3, 2026, "Bazar La Fortuna",           18600.00}
  };

  // =========================================================================
  //          MUESTRA DE DATOS DEL ARCHIVO MovimientosTC.Txt  (17 registros)
  //   Un numero de comprobante en cero significa <<sin comprobante>> y se
  //   graba en blanco; lo mismo ocurre con las cuotas de un pago unico.
  // =========================================================================
  const short CANT_MOV_TC_MUESTRA = 17;

  const MuestraMovTC MOV_TC[CANT_MOV_TC_MUESTRA] = {
    {1,  3, 2, 2026, "Pizzeria Kentucky",        "1/3",  245684,  29567.73},
    {1, 14, 2, 2026, "Panaderia Buen Dia",       "",          0,  58323.84},
    {1, 19, 2, 2026, "Combustible YPF",          "",      37485,  36491.09},
    {1, 26, 2, 2026, "Kit Herramientas Black",   "2/3",  162395,  85440.25},

    {2,  5, 2, 2026, "Indumentaria Zara",        "3/6",  118420,  64200.00},
    {2, 17, 2, 2026, "Farmacia Del Centro",      "1/1",  220145,  18750.30},
    {2, 28, 2, 2026, "Electro Hogar Sur",        "6/12", 305471, 152300.90},

    {3,  8, 2, 2026, "Libreria Alfa-Omega",      "1/3",  411256,  27600.45},
    {3, 20, 2, 2026, "Cine Monumental",          "",          0,  12500.00},
    {3, 25, 2, 2026, "Viaje Aerolineas",         "4/12", 508233, 348900.00},

    {4,  2, 2, 2026, "Supermercado Kotto",       "2/3",  601478,  45300.20},
    {4, 13, 2, 2026, "Deportes Full Sport",      "1/6",  617392,  89750.60},
    {4, 23, 2, 2026, "Rotiseria Don Pepe",       "",          0,   7650.80},

    {5,  4, 2, 2026, "Optica Vision Total",      "3/12", 702584,  78400.00},
    {5, 11, 2, 2026, "Vinoteca El Descorche",    "1/3",  715039,  23150.75},
    {5, 19, 2, 2026, "Neumaticos Fate",          "6/18", 728461, 312450.55},
    {5, 27, 2, 2026, "Bazar La Fortuna",         "",          0,  18600.00}
  };

  // =========================================================================
  //                        CARGA DE LOS DATOS PERSONALES
  // =========================================================================

  // -------------------------------------------------------------------------
  // Arma una cuenta bancaria con los datos indicados.
  // -------------------------------------------------------------------------
  void ArmarCuenta(Cuenta &cta, const char nroCta[], const char tipo[],
                   const char moneda[], double saldoInicial, char estado) {
    Validar::CopiarCadena(cta.nroCta, nroCta, LARGO_CTA);
    Validar::CopiarCadena(cta.tipo,   tipo,   3);
    Validar::CopiarCadena(cta.moneda, moneda, 3);
    cta.saldoInicial = saldoInicial;
    cta.saldo        = saldoInicial;
    cta.estado       = estado;
  } // ArmarCuenta

  // -------------------------------------------------------------------------
  // Registra internamente los cinco usuarios: datos personales, cuentas y
  // agenda de CBU. La agenda queda ORDENADA POR ALIAS, condicion necesaria
  // para poder aplicarle luego la busqueda binaria.
  // -------------------------------------------------------------------------
  void CargarDatosPersonales(DatosPers aDatosPers[]) {
    char nroCtaDolares[LARGO_CTA + 1];

    for (short u = 0; u < CANT_USUARIOS_MUESTRA; u++) {
      DatosPers &usu = aDatosPers[u];

      usu.dni = USUARIOS[u].dni;
      Validar::CopiarCadena(usu.apellidoNombre, USUARIOS[u].apellidoNombre,
                            LARGO_NOMBRE);
      usu.fechaNac.dia  = USUARIOS[u].diaNac;
      usu.fechaNac.mes  = USUARIOS[u].mesNac;
      usu.fechaNac.anio = USUARIOS[u].anioNac;
      Validar::CopiarCadena(usu.usuario,   USUARIOS[u].usuario,   LARGO_USUARIO);
      Validar::CopiarCadena(usu.clave,     USUARIOS[u].clave,     LARGO_CLAVE);
      Validar::CopiarCadena(usu.celular,   USUARIOS[u].celular,   LARGO_CELULAR);
      Validar::CopiarCadena(usu.email,     USUARIOS[u].email,     LARGO_NOMBRE);
      Validar::CopiarCadena(usu.domicilio, USUARIOS[u].domicilio, LARGO_NOMBRE);
      Validar::CopiarCadena(usu.nroCtaCA,  USUARIOS[u].nroCtaCA,  LARGO_CTA);
      Validar::CopiarCadena(usu.cbu,       USUARIOS[u].cbu,       LARGO_CBU);
      usu.nroUsuario = (short)(u + 1);

      // ----- Cuentas del usuario -----
      usu.cantCuentas = 0;
      // Cuenta principal: Caja de Ahorro en pesos. Su saldo se recalcula a
      // partir de los movimientos del archivo MovimientosCA.Txt.
      ArmarCuenta(usu.aCuentas[usu.cantCuentas++], USUARIOS[u].nroCtaCA, "CA",
                  "$", USUARIOS[u].saldoInicialCA, 'A');
      // Caja de Ahorro en dolares, asociada a la misma cuenta.
      snprintf(nroCtaDolares, sizeof nroCtaDolares, "%.*s-U",
               (int)(strlen(USUARIOS[u].nroCtaCA) - 2), USUARIOS[u].nroCtaCA);
      ArmarCuenta(usu.aCuentas[usu.cantCuentas++], nroCtaDolares, "CA", "u$s",
                  USUARIOS[u].saldoDolares, 'A');

      // ----- Agenda de CBU -----
      usu.cantAgenda = 0;
      for (short c = 0; c < CANT_CONTACTOS_MUESTRA; c++)
        if (CONTACTOS[c].usuario == u + 1 and usu.cantAgenda < MAX_AGENDA) {
          Contacto &con = usu.aAgenda[usu.cantAgenda++];
          Validar::CopiarCadena(con.alias, CONTACTOS[c].alias, LARGO_DETALLE);
          Validar::CopiarCadena(con.apellidoNombre,
                                CONTACTOS[c].apellidoNombre, LARGO_NOMBRE);
          Validar::CopiarCadena(con.cbu, CONTACTOS[c].cbu, LARGO_CBU);
          Validar::CopiarCadena(con.celular, CONTACTOS[c].celular,
                                LARGO_CELULAR);
          Validar::CopiarCadena(con.banco, CONTACTOS[c].banco, LARGO_DETALLE);
        }
      Ordenar::AgendaPorAlias(usu.aAgenda, usu.cantAgenda);
    }
  } // CargarDatosPersonales

  // =========================================================================
  //                  GENERACION DE LOS ARCHIVOS DE LA MUESTRA
  // =========================================================================

  // -------------------------------------------------------------------------
  // Crea MovimientosCA.Txt con la muestra de datos. Devuelve la cantidad de
  // registros grabados.
  // -------------------------------------------------------------------------
  short GenerarArchivoCA() {
    MovCA mov;
    short grabados = 0;

    for (short i = 0; i < CANT_MOV_CA_MUESTRA; i++) {
      mov.fecha.dia  = MOV_CA[i].dia;
      mov.fecha.mes  = MOV_CA[i].mes;
      mov.fecha.anio = MOV_CA[i].anio;
      mov.tipoMov    = MOV_CA[i].tipoMov;
      Validar::CopiarCadena(mov.detalle, MOV_CA[i].detalle, LARGO_DETALLE);
      mov.importe    = MOV_CA[i].importe;
      mov.nroUsuario = MOV_CA[i].usuario;
      mov.saldo      = 0;
      mov.nroReg     = i;
      if (Archivo::AgregarMovimientoCA(mov))
        grabados++;
    }
    return grabados;
  } // GenerarArchivoCA

  short GenerarArchivoTD() {
    MovTD mov;
    short grabados = 0;

    for (short i = 0; i < CANT_MOV_TD_MUESTRA; i++) {
      mov.fecha.dia  = MOV_TD[i].dia;
      mov.fecha.mes  = MOV_TD[i].mes;
      mov.fecha.anio = MOV_TD[i].anio;
      Validar::CopiarCadena(mov.detalle, MOV_TD[i].detalle, LARGO_DETALLE);
      mov.importe    = MOV_TD[i].importe;
      mov.nroUsuario = MOV_TD[i].usuario;
      mov.nroReg     = i;
      if (Archivo::AgregarMovimientoTD(mov))
        grabados++;
    }
    return grabados;
  } // GenerarArchivoTD

  short GenerarArchivoTC() {
    MovTC mov;
    short grabados = 0;

    for (short i = 0; i < CANT_MOV_TC_MUESTRA; i++) {
      mov.fecha.dia  = MOV_TC[i].dia;
      mov.fecha.mes  = MOV_TC[i].mes;
      mov.fecha.anio = MOV_TC[i].anio;
      Validar::CopiarCadena(mov.detalle, MOV_TC[i].detalle, LARGO_DETALLE);
      Validar::CopiarCadena(mov.cuotas,  MOV_TC[i].cuotas,  LARGO_CUOTAS);
      mov.nroCpbte   = MOV_TC[i].nroCpbte;
      mov.importe    = MOV_TC[i].importe;
      mov.nroUsuario = MOV_TC[i].usuario;
      mov.nroReg     = i;
      if (Archivo::AgregarMovimientoTC(mov))
        grabados++;
    }
    return grabados;
  } // GenerarArchivoTC

  // -------------------------------------------------------------------------
  // Crea los archivos de movimientos que todavia no existan. Los archivos ya
  // existentes NO se tocan: solo se les agregan componentes nuevas desde las
  // operaciones del Home Banking.
  // -------------------------------------------------------------------------
  void GenerarMuestraDatos() {
    if (not Archivo::Existe(ARCH_MOV_CA))
      GenerarArchivoCA();
    if (not Archivo::Existe(ARCH_MOV_TD))
      GenerarArchivoTD();
    if (not Archivo::Existe(ARCH_MOV_TC))
      GenerarArchivoTC();
  } // GenerarMuestraDatos

  // -------------------------------------------------------------------------
  // Verifica que cada archivo de movimientos tenga la cantidad minima de
  // registros que exige el enunciado (MIN_REG_ARCH). Devuelve la cantidad de
  // archivos que NO la alcanzan, y deja en <<aviso>> el detalle.
  // -------------------------------------------------------------------------
  short VerificarMuestraDatos(char aviso[], short tam) {
    const char *ARCHIVOS[3] = {ARCH_MOV_CA, ARCH_MOV_TD, ARCH_MOV_TC};
    short       faltantes = 0;
    short       usados = 0;

    aviso[0] = '\0';
    for (short i = 0; i < 3; i++) {
      long cant = Archivo::CantidadRegistros(ARCHIVOS[i]);
      if (cant < MIN_REG_ARCH) {
        faltantes++;
        usados = (short)(usados + snprintf(aviso + usados, tam - usados,
                                           "%s%s tiene %ld de %d",
                                           usados > 0 ? "; " : "",
                                           ARCHIVOS[i], cant, MIN_REG_ARCH));
        if (usados >= tam - 1)
          break;
      }
    }
    return faltantes;
  } // VerificarMuestraDatos

} // namespace Datos

#endif // DATOS_HPP
