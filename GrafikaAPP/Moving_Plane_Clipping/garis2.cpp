#ifndef GARIS2
#define GARIS2

#include "./pointcode2.cpp"
#include "./titik2.cpp"
#include <utility>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

using namespace std;

class Garis2 {
    public :
    Garis2() {

    }

    Garis2(Titik2 p, Titik2 q) {
        (this->awal).setX(p.getX());
        (this->awal).setY(p.getY());
        (this->akhir).setX(q.getX());
        (this->akhir).setY(q.getY());
    }

    Titik2 getAwal() {
        return this->awal;
    }

    Titik2 getAkhir() {
        return this->akhir;
    }

    void setAwal(Titik2 awal) {
        this->awal = awal;
    }

    void setAkhir(Titik2 akhir) {
        this->akhir = akhir;
    }

    pair<float, float> makeLine() {
        float x1 = awal.getX() * 1.0;
        float y1 = awal.getY() * 1.0;
        float x2 = akhir.getX() * 1.0;
        float y2 = akhir.getY() * 1.0;

        float a,b;
        if(x2 - x1 == 0){
            a = 0;
            b = 0;    
        } else {
            a = (y2 - y1) / (x2 - x1);
            b = y1 - (x1 * (y2 - y1) / (x2 - x1));
        }

        return make_pair(a, b);
    }

    void print(int divx, int divy, int r, int g, int b){
        
        ClipWindow2 cw(200,400, 300, 150);
        PointCode2 p1(awal, cw);
        PointCode2 p2(akhir, cw);

        if (PointCode2::logicLine(p1,p2) == 0) {
            int fbfd = 0;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        long int screensize = 0;
        char *fbp = 0;
        int x = 0, y = 0, timer = 0;
        long int location = 0;
        int maxY;
        // Open the file for reading and writing
        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1) {
            perror("Error: cannot open framebuffer device");
            exit(1);
        }
        //printf("The framebuffer device was opened successfully.\n");

        // Get fixed screen information
        if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
            perror("Error reading fixed information");
            exit(2);
        }

        // Get variable screen information
        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
            perror("Error reading variable information");
            exit(3);
        }

        //printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

        // Figure out the size of the screen in bytes
        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

        // Map the device to memory
        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if (*fbp == -1) {
            perror("Error: failed to map framebuffer device to memory");
            exit(4);
        }

        int x0 = awal.getX();
        int y0 = awal.getY();
        int x1 = akhir.getX();
        int y1 = akhir.getY();

        int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
        int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
        int err = (dx>dy ? dx : -dy)/2, e2;

        x0 += divx;
        y0 += divy;
        x1 += divx;
        y1 += divy;
        
        for(;;){
            location = (x0+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                            (y0+vinfo.yoffset) * finfo.line_length;
            if(y0 > 10 && y0 < vinfo.yres -10 && x0 > 5 && x0 < vinfo.xres){ // escape seg fault
                *(fbp + location) = b;        // Some blue
                *(fbp + location + 1) = g;     // A little green
                *(fbp + location + 2) = r;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
            if (x0==x1 && y0==y1) break;
            e2 = err;
            if (e2 >-dx) { err -= dy; x0 += sx; }
            if (e2 < dy) { err += dx; y0 += sy; }
        }

        munmap(fbp, screensize);
        close(fbfd);
        } else {
            
        }
    }

    void rotate(float degree, Titik2 topLeft, Titik2 bottomRight) {
        awal.rotate(degree, topLeft.getX(), topLeft.getY(), bottomRight.getX(), bottomRight.getY());
        akhir.rotate(degree, topLeft.getX(), topLeft.getY(), bottomRight.getX(), bottomRight.getY());;
    }

    void scale(float scale, Titik2 topLeft, Titik2 bottomRight) {
        awal.scale(scale, topLeft.getX(), topLeft.getY(), bottomRight.getX(), bottomRight.getY());
        akhir.scale(scale, topLeft.getX(), topLeft.getY(), bottomRight.getX(), bottomRight.getY());
    }

    void scaleByTitik2(float scale, Titik2 P) {
        awal.scaleByTitik2(scale, P);
        akhir.scaleByTitik2(scale, P);
    }

    void move(int h, int v, Titik2 topLeft, Titik2 bottomRight) {
        awal.move(h,v,topLeft.getX(), topLeft.getY(), bottomRight.getX(), bottomRight.getY());
        akhir.move(h,v,topLeft.getX(), topLeft.getY(), bottomRight.getX(), bottomRight.getY());
    }
    
    void geser(int x, int y){
        awal.geser(x, y);
        akhir.geser(x, y);
    }
    private :
    Titik2 awal;
    Titik2 akhir;
};

#endif