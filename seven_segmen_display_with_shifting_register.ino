
#include <avr/io.h>
#include <util/delay.h>

//buttons
#define T1 A1
#define T2 A2
#define T3 A3

int transferPin = 8; // STCP -transferPin
int clockPin = 12; // SHCP -clockPin
int dataPin = 11;

int segmentArray[10] = {
  0b11111100,  //0
  0b01100000,  //1
  0b11011010,  //2
  0b11110010,  //3
  0b01100110,  //4
  0b10110110,  //5
  0b10111110,  //6
  0b11100000,  //7
  0b11111110,  //8
  0b11110110   //9
};

int readInputButtons();
void lightening(uint8_t number);
void schiebeByte(int segment_data);



int main() {

  // Buttons as INPUT
  DDRC &= ~(1 << DDC1);
  DDRC &= ~(1 << DDC2);
  DDRC &= ~(1 << DDC3);

  DDRB |= (1 << DDB0); //transferPin
  DDRB |= (1 << DDB3); // dataPin
  DDRB |= (1 << DDB4); // clockPin


  while (1) {
    lightening(readInputButtons());

  }
}



// conversion of binary inputs of the buttons into decimal values
int readInputButtons() {

  return   myDigitalRead(PINC1) * 4  + myDigitalRead(PINC2) * 2 + myDigitalRead(PINC3)*1;

}


//digitalRead is not giving 0 or 1 as an output 
// therefore bit shifting is required
int myDigitalRead(int pinNumber) {
  return (PINC & (1 << pinNumber)) >> pinNumber ; 
}


void lightening(int number)
{

  PORTB &= ~ (1 << PORTB0); //ground transferPin LOW as long as data is transfered 
  //arduino function
  //shiftOut(dataPin, clockPin, MSBFIRST, segmentArray[number]);

  // own written shiftOut version
  schiebeByte(segmentArray[number]);

  PORTB |= (1 << PORTB0); //transferPin on HIGH for data saving -> transfer shift register in storage Register

}



void schiebeByte(int segment_data)
{
  for (int i = 7; i >= 0; i--)  { 
    PORTB &= ~(1 << PORTB4); // clockPin

    if ( segment_data & (1 << i) ) {
      PORTB |= (1 << PORTB3); // dataPin
    }
    else {
      PORTB &= ~(1 << PORTB3); // dataPin
    }

    PORTB |= (1 << PORTB4);// clockPin // rising edge with which bits are pushed in 

}















