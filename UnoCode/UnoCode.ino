const int redPaint = 13;
const int yellowPaint = 12;
const int bluePaint = 11;
const int whitePaint = 10;
const int blackPaint = 9;

const int penBrush = 8;
const int spatterBrush = 7;
const int squareBrush = 6;

void setup() {
  pinMode(redPaint, INPUT);
  pinMode(yellowPaint, INPUT);
  pinMode(bluePaint, INPUT);
  pinMode(whitePaint, INPUT);
  pinMode(blackPaint, INPUT);
  Serial.begin(9600);
}

void loop() {
  int redState = digitalRead(redPaint);
  int yellowState = digitalRead(yellowPaint);
  int blueState = digitalRead(bluePaint);
  int whiteState = digitalRead(whitePaint);
  int blackState = digitalRead(blackPaint);
  
  Serial.print(redState);
  Serial.print(",");
  Serial.print(yellowState);
  Serial.print(",");
  Serial.print(blueState);
  Serial.print(",");
  Serial.print(whiteState);
  Serial.print(",");
  Serial.println(blackState); 
  delay(50);
}
