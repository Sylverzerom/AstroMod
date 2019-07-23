/*
  Der Schrittmotors soll alle 0,127s ein Schritt machen.
  Als Treiber soll der A4988 Baustein genutzt werden.
  Der Abstand der Schrittdauer entspricht einer Timerfrequenz von 15,76Hz. Denn fÃƒÂ¼r 0,0635s muss der Pin HIGH sein und fÃƒÂ¼r 0,0635s LOW.
  Somit entsteht ein 50% Rechtecksignal mit einer Peroidendauer von 0,127s.
*/
// ---define PINs---
// A4988 for RA axis
#define enableRa 2
#define directionRa 3
#define stepRa 4
#define ms1Ra 5
#define ms2Ra 6

// A4988 for DEC axis
#define enableDec 7
#define directionDec 8
#define stepDec 9
#define ms1Dec 10
#define ms2Dec 11

// A4988 for Focuser axis
#define enableFoc 7
#define directionFoc 8
#define stepFoc 9
#define ms1Foc 10
#define ms2Foc 11

//---define constants---
//Timer compare values for diffrent speeds of the stepper
const int compareRAfast = 12670; //siehe Berechnung Excel
const int compareRAslow = 25838;
const int compareRAguide = 31677;

const int SerialBaudRate = 9600;

byte GuideStatus;
byte RaStatus;
byte DecStatus;
byte FocStatus;
byte MoveSpeedRaw;

void setup() {
  
// ---define output ports---
pinMode(enableRa, OUTPUT);
pinMode(directionRa, OUTPUT);
pinMode(stepRa, OUTPUT);
pinMode(ms1Ra, OUTPUT);
pinMode(ms2Ra, OUTPUT);
  
pinMode(enableDec, OUTPUT);
pinMode(directionDec, OUTPUT);
pinMode(stepDec, OUTPUT);
pinMode(ms1Dec, OUTPUT);
pinMode(ms2Dec, OUTPUT);
  
pinMode(enableFoc, OUTPUT);
pinMode(directionFoc, OUTPUT);
pinMode(stepFoc, OUTPUT);
pinMode(ms1Foc, OUTPUT);
pinMode(ms2Foc, OUTPUT);

  //define RX and TX for Serial1 HERE

  //Timer configuration
  cli();//stop interrupts
  
  //Timer1 Config
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  TCCR1B |= (1 << WGM12); // turn on CTC mode

  sei();//allow interrupts
  
  Serial.begin(SerialBaudRate); 
}

void loop() { //main loop starts here!

 guideStatus = FuncButtonPress(guideInput,guideStatus); //scanning guide-button on hand controller
 if (guideStatus == 1){
  guideMove();
  return;
 } 


 
slowStatus = FuncButtonPress(slowInput,slowStatus);  //scanning slow-button on hand controller
 if (slowStatus == 1){
  slowMove();
 }
 else{
  fastMove();
 }
 

}//main loop ends here!

// Interrupt routine from Timer1 (used for RA axis)
ISR(TIMER1_COMPA_vect) {
  digitalWrite(stepRa, toggleRa); // Output step PIN is HIGH/LOW with the interrupt from Timer 
  toggleRA= !toggleRA; // Pin is changed from HIGH to LOW every interrupt
  }
  
}

void guideMove(){
   if (guideMoveStatus==0){
  slowStatus = 0; //guiding mode kills manual mode
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
   // Pin config for step-config on the driverboard ms1RA ms2RA
   //...
   //
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
  // Pin config for step-config on the driverboard ms1RA ms2RA
  //...
  //
  resetStatus();  
  fastMoveStatus = 1;
   }
}

void resetStatus(){
  slowMoveStatus=0;
  fastMoveStatus = 0;
  guideMoveStatus=0;
  digitalWrite (ledMode,0);
}

