/************** Bernardo Alarcos - Universidad de Alcala ******************************
Access control with RFID reader wG26 compatible. Read tag and if the id numbre is one of the  
programmed in the application enable a relay that unlook de door. 

Componentes:
Arduino Duemilanove
RFID reader WG26 compatible: Vcc (12v), GND, Data 0, Data 1.
Relay (optoisolated):  Vcc(5V), GND, S (control).


Interfaz arduino:
Digital pin 2: RFID reader Data 0 (Green WG26)
Digital pin 3: RFID reader Data 1 (Yellow WG26)
Digital pin 5: Relay (S)
Digital pin 4: led (white)
Digital pin 6: buzzer (blue)

5v: + Relay (+)
GND: - Relay (-)
Control relay Pin 5

RF24L01+
Digital Pin 7: CS
Digital Pin 8: CE

Digital Pin 11: MOSI
Digital Pin 12: MISO
Digital Pin 13: SCK
VCC: 5v
GND: GND

Detector Garage "Door opened"
Digital Pin 9
GND

Vin: 12 v Power source and RFID Reader (Vcc)
GND: GND Power source and RFID Reder (GND) 



***************************************************************************************/
#include <Wiegand.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define DoorControlPin 5 //pin to control de door
#define DoorStatusPin 9 //pin of door status (closed/not closed)
#define LedPin 4 //pin of door status (closed/not closed)
#define BuzzerPin 6 //pin of door status (closed/not closed)
#define CLOSE_DELAY 300000

const unsigned long BERNARDO = 1224933;
const unsigned long BERNARDO_WORK = 2785847;
const unsigned long JOSE = 1249907;
const unsigned long BERNI = 2757453;
const unsigned long TANI = 2776709;
const unsigned long INVITADO = 27994742;
int doorStatus = 0;
bool countingTime = true;
unsigned long startTime;

WIEGAND wg;
unsigned long code;
byte c;
void setup() {
Serial.begin(9600); 
  wg.begin();  //inicialize reader communication
  pinMode(DoorControlPin, OUTPUT); //config pin of Door OUTPUT mode
  pinMode(LedPin, OUTPUT); //config pin of Door OUTPUT mode
  digitalWrite(DoorControlPin, LOW); //inicialize pin of control to LOW      
  digitalWrite(LedPin, HIGH); //inicialize pin of buzzer to LOW      
  pinMode(BuzzerPin, OUTPUT); //config pin of Door OUTPUT mode
  digitalWrite(BuzzerPin, HIGH); //inicialize pin of buzzer to LOW      
  pinMode(DoorStatusPin, INPUT_PULLUP); //config pin of STATUS Door INPUT AND PULLUP
        
/* To change CE / CSN Pins:*/
  
 // Mirf.cePin = 8;
 // Mirf.csnPin = 7;
 
 // Mirf.spi = &MirfHardwareSpi;
 // Mirf.init();
  
  
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
//Mirf.payload = 1; // 1 BYTE

  /*
   * To change channel:
   */ 
//Mirf.setTADDR((byte *)"GARAGE"); //RECEIVING  ADDRESS
//Mirf.channel = 90;   
//Mirf.config();
//Mirf.configRegister(RF_SETUP,0x06);  //1MHZ

}

void loop() {

if(wg.available())
{
  code = wg.getCode(); //read RFID reader and store the tag number in code variable 
}

switch (code) { //compare code with the allowed tags #
    case BERNARDO: //Bernardo tag (Work keyring)
    case BERNARDO_WORK: //Bernardo tag (Huose keyring)
    case JOSE: //Jose tag 
    case BERNI: //Berni
    case TANI: //Tani
    case INVITADO:
      allowed();
      break;
    case 0:
      break;
    default: //No data
     denied ();
     break;
      
}
  
doorStatus = digitalRead(DoorStatusPin);
if (doorStatus == HIGH) //Door not closed 
{
//  Serial.println("Puerta cerrada\n");
  countingTime = false; 
}
if (doorStatus == LOW && countingTime == false) //Door not closed 
{
//  Serial.println("Puerta abierta: inicio contador\n");
  countingTime = true; 
  startTime = millis(); //Start counter
  delay(100);
}

if (doorStatus == LOW && countingTime == true )
  {
    unsigned long time = millis()-startTime;
   //Serial.println(time);
    
    if (time > CLOSE_DELAY)
    {
    c = 32; //Alarma puerta abierta 5 min
   // Serial.println("ALARMA Puerta abierta\n");
    //Mirf.setTADDR((byte *)"DISPLAY"); //TO DISPLAY ADDRESS
    //Mirf.send(&c); 
    code = 0;
    digitalWrite(DoorControlPin, HIGH); //enable door's relay 
    delay(400); //delay 400ms
    digitalWrite(DoorControlPin, LOW); //disable door's relay
    countingTime = false;  
    }  
  }
 
}

void allowed() //access allowed
{
//  Serial.println("Acceso permitido\n");
  code = 0;
  digitalWrite(DoorControlPin, HIGH); //enable door's relay 
  delay(400); //delay 400ms
  digitalWrite(DoorControlPin, LOW); //disable door's relay
  c = 31; //acceso
  //Mirf.setTADDR((byte *)"DISPLAY"); //TO DISPLAY ADDRESS
  //Mirf.send(&c);  
}

void denied() //access denied 
{
 // Serial.println("Acceso denegado\n");
  digitalWrite(BuzzerPin, LOW); //inicialize pin of buzzer to LOW
  delay(100);
  digitalWrite(BuzzerPin, HIGH); //inicialize pin of buzzer to L
  delay(100);
  digitalWrite(BuzzerPin, LOW); //inicialize pin of buzzer to LOW
  delay(100);
  digitalWrite(BuzzerPin, HIGH); //inicialize pin of buzzer to L
  delay(100);

    c = 30; //intento de acceso denegado
  //Mirf.setTADDR((byte *)"DISPLAY"); //TO DISPLAY ADDRESS
  //Mirf.send(&c); 
  code = 0; 
  
}



