import serial
import csv
import time

puerto = serial.Serial("/dev/ttyACM0", 9600, timeout=0)

with open("datos_potenciometro.csv", "w", newline= "") as archivo:
    writer = csv.writer(archivo)
    writer.writerow(["Tiempo_s", "valor_adc"])
    inicio = time.monotonic()
    try:
        
        while True:
            linea = puerto.readline()

            try:
                texto = linea.decode("ascii").strip()
                tiempo = time.monotonic() - inicio

                if texto:
                    valor = int(texto)
                    writer.writerow([f"{tiempo:3f}", valor]) 
                    print(valor)

            except (UnicodeDecodeError, ValueError):
                print(f"Lectura corrupta: {linea!r}")
    except KeyboardInterrupt:
        print("Grabacion detenida")
    finally:
        puerto.close()
