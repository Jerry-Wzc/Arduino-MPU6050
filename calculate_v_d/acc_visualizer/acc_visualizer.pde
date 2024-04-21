import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;

String data="";
float ax, ay, vx, vy;

void setup() {
  size (960, 640);
  myPort = new Serial(this, "/dev/cu.usbmodem11101", 9600); // starts the serial communication
  myPort.bufferUntil('\n');
}

void draw() {
  background(150);
  textSize(30);
  text("ax: " + ax, 450, 80);
  stroke(255);
  strokeWeight(10);
  line(480, 100, 480+500*ax, 100);
  text("ay: "+ay, 450, 160);
  stroke(255);
  strokeWeight(10);
  line(480, 180, 480+500*ay, 180);
  text("vx: "+vx, 450, 240);
  text("vy: "+vy, 450, 320);
  

  
  // 3D 0bject



  //delay(10);
  //println("ypr:\t" + angleX + "\t" + angleY); // Print the values to check whether we are getting proper values
}

// Read data from the Serial Port
void serialEvent (Serial myPort) { 
  // reads the data from the Serial Port up to the character '.' and puts it into the String variable "data".
  data = myPort.readStringUntil('\n');

  // if you got any bytes other than the linefeed:
  if (data != null) {
    data = trim(data);
    // split the string at "/"
    String items[] = split(data, '/');
    if (items.length > 1) {

      //--- Roll,Pitch in degrees
      ax = float(items[0]);
      ay = float(items[1]);
      vx = float(items[2]);
      vy = float(items[3]);
    }
  }
}
