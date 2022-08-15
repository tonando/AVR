#include <Macros.h>
#include <math.h>
#include <vplotter.h>

int main(){
	 		
	while(1){

	}
	return 0;
}











/* float rads(int n) {
  // Return an angle in radians
  return (n/180.0 * PI);
}    

void curves_drawCurve(long x, long y, long fx, long fy, long cx, long cy) {
  curves_drawCurve(x,y,fx,fy,cx,cy,0);
}

void curves_drawCurve(long x, long y, long fx, long fy, long cx, long cy, int speedOfSegment) {
  // Draw a Quadratic Bezier curve from (x, y) to (fx, fy) using control pt
  // (cx, cy)
  float xt=0;
  float yt=0;

  for (float t=0; t<=1; t+=.01) {
    xt = pow((1-t),2) *x + 2*t*(1-t)*cx+ pow(t,2)*fx;
    yt = pow((1-t),2) *y + 2*t*(1-t)*cy+ pow(t,2)*fy;
    
    if (speedOfSegment != 0)
    {
      motorA.setSpeed(speedOfSegment);
      motorB.setSpeed(speedOfSegment);
      usingAcceleration = false;
    }
    reportingPosition = false;
    changeLength(xt, yt);
    reportingPosition = true;
    usingAcceleration = true;
  }  
}
                                                     

void curves_drawCircle(long centerx, long centery, int radius) {
  // Estimate a circle using 20 arc Bezier curve segments
  int segments =20;
  int angle1 = 0;
  int midpoint=0;
   
//  changeLength(centerx+radius, centery);

  for (float angle2=360/segments; angle2<=360; angle2+=360/segments) {

    midpoint = angle1+(angle2-angle1)/2;

    float startx=centerx+radius*cos(rads(angle1));
    float starty=centery+radius*sin(rads(angle1));
    float endx=centerx+radius*cos(rads(angle2));
    float endy=centery+radius*sin(rads(angle2));
    
    int t1 = rads(angle1)*1000 ;
    int t2 = rads(angle2)*1000;
    int t3 = angle1;
    int t4 = angle2;

    curves_drawCurve(startx,starty,endx,endy,
              centerx+2*(radius*cos(rads(midpoint))-.25*(radius*cos(rads(angle1)))-.25*(radius*cos(rads(angle2)))),
              centery+2*(radius*sin(rads(midpoint))-.25*(radius*sin(rads(angle1)))-.25*(radius*sin(rads(angle2))))
    );
    
    angle1=angle2;
  }
}
 */