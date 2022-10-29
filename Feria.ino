//Bluetooth
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial BT;

//Sensor DHT11
#include "DHT.h" //se incluye librería DHT
#define DHTPIN 25 // se define el sensor en el pin 32
#define DHTTYPE DHT11 //el sensor es un DHT11
DHT dht(DHTPIN, DHTTYPE);

//Botón
int butA= 35; //El botón A se le asigna al pin 4
int butB= 34; //El botón B se le asigna al pin 15
boolean timeA = LOW; 
boolean timeB = LOW;
boolean presionaBoton(int i); //función para presionar un botón


//FUNCIÓN DEL BLUETOOTH
void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_START_EVT) {
    Serial.println("Inicializado SPP");
  }
  else if (event == ESP_SPP_SRV_OPEN_EVT ) {
    Serial.println("Cliente conectado");
  }
  else if (event == ESP_SPP_CLOSE_EVT  ) {
    Serial.println("Cliente desconectado");
  }
  else if (event == ESP_SPP_DATA_IND_EVT ) {
    Serial.println("Datos recibidos");
    while (BT.available()) { // Mientras haya datos por recibir
      int h = dht.readHumidity(); //Mide la humedad
      int t = dht.readTemperature(); //Mide la temperatura en grados Celcius
      int f = dht.readTemperature(true); //Mide la temperatura en grados Fahrenheit
      int incoming = BT.read(); // Lee un byte de los datos recibidos
      Serial.print("Recibido: ");
      Serial.println(incoming);
      if (incoming == 49) { // 1 en ASCII, mide la temperatura
     BT.println("La temperatura en grados Celsius es de: ");
     BT.print(t);
     BT.print("°C ");
     BT.println(", o bien");
     BT.println("En grados Fahrenheit es de: ");
     BT.print(f);
     BT.print("°F ");
     BT.println("");
     BT.println("");     
     delay(1000); //tiempo de espera de un segundo        
      }
      else if (incoming == 50) { // 2 en ASCII, mide la humedad
     BT.println("La humedad es de: ");
     BT.print(h);
     BT.println("");
     BT.println("");     
     delay(1000); //tiempo de espera de un segundo         
      }     
    }
  }
}


//FUNCIÓN SETUP
void setup() {
    //DHT11
    Serial.begin(115200); //se inicia la comunicación serial
    dht.begin(); //se inicia el sensor
    //Botón
    pinMode(butA,INPUT); //botón A como salida   
    pinMode(butB,INPUT); //botón B como salida       
    //Bluetooth
    Serial.begin(115200);
    BT.begin("Divulgación"); // Nombre de tu Dispositivo Bluetooth y en modo esclavo
    Serial.println("El dispositivo Bluetooth está listo para emparejar");
    BT.register_callback(callback_function);   
}


//FUNCIÓN LOOP
void loop() {
    float h = dht.readHumidity(); //Mide la humedad
    float t = dht.readTemperature(); //Mide la temperatura en grados Celcius
    float f = dht.readTemperature(true); //Mide la temperatura en grados Fahrenheit

     //si se presiona el botón A
     if(presionaBoton(butA)==HIGH){
      BT.println("La temperatura en grados Celsius es de: ");
      BT.print(t);
      BT.print("°C ");
      BT.println(", o bien");
      BT.println("En grados Fahrenheit es de: ");
      BT.print(f);
      BT.print("°F ");
      BT.println("");
      BT.println("");     
      delay(1000); //tiempo de espera de un segundo
     }

     //si se presiona el botón B
     if(presionaBoton(butB)==HIGH){
      BT.println("La humedad es de: ");
      BT.print(h);
      BT.println("");
      BT.println("");     
      delay(1000); //tiempo de espera de un segundo
     }    
}


//FUNCIÓN BUTÓN
boolean presionaBoton(int i){
    timeB=digitalRead(i); //se le el estado del botón
    boolean resultado=LOW; //el estado es de 0
    if(timeA==LOW &&  timeB==HIGH){//si el estado anterior es apagado y el actual prendido
      resultado=HIGH; //el estado cambia a 1
    }
    timeA=timeB; //el estado anterior se vuelve el actual
    return resultado;
    delay(100); //hay un tiempo de espera de cien milisegundo    
} 
