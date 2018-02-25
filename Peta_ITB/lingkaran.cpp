#ifndef LINGKARAN
#define LINGKARAN

#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <math.h>
#include <algorithm>

using namespace std;
class Lingkaran {
    public:
    Lingkaran(int x, int y, int r) {
        x0 = x;
        y0 = y;
        radius = r;
    }

    void drawPixel(int x, int y, int r, int g, int b) {
        int fbfd = 0;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        long int screensize = 0;
        char *fbp = 0;
        long int location = 0;

        // Open the file for reading and writing
        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1) {
            perror("Error: cannot open framebuffer device");
            exit(1);
        }

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

        // Figure out the size of the screen in bytes
        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

        // Map the device to memory
        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if (*fbp == -1) {
            perror("Error: failed to map framebuffer device to memory");
            exit(4);
        }
        // =============================================
        
        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                            (y+vinfo.yoffset) * finfo.line_length;
        if(y > 0 && y < vinfo.yres  && x > 0 && x < vinfo.xres){ 
            *(fbp + location) = b;        
            *(fbp + location + 1) = g;     
            *(fbp + location + 2) = r;    
            *(fbp + location + 3) = 0;      
        }


        // ============================================
        munmap(fbp, screensize);
        close(fbfd);
    }

    void drawCircle() {
        int r = 255, g = 0, b = 0;
        int x = radius - 1;
        int y = 0;
        int dx = 1;
        int dy = 1;
        int err = dx - (radius << 1);

        while (x >= y) {
            drawPixel(x0 + x, y0 + y, r, g, b);
            drawPixel(x0 + y, y0 + x, r, g, b);
            drawPixel(x0 - y, y0 + x, r, g, b);
            drawPixel(x0 - x, y0 + y, r, g, b);
            drawPixel(x0 - x, y0 - y, r, g, b);
            drawPixel(x0 - y, y0 - x, r, g, b);
            drawPixel(x0 + y, y0 - x, r, g, b);
            drawPixel(x0 + x, y0 - y, r, g, b);

            if (err <= 0) {
                y++;
                err += dy;
                dy += 2;
            }
            if (err > 0) {
                x--;
                dx += 2;
                err += dx - (radius << 1);
            }
            // printf("%d - %d\n", x, y);
        }
    };

    private:
    int x0,y0,radius;
};

#endif