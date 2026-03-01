 // !!! IMPORTANT !!!
/* To make a connection we:
    Set the second module to Peripheral mode (the default setting),
    In order to be able for connect, the transmitter needs the following commands first:
    Set the main module to manual start mode using “AT+IMME1″.
    Then set the main module to Central mode with “AT+ROLE1″.
    Then use “AT+CON” to connect. Of course you need to know the address of the second module which can be found using “AT+DISC?”.
*/


//  SerialIn_SerialOut_HM-10_01
//
//  Uses hardware serial to talk to the host computer and AltSoftSerial for communication with the bluetooth module
//
//  What ever is entered in the serial monitor is sent to the connected device
//  Anything received from the connected device is copied to the serial monitor
//  Does not send line endings to the HM-10
//  send \n\r when working with AT-09
//
//  Pins
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//

/*
   AT COMMANDS: AT+HELP
   TO SET BAUD TO 57600 use AT+BAUD7
   
*/

// Address of the transmitter: 341513D9B22A
// Address of the receiver   : D43639D8A47A
#include <AltSoftSerial.h>
AltSoftSerial BTserial;
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html


char c = ' ';
boolean NL = true;

// Define Joystick settings
const int xJoystick = A0;
const int yJoystick = A1;
//Asign pin numbers for the Joystick
const int swPinLeft = 7; // digital pin connected to switch output (mini LEFT button between the joystick and the color buttons)
const int swPinRight = 6; // digital pin connected to switch 2 (mini RIGHT button between the joystick and the color buttons)

// Define object holding the data


void setup() {
  Serial.begin(57600);
  BTserial.begin(57600);

  // Manual start
  BTserial.print("AT+IMME1\r\n");
  delay(200);

  // Set as master
  BTserial.print("AT+ROLE1\r\n");
  delay(200);

  // Connect to HM-10
  BTserial.print("AT+CONAD43639D8A47A\r\n");
  delay(1000);

  Serial.println("Connection attempt done. If successful, modules are paired.");
}

void loop()
{
  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (BTserial.available())
  {
    c = BTserial.read();
    Serial.write(c);
  }


  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available())
  {
    c = Serial.read();

    // do not send line end characters to the HM-10
    if (c != 10 & c != 13 )
    {
      BTserial.write(c);
    }

    // Echo the user input to the main window.
    // If there is a new line print the ">" character.
    if (NL) {
      Serial.print("\r\n>");
      NL = false;
    }
    Serial.write(c);
    if (c == 10) {
      NL = true;
    }
  }

  for (int i = 0; i < 100000; i++) {
    short xAxis = analogRead(xJoystick);      // Read Joysticks X-axis
    short yAxis = analogRead(yJoystick);      // Read Joysticks Y-axis
    byte frontLights = 0;   // Read Joystick smallLefButton
    byte stopLights = 0;    // Read Joystick smallRightButton

    boolean xToBeSend = false;
    boolean yToBeSend = false;

    if (xAxis > 540 || xAxis < 470) {
      xToBeSend = true;
    }

    if (yAxis > 540 || yAxis < 470) {
      yToBeSend = true;
    }

    if (digitalRead(swPinLeft) == LOW) {
      frontLights = 1;
    }
    if (digitalRead(swPinRight) == LOW) {
      stopLights = 1;
    }


    String data = String(xAxis);
    data += ";";
    data += String(yAxis);
    data += ";";
    data += String(frontLights);
    data += ";";
    data += String(stopLights);
    data += ";";
    // add stop_bit
    data += "S";
    int len = strlen(data.c_str());

    String packet = String(len);
    packet += ";";
    packet += data;


    const char* dataChars = packet.c_str();
   // Serial.println(dataChars);
    if (frontLights > 0 || stopLights > 0) {
      BTserial.write(dataChars);
      BTserial.write("\r\n");
      delay(1000);
    }

    if (xToBeSend || yToBeSend) {
      BTserial.write(dataChars);
      BTserial.write("\r\n");
      delay(20);
    }

  }

}
