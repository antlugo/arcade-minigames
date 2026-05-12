// arcade minigames by Anthony Lugo <alugo4@alaska.edu>

// a game that is vaguely inspired by warioware

// main file, sets up hardware, variables, software, and starts the game from start_screen()

#pragma GCC optimize("Os") // Manual compiler optimization declaration saves ~40 bytes of progmem

#include <Arduino.h> // General Library
#include "sprites.h" // storage of xbm sprites
#include "text.h" // storage of text
#include "hardware.h" // code to interface with hardware, debug options, and update() loop
#include "digits.h" // code to more easily interface with 7-segment display (numbers, animations, etc)
#include "objects.h" // object struct and management of on-screen objects (text and sprites)
#include "screens.h" // various screens of the game, including intro menu, input test, transition screens, minigames, etc

// variables from hardware.h
joystick_struct joystick = {0};
bool buttons[5] = {0};
char remote[6] = "";
bool segments[7] = {0};
bool leds[5] = {0};
unsigned long last_frame = 0;
char timer[6] = {0};
// variables from objects.h
object obj[OBJECT_MAX] = {0};
int obj_count = 0;
// variables from screens.h
unsigned char lives = 9; // game over at zero
unsigned char last_lives = 9; // track to know when to signify life loss in game hub
unsigned char difficulty = 0; // used for difficulty scaling and level tracking



void setup() {
  // initialize oled screen
  oled.setBusClock(800000); // optimization saves ~14 ms @ 800 KHz
  oled.begin();
  oled.setFont(u8g2_font_ncenB08_tr); // t: variable width r: reduced character set (as in only common ascii characters)
  clear_oled();
  oled.setBitmapMode(1); // let sprites overlap
  // apply debug flag - onscreen frametime and ram labels
  #ifdef DEBUG
    add_obj(1, FRAME_TIME_TEXT, 0, 0, 0, 8);
    add_obj(1, RAM_FREE_TEXT, 0, 0, 0, 17);
  #endif
  // initialize IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN);
  
  // set up pins
  // setup via hardware registers to save progmem
  // saves ~80 bytes of progmem

  DDRB = 0b00101100;
  /* above register write equivalent to
  0:
  0:
  1: pinMode(RCLOCK, OUTPUT);
  0: pinMode(pgm_read_byte(&BUTTONS[4]), INPUT_PULLUP); (no pullup yet)
  1: pinMode(SER, OUTPUT);
  1: pinMode(SCLOCK, OUTPUT);
  0: pinMode(pgm_read_byte(&BUTTONS[1]), INPUT_PULLUP); (no pullup yet)
  0: pinMode(pgm_read_byte(&BUTTONS[0]), INPUT_PULLUP); (no pullup yet)
  */

  DDRC = 0b00110000;
  /* above register write equivalent to
  0:
  0:
  1: oled SCL i2c (already done by oled.begin())
  1: oled SDA i2c (already done by oled.begin())
  0: pinMode(JOYY, INPUT);
  0: pinMode(JOYX, INPUT);
  0: pinMode(pgm_read_byte(&BUTTONS[2]), INPUT_PULLUP); (no pullup yet)
  0: pinMode(pgm_read_byte(&BUTTONS[3]), INPUT_PULLUP); (no pullup yet)
  */

  DDRD = 0b11110000;
  /* above register write equivalent to
  1: pinMode(SEGB, OUTPUT);
  1: pinMode(SEGA, OUTPUT);
  1: pinMode(SEGF, OUTPUT);
  1: pinMode(SEGG, OUTPUT);
  0: pinmode(JOYZ, INPUT); (no pullup yet)
  0: IR recieve pin (already done by IrReceiver.begin(IR_RECEIVE_PIN);)
  0: tx (unused)
  0: rx (unused)
  */

  // enable pullup for pins that need it
  PORTB |= 0b00010011;
  PORTC |= 0b00000011;
  PORTD |= 0b00001000;

  // initialize rng seed (assumes button 3 (green LED) not pressed on boot)
  randomSeed(analogRead(SEED_ANALOG_UNCONNECTED));
}

// setup game variables that may have changed and begin by calling start_screen();
void loop() {
  oled.setBitmapMode(1);
  clear_obj();
  clear_symbol();
  clear_timer();
  clear_leds();
  timer[0] = '\0';
  lives = 9;
  last_lives = 9;
  difficulty = 0;
  start_screen();
}
