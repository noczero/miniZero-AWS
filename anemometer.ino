
/*
 * 4051 pin switch
 * S2 S1 S0   Description
 * 0  0   1   Output in y1
 * 0  1   0   Output in y2
 */

#define ADC A0 // ANALOG PIN FOR SENSOR
#define internalLED 2 
#define LED1 5
#define LED2 4
#define D5 14 // OUTPUT HIGH OR LOW S2
#define D6 12 // OUTPUT HIGH OR LOW S1
#define D7 13 // OUTPUT HIGH OR LOW S0
#define hall 2 // digital hall effect

int readingAnalog = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  // Config to output
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  // LED mode
  //pinMode(internalLED, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  //interrupt all gpio beside gpio16
  pinMode(hall, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hall),detectMagnet,CHANGE);

  // default switch to Y1
  switchMode(D7,HIGH,D6,LOW,D5,LOW);
}

void switchMode(int pinS0, int valS0, int pinS1, int valS1 , int pinS2, int valS2){
  // Digital write as binary
  // input pinS0, pinS1, pinS2 as digital pin
  // input valS0 , valS1 , valS2  : HIGH OR LOW
  digitalWrite(pinS0,valS0);
  digitalWrite(pinS1,valS1);
  digitalWrite(pinS2,valS2);
  delay(10); 
}

void ledOn(int led){
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led,LOW);
}

int valPWM = 0;
void redupTerang(int pinPWM, int rawADC){
  valPWM = map(rawADC,0,1024,0,255);
  analogWrite(pinPWM, valPWM);
}

void detectMagnet(){
  digitalWrite(LED2, HIGH);
  Serial.println("Magnet Detected");
}

String convertArahAngin(int value){
  // input 0 - 360
  // output utara, timur laur, timur, tenggara, selatan, barat daya, barat laut
  // 0 , 45 , 90 , 135, 180 , 225 , 270 , 315

  if ( (value > 337.5 && value <= 360) || (value >= 0 && value <= 22.5))
    return "Utara"; 
  else if ( value > 22.5 && value <= 45+22.5)
    return "Timur Laut";
  else if ( value > 90-22.5 && value <= 90+22.5)
    return "Timur";
  else if ( value > 135-22.5 && value <= 135+22.5)
    return "Tenggara";
  else if ( value > 180-22.5 && value <= 180+22.5)
    return "Selatan";
  else if ( value > 225-22.5 && value <= 225+22.5)
    return "Barat Daya";
  else if ( value > 270-22.5 && value <= 270+22.5)
    return "Barat";
  else if ( value > 315-22.5 && value <= 315+22.5)
    return "Barat Laut";
  
}

int arahAngin = 0, rpm = 0, hallEffectRead = 0, potensioRead = 0, digitalHall =0;
void loop() {
    // put your main code here, to run repeatedly:
    switchMode(D7,HIGH,D6,LOW,D5,LOW); // y1 Input
    potensioRead = analogRead(ADC);
    // notify to LED
    redupTerang(LED1,potensioRead);
    arahAngin = map(potensioRead,0,1024,0,360);
    Serial.print(" RAW Potensio : ");
    Serial.print(potensioRead);
    Serial.print(" Arah angin : ");
    Serial.print(arahAngin);
    Serial.print(" Mata Angin : ");
    Serial.println(convertArahAngin(arahAngin));
    
    switchMode(D7,LOW,D6,HIGH,D5,LOW); // y2 input
    hallEffectRead = analogRead(ADC);
    Serial.print("Hall Effect Reading : ");
    Serial.print(hallEffectRead);
    Serial.println();

    // hall Value
    digitalHall = digitalRead(hall);
    Serial.print("Digital Hall : ");
    Serial.println(digitalHall);
    
    //ledOn(internalLED);
    //ledOn(LED1);
    digitalWrite(LED2, LOW); // defult LOW, HIGH when detects magnet
    delay(100);
}
