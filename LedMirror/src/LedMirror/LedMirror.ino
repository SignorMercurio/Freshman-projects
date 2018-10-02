#include <LedControl.h>            //for 8*8 led matrix control
#include <SoftwareSerial.h>       //for BLE

LedControl mat = LedControl(8, 12, 10, 1); //DIN:D8, CLK:D12, CS: D10, numOfDevices: 1
unsigned int numbers[][5] =                   //store digit 0-9 matrix 
{
  {14,10,10,10,14},
  {2,2,2,2,2},
  {14,2,14,8,14},
  
  {14,2,14,2,14},
  {10,10,14,2,2},
  {14,8,14,2,14},
  {14,8,14,10,14},
  {14,2,2,2,2},
  {14,10,14,10,14},
  {14,10,14,2,14},
};


SoftwareSerial mySerial(4, 5);    //Default RX, TX
#define my_Serial mySerial

int brightness;

float light;
int heat;
boolean flag = true;               //the auto-adjusting function is enabled by default

#define PIN 9
#define SP(x) Serial.print(x)

void setup() 
{
  
  //led matrix settings
  mat.shutdown(0, false);         //switch on the led matrix 0
  mat.setIntensity(0, 1);
  mat.clearDisplay(0);
  
  //display the 'celsius' symbol
  mat.setRow(0, 5, 92);
  mat.setRow(0, 6, 16);
  mat.setRow(0, 7, 28);

  //BLE settings and instructions. Serial needs to be open.
  my_Serial.begin(9600);
  Serial.begin(9600);
  SP("To adjust the brightness manually, please enter a number between 1-255.\n");
  SP("Note: This will disable the auto-adjusting function. To enable it again, simply enter '256'.\n");
  while (Serial.read() >= 0);         //flush the serial buffer
  
}

void loop() 
{                          
  adjustBrightnessCheck();
  sensorsProcessing();
  delay(1000);
}

inline void adjustBrightnessCheck()
{
  if (Serial.available() > 0)
  {
    delay(100);                         //wait for transfering data
    brightness = Serial.parseInt();
    while (Serial.read() >= 0);

    if (brightness == 256)
    {
      flag = true;
      SP("Auto-adjusting enabled.\n");
    }
    else
    {
      while (brightness < 1 || brightness > 256)
      {
        SP("Please enter a number between 1-255!\n");
        while ((brightness = Serial.parseInt()) == 0);
        while (Serial.read() >= 0);
      }
      analogWrite(PIN, brightness);
      flag = false;
      
      SP("Set brightness to ");
      SP(brightness);
      SP(" successfully.\n");
    }
  }
}

inline void sensorsProcessing()
{
  heat = round(analogRead(A1) * (5.0 / 1023.0 * 100));
  //led displaying
  for (int i = 0; i < 5; ++i)
    mat.setRow(0, i, (numbers[heat/10][i]<<4) + numbers[heat%10][i]);
  
  if (flag)
  {
    light = analogRead(A0);
    //Serial.println(light);
    if (light < 200) light = 200;
    else if (light > 650) light = 650;
    analogWrite(PIN, round(-0.5 * light) + 355);
  }
}
