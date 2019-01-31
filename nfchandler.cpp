#include <Arduino.h>
#include <SPI.h>
#include "nfchandler.h"

#define SPI_WAIT_DELAY 1

byte RXBuffer[24];

void pollUntilRespond(){
  digitalWrite(SSPin, LOW);
  while (RXBuffer[0] != 8)
  {
    RXBuffer[0] = SPI.transfer(0x03);  // Write 3 until
    RXBuffer[0] = RXBuffer[0] & 0x08;  // bit 3 is set
  }
  digitalWrite(SSPin, HIGH);
}

byte pollUntilRespond(int maxwait_ms){
  digitalWrite(SSPin, LOW);
  while (RXBuffer[0] != 8)
  {
    RXBuffer[0] = SPI.transfer(0x03);  // Write 3 until
    RXBuffer[0] = RXBuffer[0] & 0x08;  // bit 3 is set
    if(maxwait_ms==0){
      Serial.println("WARNING: pollUntilRespond() returning 1");
      digitalWrite(SSPin, HIGH);
      return 1;
    }
    maxwait_ms--;
    delay(1);
  }
  digitalWrite(SSPin, HIGH);
  return 0;
}

void sendCommand(byte cmd, byte data){
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(0x02);
  SPI.transfer(0x01);  // length of data to follow
  SPI.transfer(data);
  digitalWrite(SSPin, HIGH);
}

void sendCommand(byte cmd, byte data1, byte data2){
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(cmd);
  SPI.transfer(0x02);  // length of data to follow
  SPI.transfer(data1);
  SPI.transfer(data2);
  digitalWrite(SSPin, HIGH);
}

void sendCommand(byte cmd, byte data1, byte data2, byte data3){
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(cmd);
  SPI.transfer(0x03);  // length of data to follow
  SPI.transfer(data1);
  SPI.transfer(data2);
  SPI.transfer(data3);
  digitalWrite(SSPin, HIGH);
}

void sendCommand(byte cmd, byte datas[], byte length){
  byte i;
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(cmd);
  SPI.transfer(length); // length
  for(i=0;i<length;i++){
    SPI.transfer(datas[i]);
  }
  digitalWrite(SSPin, HIGH);
}

void catchResponse(){
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x02);   // SPI control byte for read
  RXBuffer[0] = SPI.transfer(0);  // response code
  RXBuffer[1] = SPI.transfer(0);  // length of data
  for (byte i = 0; i < RXBuffer[1]; i++)
    RXBuffer[i + 2] = SPI.transfer(0); // data
  digitalWrite(SSPin, HIGH);
}

byte sendCommandAndWait(byte cmd, byte data1, byte data2,int maxwait){
  sendCommand(cmd,data1,data2);
  delay(SPI_WAIT_DELAY);
  if(pollUntilRespond(maxwait)==1){
    return 1;
  }
  delay(SPI_WAIT_DELAY);
  catchResponse();
  return 0;
}

byte sendCommandAndWait(byte cmd, byte data1, byte data2, byte data3,int maxwait){
  sendCommand(cmd,data1,data2,data3);
  delay(SPI_WAIT_DELAY);
  if(pollUntilRespond(maxwait)==1){
    return 1;
  }
  delay(SPI_WAIT_DELAY);
  catchResponse();
  return 0; 
}

void resetCR95HF(){
  Serial.println("Reseting");
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x01);
  digitalWrite(SSPin, HIGH);
  delay(10);
  digitalWrite(IRQPin, LOW); 
  delay(10);
  digitalWrite(IRQPin, HIGH); 
}
