// https://wiki.dfrobot.com/SKU_DFR0954_MAX98357_I2S_Amplifier_Module

#include <DFRobot_MAX98357A.h>

DFRobot_MAX98357A amplifier;   

String musicList[100];   // SD card music playlist 

const uint8_t I2S_AMP_BCLK_PIN = GPIO_NUM_0;
const uint8_t I2S_AMP_LRCLK_PIN = GPIO_NUM_1;
const uint8_t I2S_AMP_DIN_PIN = GPIO_NUM_7;
const uint8_t SD_CHIP_SELECT_PIN = GPIO_NUM_2;


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

  while ( !amplifier.initI2S(I2S_AMP_BCLK_PIN, I2S_AMP_LRCLK_PIN, I2S_AMP_DIN_PIN) ){
    Serial.println("Initialize I2S failed !");
    delay(3000);
  }
  while (!amplifier.initSDCard(SD_CHIP_SELECT_PIN)){
    Serial.println("Initialize SD card failed !");
    delay(3000);
  }
  Serial.println("Initialize succeed!");
  amplifier.scanSDMusic(musicList);
  printMusicList();
  amplifier.setVolume(5);
  amplifier.closeFilter();
  amplifier.openFilter(bq_type_highpass, 500);
  amplifier.SDPlayerControl(SD_AMPLIFIER_PLAY);
  delay(5000);   
  if(musicList[1].length()){
    Serial.println("Changing Music...\n");
    amplifier.playSDMusic(musicList[1].c_str());
  }else{
    Serial.println("The currently selected music file is incorrect!\n");
  }
}

void loop()
{
  parseSerialCommand();
  delay(500);
}

void printMusicList(void)
{
  uint8_t i = 0;
  if(musicList[i].length()){
    Serial.println("\nMusic List: ");
  }else{
    Serial.println("The SD card audio file scan is empty, please check whether there are audio files in the SD card that meet the format!");
  }

  while(musicList[i].length()){
    Serial.print("\t");
    Serial.print(i);
    Serial.print("  -  ");
    Serial.println(musicList[i]);
    i++;
  }
}

void parseSerialCommand(void)
{
  String cmd;   
  float value;   
  if(Serial.available()){   
    cmd = Serial.readStringUntil('-');   

    if(cmd.equals("hp")){   
      Serial.println("Setting a High-Pass filter...\n");
      value =Serial.parseFloat();
      amplifier.openFilter(bq_type_highpass, value);
    }else if(cmd.equals("lp")){   
      Serial.println("Setting a Low-Pass filter...\n");
      value =Serial.parseFloat();
      amplifier.openFilter(bq_type_lowpass, value);
    }else if(cmd.equals("closeFilter")){   
      Serial.println("Closing filter...\n");
      amplifier.closeFilter();
    }else if(cmd.equals("vol")){
      Serial.println("Setting volume...\n");
      value =Serial.parseFloat();
      amplifier.setVolume(value);
    }else if(cmd.equals("start")){   
      Serial.println("starting amplifier...\n");
      amplifier.SDPlayerControl(SD_AMPLIFIER_PLAY);
    }else if(cmd.equals("pause")){   
      Serial.println("Pause amplifier...\n");
      amplifier.SDPlayerControl(SD_AMPLIFIER_PAUSE);
    }else if(cmd.equals("stop")){   
      Serial.println("Stopping amplifier...\n");
      amplifier.SDPlayerControl(SD_AMPLIFIER_STOP);
    }else if(cmd.equals("musicList")){   
      Serial.println("Scanning music list...\n");
      amplifier.scanSDMusic(musicList);
      printMusicList();
    }else if(cmd.equals("changeMusic")){   
      cmd = musicList[Serial.parseInt()];
      if(cmd.length()){
        Serial.println("Changing Music...\n");
        amplifier.playSDMusic(cmd.c_str());
      }else{
        Serial.println("The currently selected music file is incorrect!\n");
      }

    }else{   
      Serial.println("Help : \n \
      Currently available commands (format: cmd-value):\n \
        Start playback: e.g. start-\n \
        Pause playback: e.g. pause-\n \
        Stop playback: e.g. stop-\n \
        Print music list: e.g. musicList-\n \
        Change songs according to the music list: e.g. changeMusic-1\n \
        Set and open high-pass filter: e.g. hp-500\n \
        Set and open low-pass filter: e.g. lp-15000\n \
        Close filter: e.g. closeFilter-\n \
        Set volume: e.g. vol-5.0\n \
      For the detailed meaning, please refer to the code comments of this demo.\n");   
    }
    while(Serial.read() >= 0);   
  }
}
