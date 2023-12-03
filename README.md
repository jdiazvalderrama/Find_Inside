# Find_Inside: Localización de Dispositivos con WiFi en Instituciones

## Descripción
**Find_Inside** es una solución innovadora que utiliza la tecnología WiFi para localizar dispositivos de valor en instituciones con infraestructura WiFi disponible.

## Microcontrolador
Se utiliza el **ESP32 WROVER** equipado con soporte para baterías tipo 18650, optimizando el rendimiento y la autonomía del dispositivo.

## Requisitos
- **Microcontrolador:** Un ESP32, preferiblemente el modelo WROVER con soporte para batería 18650. Sin embargo, se puede utilizar cualquier variante de la familia ESP32 que disponga de conectividad WiFi.
- **Configuración:** Acceso a un servidor para subir la información, plano del edificio, mapeo de puntos de acceso WiFi y datos de una red WiFi (SSID y contraseña).

## Funcionamiento del Microcontrolador
1. **Carga del Código:** Instalar el código `.ino` (disponible en el repositorio) en el ESP32, teniendo en cuenta los requisitos previos.
2. **Recolección de Datos:** El ESP32 detectará y registrará redes WiFi disponibles, almacenando información como voltaje de la batería, MAC propia, SSID, BSSID y RSSI.
3. **Conexión y Transmisión:** Conexión a la red WiFi especificada y envío de los datos al servidor.
4. **Modo de Ahorro de Energía:** El ESP32 entrará en modo Deep Sleep por un periodo definido por el usuario para conservar la batería (por defecto, 1 minuto).

## Funcionamiento en el Servidor
- **Recolección de Datos:** El servidor recopila la información enviada por el ESP32.
- **Procesamiento:** Se añade la hora de recepción al inicio de cada mensaje recibido y se almacenan los datos en un archivo CSV.
- **Preparación del Entorno:** Se debe disponer del plano del edificio y de un mapeo de los puntos de acceso WiFi.

## Funcionamiento General
Al recibir datos del ESP32, el servidor filtra y compara las redes WiFi con las MAC de los puntos de acceso mapeados. Al identificar coincidencias, se analiza la señal RSSI para resaltar en el mapa los puntos de acceso detectados por el ESP32 con señal más fuerte. Esto permite determinar aproximadamente el piso y la zona del edificio donde se encuentra el Tag WiFi.
