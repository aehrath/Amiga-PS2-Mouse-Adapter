/**
 * Reads X/Y values from a PS/2 mouse connected to an Arduino
 * using the PS2Mouse library available from
 *   http://github.com/kristopher/PS2-Mouse-Arduino/
 * Original by Kristopher Chambers <kristopher.chambers@gmail.com>
 * Updated by Jonathan Oxer <jon@oxer.com.au>
 * The Arduino code in this file was highly optimized by 
 * Alexander Ehrath <aehrath@hipergames.com>
 *   https://github.com/aehrath/Amiga-PS2-Mouse-Adapter
 * Highly responsive Amiga mouse emulation with middle mouse button.
 */

#include <PS2Mouse.h>
#define MOUSE_DATA 11
#define MOUSE_CLOCK 10

PS2IMouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);


static int pin_xa = 2;
static int pin_xb = 3;
static int pin_ya = 4;
static int pin_yb = 5;
static int pin_lb = 6;
static int pin_rb = 7;
static int pin_mb = 8;
/**
 * Setup
 */
void setup() {
  Serial.begin(38400);
  mouse.initialize();
  pinMode(pin_xa, OUTPUT);
  pinMode(pin_xb, OUTPUT);
  pinMode(pin_ya, OUTPUT);
  pinMode(pin_yb, OUTPUT);
  pinMode(pin_lb, OUTPUT);
  pinMode(pin_rb, OUTPUT);

  digitalWrite(pin_lb, HIGH);
  digitalWrite(pin_rb, HIGH);
  digitalWrite(pin_mb, HIGH);
}

void update_buttons(int16_t data)
{
  if (data & 1) {
    digitalWrite(pin_lb, LOW);
  }
  else
  {
    digitalWrite(pin_lb, HIGH);
  }
  if (data & 2) {
    digitalWrite(pin_rb, LOW);
  }
  else
  {
    digitalWrite(pin_rb, HIGH);
  }
  if (data & 4) {
    digitalWrite(pin_mb, LOW);
  }
  else
  {
    digitalWrite(pin_mb, HIGH);
  }
}

void send1(int16_t p0, int16_t p1)
{
  digitalWrite(p0, HIGH);
  delayMicroseconds(10);
  digitalWrite(p1, HIGH);
  delayMicroseconds(10);
  digitalWrite(p0, LOW);
  delayMicroseconds(10);
  digitalWrite(p1, LOW);
  delayMicroseconds(10);
}

void send2(int16_t p00, int16_t p01, int16_t p10, int16_t p11)
{
  digitalWrite(p00, HIGH);
  digitalWrite(p10, HIGH);
  delayMicroseconds(10);
  digitalWrite(p01, HIGH);
  digitalWrite(p11, HIGH);
  delayMicroseconds(10);
  digitalWrite(p00, LOW);
  digitalWrite(p10, LOW);
  delayMicroseconds(10);
  digitalWrite(p01, LOW);
  digitalWrite(p11, LOW);
  delayMicroseconds(10);
}

/**
 * Main program loop
 */

int16_t data[4];
int16_t data1dir;
int16_t data1p0;
int16_t data1p1;
int16_t data2dir;
int16_t data2p0;
int16_t data2p1;

void loop() {
  mouse.report(data);
  update_buttons(data[0]);
  if(data[1] > 0)
  {
    data1dir = -1;
    data1p0 = pin_xa;
    data1p1 = pin_xb;
  }
  else if(data[1] < 0)
  {
    data1dir = 1;
    data1p0 = pin_xb;
    data1p1 = pin_xa;
  }

  if(data[2] > 0)
  {
    data2dir = -1;
    data2p0 = pin_ya;
    data2p1 = pin_yb;
  }
  else if(data[2] < 0)
  {
    data2dir = 1;
    data2p0 = pin_yb;
    data2p1 = pin_ya;
  }
  while (data[1] != 0 && data[2] != 0)
  {
    send2(data1p0, data1p1, data2p0, data2p1);
    data[1] += data1dir;
    data[2] += data2dir;
  }
  while (data[1] != 0)
  {
    send1(data1p0, data1p1);
    data[1] += data1dir;
  }
  while (data[2] != 0)
  {
    send1(data2p0, data2p1);
    data[2] += data2dir;
  }
}
