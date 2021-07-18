#include <M5Atom.h>
#include <AtomMotion.h>

#define DIS_COLOR_BLUE  0x000000ff
#define DIS_COLOR_RED   0x0000ff00
#define DIS_COLOR_GREEN 0x00ff0000

#define ANGLE_HOME 90
#define ANGLE_MIN 0
#define ANGLE_MAX 180

AtomMotion Motion;

int adjust[4] = {-1,1,6,0};

enum
{
  MODE_STOP = 0,
  MODE_FORWARD = 1,
};

int mode = MODE_STOP;
int step = 0;

int stop[1][4] = {
  {0,0,0,0},
};

int forward[36][4] = {
  {  0,  4,  0,  4},
  {  0,  6,  0,  6},
  {  0,  8,  0,  8},
  {  0, 10,  0, 10},
  {  0, 12,  0, 12},
  {  0, 14,  0, 14},
  {  2, 12,  2, 12},
  {  4, 10,  4, 10},
  {  6,  8,  6,  8},
  {  8,  6,  8,  6},
  { 10,  4, 10,  4},
  { 12,  0, 12,  0},
  /* */
  { 10, -4, 10, -4},
  {  8, -6,  8, -6},
  {  6, -8,  6, -8},
  {  4,-10,  4,-10},
  { -2,-12,  2,-12},
  {  0,-14,  0,-14},
  { -2,-12, -2,-12},
  { -4,-10, -4,-10},
  { -6, -8, -6, -8},
  { -8, -6, -8, -6},
  {-10, -4,-10, -4},
  {-12,  0,-12,  0},
  /* */
  {-10,  4,-10,  4},
  { -8,  6, -8,  6},
  { -6,  8, -6,  8},
  { -4, 10, -4, 10},
  { -2, 12, -2, 12},
  {  0, 14,  0, 14},
  {  2, 12,  2, 12},
  {  4, 10,  4, 10},
  {  6,  8,  6,  8},
  {  8,  6,  8,  6},
  { 10,  4, 10,  4},
  { 12,  0, 12,  0},
};

void setup()
{
  M5.begin(true, false, true);
  Serial.begin(115200);
  Motion.Init();

  applyMotion();
  updateUI();
}

void loop()
{
  M5.update();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
    case 'f':
      mode = MODE_FORWARD;
      step = 0;
      break;
    case 's':
      mode = MODE_STOP;
      step = 0;
      break;
    }
  }

  if(M5.Btn.wasPressed()){
    switch(mode) {
    case MODE_STOP:
      mode = MODE_FORWARD;
      step = 0;
      break;
    case MODE_FORWARD:
      mode = MODE_STOP;
      step = 0;
      break;
    }
  }

  applyMotion();
  updateUI();

  delay(100);
}

void applyMotion()
{
  int* angle = NULL;

  switch(mode) {
  case MODE_STOP:
    angle = stop[0];
    break;
  case MODE_FORWARD:
    angle = forward[step++];
    if (step >= 36) {
      step = 12;
    }
    break;
  }

  for(int ch = 0; ch < 4; ch++){
    int value = max(min(ANGLE_MAX, 90 + angle[ch] + adjust[ch]), ANGLE_MIN);
    Motion.SetServoAngle(ch + 1, value);
  }
}

void updateUI()
{
  int color = DIS_COLOR_RED;

  switch(mode) {
  case MODE_STOP:
    color = DIS_COLOR_BLUE;
    break;

  case MODE_FORWARD:
    color = DIS_COLOR_GREEN;
    break;
  }

  M5.dis.drawpix(0, color);
}
