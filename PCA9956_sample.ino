// Very unfriendly data sheet: https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf

#include <Wire.h>

//PCA9956 registor addresses
#define MODE1       0x00
#define MODE2       0x01
#define PWM0        0x0A
#define PWM8        0x12
#define PWM23       0x21

#define GRPPWM      0x0A
#define GRPFREQ     0x0B
#define LEDOUT0     0x02
#define IREF0       0x22

#define LEDMODE_FULLOFF 0x00                 //full off
#define LEDMODE_FULLON 0b01010101            //full on
#define LEDMODE_PWM 0b10101010            //control over pwm

// #define DEV_ADDRESS 0x3F

#define CONTROL_LEDS 24
#define LED_BRIGHTNESS 0x06

uint8_t devAddress;

void i2cWrite(uint8_t slave_address, uint8_t *data, uint8_t num){
  Wire.beginTransmission(slave_address);
  for(int i=0;i<num;i++){
    Wire.write(*(data+i));
  }
  Wire.endTransmission();
}

#ifndef DEV_ADDRESS
// i2c scanner taken from here: https://playground.arduino.cc/Main/I2cScanner
uint8_t i2cScan(){
  uint8_t foundAddress = 1;
  for(uint8_t address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();
 
    if (error == 0){
      Serial.print("\nI2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
      foundAddress = address;
      break;
    }
    else if (error==4){
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  return foundAddress;
}
#endif

void setLEDPortMode(uint8_t portAdr, uint8_t mode){
  uint8_t cmd[2];
  cmd[0] = portAdr;
  cmd[1] = mode;      //disables all options...
  i2cWrite(devAddress, cmd, sizeof(cmd));
}

void setLEDCurrent(uint8_t portAdr, uint8_t iref){
  uint8_t cmd[2];
  cmd[0] = portAdr;
  cmd[1] = iref;      //disables all options...
  i2cWrite(devAddress, cmd, sizeof(cmd));
}

void onLED(uint8_t selectLED){
  uint8_t regAdr = uint8_t(selectLED/4) + LEDOUT0;
  uint8_t regVal = 1 << ((selectLED%4) * 2);
  Serial.print(selectLED);
  Serial.print(" ");
  Serial.print(regAdr, HEX);
  Serial.print(" ");
  Serial.print(regVal, BIN);
  Serial.println("");
  setLEDPortMode(regAdr, regVal);

}

void offLED(uint8_t selectLED){
  uint8_t regAdr = uint8_t(selectLED/4) + LEDOUT0;
  uint8_t modeBit = (selectLED%4) * 2;
  uint8_t regVal = 0b10 << modeBit;
  setLEDPortMode(regAdr, regVal);
}

//MODE1 reg setting
void ledMode1Setting(uint8_t regsetting){
  uint8_t cmd[2];
  cmd[0] = MODE1;
  cmd[1] = regsetting;
  i2cWrite(devAddress, cmd, 2);
}

void initLEDdriver(){
  ledMode1Setting(0x00);       //disables all options...

  //LED out put mode setting
  for(uint8_t i = 0; i < CONTROL_LEDS; i++){
    setLEDPortMode(LEDOUT0 + i, LEDMODE_FULLOFF);  // set leds to full on
    setLEDCurrent(IREF0 + i, LED_BRIGHTNESS);       //set led current
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.flush();

  Wire.begin();
  #ifndef DEV_ADDRESS
    devAddress = i2cScan();
  #else
    devAddress = DEV_ADDRESS;
  #endif

  initLEDdriver();
}

void loop()
{
  for(uint8_t i = 0; i < CONTROL_LEDS; i++){
    onLED(i);

    delay(1000);

    offLED(i);
  }
}
