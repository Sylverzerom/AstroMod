/*
  Der Schrittmotors soll alle 0,127s ein Schritt machen.
  Als Treiber soll der A4988 Baustein genutzt werden.
  Der Abstand der Schrittdauer entspricht einer Timerfrequenz von 15,76Hz. Denn fÃƒÂ¼r 0,0635s muss der Pin HIGH sein und fÃƒÂ¼r 0,0635s LOW.
  Somit entsteht ein 50% Rechtecksignal mit einer Peroidendauer von 0,127s.
*/
// ---Pins definieren---
// A4988 fuer RA-Achse
#define enableRA 2
#define directionRA 3
#define stepRA 4
#define ms1RA 5
#define ms2RA 6

// A4988 fuer DEC-Achse
#define enableDEC 7
#define directionDEC 8
#define stepDEC 9
#define ms1DEC 10
#define ms2DEC 11

//hand controller
#define RAplus A0
#define RAminus A1
#define guideInput A4
#define slowInput A5
#define ledMode 12

//---Konstanten definieren---
//Timer compare Werte fÃƒÂ¼r Geschwindigkeit der Schritte
const int compareRAfast = 12670; //siehe Berechnung Excel
const int compareRAslow = 25838;
const int compareRAguide = 31677;

boolean toggleRA = 0;
boolean slowButtonStatus = 0;
boolean slowButtonPressed = 0;
boolean slowStatus = 0;

boolean slowMoveStatus = 0;
boolean fastMoveStatus = 0;
boolean guideMoveStatus = 0;

boolean guideButtonStatus = 0;
boolean guideButtonPressed = 0;
boolean guideStatus = 0;

boolean ButtonPressed;
boolean ButtonStatus;
boolean FuncStatus;

const int debounceTime =80;

void setup() {

Serial.begin(9600); //for Debug and Testing only 
  
  // ---Ausgaenge definieren---
pinMode(enableRA, OUTPUT);
pinMode(directionRA, OUTPUT);
pinMode(stepRA, OUTPUT);
pinMode(ms1RA, OUTPUT);
pinMode(ms2RA, OUTPUT);

pinMode(ledMode, OUTPUT);

// ---Eingaenge definieren---
pinMode(RAplus, INPUT_PULLUP);
pinMode(RAminus, INPUT_PULLUP);
pinMode(guideInput, INPUT_PULLUP);
pinMode(slowInput, INPUT_PULLUP); 

//Timer configuration
  cli();//stop interrupts
  
  //Timer1 Config
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  TCCR1B |= (1 << WGM12); // turn on CTC mode

  sei();//allow interrupts
}

void loop() {

 guideStatus = FuncButtonPress(guideInput,guideStatus); //Abfrage guide-Taste
 if (guideStatus == 1){
  guideMove();
  //serial debugging starts here
Serial.print("ButtonPressed: ");
Serial.print(ButtonPressed);
Serial.print(" | ButtonStatus: ");
Serial.print(ButtonStatus);  
Serial.print(" | FuncStatus: ");
Serial.print(FuncStatus); 
Serial.print(" | Status slow: ");
Serial.print(slowStatus);
Serial.print(" | Status guide: ");
Serial.println(guideStatus);
  
//serial debugging ends here
  return;
 }


 
slowStatus = FuncButtonPress(slowInput,slowStatus);  //Abfrage slowmove-Taste
 if (slowStatus == 1){
  slowMove();
 }
 else{
  fastMove();
 }
 
//serial debugging starts here
Serial.print("ButtonPressed: ");
Serial.print(ButtonPressed);
Serial.print(" | ButtonStatus: ");
Serial.print(ButtonStatus);  
Serial.print(" | FuncStatus: ");
Serial.print(FuncStatus); 
Serial.print(" | Status slow: ");
Serial.print(slowStatus);
Serial.print(" | Status guide: ");
Serial.println(guideStatus);
  
//serial debugging ends here
//delay (debounceTime); //debounce
}

// Interrupt routine from Timer1 (used for RA axis)
ISR(TIMER1_COMPA_vect) {
 // if ((digitalRead(RAplus)==1) || (digitalRead(RAminus)==1) || guideStatus==1){ //TEST WITH INPUT BUTTONS RA
  digitalWrite(stepRA, toggleRA); // Output step PIN is HIGH/LOW with the interrupt from Timer 
  toggleRA= !toggleRA; // Pin is changed from HIGH to LOW every interrupt
}

void guideMove(){
   if (guideMoveStatus==0){
  slowStatus = 0;
  resetStatus();
  guideMoveStatus=1; 
  }
}


void slowMove() {
  if (slowMoveStatus==0){
  cli();//stop interrupts
  OCR1A = compareRAslow;
  TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS11);  //clear past config (set all back to 0)
  TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);  // Set CS12, CS11 and CS10 bits for 64 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();//allow interrupts
  resetStatus(); 
  slowMoveStatus=1;
  } 
}

void fastMove() {
   if (fastMoveStatus==0){
  cli();//stop interrupts
  OCR1A = compareRAfast;
  TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS11);  //clear past config (set all back to 0)
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);  // Set CS12, CS21, CS10 bits for 1 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();//allow interrupts
  resetStatus();  
  fastMoveStatus = 1;
   }
}

void resetStatus(){
  slowMoveStatus=0;
  fastMoveStatus = 0;
  guideMoveStatus=0;
}

//Unterfunktion zur Abfrage eines Eingangpins    
boolean FuncButtonPress (byte InputPin, boolean FuncStatus){
  
  //boolean FuncStatus;
  ButtonPressed = !digitalRead (InputPin);
  if (ButtonPressed==1){
  delay (debounceTime); //debounce
  if ((ButtonPressed==1) && (ButtonStatus==0)){
        FuncStatus= !FuncStatus;
        ButtonStatus=1;
    }
    }
    else{
      ButtonStatus=0;
      delay (debounceTime);
    }
return FuncStatus;
 
}
