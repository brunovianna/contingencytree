
#include "Adafruit_ThinkInk.h"

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


void setup() {

  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  randomSeed(analogRead(A0)); //initialize random generator
  
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
      
     float l = sqrt(sq(path_last_x[i]-path_x[i])+sq(path_last_y[i]-path_y[i]));
     float dx = ((-path_last_x[i]+path_x[i])/l);
     float dy = ((-path_last_y[i]+path_y[i])/l);
  
     
     for (int j = 0; j<int(l); j++) {
   
      int pixel_color = EPD_BLACK;
      if (k < 40) {
        pixel_color = EPD_BLACK;
      } else if (k < 45) {
        pixel_color = EPD_DARK;
      } else {
        pixel_color = EPD_LIGHT;
      }

       // x and y inverted on the display
       display.fillCircle(int(path_last_y[i]+j*dy),int(path_last_x[i]+j*dx),path_d[i], pixel_color);

     }
      
     update_path(i);
    }

  }
  display.display();
  Serial.println ("display");
  delay(3000);


}

void loop() {


 // put your main code here, to run repeatedly:
}

void update_path(int path_index) {
     if(path_x[path_index] > -10 & path_x[path_index] < WIDTH + 10 & path_y[path_index] > -10 & path_y[path_index] < HEIGHT + 10) {
        path_last_x[path_index] = path_x[path_index];
        path_last_y[path_index] = path_y[path_index];
        if (path_d[path_index] > 0.2) {  
           count++;
           bump_x = random(-100,100)/100.0;
           bump_y = random(-100,100)/100.0;
 
           ////velocity.normalize();
           float l = sqrt(sq( path_v_x[path_index])+sq(path_v_y[path_index]));
           path_v_x[path_index]/=l;
           path_v_y[path_index]/=l;
           
           bump_x*=0.2;
           bump_y*=0.2;
           path_v_x[path_index]*=0.8;
           path_v_y[path_index]*=0.8;
           path_v_x[path_index]+=bump_x;
           path_v_y[path_index]+=bump_y;
           rand_v_x= random(500,1000)/100.0;
           rand_v_y= random(500,1000)/100.0;
           path_v_x[path_index]*=rand_v_x;
           path_v_y[path_index]*=rand_v_y;
           path_x[path_index]+=path_v_x[path_index];
           path_y[path_index]+=path_v_y[path_index];
           if ((random(0, 100) < 20)&&(path_count<MAX_ELEMENTS-1)) { // control length
   
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
