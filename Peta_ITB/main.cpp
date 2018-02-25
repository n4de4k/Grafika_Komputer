#include "polygon.cpp"
#include "lingkaran.cpp"

using namespace std;

int main() {
    Basicfunction::clearscreen();

    for (int i = 60; i > 50; i--) {
        Lingkaran l1 = Lingkaran(400,300,i);
        l1.drawCircle();
    }
    

    return 0;
}