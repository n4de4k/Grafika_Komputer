#ifndef POLYGON2
#define POLYGON2

#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <math.h>
#include <algorithm>
#include "./basicfunction2.cpp"
#include "./garis2.cpp"

using namespace std;

class Polygon2 {
    public:
        Polygon2() {

        }

        /* Open a file with the fileName */
        Polygon2(char * fileName) {
            FILE *file;
            file = fopen(fileName, "r");

            int x1, y1, x2, y2;
            int numberOfLines;
            fscanf(file, "%d", &numberOfLines);

            for(int i = 0; i < numberOfLines; i++){
                
                fscanf(file, "%d %d %d %d", &x1, &y1, &x2, &y2);
                
                Titik2 p1(x1, y1);
                Titik2 p2(x2, y2);
                Garis2 temp(p1, p2);
                lines.push_back(temp);
            }
            
            // get the max Titik2
            fscanf(file, "%d %d", &x1, &y1);
            topLeft.setX(x1);
            topLeft.setY(y1);
            fscanf(file, "%d %d", &x1, &y1);
            bottomRight.setX(x1);
            bottomRight.setY(y1);
        }

        void print(int divx, int divy, int red, int green, int blue) {
            for(int i = 0; i < lines.size(); i++){
                lines[i].print(divx, divy, red, green, blue);
            }
        }

        Titik2 getTopLeft() {
            return topLeft;
        }

        Titik2 getBottomRight() {
            return bottomRight;
        }
        
        void printPolygon2(int divx, int divy, int red, int green, int blue) {
            for(int i = 0; i < lines.size(); i++){
                lines[i].print(divx, divy, red, green, blue);
            }
        }

        void scanLine(int red, int green, int blue) {
            int scanLineY = topLeft.getY(); // Represent Y = c
            
            for(int scan = topLeft.getY(); scan <= bottomRight.getY(); scan++){
                vector<Titik2> listOfIntersectTitik2s;
                
                for(int i = 0; i < lines.size(); i++){
                    int firstOrdinatPolygon2 = lines[i].getAwal().getY();
                    int secondOrdinatPolygon2 = lines[i].getAkhir().getY();
                    
                    if((scanLineY - firstOrdinatPolygon2)*(scanLineY - secondOrdinatPolygon2) <= 0){
                        pair<float, float> tempEQ = lines[i].makeLine();

                        if(tempEQ.first == 0){
                            Titik2 tempTitik2(lines[i].getAwal().getX(), scanLineY);
                            listOfIntersectTitik2s.push_back(tempTitik2);
                        } else {
                            float x = (scanLineY - tempEQ.second) / tempEQ.first;
                            Titik2 tempTitik2(x, scanLineY);
                            listOfIntersectTitik2s.push_back(tempTitik2);
                        }
                    }

                    for(int j = i + 1; j < lines.size(); j++){
                        if(isCorner(lines[i], lines[j], scanLineY)){
                            pair<float, float> tempEQ = lines[i].makeLine();

                            if(tempEQ.first == 0){
                                Titik2 tempTitik2(lines[i].getAwal().getX(), scanLineY);
                                listOfIntersectTitik2s.push_back(tempTitik2);
                            } else {
                                float x = (scanLineY - tempEQ.second) / tempEQ.first;
                                Titik2 tempTitik2(x, scanLineY);
                                listOfIntersectTitik2s.push_back(tempTitik2);
                            }
                        }
                    }
                }

                // sort the Titik2s
                if(listOfIntersectTitik2s.size() != 0){
                    for(int i = 0; i < listOfIntersectTitik2s.size() - 1; i++) {
                        int smallest = i;
                        for(int j = i+1; j < listOfIntersectTitik2s.size(); j++){
                            if(listOfIntersectTitik2s[j].getX() < listOfIntersectTitik2s[smallest].getX()){
                                smallest = j;
                            }
                        }
                        Titik2 temp(listOfIntersectTitik2s[smallest].getX(), listOfIntersectTitik2s[smallest].getY());
                        listOfIntersectTitik2s[smallest] = listOfIntersectTitik2s[i];
                        listOfIntersectTitik2s[i] = temp;
                    }

                    if(listOfIntersectTitik2s.size() > 1){
                        int count = (listOfIntersectTitik2s.size() % 2 == 1) ? 
                            (listOfIntersectTitik2s.size() - 1) : listOfIntersectTitik2s.size();

                        for(int i = 0; i < count; i++) {
                            listOfIntersectTitik2s[i].setX(listOfIntersectTitik2s[i].getX()+1);
                            listOfIntersectTitik2s[i+1].setX(listOfIntersectTitik2s[i+1].getX()-1);
                            if (listOfIntersectTitik2s[i].getX() <= 200) {
                                listOfIntersectTitik2s[i].setX(201);
                            }
                            if (listOfIntersectTitik2s[i+1].getX() >= 400) {
                                listOfIntersectTitik2s[i+1].setX(399);
                            }
                            if (listOfIntersectTitik2s[i].getX() < listOfIntersectTitik2s[i+1].getX()) {
                                Garis2 line(listOfIntersectTitik2s[i], listOfIntersectTitik2s[i+1]);
                                line.print(0,0, red, green, blue);
                            }

                            i++;
                        }
                    }
                }
                
                scanLineY++;
            }
        }		

        void rotate(int degree) {
			
            for(int i = 0;i < lines.size();i++) {
                lines[i].rotate(degree, topLeft, bottomRight);
            }
            
            // Update the topLeft and topRight
            int minX = topLeft.getX(), 
                minY = topLeft.getY(), 
                maxX = bottomRight.getX(),
                maxY = bottomRight.getY();
            for(int i = 0; i < lines.size(); i++){
                if(lines[i].getAwal().getX() < minX){
                    minX = lines[i].getAwal().getX();
                }
                if(lines[i].getAwal().getY() < minY){
                    minY = lines[i].getAwal().getY();
                }
                if(lines[i].getAkhir().getX() > maxX){
                    maxX = lines[i].getAkhir().getX();
                }
                if(lines[i].getAkhir().getY() > maxY){
                    maxY = lines[i].getAkhir().getY();
                }
            }
            topLeft.setX(minX);
            topLeft.setY(minY);
            bottomRight.setX(maxX);
            bottomRight.setY(maxY);

            // cout << topLeft.getX() << " " << topLeft.getY() << endl;
            // cout << bottomRight.getX() << " " << bottomRight.getY() << endl;
        }

        bool isCorner(Garis2 firstLine, Garis2 secondLine, int scanLine) {
            int sameY, result;
            if(firstLine.getAwal().getY() == secondLine.getAwal().getY()
                    && firstLine.getAwal().getY() == scanLine) {
                sameY = firstLine.getAwal().getY();
                result = (sameY - firstLine.getAkhir().getY()) *
                            (sameY - secondLine.getAkhir().getY());
                return result < 0;
            } else 
            if(firstLine.getAwal().getY() == secondLine.getAkhir().getY()
                    && firstLine.getAwal().getY() == scanLine) {
                sameY = firstLine.getAwal().getY();
                result = (sameY - firstLine.getAkhir().getY()) *
                            (sameY - secondLine.getAwal().getY());
                return result < 0;
            } else 
            if(firstLine.getAkhir().getY() == secondLine.getAwal().getY()
                    && firstLine.getAkhir().getY() == scanLine) {
                sameY = firstLine.getAkhir().getY();
                result = (sameY - firstLine.getAwal().getY()) *
                            (sameY - secondLine.getAkhir().getY());
                return result < 0;
            } else 
            if(firstLine.getAkhir().getY() == secondLine.getAkhir().getY()
                    && firstLine.getAkhir().getY() == scanLine) {
                sameY = firstLine.getAkhir().getY();
                result = (sameY - firstLine.getAwal().getY()) *
                            (sameY - secondLine.getAwal().getY());
                return result < 0;
            }
        }

        void scale(float scale) {
            int topLeftX = topLeft.getX();
            int topLeftY = topLeft.getY();
            int bottomRightX = bottomRight.getX();
            int bottomRightY = bottomRight.getY();
            topLeft.scale(scale, topLeftX, topLeftY, bottomRightX, bottomRightY);
            bottomRight.scale(scale, topLeftX, topLeftY, bottomRightX, bottomRightY);
            for(int i = 0;i < lines.size();i++) {
                lines[i].scale(scale, topLeft, bottomRight);
            }
        }

        void scaleByTitik2(float scale, Titik2 P) {
            topLeft.scaleByTitik2(scale, P);
            bottomRight.scaleByTitik2(scale, P);
            for(int i = 0;i < lines.size();i++) {
                lines[i].scaleByTitik2(scale, P);
            }
        }

        void geser(int x, int y) {
            topLeft.geser(x, y);
            bottomRight.geser(x, y);
            for(int i = 0; i < lines.size(); i++){
                lines[i].geser(x, y);
            }
        }

        void Dealokasi() {
            
        }

    private:
        vector<Garis2> lines;
        Titik2 topLeft, bottomRight;
};

#endif
