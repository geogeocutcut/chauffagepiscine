#include <OneWire.h> //Librairie du bus OneWire
#include <DallasTemperature.h> //Librairie du capteur

OneWire oneWire(2); //Bus One Wire sur la pin 2 de l'arduino
DallasTemperature sensors(&oneWire); //Utilistion du bus Onewire pour les capteurs
DeviceAddress sensorDeviceAddress; //V�rifie la compatibilit� des capteurs avec la librairie

void setup(void){
 Serial.begin(9600); //Permet la communication en serial
 sensors.begin(); //Activation des capteurs
 sensors.getAddress(sensorDeviceAddress, 0); //Demande l'adresse du capteur � l'index 0 du bus
 sensors.setResolution(sensorDeviceAddress, 12); //R�solutions possibles: 9,10,11,12
}

void loop(void){
 sensors.requestTemperatures(); //Demande la temp�rature aux capteurs
 Serial.print("La temp�rature est: ");
 Serial.print(sensors.getTempCByIndex(0)); //R�cup�ration de la temp�rature en celsius du capteur n�0
 Serial.println(" C�");
}