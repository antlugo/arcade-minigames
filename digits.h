// 7-segment display symbols and functions to interface with 7-segment display

// bitfields correspond to 7-segment display
//  segments: 0bGFEDCBA
#define ZERO  0b0111111 
#define ONE   0b0000110 
#define TWO   0b1011011 
#define THREE 0b1001111 
#define FOUR  0b1100110 
#define FIVE  0b1101101 
#define SIX   0b1111101
#define SEVEN 0b0000111 
#define EIGHT 0b1111111 
#define NINE  0b1101111

// index directly corresponds to symbol for numbers 0-9 
const unsigned char SYMBOLS[10] PROGMEM = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

// from hardware.h
extern bool segments[7];

// take a symbol of some index from SYMBOLS[] and update segments[] (7-segment display array) to reflect it
// actual hardware updated in update() function in hardware.h
void display_symbol(unsigned char id)
{
  unsigned char symbol = pgm_read_byte(&SYMBOLS[id]);
  for (unsigned char i = 0; i < SEGMENTS_COUNT; i++)
  {
    segments[i] = (symbol >> i) & 1;
  }
}

void clear_symbol()
{
  memset(segments, 0, SEGMENTS_COUNT);
}

unsigned char led_cycle = 0;
void cycle_led() // cycle edges of 7-segment display
{
  clear_symbol();
  segments[led_cycle] = 1;
  led_cycle += 1;
  led_cycle %= 6;
}

// indicates option 1-5 taken on LEDs and 7 segment display
// ends with segment display and all leds cleared
// only use with options 1-5
void flash_option(unsigned char option)
{
  // turn off all leds besides option led
  clear_leds(); 
  leds[option - 1] = 1;
  for (int i = 0; i < 3; i++) // flash segment option and led option
    {
      for (int j = 0; j < 4; j++)
      {
        leds[option - 1] = 1;
        display_symbol(option);
        update();
      }
      for (int j = 0; j < 4; j++)
      {
        leds[option - 1] = 0;
        clear_symbol();
        update();
      }
    }
    for (int j = 0; j < 4; j++)
    {
      leds[option - 1] = 1;
      display_symbol(option);
      update();
    }
  // clear segments and remaining led
  clear_symbol(); 
  leds[option - 1] = 0;
}
