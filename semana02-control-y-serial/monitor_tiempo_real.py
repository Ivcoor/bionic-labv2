import serial
import time 
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

tiempos = []
valores = []
buffer = bytearray()

puerto = serial.Serial("/dev/ttyACM0", 9600, timeout=0)
inicio = time.monotonic()

fig, ax = plt.subplots()

linea_grafica, = ax.plot([], [], label="Potenciometro")

ax.set_xlabel("Tiempo (s)")
ax.set_ylabel("Valores adc")
ax.set_ylim(0,1023)
ax.grid(True)
ax.legend()




def actualizar(frame):
    bitsDisponibles = puerto.in_waiting

    if bitsDisponibles > 0:
        buffer.extend(puerto.read(bitsDisponibles))

    while buffer.find(b"\n") != -1:
        posicion = buffer.find(b"\n")
        primeraLinea = buffer[:posicion]
        del buffer[:posicion +1]
        try: 
            valor = int(primeraLinea.decode("ascii").strip())
            tiempo = time.monotonic() - inicio
        except (UnicodeDecodeError, ValueError):
            print("Lectura corrupta")
            continue
        
        tiempos.append(tiempo)
        valores.append(valor)

    linea_grafica.set_data(tiempos, valores)
    if tiempos:
        ax.set_xlim(max(0, tiempos[-1] -10), max(10, tiempos[-1]))
        


animacion = FuncAnimation(
    fig,
    actualizar,
    interval=50,
    blit=False,
    cache_frame_data=False
)

plt.show()
