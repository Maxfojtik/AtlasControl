#include <iBus.h>
#include <Servo.h>

#define MAX_CHANNELS 10

iBus receiver(Serial1, MAX_CHANNELS);  // Serial on ATMega328 boards(Uno, Nano, etc.)
                                     // Serial1 on ATMega32u4 boards(Micro, Leonardo)


Servo leftMotors;
Servo rightMotors;
void setup() {
  receiver.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  leftMotors.attach(6);
  rightMotors.attach(7);
}

void loop() {

  receiver.process();

  digitalWrite(LED_BUILTIN, receiver.available());  // turn led on if connected
    boolean valid = true;
    int channels[MAX_CHANNELS];
    for(byte i = 1; i <= MAX_CHANNELS; i++){  // get channel values starting from 1
      int channelMicro = receiver.get(i);
      valid = valid && (channelMicro > 995 && channelMicro < 2005);//keep track if we have been valid
      channels[i-1] = channelMicro;
    }
    if(valid)
    {
      for(byte i = 0; i < MAX_CHANNELS; i++)
      {
//        Serial.print(channels[i]);
//        Serial.print('\t');
      }
//      Serial.println(receiver.get(0));  // received packet quality. If 0 -> there are some corrupted values

      double leftVal = (channels[1]-1500.0)/500-(channels[0]-1500.0)/500;
      double rightVal = (channels[1]-1500.0)/500+(channels[0]-1500.0)/500;

      leftVal = leftVal * 90 + 90;
      rightVal = rightVal * 90 + 90;
      

      leftVal = constrain(leftVal, 0, 180);
      rightVal = constrain(rightVal, 0, 180);
      
      
      Serial.print(leftVal);
      Serial.print("\t");
      Serial.println(rightVal);

      leftMotors.write(leftVal);
      rightMotors.write(rightVal);

    }
    else
    {
      leftMotors.write(90);
      rightMotors.write(90);
    }
  delay(10);
  
}
