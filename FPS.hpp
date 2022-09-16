#pragma once
#include <math.h>
#include <map>
#include <windows.h>

using namespace std;

extern const unsigned short fMapW, fMapH, cMapW, cMapH, screenW, screenH, FOV;
extern char screen[120*30], fullMap[50][50], circularMap[7][13], rotatedCircularMap[7][13];

extern HANDLE hConsole;
extern DWORD dwBytesWritten;
extern WORD attributes;

void fixAngle(float* angle) {
    if(*angle>=360) {*angle-=360;}
    if(*angle<0) {*angle+=360;}
}

void move(float* x, float* y, float angle, char dir) {
    float fx, fy;
    fx = sinf(angle*3.14f/180)/5;
    fy = cosf(angle*3.14f/180)/5;
    if(dir=='W'&&fullMap[(int)(*y-fy)][(int)(*x+fx)]==' ') {
        *x += fx;
        *y -= fy;
    }
    else if(dir=='A'&&fullMap[(int)(*y-fx)][(int)(*x-fy)]==' ') {
        *x -= fy;
        *y -= fx;
    }
    else if(dir=='S'&&fullMap[(int)(*y+fy)][(int)(*x-fx)]==' ') {
        *x -= fx;
        *y += fy;
    }
    else if(dir=='D'&&fullMap[(int)(*y+fx)][(int)(*x+fy)]==' ') {
        *x += fy;
        *y += fx;
    }
}

void updateCircularMap(int px, int py) {
    if(px<5) {
        px=5;
    }
    if(px>fMapW-6) {
        px=fMapW-6;
    }
    if(py<2) {
        py=2;
    }
    if(py>fMapH-3) {
        py=fMapH-3;
    }
    for(int y=0; y<cMapH; y++) {
        for(int x=0; x<cMapW; x++) {
            if(y!=0&&y!=6) {
                if(((y==1||y==5)&&(x!=0&&x!=1&&x!=2&&x!=10&&x!=11&&x!=12))||((y==2||y==4)&&(x!=0&&x!=1&&x!=11&&x!=12))||(y==3&&(x!=0&&x!=12))) {
                    circularMap[y][x]=fullMap[py+y-(cMapH/2)][px+x-(cMapW/2)];
                }
            }
        }
    }
}

void addMapToScreen() {
    for(int y=0; y<cMapH; y++) {
        for(int x=0; x<cMapW; x++) {
            if(((y==0||y==6)&&!(x==0||x==1||x==2||x==10||x==11||x==12))||((y==1||y==5)&&!(x==0||x==1||x==11||x==12))||((y==2||y==4)&&!(x==0||x==12))||y==3) {
                screen[(y*screenW)+(x+(screenW-cMapW))] = circularMap[y][x];
            }
        }
    }
}

void rayTrace(float px, float py, float angle) {
    float testX, testY, frayAngle, distance, slopeX, slopeY, fDepth=10;
    bool hitWall;
    char shading;
    int i=1;
    for(int x=0; x<screenW; x++) {
        frayAngle = (angle - ((float)FOV/2)) + (FOV*(x/(float)screenW));
        slopeX = sinf(frayAngle*3.14f/180);
        slopeY = cosf(frayAngle*3.14f/180);
        i=1;
        hitWall=false;
        while(!hitWall) {
            testX=px+(slopeX*i);
            testY=py+((slopeY*-1)*i++);
            distance=sqrtf(powf((px-testX), 2)+powf((py-testY), 2));
            if(fullMap[(int)testY][(int)testX]!=' '||distance>15) {
                if (distance <= fDepth/5.0f) {
                    shading = '#';
                }
                else if (distance < fDepth/3.0f) {
                    shading = '*';
                }
                else if (distance < fDepth/2.0f) {
                    shading = '"';
                }
                else if (distance < fDepth) {
                    shading = '\'';
                }
                else {
                    shading = ' ';
                }
                for(int y=0;y<screenH;y++) {
                    if(y>screenH-(int)(distance*0.5f)) {
                        screen[120*y+x] = ' ';
                    }
                    else {
                        screen[120*y+x] = shading;
                    }
                }

                hitWall=true;
            }
        }
    }
}