# mqtt-sensor-dashboard
Sistema di monitoraggio IoT in tempo reale sviluppato in C++. Il sistema si connette a un broker MQTT per raccogliere dati dai sensori (temperatura, umidità, gas) e fornisce una dashboard web locale accessibile da qualsiasi dispositivo della rete LAN. Include gestione thread-safe dei dati, riconnessione automatica e interfaccia web responsive.

## Idea del progetto
L'obiettivo era raccogliere dati da sensori di temperatura domestici attraverso la rete.

> [!NOTE]
> Un **sensore** che si collega alla rete: ( dotato di una scheda di rete ) è considerato come un piccolo computer. 
> Si tratta di un **chip** con la **CPU**, la **RAM**, l'**interfaccia di rete** e un **firmware**.

*E come qualunque computer connesso, può comunicare usando diversi **protocolli di rete**.*
> I protocolli definiscono le regole per la comprensione dello scambio di dati.

### Caratteristiche dei sensori IoT 
- Connessione Wi-Fi, Ethernet oppure altro.
- Proprio indirizzo IP nella rete locale LAN
- Utilizzo di protocolli di trasporto **TCP/UDP** sopra IP
	- TCP (connessione affidabile)
	- UDP (più leggero, meno affidabile)
- **Applicazione**: qui arriva il protocollo vero e proprio: HTTP, MQTT, Modbus/TCP, CoAP, SNMP, …

Quindi per comunicare c'è bisogno di definire un **protocollo applicativo** e una **porta di rete**.

### MQTT
In questo caso si utilizza MQTT come protocollo applicativo, un protocollo di messaggistica leggero che utilizza TCP/IP.
Questo protocollo utilizza un concetto a **publish / subscribe**.

Il **broker** è un **server MQTT** che rappresenta il cuore del sistema. Esistono diversi tipi, tra cui Mosquitto, HiveMQ, EMQX.
Tutti i sensori e i programmi si collegano al broker e inviano i loro dati. Il broker si occuperà dello smistamento dei dati ai client interessati. Perciò i client non comunicano tra loro direttamente.
Si può pensare al broker come a un ufficio postale.

Chiunque parli MQTT è un **client**:
- Un sensore di temperatura → pubblica i valori.
- Un’app mobile → si sottoscrive per leggere i valori.
- Un altro dispositivo → riceve comandi tramite MQTT.
I client non comunicano mai direttamente tra loro: passano sempre dal broker.

I messaggi viaggiano su **topic**: che sono stringhe tipo percorsi:
```
sensors/bedroom/temperature
sensors/bedroom/humidity
actuators/lamp1/state
```
Si possono usare **wildcard**:
- `sensors/#` → tutto sotto `sensors/…`
- `sensors/+/temperature` → tutti i sensori, ma solo il livello `temperature`.
Si può pensare ai topic come a “canali radio”: tu decidi a quale frequenza sintonizzarti.

Per richiedere o inviare dei dati si utilizzano due operazioni:
- **Publish**: un client invia un messaggio/dato al broker su un topic.
- **Subscribe**: un client dice al broker “voglio ricevere tutti i messaggi su questo topic”.

Il broker fa da “smistatore”: chi pubblica non sa chi riceve, chi riceve non sa chi pubblica.

## Cosa ho fatto?
Cosa avevo a disposizione:
- Topic del broker MQTT
- Indirizzo e port seguenti del broker MQTT: ...

Ho deciso di utilizzare la libreria **libmosquitto** per la sua semplicità e poiché paho.mqtt.cpp dava tanti problemi.

Per installare le librerie necessità di vcpkg: **package manager** per librerie C/C++ su Windows (ma anche Linux/macOS).
Evita di scaricare a mano i sorgenti, compilare e linkare: un po’ come `pip` in Python o `npm` in JavaScript, ma per C++.

Per installare **vcpkg**: 
- `git clone https://github.com/microsoft/vcpkg.git`
- `cd vcpkg`
- `.\bootstrap-vcpkg.bat`
Alla fine si troverà `vcpkg.exe` nella cartella.

A questo punto si può installare **libmosquitto**: 
- `.\vcpkg.exe install mosquitto:x64-windows`
- `C:\Users\...\vcpkg\vcpkg.exe install mosquitto:x64-windows`

A questo punto ho scritto il codice sorgente tutto in un unico file e il CMakeLists.txt.

### Build ed esecuzione del progetto
Creare la cartella build direttamente da terminale:
- `mkdir build`  e `cd build`

Poi si rigenera e rebuilda con:
- genera i file di progetto (Makefiles, ...) : `cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/Users/.../vcpkg/scripts/buildsystems/vcpkg.cmake`
- build del progetto: `cmake --build . --config Release`

Per l'esecuzione si ha:
`.\Release\mqtt_mosq_min.exe`

Ovviamente deve essere fatto tutto da dentro la cartella /build.


## Web server
Una volta ottenuti i dati via rete con il protocollo MQTT il mio obiettvo è quello di **creare un web server all'interno della propria rete locale LAN con una dashboard relativa ai dati ottenuti.**

Questa dashboard mostra i valori dei sensori (temperatura, umidità, ...) in real time.

Il nostro **web server HTTP** girerà sulla macchina locale e fungerà da sito web per i dispositivi connessi alla LAN.
- Perciò l'indirizzo sarà relativo a quello della macchina: ...
- Per controllare l'indirizzo utilizzare `ipconfig` nella sezione **Wireless LAN adapter Wi-Fi**. 
- Qualsiasi device connesso al Wi-Fi (o cablato) potrà aprire quell’URL dal browser.

Sostanzialmente :
- Quando arriva un messaggio MQTT, aggiorna una variabile condivisa
- Il server web legge questa variabile e genera HTML dinamico

##### Come permetto di settare il web server non in localhost ma nella mia LAN?
La differenza chiave è dove si fa il bind del server.
Per renderlo raggiungibile da qualsiasi host all'interno della LAN si deve fare il bind su:
- `0.0.0.0` (tutte le interfacce) **oppure**
- l’IP LAN della tua macchina (es. `192.168.1.x`)

Quindi:
```cpp
setAddressAndPort("127.0.0.1", 8080); // localhost
setAddressAndPort("0.0.0.0", 8080); // tutte le interfacce
setAddressAndPort("192.168.1.x", 8080);
```

Oppure si può utilizzare: `address.sin_addr.s_addr = INADDR_ANY;`
`INADDR_ANY` è una costante che vale `0.0.0.0` e dice al socket: **"Ascolta su TUTTE le interfacce di rete disponibili"**.

Un computer ha multiple interfacce:
- **Loopback** (127.0.0.1) - solo locale: accessibile solo da: `http://127.0.0.1:8080` o `http://localhost:8080`
- **WiFi** (es. 192.168.1.x) - rete locale
- **Ethernet** (es. 192.168.1.x) - rete cablata
- **VPN** (es. 10.0.0.5) - se attiva

Grazie alla configurazione del programma con `INADDR_ANY` il web server sarà accessibile da:
- `http://127.0.0.1:8080` (locale)
- `http://192.168.1.x:8080` (da altri PC in LAN)
- `http://[nome-computer]:8080` (risoluzione DNS locale)
Questo perché c'è un socket che ascolta su TUTTE le interfacce per porta 8080.

###### Firewall
Il binding su `INADDR_ANY` è necessario ma non sufficiente. Questo perché è necessario fare i conti con il Firewall che blocca le connessioni in entrata su alcune porte. A tale proposito è necessario sbloccare le connessioni sulla porta 8080 (dov'è il web server) per far sì che gli altri host all'interno della LAN possano raggiungere e visita il piccolo sito web (web server).


### Spiegazione codice
##### Include e Definizioni
```cpp
#define NOMINMAX
#include <mosquitto.h>
#include <iostream>
// ... altri include
```
- `NOMINMAX`: Previene conflitti con macro Windows
- `mosquitto.h`: Libreria per comunicazione MQTT
- Include standard C++ per I/O, thread, sincronizzazione

##### Strutture Dati
```cpp
struct SensorReading {
    std::string timestamp;
    std::string type;
    std::string value;
};
```
Struttura per memorizzare una singola lettura del sensore.

##### Variabili Globali
```cpp
std::vector<SensorReading> readings;
std::mutex data_mutex;
std::string current_temp = "N/A";
// ... altre variabili sensori
```
- `readings`: Storico delle letture (max 20)
- `data_mutex`: Mutex per accesso thread-safe ai dati
- Variabili stringa per ogni tipo di sensore

##### Funzione Timestamp
```cpp
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
```
Genera timestamp nel formato "YYYY-MM-DD HH:MM:SS".

##### Callback MQTT - on_message
**Funzionamento**:
1. Converte il payload in stringa
2. Stampa messaggio ricevuto
3. **Lock del mutex** per accesso sicuro ai dati
4. **Switch sui topic**: ogni topic aggiorna la variabile corrispondente
5. **Aggiorna timestamp** e aggiunge alla cronologia
6. **Mantiene solo 20 letture** (cancella la più vecchia se necessario)
```cpp
void on_message(struct mosquitto*, void*, const struct mosquitto_message* msg) {
    std::string payload((char*)msg->payload, msg->payloadlen);
    std::cout << "[MQTT] " << msg->topic << " => " << payload << "\n";

    std::lock_guard<std::mutex> lock(data_mutex);
    std::string topic(msg->topic);
    
    if (topic == "/.../....") {
        current_temp = payload;
        last_updated = getTimestamp();
        
        SensorReading reading = { last_updated, "Temperatura stanza ", payload};
        readings.push_back(reading);
        
        if (readings.size() > 20) {
            readings.erase(readings.begin());
        }
    }
    // ... altri topic
}
```

##### Callback MQTT - on_connect
Si sottoscrive a tutti i topic quando la connessione MQTT è stabilita.
```cpp
void on_connect(struct mosquitto* mosq, void*, int result) {
    if (result == 0) {
        std::cout << "[MQTT] Connected!\n";
        mosquitto_subscribe(mosq, nullptr, "/..../....", 1);
        mosquitto_subscribe(mosq, nullptr, "/..../....", 1);
        // ... altre sottoscrizioni
    }
    else {
        std::cout << "[MQTT] Connection failed\n";
    }
}
```

##### Callback MQTT - on_disconnect
Semplice notifica di disconnessione.
```cpp
void on_disconnect(struct mosquitto*, void*, int) {
    std::cout << "[MQTT] Disconnected\n";
}
```

##### Web Server Function
```cpp
void webServer() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
```
**Setup socket**:
1. **Inizializzazione Winsock** (solo Windows): obbligatoria prima di usare qualsiasi funzione di rete su Windows
2. **Creazione socket TCP**:
	1.  **AF_INET**: famiglia IPv4 
	2. **SOCK_STREAM**: socket di tipo stream, cioè TCP
3. **setsockopt(...)**: Permette riuso immediato della porta, cioè:
	- Se il server si chiudesse improvvisamente (o Ctrl+C) e poi si proverebbe a riavviarlo immediatamente sulla stessa porta si otterrebbe probabilmente Address already in use.
	- Questo accade perché il sistema operativo mantiene la porta in uno stato chiamato  TIME_WAIT per alcuni minuti dopo la chiusura della connessione, per assicurarsi che eventuali pacchetti ritardati non interferiscano con nuove connessioni.
	- L'opzione SO_REUSEADDR dice al sistema: "permettimi di riutilizzare questo indirizzo anche se è ancora in TIME_WAIT".


**Binding e ascolto**:
1. **Configura indirizzo**: porta 8080, tutte le interfacce
2. **Bind**: Associa socket all'indirizzo
3. **Listen**: Accetta fino a 5 connessioni in coda
```cpp
    // Configurazione indirizzo e porta
    struct sockaddr_in address;
    address.sin_family = AF_INET;      // IPv4 (AF_INET) 
    address.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY: TUTTE le interfacce di rete disponibili
    address.sin_port = htons(8080);  // associazione porta 8080

	// Associazione all'indirizzo
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    
    // Ascolto fino a 5 connessioni
    listen(server_fd, 5);
```

**Loop principale**:
1. **Accept**: Attende connessioni client a cui viene associato un identificativo (int client_fd)
2. **Receive**: Si riceve la richiesta HTTP del client (browser) e la si associa al buffer.
	1. In base alla richiesta buffer fa cose diverse: ROUTING
		1. Ad esempio */, /index.html , /api/data, /about.html, ...*
In questo caso il server ignora la richiesta (buffer). Risponde sempre con la stessa pagina HTML, indipendentemente da cosa chiede il client.

3. **Generate HTML**: Chiama createHTML() che crea l'HTML corrispondente alla richiesta.
4. **Send**: Invia risposta completa (header + HTML)
5. **Close**: Chiude connessione (HTTP non persistente)
```cpp
while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd > 0) {
            char buffer[1024];
            recv(client_fd, buffer, sizeof(buffer), 0);

            std::string html_response = createHTML();
            send(client_fd, html_response.c_str(), html_response.length(), 0);

            close(client_fd);
        }
    }
```

##### Main Function 
```cpp
int main() {
    std::cout << "Starting MQTT + Web Server new version...\n";

    // Avvia web server in thread separato
    std::thread web_thread(webServer);
    web_thread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));
```
- **Thread separato** per web server
- **Detach**: Thread indipendente
- **Sleep**: Attende avvio web server

```cpp
// MQTT setup
    mosquitto_lib_init();
    std::string client_id = "cpp-client-" + std::to_string(time(nullptr));
    mosquitto* m = mosquitto_new(client_id.c_str(), true, nullptr);

    mosquitto_message_callback_set(m, on_message);
    mosquitto_connect_callback_set(m, on_connect);
    mosquitto_disconnect_callback_set(m, on_disconnect);

    mosquitto_username_pw_set(m, "username", "passw");
```
**Setup MQTT**:
1. **Inizializzazione libreria** mosquitto
2. **Client ID univoco** con timestamp
3. **Creazione client** MQTT
4. **Registrazione callback** per eventi
5. **Credenziali** per autenticazione

```cpp
int rc = mosquitto_connect(m, "address", port, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Connection failed: " << mosquitto_strerror(rc) << "\n";
        return 1;
    }
```
**Connessione MQTT**:
- **IP broker**: address
- **Porta**: porta
- **Keepalive**: 60 secondi

```cpp
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
```
**Loop principale MQTT**:
1. **mosquitto_loop**: Gestisce I/O MQTT (1 secondo timeout)
2. **Controllo errori**: Esce se problemi di connessione
3. **Cleanup**: Distrugge client e pulisce libreria

### CMakeLists spiegazione
Si utilizza **CMake** come sistema di building per la sua gestione della compatibilità cross-platform automatica. Quindi è in grado di compilare lo stesso progetto su Windows (con Visual Studio), Linux (con g++) e Mac (con clang).

Questo perché utilizzare compilare ed eseguire da terminale potrebbe risultare complesso dato che è necessario includere dei flag e path per mosquitto.
- **Percorsi delle librerie:** Su Windows con vcpkg, mosquitto potrebbe essere in `C:/vcpkg/installed/x64-windows/`, su Linux magari in `/usr/lib/`, su Mac in `/opt/homebrew/`.
- **Estensioni dei file:** Windows usa `.dll`, Linux `.so`, Mac `.dylib`. 
- **Nomi delle librerie:** A volte si chiamano `libmosquitto` su Linux ma `mosquitto.lib` su Windows. 

##### Setup base del progetto
```bash 
cmake_minimum_required(VERSION 3.20)
```
Indica che questo progetto ha bisogno di CMake versione 3.20 o superiore per funzionare.

```bash
project(mqtt_mosq_min CXX)
```
Definisce:
- Nome progetto: `mqtt_mosq_min`
- `CXX` = linguaggio C++ (non C puro)
- CMake ora sa che deve cercare un compilatore C++

```bash 
set(CMAKE_CXX_STANDARD 17)
```
Usa C++17 per compilare (equivale al flag `-std=c++17`)

##### Ricerca librerie
```bash 
find_package(unofficial-mosquitto CONFIG REQUIRED)
```
- Cerca la libreria mosquitto installata da vcpkg
- `unofficial-` = prefix che vcpkg usa per distinguere i suoi pacchetti
- `REQUIRED` = se non la trova, ferma tutto con errore
- CMake ora sa dove sono headers e librerie di mosquitto

```bash 
add_executable(mqtt_mosq_min main.cpp)
```
Crea un eseguibile chiamato `mqtt_mosq_min` compilando `main.cpp` (Equivale a `g++ -o mqtt_mosq_min main.cpp` ma senza le librerie)

##### Linking librerie
```bash 
target_link_libraries(mqtt_mosq_min PRIVATE unofficial::mosquitto::mosquitto)
```
- Collega la libreria mosquitto al nostro eseguibile
- `::` = sintassi moderna CMake per i "target" (più sicura)
- `PRIVATE` = solo questo eseguibile usa mosquitto

##### Sezione Windows
```bash 
if(WIN32)
    target_link_libraries(mqtt_mosq_min PRIVATE ws2_32 crypt32)
```
- `WIN32` = "se stiamo compilando su Windows"
- `ws2_32` = libreria Windows per socket/networking
- `crypt32` = libreria Windows per crittografia
- Mosquitto su Windows ha bisogno di queste due librerie del sistema

```bash 
target_compile_definitions(mqtt_mosq_min PRIVATE *WIN32*WINNT=0x0601)
```
- Definisce una macro `_WIN32_WINNT=0x0601`
- `0x0601` = Windows 7 o superiore
- Equivale a `#define _WIN32_WINNT 0x0601` nel codice
- Mosquitto ha bisogno di sapere quale versione Windows supportare

##### Sezione Linux/Mac
```bash 
else() find_package(Threads REQUIRED) target_link_libraries(mqtt_mosq_min PRIVATE Threads::Threads) endif()
```
- Su Linux/Mac, mosquitto ha bisogno del supporto per thread
- `Threads::Threads` = libreria pthread (equivale a `-lpthread`)

##### Copia Automatica DLL (Windows)
```bash 
if(WIN32)
    set(VCPKG_ROOT "C:/Users/.../vcpkg/installed/x64-windows")
    add_custom_command(TARGET mqtt_mosq_min POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VCPKG_ROOT}/bin/mosquitto.dll"
        $<TARGET_FILE_DIR:mqtt_mosq_min>
        COMMENT "Copying mosquitto.dll")
endif()
```
- **Problema:** Su Windows, l'eseguibile ha bisogno del file `mosquitto.dll` nella stessa cartella
- **Soluzione:** Dopo aver compilato (`POST_BUILD`), copia automaticamente la DLL accanto all'eseguibile
- `copy_if_different` = copia solo se necessario (più veloce)

```bash 
message(STATUS "Configuration complete!")
```
Stampa un messaggio durante la configurazione (come un `printf`)

## Preview Dashboard
<img width="300" height="500" alt="dashboard" src="https://github.com/user-attachments/assets/172bedaf-524f-43e4-b6fe-dbe73270536f" />



