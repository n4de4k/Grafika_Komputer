#ifndef CLIPWINDOW2
#define CLIPWINDOW2

class ClipWindow2 {
    public:
    ClipWindow2(int l, int r, int b, int t) {
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
        
    private:
    int l,r,b,t;
};

#endif