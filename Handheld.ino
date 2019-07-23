
//hand controller PINOUT Arduino Pro Mini 5V
#define RaPlusInput 2
#define RaMinusInput 3
#define DecPlusInput 4
#define DecMinusInput 5
#define FocPlusInput 6
#define FocMinusInput 7
#define GuideInput 8
#define LedGuideOutput 9
#define AnalogSpeed A3

// define variables & constants
const int debounceTime = 80;
const int SerialBaudRate = 9600;
string SerialSeperate = ':';
string SerialEnd = ":X";


boolean ButtonPressed;
boolean ButtonStatus;
boolean FuncStatus;

boolean RaPlusPressed;
boolean RaMinusPressed;
boolean DecPlusPressed;
boolean DecMinusPressed;
boolean FocPlusPressed;
boolean FocMinusPressed;
boolean GuidePressed;

byte RaStatus;
byte DecStatus;
byte FocStatus;
byte GuideStatus;

int MoveSpeed;

void setup() {
  
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
 Serial.parseInt();
}

void loop() 
{
//StartMain: //Löschen
  //--- Check for guide key ---
 GuidePressed = FuncButtonPress(GuideInput,GuidePressed); //scanning guide-button on hand controller
 if (GuidePressed == 1){
    if (GuideStatus == 0 ){ //If guide status is OFF, then change it to ON
      GuideStatus=1;
      digitalWrite (LedGuideOutput,1);//Staus led shows the active guiding
    }
   else{ //If guide status is ON, then change it to OFF 
      GuideStatus=0;
      digitalWrite (LedGuideOutput,0);
   }
 }
  
  //--- Check Speed dial here ---
  MoveSpeed = analogRead(AnalogSpeed);
  MoveSpeed = Movespeed / 10;
  
  //--- Check for FOCUS keys here (when locked in guide, then put after DEC keys---
  FocPlusPressed = FuncButtonPress(FocPlusInput,FocPlusPressed); //read RA+ key
  FocMinusPressed = FuncButtonPress(FocMinusInput,FocMinusPressed); //read RA- key
  if (FocPlusPressed == 1 && FocMinusPressed == 0){
      FocStatus=1; //when RA+ key is pressed and RA- is not pressed set Status to 1
  }
   else if (FocPlusPressed == 0 && FocMinusPressed == 1){
      FocStatus=2; //when RA- key is pressed and RA+ is not pressed set Status to 2
   }
  else {
    FocStatus=0; // if none or all RA keys are pressed, Status is 0
  }
  
 
  //--- Lock RA DEC keys when guiding in active ---
  if (GuideStatus == 1 ){ //If guide status is ON, then lock the buttons for RA & DEC
      RaStatus=0; // Lock RA buttons
      DecStatus=0; // Lock DEC buttons
      //FocStatus=0;
      SendSerial();
  }
  //--- Check for RA keys ---
  RaPlusPressed = FuncButtonPress(RaPlusInput,RaPlusPressed); //read RA+ key
  RaMinusPressed = FuncButtonPress(RaMinusInput,RaMinusPressed); //read RA- key
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
  DecPlusPressed = FuncButtonPress(DecPlusInput,DecPlusPressed); //read RA+ key
  DecMinusPressed = FuncButtonPress(DecMinusInput,DecMinusPressed); //read RA- key
  if (DecPlusPressed == 1 && DecMinusPressed == 0){
      DecStatus=1; //when RA+ key is pressed and RA- is not pressed set Status to 1
  }
   else if (DecPlusPressed == 0 && DecMinusPressed == 1){
      DecStatus=2; //when RA- key is pressed and RA+ is not pressed set Status to 2
   }
  else {
    DecStatus=0; // if none or all DEC keys are pressed, Status is 0
  }

  
 void SendSerial(){
  string SerialHandheld = (GuideStatus + SerialSeperate + RaStatus + SerialSeperate + DecStatus + SerialSeperate + FocStatus + SerialSeperate + MoveSpeed + SerialEnd);
   // wenn das nicht geht probieren mit byte als Variablen für string statt boolean
  Serial.println(SerialHandheld);
   delay(100)
     //goto StartMain; //Löschen
  }
}
  
//debounce of input pins   
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
