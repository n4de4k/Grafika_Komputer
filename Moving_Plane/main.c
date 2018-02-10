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

typedef struct {
    int x;
    int y;
} dot;

typedef struct {
    dot awal;
    dot akhir;
} line;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0 ; //pointer framebuffer
int fbfd = 0; //pointer framebuffer driver

void clear_screen(int x_length, int y_length);
void draw_one_pixel(int x, int y, color* c);
void draw_line_low(int x0, int y0, int x1, int y1);
void draw_line_high(int x0, int y0, int x1, int y1);
void draw_line(int x0, int y0, int x1, int y1);
void floodFill(int x,int y,color* oldcolor,color* newcolor);
void draw_rectangle(line[4]);
dot rotate_dot(dot, float, float, float);
void rotate_rectangle(float, line[4]);

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
    /* ================================================================================= */
    clear_screen(800,600);

    dot sudut1 = {200,200};

    line rec[4] = {
        {{200,200},{380,200}},
        {{380,200},{380,220}},
        {{380,220},{200,220}},
        {{200,220},{200,200}}
    };
    //drawing rectangle
    float sudut = 1;
    while(1) {
        usleep(1);
        rotate_rectangle((sudut*3.142)/180, rec);
        sudut++;
        clear_screen(400,300);
        usleep(1);
    }

    /* ================================================================================= */    
    munmap(fbp, screenSize);
    close(fbfd);
    return 0;
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

void floodFill(int x,int y,color* oldcolor,color* newcolor) {
    long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (y+vinfo.yoffset)*finfo.line_length;

    color c;
    c.r = *(fbp + location);
    c.g = *(fbp + location + 1);
    c.b = *(fbp + location + 3);
    c.a = 0;

    // printf("%d %d %d\n", c.r, c.g, c.b);
    if (c.r == oldcolor->r &&
        c.g == oldcolor->g &&
        c.b == oldcolor->b) {
            *(fbp + location) = newcolor->b;
	        *(fbp + location + 1) = newcolor->g;
	        *(fbp + location + 2) = newcolor->r;
	        *(fbp + location + 3) = newcolor->a;

            floodFill(x+1, y, oldcolor, newcolor);
            floodFill(x, y+1, oldcolor, newcolor);
            floodFill(x-1, y, oldcolor, newcolor);
            floodFill(x, y-1, oldcolor, newcolor);
        }
}

void draw_rectangle(line rec[4]) {
    color old;
    old.r = -1;
    old.g = -1;
    old.b = -1;
    old.a = 0;
    color new;
    new.r = 255;
    new.g = 0;
    new.b = 255;
    new.a = 0;

    for (int i = 0; i < 4; i++) {
        draw_line(rec[i].awal.x,rec[i].awal.y,rec[i].akhir.x,rec[i].akhir.y);
    }
    
    // floodFill(290, 210, &old, &new);

}

dot rotate_dot(dot ori, float pusat_x, float pusat_y, float sudut) {
    // translasi ke origin dot
    // printf("rotate titik %d berjalan\n", ori.x);
    ori.x -= pusat_x;
    ori.y -= pusat_y;

    // rotasi
    float xnew = ori.x * cos(sudut) - ori.y * sin(sudut);
    float ynew = ori.x * sin(sudut) + ori.y * cos(sudut);

    // update titik
    ori.x = xnew + pusat_x;
    ori.y = ynew + pusat_y;
    return ori;
}

void rotate_rectangle(float sudut, line original_rec[4]) {
    //Pusat persegi panjang
    float pusat_x = (original_rec[0].awal.x + original_rec[0].akhir.x) / 2;
    float pusat_y = (original_rec[1].awal.y + original_rec[1].akhir.y) / 2;

    // Dapat sudut hasil rotasi
    line new_rec[4];
    for (int i = 0; i < 4; i++) {
        dot awal = rotate_dot(original_rec[i].awal,pusat_x, pusat_y, sudut);
        dot akhir = rotate_dot(original_rec[i].akhir,pusat_x, pusat_y, sudut);
        new_rec[i].awal = awal;
        new_rec[i].akhir = akhir;
    }

    draw_rectangle(new_rec);
}