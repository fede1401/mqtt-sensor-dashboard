cmake_minimum_required(VERSION 3.20)
project(mqtt_mosq_min CXX)
set(CMAKE_CXX_STANDARD 17)

# Trova il pacchetto unofficial-mosquitto (come installato da vcpkg)
find_package(unofficial-mosquitto CONFIG REQUIRED)

add_executable(mqtt_mosq_min main.cpp)

# Usa il target corretto di vcpkg
target_link_libraries(mqtt_mosq_min PRIVATE unofficial::mosquitto::mosquitto)

# Librerie Windows necessarie
if(WIN32)
    target_link_libraries(mqtt_mosq_min PRIVATE ws2_32 crypt32)
    # Aggiungi definizioni per Windows
    target_compile_definitions(mqtt_mosq_min PRIVATE _WIN32_WINNT=0x0601)
else()
    # Su Linux/Mac
    find_package(Threads REQUIRED)
    target_link_libraries(mqtt_mosq_min PRIVATE Threads::Threads)
endif()

# Copia automaticamente le DLL necessarie dopo il build
if(WIN32)
    set(VCPKG_ROOT "C:/Users/.../vcpkg/installed/x64-windows")
    add_custom_command(TARGET mqtt_mosq_min POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VCPKG_ROOT}/bin/mosquitto.dll"
        $<TARGET_FILE_DIR:mqtt_mosq_min>
        COMMENT "Copying mosquitto.dll")
endif()

message(STATUS "Configuration complete!")
