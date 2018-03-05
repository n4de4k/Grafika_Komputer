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

/* MAIN PROGRAM */
void Font() {
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
    if (*fbp == -1) {
        perror("Error : Failed to map framebuffer to memory");
        exit(4);
    }


    int x = 0, y = 10;

    unsigned char m[20][20];
    char input[30], ch;
    int w = 0;
    while (ch != '\n') {
            ch = getchar();
            input[w] = ch;
            w++;
    }
    input[w-1] = '\0';
    w = 0;
    clear_screen();
    while (input[w] != '\0') {
        char parsed[18];
        parsed[0] = '.';
        parsed[1] = '/';
        parsed[2] = 'F';
        parsed[3] = 'o';
        parsed[4] = 'n';
        parsed[5] = 't';
        parsed[6] = '_';
        parsed[7] = 'H';
        parsed[8] = 'u';
        parsed[9] = 'r';
        parsed[10] = 'u';
        parsed[11] = 'f';
        parsed[12] = '/';
        if (input[w] == ' ') {
            parsed[13] = '1';    
        } else {
            parsed[13] = input[w];
        }        
        parsed[14] = '.';
        parsed[15] = 't';
        parsed[16] = 'x';
        parsed[17] = 't';
        parsed[18] = '\0';
        w++;
        FILE * f = fopen(parsed, "r");
        char dummy[20];
        int a = 0;
        while(!feof(f)) {
            fscanf(f, "%s", dummy);
            int u;
            for (u = 0; u < 20; u++) {
                m[a][u] = dummy[u];
            }
            a++;
        }

        fclose(f);

        int i, j;
        for (i = 0; i < 20; i++) {
            for (j = 0; j < 20; j++) {
                color c;
                if (m[i][j] == '1') {
                    c.r = 20;
                    c.g = 130;
                    c.b = 125;
                    c.a = 0;
                } else {
                    c.r = 255;
                    c.g = 255;
                    c.b = 255;
                    c.a = 255;
                };
                draw_one_pixel(x + j, y + i, &c);
            }
        }
        x = w * 20;
	if (w % 40 == 0) {
	    x = 0;
  	    y += 21;
	} 
    }
    // char* nama_file = input + "txt";

    munmap(fbp, screenSize);
    close(fbfd);

    getchar();
}

/* FUNCTION AND PROCEDURE */
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

void draw_one_pixel(int x, int y, color* c) {
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
