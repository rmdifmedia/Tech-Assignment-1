from flask import Flask, request, jsonify, render_template_string
import json

app = Flask(__name__)

# Global variable to store the sensor data
sensor_data = {"temperature": None, "humidity": None}

@app.route('/post-data', methods=['POST'])
def post_data():
    global sensor_data
    if request.is_json:
        data = request.get_json()
        sensor_data['temperature'] = data['temperature']
        sensor_data['humidity'] = data['humidity']
        print(f"Received temperature: {sensor_data['temperature']}")
        print(f"Received humidity: {sensor_data['humidity']}")
        return jsonify({"pesan": "Data diterima"}), 200
    else:
        return jsonify({"pesan": "Coba lagi"}), 400

@app.route('/')
def index():
    return render_template_string('''
    <!DOCTYPE html>
    <html>
    <head>
        <title>Data DHT11</title>
        <script>
            async function fetchData() {
                const response = await fetch('/get-data');
                const data = await response.json();
                document.getElementById("temperature").innerText = data.temperature;
                document.getElementById("humidity").innerText = data.humidity;
            }

            setInterval(fetchData, 5000);
        </script>
    </head>
    <body>
        <h1>Data DHT11</h1>
        <p>Temperature: <span id="temperature">Loading...</span></p>
        <p>Humidity: <span id="humidity">Loading...</span></p>
    </body>
    </html>
    ''')

@app.route('/get-data', methods=['GET'])
def get_data():
    global sensor_data
    return jsonify(sensor_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)