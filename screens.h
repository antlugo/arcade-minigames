// storage of screens
// the primary functions that actually do things
// screens are functions, and as such can lead into other screens (intro screen --> game hub --> minigame etc)


extern unsigned char lives; // initially set to 9, game ends when lives == 0
extern unsigned char last_lives; // track loss to know when to signify heartbreak in game hub
extern unsigned char difficulty; // used for difficulty scaling and level tracking
extern char timer[6]; // stores on-screen time (written every update() loop, first index set to null to turn off)

typedef struct
{
  void (*screen)(void); // the game function
  const char* text; // the game title
  const char* icon; // the game XBM sprite to show on transition screen
  unsigned char icon_width;
  unsigned char icon_height;
}
game;

// input digits of pi via remote
void pie_game();

// turn off the lights via buttons
void lights_game();

// do nothing, no inputs allowed (or lose)
void sleep_game();

// deliver direction via joystick
void deliver_game();

// what is the creator's last name? via remote or buttons
void name_game();

#define GAMES_COUNT 5 // games randomly selected from this array
game games[] = 
{
  {pie_game, GAME_PIE, PIE, PIE_WIDTH, PIE_HEIGHT},
  {lights_game, GAME_LIGHTS, BULB, BULB_WIDTH, BULB_HEIGHT},
  {sleep_game, GAME_SLEEP, SLEEP, SLEEP_WIDTH, SLEEP_HEIGHT},
  {deliver_game, GAME_DELIVER, GIFT, GIFT_WIDTH, GIFT_HEIGHT},
  {name_game, GAME_NAME, MAGE, MAGE_WIDTH, MAGE_HEIGHT},
};

// picks random minigame from games[]
// transition screen showing game name, icon, and level
// then starts that game by calling its function
void pick_game();

// make 2 timer objects, return first timer object index
// must run step_timer() after start_timer(), but before update() (and for each sequential update())
unsigned char start_timer(); 

// update position of timer objects and displayed seconds
// false return signifies time over, true return signifies time still counting down
bool step_timer(unsigned long start, unsigned long ms, unsigned char timer_index);                        

// simply sets first timer character to null character. timer objects cleared by clear_obj()
void clear_timer(); 

// show final end credits with score
void end_screen();

// input test screen
// shows inputs, debug info, and tests wire connections
void test_inputs();

// shows lives between minigames
// animates lives being lost
// updates 7-segment life display
void game_hub();

// display startup animation, and prompt menu
// can lead into either input_test or game_hub
void start_screen()
{
  // create initial title and credit
  unsigned char title_index = obj_count;
  add_obj(1, START_SCREEN_STARTUP_2, 0, 0, 13, 31);
  unsigned char credit_index = obj_count;
  add_obj(1, START_SCREEN_CREDITS, 0, 0, 17, 40);

  // begin startup animations
  unsigned char assortment_index = obj_count;
  // cape sprite flies to right, dragging in 2 prompts
  add_obj(0, CAPE, CAPE_WIDTH, CAPE_HEIGHT, -74, 0);

  for (int i = 0; i < 5; i++)
  {
    cycle_led();
    update();
  }
  for (int i = 0; i < 11; i++)
  {
    cycle_led();

    obj[title_index].y -= 2;
    obj[credit_index].y += 2;

    obj[assortment_index].x += 7;
    obj[assortment_index].y = 5 * sin(i / 1.2) + 17;

    update();
  }

  // create prompts and drag them behind cape sprite
  unsigned char prompt_index = obj_count;
  add_obj(1, START_SCREEN_PLAY_1, 0, 0, -126, 20);
  add_obj(1, START_SCREEN_TEST_1, 0, 0, -126, 40);

  for (int i = 0; i < 18; i++)
  {
    obj[assortment_index].x += 7; // Continue i from previous frame, with adjustments for lag
    obj[assortment_index].y = 5 * sin((i + 9) / 1.2 + 2) + 17;

    obj[prompt_index].x += 7;
    obj[prompt_index + 1].x += 7;

    cycle_led();

    update();
  }
  // remove (offscreen) cape sprite
  remove_obj(assortment_index);
  prompt_index--; // adjust pointer for obj removal
  // create and drag in prompt labels
  add_obj(1, START_SCREEN_PLAY_2, 0, 0,  139, 30);
  add_obj(1, START_SCREEN_TEST_2, 0, 0, 139, 50);

  for (int i = 0; i < 16; i++)
  {
    obj[prompt_index + 2].x -= 8;
    obj[prompt_index + 3].x -= 8;
    
    cycle_led();

    update();
  }
  clear_symbol();

  // end of startup animation
  // begin prompting user for input

  leds[0] = 1;
  leds[1] = 1;

  // wait until numbers 1 or 2 pressed via button or remote
  while (buttons[0] == 0 && buttons[1] == 0 && strcmp_P(remote, REMOTE_ONE_TEXT) && strcmp_P(remote, REMOTE_TWO_TEXT))
  {
    update();
  }

  if (buttons[0] || !strcmp_P(remote, REMOTE_ONE_TEXT)) // if 1 input, signify gamehub
  {
    // leave only "Play!" text on screen
    remove_obj(prompt_index);
    remove_obj(prompt_index);
    remove_obj(prompt_index + 1);
    remove_obj(credit_index);
    remove_obj(title_index);

    flash_option(1);

    clear_obj();

    // start the minigames
    game_hub();

  }
  else // if 2 input, signify input_test screen
  {
    // leave only "Test Inputs" text on screen
    remove_obj(prompt_index);
    remove_obj(prompt_index);
    remove_obj(prompt_index);
    remove_obj(credit_index);
    remove_obj(title_index);

    flash_option(2);

    // turn on all leds and segments to test for connected wires
    // unrolled loop uses slightly fewer bytes
    leds[0] = 1;
    leds[1] = 1;
    leds[2] = 1;
    leds[3] = 1;
    leds[4] = 1;
    display_symbol(8);
    clear_obj();
    add_obj(1, INPUT_TEST_1, 0, 0, 0, 60);
    test_inputs();
  }
}

// blank screen with debug options on, remote power button + button 5 to exit
// basically a copy of main update(); loop but with debug flag enabled
void test_inputs()
{
  while (true)
  {
    poll_joystick();
    poll_remote();
    poll_buttons();
    update_segments();
    update_shift_register();

    // delay to maintain target fps
    unsigned long elapsed = millis() - last_frame;

    if (elapsed < TARGET_FRAMETIME)
    {
      delay(TARGET_FRAMETIME - elapsed);
    }
    // update last frame
    last_frame = millis();
    
    // used to avoid slowdown from calling functions every paging loop
    unsigned int current_ram_free = ram_free();

    // update oled screen
    oled.firstPage();
    do
    {
      draw();
      oled.setCursor(0, 8); // frame time
      oled.print(elapsed);
      oled.print((__FlashStringHelper*)INPUT_TEST_MS);
      oled.setCursor(0, 40);
      oled.print((__FlashStringHelper*)INPUT_TEST_LOCK);
      oled.print(TARGET_FRAMETIME);
      oled.print((__FlashStringHelper*)INPUT_TEST_MS);
      oled.setCursor(0, 17); // ram free
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
      oled.setCursor(0, 49); // total time elapsed in ms
      oled.print(millis());
    }
    while (oled.nextPage());

    if (!strcmp_P(remote, REMOTE_POW_TEXT) && buttons[4])
    {
      obj[obj_count - 1].data = INPUT_TEST_2;
      obj[obj_count - 1].x = 10;
      obj[obj_count - 1].y = 35;
      for (int i = 0; i < 20; i++)
      {
        update();
      }
      clear_obj();
      clear_symbol();
      clear_leds();
      return;
    }
  }
}

// shows lives between minigames
// animates lives being lost
// updates 7-segment life display
// leads into pick_game(), which starts a random minigame
void game_hub()
{
  while (true)
  {
    difficulty++; // update difficulty
    // prepare and draw heart sprites
    unsigned char heart_index = obj_count;
    bool heartbreak = last_lives != lives;
                        // pi * 2
    float angular_offset = 6.283 / (lives + heartbreak);
    display_symbol(last_lives);
                        // extra heart created to symbolize heartbreak on loss
    for (int i = 0; i < (lives + heartbreak); i++)
    {
      add_obj(0, HEART, HEART_WIDTH, HEART_HEIGHT, 10, 10);
    }
    if (lives == 0) // for last heart, just display at center solemnly
    {
      obj[heart_index].x = 52;
      obj[heart_index].y = 24;
    }

    // animate hearts
    for (int i = 0; i < 20; i++)
    {
      for (int j = 0; j < lives; j++)
      {
        obj[heart_index + j].x = 50 * cos(0.3 * i + angular_offset * j) + 52;
        obj[heart_index + j].y = 20 * sin(0.3 * i + angular_offset * j) + 20;
        if (heartbreak)
        {
          obj[heart_index + lives].x = 50 * cos(0.3 * i + angular_offset * lives) + 52;
          obj[heart_index + lives].y = 20 * sin(0.3 * i + angular_offset * lives) + 20 ;
        }
      }
      
      update();
    }

    // signify loss of life
    if (heartbreak)
    {
      for (int i = 20; i < 40; i++)
      {
        for (int j = 0; j < lives; j++) // extra heart spirals towards center
        {
          obj[heart_index + j].x = 50 * cos(0.3 * i + angular_offset * j) + 52;
          obj[heart_index + j].y = 20 * sin(0.3 * i + angular_offset * j) + 20;
          obj[heart_index + lives].x = (20 - (i - 20)) * 1.67 * cos(0.3 * i + angular_offset * lives) + 52;
          obj[heart_index + lives].y = (20 - (i - 20)) * 0.67 * sin(0.3 * i + angular_offset * lives) + 20 ;
        }
        update();
      }
      obj[heart_index + lives].data = HEARTBREAK; // extra heart breaks
      for (int i = 40; i < 60; i++) // normal orbitting of remaining hearts while flashing lives counter
      {
        clear_symbol();
        for (int j = 0; j < lives; j++)
        {
          obj[heart_index + j].x = 50 * cos(0.3 * i + angular_offset * j) + 52;
          obj[heart_index + j].y = 20 * sin(0.3 * i + angular_offset * j) + 20;
        }
        update();
        display_symbol(last_lives);
        i++;
        for (int j = 0; j < lives; j++)
        {
          obj[heart_index + j].x = 50 * cos(0.3 * i + angular_offset * j) + 52;
          obj[heart_index + j].y = 20 * sin(0.3 * i + angular_offset * j) + 20;
        }
        update();
      }
      remove_obj(heart_index + lives); // remove broken heart
      display_symbol(lives); // new lives counter value
    }

    for (int i = 20 + heartbreak * 40; i < 45 + heartbreak * 40; i++) // continue animating hearts
    {
      for (int j = 0; j < lives; j++)
      {
        obj[heart_index + j].x = 50 * cos(0.3 * i + angular_offset * j) + 52;
        obj[heart_index + j].y = 20 * sin(0.3 * i + angular_offset * j) + 20;
      }
      update();
    }

    // update last_lives for next iteration
    last_lives = lives;

    // remove heart sprites, transition to next screen
    clear_obj();
    
    if (lives == 0)
    {
      // end game screen
      end_screen();
      return;
    }

    // call random game 
    pick_game();
  }
}

// make 2 timer objects, return first timer object index
// must run step_timer() after start_timer(), but before update() (and for each sequential update())
unsigned char start_timer()
{
  add_obj(0, TIMER1, TIMER_WIDTH, TIMER_HEIGHT, 0, 41);
  add_obj(0, BAR, BAR_WIDTH, BAR_HEIGHT, -1, 55);
  return obj_count - 2;
}

// update position of timer objects and displayed seconds
// false return signifies time over, true return signifies time still counting down
bool step_timer(unsigned long start, unsigned long ms, unsigned char timer_index)
{
  unsigned long current = millis();
  if (current > start + ms)
  {
    return false;
  }
  float percent = (current - start) / (float)ms;
  obj[timer_index].data = pgm_read_ptr(&TIMER_ANIM[(int)( percent * TIMER_COUNT)]); // cycle hourglass animation
  obj[timer_index + 1].x = -1 - (int)(percent * 141); // cycle timer bar
  unsigned long remaining_ms = (start + ms) - millis(); 
  // avoid sprintf, saving program memory
  timer[0] = '0' + remaining_ms / 10000;          // tens
  timer[1] = '0' + (remaining_ms % 10000) / 1000; // ones
  timer[2] = '.';                                 // decimal point
  timer[3] = '0' + (remaining_ms % 1000) / 100;   // tenths
  timer[4] = '0' + (remaining_ms % 100) / 10;     // hundreths
  return true;
}

// clears on-screen timer countdown
// timer objects cleared by clear_obj()
void clear_timer()
{
  timer[0] = '\0';
}

// show final end credits with score
void end_screen()
{
  add_obj(1, END_SCREEN_1, 0, 0, 20, 10);
  add_obj(1, END_SCREEN_2, 0, 0, 0, 20);
  add_obj(1, pgm_read_ptr(&NUM_TO_WORD[difficulty - 1]), 0, 0, 11, 30);
  add_obj(1, START_SCREEN_STARTUP_2, 0, 0, 0, 40);
  add_obj(1, START_SCREEN_CREDITS, 0, 0, 0, 50);
  add_obj(1, END_SCREEN_3, 0, 0, 0, 60);
  // bottom right sprite
  add_obj(0, PEACE, PEACE_WIDTH, PEACE_HEIGHT, 112, 40);
  for (int i = 0; i < 100; i++)
  {
    cycle_led();
    update();
  }
  clear_obj();
}

// picks random minigame from games[]
// transition screen showing game name, icon, and level
// then starts that game by calling its function
void pick_game() 
{
  unsigned char game_index = random(GAMES_COUNT);

  add_obj(1, INTERMISSION_LEVEL, 0, 0, 17, 8);
  add_obj(1, pgm_read_ptr(&NUM_TO_WORD[difficulty]), 0, 0, 52, 8);
  add_obj(1, INTERMISSION_INTRO, 0, 0, 0, 20);
  add_obj(1, games[game_index].text, 0, 0, 0, 33);

  add_obj(0, games[game_index].icon, games[game_index].icon_width, games[game_index].icon_height, 50, 40);

  for (int i = 0; i < 20; i++)
  {
    update();
  }
  clear_obj();

  games[game_index].screen();
}

// input digits of pi via remote
void pie_game()
{
  // timer setup
  unsigned char timer_index = start_timer();
  unsigned long start = millis();
  unsigned long time = (difficulty / 3) * 10000UL - difficulty * 3000UL + 10000UL; // UL to avoid logical errors from high numbers

  // digits to prompt for
  unsigned char digits = difficulty / 3 + 1;
  unsigned char completed_digits = 0;

  // initialize sprites
  unsigned char sprite_index = obj_count;
  unsigned char sprite_count = digits + 1; // +1 for dot

  
  add_obj(1, QUESTION_MARK, 0, 0, 60, 25); 
  add_obj(1, DOT, 0, 0, 67, 25);

  for (int i = 1; i < sprite_count - 1; i++)
  {
    add_obj(1, QUESTION_MARK, 0, 0, 67 + 6 * i, 25);
  }

  add_obj(1, CARET, 0, 0, 60, 35);
  unsigned char nerd_index = obj_count;
  add_obj(0, NERD, NERD_WIDTH, NERD_HEIGHT, 82, 29);

  update();

  // main game loop
  while (step_timer(start, time, timer_index) && completed_digits < digits) // while remaining time and remaining digits
  {

    for (int i = 0; i < 10; i++)
    {
      if (!strcmp_P(remote, pgm_read_ptr(&REMOTE_NUMBERS[i]))) // poll remote for number input
      {
        if (i == pgm_read_byte(&GAME_PIE_DIGITS[completed_digits])) // if input matches pi digit
        {
          for (int j = 0; j < sprite_count; j++)
          {
            obj[sprite_index + j].x -= 6 + (completed_digits == 0) * 7; // adjust position of digits, accounting for dot
          }
          obj[sprite_index + completed_digits + (completed_digits != 0)].data = pgm_read_ptr(&INT_TO_CHAR[pgm_read_byte(&GAME_PIE_DIGITS[completed_digits])]); // change question mark to revealed number, accounting for dot
          completed_digits++;
          obj[nerd_index].data = pgm_read_ptr(&NERD_STEP[completed_digits % 2]); // make nerd step for each digit input
          while (step_timer(start, time, timer_index) && strcmp_P(remote, REMOTE_BLANK_TEXT) && completed_digits < digits) // await button release if still more numbers to input
          {
            update();
          }
          break; // dont continue checking rest of numbers (0-10)
        }
        else // if input wrong, fail minigame
        {
          goto pi_failure; // fail if wrong digit input
        }
      }
    }
    update();
  }
  
  if (!step_timer(start, time, timer_index)) // if out of time
  {
    pi_failure:
    lives--;
  }

  clear_timer();
  clear_obj();
}

// turn off the lights via buttons
void lights_game()
{
  // timer setup
  unsigned char timer_index = start_timer();
  unsigned long start = millis();
  unsigned long time = 7000UL - difficulty * 70UL; // UL to avoid logical errors from high numbers

  // chance of each light being on (out of 100) (at least 1 is always on)
  unsigned char on_rng = difficulty * 2; 
  
  unsigned char sprite_index = obj_count;
  unsigned char sprites_created = 0;

  // randomly turn on at least 1 led
  for (int i = 0; i < 5; i++)
  {
    leds[i] = random(100) < on_rng;
  }
  leds[random(5)] = 1;

  bool button_last[5] = {0};
  
  // main game loop
  while (step_timer(start, time, timer_index) && (leds[0] || leds[1] || leds[2] || leds[3] || leds[4])) // exits when all lights off or end of timer
  {
    for (int i = 0; i < 5; i++) 
    {
      if (button_last[i] != buttons[i] && buttons[i] == 1) // poll buttons and update leds[]
      {
        leds[i] = !leds[i];
      }
      if (leds[i])
      {
        add_obj(0, BULB, BULB_WIDTH, BULB_HEIGHT, 6 + 25 * i, 17); // create on-screen representation for physical leds
        sprites_created++;
      }
      button_last[i] = buttons[i]; // update last button pressed
    }

    update(); // push leds[] and objects

    while (sprite_index != obj_count) // clear on-screen light bulbs, adding them again before the next update
    {
      remove_obj(sprite_index); 
    }
    sprites_created = 0;
  }

  if (!step_timer(start, time, timer_index)) // if out of time
  {
    lives--;
  }

  clear_leds();
  clear_timer();
  clear_obj();
}

// do nothing, no inputs allowed (or lose)
void sleep_game()
{
  // timer setup
  unsigned char timer_index = start_timer();
  unsigned long start = millis();
  unsigned long time = 15000UL - difficulty * 150UL; // UL to avoid logical errors from high numbers

  // initialize sprites
  unsigned char sprite_index = obj_count;
  add_obj(0, SLEEP, SLEEP_WIDTH, SLEEP_HEIGHT, 50, 40);
  add_obj(0, ZZZ, ZZZ_WIDTH, ZZZ_HEIGHT, 56, 20);

  // main game loop
  while (step_timer(start, time, timer_index))
  {
    if // check for any inputs at all
    (
      buttons[0] ||
      buttons[1] ||
      buttons[2] ||
      buttons[3] ||
      buttons[4] ||
      joystick.x > 600 ||
      joystick.x < 300 ||
      joystick.y > 600 ||
      joystick.y < 300 ||
      joystick.z ||
      strcmp_P(remote, REMOTE_BLANK_TEXT)
    )
    { // show angry dog on fail
      remove_obj(sprite_index);
      remove_obj(sprite_index);
      add_obj(0, DOG, DOG_WIDTH, DOG_HEIGHT, 51, 15); 

      if (millis() > start + time - 4000) // allow minimum 4 sec to show angry dog
      {
        time += 4000UL - ((start + time) - millis());
      }

      while(step_timer(start, time, timer_index)) // for until time runs out
      {
        obj[obj_count - 1].x = 51 + random(10); // shake the dog; he's angry
        obj[obj_count - 1].y = 15 + random(10);
        update();
      }
      lives--;
      break;
    }
    update();
  }

  clear_timer();
  clear_obj();
}

// what is the creator's last name? via remote or buttons
void deliver_game()
{
  // timer setup
  unsigned char timer_index = start_timer();
  unsigned long start = millis();
  unsigned long time = 8000UL - difficulty * 80UL; // UL to avoid logical errors from high numbers

  unsigned char correct_direction = random(4);

  // initialize sprites
  add_obj(1, pgm_read_ptr(&GAME_DELIVER_DIRECTIONS[correct_direction]), 0, 0, 10, 20);
  add_obj(0, GIFT, GIFT_WIDTH, GIFT_HEIGHT, 55, 30);

  // N=0, E=1, S=2, W=3
  unsigned char direction;

  // poll for direction
  while (step_timer(start, time, timer_index))
  {
    if (joystick.x > 900) // east
    {
      direction = 1;
      break;
    }
    else if (joystick.x < 100) // west
    {
      direction = 3;
      break;
    }
    else if (joystick.y > 900) // north
    {
      direction = 0;
      break;
    }
    else if (joystick.y < 100) // south
    {
      direction = 2;
      break;
    }
    update();
  }

  if (!step_timer(start, time, timer_index))
  {
    goto delivery_early_end;
  }

  // feedback for input
  for (int i = 0; i < 27; i++) // animate giftbox going off screen in specified direction
  {
    switch (direction)
    {
      case 0: // north
        obj[obj_count - 1].y -= 3;
        break;
        i++;
      case 1: // east
        obj[obj_count - 1].x += 3;
        break;
      case 2: // south
        obj[obj_count - 1].y += 3;
        i++; 
        break;
      case 3: // west
        obj[obj_count - 1].x -= 3;
        break;
    }
    update();
  }

  // check for fail condition
  if (direction != correct_direction)
  {
    delivery_early_end:
    lives--;
  }

  clear_timer();
  clear_obj();
}

// what is the creator's last name? via remote or buttons
void name_game()
{
  // timer setup
  unsigned char timer_index = start_timer();
  unsigned long start = millis();
  unsigned long time = 9000UL - difficulty * 90UL; // UL to avoid logical errors from high numbers
  // 4 choices at every difficulty as to not make it too easy (early last name reveal)
  unsigned char correct_index = random(4); 

  // track on-screen guess prompts as to not repeat them
  // 255 can not be generated from random(DOPPELGANGER_COUNT)
  unsigned char guess = 255;
  unsigned char doppelgangers[4];
  memset(doppelgangers, 255, 4);

  // intialize sprites
  unsigned char sprite_index = obj_count;

  for (int i = 0; i < 4; i++) // create on-screen numbered names list and light leds
  {
    add_obj(1, pgm_read_ptr(&INT_TO_CHAR[i + 1]), 0, 0, 0, 9 + 10 * i);
    add_obj(1, DOT, 0, 0, 7, 7 + 10 * i);
    leds[i] = 1;
    if (i == correct_index)
    {
      add_obj(1, pgm_read_ptr(&GAME_NAME_CORRECT), 0, 0, 20, 9 + 10 * i);
    }
    else
    {
      // populate incorrect answers, non-repeating
      unsigned char rng;
      do
      {
        rng = random(DOPPELGANGER_COUNT);
      }
      while 
      (memchr(doppelgangers, rng, 4));

      add_obj(1, pgm_read_ptr(&GAME_NAME_DOPPELGANGER[rng]), 0, 0, 20, 9 + 10 * i);
      doppelgangers[i] = rng;
    }
  }

  // poll for input
  while (step_timer(start, time, timer_index))
  {

    for (int i = 0; i < 4; i++)
    {
      if (buttons[i] || !strcmp_P(remote, pgm_read_ptr(&REMOTE_NUMBERS[i + 1])))
      {
        guess = i + 1;
        if (i == correct_index)
        {
          goto correct_last_name;
        }
        goto wrong_last_name;
      }
    }
    update();
  }


  if (!step_timer(start, time, timer_index))
  {
    wrong_last_name:
    lives--;
  }

  correct_last_name:

  // skip choice indication routine if no guess made
  if (guess == 255)
  {
    goto no_guess;
  }

  // only leave guess on screen

  for (int i = 0; i < (guess - 1) * 3; i++) // remove leading guesses
  {
    remove_obj(sprite_index);
  }

  for (int i = 0; i < (4 - guess) * 3; i++) // remove trailing guesses
  {
    remove_obj(sprite_index + 3);
  }

  // indicate option chosen
  flash_option(guess);

  no_guess:

  display_symbol(lives); // reset 7 segment display

  clear_leds();

  clear_timer();
  clear_obj();
}

