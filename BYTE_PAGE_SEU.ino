/*
   Used the following components:
   (1) Arduino Uno
   (2) 2 SRAM Microchips: 23LC1024
   
   ORIGINAL Code by Jim Eli 
   MOdified by Sahra A. Kulmiya
   
   Explanation::-
   This code was created to measure bit-flip errors that would occur in an SRAM (23LC1024) when placed 
   in an intense neutron beam. The address of the SRAM was read using two different methods to investi-
   gate any discrepancies, and though, hypothetically, none should occur, the code did end up generating
   unequal rates of error. Further investigation is required in this regard as the discrepancies could be
   due to any number of reason, whether it was hardware or software. 
   
   In this particular code, we chose to fill the entire memory of the SRAM with the binary number:
   '10101010' also known as the checkerboard pattern. In decimal format this would be: '170'. The checker-
   board pattern was chosen to allow a better probability of 1->0 and 0->1 bit-flip, and sub-sequently, 
   error rate.
   
   The number of SRAMs used can be increased as needed.
*/

#include <SPI.h>
 
//SRAM opcodes
#define RDSR        5
#define WRSR        1
#define READ        3
#define WRITE       2
 
byte cs_pin[2] = {10, 9}; //cs_pin location of the two SRAMS

uint32_t maxram = 131072; //number of bytes in 23LC1024 SRAM
uint32_t maxpage = 4096;  //number of pages in 23LC1024 SRAM

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/
//Byte transfer functions
//here we read and write one byte at a time at each address

uint8_t Spi23LC1024Read8(uint32_t address, uint8_t cs_pin)
{
  uint8_t read_byte;
  
  digitalWrite(cs_pin, LOW);   
  SPI.transfer(READ);
  SPI.transfer((uint8_t)(address >> 16));
  SPI.transfer((uint8_t)(address >> 8) & 0xFF);
  SPI.transfer((uint8_t)address);
  read_byte = SPI.transfer(0x00);
  digitalWrite(cs_pin, HIGH); 

  return read_byte;
}

void Spi23LC1024Write8(uint32_t address, uint8_t data_byte, uint8_t cs_pin) 
{
  digitalWrite(cs_pin, LOW); 
  SPI.transfer(WRITE);
  SPI.transfer((uint8_t)(address >> 16) & 0xff);
  SPI.transfer((uint8_t)(address >> 8) & 0xff);
  SPI.transfer((uint8_t)address);
  SPI.transfer(data_byte);
  digitalWrite(cs_pin, HIGH); 
 }
 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/
//Page transfer Functions
//here we can read and write 32 bytes at each address

void Spi23LC1024Read32(uint32_t address, uint8_t cs_pin)//, uint8_t* buffer)
{
  uint32_t i; 
  uint8_t read_page; 

  digitalWrite(cs_pin, LOW);  
  SPI.transfer(READ);
  SPI.transfer((uint8_t)(address >> 16));
  SPI.transfer((uint8_t)(address >> 8)); 
  SPI.transfer((uint8_t)address);
  
  for (i = 0; i < 32; i++)
  {
      read_page = SPI.transfer(0x00);
  }
  digitalWrite(cs_pin, HIGH);   
}

void Spi23LC1024Write32(uint32_t address, uint8_t* buffer170, uint8_t cs_pin)
{
    uint32_t i;
    
    digitalWrite(cs_pin, LOW); 
    SPI.transfer(WRITE);
    SPI.transfer((uint8_t)(address >> 16));
    SPI.transfer((uint8_t)(address >> 8));
    SPI.transfer((uint8_t)address);
    
    for (i = 0; i < 32; i++)
    {
        SPI.transfer(buffer170[i]);
    }
    digitalWrite(cs_pin, HIGH); 
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-*/

uint32_t i, j, n; 
uint32_t value; 
uint8_t set_value = 170; 
uint32_t num = 0; 
uint8_t buffer170[32] = {170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170}; 

void setup(void) 
{
  Serial.begin(115200);
  SPI.begin();

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH); 
}
 
void loop() 
{
/*----THIS IS TO READ ALL BYTES IN THE TWO SRAMS ONE AT A TIME----*/
  Serial.println("BYTE MODE");
  for (j = 0; j < 2; j++) 
  {
    num = 0;  
    for (i = 0; i < maxram; i++)
    {
      Spi23LC1024Write8(i, (uint8_t)set_value, cs_pin[j]);
    }
    for (i = 0; i < maxram; i++)
    {
      value = Spi23LC1024Read8(i, cs_pin[j]);
      if (value != set_value)  //will check if any of the 131,072 bytes is not equal to the value we set it: 170
      {
        Serial.print(value, HEX); //returns the value it holds in HEX format
        Serial.print(" at: ");
        Serial.println(i, HEX);  //returns the address at which this occurs in HEX format
        num++;
       }
     }
     Serial.print(cs_pin[j]);  //returns the cs line/SRAM it was reading from
     Serial.print(":");
     Serial.println(num); //returns no. of errors occured in this cs_line/SRAM
     delay(1000); //To give the cs_line time to die down
  }

  /*----THIS IS TO READ ALL PAGES IN THE TWO SRAMS ONE AT A TIME----*/
  Serial.println("PAGE MODE");
  for (j = 0; j < 2; j++)
  {
    num = 0;
    for (i = 0; i < maxpage; i++)
    { 
      Spi23LC1024Write32(i, buffer170, cs_pin[j]);
    }
    for (i = 0; i < maxpage; i++) 
    {
      Spi23LC1024Read32(i, cs_pin[j]);
      for (n = 0; n < 32; n++)  //because each page consists of a further 32 bytes we need to check
      {
        if (buffer170[n] != set_value)
        {
          Serial.print(value, HEX);
          Serial.print(" at: ");
          Serial.println(n, HEX); 
          num++;
        }
      }
    }
    Serial.print(cs_pin[j]);  //returns the cs line/SRAM it was reading from
    Serial.print(":");
    Serial.println(num);  //How many errors occured in this cs_line/SRAM
    delay(1000); //To give the cs_line time to die down
  }
  
  /*-----YOU SHOULD SEE THE FOLLOWING OUTPUT IN SERIAL MONITOR----*/
  /* BYTE MODE
   * 10:0 
   * 9:0
   * PAGE MODE
   * 10:0
   * 9:0
   * etc
   */
}
