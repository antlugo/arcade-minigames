// hardware interfacing file (inputs and outputs)

/*
  list of peripherals/hardware:
    5 buttons
    5 led diodes
    1 OLED display
    1 joystick
    1 TL1838 IR reciever with Elegoo IR emitter remote
    1 single digit 7-segment display (dot not wired)
    1 SNx4HC595 shift register (exclusively used for output)
*/


// debug option to show:
// frametime, excluding delay for locked frame rate
// memory usage each frame
// display all inputs (joystick remote and buttons)
// reduces frame rate by being turned on
// uses significant program memory
// #define DEBUG 1

// use to show IR remote reliability
// not relevant to circuits course, used for a different lab for my writing 213X course
// displays percentage of remote inputs successfully recieved to OLED screen
// data reset when pressing power button on IR, then freezes for 1000 ms and until another remote button is pressed in order to give time to get input ready
// #define IR_RELIABILITY 1

// used by update() to automatically delay ms until next frame (lock FPS)
// formula hertz = 1 / period to get FPS
// frametime in ms
// remote input seems most stable at ~130 ms frametime
// Can go above this frametime quite often
#define TARGET_FRAMETIME 130

// for TL1838 IR receiver
#include <IRremote.h>
#define DECODE_NEC
#define IR_RECEIVE_PIN 2

// for OLED display
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_2_HW_I2C oled{U8G2_R0};

// access draw() from objects.h for update()
extern void draw();

// pin to peripheral definitions

// joystick input
#define JOYX A2
#define JOYY A3
#define JOYZ 3

// shift register outputs
#define SER 11
#define SCLOCK 10
#define RCLOCK 13

// button inputs
#define BUTTONS_COUNT 5

// unused in final, optimized version to save program memory
// const static unsigned char BUTTONS[5] PROGMEM = {8, 9, A1, A0, 12};

extern char timer[6]; // stores on-screen time, written every update() loop

// analog pin, open circuit on boot
// used for seeding RNG
#define SEED_ANALOG_UNCONNECTED A1

// all leds go through the shift register
#define LEDS_COUNT 5

// 1 digit 7 segment display
// EDC segments belong to shift register, rest of segments have pins that are direct to the arduino board
// all segments: ABCDEFG
   #define SEGA 6
   #define SEGB 7
// #define SEGC (handled by shift register)
// #define SEGD (handled by shift register)
// #define SEGE (handled by shift register)
   #define SEGF 5
   #define SEGG 4

   #define SEGMENTS_COUNT 7

// internal arrays: how the hardware is exposed

// polling arrays
// polling done every frame by update()

// internal button inputs
extern bool buttons[5];

// internal 7-segment display array
extern bool segments[7];

// internal LEDS
extern bool leds[5];

// internal IR decode
extern char remote[6];

// joystick, coordinates and button
struct joystick_struct 
{
    unsigned int x;
    unsigned int y;
    bool z;
};
extern joystick_struct joystick;

// delta time, frame time tracking to maintain target fps
extern unsigned long last_frame;

// used for testing IR remote reliability, not relevant to circuits course
#ifdef IR_RELIABILITY
  unsigned long reliability_frames_passed = 0;
  unsigned long reliability_frames_pressed = 0;
  char reliability_buffer[6] = "";
#endif

// hardware functions, including update()

// update hardware shift register to reflect internal pins (LEDS and EDC digit segments)
void update_shift_register()
{
  // get physical real-world representation ready and send
  unsigned char physical = (leds[0]<<7)     | // shift register 7, QH
                           (leds[1]<<6)     | // shift register 6, QG
                           (leds[2]<<5)     | // shift register 5, QF
                           (leds[3]<<4)     | // shift register 4, QE
                           (leds[4]<<3)     | // shift register 3, QD
                           (segments[4]<<2) | // shift register 2, QC
                           (segments[3]<<1) | // shift register 1, QB
                            segments[2];      // shift register 0, QA

  // toggle rclock to update shift register outputs all at once
  PORTB &= ~(1<<5);          // pin 13, equivalent to digitalWrite(RCLOCK, 0);
  shiftOut(SER, SCLOCK, MSBFIRST, physical);
  PORTB |=  (1<<5);          // pin 13, equivalent to digitalWrite(RCLOCK, 1); 
}

void poll_joystick()
{
  joystick.x = analogRead(JOYX);
  joystick.y = analogRead(JOYY);

  joystick.z = !(PIND & (1<<3)); // pin 3
  /* register read equivalent to
  joystick.z = !digitalRead(JOYZ); // inwards joystick button
  */
}

void poll_buttons()
{

  buttons[0] = !(PINB & (1<<0)); // pin 8
  buttons[1] = !(PINB & (1<<1)); // pin 9
  buttons[2] = !(PINC & (1<<1)); // A1
  buttons[3] = !(PINC & (1<<0)); // A0
  buttons[4] = !(PINB & (1<<4)); // pin 12

  /*
  register reads equivalent to
  for (unsigned char i = 0; i < BUTTONS_COUNT; i++)
  {             
    buttons[i] = !digitalRead(pgm_read_byte(&BUTTONS[i])); // inverted to account for INPUT_PULLUP behavior
  }
  */ 
  
}

void poll_remote()
{
  if (IrReceiver.decode())
  {
    switch (IrReceiver.decodedIRData.command) // remote hex signal --> string
    {
      case 0x45:
        strcpy_P(remote, REMOTE_POW_TEXT);
        break;
      case 0x46:
        strcpy_P(remote, REMOTE_VOLPLUS_TEXT);
        break;
      case 0x47:
        strcpy_P(remote, REMOTE_FUNCSTOP_TEXT);
        break;
      case 0x44:
        strcpy_P(remote, REMOTE_REWIND_TEXT);
        break;
      case 0x40:
        strcpy_P(remote, REMOTE_PLAYPAUSE_TEXT);
        break;
      case 0x43:
        strcpy_P(remote, REMOTE_FASTFORWARD_TEXT);
        break;
      case 0x07:
        strcpy_P(remote, REMOTE_DOWN_TEXT);
        break;
      case 0x15:
        strcpy_P(remote, REMOTE_VOLMINUS_TEXT);
        break;
      case 0x09:
        strcpy_P(remote, REMOTE_UP_TEXT);
        break;
      case 0x16:
        strcpy_P(remote, REMOTE_ZERO_TEXT);
        break;
      case 0x19:
        strcpy_P(remote, REMOTE_EQ_TEXT);
        break;
      case 0x0D:
        strcpy_P(remote, REMOTE_STREPT_TEXT);
        break;
      case 0x0C:
        strcpy_P(remote, REMOTE_ONE_TEXT);
        break;
      case 0x18:
        strcpy_P(remote, REMOTE_TWO_TEXT);
        break;
      case 0x5E:
        strcpy_P(remote, REMOTE_THREE_TEXT);
        break;
      case 0x08:
        strcpy_P(remote, REMOTE_FOUR_TEXT);
        break;
      case 0x1C:
        strcpy_P(remote, REMOTE_FIVE_TEXT);
        break;
      case 0x5A:
        strcpy_P(remote, REMOTE_SIX_TEXT);
        break;
      case 0x42:
        strcpy_P(remote, REMOTE_SEVEN_TEXT);
        break;
      case 0x52:
        strcpy_P(remote, REMOTE_EIGHT_TEXT);
        break;
      case 0x4A:
        strcpy_P(remote, REMOTE_NINE_TEXT);
        break;
    }
  }
  else // no (successful) decode --> remote is blank
  {
    strcpy_P(remote, REMOTE_BLANK_TEXT);
  }
  IrReceiver.resume();
}

// segments EDC handled by update_shift_register()
// segments ABFG handled here by update_segments()
void update_segments()
{

  PORTD = (PORTD & 0b00001111) |
          (segments[0] << 6)   |  // pin 6
          (segments[1] << 7)   |  // pin 7
          (segments[5] << 5)   |  // pin 5
          (segments[6] << 4);     // pin 4
    
  /*register writes equivalent to
  digitalWrite(SEGA, segments[0]);
  digitalWrite(SEGB, segments[1]);
  digitalWrite(SEGF, segments[5]);
  digitalWrite(SEGG, segments[6]);
  */
}

void clear_leds()
{
  memset(leds, 0, 5);
}

// clear screen by drawing empty frame
void clear_oled()
{
  oled.firstPage();
  do {} 
  while (oled.nextPage());  
}

// used in input test and for debug flag to track ram free
extern char *__brkval;
extern char *__heap_start;
unsigned int ram_free() 
{
  char c;
  return &c - (__brkval == 0 ? (char*)&__heap_start : __brkval);
}



// main update function
// polls inputs and updates outputs
// used to advance frame, fps lock/minimum frame time determined by TARGET_FRAMETIME
void update()
{

  #ifdef IR_RELIABILITY
    if (strcmp_P(remote, REMOTE_POW_TEXT) == 0) // if pow button, reset data, wait 1 sec, and wait til any remote button pressed
      {
        reliability_frames_pressed = 0;
        reliability_frames_passed = 0;
        delay(1000);
        while (!IrReceiver.decode()) // wait until any remote input to resume
        {
          delay(1);
        }
      }
  #endif

  // poll and update non-oled hardware
  poll_joystick();
  poll_remote();
  poll_buttons();
  update_segments();
  update_shift_register();

  // record data
  #ifdef IR_RELIABILITY
    reliability_frames_pressed += strcmp_P(remote, REMOTE_BLANK_TEXT) != 0;
    reliability_frames_passed += 1;
  #endif

  // delay to maintain target frametime
  unsigned long elapsed = millis() - last_frame;
  if (elapsed < TARGET_FRAMETIME)
  {
    delay(TARGET_FRAMETIME - elapsed);
  }
  // record when this frame took place to lock frametime
  last_frame = millis();

  // used to avoid slowdown from calling functions every paging loop
  #ifdef DEBUG
    unsigned int current_ram_free = ram_free();
  #endif
  #ifdef IR_RELIABILITY
    char reliability_buffer[7]; // buffer written each frame (if reliability flag on)
  #endif
  // update oled screen
  oled.firstPage();
  do
  {
    draw();
    oled.setCursor(100, 53);
    oled.print(timer);
    #ifdef DEBUG
      oled.setCursor(63, 8); // frame time
      oled.print(elapsed);
      oled.print((__FlashStringHelper*)INPUT_TEST_MS);
      oled.setCursor(50, 17); // ram free
      oled.print(current_ram_free);
      oled.print((__FlashStringHelper*)INPUT_TEST_BYTES);
      oled.setCursor(0, 26); // IR remote input
      oled.print(remote);
      oled.setCursor(38, 26); // button input
      oled.print(buttons[0]);
      oled.print(buttons[1]);
      oled.print(buttons[2]);
      oled.print(buttons[3]);
      oled.print(buttons[4]);
      oled.setCursor(70, 26); // joystick input
      oled.print(joystick.x);
      oled.setCursor(90, 26);
      oled.print(joystick.y);
      oled.setCursor(110, 26);
      oled.print(joystick.z);
    #endif
    #ifdef IR_RELIABILITY
      oled.setCursor(35, 26); // IR proportion
      dtostrf((float)reliability_frames_pressed / reliability_frames_passed, 6, 3, reliability_buffer);
      oled.print(reliability_buffer);
    #endif

  }
  while (oled.nextPage());
}