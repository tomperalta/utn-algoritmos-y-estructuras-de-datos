#!/usr/bin/env python3
# ---------------------------------------------------------------------------
# armar-entrega.py
#
# Genera el archivo UNICO en C++ que se entrega a la catedra, a partir del
# codigo organizado en utilidades/ y modulos/.
#
# El enunciado pide que en el disco de la entrega figure un solo archivo de
# codigo, con el nombre  TP1V1_K1023G3_PEREZ JUAN.CPP . Este script recorre
# trabajo-practico-1.cpp, reemplaza cada  #include "..."  propio del proyecto
# por el contenido del archivo incluido (una sola vez por archivo, respetando
# el orden) y deja los  #include <...>  del sistema al principio.
#
# El resultado es equivalente byte a byte a lo que ve el compilador, de modo
# que si el proyecto compila, el archivo unico tambien.
#
# Uso:
#   python3 armar-entrega.py "TP1V1_K1023G3_PEREZ JUAN.cpp"
#   python3 armar-entrega.py                # usa TP1V1_ENTREGA.cpp
# ---------------------------------------------------------------------------
import os
import re
import sys

RAIZ = os.path.dirname(os.path.abspath(__file__))
PRINCIPAL = "trabajo-practico-1.cpp"
INCLUDE_PROPIO = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')

incluidos = set()


def expandir(ruta_rel, salida):
    """Vuelca el archivo en la salida, reemplazando sus includes propios."""
    ruta = os.path.join(RAIZ, ruta_rel)
    if ruta_rel in incluidos:
        return
    incluidos.add(ruta_rel)

    with open(ruta, encoding="utf-8") as f:
        for linea in f:
            m = INCLUDE_PROPIO.match(linea)
            if m:
                hijo = os.path.normpath(
                    os.path.join(os.path.dirname(ruta_rel), m.group(1)))
                salida.append(
                    f"// ===== inicio de {hijo} "
                    f"{'=' * max(0, 60 - len(hijo))}\n")
                expandir(hijo, salida)
                salida.append(f"// ===== fin de {hijo}\n\n")
            else:
                salida.append(linea)


def main():
    destino = sys.argv[1] if len(sys.argv) > 1 else "TP1V1_ENTREGA.cpp"
    salida = []
    expandir(PRINCIPAL, salida)
    texto = "".join(salida)

    with open(os.path.join(RAIZ, destino), "w", encoding="utf-8") as f:
        f.write(texto)

    lineas = texto.count("\n")
    print(f"Generado: {destino}")
    print(f"  archivos integrados : {len(incluidos)}")
    print(f"  lineas              : {lineas}")
    print()
    print("Para verificar que compila:")
    print(f'  g++ -std=c++17 -Wall -Wextra "{destino}" -o entrega.exe')
    print()
    print("Recorda completar el encabezado del archivo con los datos del grupo")
    print("(fecha de entrega, curso, dia, turno, nro. de grupo e integrantes).")


if __name__ == "__main__":
    main()
