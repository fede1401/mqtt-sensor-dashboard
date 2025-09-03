#define NOMINMAX
#include <mosquitto.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

// Struttura dei dati per memorizzare una singola lettura del sensore
struct SensorReading {
    std::string timestamp;
    std::string type;
    std::string value;
};

// Variabili globali
std::vector<SensorReading> readings; // storici dei valori letti
std::mutex data_mutex; // per l'accesso sicuro (thread) ai dati

// variabili stringhe per i dati
std::string current_temp = "N/A";
std::string current_umi_fed = "N/A";
std::string current_hic_fed = "N/A";

std::string current_temp_studio = "N/A";
std::string current_umi_studio = "N/A";

std::string current_temp_sala_hobby = "N/A";

std::string current_metano_density = "N/A";
std::string current_gas_density = "N/A";

std::string current_wifi_signal = "N/A";
std::string current_wifi_disconnected = "N/A";


std::string current_power = "N/A";
std::string kw_consumption = "N/A";
std::string volts = "N/A";
std::string current = "N/A";
std::string apparent_power = "N/A";

std::string last_updated = "Never";

// Genera i timestamp nel formato adatto: YYYY-MM-DD HH:MM:SS
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Funzione che si occupa di trasformare il payload ricevuto in stringa e stamparlo
void on_message(struct mosquitto*, void*, const struct mosquitto_message* msg) {
    std::string payload((char*)msg->payload, msg->payloadlen);
    std::cout << "[MQTT] " << msg->topic << " => " << payload << "\n";

    std::lock_guard<std::mutex> lock(data_mutex);

    std::string topic(msg->topic);

    if (topic == "/.../...") {
        current_temp = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Temperatura stanza ", payload};
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../...") {
        current_temp_studio = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Temperatura stanza ....", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../...") {
        current_metano_density = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Density metano", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../...") {
        current_gas_density = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Density gas", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../.../...") {
        current_temp_sala_hobby = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Temperatura stanza ...", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../...") {
        current_umi_fed = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Umidity stanza ...", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../...") {
        current_hic_fed = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Temperatura effettiva stanza ...", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../...") {
        current_umi_studio = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Umidity stanza ...", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../.../...") {
        current_wifi_signal = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Wifi Signal", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../...") {
        current_wifi_disconnected = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Wifi signal disconnected", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    // Power
    else if (topic == "/.../...") {
        current_power = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Current power", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../...") {
        kw_consumption = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Consumo in kWatt", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../...") {
        volts = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Volts", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../...") {
        current = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Current", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

    else if (topic == "/.../...") {
        apparent_power = payload;
        last_updated = getTimestamp();

        SensorReading reading = { last_updated, "Potenza apparente", payload };
        readings.push_back(reading);

        // Mantieni solo 20 letture
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }

}

// Funzione per sottoscriversi a tutti i topic quando la connessione viene stabilita
void on_connect(struct mosquitto* mosq, void*, int result) {
    if (result == 0) {
        std::cout << "[MQTT] Connected!\n";
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../.../.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "c", 1);


        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);

        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);
        mosquitto_subscribe(mosq, nullptr, "/.../...", 1);

    }
    else {
        std::cout << "[MQTT] Connection failed\n";
    }
}

// Stampa di disconnessione
void on_disconnect(struct mosquitto*, void*, int) {
    std::cout << "[MQTT] Disconnected\n";
}

 
// Generazione dell'HTML
std::string createHTML() {
    std::lock_guard<std::mutex> lock(data_mutex);

    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html; charset=UTF-8\r\n";
    response << "Cache-Control: no-cache\r\n";
    response << "\r\n";

    /*response << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard Temperatura</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            border-radius: 15px;
            padding: 30px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }
        .header {
            text-align: center;
            margin-bottom: 30px;
        }
        .temp-display {
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            padding: 30px;
            border-radius: 10px;
            text-align: center;
            margin-bottom: 30px;
        }
        .gasmet-display{
            background: linear-gradient(45deg, #e74c3c, #c0392b);
            color: white;
            padding: 30px;
            border-radius: 10px;
            text-align: center;
            margin-bottom: 30px;
        }
        .temperature {
            font-size: 3em;
            font-weight: bold;
            margin: 10px 0;
        }
        .status {
            font-size: 1.2em;
        }
        .history {
            background: #f5f5f5;
            padding: 20px;
            border-radius: 10px;
        }
        .history-item {
            display: flex;
            justify-content: space-between;
            padding: 8px 0;
            border-bottom: 1px solid #ddd;
        }
        .history-item:last-child {
            border-bottom: none;
        }
        .timestamp {
            color: #666;
        }
        .value {
            font-weight: bold;
            color: #667eea;
        }
        .info {
            text-align: center;
            margin-top: 20px;
            color: #666;
            font-style: italic;
        }
    </style>
    <script>
        setTimeout(function() { location.reload(); }, 5000);
    </script>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Dashboard Home IoT</h1>
            <p>Monitoraggio in Tempo Reale</p>
        </div>

        <div class="temp-display">
            <div class="status">Temperatura Attuale camera ...</div>
            <div class="temperature">)" << current_temp << R"(</div>
            <div class="status">)" << (current_temp != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
            <div>Aggiornato: )" << last_updated << R"(</div>
        </div>

        <div class="temp-display">
            <div class="status">Umidity camera ...</div>
            <div class="temperature">)" << current_umi_fed << R"(</div>
            <div class="status">)" << (current_umi_fed != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>

        <div class="temp-display">
            <div class="status">Hic camera ...</div>
            <div class="temperature">)" << current_hic_fed << R"(</div>
            <div class="status">)" << (current_hic_fed != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>
            <br>
            <br>

        <div class="temp-display">
            <div class="status">Temperatura Attuale Studio ...</div>
            <div class="temperature">)" << current_temp_studio << R"(</div>
            <div class="status">)" << (current_temp_studio != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>

        <div class="temp-display">
            <div class="status">Umidity studio ...</div>
            <div class="temperature">)" << current_umi_studio << R"(</div>
            <div class="status">)" << (current_umi_studio != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>
            <br>
            <br>

        <div class="temp-display">
            <div class="status">Temperatura Attuale Sala</div>
            <div class="temperature">)" << current_temp_sala_hobby << R"(</div>
            <div class="status">)" << (current_temp_sala_hobby != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>
            <br>
            <br>

        <div class="gasmet-display">
            <div class="status">Density del metano: </div>
            <div class="temperature">)" << current_metano_density << R"(</div>
            <div class="status">)" << (current_metano_density != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>

        <div class="gasmet-display">
            <div class="status">Density del gas</div>
            <div class="temperature">)" << current_gas_density << R"(</div>
            <div class="status">)" << (current_gas_density != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>
            <br>
            <br>

        <div class="temp-display">
            <div class="status">Segnale wifi</div>
            <div class="temperature">)" << current_wifi_signal << R"(</div>
            <div class="status">)" << (current_wifi_signal != "N/A" ? "ONLINE" : "OFFLINE") << R"(</div>
        </div>
        </div>

        <div class="history">
            <h3>Storico Letture</h3>)";

    if (readings.empty()) {
        response << "<p>Nessun dato disponibile</p>";
    }
    else {
        int count = 0;
        for (int i = readings.size() - 1; i >= 0 && count < 10; i--, count++) {
            response << "<div class='history-item'>";
            response << "<span class='timestamp'>" << readings[i].timestamp << "</span>";
            response << "<span class='value'>" << readings[i].value << "</span>";
            response << "</div>";
        }
    }

    response << R"(
        </div>

        <div class="info">
            Server C++ integrato - Aggiornamento automatico ogni 3 secondi
        </div>
    </div>
</body>
</html>)";

    return response.str();
}*/
    response << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard Temperatura</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: white;
            border-radius: 15px;
            padding: 30px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }
        .header {
            text-align: center;
            margin-bottom: 40px;
        }
        
        /* Sezioni per categorie */
        .category {
            margin-bottom: 40px;
        }
        .category-title {
            font-size: 1.8em;
            font-weight: bold;
            color: #333;
            margin-bottom: 20px;
            text-align: center;
            border-bottom: 3px solid #667eea;
            padding-bottom: 10px;
        }
        .category-title.gas {
            border-bottom-color: #e74c3c;
        }
        .category-title.wifi {
            border-bottom-color: #27ae60;
        }
        
        /* Griglia responsive */
        .sensor-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 20px;
        }
        
        /* Card sensori */
        .sensor-card {
            color: white;
            padding: 25px;
            border-radius: 12px;
            text-align: center;
            box-shadow: 0 4px 15px rgba(0,0,0,0.1);
            transition: transform 0.2s ease;
        }
        .sensor-card:hover {
            transform: translateY(-5px);
        }
        
        /* Stili per categoria temperatura */
        .temp-card {
            background: linear-gradient(45deg, #667eea, #764ba2);
        }
        
        /* Stili per categoria gas */
        .gas-card {
            background: linear-gradient(45deg, #e74c3c, #c0392b);
        }
        
        /* Stili per categoria wifi */
        .wifi-card {
            background: linear-gradient(45deg, #27ae60, #2ecc71);
        }

        /* Stili per categoria corrente-potenza */
        .power-card {
            background: linear-gradient(45deg, #00202e, #003f5c);
        }
            
        
        .sensor-value {
            font-size: 2.2em;
            font-weight: bold;
            margin: 15px 0;
        }
        .sensor-name {
            font-size: 1.1em;
            margin-bottom: 10px;
            opacity: 0.9;
        }
        .sensor-status {
            font-size: 1em;
            margin-top: 10px;
            font-weight: bold;
        }
        .sensor-status.online {
            color: #2ecc71;
        }
        .sensor-status.offline {
            color: #e74c3c;
        }
        
        /* Storico */
        .history {
            background: #f8f9fa;
            padding: 25px;
            border-radius: 12px;
            margin-top: 30px;
        }
        .history h3 {
            color: #333;
            margin-bottom: 20px;
        }
        .history-item {
            display: flex;
            justify-content: space-between;
            padding: 12px 0;
            border-bottom: 1px solid #dee2e6;
        }
        .history-item:last-child {
            border-bottom: none;
        }
        .timestamp {
            color: #6c757d;
            font-weight: 500;
        }
        .value {
            font-weight: bold;
            color: #495057;
        }
        
        .info {
            text-align: center;
            margin-top: 25px;
            color: #6c757d;
            font-style: italic;
        }
        
        /* Responsive design */
        @media (max-width: 768px) {
            .sensor-grid {
                grid-template-columns: 1fr;
            }
            .container {
                padding: 20px;
            }
        }
    </style>
    <script>
        setTimeout(function() { location.reload(); }, 5000);
    </script>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Dashboard Home IoT</h1>
            <p>Monitoraggio in Tempo Reale</p>
        </div>
        
        <!-- CATEGORIA TEMPERATURA -->
        <div class="category">
            <div class="category-title"> Sensori Temperatura</div>
            <div class="sensor-grid">
                <div class="sensor-card temp-card">
                    <div class="sensor-name">Camera ...</div>
                    <div class="sensor-value">)" << current_temp << R"(</div>
                    <div class="sensor-status )" << (current_temp != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_temp != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                
                <div class="sensor-card temp-card">
                    <div class="sensor-name">Studio ...</div>
                    <div class="sensor-value">)" << current_temp_studio << R"(</div>
                    <div class="sensor-status )" << (current_temp_studio != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_temp_studio != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                
                <div class="sensor-card temp-card">
                    <div class="sensor-name">Sala Hobby</div>
                    <div class="sensor-value">)" << current_temp_sala_hobby << R"(</div>
                    <div class="sensor-status )" << (current_temp_sala_hobby != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_temp_sala_hobby != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
            </div>
        </div>
        
        <!-- CATEGORIA UMIDITÀ -->
        <div class="category">
            <div class="category-title">Sensori Umidity</div>
            <div class="sensor-grid">
                <div class="sensor-card temp-card">
                    <div class="sensor-name">Umidity Camera ...</div>
                    <div class="sensor-value">)" << current_umi_fed << R"(</div>
                    <div class="sensor-status )" << (current_umi_fed != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_umi_fed != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                
                <div class="sensor-card temp-card">
                    <div class="sensor-name">Umidity Studio ...</div>
                    <div class="sensor-value">)" << current_umi_studio << R"(</div>
                    <div class="sensor-status )" << (current_umi_studio != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_umi_studio != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                
                <div class="sensor-card temp-card">
                    <div class="sensor-name">HIC Camera ...</div>
                    <div class="sensor-value">)" << current_hic_fed << R"(</div>
                    <div class="sensor-status )" << (current_hic_fed != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_hic_fed != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
            </div>
        </div>
        
        <!-- CATEGORIA GAS -->
        <div class="category">
            <div class="category-title gas">Sensori Gas</div>
            <div class="sensor-grid">
                <div class="sensor-card gas-card">
                    <div class="sensor-name">Density Metano</div>
                    <div class="sensor-value">)" << current_metano_density << R"(</div>
                    <div class="sensor-status )" << (current_metano_density != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_metano_density != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                
                <div class="sensor-card gas-card">
                    <div class="sensor-name">Density Gas</div>
                    <div class="sensor-value">)" << current_gas_density << R"(</div>
                    <div class="sensor-status )" << (current_gas_density != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_gas_density != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
            </div>
        </div>
        
        <!-- CATEGORIA CONNETTIVITÀ -->
        <div class="category">
            <div class="category-title wifi">Connettivity</div>
            <div class="sensor-grid">
                <div class="sensor-card wifi-card">
                    <div class="sensor-name">Segnale WiFi</div>
                    <div class="sensor-value">)" << current_wifi_signal << R"(</div>
                    <div class="sensor-status )" << (current_wifi_signal != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_wifi_signal != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                <div class="sensor-card wifi-card">
                    <div class="sensor-name">Segnale WiFi Disconnesso</div>
                    <div class="sensor-value">)" << current_wifi_disconnected << R"(</div>
                    <div class="sensor-status )" << (current_wifi_disconnected != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_wifi_disconnected != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
            </div>
        </div>

        <!-- CATEGORIA CORRENTE -->
        <div class="category">
            <div class="category-title power">Power and current</div>
            <div class="sensor-grid">
                <div class="sensor-card power-card">
                    <div class="sensor-name">Potenza corrente</div>
                    <div class="sensor-value">)" << current_power << R"(</div>
                    <div class="sensor-status )" << (current_power != "N/A" ? "online" : "offline") << R"(">
                        )" << (current_power != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>
                
                <div class="sensor-card power-card">
                    <div class="sensor-name">Consumo kW</div>
                    <div class="sensor-value">)" << kw_consumption << R"(</div>
                    <div class="sensor-status )" << (kw_consumption != "N/A" ? "online" : "offline") << R"(">
                        )" << (kw_consumption != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>

                <div class="sensor-card power-card">
                    <div class="sensor-name">Volts</div>
                    <div class="sensor-value">)" << volts << R"(</div>
                    <div class="sensor-status )" << (volts != "N/A" ? "online" : "offline") << R"(">
                        )" << (volts != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>

                <div class="sensor-card power-card">
                    <div class="sensor-name">Corrente</div>
                    <div class="sensor-value">)" << current << R"(</div>
                    <div class="sensor-status )" << (current != "N/A" ? "online" : "offline") << R"(">
                        )" << (current != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>

                <div class="sensor-card power-card">
                    <div class="sensor-name">Potenza apparente</div>
                    <div class="sensor-value">)" << apparent_power << R"(</div>
                    <div class="sensor-status )" << (apparent_power != "N/A" ? "online" : "offline") << R"(">
                        )" << (apparent_power != "N/A" ? "ONLINE" : "OFFLINE") << R"(
                    </div>
                </div>


            </div>
        </div>
        
        <!-- STORICO -->
        <div class="history">
            <h3>Storico Letture</h3>)";

    if (readings.empty()) {
        response << "<p>Nessun dato disponibile</p>";
    }
    else {
        int count = 0;
        for (int i = readings.size() - 1; i >= 0 && count < 20; i--, count++) {
            response << "<div class='history-item'>";
            response << "<span class='timestamp'>" << readings[i].timestamp << "</span>";
            response << "<span class='timestamp'>" << readings[i].type << "</span>";
            response << "<span class='value'>" << readings[i].value << "</span>";
            response << "</div>";
        }
    }

    response << R"(
        </div>
        
        <div class="info">
            Server C++ integrato - Aggiornamento automatico ogni 5 secondi
        </div>
    </div>
</body>
</html>)";

    return response.str();
}

// Definizione del web server
void webServer() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData); // inizializzazione socket: obbligatoria prima di usare qualsiasi funzione di rete su Windows
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // creazione socket: AF_INET: famiglia IPv4 - SOCK_STREAM: socket di tipo stream, cioè TCP
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    /*
    Se il server si chiudesse improvvisamente (o Ctrl+C) e poi si proverebbe a riavviarlo immediatamente 
    sulla stessa porta si otterrebbe probabilmente Address already in use.
    Questo accade perché il sistema operativo mantiene la porta in uno stato chiamato 
    TIME_WAIT per alcuni minuti dopo la chiusura della connessione, 
    per assicurarsi che eventuali pacchetti ritardati non interferiscano con nuove connessioni.

    L'opzione SO_REUSEADDR dice al sistema: "permettimi di riutilizzare questo indirizzo 
    anche se è ancora in TIME_WAIT".
    */

    // Configurazione indirizzo e porta
    struct sockaddr_in address;
    address.sin_family = AF_INET;           // IPv4 (AF_INET) 
    address.sin_addr.s_addr = INADDR_ANY;   // INADDR_ANY: TUTTE le interfacce di rete disponibili
    address.sin_port = htons(8080);         // associazione porta 8080

    // Associazione all'indirizzo
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // Ascolto fino a 5 connessioni
    listen(server_fd, 5);

    std::cout << "[WEB] Server started on port 8080\n";
    std::cout << "[WEB] Access: http://192.168.1.xx:8080\n";

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        // Associazione client sulla socket del server e identificativo del client
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd > 0) {
            char buffer[1024];

            // Si riceve la richiesta HTTP del client (browser) e la si associa al buffer
            recv(client_fd, buffer, sizeof(buffer), 0);

            /*
            * In base alla richiesta buffer fa cose diverse: ROUTING
            * Ad esempio /, /index.html , /api/data, /about.html, ...
            */ 

            std::string html_response = createHTML();
            // Invio dell'HTML + header
            send(client_fd, html_response.c_str(), html_response.length(), 0);

#ifdef _WIN32
            closesocket(client_fd);
#else
            close(client_fd);
#endif
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif
}

int main() {
    std::cout << "Starting MQTT + Web Server new version...\n";

    // Avvia web server in thread separato
    std::thread web_thread(webServer);
    web_thread.detach(); // thread indipendente

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // MQTT setup
    mosquitto_lib_init(); // inizializzazione
    std::string client_id = "cpp-client-" + std::to_string(time(nullptr));
    mosquitto* m = mosquitto_new(client_id.c_str(), true, nullptr); // creazione cliente

    // Registrazione callback per eventi
    mosquitto_message_callback_set(m, on_message);
    mosquitto_connect_callback_set(m, on_connect);
    mosquitto_disconnect_callback_set(m, on_disconnect);

    // Inserimento delle credenziali per l'autenticazione
    mosquitto_username_pw_set(m, "your_username", "your_password");

    std::cout << "[MQTT] Connecting...\n";
    int rc = mosquitto_connect(m, "192.168.1.xx", port, 60); // connessione
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Connection failed: " << mosquitto_strerror(rc) << "\n";
        return 1;
    }

    std::cout << "System ready!\n";
    std::cout << "Dashboard: http://localhost:8080\n";
    std::cout << "LAN access: http://192.168.1.xx:8080\n\n";

    while (true) {
        rc = mosquitto_loop(m, 1000, 1);
        if (rc != MOSQ_ERR_SUCCESS) {
            std::cout << "Loop error: " << mosquitto_strerror(rc) << "\n";
            break;
        }
    }

    mosquitto_destroy(m);
    mosquitto_lib_cleanup();
    return 0;
}
