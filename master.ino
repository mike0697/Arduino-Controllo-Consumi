#include <SoftwareSerial.h>
//Includiamo la libreria per la gestione delle istruzioni di utilizzo del modulo bluetooth

const int rxpin = 4;  
//Andiamo ad assegnare al pin 3 l’indirizzo di ricezione dati (e lo colleghiamo al pin TXD del modulo)
const int txpin = 3;  
//Assegnamo invece al pin 3 l’indirizzo di trasmissione dati (collegandolo al pin RXD del nostro componente
unsigned int dato=160;
SoftwareSerial bluetooth(rxpin, txpin);  
//Assegnamo ad un nome (bluetooth) i suoi pin di ricezione e trasmissione, per facilitarci la scrittura del codice
char c;
#include <Wire.h>
int soglia_massima=130; 
int soglia_minima=35;
int controllo=1; 
int stato_rele1=1;
int stato_rele2=1;
int rele=7;  // pin dei rele con minore priorità
int rele2=6; // pin dei rele
String message; //stringa per leggere l i2c
void setup(){
pinMode(rele2, OUTPUT);    // uscita rele output
pinMode(rele, OUTPUT);     // uscita rele output
digitalWrite(rele, stato_rele2);  // rele high
digitalWrite(rele2, stato_rele1); // rele high
Wire.begin();
Serial.begin(9600);  
//Inizializziamo l’interfaccia seriale al baud rate dell’arduino
Serial.println("avvio");
bluetooth.begin(9600);  
//Inizializziamo l’interfaccia del modulo bluetooth sempre al bit rate dell'arduino
}

void loop(){
Wire.requestFrom(2, 1); // richiedi 1 byte al dispositivo presente all'indirizzo 2.
int b = 0;
int c = Wire.read(); // leggi il byte ricevuto
//Serial.println(c);
b = b + c;
Serial.print(b);Serial.println("W");
if (b > soglia_massima)
  {
    Serial.println("luce accesa");
    delay(2000);
    stato_rele1=0;
    digitalWrite(rele, stato_rele1);
    delay(20);
  }
  else
  {
    Serial.println("luce spenta");
    delay(20);
  }

while(bluetooth.available()){
    message+=char(bluetooth.read());
  }
  if(!bluetooth.available())
  {
    if(message!="")
    {
      Serial.println("connesso"); //show the datas
      Serial.println(message);
      delay(200);
    if(message=="O")
    {
        Serial.println("rele2 acceso");
        stato_rele2=1;
        digitalWrite(rele2, stato_rele2);
        bluetooth.println("rele2 acceso");
        delay(200);
        message="";
    }
    if(message=="D")
    {
        Serial.println("rele2 spento");
        stato_rele2=0;
        digitalWrite(rele2, stato_rele2);
        bluetooth.println("rele2 spento");
        delay(200);
        message="";
    }
     if(message=="H")
    {
        Serial.println("rele1 ACCESO");
        stato_rele1=1;
        digitalWrite(rele, stato_rele1);
        bluetooth.println("rele1 acceso");
        delay(200);
        message="";
    }
     if(message=="L")
    {
        Serial.println("rele1 spento");
        stato_rele1=0;
        digitalWrite(rele, stato_rele1);
          bluetooth.println("rele1 spento");
        delay(200);
        message="";
    }
     if(message=="A")
    {
        Serial.println("reset rele");
        stato_rele1=1;
        stato_rele2=1;
        digitalWrite(rele, stato_rele1);
        digitalWrite(rele2, stato_rele2);
        bluetooth.println("rele1acceso");
        bluetooth.println("rele1 acceso");
        delay(200);
        message="";
    }
     if(message=="R")
    {
        Serial.println("stato rele");
        bluetooth.println("stato rele");
        if(stato_rele1==1)
        {
          bluetooth.println("rele1 acceso");
        }
        else
        {
          bluetooth.println("rele1 spento");
        }
        if(stato_rele2==1)
        {
          bluetooth.println("rele2 acceso");
        }
        else
        {
          bluetooth.println("rele2 spento");
        }
        delay(200);
        message="";
    }
    if(message=="P")
    {     
     bluetooth.print(b);bluetooth.println(" W");  
     message=""; //clear the data
    }
    message=""; //clear the data

  }}
  delay(20); //delay
}
