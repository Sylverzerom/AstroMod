
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

boolean guideButtonStatus = 0;
boolean guideuttonPressed = 0;
boolean guideStatus = 0;


void setup() {

  // ---Ausgaenge definieren---
pinMode(enableRA, OUTPUT);
pinMode(directionRA, OUTPUT);
pinMode(stepRA, OUTPUT);
pinMode(ms1RA, OUTPUT);
pinMode(ms2RA, OUTPUT);

pinMode(ledMode, OUTPUT);

// ---Eingaenge definieren---
digitalWrite(RAplus, HIGH);
digitalWrite(RAminus, HIGH);
digitalWrite(guideInput, HIGH);
digitalWrite(slowInput, HIGH);

  cli();//stop interrupts

  //Timer1 Config
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  TCCR1B |= (1 << WGM12); // turn on CTC mode

  sei();//allow interrupts
}

void loop() {

slowButtonPressed = digitalRead(slowInput);
 if (slowButtonStatus != slowButtonPressed) { // Aenderung am Schalter?
   if (slowButtonPressed == 1) { // gedrueckt
      slowStatus = !slowStatus; // ...welche das ist wird abgewechselt.
      digitalWrite(ledMode, slowStatus);
   }
   slowButtonStatus = slowButtonPressed; // Merken
    // Debounce
 }

/*
slowButtonPressed = digitalRead(slowInput);
if (slowButtonPressed) { // gedrueckt
      slowStatus = 1;
}
else {
  slowStatus=0;
  }
*/  
 if (slowStatus == 1){
  slowMove();
 }
 else{
  fastMove();
 }
delay (50); //debounce
}

// Interrupt routine from Timer1 (used for RA axis)
ISR(TIMER1_COMPA_vect) {
 // if ((digitalRead(RAplus)==1) || (digitalRead(RAminus)==1) || guideStatus==1){ //TEST WITH INPUT BUTTONS RA
  digitalWrite(stepRA, toggleRA); // Output step PIN is HIGH/LOW with the interrupt from Timer 
  toggleRA = !toggleRA; // Pin is changed from HIGH to LOW every interrupt
 // }
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
  fastMoveStatus==1;
   }
}

void resetStatus(){
  slowMoveStatus=0;
  fastMoveStatus = 0;
  guideStatus=0;
  {
