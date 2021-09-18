# Small World Clock
 It's a Small World themed clock
 
 Being a big Disney fan I wanted to recreate a little bit of the park for a good friend whom also loves Disney. This ones not overly complex but it requires some soldering,
 jig saw to cut acrylic, a cricut for the front face and some 3d printing. If you're up for that then follow on...
 
 ![Front](/images/front.jpg)
 ![Back view](/images/back.jpg)
 ![Back view 2](/images/back2.jpg)

 Parts List
 ===========
 * Arduino Nano
 * Dfplayer
 * Speaker
 * Switch
 * Black Acrylic
 * Heavy fishing braid (80lbs +)
 * SG90 Servo
 * Adafruit PCF8532 RTC
 * Bearing
 * 2mm screws (assorted length)
 * M4 bolt 20mm
 * protoboard to solder onto
 * header pins
 


Build Process
================
## Acrylic
Cut 12" circle in the black acrylic
Drill hole in centre big enough for clock mechanism to fit in


## MP3 Player
In my version I had 6 clips aprox 15 seconds long, save each as an mp3 in /mp3 folder on your SD card.
Filename convention must be 000x.mp3 (e.g. 0001.mp3, 0002.mp3)

## 3d Printing
Print stuff 

## Circuit board
Sorry didn't make a proper pic of the circuit but here was my rough drawing while I was soldering.
Key pings are:
 ```
 D4 - Switch (sound on/off)
 D9 - Servo control
 D10 - TX (this connects to 1k ohm resistor then to RX pin on Dfplayer)
 D11 - RX (this connects to TX pin on Dfplayer)
 ```
 
  ![Back view](/images/circuit.jpg)
  
 ## Arduino
 upload .ino file and edit to any pins you changed or frequency you want it to play.  
 
 ## Assemble
Yeah put the stuff together now. Tried not to stuff up the vinyl cut when putting on and try to git rid of as many air bubbles as possible.
I found a drop of superglue on the ends of the braid helped keep them securely in place once you have their lengths correct.