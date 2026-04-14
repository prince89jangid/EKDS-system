from flask import Flask, jsonify
import serial
import threading

app = Flask(__name__)

# ⚠️ COM port check karo (Device Manager me dekho)
ser = serial.Serial('COM8', 9600, timeout=1)

latest_value = 0

def read_serial():
    global latest_value
    while True:
        try:
            line = ser.readline().decode().strip()
            if line.isdigit():
                latest_value = int(line)
        except:
            pass

threading.Thread(target=read_serial, daemon=True).start()

@app.route('/data')
def get_data():
    return jsonify({"value": latest_value})

@app.route('/')
def index():
    return open("index.html").read()

app.run(debug=False)    