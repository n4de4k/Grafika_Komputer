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

void clear_screen();
void draw_one_pixel(int x, int y, color* c);
void drawPlane(int posX, int posY);

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
    //Posisi awal pesawat
    int x_start = 768;

    // Loop untuk menggerakan pesawat
    while(1) {
        clear_screen();
        drawPlane(x_start, 1);
        x_start--;
        if (x_start <= 0) x_start = 768;
        usleep(17);
    }

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

void drawPlane(int posX, int posY){
    //Mencetak pesawat pada posisi X dan Y
    FILE * f = fopen("plane.txt", "r");
    char dummy[32];
    int j = posY;

    while(!feof(f)) {
        fscanf(f, "%s", dummy);
        int i;
        for (i = 0; i < 32; i++) {
            color c;
            if (dummy[i] == '1') {
                c.r = 0;
                c.g = 0;
                c.b = 0;
                c.a = 0;
            } else {
                c.r = 255;
                c.g = 255;
                c.b = 255;
                c.a = 255;
            };
            draw_one_pixel(posX + i, posY + j, &c);
            if (i == 31) j++;
            // printf("%c", dummy[i]);
        }
        // printf("\n");
        j++;
    }
}

void clear_screen() {
    int x, y;

    for (x = 0; x < 800; x++) {
        for (y = 0; y < 600; y++) {
            long int position = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + 
			                    (y + vinfo.yoffset) * finfo.line_length;
			                    *(fbp + position) = 255;
		                        *(fbp + position + 1) = 255;
		                        *(fbp + position + 2) = 255;
		                        *(fbp + position + 3) = 255;
        }
    }
}
