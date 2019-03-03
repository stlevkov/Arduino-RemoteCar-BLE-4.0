#include <SoftwareSerial.h>
SoftwareSerial BTSerial(11, 10);

//#include "LowPower.h"

/*
   Arduino UNO - Bluetooth HM-10 Measurements
   --Test with Sleep Mode OFF:

   -4.00V supply with 0.120A Current resource = 0.034A (0.136W)
   -Consumption on Idle (Both Connected and Not Connected)
   -There is no difference when Connecting or Disconnecting - No voltage drop or high current drain

   --Test with Bluetooth Sleep Mode ON:
   -4.00V supply with 0.120A Current resource = 0.023A (0.092W)

   --Test with Arduino Sleep Mode ON:
   -4.00V supply with 0.120A Current resource =  0.017A ~ 0.018A (0.072W)

   Calculating by formula:
   -Battery Life = Battery Capacity in mAh / Load Current in mA * 0.70
    0.70 can be 1, it is use for external factors wich can affect the measurements

   Calculating with 0.017A and Factor exact 1
   With 120mAh battery ~ 7 hours (0.291666667 days)
   With 650mAh battery ~ 38 hours (1.58333333 days)
   With 1200mAh battery ~ 70 hours (2.91666667 days)
   With 6800mAh battery ~ 400 hours (16.6666667 days)
   With 13600mAh battery ~ 800 hours (33.3333333 days)

   TODO - Add Delay to wake up at specific time period
   TODO - Add BME280

*/

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
String package;
char character;
String packet;


// Head and Back lights
const int stop_led_pin = 7;
const int front_led_pin = 12;
int turnStopLights = 0;
int turnFrontLights = 0;

/*
     FL []=-|-=[] FR
            |
            |
            |
     RL []-=|=-[] RR
*/

// This is the library for the Servo motor micro
#include <Servo.h>
Servo servo; // using the Class Servo
const int servo_pin = 9;
long sval = 0;

// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 2
#define AIN2 4
#define PWMA 5
#define STBY 3  // Changed after Rover 1.0 with 433MHz, so we need for 2.4GHz 24l01 the 9, 10 pins for CSN, CE
// these constants are used to allow you to make your motor configuration

// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;

/* Initializing motors.  The library will allow you to initialize as many
    motors as you have memory for.  If you are using functions like forward
    that take 2 motors as arguements you can either write new functions or
    call the function more than once.
*/
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
long mval = 0;
long mval1 = 0;

void setup() {
  Serial.begin(57600);
  BTSerial.begin(57600);

  pinMode(front_led_pin, OUTPUT);
  pinMode(stop_led_pin, OUTPUT);

  // Servo settings
  servo.attach(servo_pin);

  //  Serial.println("Testing Motor");
  //  for (int i = 0; i < 1000; i++) {
  //    motor1.drive(255, 1000);                    // drive(), but with a drive(drive, duration)
  //    Serial.println("MOTOR GO FORD");
  //  }

}

void loop() {
  while (BTSerial.available()) {
    character = BTSerial.read();
    packet = character;
    package += packet;

    // Synchronization
    if (packet.equals("S")) {

      String packageLen = getValue(package, ';', 0);
      String xAxis = getValue(package, ';', 1);
      String yAxis = getValue(package, ';', 2);
      String frontLights = getValue(package, ';', 3);
      String stopLights = getValue(package, ';', 4);

      Serial.print("len: ");
      Serial.print(packageLen);
      Serial.print(", xAxis: ");
      Serial.print(xAxis);
      Serial.print(", yAxis: ");
      Serial.print(yAxis);
      Serial.print(", frontLights: ");
      Serial.print(frontLights);
      Serial.print(", stopLights: ");
      Serial.print(stopLights);

      const int len = packageLen.toInt();

      // CheckLenght
      if (len > 8 || len < 16) {
        Serial.println("");
        Serial.println("Check Len: Yes");

        int servoVal = xAxis.toInt();
        int motorVal = yAxis.toInt();

        if (motorVal >= 540 && motorVal <= 1023) {
          mval = map(motorVal, 540, 1023, 0, -255);
          motor1.drive(mval, 10);
          Serial.println("MOTOR GO FORD");
        } else if (motorVal <= 470 && motorVal >= 0) {
          mval = map(motorVal, 470, 0, 0, 255);
          motor1.drive(mval, 10);
          Serial.println("MOTOR GO BACK");
        }

        if (servoVal >= 540 && servoVal <= 1023) {
          sval = map(servoVal, 540, 1023, 110, 190);
          servo.write(sval);
        } else if (servoVal <= 470 && servoVal >= 0) {
          sval = map(servoVal, 470, 0, 110, 40);
          Serial.println(sval);
          servo.write(sval);
        } else {
          Serial.println(sval);
          servo.write(110);
        }

        if (frontLights.toInt() == 1) {
          if (turnFrontLights == 0) {
            digitalWrite(front_led_pin, HIGH);
            turnFrontLights = 1;
            Serial.println("FRONT LIGHTS ON");
          } else if (turnFrontLights == 1) {
            digitalWrite(front_led_pin, LOW);
            turnFrontLights = 0;
            Serial.println("Front LIGHTS OFF");
          }
        }

        if (stopLights.toInt() == 1) {
          if (turnStopLights == 0) {
            digitalWrite(stop_led_pin, HIGH);
            turnStopLights = 1;
            Serial.println("STOP LIGHTS ON");
          } else if (turnStopLights == 1) {
            digitalWrite(stop_led_pin, LOW);
            turnStopLights = 0;
            Serial.println("STOP LIGHTS OFF");
          }
        }

        package = "";
        Serial.println("");
      }
    } else {
      motor1.standby();
    }
  }
}


// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
