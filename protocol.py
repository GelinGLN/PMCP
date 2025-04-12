import json
import serial
import time

# Configura a conexão serial
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  # Espera o Arduino reiniciar após a conexão serial

# Espera a mensagem "Arduino pronto para receber comandos"
ready = False
while not ready:
    if ser.in_waiting:
        response = ser.readline().decode('utf-8').strip()
        print(f"Arduino diz: {response}")
        if "pronto" in response:
            ready = True

# Comandos para enviar
commands = [
    {"cmd": "shieldLed_on"},
    {"cmd": "delay", "param": 20000},
    {"cmd": "shieldLed_off"},
]

# Envia os dados
msg = json.dumps(commands) + '\n'
print(f"Enviando: {msg}")
ser.write(msg.encode())

# Aguarda e exibe respostas do Arduino
time.sleep(0.5)
while ser.in_waiting:
    response = ser.readline().decode('utf-8').strip()
    print(f"Arduino responde: {response}")

# Fecha a conexão
ser.close()