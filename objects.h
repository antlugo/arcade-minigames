// management of on-screen objects
// an object is an on-screen object, for example an actively rendered sprite or text
// objects drawn in update() with draw()

// lots of concurrent objects can quickly cause lag
// no actual bounds checking, just how much to allocate
// 9 bytes of ram per object
#define OBJECT_MAX 20

typedef struct
{
  bool is_text; // false if XBMP, true if text
  const char* data; // points to text or XBMP in PROGMEM
  unsigned char width; // only for XBMP
  unsigned char height; // only for XBMP
  int x; // for text and XBMP
  int y; // for text and XBMP
}
object;

// objects in this array are rendered
extern object obj[OBJECT_MAX]; 
extern int obj_count; // can be thought of as a working pointer for obj[]

// creates an object and adds it to obj[]
void add_obj(bool is_text, const char* data, unsigned char width, unsigned char height, int x, int y)
{
  obj[obj_count] = {.is_text = is_text, .data = data, .width = width, .height = height, .x = x, .y = y};
  obj_count++;
}

// clears obj[] by decreasing obj_count
void clear_obj()
{
  #ifdef DEBUG
    obj_count = 2; // preserve RAM FREE and FRAMETIME objects if DEBUG flag enabled
  #else
    obj_count = 0;
  #endif
}

// removes an object as specific index
// shifts above objects down
void remove_obj(unsigned char index)
{
  for (int i = index; i < obj_count - 1; i++)
  {
    obj[i] = obj[i + 1];
  }
  obj_count--;
}

// draws all objects in obj[] to oled display
void draw()
{
  for (int i = 0; i < obj_count; i++)
  {
    if (obj[i].is_text)
    {
      oled.setCursor(obj[i].x, obj[i].y);
      oled.print((__FlashStringHelper*)obj[i].data); // cast needed to print strings from program memory
    }            
    else
    {
      oled.drawXBMP(obj[i].x, obj[i].y, obj[i].width, obj[i].height, obj[i].data);
    }
  }
}


