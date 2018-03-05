#ifndef MOVING_PLANE_CLIPPING
#define MOVING_PLANE_CLIPPING
#include "polygon2.cpp"

using namespace std;

void MVClipping() {
    float skala = 0;
    int sudut = 0;
    while (skala <= 3) {
        Polygon2 pesawat((char*) "./Moving_Plane_Clipping/objek/pesawat.txt");
        Polygon2 baling((char*) "./Moving_Plane_Clipping/objek/persegipanjang.txt");
        Polygon2 baling2((char*) "./Moving_Plane_Clipping/objek/persegipanjang.txt");
        Basicfunction2::clearscreen();
        Titik2 p(201,151);
        Titik2 q(399,151);
        Titik2 r(399,299);
        Titik2 s(201,299);
        Garis2 a(p,q);
        Garis2 b(q,r);
        Garis2 c(r,s);
        Garis2 d(s,p);
        a.print(0,0,255,0,0);
        b.print(0,0,255,0,0);
        c.print(0,0,255,0,0);
        d.print(0,0,255,0,0);
        pesawat.geser(250,150);
        baling.geser(250,150);
        baling2.geser(250,150);

        pesawat.scale(skala);
        baling.scale(skala);
        baling2.scale(skala);

        baling.rotate(sudut);
        baling2.rotate(sudut+90);

        // pesawat.printPolygon2(0,0,255,255,255);
        pesawat.scanLine(255,255,255);
        baling.scanLine(255,0,0);
        baling2.scanLine(255,0,0);

        skala += 0.01;
        sudut += 10;
        usleep(10000);
    }
    // Titik p(100,100);
    // Titik q(150,300);
    // ClipWindow cw(50,200,400,50);
    // PointCode p1(p,cw);
    // PointCode p2(q,cw);
    // printf("%d\n", PointCode::logicLine(p1,p2));
}

#endif