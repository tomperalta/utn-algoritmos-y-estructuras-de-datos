# Trabajo Práctico #1: Metodología-DP-ECP-Módulos-ED: Registro, Arreglo y Archivo texto

Su grupo es parte de un proyecto para construir una simulación de un sitio web de Home Banking que deberá ajustarse a un mínimo de acciones para diversas consultas y operaciones de los usuarios.

Se aconseja realizar una investigación y exploración de sitios web reales de distintos bancos, como así también preguntarle a la IA, como realizar un proyecto de Home Banking cuales podrían ser las opciones y profundizar en algunas de ellas, Plazo Fijo, Transferencia a Terceros, Movimientos de Cuentas de Débito y/o Crédito, Caja de Ahorros, Fondo de Inversión, etc.

A partir de ahí, se deberán construir las distintas pantallas para llevar a cabo las diferentes operaciones/consultas.

## Algunas de las operaciones/consultas a realizar, se detallan a continuación

- Login al sitio (ingresar DNI, usuario y clave, se deberá contemplar cinco usuarios).
- Cuentas:
  - Caja de Ahorros (CA) en pesos $. (Crear un archivo de texto con datos de los movimientos).
  - En dólares u$s.
- Transferir dinero:
  - A CBU, Alias. (Generar un nuevo movimiento que modifica el saldo en CA).
  - A Celular. (Generar un nuevo movimiento que modifica el saldo en CA).
- Tarjetas:
  - De Crédito (Crear un archivo de texto con datos de los movimientos).
  - De Débito (Crear un archivo de texto con datos de los movimientos).
- Simulación P.F.: (ingresar datos de capital, tiempo y la tasa actual ofrecida por el banco).
- Compra/Venta dólares: (ingresar datos del monto y tipo de la operación, modifica el saldo de la CA).
- Inversión Plazo Fijo: (Generar nuevo movimiento que modifica el saldo de la CA).
  - Fondo de inversión (se deberá ingresar el monto del Capital a invertir).
- Recargar:
  - Celular. (Generar un nuevo movimiento que modifica el saldo en CA).
  - Sube. (Generar un nuevo movimiento que modifica el saldo en CA).
- Generar Token: (Generar al azar con 6 dígitos, en aquellos casos que se requiera por seguridad).
- Mostrar el CBU del usuario: (es un número con 22 dígitos).
- Mis Cuentas: Datos bancarios (Nro.Cta, tipo <<CA, CC>>, moneda <<$, u$s>>, saldo, estado).
- Datos personales: (dni, ApellidoNombre, Fecha Nacimiento, Usuario, Clave, Nro. Celular, e-mail, Domicilio, Nro. Cuenta CA, nro. CBU). Diseñar el registro con estos campos, combinada con array para almacenar cinco usuarios.
- Modificar contraseña: (Debe solicitar contraseña actual, contraseña nueva, confirmar contraseña nueva, confirmar. Dar mensajes apropiados en caso de que NO EXISTA o si nueva contraseña confirma es distinta a contraseña nueva).
- Movimientos de Caja de Ahorros: (Fecha, Tipo Mov.: 'D' o 'H', Descripción, Importe, Saldo).
- Movimientos de Tarjeta de Débito: (Fecha, Descripción, Importe).
- Movimientos de Tarjeta de Crédito: (Fecha, Descripción, Cuotas: x/y, Importe).
- Crear nueva cuenta: Tipo de cuenta <<CA, CC>>, por cada cuenta <<en pesos, en dólares>>, luego de creada indicar mensaje "Cuenta creada nro. 009-0000099", en donde 9 indica nro. correlativo a otras cuentas previamente creadas.
- Depósito: (Fecha -día, mes, año-, descripción, importe).
- Compras: (Fecha <<día, mes, año>>, descripción, importe, comercio y modo de pago <<TD, TC>>. Debe modificar agregando la compra en CA, y TD o TC).
- Logout: Cerrar la sesión.

## Se pide

Realizar las acciones utilizando las estructuras de control de programas y la utilizaciòn de módulos, en base al siguiente diseño:

El bloque principal representa la estrategia de nivel cero, solo debe contener las acciones indicadas a continuación:

```cpp
main()  {
    SistemaHomeBanking();
    return 0;
}
```

Las operaciones/consultas señaladas anteriormente, a modo de ejemplo (pueden ser otras) se mostrarán en el menú de nivel 1.

Cada una de las opciones del menú seleccionadas para su desarrollo, se realizarán en un nivel 2 de jerarquía. Por lo tanto, los distintos menúes serán un sistema en sí mismo.

## Breve estrategia

Desde el Bloque Principal se invoca al módulo `SistemaHomeBanking()`.

Desde el Módulo SistemaHomeBanking, se incorporan los datos personales para cinco usuarios según se indica en Datos Personales más abajo, se invoca a `MenuLogin()` con argumentos (aDatosPers, usuario), y retorna el usuario. Finalmente se invoca al módulo `MenuPpal()` que retorna la opción elegida, y con esta opción se despacha al módulo correspondiente, utilizando la selección múltiple.

## Observaciones, restricciones y recursos disponibles

Utilizar las funciones entregadas más todas aquellas otras que se requieran.

Se deberán crear archivos de datos para los movimientos de:

- Caja de Ahorro: (`short` dia, mes, año; `char` tipoMov ('D', 'H'); `str25` detalle, `float` importe).
- Tarjeta de Crédito: (`short` dia, mes, año; `str25` detalle; `str5` cuotas; `float` importe).
- Tarjeta de Débito: (`short` dia, mes, año; `str25` detalle; `float` importe).

En todos los casos, no menos de 15 (quince) registros por cada archivo.

Crear 5 (cinco) Datos Personales para cada usuario registrado; DNI, Apellido y Nombre, Fecha Nacimiento, Usuario, Clave, Nro.Celular, Correo Electrónico, Domicilio, Nro.Cta.CA, CBU. Estos datos deberán registrarse internamente en el código utilizando estructuras de datos combinadas entre array con registros.

Crear Cuentas de CBU que estén agendadas para transferencia de dinero y la posibilidad de agendar nuevas cuentas CBU.

Las estructuras de datos a utilizar: registros, arrays, archivos de texto. El diseño de cada una de estas estructuras, quedarán a criterio de cada grupo de trabajo.

Las estructuras de datos pueden estar combinadas, es decir, arrays de registros, campos de registros combinadas con arrays.

Las operaciones que se estiman llevar a cabo: ordenar, insertar en orden, búsqueda binaria, búsqueda secuencial si corresponde, acceso secuencial y/o acceso aleatorio a las componentes en los archivos, leer componentes, grabar nuevas componentes agregándolas a las ya existentes, actualizar componentes de archivo.

Aplicar las operaciones clásicas vistas en las clases, ajustadas a las situaciones que correspondan.

El uso de las estructuras de control de programas deberán ser las más convenientes, entre repetición exacta, 0-x, 1-x, en este último caso limitadas a validación de datos, creación de menúes y para el ordenamiento de bubuja optimizada.

### Se entregan las siguientes funciones para que puedan ser utilizadas

- `long GetTime(int hora, int min, int seg)`. La función retorna la hora larga, como un solo número en el formato hhmmss. Además en sus parámetros devuelve la hora, los min. y los segundos.
- `long GetDate(int year, int mes, int dia, int diaSem)`. La función retorna la fecha larga, como un solo número en el formato aaaammdd. Además en sus parámetros devuelve el año, el mes, el día y el día de la semana. Esta función se encuentra en OBTENER LA FECHA Y HORA DEL SISTEMA del apunte del prof. Hugo Cuello Teoría y Práctica del Lenguaje C/C++ ANEXOS.
- Otras funciones, concretamente para el manejo de la consola (pantalla, teclado): `textColor()`, `GoToXY()`, `textBackColor()`, `clrEol()`, `Window()`, `whereX()`, `whereY()`, `MostrarCursor()`, `OcultarCursor()`, `MenuNavegar()`.
- Las funciones definidas por el grupo, en lo referido a las opciones establecidas deberán invocarse con el prefijo `Menu_opc()` en donde: opc es el nombre de la opción seleccionada, p.e.: si la opción seleccionada fuera "Generar Token", entonces, se invoca a `Menu_GenToken()`.

### Restricciones

- Espacio en disco: Solo para generar nuevas componentes a los archivos creados de movimientos en "MovimientosCA.Txt", "MovimientosTD.Txt", "MovimientosTC.Txt", en formato texto.
- Espacio para arrays y registros: Lo necesario que requiera este proyecto.
- Espacio en memoria dinámica: 0 bytes.
- Accesos a los archivos: Acceso Secuencial cuando sea necesario, acceso Aleatorio para aquellos casos que requiera acceder a las componentes en distinto orden en que fueron grabados.
- Bloque Principal: sólo invocaciones a módulos, según lo establecido anteriormente.
- Paradigma de Programación: Solo se aceptará el Paradigma Imperativo Procedural, Programación Estructurada y Modular.
- Optimización: dado que el uso de ciclos afecta el tiempo de ejecución de un proceso, se evaluará la eficiencia en el uso de los mismos.

Utilizar nombres significativos para los identificadores, dibujos para las estructuras de datos a utilizar, rotulando cada elemento, tamaño, breve leyenda de cómo se generan y estado inicial, respetar esos nombres para utilizarlos en el algoritmo. Preparar una muestra de datos para los archivos para ejecutar el programa e imprimirla. En el disco solo contendrán los archivos "TP1V_K1_ _ _G_-Apellido Nombre.cpp", y los archivos de datos indicados anteriormente como así también el archivo de salida si corresponde, cuyo nombre debe ser "ListadosHB.Txt", todos ubicados en la carpeta raíz del disco.

Ejemplo: `TP1V1_K1023G3_PEREZ JUAN.CPP`

Cada grupo debe crear su propia muestra de datos para los archivos necesarios para el Home Banking.

Se deben utilizar constantes con nombres para indicar cantidades.

## Pautas de entrega

El Trabajo Práctico deberá ser entregado de acuerdo a las pautas indicadas más abajo, el cual se aprobará si reúne los requerimientos solicitados en tiempo y forma (tres fechas máximas): A: Aprobado, N: No Aprobado. (Se debe respetar el orden indicado a continuación):

1. Entregar en carpeta tamaño A4 de tapa transparente y con sujetador de gancho perfectamente alineadas para las hojas lo siguiente: (no se aceptan hojas sueltas ni otro tipo de carpeta).
2. Carátula con los datos de los integrantes del TP, la cantidad de alumnos por grupo del mismo curso se determinará en clase no superando 5 grupos como máximo.
3. Esta misma hoja que establece el enunciado del problema a resolver. Cada grupo elegirá un líder del proyecto, que será el responsable de realizar las entregas del TP. Si un líder abandona la cursada, se deberá elegir otro líder.
4. Diseñar las estructuras de Datos graficándolas indicando con rótulos apropiados, cada elemento, su tamaño en bytes y las variables utilizadas. Las estructuras de datos a graficar son:
   1. El diseño de los registros de cada uno de los archivos.
   2. Otras estructuras de datos que considere necesarias para poder realizar el proceso solicitado. Algunas serán explicadas en clase, como complemento a este documento.
5. Graficar el Bloque Principal.
6. Graficar cada uno de los módulos -funciones- a utilizar, cabecera y cuerpo.
7. Construir una muestra de datos, para los archivos de datos, la cual se la utilizará para probar el Algoritmo. En la hoja impresa a entregar debe haber rótulos apropiados, pero, NO en los archivos de Datos, el cual contendrán solamente, los datos. Ver detalle del formato más abajo.
8. Emitir según la muestra establecida, los resultados esperados, siempre acompañada de los rótulos apropiados, según formato de salida indicados anteriormente.
9. Codificación del Algoritmo completo en el Lenguaje C++, emitiendo números de líneas. Usar Code-Blocks. Las primeras líneas serán de comentario indicando: Nombre del programa, fecha entrega, Nro. versión, breve comentario del objetivo del programa, datos del curso, nombre del día, turno, nro. del grupo e integrantes (Nro.Legajo, Apellido, Nombre).
   - Nombre del compilador: GNU-GCC TDM-GCC-64 x86_64-w64-mingw32-g++.exe.
   - Sitio web descargar TDM-GCC: https://sourceforge.net/projects/tdm-gcc/
   - Luego de la instalación se podrá utilizar con distintos IDE's Code-Blocks, Dev-C++, Visual Studio Code.

## Formato de los archivo de datos en formato .txt

Se deberá ajustar la escritura de los datos respetando a raja tabla el formato tabular, es decir, en columnas, alineando las columnas numéricas a la derecha, las cadenas a izquierda.

Cada columna representa de izquierda a derecha lo siguiente:

```text
Fecha        Detalle del movimiento          Importe
99-99-9999   X(30)                           9(7).99
```

Los números enteros o reales ajustado a la derecha, las cadenas ajustadas a la izquierda. Si una cadena tiene menos caracteres de los indicados, se deberá rellenar con espacios en blanco a derecha.

Diseñar los registros correspondientes a cada tipo de Movimiento, CA, TD, TC.

### Ejemplos

Movimientos de Tarjeta de Débito

```text
Fecha      Comercio                         Importe
29- 3-2026 Librería Alfa-Omega              9782.67
15- 3-2026 Supermercado Kotto            2563294.80
12- 3-2026 Pescaderia El Pez Cado         106491.09
 5- 3-2026 Casa de Pastas La Juvenil       38412.25
                          Total          2717980.81
```

Movimientos de Tarjeta de Crédito

```text
Fecha      Comercio                      Cuotas Nro.Cpbte.    Importe
 3- 2-2026 Pizzeria Kentucky                1/3    245684    29567.73
14- 2-2026 Panaderia Buen Día                                583230.84
19- 2-2026 Combustible YPF                          37485    36491.09
26- 2-2026 Kit herramientas Black&Decker    2/3    162395   854402.56
                                          Total            1503692.22
```

Movimientos de la Cuenta Caja de Ahorro en Pesos

```text
Saldo    $3852419.26
Cta.CA:  106-4235689707-3
Fecha      T Detalle                    Debe       Haber       Saldo
 1- 4-2026 D Ingresos              1542367.21              1542367.21
 1- 4-2026 H Combustible YPF                     35983.68  1506383.53
 3- 4-2026 H Kiosco Las 5 Esquina                 8952.03  1497431.50
10- 4-2026 D Transferencia por Serv. 156236.40             1653667.90
11- 4-2026 H Restaurant Siga la Vaca             76591.31  1577076.59
```

Para los ingresos de los datos de cada archivo de texto, utilizar un editor de texto plano.

Se utilizará este formato semejante a los demás archivos.

La salida de los resultados si corresponde, debe estar dirigida a un archivo de texto con el nombre ListadosHB.Txt se debe utilizar la sentencia `freopen`, para redirigir la salida de la pantalla a archivo de texto al utilizar `cout`, como alternativa.

Se deben leer todos los datos de cada línea en los archivos, sin importar si algunos datos no se utilicen en el proceso.

Dar nombres de identificadores representativos a su uso, es decir, con significado.

Cada nueva entrega además del nombre indicado para el archivo del código en C++ irá acompañado de la versión entregada, iniciando la primera entrega con el sufijo V1, luego la segunda entrega V2, y así sucesivamente.

## Contenido del archivo ModulosHB.cpp

Creación de Menú Dinámico

- `short MenuNavegar(aMenu, short linIni, short linFin, short colIni)`
- Establecer limites de la ventana: `void Window(short x1, short y1, short x2, short y2)`
- Ubicar cursor: `void GoToXY(int x, int y)`
- Obtener posición x e y: `int whereX()`; `int whereY()`
- Limpiar la pantalla: `void clrScr()`
- Limpiar la lìnea desde la posición del cursor: `void clrEol()`
- Aplicación de colores de texto y fondo: `void textColor(int n)`; `void textBackColor(int n)`; `void setConsoleColor(WORD c, WORD f)`
- Mostrar u Ocultar el Cursor: `void MostrarCursor()`; `void OcultarCursor()`
- Mensaje en posición x, y: `void MnsgBox(short x, short y, string mensaje)`; `void MnsgBox(short x, short y, string mensaje, char alinea, short ancho = 0)`
- Borde o Marco: `void Marco(short x1, short y1, short x2, short y2, short color)`
- Ventana alternativa: `void Ventana(short x1, short y1, short x2, short y2)`

Generar valores al azar:

```cpp
srand(time(NULL));
rand() % (MAX-MIN+1)+MIN
```

Inhabilita historial para las teclas flecha arriba, abajo:

```cpp
CONSOLE_HISTORY_INFO history = {sizeof(history), 0};
SetConsoleHistoryInfo(&history);
```

Code-Blocks:

- Settings/Editor/General settings/Encoding settings/Use encoding when opening files: UTF-8
- Use this encoding: Seleccionar "As fallbackencoding".

Uso de caracteres gráficos para hacer marcos:

```cpp
SetConsoleOutputCP(CP_UTF8);
```

Se entregan diversas funciones mencionadas a efectos de utilizarlas en el proyecto HomeBanking.
