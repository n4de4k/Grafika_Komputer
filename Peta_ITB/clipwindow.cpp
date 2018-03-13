#ifndef CLIPWINDOW
#define CLIPWINDOW


#include "./titik.cpp"
#include "./garis.cpp"

class ClipWindow {
    public:
    ClipWindow(int l, int r, int b, int t) {
        this->l = l;
        this->r = r;
        this->b = b;
        this->t = t;
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
    
    void setL(int x) {
        l = x;
    }

    void setR(int x) {
        r = x;
    }

    void setB(int x) {
        b = x;
    }

    void setT(int x) {
        t = x;
    }

    void print() {
        Titik p1(l,t);
        Titik p2(r,t);
        Titik p3(r,b);
        Titik p4(l,b);
        Garis a(p1,p2);
        Garis b(p2,p3);
        Garis c(p3,p4);
        Garis d(p4,p1);
        a.print(0,0,255,0,0);
        b.print(0,0,255,0,0);
        c.print(0,0,255,0,0);
        d.print(0,0,255,0,0);
    }
        
    private:
    int l,r,b,t;
};

#endif