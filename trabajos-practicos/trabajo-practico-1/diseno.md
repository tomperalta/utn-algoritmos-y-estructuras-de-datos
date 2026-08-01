# TP #1 — Home Banking · Diseño de estructuras de datos y de módulos

> Documento de acompañamiento del código. Cubre los puntos 4 a 8 de las
> **Pautas de entrega** del enunciado: diseño gráfico de las estructuras de
> datos con sus rótulos y tamaños, gráfico del bloque principal, gráfico de
> cada módulo, muestra de datos y resultados esperados.

| | |
|---|---|
| **Programa** | `TP1V1_K____G__` — Home Banking Mesopotamia |
| **Versión** | V1 |
| **Fecha de entrega** | *(completar)* |
| **Curso / día / turno** | *(completar)* |
| **Grupo Nº** | *(completar)* |
| **Integrantes** | *(Nº legajo — Apellido, Nombre)* |
| **Líder del proyecto** | *(completar)* |
| **Compilador** | GNU-GCC TDM-GCC-64 `x86_64-w64-mingw32-g++.exe` (Windows) · `clang++ -std=c++17` (macOS) |

---

## 1. Organización del código fuente

```
trabajo-practico-1/
├── trabajo-practico-1.cpp        BLOQUE PRINCIPAL. Sólo incluye y llama al módulo.
│
├── utilidades/                   herramientas transversales
│   ├── compatibilidad.hpp        todo lo que depende del sistema operativo
│   ├── constantes.hpp            constantes con nombre (dimensiones, colores, tasas)
│   ├── tipos.hpp                 registros y typedefs de cadenas
│   ├── prototipos.hpp            cabeceras de todos los módulos
│   ├── consola.hpp               namespace Screen (versión portable de ModulosHB)
│   ├── fechas.hpp                namespace FechaHora (GetDate/GetTime de la cátedra)
│   ├── validaciones.hpp          namespace Validar (ingreso validado de datos)
│   ├── ordenamiento.hpp          namespace Ordenar (burbuja, binaria, secuencial…)
│   ├── archivos.hpp              namespace Archivo (acceso secuencial y aleatorio)
│   ├── datos.hpp                 namespace Datos (los 5 usuarios y la muestra)
│   └── operaciones.hpp           namespace Operacion (lógica común del negocio)
│
├── modulos/                      un archivo por módulo Menu_opc()
│   ├── SistemaHomeBanking.hpp    MenuLogin.hpp    MenuPpal.hpp
│   ├── Menu_MisCuentas.hpp       Menu_Movimientos.hpp     Menu_Transferir.hpp
│   ├── Menu_ActualizarMov.hpp    (actualiza componentes de los archivos)
│   ├── Menu_Deposito.hpp         Menu_Compras.hpp         Menu_Tarjetas.hpp
│   ├── Menu_PlazoFijo.hpp        Menu_CompraVentaDolares.hpp
│   ├── Menu_Recargar.hpp         Menu_GenToken.hpp        Menu_MostrarCBU.hpp
│   ├── Menu_AgendaCBU.hpp        Menu_DatosPers.hpp       Menu_ModifClave.hpp
│   ├── Menu_NuevaCuenta.hpp      Menu_Listados.hpp        Menu_Logout.hpp
│
├── lib/lib.cpp                   ModulosHB.cpp original de la cátedra (referencia)
│
├── armar-entrega.py              genera el .cpp UNICO de la entrega
├── diseno.md                     este documento
├── MovimientosCA.Txt             26 registros de Caja de Ahorro
├── MovimientosTD.Txt             17 registros de Tarjeta de Débito
├── MovimientosTC.Txt             17 registros de Tarjeta de Crédito
└── ListadosHB.Txt                salida generada por Menu_Listados()
```

Todo se compila como **una sola unidad de compilación**: `trabajo-practico-1.cpp`
incluye los `.hpp` en el orden correcto. Para la entrega en Code::Blocks basta
con compilar ese único archivo.

```
clang++ -std=c++17 -Wall -Wextra trabajo-practico-1.cpp -o homebanking     (macOS)
g++     -std=c++17 -Wall -Wextra trabajo-practico-1.cpp -o homebanking.exe (Windows)
```

---

## 2. Bloque principal (nivel 0)

```
┌──────────────────────────────────────────┐
│  main()                                  │
│  ┌────────────────────────────────────┐  │
│  │  SistemaHomeBanking();             │  │
│  │  return 0;                         │  │
│  └────────────────────────────────────┘  │
└──────────────────────────────────────────┘
```

El bloque principal **sólo** contiene la invocación al módulo, como exige el
enunciado.

---

## 3. Estrategia de `SistemaHomeBanking()` (nivel 1)

```
SistemaHomeBanking()
│
├─ srand(time(NULL))                     semilla de los tokens al azar
├─ PrepararConsola()                     UTF-8, título, historial de teclas
├─ Screen::ModoTextoPlano(…)             según la salida sea consola o archivo
│
├─ Datos::CargarDatosPersonales(aDatosPers)   ← los 5 usuarios, array de registros
├─ Datos::GenerarMuestraDatos()               ← crea los .Txt si no existen
├─ PantallaBienvenida()
│
└─ REPETIR (0-x)
   ├─ MenuLogin(aDatosPers, usuario) ──────────► devuelve el usuario que ingresó
   │
   └─ SI usuario ≠ NO_ENCONTRADO
      └─ SesionUsuario(aDatosPers, usuario)
         ├─ Operacion::RecalcularSaldoCA(...)   saldo leído del archivo
         └─ REPETIR (0-x)
            ├─ opcion ← MenuPpal(aDatosPers, usuario)
            └─ SEGÚN opcion HACER  ◄── selección múltiple
                 1 → Menu_MisCuentas()          10 → Menu_GenToken()
                 2 → Menu_Movimientos()         11 → Menu_MostrarCBU()
                 3 → Menu_Transferir()          12 → Menu_AgendaCBU()
                 4 → Menu_Deposito()            13 → Menu_DatosPers()
                 5 → Menu_Compras()             14 → Menu_ModifClave()
                 6 → Menu_Tarjetas()            15 → Menu_NuevaCuenta()
                 7 → Menu_PlazoFijo()           16 → Menu_Listados()
                 8 → Menu_CompraVentaDolares()  17 → Menu_Logout()
                 9 → Menu_Recargar()
            MIENTRAS no se cierre la sesión
      MIENTRAS el usuario quiera iniciar otra sesión
```

---

## 4. Jerarquía completa de módulos

```
NIVEL 0   main()
NIVEL 1   └── SistemaHomeBanking()
              ├── MenuLogin(aDatosPers, usuario) : short
              └── MenuPpal(aDatosPers, nroUsu)   : short
NIVEL 2       ├── Menu_MisCuentas()
NIVEL 3       │     ├── Menu_CuentasEnPesos()
              │     └── Menu_CuentasEnDolares()
NIVEL 2       ├── Menu_Movimientos()
NIVEL 3       │     ├── Menu_MovimientosCA()          ← acceso secuencial
              │     ├── Menu_MovimientosTD()
              │     ├── Menu_MovimientosTC()
              │     ├── Menu_BuscarMovimientoCA()     ← ACCESO ALEATORIO
              │     └── Menu_EditarDetalleCA()        ← ACTUALIZA COMPONENTE
NIVEL 2       ├── Menu_Transferir()
NIVEL 3       │     ├── Menu_TransferirCBU()          ← binaria por alias
              │     └── Menu_TransferirCelular()      ← secuencial por celular
NIVEL 2       ├── Menu_Deposito()
              ├── Menu_Compras()
              ├── Menu_Tarjetas()
NIVEL 3       │     ├── Menu_ResumenTarjetas()
NIVEL 3       │     └── Menu_PagarCuotaTC()           ← ACTUALIZA COMPONENTE
NIVEL 2       ├── Menu_PlazoFijo()
NIVEL 3       │     ├── Menu_SimulacionPF()
              │     ├── Menu_InversionPF()
              │     └── Menu_FondoInversion()
NIVEL 2       ├── Menu_CompraVentaDolares()
NIVEL 3       │     ├── Menu_ComprarDolares()
              │     └── Menu_VenderDolares()
NIVEL 2       ├── Menu_Recargar()
NIVEL 3       │     ├── Menu_RecargaCelular()
              │     └── Menu_RecargaSube()
NIVEL 2       ├── Menu_GenToken()
              ├── Menu_MostrarCBU()
              ├── Menu_AgendaCBU()
NIVEL 3       │     ├── Menu_AgendarCBU()             ← INSERCIÓN EN ORDEN
              │     ├── Menu_BuscarAlias()            ← BÚSQUEDA BINARIA
              │     └── Menu_EliminarCBU()
NIVEL 2       ├── Menu_DatosPers()
              ├── Menu_ModifClave()
              ├── Menu_NuevaCuenta()
              ├── Menu_Listados()                     ← freopen a ListadosHB.Txt
              └── Menu_Logout() : bool
```

### 4.1 Cabeceras de los módulos

| Módulo | Cabecera | Devuelve |
|---|---|---|
| Sistema | `void SistemaHomeBanking()` | — |
| Login | `short MenuLogin(DatosPers aDatosPers[], short &usuario)` | posición del usuario o `NO_ENCONTRADO` |
| Menú principal | `short MenuPpal(DatosPers aDatosPers[], short nroUsu)` | opción elegida (`OPC_*`) |
| Logout | `bool Menu_Logout(DatosPers aDatosPers[], short nroUsu)` | `true` si cierra la sesión |
| Los demás | `void Menu_opc(DatosPers aDatosPers[], short nroUsu)` | — |

Todos reciben el **array de registros de datos personales** y el **índice del
usuario** de la sesión (0…4). El campo `aDatosPers[nroUsu].nroUsuario` (1…5) es
el que identifica al usuario dentro de los archivos.

### 4.2 Cuerpo típico de un módulo

**Submenú (nivel 2 que despacha a nivel 3)**

```
Menu_opc(aDatosPers, nroUsu)
│  aMenu ← {"opción 1", "opción 2", …, "Volver al menú principal"}
└─ REPETIR (0-x)
   ├─ linMenu ← Operacion::PantallaMenu(título, aDatosPers, nroUsu)
   ├─ opcion  ← Screen::MenuNavegar(aMenu, cantOpc, linMenu, COL_IZQ)
   └─ SEGÚN opcion HACER
        0 → Menu_subopcion1(…)
        1 → Menu_subopcion2(…)
        otro → volver ← verdadero        (también cubre ESC y opción inválida)
   MIENTRAS no volver Y no se agotó la entrada
```

**Pantalla de operación (nivel 3)**

```
Menu_opc(aDatosPers, nroUsu)
├─ Operacion::PantallaOperacion(título, aDatosPers, nroUsu)
├─ mostrar el saldo y los datos que el usuario necesita para decidir
├─ pedir los datos con Validar::Leer*   ← cada uno con su ciclo 1-x de validación
├─ SI se agotó la entrada → salir
├─ validar la regla del negocio (saldo suficiente, tope, cuenta existente…)
├─ pedir confirmación   (Validar::LeerSiNo)
├─ SI el importe supera TOPE_SIN_TOKEN → Operacion::AutorizarOperacion (token)
├─ registrar el movimiento (Operacion::RegistrarMovimiento*)
├─ emitir el comprobante con rótulos y el saldo resultante
└─ Operacion::Continuar()
```

---

## 5. Estructuras de datos

### 5.1 `Fecha` — 6 bytes

```
        ┌────────┬────────┬────────┐
Fecha   │  dia   │  mes   │  anio  │
        │ short  │ short  │ short  │
        │ 2 by   │ 2 by   │ 2 by   │
        └────────┴────────┴────────┘
          1..31    1..12   1900..2100
```

Se genera con `FechaHora::FechaDeHoy()` o con `Validar::LeerFecha()`.
Estado inicial: la fecha del sistema.
`FechaHora::FechaLarga(f)` la convierte en `aaaammdd` para comparar y ordenar.

### 5.2 `Cuenta` — 56 bytes

```
         ┌──────────────────┬──────┬────────┬─────────────┬──────────┬────────┐
Cuenta   │ nroCta           │ tipo │ moneda │ saldoInicial│  saldo   │ estado │
         │ str20            │ str3 │ str4   │  double     │  double  │  char  │
         │ 21 by            │ 4 by │ 4 by   │  8 by       │  8 by    │  1 by  │
         └──────────────────┴──────┴────────┴─────────────┴──────────┴────────┘
          "106-4235689707-3"  "CA"   "$"                              'A' | 'I'
                              "CC"   "u$s"
                                          (+ 11 bytes de relleno de alineación)
```

Estado inicial: cargada en `Datos::CargarDatosPersonales()`. Para la Caja de
Ahorro en pesos, `saldo` se **recalcula** como
`saldoInicial + Σ movimientos del archivo`, de modo que archivo y pantalla nunca
queden desfasados.

### 5.3 `Contacto` (CBU agendado) — 132 bytes

```
           ┌──────────┬────────────────┬────────────┬─────────┬──────────┐
Contacto   │ alias    │ apellidoNombre │ cbu        │ celular │ banco    │
           │ str25    │ str40          │ str22      │ str15   │ str25    │
           │ 26 by    │ 41 by          │ 23 by      │ 16 by   │ 26 by    │
           └──────────┴────────────────┴────────────┴─────────┴──────────┘
            "mama"     "ALVAREZ ELSA…"  22 dígitos
```

Estado inicial: 3 ó 4 contactos por usuario, **ordenados por alias**. Ese orden
es el invariante que habilita la búsqueda binaria; se conserva porque las altas
se hacen con `Ordenar::InsertarContactoEnOrden()` y las bajas compactan el array.

### 5.4 `DatosPers` — 2272 bytes · **estructura combinada**

```
            ┌───────────────────────────────────────────────────────────────┐
DatosPers   │ dni             long                                    8 by  │
            │ apellidoNombre  str40                                  41 by  │
            │ fechaNac        ┌───────────────────────────┐           6 by  │  ◄ registro
            │                 │ Fecha: dia │ mes │ anio   │                 │    dentro de
            │                 └───────────────────────────┘                 │    registro
            │ usuario         str15                                  16 by  │
            │ clave           str15                                  16 by  │
            │ celular         str15                                  16 by  │
            │ email           str40                                  41 by  │
            │ domicilio       str40                                  41 by  │
            │ nroCtaCA        str20                                  21 by  │
            │ cbu             str22                                  23 by  │
            │                                                               │
            │ aCuentas    ┌───────┬───────┬─────┬───────┐          448 by   │  ◄ array de
            │  [8]        │Cuenta0│Cuenta1│ ... │Cuenta7│                   │    registros
            │             └───────┴───────┴─────┴───────┘                   │    dentro de
            │ cantCuentas     short  (ocupación real)                2 by   │    un registro
            │                                                               │
            │ aAgenda     ┌────────┬────────┬─────┬─────────┐      1584 by  │  ◄ ídem
            │  [12]       │Contact0│Contact1│ ... │Contact11│               │
            │             └────────┴────────┴─────┴─────────┘               │
            │ cantAgenda      short  (ocupación real)                2 by   │
            │ nroUsuario      short  1..5                            2 by   │
            └───────────────────────────────────────────────────────────────┘
                                                          (+5 by de relleno)
```

**El array del sistema:**

```
                     0            1            2            3            4
              ┌────────────┬────────────┬────────────┬────────────┬────────────┐
aDatosPers    │ ALVAREZ    │ BENITEZ    │ CANTERO    │ DOMINGUEZ  │ ESPINDOLA  │
[MAX_USUARIOS]│ 2272 bytes │ 2272 bytes │ 2272 bytes │ 2272 bytes │ 2272 bytes │
              └────────────┴────────────┴────────────┴────────────┴────────────┘
              nroUsuario=1        =2           =3           =4           =5

              MAX_USUARIOS = 5   →   total 11.360 bytes
```

*Cómo se genera:* `Datos::CargarDatosPersonales(aDatosPers)` la carga por
completo al arrancar, con los datos escritos dentro del propio código, como pide
el enunciado. *Estado inicial:* los 5 usuarios con 2 cuentas cada uno
(CA en $ y CA en u$s) y su agenda de CBU ordenada por alias.

### 5.5 Registros de los archivos

```
             ┌───────┬─────────┬─────────┬─────────┬────────────┬───────┬────────┐
MovCA        │ fecha │ tipoMov │ detalle │ importe │ nroUsuario │ saldo │ nroReg │
(72 by en    │ Fecha │  char   │  str25  │ double  │   short    │double │ short  │
 memoria)    │ 6 by  │  1 by   │  26 by  │  8 by   │   2 by     │ 8 by  │ 2 by   │
             └───────┴─────────┴─────────┴─────────┴────────────┴───────┴────────┘
                                                                  └──── derivados ────┘
                                                                  no se graban

             ┌───────┬─────────┬─────────┬────────────┬────────┐
MovTD        │ fecha │ detalle │ importe │ nroUsuario │ nroReg │
(48 by)      └───────┴─────────┴─────────┴────────────┴────────┘

             ┌───────┬─────────┬────────┬──────────┬─────────┬────────────┬────────┐
MovTC        │ fecha │ detalle │ cuotas │ nroCpbte │ importe │ nroUsuario │ nroReg │
(64 by)      │       │  str25  │  str5  │   long   │ double  │            │        │
             └───────┴─────────┴────────┴──────────┴─────────┴────────────┴────────┘
```

**Arrays de trabajo** (variables locales de los módulos que listan movimientos):

| Array | Dimensión | Tamaño |
|---|---|---|
| `MovCA aMov[MAX_MOV_CA]` | 300 | 21.600 bytes |
| `MovTD aMov[MAX_MOV_TD]` | 150 | 7.200 bytes |
| `MovTC aMov[MAX_MOV_TC]` | 150 | 9.600 bytes |

*Cómo se generan:* `Archivo::LeerMovimientos*()` los llena leyendo el archivo en
forma secuencial y filtrando por el número de usuario. *Estado inicial:* vacíos
(`cant = 0`).

---

## 6. Formato de los archivos de texto

Los tres archivos son **tabulares y de registro fijo**: los números se ajustan a
la derecha, las cadenas a la izquierda rellenando con espacios en blanco. **No
llevan encabezados**: contienen solamente los datos, tal como pide el enunciado.

Que todos los registros ocupen la misma cantidad de bytes es lo que permite el
**acceso aleatorio**: el registro *n* empieza en el byte `n × largoRegistro`.

### 6.1 `MovimientosCA.Txt` — 58 bytes + salto de línea

```
 columna: 1234567890 1 2 3456789012345678901234567890 1 234567890123 1 4
          ┌────────┐   ┌┐  ┌────────────────────────┐   ┌──────────┐   ┌┐
          │ fecha  │ ␣ ││ ␣│        detalle         │ ␣ │ importe  │ ␣ ││
          └────────┘   └┘  └────────────────────────┘   └──────────┘   └┘
            10 col     1        30 col                     12 col      1
          99-99-9999   D/H      X(30) a izquierda        9(7).99      1..5
                                                       a la derecha   usuario

 1- 4-2026 D Sueldo Marzo 2026                1542367.21 1
 1- 4-2026 H Combustible YPF                    35983.68 1
```

### 6.2 `MovimientosTD.Txt` — 56 bytes + salto de línea

```
          ┌────────┐   ┌────────────────────────┐   ┌──────────┐   ┌┐
          │ fecha  │ ␣ │  detalle (comercio)    │ ␣ │ importe  │ ␣ ││
          └────────┘   └────────────────────────┘   └──────────┘   └┘
            10 col            30 col                   12 col       1

 5- 3-2026 Casa de Pastas La Juvenil          38412.25 1
12- 3-2026 Pescaderia El Pez Cado             10649.09 1
```

### 6.3 `MovimientosTC.Txt` — 71 bytes + salto de línea

```
          ┌────────┐  ┌──────────────────────┐  ┌───┐  ┌──────┐  ┌────────┐  ┌┐
          │ fecha  │␣ │ detalle (comercio)   │␣ │cuo│␣ │ cpbte│␣ │ importe│␣ ││
          └────────┘  └──────────────────────┘  └───┘  └──────┘  └────────┘  └┘
            10 col          30 col               5 col   8 col     12 col     1

 3- 2-2026 Pizzeria Kentucky                1/3   245684     29567.73 1
14- 2-2026 Panaderia Buen Dia                                58323.84 1
```

Un número de comprobante en cero y unas cuotas vacías significan «sin
comprobante» y «pago en una sola cuota»: se graban en blanco, respetando el
ancho de la columna, exactamente como en el ejemplo del enunciado.

### 6.4 Decisiones sobre el formato

Dos aspectos del formato merecen explicación aparte —la columna `nroUsuario` y
el uso de `double` para los importes—, junto con la tolerancia a los archivos
preparados a mano. Están detallados en la sección **12. Decisiones de diseño y
limitaciones conocidas**.

---

## 7. Operaciones clásicas aplicadas

| Operación | Dónde | Sobre qué | Por qué ahí |
|---|---|---|---|
| **Burbuja optimizada** | `Ordenar::MovCAporFecha`, `MovTDporFecha`, `MovTCporFecha`, `MovTDporImporteDesc`, `AgendaPorAlias` | arrays de movimientos y agenda | los movimientos se graban en el orden en que se producen; para listarlos hay que ordenarlos por fecha. El ciclo externo es 1-x y corta apenas una pasada no produce intercambios; el interno reduce su límite en cada pasada. |
| **Inserción en orden** | `Ordenar::InsertarContactoEnOrden` | agenda de CBU | mantiene el array ordenado por alias sin volver a ordenarlo, con lo que la búsqueda binaria sigue siendo válida después de cada alta. |
| **Búsqueda binaria** | `Ordenar::BinariaAlias` | agenda de CBU | el array **está ordenado por alias**: log₂(n) comparaciones en lugar de n. |
| **Búsqueda secuencial** | `Ordenar::SecuencialDni`, `SecuencialCBU`, `SecuencialCelular`, `SecuencialCuenta`, `SecuencialCuentaPorTipo` | usuarios, agenda por CBU/celular, cuentas | esas claves **no** están ordenadas, de modo que no queda más remedio que recorrer con corte. |
| **Acceso secuencial** | `Archivo::LeerMovimientos*` (lectura completa) y `AgregarMovimiento*` (alta al final) | los tres `.Txt` | recorrido de principio a fin y agregado de nuevas componentes sin destruir lo existente. |
| **Acceso aleatorio** | `Archivo::LeerRegistro*` (`seekg` a `nroReg × largoRegistro`) | los tres `.Txt` | permite leer una componente sin recorrer las anteriores. Lo ejercita `Menu_BuscarMovimientoCA()`. |
| **Actualizar componentes** | `Archivo::ActualizarRegistroCA/TC` (`seekp` + sobrescritura en el lugar) | `MovimientosCA.Txt`, `MovimientosTC.Txt` | `Menu_EditarDetalleCA()` corrige la descripción de un movimiento y `Menu_PagarCuotaTC()` avanza el contador de cuotas («2/6» → «3/6»). Sólo es posible porque los registros son de largo fijo. |

---

## 8. Muestra de datos

### 8.1 Datos personales de los cinco usuarios

| Nº | DNI | Apellido y Nombre | F. Nac. | Usuario | Clave | Celular | Correo | Domicilio | Nro. Cta. CA | CBU | Saldo inicial CA $ | Saldo u$s |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 30125478 | ALVAREZ MARIA SOLEDAD | 14-03-1983 | malvarez | Sol1983 | 3414785296 | malvarez@correo.com | Av. Pellegrini 1450 - Rosario | 106-4235689707-3 | 1060099920000423568970 | 1250000.00 | 4820.55 |
| 2 | 27890456 | BENITEZ CARLOS ALBERTO | 02-11-1979 | cbenitez | Cba2024 | 3415236987 | cbenitez@correo.com | San Martin 785 - Santa Fe | 106-4235690112-8 | 1060099920000423569011 | 480000.00 | 12750.00 |
| 3 | 35478912 | CANTERO LUCIA BEATRIZ | 25-07-1990 | lcantero | Luc2590 | 3415874123 | lcantero@correo.com | Mitre 342 - Parana | 106-4235691345-5 | 1060099920000423569134 | 92500.00 | 980.20 |
| 4 | 41236789 | DOMINGUEZ JAVIER HORACIO | 09-01-1999 | jdominguez | Jav0199 | 3416987452 | jdominguez@correo.com | Bv. Orono 2130 - Rosario | 106-4235692578-1 | 1060099920000423569257 | 35800.00 | 150.00 |
| 5 | 23456781 | ESPINDOLA ROBERTO DANIEL | 18-05-1974 | respindola | Rob1874 | 3414123658 | respindola@correo.com | Rioja 1290 - Rosario | 106-4235693781-9 | 1060099920000423569378 | 2100000.00 | 35600.40 |

> Los datos son de fantasía: no corresponden a personas reales.

### 8.2 Cuentas de CBU agendadas

| Usuario | Alias | Titular | CBU | Banco |
|---|---|---|---|---|
| 1 | alquiler.depto | SOSA INMOBILIARIA SRL | 0720099950000078912345 | Banco Santander |
| 1 | gas.litoral | LITORAL GAS S.A. | 0170099940000012345678 | Banco Frances |
| 1 | juan.perez | PEREZ JUAN MANUEL | 0290099910000098765432 | Banco Nacion |
| 1 | mama | ALVAREZ ELSA NOEMI | 1910099930000045678912 | Banco Credicoop |
| 2 | ana.gomez | GOMEZ ANA CLARA | 0110099960000032165498 | Banco Nacion |
| 2 | hermano | BENITEZ MARTIN JOSE | 0340099980000014785236 | Banco Galicia |
| 2 | seguro.auto | PROTEGER SEGUROS S.A. | 0150099970000065498732 | Banco ICBC |
| 3 | escuela | COLEGIO SAN JOSE | 0140099910000025836914 | Banco Provincia |
| 3 | pablo.ruiz | RUIZ PABLO ESTEBAN | 0270099920000036925814 | Banco Supervielle |
| 3 | veterinaria | VETERINARIA HUELLAS SRL | 0450099930000041236587 | Banco Macro |
| 4 | expensas | CONSORCIO EDIFICIO ALTO | 0160099940000052147896 | Banco BBVA |
| 4 | gimnasio | SPORT CLUB ROSARIO S.A. | 0330099960000074185296 | Banco HSBC |
| 4 | sofia.luna | LUNA SOFIA BELEN | 0280099950000063258741 | Banco Patagonia |
| 5 | contador | MORALES ESTUDIO CONTABLE | 0170099970000085296314 | Banco Frances |
| 5 | nieta | ESPINDOLA CAMILA ROCIO | 0290099980000096325874 | Banco Nacion |
| 5 | prepaga.salud | SALUD INTEGRAL S.A. | 0200099910000017539514 | Banco Ciudad |

### 8.3 `MovimientosCA.Txt` — 26 registros

| Fecha | T | Detalle | Importe | Usu. |
|---|:-:|---|---:|:-:|
| 01-04-2026 | D | Sueldo Marzo 2026 | 1542367.21 | 1 |
| 01-04-2026 | H | Combustible YPF | 35983.68 | 1 |
| 03-04-2026 | H | Kiosco Las 5 Esquinas | 8952.03 | 1 |
| 10-04-2026 | D | Transferencia por Serv. | 156236.40 | 1 |
| 11-04-2026 | H | Restaurant Siga la Vaca | 76591.31 | 1 |
| 15-04-2026 | H | Pago Tarjeta de Credito | 245680.55 | 1 |
| 02-04-2026 | D | Sueldo Marzo 2026 | 987450.30 | 2 |
| 04-04-2026 | H | Supermercado Kotto | 125634.87 | 2 |
| 08-04-2026 | H | Recarga Celular | 12000.00 | 2 |
| 12-04-2026 | D | Plazo Fijo acreditado | 315780.44 | 2 |
| 20-04-2026 | H | Alquiler Departamento | 420000.00 | 2 |
| 03-04-2026 | D | Sueldo Marzo 2026 | 743210.15 | 3 |
| 05-04-2026 | H | Farmacia Del Centro | 34567.20 | 3 |
| 09-04-2026 | H | Recarga SUBE | 5000.00 | 3 |
| 14-04-2026 | H | Libreria Alfa-Omega | 9782.67 | 3 |
| 22-04-2026 | D | Venta de dolares | 195050.00 | 3 |
| 01-04-2026 | D | Sueldo Marzo 2026 | 612345.90 | 4 |
| 06-04-2026 | H | Pescaderia El Pez Cado | 106491.09 | 4 |
| 13-04-2026 | H | Casa de Pastas La Juvenil | 38412.25 | 4 |
| 18-04-2026 | D | Transferencia recibida | 87500.00 | 4 |
| 25-04-2026 | H | Compra Fondo Comun | 150000.00 | 4 |
| 02-04-2026 | D | Jubilacion Marzo 2026 | 458900.75 | 5 |
| 07-04-2026 | H | Pago Servicios EPE | 62340.18 | 5 |
| 16-04-2026 | H | Panaderia Buen Dia | 15230.84 | 5 |
| 21-04-2026 | D | Plazo Fijo acreditado | 1102450.60 | 5 |
| 28-04-2026 | H | Ferreteria Black Decker | 85440.25 | 5 |

### 8.4 `MovimientosTD.Txt` — 17 registros

| Fecha | Comercio | Importe | Usu. |
|---|---|---:|:-:|
| 05-03-2026 | Casa de Pastas La Juvenil | 38412.25 | 1 |
| 12-03-2026 | Pescaderia El Pez Cado | 10649.09 | 1 |
| 15-03-2026 | Supermercado Kotto | 25632.94 | 1 |
| 29-03-2026 | Libreria Alfa-Omega | 9782.67 | 1 |
| 03-03-2026 | Farmacia Del Centro | 8450.30 | 2 |
| 11-03-2026 | Estacion Servicio Shell | 32780.55 | 2 |
| 22-03-2026 | Verduleria La Huerta | 6320.40 | 2 |
| 07-03-2026 | Perfumeria Rouge | 14990.00 | 3 |
| 18-03-2026 | Panaderia Buen Dia | 4780.25 | 3 |
| 27-03-2026 | Cine Monumental | 12500.00 | 3 |
| 02-03-2026 | Rotiseria Don Pepe | 7650.80 | 4 |
| 14-03-2026 | Kiosco Las 5 Esquinas | 3250.00 | 4 |
| 25-03-2026 | Heladeria Vittorio | 9100.45 | 4 |
| 06-03-2026 | Ferreteria Black Decker | 45320.90 | 5 |
| 16-03-2026 | Optica Vision Total | 78400.00 | 5 |
| 24-03-2026 | Vinoteca El Descorche | 23150.75 | 5 |
| 30-03-2026 | Bazar La Fortuna | 18600.00 | 5 |

### 8.5 `MovimientosTC.Txt` — 17 registros

| Fecha | Comercio | Cuotas | Nro.Cpbte. | Importe | Usu. |
|---|---|:-:|---:|---:|:-:|
| 03-02-2026 | Pizzeria Kentucky | 1/3 | 245684 | 29567.73 | 1 |
| 14-02-2026 | Panaderia Buen Dia | | | 58323.84 | 1 |
| 19-02-2026 | Combustible YPF | | 37485 | 36491.09 | 1 |
| 26-02-2026 | Kit Herramientas Black | 2/3 | 162395 | 85440.25 | 1 |
| 05-02-2026 | Indumentaria Zara | 3/6 | 118420 | 64200.00 | 2 |
| 17-02-2026 | Farmacia Del Centro | 1/1 | 220145 | 18750.30 | 2 |
| 28-02-2026 | Electro Hogar Sur | 6/12 | 305471 | 152300.90 | 2 |
| 08-02-2026 | Libreria Alfa-Omega | 1/3 | 411256 | 27600.45 | 3 |
| 20-02-2026 | Cine Monumental | | | 12500.00 | 3 |
| 25-02-2026 | Viaje Aerolineas | 4/12 | 508233 | 348900.00 | 3 |
| 02-02-2026 | Supermercado Kotto | 2/3 | 601478 | 45300.20 | 4 |
| 13-02-2026 | Deportes Full Sport | 1/6 | 617392 | 89750.60 | 4 |
| 23-02-2026 | Rotiseria Don Pepe | | | 7650.80 | 4 |
| 04-02-2026 | Optica Vision Total | 3/12 | 702584 | 78400.00 | 5 |
| 11-02-2026 | Vinoteca El Descorche | 1/3 | 715039 | 23150.75 | 5 |
| 19-02-2026 | Neumaticos Fate | 6/18 | 728461 | 312450.55 | 5 |
| 27-02-2026 | Bazar La Fortuna | | | 18600.00 | 5 |

---

## 9. Resultados esperados

Con la muestra anterior, los saldos que el sistema debe calcular son:

| Usuario | Saldo inicial | Σ 'D' (acredita) | Σ 'H' (debita) | **Saldo final CA $** | Saldo u$s |
|---|---:|---:|---:|---:|---:|
| 1 ALVAREZ | 1250000.00 | 1698603.61 | 367207.57 | **2581396.04** | 4820.55 |
| 2 BENITEZ | 480000.00 | 1303230.74 | 557634.87 | **1225595.87** | 12750.00 |
| 3 CANTERO | 92500.00 | 938260.15 | 49349.87 | **981410.28** | 980.20 |
| 4 DOMINGUEZ | 35800.00 | 699845.90 | 294903.34 | **440742.56** | 150.00 |
| 5 ESPINDOLA | 2100000.00 | 1561351.35 | 163011.27 | **3498340.08** | 35600.40 |

### 9.1 Movimientos de la Caja de Ahorro (usuario 1)

```
  Cta.CA:  106-4235689707-3
  Saldo inicial $    1250000.00

  Fecha      T Detalle                           Debe        Haber         Saldo
  ------------------------------------------------------------------------------
   1- 4-2026 D Sueldo Marzo 2026           1542367.21                 2792367.21
   1- 4-2026 H Combustible YPF                            35983.68    2756383.53
   3- 4-2026 H Kiosco Las 5 Esquinas                       8952.03    2747431.50
  10- 4-2026 D Transferencia por Serv.      156236.40                 2903667.90
  11- 4-2026 H Restaurant Siga la Vaca                    76591.31    2827076.59
  15- 4-2026 H Pago Tarjeta de Credito                   245680.55    2581396.04
  ------------------------------------------------------------------------------
               Totales                     1698603.61    367207.57    2581396.04

  Cantidad de movimientos: 6
```

### 9.2 Movimientos de Tarjeta de Crédito (usuario 1)

```
  Fecha      Comercio                       Cuotas Nro.Cpbte.       Importe
  -------------------------------------------------------------------------
   3- 2-2026 Pizzeria Kentucky                 1/3     245684      29567.73
  14- 2-2026 Panaderia Buen Dia                                    58323.84
  19- 2-2026 Combustible YPF                            37485      36491.09
  26- 2-2026 Kit Herramientas Black            2/3     162395      85440.25
  -------------------------------------------------------------------------
                                                        Total     209822.91

  Cantidad de consumos: 4
```

### 9.3 Movimientos de Tarjeta de Débito (usuario 3)

```
  Fecha      Comercio                             Importe
  -------------------------------------------------------
   7- 3-2026 Perfumeria Rouge                    14990.00
  18- 3-2026 Panaderia Buen Dia                   4780.25
  27- 3-2026 Cine Monumental                     12500.00
  -------------------------------------------------------
                                      Total      32270.25

  Cantidad de consumos: 3
```

### 9.4 Acceso aleatorio a una componente

```
  Archivo....................: MovimientosCA.Txt
  Cantidad de registros......: 26
  Largo del registro (bytes).: 59
  Su nro. de usuario.........: 3

  Nro. de registro a consultar: 3

  Nro. de registro pedido....: 3
  Nro. de registro (base 0)..: 2
  Fecha......................: 03-04-2026
  Fecha en letras............: 3 de Abril de 2026
  Tipo de movimiento.........: H   (Haber: debita el saldo)
  Detalle....................: Kiosco Las 5 Esquinas
  Importe....................: $         8952.03
  Nro. de usuario duenio.....: 1
  Titular....................: ALVAREZ MARIA SOLEDAD
```

### 9.5 Simulación de plazo fijo

Con interés simple `interés = capital × (TNA / 100) × días / 365` y
`TASA_PF_ANUAL = 37,50 %`, para un capital de $500.000:

```
     Plazo   Vencimiento           Interes      Total a cobrar
  ------------------------------------------------------------
        30    31-08-2026          15410.96           515410.96
        60    30-09-2026          30821.92           530821.92
        90    30-10-2026          46232.88           546232.88
       180    28-01-2027          92465.75           592465.75
       365    01-08-2027         187500.00           687500.00
  ------------------------------------------------------------
```

### 9.6 Creación de una cuenta nueva

```
  [OK] Cuenta creada nro. 009-0000011
```

---

## 10. Parámetros del negocio

| Constante | Valor | Uso |
|---|---:|---|
| `TASA_PF_ANUAL` | 37,50 % | TNA del plazo fijo tradicional |
| `TASA_PF_UVA` | 28,00 % | TNA del plazo fijo UVA |
| `TASA_FCI_ANUAL` | 32,75 % | rendimiento estimado del fondo común |
| `COTIZ_DOLAR_COMPRA` | $ 1010,50 | precio al que el **banco vende** al cliente |
| `COTIZ_DOLAR_VENTA` | $ 975,25 | precio al que el **banco compra** al cliente |
| `MIN_PLAZO_FIJO` | $ 10.000 | capital mínimo de un plazo fijo |
| `MIN_FONDO_INV` | $ 5.000 | capital mínimo del fondo común |
| `TOPE_SIN_TOKEN` | $ 100.000 | por encima de este importe se exige token |
| `TOPE_TRANSF_DIARIO` | $ 5.000.000 | tope diario de transferencias |
| `MIN/MAX_RECARGA_CEL` | $ 500 / $ 50.000 | recarga de celular |
| `MIN/MAX_RECARGA_SUBE` | $ 300 / $ 30.000 | recarga SUBE |
| `DIAS_PF_MINIMO/MAXIMO` | 30 / 365 | plazo admitido |
| `MAX_CUOTAS_TC` | 18 | cuotas máximas de una compra con crédito |
| `INTENTOS_MAX` | 3 | intentos de login y de token |
| `TOKEN_MINIMO/MAXIMO` | 100000 / 999999 | token de 6 dígitos al azar |

---

## 11. Cumplimiento del enunciado

| Requisito | Dónde se resuelve |
|---|---|
| Login con DNI, usuario y clave, cinco usuarios | `MenuLogin()` + `Datos::CargarDatosPersonales()` |
| Caja de Ahorro en pesos y en dólares | `Menu_MisCuentas()`, `Menu_CuentasEnPesos/EnDolares()` |
| Transferir a CBU / alias | `Menu_TransferirCBU()` |
| Transferir a celular | `Menu_TransferirCelular()` |
| Tarjeta de Crédito y de Débito con sus archivos | `Menu_Tarjetas()`, `MovimientosTC.Txt`, `MovimientosTD.Txt` |
| Simulación de plazo fijo | `Menu_SimulacionPF()` |
| Compra / venta de dólares | `Menu_ComprarDolares()`, `Menu_VenderDolares()` |
| Inversión en plazo fijo | `Menu_InversionPF()` |
| Fondo de inversión | `Menu_FondoInversion()` |
| Recarga de celular y de SUBE | `Menu_RecargaCelular()`, `Menu_RecargaSube()` |
| Token al azar de 6 dígitos | `Operacion::GenerarToken()`, `Menu_GenToken()` |
| Mostrar el CBU de 22 dígitos | `Menu_MostrarCBU()` |
| Mis Cuentas (nro., tipo, moneda, saldo, estado) | `Operacion::ListarCuentas()` |
| Datos personales (registro + array, 5 usuarios) | `record DatosPers`, `aDatosPers[MAX_USUARIOS]` |
| Modificar contraseña con sus mensajes | `Menu_ModifClave()` |
| Movimientos de CA (fecha, tipo, detalle, importe, saldo) | `Operacion::TablaCA()` |
| Movimientos de TD (fecha, detalle, importe) | `Operacion::TablaTD()` |
| Movimientos de TC (fecha, detalle, cuotas x/y, importe) | `Operacion::TablaTC()` |
| Crear cuenta nueva con nro. correlativo | `Menu_NuevaCuenta()`, `Operacion::NuevoNroCuenta()` |
| Depósito (fecha, descripción, importe) | `Menu_Deposito()` |
| Compras (fecha, descripción, importe, comercio, TD/TC) | `Menu_Compras()` |
| Logout | `Menu_Logout()` |
| CBU agendados + agendar nuevos | `Menu_AgendaCBU()`, `Menu_AgendarCBU()` |
| ≥ 15 registros por archivo | 26 / 17 / 17 |
| Bloque principal sólo con invocaciones | `trabajo-practico-1.cpp` |
| Memoria dinámica = 0 bytes | sólo arrays y registros estáticos |
| `freopen` hacia `ListadosHB.Txt` | `Menu_Listados()` |
| Funciones con el prefijo `Menu_opc()` | `modulos/` |

---

## 12. Decisiones de diseño y limitaciones conocidas

**a) Los importes se manejan en `double`.** El formato `9(7).99` admite hasta
nueve dígitos significativos y un `float` sólo garantiza siete: con `float`,
`1542367.21` se almacena como `1542367.25` y el error se arrastra en cada
acumulación de saldos.

**b) Se agregó la columna `nroUsuario` al final de cada registro.** El enunciado
permite un solo juego de archivos de movimientos pero exige cinco usuarios.
Ponerla como última columna deja intactas las columnas que el enunciado
especifica y permite que cada usuario tenga su propio saldo.

**c) Los detalles se normalizan a ASCII antes de grabarse.** En UTF-8 una letra
acentuada ocupa dos bytes; si se grabaran tal cual, los registros dejarían de
tener el mismo largo en bytes y el acceso aleatorio apuntaría a lugares
equivocados. `Validar::NormalizarASCII()` convierte «á» en «a», «ñ» en «n», etc.

**d) El saldo se recalcula recorriendo el archivo, no un array.** Un array
siempre tiene un tope, y superarlo daría un saldo mal calculado *en silencio*
que además autorizaría débitos por dinero inexistente.
`Archivo::AcumularMovimientosCA()` acumula leyendo el archivo completo.

**e) Los archivos toleran ser preparados con un editor de texto.** El enunciado
indica preparar la muestra de datos «utilizando un editor de texto plano», de
modo que el sistema contempla lo que un editor produce:

| Situación | Cómo se resuelve |
|---|---|
| El archivo usa **CR+LF** (editor de Windows) | `Archivo::TerminadorDeLinea()` lo detecta y las componentes nuevas se agregan con el **mismo** terminador, para que todos los registros sigan midiendo lo mismo. |
| La última línea quedó **sin salto de línea** | `Archivo::AsegurarFinDeLinea()` lo agrega antes de anexar, para que el registro nuevo no se pegue al anterior. `CantidadRegistros()` cuenta igual esa última componente. |
| El archivo tiene una **línea inválida** | El recorrido secuencial la saltea, pero `nroReg` sigue contando **posiciones físicas**: si contara sólo las válidas, el acceso aleatorio leería o actualizaría la componente equivocada. |
| El archivo empieza con un **renglón en blanco** | `LargoRegistro()` valida el largo detectado contra `LARGO_REG_*`; si no coincide devuelve 0 y el acceso aleatorio se desactiva en lugar de devolver basura. |

**f) Qué persiste y qué no.** La restricción «espacio en disco: sólo para
generar nuevas componentes a los archivos de movimientos» determina el alcance:

| Dato | ¿Persiste entre ejecuciones? |
|---|---|
| Movimientos de CA, TD y TC | **Sí**, en los tres `.Txt` |
| Saldo de la Caja de Ahorro en pesos | **Sí**: se recalcula desde `MovimientosCA.Txt` |
| Saldo en dólares | No: no hay archivo donde grabarlo |
| Contraseña modificada | No: los datos personales viven en el código |
| Cuentas creadas y CBU agendados | No: viven en el array de datos personales |

Los módulos avisan en pantalla cuando un cambio vale sólo para la sesión, para
no dar por persistido algo que no lo está.

**g) Sobre la compra con tarjeta de crédito.** El enunciado pide que la compra
«modifique agregando la compra en CA, y TD o TC». Se implementó literalmente:
la compra con crédito impacta en la Caja de Ahorro *y* en `MovimientosTC.Txt`.

---

## 13. Cómo probar el programa

**Sesión interactiva**

```
cd trabajos-practicos/trabajo-practico-1
clang++ -std=c++17 -Wall -Wextra trabajo-practico-1.cpp -o homebanking
./homebanking
```

En consola los menús se recorren con las **flechas ↑ ↓**, se confirma con
**ENTER** y se puede saltar a una opción tecleando su número. **ESC** vuelve.

**Sesión guionada** (para reproducir siempre la misma corrida)

```
printf '30125478\nmalvarez\nSol1983\n\n2\n1\n\n5\n17\nS\nN\n' | ./homebanking
```

Cuando la entrada no es una consola, los menús se eligen tecleando el número de
la opción, y el programa suprime colores y posicionamiento para que la
transcripción quede legible.

**Generar el archivo único de la entrega**

El enunciado pide entregar un solo archivo de código. `armar-entrega.py` lo
arma integrando todos los `.hpp` en el orden en que los incluye el bloque
principal:

```
python3 armar-entrega.py "TP1V1_K1023G3_PEREZ JUAN.cpp"
clang++ -std=c++17 -Wall -Wextra "TP1V1_K1023G3_PEREZ JUAN.cpp" -o entrega
```

Hay que volver a generarlo cada vez que se modifique el código. Se verificó que
la versión de un solo archivo produce exactamente la misma salida que la
versión modular en las 19 corridas de prueba.

---

## 14. Qué falta completar antes de entregar

1. El **encabezado** de `trabajo-practico-1.cpp`: fecha de entrega, curso, día,
   turno, número de grupo e integrantes (legajo, apellido y nombre).
2. La **carátula** y los datos del líder del proyecto en este documento.
3. El **nombre del archivo** de la entrega, con el formato
   `TP1V1_K1023G3_APELLIDO NOMBRE.cpp` (generarlo con `armar-entrega.py`).
