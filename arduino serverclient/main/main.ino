String inputString = "";
boolean stringComplete = false;

void setup() 
{
  Serial.begin(9600);
  udp_setup(); 
  inputString.reserve(200);
}

void loop() 
{
  udp_check(); 
  if (stringComplete) 
  {
    udp_send(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    char inChar = (char)Serial.read();
    if (inChar>0)
    inputString += inChar;
    if (inChar == '\n') 
    {
      stringComplete = true;
    }
  }
}
