#ifndef POLYGON1
#define POLYGON1

#include "./garis1.cpp"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <math.h>
#include <algorithm>
#include "./basicfunction1.cpp"

using namespace std;

class Polygon1 {
    public:
        Polygon1() {

        }

        /* Open a file with the fileName */
        Polygon1(char * fileName) {
            FILE *file;
            file = fopen(fileName, "r");

            int x1, y1, x2, y2;
            int numberOfLines;
            fscanf(file, "%d", &numberOfLines);

            for(int i = 0; i < numberOfLines; i++){
                
                fscanf(file, "%d %d %d %d", &x1, &y1, &x2, &y2);
                
                Titik1 p1(x1, y1);
                Titik1 p2(x2, y2);
                Garis1 temp(p1, p2);
                lines.push_back(temp);
            }
            
            // get the max Titik1
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

        Titik1 getTopLeft() {
            return topLeft;
        }

        Titik1 getBottomRight() {
            return bottomRight;
        }
        
        void printPolygon1(int divx, int divy, int red, int green, int blue) {
            for(int i = 0; i < lines.size(); i++){
                lines[i].print(divx, divy, red, green, blue);
            }
        }

        void scanLine(int red, int green, int blue) {
            int scanLineY = topLeft.getY(); // Represent Y = c
            
            for(int scan = topLeft.getY(); scan <= bottomRight.getY(); scan++){
                vector<Titik1> listOfIntersectTitik1s;
                
                for(int i = 0; i < lines.size(); i++){
                    int firstOrdinatPolygon1 = lines[i].getAwal().getY();
                    int secondOrdinatPolygon1 = lines[i].getAkhir().getY();
                    
                    if((scanLineY - firstOrdinatPolygon1)*(scanLineY - secondOrdinatPolygon1) <= 0){
                        pair<float, float> tempEQ = lines[i].makeLine();

                        if(tempEQ.first == 0){
                            Titik1 tempTitik1(lines[i].getAwal().getX(), scanLineY);
                            listOfIntersectTitik1s.push_back(tempTitik1);
                        } else {
                            float x = (scanLineY - tempEQ.second) / tempEQ.first;
                            Titik1 tempTitik1(x, scanLineY);
                            listOfIntersectTitik1s.push_back(tempTitik1);
                        }
                    }

                    for(int j = i + 1; j < lines.size(); j++){
                        if(isCorner(lines[i], lines[j], scanLineY)){
                            pair<float, float> tempEQ = lines[i].makeLine();

                            if(tempEQ.first == 0){
                                Titik1 tempTitik1(lines[i].getAwal().getX(), scanLineY);
                                listOfIntersectTitik1s.push_back(tempTitik1);
                            } else {
                                float x = (scanLineY - tempEQ.second) / tempEQ.first;
                                Titik1 tempTitik1(x, scanLineY);
                                listOfIntersectTitik1s.push_back(tempTitik1);
                            }
                        }
                    }
                }

                // sort the Titik1s
                if(listOfIntersectTitik1s.size() != 0){
                    for(int i = 0; i < listOfIntersectTitik1s.size() - 1; i++) {
                        int smallest = i;
                        for(int j = i+1; j < listOfIntersectTitik1s.size(); j++){
                            if(listOfIntersectTitik1s[j].getX() < listOfIntersectTitik1s[smallest].getX()){
                                smallest = j;
                            }
                        }
                        Titik1 temp(listOfIntersectTitik1s[smallest].getX(), listOfIntersectTitik1s[smallest].getY());
                        listOfIntersectTitik1s[smallest] = listOfIntersectTitik1s[i];
                        listOfIntersectTitik1s[i] = temp;
                    }

                    if(listOfIntersectTitik1s.size() > 1){
                        int count = (listOfIntersectTitik1s.size() % 2 == 1) ? 
                            (listOfIntersectTitik1s.size() - 1) : listOfIntersectTitik1s.size();

                        for(int i = 0; i < count; i++) {
                            listOfIntersectTitik1s[i].setX(listOfIntersectTitik1s[i].getX()+1);
                            listOfIntersectTitik1s[i+1].setX(listOfIntersectTitik1s[i+1].getX()-1);

                            Garis1 line(listOfIntersectTitik1s[i], listOfIntersectTitik1s[i+1]);
                            line.print(0,0, red, green, blue);

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

        bool isCorner(Garis1 firstLine, Garis1 secondLine, int scanLine) {
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

        void scaleByTitik1(float scale, Titik1 P) {
            topLeft.scaleByTitik1(scale, P);
            bottomRight.scaleByTitik1(scale, P);
            for(int i = 0;i < lines.size();i++) {
                lines[i].scaleByTitik1(scale, P);
            }
        }

        void geser(int x, int y) {
            topLeft.geser(x, y);
            bottomRight.geser(x, y);
            for(int i = 0; i < lines.size(); i++){
                lines[i].geser(x, y);
            }
        }

    private:
        vector<Garis1> lines;
        Titik1 topLeft, bottomRight;
};

#endif
