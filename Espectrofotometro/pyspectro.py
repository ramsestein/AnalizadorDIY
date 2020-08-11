#!/bin/env python

import serial
from xlrd import open_workbook
from xlutils.copy import copy
import datetime


def wait_for_button():
    raw_input()
    ser.write("S")      # write a string


def read_serial_line():
    out = ''
    char = ''
    while char != '\n':
        while ser.inWaiting() > 0:
            char = ser.read(1)
            out += char
    return out

# Volcado de la hoja Excel


def update_xls(out):

    def process_lineas(_out):
        _lineas = _out.replace('\r', '').split('\n')
        _lineas.remove('')
        return _lineas

    def write_linea_2_xls(_linea, _ws, x):
        # Si es la cabecera lo descarta
        if _linea == 'i_0,i':
            return False
        # Se parsea la linea
        linea_p = _linea.split(',')
        # Se escribe la intensidad
        i_0 = linea_p[0]
        _ws.write(1 + x, 0, int(i_0))
        # Se escribe la intensidad
        i = linea_p[1]
        _ws.write(1 + x, 1, int(i))
        # Calcula el/la valor interesante
        from math import log
        _ws.write(1 + x, 2, log(float(i)/float(i_0)))

    # Se abre la plantilla XSL
    rb = open_workbook('plantilla.xls', formatting_info=True)
    # Se obtine una copia de la plantilla
    wb = copy(rb)
    # Se selecciona la Hoja1
    ws = wb.get_sheet(0)
    # import pdb
    # pdb.set_trace()
    lineas = process_lineas(out)
    x = 0
    _exit = False
    _continue = True
    _ignore = 1
    # Bucle para escribir en el XLS
    while not _exit:
        # import pdb
        # pdb.set_trace()
        # Bucle recorriendo la linea
        for linea in lineas:
            if "[CSV-START]" in linea:
                _continue = False
                continue
            if _continue:
                if not _continue and _ignore > 0:
                    _ignore = _ignore - 1
                continue
            # Se imprime la linea
            print linea
            # Fin de fichero
            if "[CSV-END]" in linea:
                _exit = True
                break
            # Escribe la linea en el fichero
            write_linea_2_xls(linea, ws, x)
            # Se incrementa contador de muestra
            x = x + 1

        # Sale
        if _exit:
            break
        # import pdb
        # pdb.set_trace()
        lineas = []
        while lineas == []:
            # Se lee la linea de datos CSV desde serial
            lineas = process_lineas(read_serial_line())

    # Se registra en un nuevo fichero XLS
    now = datetime.datetime.now()
    filename = str(now.strftime('%m-%d_%H%M%S')) + '.xls'
    print filename
    wb.save(filename)
    from subprocess import call
    call(["nohup", "gnome-open", filename, " &"])


# Se incia el puerto serial
ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=9600,
    #    parity=serial.PARITY_ODD,
    #    stopbits=serial.STOPBITS_TWO,
    #    bytesize=serial.SEVENBITS
)

# Se compruba el puerto serial que se ha abierto
print ser.portstr

out = ''
while not "[CSV-END]" in out:
    # Se lee la linea desde serial
    out = read_serial_line()
    # Se imprime la linea leida
    print out,
    # Si el arduino esta esperando se manda senyal
    #   por serial al pulsar <ENTER>
    if "[S + <enter>]" in out:
        wait_for_button()
    if "[CSV-START]" in out:
        # Lee la linea de cabecera (Ya se conoce)
        print " Volcando datos a XLS ... "
        # Vuelca los datos al XLS
        update_xls(out)
        print " [OK] Volcado a XLS "
        break

ser.close()             # close port
