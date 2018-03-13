#ifndef MOVING_PLANE
#define MOVING_PLANE
#include "polygon1.cpp"

using namespace std;

void MovingPlane() {
    float skala = 0;
    int sudut = 0;
    
    while (skala <= 3) {
        Polygon1 pesawat((char*) "./Moving_Plane/objek/pesawat.txt");
        Polygon1 baling((char*) "./Moving_Plane/objek/persegipanjang.txt");
        Polygon1 baling2((char*) "./Moving_Plane/objek/persegipanjang.txt");
        Basicfunction1::clearscreen();

        pesawat.geser(250,150);
        baling.geser(250,150);
        baling2.geser(250,150);

        pesawat.scale(skala);
        baling.scale(skala);
        baling2.scale(skala);

        baling.rotate(sudut);
        baling2.rotate(sudut+90);

        // pesawat.printPolygon1(0,0,255,255,255);
        pesawat.scanLine(255,255,255);
        baling.scanLine(255,0,0);
        baling2.scanLine(255,0,0);

        skala += 0.01;
        sudut += 10;
        usleep(1000);
    }

}

#endif