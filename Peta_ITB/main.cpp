#include "polygon.cpp"
#include "lingkaran.cpp"

using namespace std;

char getch() {
      char c; // This function should return the keystroke
      system("stty raw");    // Raw input - wait for only a single keystroke
      system("stty -echo");  // Echo off
      c = getchar();
      system("stty cooked"); // Cooked input - reset
      system("stty echo");   // Echo on - Reset
      return c;
}

void PrintITB(ClipWindow cw) {
    Basicfunction::clearscreen();
    Polygon labtek5 = Polygon((char *) "./objek/labtek5.txt");
    Polygon labtek6 = Polygon((char *) "./objek/labtek6.txt");
    Polygon labtek7 = Polygon((char *) "./objek/labtek7.txt");
    Polygon labtek8 = Polygon((char *) "./objek/labtek8.txt");
    Polygon jalancc = Polygon((char *) "./objek/jalanantarcc.txt");
    Polygon jalandpr = Polygon((char *) "./objek/jalanantardpr.txt");
    Polygon ccbar = Polygon((char *) "./objek/ccbarat.txt");
    Polygon cctim = Polygon((char *) "./objek/cctimur.txt");
    Polygon gkubar = Polygon((char *) "./objek/gkubar.txt");
    Polygon gkutim = Polygon((char *) "./objek/gkutim.txt");
    Polygon tvst = Polygon((char *) "./objek/tvst.txt");
    Polygon pln = Polygon((char *) "./objek/pln.txt");
    Polygon jln1 = Polygon((char *) "./objek/jalanlabtek1.txt");
    Polygon jln2 = Polygon((char *) "./objek/jalanlabtek2.txt");

    cw.print();
    for (int i = 20; i > 10; i--) {
        Lingkaran l1 = Lingkaran(400,300,i);
        l1.drawCircle(cw);
    }

    for (int i = 40; i > 30; i--) {
        Lingkaran l1 = Lingkaran(400, 559, i);
        l1.drawCircle(cw);
    }
    
    labtek5.scanLine(0,255,0, cw);
    labtek6.scanLine(0,255,0, cw);
    labtek7.scanLine(0,255,0, cw);
    labtek8.scanLine(0,255,0, cw);
    jalancc.scanLine(0,0,255, cw);
    jalandpr.scanLine(0,0,255, cw);
    ccbar.scanLine(255,180,10, cw);
    cctim.scanLine(255,180,10, cw);
    gkubar.scanLine(180,220,79, cw);
    gkutim.scanLine(180,220,79, cw);
    tvst.scanLine(0,200,170,cw);
    pln.scanLine(0,200,170, cw);
    jln1.scanLine(0,0,255, cw);
    jln2.scanLine(0,0,255, cw);
    char c = getch();

    if (c != 'x') {
        if (c == 's') {
            cw.setT(cw.getT() + 1);
            cw.setB(cw.getB() + 1);
        } else if (c == 'w') {
            cw.setT(cw.getT() - 1);
            cw.setB(cw.getB() - 1);
        } else if (c == 'a') {
            cw.setR(cw.getR() - 1);
            cw.setL(cw.getL() - 1);
        } else if (c == 'd') {
            cw.setR(cw.getR() + 1);
            cw.setL(cw.getL() + 1);
        }

        PrintITB(cw);
    }
}

int main() {
    ClipWindow cw(80,590, 390, 30);
    PrintITB(cw);

    return 0;
}