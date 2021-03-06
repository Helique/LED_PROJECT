#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>

#define PIN 6
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
double temp;
int address;
int red;
int green;
int blue;
int receivingState = 0;
int frame_size = 50;
int strip_index = 0;
int swap_ready = 0;
void setup() {
 pinMode(13, OUTPUT);
 //Serial.begin(9600);
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
 strip.begin();
 //Timer1.initialize(33333);
 Timer1.initialize(20000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
 Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}
 
void loop() {
 delay(100);
 temp = GetTemp();
}
 
// callback for received data
void receiveData(int byteCount){
 while(Wire.available()) {
  number = Wire.read();
 
  switch(receivingState){
    //case 0:
      
      //strip.show();
      /*Serial.print(red);
      Serial.print(',');
      Serial.print(green);
      Serial.print(' ');
      Serial.print(blue);
      Serial.print('\n');*/
      
      //receivingState++;
      //break;
    //case 0:
     // address = number;
      //receivingState++;
      //break;
    case 0:
      red = number;
      receivingState++;
      break;
    case 1:
      green = number;
      receivingState++;
      break;
    case 2:
      blue = number;
      strip.setPixelColor(strip_index, strip.Color(red,green, blue));
      strip_index++;
      if(strip_index >= frame_size){
        strip_index = 0;
        swap_ready = 0;
        while(swap_ready == 1){
           //do nothing
        }
      }
      receivingState = 0;
      break;
  }  
  
 }
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
}
// callback for sending data
void sendData(){
 Wire.write(receivingState);
}
 
// Get the internal temperature of the arduino
double GetTemp(void)
{
 unsigned int wADC;
 double t;
 ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
 ADCSRA |= _BV(ADEN); // enable the ADC
 delay(20); // wait for voltages to become stable.
 ADCSRA |= _BV(ADSC); // Start the ADC
 while (bit_is_set(ADCSRA,ADSC));
 wADC = ADCW;
 t = (wADC - 324.31 ) / 1.22;
 return (t);
}
void timerIsr()
{
    strip.show();
    swap_ready = 1;
    //digitalWrite( 13, digitalRead( 13 ) ^ 1 );
}
