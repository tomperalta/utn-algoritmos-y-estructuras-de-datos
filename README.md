# Algoritmos y Estructuras de Datos — UTN

Material de cursada de **Algoritmos y Estructuras de Datos**, primer año de
**Ingeniería en Sistemas de Información** en la **UTN**, cátedra del profesor
**Hugo Cuello**.

El repositorio junta la resolución de la guía de ejercicios, los trabajos
prácticos y los parciales de años anteriores usados para practicar. Todo está en
**C++**, con el paradigma que pide la materia: **imperativo procedural,
programación estructurada y modular**, sin memoria dinámica.

---

## Cómo está organizado

```
.
├── ejercicios/                  guía práctica de la cátedra
│   └── guia-1.cpp               guía 1: ejercicios 1 a 5, 13 y 14 resueltos
│                                (una función por ejercicio, invocadas desde main)
│
├── parciales/                   parciales de años anteriores, para practicar
│   ├── parcial-1-07-2018.md     gastos diarios: acumulación por mes y día
│   └── parcial-1-01-07-2019.md  Boletek: ranking de las 10 películas más vistas
│
└── trabajos-practicos/
    └── trabajo-practico-1/      TP #1 — Home Banking (registros, arreglos y archivos)
```

Cada parcial está transcripto en Markdown con el enunciado completo y el
encabezado a completar (curso, día, turno, legajo), tal como se entrega.

---

## Compilar

El entorno es el que pide la cátedra: **Code::Blocks** con **TDM-GCC-64**
(`x86_64-w64-mingw32-g++.exe`) sobre Windows, estándar C++17.

Para el TP1 está el proyecto `TP1.cbp`. El paso a paso —instalar el compilador,
configurar el IDE, ejecutar y resolver los errores más comunes— está en
[CODE-BLOCKS.md](trabajos-practicos/trabajo-practico-1/CODE-BLOCKS.md).

Desde la terminal:

```
g++ -std=c++17 -Wall -Wextra ejercicios/guia-1.cpp -o guia-1.exe
```
