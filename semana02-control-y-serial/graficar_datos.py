import csv 
import matplotlib
matplotlib.use("qtagg")

import matplotlib.pyplot as plt

tiempos = []
valores = []

with open("datos_potenciometro.csv", "r") as archivo:
    lector = csv.DictReader(archivo)

    for fila in lector:
        tiempos.append(float(fila["Tiempo_s"]))
        valores.append(int(fila["valor_adc"]))
    
plt.plot(tiempos,valores)
plt.xlabel("tiempo en segundos")
plt.axhline(y=400, color="green", linestyle="--", label="Umbral apertura")
plt.axhline(y=600, color="red", linestyle="--", label="Umbral cierre")
plt.legend()
plt.grid(True)
plt.axhline(400, 600)
plt.show()

