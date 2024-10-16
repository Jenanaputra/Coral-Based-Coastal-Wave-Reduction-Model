#include <Servo.h>

Servo myServo;  // Create a servo object

int pos = 45;         // Variable to store the servo position
int potPin = A0;     // Pin where the potentiometer is connected
int potValue = 0;    // Variable to store the potentiometer value
int stepSize = 1;    // Variable to control the speed based on potentiometer value

int goingUp, goingDown;
int upperLS, lowerLS;

bool onGoingUp, onGoingDown;
bool onUpperLS, onLowerLS;

void setup() {
    
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  myServo.attach(9);  // Attach the servo to pin 9
  myServo.write(pos);
  
  Serial.begin(9600);
  
  pinMode(2, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);  
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  onGoingUp = false;
  onGoingDown = false;
  onUpperLS = false;
  onLowerLS = false;
}

void loop() {
  read_potentioData();
  run_servo();
  get_switch_updown();
  onMotorDC();
}

void read_potentioData(){
  potValue = analogRead(potPin);            // Read the potentiometer value (0 to 1023)
  stepSize = map(potValue, 0, 1023, 15, 5); // Map potentiometer value to step size (1 to 10)
}


void run_servo(){
  // Move the servo from 45 to 135 degrees
  for (pos = 45; pos <= 135; pos += 1) {
    myServo.write(pos);  // Set the servo position
    delay(stepSize);           // A small delay for smooth movement
  }

  // Move the servo from 135 to 45 degrees
  for (pos = 135; pos >= 45; pos -= 1) {
    myServo.write(pos);  // Set the servo position
    delay(stepSize);           // A small delay for smooth movement
  }
}


void run_upward_motorDC(){
  digitalWrite(3,HIGH);
  digitalWrite(4,LOW);
}

void run_downward_motorDC(){
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
}

void stop_motorDC(){
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
}

void onMotorDC(){
  if (onGoingUp == true || onGoingDown == true){
    if(onGoingUp == true){
      get_limit_switch();
      if (onUpperLS == true){
        stop_motorDC();
        delay(10);
        Serial.println(" stop");
      }else{
        run_upward_motorDC();
        delay(100);
        Serial.println("putar kiri");
      }
    } else if (onGoingDown == true){
      get_limit_switch();
      if (onLowerLS == true){
        stop_motorDC();
        delay(10);
        Serial.println(" stop");
      } else {
        run_downward_motorDC();
        delay(100);
        Serial.println("putar kanan");
      }
    }
  }else{
    stop_motorDC();
    delay(10);
    Serial.println(" Tidak masuk");
  }
}

void get_switch_updown(){
  goingUp = digitalRead(2);
  goingDown = digitalRead(5);
  
  if (goingUp == LOW){
  	onGoingUp = true;
    onGoingDown = false;
  } else if (goingDown == LOW){
  	onGoingUp = false;
    onGoingDown = true;
  }else{
  	onGoingUp = onGoingUp;
    onGoingDown = onGoingDown;
  }
  
}


void get_limit_switch(){
  upperLS = digitalRead(6);
  lowerLS = digitalRead(7);

  if (upperLS == LOW){
  	onUpperLS = true;
    onLowerLS = false;
  } else if (lowerLS == LOW){
  	onUpperLS = false;
    onLowerLS = true;
  }else{
  	onUpperLS = onUpperLS;
    onLowerLS = onLowerLS;
  }
}

