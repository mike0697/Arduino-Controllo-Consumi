//slave 
//riservato solo al pinza amperometrica
#include <Wire.h>


//stc
#include "EmonLib.h"    // Include Emon Library               
//Calibrations
const int volt = 220;   // tensione per il calcolo della potenza 
const float ct_calibration = 29;
const int currentSensorPin = A2;

//variabili fisse non cambiabili
float tempValue = 0;
float Irms = 0;
float power =0;
int dati_inviati=0;
int dati_inviati_irms=0;
// Create an Emon instance
EnergyMonitor emon1;  
int n=0;
void setup()
{
  pinMode(2, INPUT);    // pin della pinza amperoemtrica
  pinMode(6, OUTPUT);   // pin dei rele
  pinMode(7, OUTPUT);   // pin dei rele
  Wire.begin(2);        // entra nel bus I2C con indirizzo 2
  // esegue la funzione quando ha una richiesta
  Serial.begin(9600);
  Serial.println("Sensors test starting...");
  Serial.println("Slave");
  digitalWrite(7, HIGH);  // rele high
  digitalWrite(6, HIGH);  // rele high
  //initialize Emon instance
  emon1.current(currentSensorPin, ct_calibration); 
  Wire.onRequest(requestEvent);
}
void loop()
{
  
  //n=n+1;
  Irms = emon1.calcIrms(1480); // legge la corrente
  Serial.print(" ; Power : ");
  Serial.println(Irms*volt); // mostra la potenza sul monitor seriale
  Serial.println(Irms);  // mostra la corrente sul monitor seriale
  power =Irms*volt;    
 // int dati_inviati = (int)power; 
 if (power<9.0)  //a tensione 0 puo dare letture non corrette intorno allo zero.
 {
  power =0;    // per cui si approssima a 0
 }
 Serial.print(n);
 // si puo eseguire il controllo dei rele  ma viene effettuato sul arduino  "master"
  /*if(n>3)
  {
  if(power >= 120)
  {
    
    digitalWrite(6, LOW);
    Serial.print("spegni");
  }
  
  }*/
  
  delay(100);

}

void requestEvent()
{

int dati_inviati = (int)power; 
 //Serial.print(" invio ");  //istruzione per il debug dell i2c
 //int dati_inviati_irms = (int)Irms;  //istruzione per inviare anche la corrente 
  Wire.write(dati_inviati);       // invia la potenza
}
