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

Detector Garage "Door opened"
Digital Pin 9
GND

Vin: 12 v Power source and RFID Reader (Vcc)
GND: GND Power source and RFID Reder (GND) 



***************************************************************************************/
#include <Wiegand.h>
#include <SPI.h>
#define DoorControlPin 5 //pin to control de door
#define DoorStatusPin 9 //pin of door status (closed/not closed)
#define LedPin 4 //pin of door status (closed/not closed)
#define BuzzerPin 6 //pin of door status (closed/not closed)
#define CLOSE_DELAY 300000

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
        

}

void loop() {

if(wg.available())
{
  code = wg.getCode(); //read RFID reader and store the tag number in code variable 
}

switch (code) { //compare code with the allowed tags #
    case 9999999: // Put here the ID of the tags that do you want to give access
    case 8888888:
    case 7777777:
    case 6666666:
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
  code = 0;
  digitalWrite(DoorControlPin, HIGH); //enable door's relay 
  delay(400); //delay 400ms
  digitalWrite(DoorControlPin, LOW); //disable door's relay
}

void denied() //access denied 
{
  digitalWrite(BuzzerPin, LOW); //inicialize pin of buzzer to LOW
  delay(100);
  digitalWrite(BuzzerPin, HIGH); //inicialize pin of buzzer to L
  delay(100);
  digitalWrite(BuzzerPin, LOW); //inicialize pin of buzzer to LOW
  delay(100);
  digitalWrite(BuzzerPin, HIGH); //inicialize pin of buzzer to L
  delay(100);
  code = 0; 
}



