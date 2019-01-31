#ifndef NFCHANDLER_H
#define NFCHANDLER_H
//Handler for communicating with CR95HF

extern byte RXBuffer[24];

const int SSPin = 10;  // Slave Select pin
const int IRQPin = 9;  // Sends wake-up pulse for BM019
const int NFCPin1 = 7; // Power pin BM019
const int NFCPin2 = 8; // Power pin BM019
const int NFCPin3 = 4; // Power pin BM019
const int MOSIPin = 11;
const int SCKPin = 13;

void pollUntilRespond();
/**
 * Returns 1 when maxwait_ms is reached, 0 otherwise.
 */
byte pollUntilRespond(int maxwait_ms);
void sendCommand(byte cmd, byte data); //length 1
void sendCommand(byte cmd, byte data1, byte data2); //length 2
void sendCommand(byte cmd, byte data1, byte data2, byte data3); //length 3
void sendCommand(byte cmd, byte datas[], byte length);

/**
 * Sends and waits for its response which is automatically stored in RXBuffer, if successful.
 * Returns 0 if no error occured.
 * 
 * maxwait: in miliseconds
 */
byte sendCommandAndWait(byte cmd, byte data1, byte data2,int maxwait); //length 2
byte sendCommandAndWait(byte cmd, byte data1, byte data2, byte data3,int maxwait); //length 3

void resetCR95HF();

#endif
