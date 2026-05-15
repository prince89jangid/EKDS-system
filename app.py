from flask import Flask, jsonify, request, send_file
import serial
import threading
import time

# ✅ static_folder='.' — current folder ki saari files serve hogi (img.jpeg bhi)
app = Flask(__name__, static_folder='.', static_url_path='')

latest_value = 0

# 🔹 Serial setup
try:
    time.sleep(2)
    ser = serial.Serial('COM14', 9600, timeout=1)
    print("Serial connected")
except Exception as e:
    ser = None
    print(f"Serial NOT connected: {e}")

def read_serial():
    global latest_value
    while True:
        if ser:
            try:
                line = ser.readline().decode(errors='ignore').strip()
                if line:
                    try:
                        latest_value = int(float(line))
                    except ValueError:
                        pass
            except Exception:
                pass
        time.sleep(0.01)

threading.Thread(target=read_serial, daemon=True).start()

@app.route('/data')
def get_data():
    return jsonify({"value": latest_value})

@app.route('/threshold')
def set_threshold():
    val = request.args.get('value', '700')
    try:
        val_int = int(float(val))
    except ValueError:
        val_int = 700
    if ser:
        try:
            ser.write(f"T:{val_int}\n".encode())
        except Exception:
            pass
    return jsonify({"ok": True, "threshold": val_int})

@app.route('/')
def index():
    return send_file("index.html")

if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)
