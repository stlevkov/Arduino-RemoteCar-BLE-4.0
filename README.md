# Arduino-RemoteCar-BLE-4.0
Arduino controlled remote car using BLUETOOTH Transiever HM-10

<h1> Hardware modules used in this project:</h1>

<ol>
  <li>Arduino Leonardo</li>
  <li>2x BLUETOOTH Transiever HM-10</li>
  <li>X,Y, Joystick Shield for Arduino</li>
  <li>Arduino UNO</li>
</ol>

<h2>Rover Diagram</h2>

![Rover Diagram](https://github.com/stlevkov/Arduino-RemoteCar-2.4GHz/blob/master/Resources/Rover/ROVER-RF24l01-SCHEMATIC_bb.png) 


<h2>Joystick Diagram</h2>

![Rover Diagram](https://github.com/stlevkov/Arduino-RemoteCar-2.4GHz/blob/master/Resources/Joystick/JOYSTICK-RF24l01-SCHEMATIC_bb.png)

# Communication protocol:
      "packageLen;xAxis;yAxis;frontLights;stopLights;"

	@packageLen 	= size Of (data packets)
	@xAxis      	= 0 - 255
	@yAxis			= 0 - 255
	@frontLights	= 0 - 1
	@stopLights		= 0 - 1