// https://wiki.dfrobot.com/SKU_DFR0954_MAX98357_I2S_Amplifier_Module

void setup()
{
  const int led = 10;
  pinMode(led,OUTPUT);
  for (int i=0; i<5;i++)
  {
    digitalWrite(led,HIGH);
    delay(500);
    digitalWrite(led,LOW);
    delay(500);
  }

  Serial.begin(115200);

  Serial.println("Here we go...");
}

void loop()
{

  
}
