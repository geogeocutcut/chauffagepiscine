#include <OneWire.h> //Librairie du bus OneWire
#include <DallasTemperature.h> //Librairie du capteur

OneWire oneWire(2); //Bus One Wire sur la pin 2 de l'arduino
DallasTemperature sensors(&oneWire); //Utilistion du bus Onewire pour les capteurs
DeviceAddress sensorDeviceAddress; //Vérifie la compatibilité des capteurs avec la librairie

void setup(void){
 Serial.begin(9600); //Permet la communication en serial
 sensors.begin(); //Activation des capteurs
 sensors.getAddress(sensorDeviceAddress, 0); //Demande l'adresse du capteur à l'index 0 du bus
 sensors.setResolution(sensorDeviceAddress, 12); //Résolutions possibles: 9,10,11,12
}

void loop(void){
 sensors.requestTemperatures(); //Demande la température aux capteurs
 Serial.print("La température est: ");
 Serial.print(sensors.getTempCByIndex(0)); //Récupération de la température en celsius du capteur n°0
 Serial.println(" C°");
}