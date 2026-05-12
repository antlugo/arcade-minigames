// storage of text (in program memory)

// debug
const static unsigned char FRAME_TIME_TEXT[] PROGMEM = "frame time: ";
const static unsigned char RAM_FREE_TEXT[] PROGMEM = "ram free: ";

// start screen
const static unsigned char START_SCREEN_STARTUP_1[] PROGMEM = "Starting...";
const static unsigned char START_SCREEN_STARTUP_2[] PROGMEM = "Arcade Minigames";
const static unsigned char START_SCREEN_CREDITS[] PROGMEM = "By: Anthony Lugo";
const static unsigned char START_SCREEN_PLAY_1[] PROGMEM = "Button or Remote 1 to:";
const static unsigned char START_SCREEN_PLAY_2[] PROGMEM = "Play! (fun)";
const static unsigned char START_SCREEN_TEST_1[] PROGMEM = "Button or Remote 2 to:";
const static unsigned char START_SCREEN_TEST_2[] PROGMEM = "Test Inputs (boring)";

// input test screen
const static unsigned char INPUT_TEST_1[] PROGMEM = "Power+Button 5 to Exit";
const static unsigned char INPUT_TEST_2[] PROGMEM = "Input Test Finished";
const static unsigned char INPUT_TEST_MS[] PROGMEM = " ms";
const static unsigned char INPUT_TEST_BYTES[] PROGMEM = " bytes";
const static unsigned char INPUT_TEST_LOCK[] PROGMEM = "frametime lock: ";

// end screen
const static unsigned char END_SCREEN_1[] PROGMEM = "Congratulations!";
const static unsigned char END_SCREEN_2[] PROGMEM = "Levels Beaten:";
const static unsigned char END_SCREEN_3[] PROGMEM = "Thanks For Playing!";

// used for ir remote
const static unsigned char REMOTE_POW_TEXT[] PROGMEM = "power";
const static unsigned char REMOTE_VOLPLUS_TEXT[] PROGMEM = "vol+";
const static unsigned char REMOTE_FUNCSTOP_TEXT[] PROGMEM = "func";
const static unsigned char REMOTE_REWIND_TEXT[] PROGMEM = "rwind";
const static unsigned char REMOTE_PLAYPAUSE_TEXT[] PROGMEM = "pause";
const static unsigned char REMOTE_FASTFORWARD_TEXT[] PROGMEM = "ffwd";
const static unsigned char REMOTE_DOWN_TEXT[] PROGMEM = "down";
const static unsigned char REMOTE_VOLMINUS_TEXT[] PROGMEM = "vol-";
const static unsigned char REMOTE_UP_TEXT[] PROGMEM = "up";
const static unsigned char REMOTE_ZERO_TEXT[] PROGMEM = "zero";
const static unsigned char REMOTE_EQ_TEXT[] PROGMEM = "eq";
const static unsigned char REMOTE_STREPT_TEXT[] PROGMEM = "rept";
const static unsigned char REMOTE_ONE_TEXT[] PROGMEM = "one";
const static unsigned char REMOTE_TWO_TEXT[] PROGMEM = "two";
const static unsigned char REMOTE_THREE_TEXT[] PROGMEM = "three";
const static unsigned char REMOTE_FOUR_TEXT[] PROGMEM = "four";
const static unsigned char REMOTE_FIVE_TEXT[] PROGMEM = "five";
const static unsigned char REMOTE_SIX_TEXT[] PROGMEM = "six";
const static unsigned char REMOTE_SEVEN_TEXT[] PROGMEM = "seven";
const static unsigned char REMOTE_EIGHT_TEXT[] PROGMEM = "eight";
const static unsigned char REMOTE_NINE_TEXT[] PROGMEM = "nine";
const static unsigned char REMOTE_BLANK_TEXT[] PROGMEM = "";

const static unsigned char* const REMOTE_NUMBERS[] PROGMEM = {
  REMOTE_ZERO_TEXT, REMOTE_ONE_TEXT, REMOTE_TWO_TEXT, REMOTE_THREE_TEXT,
  REMOTE_FOUR_TEXT, REMOTE_FIVE_TEXT, REMOTE_SIX_TEXT, REMOTE_SEVEN_TEXT,
  REMOTE_EIGHT_TEXT, REMOTE_NINE_TEXT
};

// numbers
const static unsigned char ZERO[] PROGMEM = "0";
const static unsigned char ONE[] PROGMEM = "1";
const static unsigned char TWO[] PROGMEM = "2";
const static unsigned char THREE[] PROGMEM = "3";
const static unsigned char FOUR[] PROGMEM = "4";
const static unsigned char FIVE[] PROGMEM = "5";
const static unsigned char SIX[] PROGMEM = "6";
const static unsigned char SEVEN[] PROGMEM = "7";
const static unsigned char EIGHT[] PROGMEM = "8";
const static unsigned char NINE[] PROGMEM = "9";

const static unsigned char* const INT_TO_CHAR[] PROGMEM = {
  ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE
};



// transition screen
const static unsigned char INTERMISSION_LEVEL[] PROGMEM = "Level ";
const static unsigned char INTERMISSION_INTRO[] PROGMEM = "Minigame: ";


// games
const static unsigned char GAME_PIE[] PROGMEM = "Input Digits of Pi";
const static unsigned char GAME_PIE_DIGITS[] PROGMEM = {
  3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4,
  6, 2, 6, 4, 3, 3, 8, 3, 2, 7, 9, 5, 0, 2, 8, 8, 4, 1, 9, 7,
  1, 6, 9, 3, 9, 9, 3, 7, 5, 1, 0, 5, 8, 2, 0, 9, 7, 4, 9, 4,
  4, 5, 9, 2, 3, 0, 7, 8, 1, 6, 4, 0, 6, 2, 8, 6, 2, 0, 8, 9,
  9, 8, 6, 2, 8, 0, 3, 4, 8, 2, 5, 3, 4, 2, 1, 1, 7, 0, 6, 7,
  9
};

const static unsigned char GAME_LIGHTS[] PROGMEM = "Turn Off The Lights!";

const static unsigned char GAME_SLEEP[] PROGMEM = "Don't Disturb The Dog";

const static unsigned char GAME_DELIVER[] PROGMEM = "Deliver The Gift...";
const static unsigned char GAME_DELIVER_NORTH[] PROGMEM = "North!";
const static unsigned char GAME_DELIVER_EAST[] PROGMEM = "East!";
const static unsigned char GAME_DELIVER_SOUTH[] PROGMEM = "South!";
const static unsigned char GAME_DELIVER_WEST[] PROGMEM = "West!";
const static unsigned char* const GAME_DELIVER_DIRECTIONS[] PROGMEM = {
  GAME_DELIVER_NORTH, GAME_DELIVER_EAST, GAME_DELIVER_SOUTH,
  GAME_DELIVER_WEST
};

#define DOPPELGANGER_COUNT 14
const static unsigned char GAME_NAME[] PROGMEM = "Creator's Last Name?";
const static unsigned char* const GAME_NAME_CORRECT PROGMEM = START_SCREEN_CREDITS + 12;  // same as "Lugo", saves some bytes by referencing latter part of previously declared string
const static unsigned char GAME_NAME_DOPPELGANGER1[] PROGMEM = "Lego";
const static unsigned char GAME_NAME_DOPPELGANGER2[] PROGMEM = "Hugo";
const static unsigned char GAME_NAME_DOPPELGANGER3[] PROGMEM = "Hego";
const static unsigned char GAME_NAME_DOPPELGANGER4[] PROGMEM = "Arugo";
const static unsigned char GAME_NAME_DOPPELGANGER5[] PROGMEM = "Walego";
const static unsigned char GAME_NAME_DOPPELGANGER6[] PROGMEM = "Huckleberry";
const static unsigned char GAME_NAME_DOPPELGANGER7[] PROGMEM = "Kamugo";
const static unsigned char GAME_NAME_DOPPELGANGER8[] PROGMEM = "Dugo";
const static unsigned char GAME_NAME_DOPPELGANGER9[] PROGMEM = "Yugo";
const static unsigned char GAME_NAME_DOPPELGANGER10[] PROGMEM = "Yego";
const static unsigned char GAME_NAME_DOPPELGANGER11[] PROGMEM = "Santiago";
const static unsigned char GAME_NAME_DOPPELGANGER12[] PROGMEM = "Ngo";
const static unsigned char GAME_NAME_DOPPELGANGER13[] PROGMEM = "Rodriguez";
const static unsigned char GAME_NAME_DOPPELGANGER14[] PROGMEM = "Dego";
const static unsigned char* const GAME_NAME_DOPPELGANGER[] PROGMEM = {
  GAME_NAME_DOPPELGANGER1, GAME_NAME_DOPPELGANGER2, GAME_NAME_DOPPELGANGER3,
  GAME_NAME_DOPPELGANGER4, GAME_NAME_DOPPELGANGER5, GAME_NAME_DOPPELGANGER6,
  GAME_NAME_DOPPELGANGER7, GAME_NAME_DOPPELGANGER8, GAME_NAME_DOPPELGANGER9,
  GAME_NAME_DOPPELGANGER10, GAME_NAME_DOPPELGANGER11, GAME_NAME_DOPPELGANGER12,
  GAME_NAME_DOPPELGANGER13, GAME_NAME_DOPPELGANGER14
};

// NUM_TO_WORD[], used for displaying number on screen as word
const static unsigned char NW0[] PROGMEM = "Zero";
const static unsigned char NW1[] PROGMEM = "One";
const static unsigned char NW2[] PROGMEM = "Two";
const static unsigned char NW3[] PROGMEM = "Three";
const static unsigned char NW4[] PROGMEM = "Four";
const static unsigned char NW5[] PROGMEM = "Five";
const static unsigned char NW6[] PROGMEM = "Six";
const static unsigned char NW7[] PROGMEM = "Seven";
const static unsigned char NW8[] PROGMEM = "Eight";
const static unsigned char NW9[] PROGMEM = "Nine";
const static unsigned char NW10[] PROGMEM = "Ten";
const static unsigned char NW11[] PROGMEM = "Eleven";
const static unsigned char NW12[] PROGMEM = "Twelve";
const static unsigned char NW13[] PROGMEM = "Thirteen";
const static unsigned char NW14[] PROGMEM = "Fourteen";
const static unsigned char NW15[] PROGMEM = "Fifteen";
const static unsigned char NW16[] PROGMEM = "Sixteen";
const static unsigned char NW17[] PROGMEM = "Seventeen";
const static unsigned char NW18[] PROGMEM = "Eighteen";
const static unsigned char NW19[] PROGMEM = "Nineteen";
const static unsigned char NW20[] PROGMEM = "Twenty";
const static unsigned char NW21[] PROGMEM = "Twenty-One";
const static unsigned char NW22[] PROGMEM = "Twenty-Two";
const static unsigned char NW23[] PROGMEM = "Twenty-Three";
const static unsigned char NW24[] PROGMEM = "Twenty-Four";
const static unsigned char NW25[] PROGMEM = "Twenty-Five";
const static unsigned char NW26[] PROGMEM = "Twenty-Six";
const static unsigned char NW27[] PROGMEM = "Twenty-Seven";
const static unsigned char NW28[] PROGMEM = "Twenty-Eight";
const static unsigned char NW29[] PROGMEM = "Twenty-Nine";
const static unsigned char NW30[] PROGMEM = "Thirty";
const static unsigned char NW31[] PROGMEM = "Thirty-One";
const static unsigned char NW32[] PROGMEM = "Thirty-Two";
const static unsigned char NW33[] PROGMEM = "Thirty-Three";
const static unsigned char NW34[] PROGMEM = "Thirty-Four";
const static unsigned char NW35[] PROGMEM = "Thirty-Five";
const static unsigned char NW36[] PROGMEM = "Thirty-Six";
const static unsigned char NW37[] PROGMEM = "Thirty-Seven";
const static unsigned char NW38[] PROGMEM = "Thirty-Eight";
const static unsigned char NW39[] PROGMEM = "Thirty-Nine";
const static unsigned char NW40[] PROGMEM = "Forty";
const static unsigned char NW41[] PROGMEM = "Forty-One";
const static unsigned char NW42[] PROGMEM = "Forty-Two";
const static unsigned char NW43[] PROGMEM = "Forty-Three";
const static unsigned char NW44[] PROGMEM = "Forty-Four";
const static unsigned char NW45[] PROGMEM = "Forty-Five";
const static unsigned char NW46[] PROGMEM = "Forty-Six";
const static unsigned char NW47[] PROGMEM = "Forty-Seven";
const static unsigned char NW48[] PROGMEM = "Forty-Eight";
const static unsigned char NW49[] PROGMEM = "Forty-Nine";
const static unsigned char NW50[] PROGMEM = "Fifty";
const static unsigned char NW51[] PROGMEM = "Fifty-One";
const static unsigned char NW52[] PROGMEM = "Fifty-Two";
const static unsigned char NW53[] PROGMEM = "Fifty-Three";
const static unsigned char NW54[] PROGMEM = "Fifty-Four";
const static unsigned char NW55[] PROGMEM = "Fifty-Five";
const static unsigned char NW56[] PROGMEM = "Fifty-Six";
const static unsigned char NW57[] PROGMEM = "Fifty-Seven";
const static unsigned char NW58[] PROGMEM = "Fifty-Eight";
const static unsigned char NW59[] PROGMEM = "Fifty-Nine";
const static unsigned char NW60[] PROGMEM = "Sixty";
const static unsigned char NW61[] PROGMEM = "Sixty-One";
const static unsigned char NW62[] PROGMEM = "Sixty-Two";
const static unsigned char NW63[] PROGMEM = "Sixty-Three";
const static unsigned char NW64[] PROGMEM = "Sixty-Four";
const static unsigned char NW65[] PROGMEM = "Sixty-Five";
const static unsigned char NW66[] PROGMEM = "Sixty-Six";
const static unsigned char NW67[] PROGMEM = "Sixty-Seven";
const static unsigned char NW68[] PROGMEM = "Sixty-Eight";
const static unsigned char NW69[] PROGMEM = "Sixty-Nine";
const static unsigned char NW70[] PROGMEM = "Seventy";
const static unsigned char NW71[] PROGMEM = "Seventy-One";
const static unsigned char NW72[] PROGMEM = "Seventy-Two";
const static unsigned char NW73[] PROGMEM = "Seventy-Three";
const static unsigned char NW74[] PROGMEM = "Seventy-Four";
const static unsigned char NW75[] PROGMEM = "Seventy-Five";
const static unsigned char NW76[] PROGMEM = "Seventy-Six";
const static unsigned char NW77[] PROGMEM = "Seventy-Seven";
const static unsigned char NW78[] PROGMEM = "Seventy-Eight";
const static unsigned char NW79[] PROGMEM = "Seventy-Nine";
const static unsigned char NW80[] PROGMEM = "Eighty";
const static unsigned char NW81[] PROGMEM = "Eighty-One";
const static unsigned char NW82[] PROGMEM = "Eighty-Two";
const static unsigned char NW83[] PROGMEM = "Eighty-Three";
const static unsigned char NW84[] PROGMEM = "Eighty-Four";
const static unsigned char NW85[] PROGMEM = "Eighty-Five";
const static unsigned char NW86[] PROGMEM = "Eighty-Six";
const static unsigned char NW87[] PROGMEM = "Eighty-Seven";
const static unsigned char NW88[] PROGMEM = "Eighty-Eight";
const static unsigned char NW89[] PROGMEM = "Eighty-Nine";
const static unsigned char NW90[] PROGMEM = "Ninety";
const static unsigned char NW91[] PROGMEM = "Ninety-One";
const static unsigned char NW92[] PROGMEM = "Ninety-Two";
const static unsigned char NW93[] PROGMEM = "Ninety-Three";
const static unsigned char NW94[] PROGMEM = "Ninety-Four";
const static unsigned char NW95[] PROGMEM = "Ninety-Five";
const static unsigned char NW96[] PROGMEM = "Ninety-Six";
const static unsigned char NW97[] PROGMEM = "Ninety-Seven";
const static unsigned char NW98[] PROGMEM = "Ninety-Eight";
const static unsigned char NW99[] PROGMEM = "Ninety-Nine";
const static unsigned char NW100[] PROGMEM = "One Hundred";

const static unsigned char* const NUM_TO_WORD[] PROGMEM = {
  NW0, NW1, NW2, NW3, NW4, NW5, NW6, NW7, NW8, NW9, NW10,
  NW11, NW12, NW13, NW14, NW15, NW16, NW17, NW18, NW19, NW20,
  NW21, NW22, NW23, NW24, NW25, NW26, NW27, NW28, NW29, NW30,
  NW31, NW32, NW33, NW34, NW35, NW36, NW37, NW38, NW39, NW40,
  NW41, NW42, NW43, NW44, NW45, NW46, NW47, NW48, NW49, NW50,
  NW51, NW52, NW53, NW54, NW55, NW56, NW57, NW58, NW59, NW60,
  NW61, NW62, NW63, NW64, NW65, NW66, NW67, NW68, NW69, NW70,
  NW71, NW72, NW73, NW74, NW75, NW76, NW77, NW78, NW79, NW80,
  NW81, NW82, NW83, NW84, NW85, NW86, NW87, NW88, NW89, NW90,
  NW91, NW92, NW93, NW94, NW95, NW96, NW97, NW98, NW99, NW100
};

// extra strings
const static unsigned char QUESTION_MARK[] PROGMEM = "?";
const static unsigned char CARET[] PROGMEM = "^";
const static unsigned char DOT[] PROGMEM = ".";