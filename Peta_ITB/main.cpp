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
    cw.print();
    for (int i = 20; i > 10; i--) {
        Lingkaran l1 = Lingkaran(400,300,i);
        l1.drawCircle(cw);
    }
    
    labtek5.scanLine(0,255,0, cw);
    labtek6.scanLine(0,255,0, cw);
    labtek7.scanLine(0,255,0, cw);
    labtek8.scanLine(0,255,0, cw);

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
    ClipWindow cw(260,400, 300, 150);
    PrintITB(cw);

    return 0;
}