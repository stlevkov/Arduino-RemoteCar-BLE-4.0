# Arduino RemoteCar BLE 4.0
Arduino controlled remote car using Bluetooth 4.0 BLE Transiever module HM-10

Hardware used in this project:
<h1>Rover side</h1>
<ol>
  <li>Arduino Leonardo</li>
  <li>BLUETOOTH BLE Transiever HM-10</li>
  <li>Mini servo motor (blue)</li>
  <li>4x normal RED LED</li>
  <li>2x 1W warm WHITE LED</li>
  <li>NPN BC337 transistor</li>
  <li>microUSB charger board TP4056 (without protection)</li>
  <li>Small motor 12V</li>
  <li>7x AA Batteries</li>
  <li>18650 3000mAh RED</li>		
</ol>

<h1>Joystick side</h1>
<ol>
  <li>Arduino UNO R3</li>
  <li>Joystick Shield R2012</li>
  <li>BLUETOOTH BLE Transiever HM-10</li>
</ol>

# Schematic diagrams
<h2>Rover Diagram</h2>

![Rover Diagram](https://github.com/stlevkov/Arduino-RemoteCar-BLE-4.0/blob/master/Resources/Rover/ROVER-BLE-SCHEMATIC_bb.png) 

<h2>Joystick Diagram</h2>

![Rover Diagram](https://github.com/stlevkov/Arduino-RemoteCar-BLE-4.0/blob/master/Resources/Joystick/JOYSTICK-BLE-SCHEMATIC_bb.png)

# Communication protocol:
<h3>packageLen;xAxis;yAxis;frontLights;stopLights;stopChar</h3>

<ol>
	<li>packageLen  = size Of (data packets)</li>
	<li>xAxis       = 0 - 255               </li>
	<li>yAxis       = 0 - 255               </li>
	<li>frontLights = 0 - 1                 </li>
	<li>stopLights  = 0 - 1                 </li>
	<li>stopChar    = S                     </li>
</ol>
