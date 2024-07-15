import sys
from PyQt5 import QtWidgets, QtGui, QtCore, uic
import threading
from flask import Flask, request, jsonify
import requests
from ifplano2 import Ui_MainWindow
from dotenv import load_dotenv
import os
import telebot

# Carga las variables de entorno desde el archivo .env
load_dotenv()

# Accede al valor del token de tu bot
bot_token = os.getenv("BOT_TOKEN")

bot = telebot.TeleBot(bot_token)

def get_sensor_data():
    try:
        response = requests.get("http://192.168.0.49:5000/datos")
        if response.status_code == 200:
            return response.json()
        else:
            return None
    except requests.RequestException as e:
        print(f"Error de conexión: {e}")
        return None

@bot.message_handler(commands=['start', 'help'])
def send_welcome(message):
    bot.reply_to(message, "Hola! bienvenido al centro de control de tu hogar inteligente! Para consultar los datos del sensor, solo escribe el nombre de la habitación.\n Comandos Disponibles:\n Pieza 1\n Pieza 2\n Baño 1\n Baño 2\n Cocina")

@bot.message_handler(func=lambda message: message.text in ["Pieza 1", "Pieza 2", "Baño 1", "Baño 2", "Cocina"])
def send_sensor_data(message):
    data = get_sensor_data()
    if data:
        if message.text == "Pieza 1":
            bot.send_message(message.chat.id, f"Pieza 1 - Temperatura: {data['temp1']}°C, Humedad: {data['hum1']}%")
        elif message.text == "Pieza 2":
            bot.send_message(message.chat.id, f"Pieza 2 - Temperatura: {data['temp2']}°C, Humedad: {data['hum2']}%")
        elif message.text == "Cocina":
            bot.send_message(message.chat.id, f"Cocina - Temperatura: {data['temp3']}°C, Humedad: {data['hum3']}%")
        elif message.text == "Baño 1":
            bot.send_message(message.chat.id, f"Baño 1 - Temperatura: {data['temp4']}°C, Humedad: {data['hum4']}%")
        elif message.text == "Baño 2":
            bot.send_message(message.chat.id, f"Baño 2 - Temperatura: {data['temp5']}°C, Humedad: {data['hum5']}%")
    else:
        bot.send_message(message.chat.id, "Error al obtener datos del sensor.")

@bot.message_handler(func=lambda message: True)
def echo_all(message):
    bot.reply_to(message, message.text)

def start_telegram_bot():
    bot.infinity_polling()

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        # Configurar un temporizador para verificar si se reciben datos
        self.Datos_timer = QtCore.QTimer(self)
        self.Datos_timer.timeout.connect(self.leer_Datos)
        self.Datos_timer.start(5000)  # Verificar cada 5 segundos

    def leer_Datos(self):
        try:
            # Solicitar datos al servidor Flask usando GET
            response = requests.get("http://192.168.0.49:5000/datos")
            if response.status_code == 200:
                # Leer los datos recibidos desde la respuesta
                data = response.json()
                temp1, hum1, temp2, hum2, temp3, hum3, temp4, hum4, temp5, hum5 = data['temp1'], data['hum1'], data['temp2'], data['hum2'], data['temp3'], data['hum3'], data['temp4'], data['hum4'], data['temp5'], data['hum5']
                self.actualizar_interfaz(temp1, hum1, temp2, hum2, temp3, hum3, temp4, hum4, temp5, hum5)
            else:
                print("Error al recibir datos")
        except requests.RequestException as e:
            print(f"Error de conexión: {e}")

    def actualizar_interfaz(self, temp1, hum1, temp2, hum2, temp3, hum3, temp4, hum4, temp5, hum5):
        self.cuartoA.setText(f"Temperatura: {temp1}<br>Humedad: {hum1}")
        self.cuartoB.setText(f"Temperatura: {temp2}<br>Humedad: {hum2}")
        self.cocina.setText(f"Temperatura: {temp3}<br>Humedad: {hum3}")
        self.banoA.setText(f"Temperatura: {temp4}<br>Humedad: {hum4}")
        self.banoB.setText(f"Temperatura: {temp5}<br>Humedad: {hum5}")

def start_flask_app():
    app = Flask(__name__)
    data_storage = {}

    @app.route('/datos', methods=['POST'])
    def recibir_datos():
        temp1 = request.form.get('temp1')
        hum1 = request.form.get('hum1')
        temp2 = request.form.get('temp2')
        hum2 = request.form.get('hum2')
        temp3 = request.form.get('temp3')
        hum3 = request.form.get('hum3')
        temp4 = request.form.get('temp4')
        hum4 = request.form.get('hum4')
        temp5 = request.form.get('temp5')
        hum5 = request.form.get('hum5')

        if None in [temp1, hum1, temp2, hum2, temp3, hum3, temp4, hum4, temp5, hum5]:
            print(f"Error: datos no recibidos correctamente: {temp1}, {hum1}, {temp2}, {hum2}, {temp3}, {hum3}, {temp4}, {hum4}, {temp5}, {hum5}")
            return "Datos no recibidos correctamente", 400
        else:
            #print(f"Sensor 1 - Temperatura: {temp1}, Humedad: {hum1}")
            #print(f"Sensor 2 - Temperatura: {temp2}, Humedad: {hum2}")
            #print(f"Sensor 3 - Temperatura: {temp3}, Humedad: {hum3}")
            #print(f"Sensor 4 - Temperatura: {temp4}, Humedad: {hum4}")
            #print(f"Sensor 5 - Temperatura: {temp5}, Humedad: {hum5}")

            # Almacenar los datos recibidos
            data_storage.update({
                'temp1': temp1,
                'hum1': hum1,
                'temp2': temp2,
                'hum2': hum2,
                'temp3': temp3,
                'hum3': hum3,
                'temp4': temp4,
                'hum4': hum4,
                'temp5': temp5,
                'hum5': hum5
            })

            return "Datos recibidos correctamente", 200

    @app.route('/datos', methods=['GET'])
    def enviar_datos():
        return jsonify(data_storage)

    app.run(host='192.168.0.49', port=5000)

if __name__ == "__main__":
    # Iniciar el servidor Flask en un hilo separado
    flask_thread = threading.Thread(target=start_flask_app)
    flask_thread.daemon = True
    flask_thread.start()

    # Iniciar el bot de Telegram en un hilo separado
    telegram_thread = threading.Thread(target=start_telegram_bot)
    telegram_thread.daemon = True
    telegram_thread.start()

    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    window.setWindowTitle("Plano general del Hogar")
    app.exec()
