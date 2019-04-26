

// Enable debug prints
//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24
#define MY_RF24_PA_LEVEL RF24_PA_MAX

#define SKETCH_NAME "Pool Heater"
#define SKETCH_MAJOR_VER "1"
#define SKETCH_MINOR_VER "0"

#define MY_NODE_ID 5

#define TEMP_ID 1    // Id of the sensor child
#define RELAY_ID 2   // Id of the sensor child

#define RELAY_PIN  4  // Arduino Digital I/O pin number for relay 
#define BUTTON_PIN  3  // Arduino Digital I/O pin number for button 
#define TEMP_PIN  2  // Arduino Digital I/O pin number for button 
#define RELAY_ON 1
#define RELAY_OFF 0

uint32_t TEMPO_TIME = 10000; 


#include <MySensors.h>

#include <OneWire.h> //Librairie du bus OneWire
#include <DallasTemperature.h> //Librairie du capteur

bool buttonState = false;
bool relayState;

float Temp_tmp=0 ;               // 7 création d'une variable temporaire de la températude du ballon d'eau chaude
float Temp_good = 0;      // 7 variable flotante pour récupérer la température 

OneWire oneWire(TEMP_PIN); //Bus One Wire sur la pin 2 de l'arduino
DallasTemperature sensors(&oneWire); //Utilistion du bus Onewire pour les capteurs
DeviceAddress sensorDeviceAddress; //Vérifie la compatibilité des capteurs avec la librairie

unsigned long prev_millis;

MyMessage msgRelay(RELAY_ID,V_LIGHT);
MyMessage msgTemp(TEMP_ID, V_TEMP);

void setup()
{
    // Setup the button
    pinMode(BUTTON_PIN,INPUT_PULLUP);

    // Make sure relays are off when starting up
    digitalWrite(RELAY_PIN, RELAY_OFF);
    // Then set relay pins in output mode
    pinMode(RELAY_PIN, OUTPUT);   

    // Set relay to last known state (using eeprom storage) 
    relayState = loadState(RELAY_ID);
    digitalWrite(RELAY_PIN, relayState?RELAY_ON:RELAY_OFF);


    sensors.begin(); //Activation des capteurs
    sensors.getAddress(sensorDeviceAddress, 0); //Demande l'adresse du capteur à l'index 0 du bus
    sensors.setResolution(sensorDeviceAddress, 12); //Résolutions possibles: 9,10,11,12
}

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo(SKETCH_NAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

    // Register this device as power sensor
    present(RELAY_ID, S_LIGHT);
    delay(100);
    present(TEMP_ID, S_TEMP);
    delay(100);
}

void loop() 
{
    
    // Get the update value
    int readButtonImpulsion = digitalRead(BUTTON_PIN);  
    if (readButtonImpulsion == 0 )
    {
        buttonState = !buttonState;
        if(buttonState==true && relayState ==false) 
        {
            send(msgRelay.set(true), true); // Send new state and request ack back
        }
    }
    sensors.requestTemperatures(); //Demande la température aux capteurs
    Temp_tmp=sensors.getTempCByIndex(0);             // 7 Récupération de la température en celsius à l'index 0
    if(millis() - prev_millis > TEMPO_TIME) 
    { 
        if (Temp_tmp>0)                     //7 pour éviter les sauts de valeur négative de la sonde dallas (genre artefact)
        {
            Temp_good=Temp_tmp; //7 si la temp >0 on récupère la valeur de la température sinon laisser l'artefact de coté)
            send(msgTemp.set(Temp_good, 1));
        }
        prev_millis = millis();
        

        if(buttonState==false)
        {
            if(relayState==false && Temp_good>=50)
            {
                send(msgRelay.set(true), true);
            }  
            if(relayState==true && Temp_good<50)
            {
                send(msgRelay.set(false), true);
            }  
        } 
    }
    
} 

void receive(const MyMessage &message) {
  if (message.type == V_LIGHT) {
     // Change relay state
     relayState = message.getBool();
     digitalWrite(RELAY_PIN, relayState?RELAY_ON:RELAY_OFF);
     // Store state in eeprom
     saveState(RELAY_ID, relayState);
   } 
}