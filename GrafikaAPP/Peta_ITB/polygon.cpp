#ifndef POLYGON
#define POLYGON

#include "./garis.cpp"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <math.h>
#include <algorithm>
#include "./pointcode.cpp"

using namespace std;

class Polygon {
    public:
        Polygon() {

        }

        /* Open a file with the fileName */
        Polygon(char * fileName) {
            FILE *file;
            file = fopen(fileName, "r");

            int x1, y1, x2, y2;
            int numberOfLines;
            fscanf(file, "%d", &numberOfLines);

            for(int i = 0; i < numberOfLines; i++){
                
                fscanf(file, "%d %d %d %d", &x1, &y1, &x2, &y2);
                
                Titik p1(x1, y1);
                Titik p2(x2, y2);
                Garis temp(p1, p2);
                lines.push_back(temp);
            }
            
            // get the max Titik
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

        Titik getTopLeft() {
            return topLeft;
        }

        Titik getBottomRight() {
            return bottomRight;
        }
        
        void printPolygon(int divx, int divy, int red, int green, int blue) {
            for(int i = 0; i < lines.size(); i++){
                lines[i].print(divx, divy, red, green, blue);
            }
        }

        void scanLine(int red, int green, int blue, ClipWindow cw) {
            int scanLineY = topLeft.getY(); // Represent Y = c
            
            for(int scan = topLeft.getY(); scan <= bottomRight.getY(); scan++){
                vector<Titik> listOfIntersectTitiks;
                
                for(int i = 0; i < lines.size(); i++){
                    int firstOrdinatPolygon = lines[i].getAwal().getY();
                    int secondOrdinatPolygon = lines[i].getAkhir().getY();
                    
                    if((scanLineY - firstOrdinatPolygon)*(scanLineY - secondOrdinatPolygon) <= 0){
                        pair<float, float> tempEQ = lines[i].makeLine();

                        if(tempEQ.first == 0){
                            Titik tempTitik(lines[i].getAwal().getX(), scanLineY);
                            listOfIntersectTitiks.push_back(tempTitik);
                        } else {
                            float x = (scanLineY - tempEQ.second) / tempEQ.first;
                            Titik tempTitik(x, scanLineY);
                            listOfIntersectTitiks.push_back(tempTitik);
                        }
                    }

                    for(int j = i + 1; j < lines.size(); j++){
                        if(isCorner(lines[i], lines[j], scanLineY)){
                            pair<float, float> tempEQ = lines[i].makeLine();

                            if(tempEQ.first == 0){
                                Titik tempTitik(lines[i].getAwal().getX(), scanLineY);
                                listOfIntersectTitiks.push_back(tempTitik);
                            } else {
                                float x = (scanLineY - tempEQ.second) / tempEQ.first;
                                Titik tempTitik(x, scanLineY);
                                listOfIntersectTitiks.push_back(tempTitik);
                            }
                        }
                    }
                }

                // sort the Titiks
                if(listOfIntersectTitiks.size() != 0){
                    for(int i = 0; i < listOfIntersectTitiks.size() - 1; i++) {
                        int smallest = i;
                        for(int j = i+1; j < listOfIntersectTitiks.size(); j++){
                            if(listOfIntersectTitiks[j].getX() < listOfIntersectTitiks[smallest].getX()){
                                smallest = j;
                            }
                        }
                        Titik temp(listOfIntersectTitiks[smallest].getX(), listOfIntersectTitiks[smallest].getY());
                        listOfIntersectTitiks[smallest] = listOfIntersectTitiks[i];
                        listOfIntersectTitiks[i] = temp;
                    }

                    if(listOfIntersectTitiks.size() > 1){
                        int count = (listOfIntersectTitiks.size() % 2 == 1) ? 
                            (listOfIntersectTitiks.size() - 1) : listOfIntersectTitiks.size();

                        for(int i = 0; i < count; i++) {
                            listOfIntersectTitiks[i].setX(listOfIntersectTitiks[i].getX()+1);
                            listOfIntersectTitiks[i+1].setX(listOfIntersectTitiks[i+1].getX()-1);

                            if (listOfIntersectTitiks[i].getX() <= cw.getL()) {
                                listOfIntersectTitiks[i].setX(cw.getL() + 1);
                            }
                            if (listOfIntersectTitiks[i+1].getX() >= cw.getR()) {
                                listOfIntersectTitiks[i+1].setX(cw.getR() - 1);
                            }
                            if (listOfIntersectTitiks[i].getX() < listOfIntersectTitiks[i+1].getX()) {
                                Garis line(listOfIntersectTitiks[i], listOfIntersectTitiks[i+1]);
                                PointCode p1(line.getAwal(), cw);
                                PointCode p2(line.getAkhir(), cw);

                                if (PointCode::logicLine(p1,p2) == 0) {
                                    line.print(0,0, red, green, blue);
                                }
                            }
                            // Garis line(listOfIntersectTitiks[i], listOfIntersectTitiks[i+1]);
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

        bool isCorner(Garis firstLine, Garis secondLine, int scanLine) {
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

        void scaleByTitik(float scale, Titik P) {
            topLeft.scaleByTitik(scale, P);
            bottomRight.scaleByTitik(scale, P);
            for(int i = 0;i < lines.size();i++) {
                lines[i].scaleByTitik(scale, P);
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
        vector<Garis> lines;
        Titik topLeft, bottomRight;
};

#endif
