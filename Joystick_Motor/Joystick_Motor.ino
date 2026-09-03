int xyzPins[] = {28, 27, 26}; // x, y, and z pins in that order
int stepperPorts[] = {21, 20, 19, 18};

void setup() {
  Serial.begin(115200);
  pinMode(xyzPins[2], INPUT_PULLUP); // z axis is a button

  // Set stepper pins to output
  for (int i = 0; i  < 4; i++) {
    pinMode(stepperPorts[i], OUTPUT);
  }
}

void loop() {
  int xVal = analogRead(xyzPins[0]);
  int yVal = analogRead(xyzPins[1]);
  int zVal = digitalRead(xyzPins[2]);

  if (zVal == 0) { // Button is pressed
    // Rotate a full turn
    moveSteps(true, 32*64, 3);
    delay(1000);

    // Rotate a full turn towards the other direction
    moveSteps(false, 32*64, 3);
    delay(1000);
  } else { // Button is not pressed
    moveAround(xVal < (1023/2)+100, 1, map(xVal, 0, 1023, 3, 20));
  }
}

void moveSteps(bool dir, int steps, byte ms) {
  for (unsigned long i = 0; i < steps; i++) {
    moveOneStep(dir); // Rotate a step
    delay(constrain(ms, 3, 20)); // Control the speed
  }
}

void moveOneStep(bool dir) {
  // Define a variable, use four low bit to indicate the state of port
  static byte out = 0x01;
  // Decide the shift direction according to the rotation direction
  if (dir) {  // ring shift left
    out != 0x08 ? out = out << 1 : out = 0x01;
  }
  else {      // ring shift right
    out != 0x01 ? out = out >> 1 : out = 0x08;
  }
  // Output singal to each port
  for (int i = 0; i < 4; i++) {
    digitalWrite(stepperPorts[i], (out & (0x01 << i)) ? HIGH : LOW);
  }
}

void moveAround(bool dir, int turns, byte ms){
  for(int i = 0; i < turns; i++)
    moveSteps(dir, 32*64 ,ms);
}

void moveAngle(bool dir, int angle, byte ms){
  moveSteps(dir, (angle*32*64/360) ,ms);
}