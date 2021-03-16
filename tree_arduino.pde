/* OpenProcessing Tweak of *@*http://www.openprocessing.org/sketch/144159*@* */
/* !do not delete the line above, required for linking your tweak if you upload again */

class pathfinder {
  PVector lastLocation;
  PVector location;
  PVector velocity;
  float diameter;
  boolean isFinished;
  pathfinder() {
    location = new PVector(width/2, height);
    lastLocation = new PVector(location.x, location.y);
    velocity = new PVector(0, -10);
    diameter = 19.88400223199278;
    isFinished = false;
  }
  pathfinder(pathfinder parent) {
    location = parent.location.get();
    lastLocation = parent.lastLocation.get();
    velocity = parent.velocity.get();
    diameter = parent.diameter * 0.62;
    isFinished = parent.isFinished;
    parent.diameter = diameter;
  }
  void update() {
    
    if(location.x > -10 & location.x < width + 10 & location.y > -10 & location.y < height + 10) {
      lastLocation.set(location.x, location.y);
      if (diameter > 0.2) {
        count ++;
        PVector bump = new PVector(random(-1, 1), random(-1, 1));
        velocity.normalize();
        bump.mult(0.2);
        velocity.mult(0.8);
        velocity.add(bump);
        velocity.mult(random(5, 10));
        location.add(velocity);
        if (random(0, 1) < 0.2) { // control length
          paths = (pathfinder[]) append(paths, new pathfinder(this));
        }
      } else {
        if(!isFinished) {
          isFinished = true;
          noStroke();
          fill(0, 0, 0, 0);
          ellipse(location.x, location.y, 10, 10);
          stroke(0, 0, 0);
        }
      }
    }
  }
}
pathfinder[] paths;
int num = 1000;
static int count;

int[] path_x = new int[num];
int[] path_y = new int[num];
int[] path_last_x = new int[num];
int[] path_last_y = new int[num];
int[] path_parent = new int[num];
float[] path_v_x  = new float[num];
float[] path_v_y  = new float[num];
float[] path_d  = new float[num];
boolean[] path_f = new boolean[num];
int path_count, path_start;
float bump_x, bump_y, rand_v_x, rand_v_y;

void setup() {
  size(128, 296);
  background(250);
  ellipseMode(CENTER);
  stroke(0, 0, 0);
  //smooth();

  count = 0;
  
  //paths = new pathfinder[num];
  //for(int i = 0; i < num; i++) paths[i] = new pathfinder();
  
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

  
}

void update_path(int path_index) {
     if(path_x[path_index] > -10 & path_x[path_index] < width + 10 & path_y[path_index] > -10 & path_y[path_index] < height + 10) {
        path_last_x[path_index] = path_x[path_index];
        path_last_y[path_index] = path_y[path_index];
        if (path_d[path_index] > 0.2) {  
           count++;
           bump_x = random(-1,1);
           bump_y = random(-1,1);

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
           rand_v_x= random(5,10);
           rand_v_y= random(5,10);
           path_v_x[path_index]*=rand_v_x;
           path_v_y[path_index]*=rand_v_y;
           path_x[path_index]+=path_v_x[path_index];
           path_y[path_index]+=path_v_y[path_index];
           if ((random(0, 1) < 0.2)&&(path_count<num-1)) { // control length
   
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
             noStroke();
             fill(0, 0, 0, 0);
             ellipse(path_x[path_index], path_x[path_index], 10, 10);
             stroke(0, 0, 0);
           }
        }
     }
}


void draw() {
//  println(count);
  //println(paths.length);
 for (int i = 0; i < path_count; i++) {
    //println(path_count);
    //strokeWeight(1); //used to be paths[i].diameter
    stroke(0, 0, 0);
    //if ((path_last_x[i]==path_x[i])&&(path_last_y[i]==path_y[i])) {
    //  //line(path_x[i]-path_d[i]/2, path_y[i], path_x[i]+path_d[i]/2, path_y[i]);
    //} else {
    //  float l = sqrt(sq(path_last_x[i]-path_x[i])+sq(path_last_y[i]-path_y[i]));
    //  float dl = path_d[i] /l*2;
    //  int dx = int(abs(path_last_x[i]-path_x[i])*dl);
    //  int dy = int(abs(path_last_y[i]-path_y[i])*dl);
      
    //  if (path_last_x[i]<path_x[i]) {
    //    for (int j = 0; j<int(l); j++) {
    //      line(path_last_x[i]-dx-j, path_last_y[i]-dy-j, path_last_x[i]+dx-j, path_last_y[i]+dy-j);
    //    }
    //  } else {
    //    for (int j = 0; j<int(l); j++) {
    //      line(path_last_x[i]-dx-j, path_last_y[i]+dy-j, path_last_x[i]+dx-j, path_last_y[i]-dy-j);
    //    }
    //  }
    //}
   fill(0);
   float l = sqrt(sq(path_last_x[i]-path_x[i])+sq(path_last_y[i]-path_y[i]));
   float dx = ((-path_last_x[i]+path_x[i])/l);
   float dy = ((-path_last_y[i]+path_y[i])/l);

   
   for (int j = 0; j<int(l); j++) {
     //println (path_last_x[i]+j*dx/10);
     //println (path_last_y[i]+j*dy/10);
     //println (j);
     
     circle(int(path_last_x[i]+j*dx),int(path_last_y[i]+j*dy),path_d[i]);
   }
    
   stroke(0, 0, 0);
   line(path_last_x[i], path_last_y[i], path_x[i], path_y[i]);

    update_path(i);
  }
}
void mousePressed() {
 
}
