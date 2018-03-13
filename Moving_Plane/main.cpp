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

    return 0;
}