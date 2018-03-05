#include "./titik2.cpp"
#include "./clipwindow2.cpp"
#include "./garis2.cpp"
#include <iostream>
using namespace std;
class PointCode2 {
    public:
    PointCode2 (Titik2 p, ClipWindow2 cw) {
        if (p.getX() > cw.getL()) l = false;
        else l = true;

        if (p.getX() < cw.getR()) r = false;
        else r = true;

        if (p.getY() < cw.getB()) b = false;
        else b = true;

        if (p.getY() > cw.getT()) t = false;
        else t = true;
    }

    static int logicLine(PointCode2 p1, PointCode2 p2) {
        int dummyLogic[4];
        dummyLogic[0] = p1.getL() && p2.getL();
        dummyLogic[1] = p1.getR() && p2.getR();
        dummyLogic[2] = p1.getT() && p2.getT();
        dummyLogic[3] = p1.getB() && p2.getB();
        
        return (dummyLogic[0] || dummyLogic[1] || dummyLogic[2] || dummyLogic [3]);
    }

    int getL() {
        return l;
    }

    int getR() {
        return r;
    }

    int getB() {
        return b;
    }

    int getT() {
        return t;
    }

    private:
    bool l,r,t,b;
};