// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
 
// Include dependant SPI Library 
#include <SPI.h>
 
// Define addresses for radio channels
#define CLIENT_ADDRESS 1   
#define SERVER_ADDRESS 2
 
// Create an instance of the radio driver
RH_NRF24 RadioDriver;
 
// Sets the radio driver to NRF24 and the client address to 1
RHReliableDatagram RadioManager(RadioDriver, CLIENT_ADDRESS);
 
// Declare unsigned 8-bit joystick array
uint8_t joystick[3]; 
 
// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
 
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
 
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");
}
 
void loop()
{
  // Print to Serial Monitor
  Serial.println("Reading joystick values ");
  
  // Fake Joystick for test. Must be 0-255.
  joystick[0] = 19;
  joystick[1] = 12;
  joystick[2] = 13;
 
  //Display the joystick values in the serial monitor.
  Serial.println("-----------");
  Serial.print("x:");
  Serial.println(joystick[0]);
  Serial.print("y:");
  Serial.println(joystick[1]);
 
  Serial.println("Sending Joystick data to nrf24_reliable_datagram_server");
  
  //Send a message containing Joystick data to manager_server
  if (RadioManager.sendtoWait(joystick, sizeof(joystick), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("No reply, is nrf24_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");
 
  delay(100);  // Wait a bit before next transmission
}