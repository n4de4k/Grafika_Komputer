#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>

/* GLOBAL VARIABLE */
typedef struct{
	int r,g,b,a;
} color;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0 ; //pointer framebuffer
int fbfd = 0; //pointer framebuffer driver

void clear_screen(int x_length, int y_length);
void draw_one_pixel(int x, int y, color* c);
void draw_line_low(int x0, int y0, int x1, int y1);
void draw_line_high(int x0, int y0, int x1, int y1);
void draw_line(int x0, int y0, int x1, int y1);

int main() {
    // Open driver framebuffer
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error : Can't open framebuffer");
        exit(1);
    }

    // Get screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error : Can't get fixed screen information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error : Can't get fixed screen information");
        exit(3);
    }

    // Mapping framebuffer to memory
    int screenSize = vinfo.xres * vinfo.yres *vinfo.bits_per_pixel / 8;
    fbp = (char *) mmap(0, screenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int) fbp == -1) {
        perror("Error : Failed to map framebuffer to memory");
        exit(4);
    }

    // Titik - titik huruf
    int A[15][4] = {{9,0,12,0},
                 {12,0,20,9},
                 {20,9,20,20},
                 {20,20,16,20},
                 {16,20,16,16},
                 {16,16,5,16},
                 {5,16,5,20},
                 {5,20,0,20},
                 {0,20,0,9},
                 {0,9,9,0},
                 {10,5,16,9},
                 {16,9,16,12},
                 {16,12,5,12},
                 {5,12,5,9},
                 {5,9,10,5}};
    clear_screen(400,300);
    for (int i = 0; i < 15; i++) {
        draw_line(A[i][0], A[i][1], A[i][2], A[i][3]);
    }
    return 0;
}

void clear_screen(int x_length, int y_length) {
    int x, y;

    for (x = 0; x < x_length; x++) {
        for (y = 0; y < y_length; y++) {
            long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
			                    (y + vinfo.yoffset) * finfo.line_length;
			                    *(fbp + position) = 255;
		                        *(fbp + position + 1) = 255;
		                        *(fbp + position + 2) = 255;
		                        *(fbp + position + 3) = 255;
        }
    }
}

void draw_one_pixel(int x, int y, color* c) {
    //Mencetak 1 dot pixel ke layar pada posisi x dan y
    long int pos = (x + vinfo.xoffset) *
                   (vinfo.bits_per_pixel / 8) +
                   (y + vinfo.yoffset) *
                   finfo.line_length;

    if(vinfo.bits_per_pixel == 32){
		*(fbp + pos) = c->b;
	    *(fbp + pos + 1) = c->g;
	    *(fbp + pos + 2) = c->r;
	    *(fbp + pos + 3) = c->a;
	}
	else {
        int b = c->b;
        int g = c->g;
        int r = c->r;
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + pos)) = t;
	}
}

void draw_line_low(int x0, int y0, int x1, int y1) {
    int dy = y1 - y0;
    int dx = x1 - x0;
    int adder_y = 1;
    if (dy < 0) {
        adder_y = -1;
        dy *= -1;
    }
    int D = 2*dy - dx;
    int y = y0;
    color c;
    c.r = 255;
    c.g = 0;
    c.b = 0;
    c.a = 0;

    for (int x = x0; x < x1; x++) {
        draw_one_pixel(x,y, &c);
        if (D > 0) {
            y += adder_y;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

void draw_line_high(int x0, int y0, int x1, int y1) {
    int dy = y1 - y0;
    int dx = x1 - x0;
    int adder_x = 1;
    if (dx < 0) {
        adder_x = -1;
        dx *= -1;
    }
    int D = 2*dx - dy;
    int x = x0;
    color c;
    c.r = 255;
    c.g = 0;
    c.b = 0;
    c.a = 0;

    for (int y = y0; y < y1; y++) {
        draw_one_pixel(x,y, &c);
        if (D > 0) {
            x += adder_x;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}

void draw_line(int x0, int y0, int x1, int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 >= x1) {
            draw_line_low(x1,y1,x0,y0);
        } else {
            draw_line_low(x0,y0,x1,y1);
        }
    } else {
        if (y0 >= y1) {
            draw_line_high(x1,y1,x0,y0);
        } else {
            draw_line_high(x0,y0,x1,y1);
        }
    }
}