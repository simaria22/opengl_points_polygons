#include <GL/glut.h> 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


int x=rand()%5;
int y=rand()%7;
char result;
int count=0;
int test;
int arrayPolygon[5][2];

void points()
{
   // glClear(GL_COLOR_BUFFER_BIT);
   //double x;
    glPointSize(5.0f);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
    glFlush();
}

void line()
{

glBegin(GL_LINES);
    glVertex2f(x,y);
    glVertex2f(50.0f,y);
    glEnd();
    glFlush();

}



bool intersect(float v1w1, float v1z1, float v1w2, float v1z2,float v2w1, float v2z1, float v2w2, float v2z2)
{
    float dis1, dis2;
    float k1, k2, b1, b2, coeff_1, coeff_2;
    
    k1 = v1z2 - v1z1;
    b1 = v1w1 - v1w2;
    coeff_1 = (v1w2 * v1z1) - (v1w1 * v1z2);
    

    dis1 = (k1 * v2w1) + (b1 * v2z1) + coeff_1;
    dis2 = (k1 * v2w2) + (b1 * v2z2) + coeff_1;

   
    if (dis1 > 0 && dis2 > 0) return false;
    if (dis1 < 0 && dis2 < 0) return false;

    k2 = v2z2 - v2z1;
    b2 = v2w1 - v2w2;
    coeff_2 = (v2w2 * v2z1) - (v2w1 * v2z2); 

   
    dis1 = (k2 * v1w1) + (b2 * v1z1) + coeff_2;
    dis2 = (k2 * v1w2) + (b2 * v1z2) + coeff_2;

   
    if (dis1 > 0 && dis2 > 0) return false;
    if (dis1 < 0 && dis2 < 0) return false;

    if ((k1 * b2) - (k2 * b1) == 0.0f)    return true;

    
    return true;
}





void polygon(){
  
   int i;
    glPointSize(2.0f);
    glColor3f(0,0,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    glBegin(GL_LINE_LOOP);
    //printf("%d, %d\n",x,y);
    srand(time(NULL));
    for (i=0; i<5; i++){
        int z=rand()%22;
        int w=rand()%22;
        arrayPolygon[i][0]=z;
        arrayPolygon[i][1]=w;
        glVertex2f(z,w);
       printf("z= %d , w=%d\n",z,w);
        if(i>1){
        result=intersect(x,y,50,y,arrayPolygon[i][0],arrayPolygon[i][1],arrayPolygon[i-1][0],arrayPolygon[i-1][1]);
        if(result==true) {
        count++;
         }
        }
        
    }

   
     //printf("%d",i);
    bool test=intersect(x,y,50,y,arrayPolygon[i-1][0],arrayPolygon[i-1][1],arrayPolygon[0][0],arrayPolygon[0][1]);
    if (test==true) {
        count++;
    }
    
    glEnd();
    glFlush();

}





int leftRight( int p0x,int p0y,int p1x,int p1y,int p2x,int p2y )
{
    return ( (p1x - p0x) * (p2y - p0y)- (p2x -  p0x) * (p1y - p0y) );
}



//winding number test

int wnPoly(int px,int py, int array[][2], int n)
{
    int wn = 0;   
    int i=0;
    
    for (i=0; i<n; i++)
    {   

      if (array[i][1] <= py) {         
            if (array[i+1][1]  > py) {    
                     if (leftRight(array[i][0],array[i][1], array[i+1][0],array[i+1][1],px,py) > 0 && intersect(array[i][0],array[i][1], array[i+1][0],array[i+1][1],px,py,50.0f,y)==true)  { 
                             wn= wn+1;
                             
                     }
            }
                   
      }


     else {                 
            if(i!=(n-1)){
            if (array[i+1][1]  <= py) {
                 if (leftRight( array[i][0],array[i][1],array[i+1][0],array[i+1][1],px,py) < 0 && intersect(array[i][0],array[i][1],array[i+1][0],array[i+1][1],px,py,50.0f,y)==true) { 
                           wn=wn-1;  
                }
            }
         }
     }
    }          
        
    if(leftRight(array[i-1][0],array[i-1][1],array[0][0],array[0][1],px,py) > 0 && (intersect(px,py,50,y,arrayPolygon[i-1][0],arrayPolygon[i-1][1],arrayPolygon[0][0],arrayPolygon[0][1])==true)){
           wn=wn+1;   
    }else if(leftRight(array[i-1][0],array[i-1][1],array[0][0],array[0][1],px,py) < 0 && intersect(px,py,50,y,arrayPolygon[i-1][0],arrayPolygon[i-1][1],arrayPolygon[0][0],arrayPolygon[0][1])==true){
        wn=wn-1;
    }
    return wn;
}



void drawText(const char* string, float x, float y)
{
    const char* c;
     glColor3f(0,0,0);
    glRasterPos2f(x,y);
    for (c=(char*)string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}
void print_count(int count2){
 count2=wnPoly(x,y,arrayPolygon,5);
    printf("Winding number: %d\n",count2);
    drawText("Results of winding number test:", 9, 24);

    if(count2%2==0){
       drawText("Even winding number.", 9, 22);
       drawText("The red point is outside the polygon.", 9, 20);    
     }
    else {
      drawText("The red point is inside the polygon", 9, 20);
      drawText("Odd winding number.", 9, 22);
    }
}




void display()
{
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0,0,0);
    polygon();
    points();
    line();  
    print_count(test);  
    printf("Number of intersection points: %d\n",count); 
    drawText("Results of parity test:", 9, 35);
     if(count%2==0){
       drawText("Even number of intersection points", 9, 33);
       drawText("The red point is outside the polygon.", 9, 31);    
     }
    else {
      drawText("The red point is inside the polygon", 9, 33);
      drawText("Odd number of intersection points.", 9, 31);
    }
    
    
    
    //glEnd();
    glFlush();
}



    int main(int argc, char** argv)
    {
        glutInit(&argc,argv);
        glutInitWindowPosition(100,100);
        glutInitWindowSize(680,480);
        glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
        glutCreateWindow("Drawing polygons");
        glMatrixMode(GL_PROJECTION);
        gluOrtho2D(0,50,0,50);
        glutDisplayFunc(display);
        //glutPolygonFunc(polygon);
        glutMainLoop();
        return 0;


    }
