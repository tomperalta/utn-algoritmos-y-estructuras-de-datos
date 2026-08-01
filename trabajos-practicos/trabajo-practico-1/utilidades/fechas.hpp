// ---------------------------------------------------------------------------
// fechas.hpp
// Obtencion y manejo de la fecha y la hora del sistema.
//
// GetTime() y GetDate() son las funciones entregadas por la catedra (apunte
// <<Teoria y Practica del Lenguaje C/C++ - ANEXOS>>, prof. Hugo Cuello). El
// resto son funciones propias del grupo para validar, comparar y dar formato a
// las fechas.
// ---------------------------------------------------------------------------
#ifndef FECHAS_HPP
#define FECHAS_HPP

namespace FechaHora {

  const char NOMBRE_MES[13][12] = {"",
    "Enero", "Febrero", "Marzo",     "Abril",   "Mayo",      "Junio",
    "Julio", "Agosto",  "Setiembre", "Octubre", "Noviembre", "Diciembre"};

  const char NOMBRE_DIA[8][12] = {"",
    "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

  // -------------------------------------------------------------------------
  // Devuelve la hora del sistema como un solo numero con formato hhmmss y, por
  // parametro, la hora, los minutos y los segundos.  (Funcion de la catedra)
  // -------------------------------------------------------------------------
  long GetTime(int &hh, int &mm, int &ss) {
    time_t rawtime;
    record tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    hh = timeinfo->tm_hour;
    mm = timeinfo->tm_min;
    ss = timeinfo->tm_sec;
    return timeinfo->tm_hour * 10000L + timeinfo->tm_min * 100L +
           timeinfo->tm_sec;
  } // GetTime

  // -------------------------------------------------------------------------
  // Devuelve la fecha del sistema como un solo numero con formato aaaammdd y,
  // por parametro, el anio, el mes, el dia y el dia de la semana (1 = Domingo).
  // (Funcion de la catedra)
  // -------------------------------------------------------------------------
  long GetDate(int &anio, int &mes, int &dia, int &diaSem) {
    time_t rawtime;
    record tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    anio   = 1900 + timeinfo->tm_year;
    mes    = 1 + timeinfo->tm_mon;
    dia    = timeinfo->tm_mday;
    diaSem = 1 + timeinfo->tm_wday;
    return (1900 + timeinfo->tm_year) * 10000L + (1 + timeinfo->tm_mon) * 100L
           + timeinfo->tm_mday;
  } // GetDate

  // -------------------------------------------------------------------------
  // Carga en <<f>> la fecha de hoy.
  // -------------------------------------------------------------------------
  void FechaDeHoy(Fecha &f) {
    int anio, mes, dia, diaSem;

    GetDate(anio, mes, dia, diaSem);
    f.dia  = (short) dia;
    f.mes  = (short) mes;
    f.anio = (short) anio;
  } // FechaDeHoy

  // -------------------------------------------------------------------------
  // Convierte la fecha a un unico numero aaaammdd, apto para comparar y
  // ordenar cronologicamente.
  // -------------------------------------------------------------------------
  long FechaLarga(Fecha f) {
    return f.anio * 10000L + f.mes * 100L + f.dia;
  } // FechaLarga

  bool AnioBisiesto(short anio) {
    return (anio % 4 == 0 and anio % 100 != 0) or anio % 400 == 0;
  } // AnioBisiesto

  short DiasDelMes(short mes, short anio) {
    const short DIAS[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes < 1 or mes > 12)
      return 0;
    if (mes == 2 and AnioBisiesto(anio))
      return 29;
    return DIAS[mes];
  } // DiasDelMes

  bool FechaValida(Fecha f) {
    return f.anio >= ANIO_MINIMO and f.anio <= ANIO_MAXIMO and
           f.mes  >= 1 and f.mes <= 12 and
           f.dia  >= 1 and f.dia <= DiasDelMes(f.mes, f.anio);
  } // FechaValida

  // -------------------------------------------------------------------------
  // Da formato a la fecha en el formato tabular del enunciado: 99-99-9999, con
  // el dia ajustado a la derecha (" 5- 3-2026").
  // -------------------------------------------------------------------------
  void FormatearFecha(Fecha f, char dest[]) {
    snprintf(dest, ANCHO_FECHA + 1, "%2d-%2d-%4d", f.dia, f.mes, f.anio);
  } // FormatearFecha

  // Formato con ceros a la izquierda, para pantalla: "05-03-2026".
  void FormatearFechaCeros(Fecha f, char dest[]) {
    snprintf(dest, ANCHO_FECHA + 1, "%02d-%02d-%04d", f.dia, f.mes, f.anio);
  } // FormatearFechaCeros

  // Formato en letras: "5 de Marzo de 2026".
  void FormatearFechaLetras(Fecha f, char dest[], short tam) {
    snprintf(dest, tam, "%d de %s de %d", f.dia,
             (f.mes >= 1 and f.mes <= 12 ? NOMBRE_MES[f.mes] : "?"), f.anio);
  } // FormatearFechaLetras

  // -------------------------------------------------------------------------
  // Suma <<dias>> dias a la fecha <<f>> y deja el resultado en <<res>>. La
  // normalizacion la realiza mktime(), por lo que contempla meses de distinta
  // duracion y anios bisiestos.
  // -------------------------------------------------------------------------
  void SumarDias(Fecha f, short dias, Fecha &res) {
    record tm calendario;

    memset(&calendario, 0, sizeof calendario);
    calendario.tm_mday  = f.dia + dias;
    calendario.tm_mon   = f.mes - 1;
    calendario.tm_year  = f.anio - 1900;
    calendario.tm_hour  = 12;          // mediodia: evita saltos por horario de verano
    calendario.tm_isdst = -1;
    mktime(&calendario);
    res.dia  = (short) calendario.tm_mday;
    res.mes  = (short)(calendario.tm_mon + 1);
    res.anio = (short)(calendario.tm_year + 1900);
  } // SumarDias

  // -------------------------------------------------------------------------
  // Devuelve la hora del sistema con formato "hh:mm:ss".
  // -------------------------------------------------------------------------
  void HoraDeHoy(char dest[]) {
    int hh, mm, ss;

    GetTime(hh, mm, ss);
    snprintf(dest, 9, "%02d:%02d:%02d", hh, mm, ss);
  } // HoraDeHoy

  // -------------------------------------------------------------------------
  // Devuelve el nombre del dia de la semana de hoy.
  // -------------------------------------------------------------------------
  const char *DiaSemanaDeHoy() {
    int anio, mes, dia, diaSem;

    GetDate(anio, mes, dia, diaSem);
    return NOMBRE_DIA[diaSem >= 1 and diaSem <= 7 ? diaSem : 0];
  } // DiaSemanaDeHoy

} // namespace FechaHora

#endif // FECHAS_HPP
