/*
- Interfacing a M74HC238 3-8 Line Decoder With an Arduino (Example)
Zach Cross - Oct 19 2011
- Interfacing an 23LC1024 SRAM with an Arduino
Used the following components and wire routing: 
(1) Arduino Uno
(2) Microchips 23LC1024
(3) M74HC238 3 to 8 line Decoder (refer to microchip spec for further clarification of code)
original code by Jim Eli
modifications by Alex Mellish
Final Code by Sahra A. Kulmiya (19_05_17)

Revision 22/05/17 :- Replaced M74HC238E by SN74HC138 (inverted logic) */

#include <SPI.h>
//SRAM opcodes #define RDSR 5 #define WRSR 1 #define READ 3 #define WRITE 2
const int P0 = 2; //Output 1
const int P1 = 3; //Output 2
const int P2 = 4; //Output 3
const int E3 = 5; //All Output control G1

/* To control the SRAMS cs-pins, i need to set them high and Low accordingly through the
decoder SN74HC138E
Using the truth table on the data sheet, you can see that there are 7 ways to set Y0 and Y1 HIGH: a) digitalWrite(E3, LOW);
b) digitalWrite(E3, HIGH); 
digitalWrite(P0, LOW); 
digitalWrite(P1, HIGH); 
digitalWrite(P2, LOW);
etc.
But there are only 2 ways to set Y0,Y1...Y3 etc. LOW:
SET Y0 LOW:-
a) digitalWrite(E3, HIGH);
digitalWrite(P0, LOW); 
digitalWrite(P1, LOW); 
digitalWrite(P2, LOW);
SET Y1 LOW:-
a) digitalWrite(E3, HIGH);
digitalWrite(P0, HIGH); 
digitalWrite(P1, LOW); 
digitalWrite(P2, LOW);
*/

//Putting the commands to set the outputs HIGH in a function makes it easier and less cluttered. //for the M74HC238, the following commands wold make the pin HIGH instead of LOW
 
 
void Y0LOW() {
digitalWrite(E3, HIGH); digitalWrite(P0, LOW); digitalWrite(P1, LOW); digitalWrite(P2, LOW);
}
void Y1LOW() {
digitalWrite(E3, HIGH); digitalWrite(P0, HIGH); digitalWrite(P1, LOW); digitalWrite(P2, LOW);
}
void Y2LOW() {
digitalWrite(E3, HIGH); digitalWrite(P0, LOW); digitalWrite(P1, HIGH); digitalWrite(P2, LOW);
}
void Y3LOW() {
digitalWrite(E3, HIGH); digitalWrite(P0, HIGH); digitalWrite(P1, HIGH); digitalWrite(P2, LOW);
}

///////////////////////////////////////////////////////////////////////////////// 
/*The following code could be put into a loop to avoid clutter*/
uint8_t SpiY0Read8(uint32_t address) {
uint8_t read_byte;
Y0LOW(); //sets all pins low 
SPI.transfer(READ); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address);
read_byte = SPI.transfer(0x00); 
digitalWrite(E3, LOW); 
Serial.print("READ BYTE: "); 
Serial.print("Address: "); 
Serial.print(address); 
Serial.print(", value: ");
return read_byte; 
}

void SpiY0Write8(uint32_t address, uint8_t data_byte) {
Y0LOW(); //sets all pins low 
SPI.transfer(WRITE); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address); 
SPI.transfer(data_byte);
digitalWrite(E3, LOW); 
Serial.print("WRITE BYTE: "); 
Serial.print("Address: "); 
Serial.print(address);

/*The code on the left summarises the functions that allow the 8 output pins (of which 4 are be- ing used) to be set low, so that the SRAMs know they are being communicated with.
This part of the code cannot be shortened down and will have to be written explicitly when in- creasing the number of outputs being used.
34*/

Serial.print(", data byte = ");
Serial.println(data_byte);*/ 
}
uint8_t SpiY1Read8(uint32_t address) {
uint8_t read_byte;
Y1LOW(); //digitalWrite(E3, LOW); 
SPI.transfer(READ); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address);
read_byte = SPI.transfer(0x00); 
digitalWrite(E3, LOW); 
Serial.print("READ BYTE: "); 
Serial.print("Address: "); 
Serial.print(address); 
Serial.print(", value: ");
return read_byte; 
}

void SpiY1Write8(uint32_t address, uint8_t data_byte) {
Y1LOW();
SPI.transfer(WRITE); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address); 
SPI.transfer(data_byte);
digitalWrite(E3, LOW);
Serial.print("WRITE BYTE: "); 
Serial.print("Address: ");
Serial.print(address);
Serial.print(", data byte = "); 
Serial.println(data_byte);*/
}
uint8_t SpiY2Read8(uint32_t address) {
uint8_t read_byte;
Y2LOW();
SPI.transfer(READ); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address);
read_byte = SPI.transfer(0x00); 
digitalWrite(E3, LOW); 
Serial.print("READ BYTE: "); 
Serial.print("Address: "); 
Serial.print(address); 
Serial.print(", value: ");
return read_byte; 
}

void SpiY2Write8(uint32_t address, uint8_t data_byte) {
Y2LOW();
SPI.transfer(WRITE); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address); 
SPI.transfer(data_byte);
digitalWrite(E3, LOW);

/*The code on the left summarises the functions that carry out the reading and writing of the SRAMs and are written indi- vidually for each output. The bulk of the code is the same however could be potentially condensed into one loop to allow a much less cluttered code.

Serial.print("WRITE BYTE: "); 
Serial.print("Address: "); 
Serial.print(address); 
Serial.print(", data byte = ");
Serial.println(data_byte);*/

}

Void uint8_t SpiY3Read8(uint32_t address) {
uint8_t read_byte;
Y3LOW();
SPI.transfer(READ); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address);
read_byte = SPI.transfer(0x00); 
digitalWrite(E3, LOW); 
Serial.print("READ BYTE: "); 
Serial.print("Address: "); 
Serial.print(address); 
Serial.print(", value: ");
return read_byte; 
}

void SpiY3Write8(uint32_t address, uint8_t data_byte) {
Y3LOW();
SPI.transfer(WRITE); 
SPI.transfer((uint8_t)(address >> 16) & 0xff); 
SPI.transfer((uint8_t)(address >> 8) & 0xff); 
SPI.transfer((uint8_t)address); 
SPI.transfer(data_byte);
digitalWrite(E3, LOW);
Serial.print(“WRITE BYTE: “); 
Serial.print("Address: ");
Serial.print(address);
Serial.print(", data byte = "); 
Serial.println(data_byte);*/

}

void setup() {
uint32_t i;
uint8_t value;
uint8_t set_value = 170;
// initialize the serial communication:
Serial.begin(9600); //Anything from 9600 to 230400 will work, 250k wont

// initialize the control outputs 
pinMode(P0, OUTPUT); 
pinMode(P1, OUTPUT); 
pinMode(P2, OUTPUT);
SPI.begin();
Serial.println("--CHIP 1-----"); 
for (i=0; i<32; i++) {
SpiY0Write8(i, (uint8_t)set_value); 
value = SpiY0Read8(i); 
Serial.println((uint16_t)value);
}

Serial.println("--CHIP 2------"); 
for (i=0; i<32; i++) {
SpiY1Write8(i, (uint8_t)set_value); 
value = SpiY1Read8(i); 
Serial.println((uint16_t)value);
}
Serial.println("--CHIP 3-----"); 
for (i=0; i<32; i++) {
SpiY2Write8(i, (uint8_t)set_value); 
value = SpiY2Read8(i); 
Serial.println((uint16_t)value);
}
Serial.println("--CHIP 4-----"); 
for (i=0; i<32; i++) {
SpiY3Write8(i, (uint8_t)set_value); 
value = SpiY3Read8(i); 
Serial.println((uint16_t)value);
}
}
void loop() {
/*Alternatively, the code after "void setup" can be placed here for continous looping*/
}
