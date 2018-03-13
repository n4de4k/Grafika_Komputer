#ifndef TITIK1
#define TITIK1
#define PI 3.142

#include <math.h>

class Titik1 {
    public :
        Titik1() {
            this->x = 0;
            this->y = 0;
        }
        Titik1(int x, int y) {
            this->x = x;
            this->y = y;
        }
        int getX() {
            return x;
        }

        int getY() {
            return y;
        }

        int setX(int x) {
            this->x = x;
        }

        int setY(int y) {
            this->y = y;
        }

        void rotate(int degree, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
            double val = PI/180;
            float sinus = sin(degree*val);
            float minsin = -sinus;
            float cosinus = cos(degree*val);
            float centerX = 375;//((topLeftX + bottomRightX) / 2);
            float centerY = 210; //((topLeftY + bottomRightY) / 2);
            
            float tempx = getX() - centerX;
            float tempy = getY() - centerY;
            float finalx = (tempx * cosinus - tempy * sinus) + centerX; 
            float finaly = (tempx * sinus + tempy * cosinus) + centerY;
            //round to int
            setX(finalx);
            setY(finaly);
        }

        void move(int h, int v,int topLeftX, int topLeftY, int bottomRightX, int bottomRightY){
			float centerX = ((topLeftX + bottomRightX) / 2);
            float centerY = ((topLeftY + bottomRightY) / 2);
            float finalx = getX() + h;
            float finaly = getY() + v;
            
            setX(finalx);
            setY(finaly);
		}

        void scaleByTitik1(float scale, Titik1 P) {
            float finalx = scale * (getX() - P.getX()) + P.getX();
            float finaly = scale * (getY() - P.getY()) + P.getY();
            setX(finalx);
            setY(finaly);
        }

        void scale(float scale, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
            float centerX = ((topLeftX + bottomRightX) / 2);
            float centerY = ((topLeftY + bottomRightY) / 2);
            
            float finalx = scale * (getX() - centerX) + centerX;
            float finaly = scale * (getY() - centerY) + centerY;
            setX(finalx);
            setY(finaly);
        }

        void geser(int divx, int divy) {
            x += divx;
            y += divy;
        }

    private :
    int x;
    int y;
};

#endif