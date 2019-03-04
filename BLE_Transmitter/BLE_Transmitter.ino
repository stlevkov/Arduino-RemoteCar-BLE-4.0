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
//
//  Pins
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//

/*
   AT COMMANDS:

  AT+ADDR?
  Usage: Query the native MAC address 
  Should return: OK + LADD: MAC address (address for 12 string)
  AT+BAUD?
  Usage: Query the baud rate 
  Should return: OK + Get: [para1] 
  Details: Scope of para1:0 ~ 8. The parameters corresponding to: 0 represents 9600, 1, 2, 9600, 38400, on behalf of the representative representative of 57600, 115200, 5, 4800, 6, 7 represents 1200, 1200 2400. The default baud rate is 9600.
  AT+BAUD[para1]
  Usage: Set the baud rate 
  Should return: OK+Set:[para1]
  Details: Scope of para1:0 ~ 8. The parameters corresponding to: 0 represents 9600, 1, 2, 9600, 38400, on behalf of the representative representative of 57600, 115200, 5, 4800, 6, 7 represents 1200, 1200 2400. The default baud rate is 9600.
  Note: If you switch to the 1200, module will no longer support the configurations of the AT command, and press the PIO0 under standby, module can restore the factory Settings.Do not recommend using the baud rate.After setting the baud rate, modules should be on electricity, anew set parameters can take effect.
  AT+CON[para1]
  Usage: Connect to the bluetooth address specified (para1)
  Should return: OK+CONN[para2] 
  Details: Para2 is the range of the connection and can be A, E or FE.
  Example: To connect to bluetooth address 0017EA0943AE: sending the AT + CON0017EA0943AE, module returns: OK + CONNA or OK + + CONNF CONNE or OK.
  AT+CLEAR 
  Usage: Removal of equipment matching information 
  Should return: OK + CLEAR 
  Details: Clear success had connected device address code information.
  AT+MODE?
  Usage:Query module working mode 
  Should return: OK + Get: [para] 
  Details: The range of [para] is 0 ~ 2. 0 represents passthrough mode, on behalf of the PIO acquisition + remote control + 1 passthrough, 2 representative passthrough + remote control mode.The default is 0.
  AT + MODE [para]
  Usage:Set module working mode 
  Should return: OK + Set: [para]
  Details: The range of [para] is 0 ~ 2. 0 represents passthrough mode, on behalf of the PIO acquisition + remote control + 1 passthrough, 2 representative passthrough + remote control mode.The default is 0.
  AT + NAME?
  Usage:Query device name 
  Should return: OK + NAME [para1]
  AT + NAME [para1] 
  Usage:Set the device name 
  Should return: OK + Set: [para1] 
  Note: After the execution of the instruction, required to restart in order to set the parameters of the approval.
  Example: Set the device name to Seeed, sending the AT + NAMESeeed, return OK + Set: Seeed AT this time, the name of the bluetooth module has been changed to Seeed.
  AT + PASS?
  Usage:Query pairing password.
  Should return: K + PASS: [para1] 
  Details: Para1 range is 000000 ~ 999999, the default is 000000.
  AT + PASS [para1] 
  Usage:Set pairing password.
  Should return: OK + Set: [para1]
  AT + RENEW
  Usage:Restore factory default settings 
  Should return: OK + RENEW 
  Details: Restore the default factory settings for the module. Ensure a delay of 500 ms after the restart. Note: For some reason the datasheet says: "If no need, please be careful".
  AT + RESET 
  Usage:Reset the module.
  Should return: OK + RESET 
  Note: After the instruction execution module will have a delay of 500 ms.
  AT + ROLE [para1] 
  Usage:Set the master-slave mode 
  Should return: OK + Set: [para1]
  Details: Para1 can be
  AT+SLEEP
  Usage:Set the device in sleep mode.
  Should return: OK+SLEEP
  Details: To wake up, send a large (>80 characters) string.
  There are more AT commands, look for them in the datasheet!
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


void setup()
{
  Serial.begin(57600);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");

  BTserial.begin(57600);
  Serial.println("BTserial started at 57600");

  //Auto connect to the receiver
  BTserial.print("AT+IMME1" );
  delay(1000);
  BTserial.print("AT+ROLE1" );
  delay(1000);
  BTserial.print("AT+COND43639D8A47A" );
  delay(1000);

  // The receiver MUST have IMME1, ROLE0, MODE0 in order to be able for paring
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
    Serial.println(dataChars);
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
