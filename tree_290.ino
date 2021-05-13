#include <stdio.h> // for function sprintf

#include "Adafruit_ThinkInk.h"
#include <Adafruit_NeoPixel_ZeroDMA.h>


#define EPD_DC      10 // can be any pin, but required!
#define EPD_CS      9  // can be any pin, but required!
#define EPD_BUSY    -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS     6  // can set to -1 to not use a pin (uses a lot of RAM!)
#define EPD_RESET   -1  // can set to -1 and share with chip Reset (can't deep sleep)

//ThinkInk_154_Grayscale4_T8 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
//ThinkInk_213_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

// 2.9" Grayscale Featherwing or Breakout:
ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
// 4.2" Grayscale display
//ThinkInk_420_Grayscale4_T2 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

#define COLOR1 EPD_BLACK
#define COLOR2 EPD_LIGHT
#define COLOR3 EPD_DARK

#define MAX_ELEMENTS 1000
#define WIDTH 128
#define HEIGHT 296

#define LOOP_LIMIT 50

#define LED_PIN        8
#define NUM_PIXELS 1

int num = 1000;
static int count;

int path_x[MAX_ELEMENTS];
int path_y[MAX_ELEMENTS];
int path_last_x[MAX_ELEMENTS];
int path_last_y[MAX_ELEMENTS];
int path_parent[MAX_ELEMENTS];
float path_v_x[MAX_ELEMENTS];
float path_v_y[MAX_ELEMENTS];
float path_d[MAX_ELEMENTS];
boolean path_f[MAX_ELEMENTS];
int path_count, path_start;
float bump_x, bump_y, rand_v_x, rand_v_y;

int led_delay, g, b;
bool grow_blue = true;
char serial_string[16];

Adafruit_NeoPixel_ZeroDMA strip(NUM_PIXELS, LED_PIN, NEO_GRB);


void setup() {

  strip.begin();
  strip.show();

  Serial.begin(115200);
//  while (!Serial) {
//    delay(10);
//  }

  randomSeed(analogRead(A0)); //initialize random generator


  Serial.println ("beginning");

  led_delay = 50;

  for (g = 0; g < 50; g++) {
    strip.setPixelColor(0, strip.Color(0, g, 0));
    strip.show();
    delay(led_delay);
  }
  b = 0;

  led_delay = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  grow_tree();
  delay(5000);

}

void grow_tree() {

  // put your setup code here, to run once:
  count = 0;

  path_x[0] = 64; //width /2
  path_y[0] = 296; //height
  path_last_x[0] = 64; //width /2
  path_last_y[0] = 296; //height
  path_v_x[0] = 0;
  path_v_y[0] = -10;
  path_d[0] = 19.88400223199278;
  path_f[0] = false;

  path_x[1] = 64; //width /2
  path_y[1] = 296; //height
  path_last_x[1] = 64; //width /2
  path_last_y[1] = 296; //height
  path_v_x[1] = 0;
  path_v_y[1] = -10;
  path_d[1] = 19.88400223199278;
  path_f[1] = false;

  path_count = 2;

  display.begin(THINKINK_GRAYSCALE4);
  display.clearBuffer();

  for (int k = 0; k < LOOP_LIMIT ; k++) {
    for (int i = 0; i < path_count; i++) {

      float l = sqrt(sq(path_last_x[i] - path_x[i]) + sq(path_last_y[i] - path_y[i]));
      float dx = ((-path_last_x[i] + path_x[i]) / l);
      float dy = ((-path_last_y[i] + path_y[i]) / l);


      for (int j = 0; j < int(l); j++) {

        int pixel_color = EPD_BLACK;
        if (k < 40) {
          pixel_color = EPD_BLACK;
        } else if (k < 45) {
          pixel_color = EPD_DARK;
        } else {
          pixel_color = EPD_LIGHT;
        }

        // x and y inverted on the display
        display.fillCircle(int(path_last_y[i] + j * dy), int(path_last_x[i] + j * dx), path_d[i], pixel_color);

      }

      Serial.println("upadte path ");
      update_path(i);

      if (b > 100) {
        b = 100;
        grow_blue = false;
      } else {
        if (b < 0) {
          b = 0;
          grow_blue = true;
        }
      }

      if (grow_blue) {
        b++;
        if ((b % 2) == 0) g--;
      } else {
        b--;
        if ((b % 2) == 0) g++;
      }
      constrain (g,0,50);
      strip.setPixelColor(0, strip.Color(0, g, b));
      strip.show();
      delay(led_delay);
      //    sprintf(serial_string, "green %4d, blue %4d", g, b);
      //    Serial.println(serial_string);
      Serial.print("i = ");
      Serial.println(i);
      Serial.print("l = ");
      Serial.println(l);


    }

  }

  display.display();
  Serial.println ("display");
  delay(3000);
}


void update_path(int path_index) {
  if (path_x[path_index] > -10 & path_x[path_index] < WIDTH + 10 & path_y[path_index] > -10 & path_y[path_index] < HEIGHT + 10) {
    path_last_x[path_index] = path_x[path_index];
    path_last_y[path_index] = path_y[path_index];
    if (path_d[path_index] > 0.2) {
      count++;
      bump_x = random(-100, 100) / 100.0;
      bump_y = random(-100, 100) / 100.0;

      ////velocity.normalize();
      float l = sqrt(sq( path_v_x[path_index]) + sq(path_v_y[path_index]));
      path_v_x[path_index] /= l;
      path_v_y[path_index] /= l;

      bump_x *= 0.2;
      bump_y *= 0.2;
      path_v_x[path_index] *= 0.8;
      path_v_y[path_index] *= 0.8;
      path_v_x[path_index] += bump_x;
      path_v_y[path_index] += bump_y;
      rand_v_x = random(500, 1000) / 100.0;
      rand_v_y = random(500, 1000) / 100.0;
      path_v_x[path_index] *= rand_v_x;
      path_v_y[path_index] *= rand_v_y;
      path_x[path_index] += path_v_x[path_index];
      path_y[path_index] += path_v_y[path_index];
      if ((random(0, 100) < 20) && (path_count < MAX_ELEMENTS - 1)) { // control length

        path_count++;
        path_x [path_count] = path_x[path_index];
        path_y [path_count] = path_y[path_index];
        path_last_x [path_count] = path_last_x[path_index];
        path_last_y [path_count] = path_last_y[path_index];
        path_v_x [path_count] = path_v_x[path_index];
        path_v_y [path_count] = path_v_y[path_index];
        path_d [path_count] = path_d[path_index] * 0.62;
        path_f [path_count] = path_f[path_index];
        path_d[path_index] = path_d [path_count];
      }
    } else {
      if (!path_f[path_index]) {
        path_f[path_index] = true;
        //noStroke();
        //fill(0, 0, 0, 0);
        //ellipse(path_x[path_index], path_x[path_index], 10, 10);
        //stroke(0, 0, 0);

        //display.fillCircle(x, y, radius, color);
        //display.fillCircle(path_x[path_index], path_x[path_index], 10,  EPD_BLACK);
      }
    }
  }
}
