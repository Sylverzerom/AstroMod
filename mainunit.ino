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
  
  //--- check Serial Connection for new data and seperate the string into diffrent variables ---
   if(Serial.available()) //while there is data available
  {
    string_received = Serial.read();  // read serial data into string
    GuideStatus = atof(strtok(string_received, ":"));
    RaStatus = atof(strtok(NULL, ":"));
    DecStatus = atof(strtok(NULL, ":"));
    FocStatus = atof(strtok(NULL, ":"));
    MoveSpeedRaw = atof(strtok(NULL, ":"));
  }

 if (GuideStatus == 1){
  guideMove();
 } 
  else{   //weglassen??
  }
  
  
  
  if (RaStatus == 1){
    
 
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
  digitalWrite(stepRa, toggle);// Output step PIN is HIGH/LOW with the interrupt from Timer 
  digitalWrite(stepDec, toggle);// Output step PIN is HIGH/LOW with the interrupt from Timer 
  digitalWrite(stepFoc, toggle);// Output step PIN is HIGH/LOW with the interrupt from Timer 
  toggle= !toggle; // Pin is changed from HIGH to LOW every interrupt
  }
  
}

void guideMove(){
  if guide
  cli();//stop interrupts
  OCR1A = compareRaGuide;
  TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS11);  //clear past config (set all back to 0)
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);  // Set CS12, CS21, CS10 bits for 1 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();//allow interrupts
  // Pin config for step-config on the driverboard ms1RA ms2RA
  digitalWrite(enableRa, HIGH);
  digitalWrite(directionRa, HIGH);
  digitalWrite(ms1Ra, HIGH);
  digitalWrite(ms2Ra, HIGH);
  digitalWrite(ms3Ra, HIGH);
/*pinMode(enableRa, OUTPUT);
pinMode(directionRa, OUTPUT);
pinMode(stepRa, OUTPUT);
pinMode(ms1Ra, OUTPUT);
pinMode(ms2Ra, OUTPUT);
  *///
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
  fastMoveStatus = 0;
  guideMoveStatus=0;
}

