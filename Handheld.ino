/*the handheld will send a message over serial bus:
 * A:B:C:D:EEEE
 * A = Guide active(1) guide inactive(0)
 * B = RA axis+ (1) RA axis- (2)
 * C = DEC axis+ (1) DEC axis- (2)
 * D = Focuser+ (1) Focuser- (2) 
 * E = Analogvalue for movement speed 0-255
 */

//hand controller PINOUT Arduino Pro Mini 5V
#define RaPlusInput 2
#define RaMinusInput 3
#define DecPlusInput 4
#define DecMinusInput 5
#define FocPlusInput 6
#define FocMinusInput 7
#define GuideInput 8
#define LedGuideOutput 9
#define AnalogSpeed A7

// define variables & constants
const int debounceTime = 50;
const int SerialBaudRate = 9600;
String SerialSeperate = ":";
String SerialSendOld;

boolean FuncStatus;
//boolean ButtonPressed;
boolean ButtonStatus;


boolean RaPlusPressed;
boolean RaMinusPressed;
boolean DecPlusPressed;
boolean DecMinusPressed;
boolean FocPlusPressed;
boolean FocMinusPressed;
boolean GuidePressed;
boolean GuideReset;

byte RaStatus;
byte DecStatus;
byte FocStatus;
byte GuideStatus;

int MoveSpeed;

void setup() {
  // put your setup code here, to run once:
 // ---define input ports---
pinMode(RaPlusInput, INPUT_PULLUP);
pinMode(RaMinusInput, INPUT_PULLUP);
pinMode(DecPlusInput, INPUT_PULLUP);
pinMode(DecMinusInput, INPUT_PULLUP);
pinMode(FocPlusInput, INPUT_PULLUP);
pinMode(FocMinusInput, INPUT_PULLUP);
pinMode(GuideInput, INPUT_PULLUP);

// ---define output ports---
pinMode(LedGuideOutput, OUTPUT);
  
 delay(1000);
 Serial.begin(SerialBaudRate);
 //Serial.parseInt();

 resetStatus();
}


void loop() {
  // put your main code here, to run repeatedly:
//StartMain: //Löschen

  //--- Check for guide key ---
 GuidePressed = FuncButtonPress (GuideInput,GuidePressed); //scanning guide-button on hand controller
 //Serial.print("Guide pressed: "); //debug
 //Serial.println(GuidePressed); //debug
 if (GuidePressed == 1 && GuideReset == 0 ){ //If guide key is pressed and wasn't pressed before
      if (GuideStatus==0){          //if guide status was OFF, then activate guide function here
        GuideStatus=1;
       digitalWrite (LedGuideOutput,1);//Staus led shows the active guiding
       }
  else if (GuideStatus == 1) { //If guide status is ON, then change it to OFF 
      GuideStatus=0;
      digitalWrite (LedGuideOutput,0);
   }
   GuideReset=1;    //set marker for pressed guide key for next loop
 }
   else{
    GuideReset=0; //reset marker if guide key is released
   }
  
  //--- Check Speed dial here ---
  MoveSpeed = analogRead(AnalogSpeed);
  MoveSpeed = MoveSpeed / 40;
  
  //--- Check for FOCUS keys here (when locked in guide, then put after DEC keys---
  FocPlusPressed = FuncButtonPress (FocPlusInput,FocPlusPressed); //read RA+ key
   //Serial.println(FocPlusPressed); //debug
  FocMinusPressed = FuncButtonPress (FocMinusInput,FocMinusPressed); //read RA- key
  //Serial.println(FocMinusPressed); //debug
  if (FocPlusPressed == 1 && FocMinusPressed == 0){
      FocStatus=1; //when RA+ key is pressed and RA- is not pressed set Status to 1
  }
   else if (FocPlusPressed == 0 && FocMinusPressed == 1){
      FocStatus=2; //when RA- key is pressed and RA+ is not pressed set Status to 2
   }
  else {
    FocStatus=0; // if none or all RA keys are pressed, Status is 0
  }
  
 
 
  
  //--- Check for RA keys ---
  RaPlusPressed = FuncButtonPress (RaPlusInput,RaPlusPressed); //read RA+ key
  RaMinusPressed = FuncButtonPress (RaMinusInput,RaMinusPressed); //read RA- key
  if (RaPlusPressed == 1 && RaMinusPressed == 0){
      RaStatus=1; //when RA+ key is pressed and RA- is not pressed set Status to 1
  }
   else if (RaPlusPressed == 0 && RaMinusPressed == 1){
      RaStatus=2; //when RA- key is pressed and RA+ is not pressed set Status to 2
   }
  else {
    RaStatus=0; // if none or all RA keys are pressed, Status is 0
  }
    
  //--- Check for DEC keys ---
  DecPlusPressed = FuncButtonPress (DecPlusInput,DecPlusPressed); //read DEC+ key
  DecMinusPressed = FuncButtonPress (DecMinusInput,DecMinusPressed); //read DEC- key
  if (DecPlusPressed == 1 && DecMinusPressed == 0){
      DecStatus=1; //when DEC+ key is pressed and RA- is not pressed set Status to 1
  }
   else if (DecPlusPressed == 0 && DecMinusPressed == 1){
      DecStatus=2; //when DEC- key is pressed and RA+ is not pressed set Status to 2
   }
  else {
    DecStatus=0; // if none or all DEC keys are pressed, Status is 0
  }

   //--- Lock RA DEC keys when guiding in active ---
  if (GuideStatus == 1 ){ //If guide status is ON, then lock the buttons for RA & DEC
      RaStatus=0; // Lock RA buttons
      DecStatus=0; // Lock DEC buttons
      //FocStatus=0;
  }
SendSerial();
delay(100); 
 
}

void SendSerial(){
  //create the string with the keys and values
   String SerialSendActual = (GuideStatus + SerialSeperate + RaStatus + SerialSeperate + DecStatus + SerialSeperate + FocStatus + SerialSeperate + MoveSpeed); 
   if ( SerialSendActual != SerialSendOld){ //check if the message has changed, otherwise only send changes over serial
    Serial.println(SerialSendActual);
  SerialSendOld = SerialSendActual;
   }
 else{
 }
   return;
  }

void resetStatus(){
  GuideStatus=0;
  RaStatus=0;
  DecStatus = 0;
  FocStatus=0;
  return;
}
  
//debounce of input pins   
boolean FuncButtonPress (byte InputPin, boolean FuncStatus){
  boolean ButtonPressed = !digitalRead (InputPin);
  if (ButtonPressed == 1){
  delay (debounceTime); //debounce
  if (ButtonPressed == 1) {
        FuncStatus= 1;
    }
    }
    else{
      FuncStatus= 0;
    }
return FuncStatus;
 
}
