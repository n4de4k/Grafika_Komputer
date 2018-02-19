#include "polygon.cpp"

using namespace std;

int main() {
    float skala = 0;
    int sudut = 0;
    while (1) {
        Polygon pesawat((char*) "./objek/pesawat.txt");
        Polygon baling((char*) "./objek/persegipanjang.txt");
        Polygon baling2((char*) "./objek/persegipanjang.txt");
        Basicfunction::clearscreen();
        Titik p(301,131);
        Titik q(499,131);
        Titik r(499,279);
        Titik s(301,279);
        Garis a(p,q);
        Garis b(q,r);
        Garis c(r,s);
        Garis d(s,p);
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

        // pesawat.printPolygon(0,0,255,255,255);
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
    return 0;
}