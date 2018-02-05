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
int dotA[2] = {10,3};
int A_num = 15;

int B[14][4] = {{0,0,18,0},
                 {18,0,20,3},
                 {20,3,20,18},
                 {20,18,17,20},
                 {18,20,0,20},
                 {0,20,0,0},
                 {5,4,15,4},
                 {15,4,15,7},
                 {15,7,5,7},
                 {5,7,5,4},
                 {5,11,15,11},
                 {15,11,15,17},
                 {15,17,5,17},
                 {5,17,5,11}};
int dotB[2] = {1,1};
int B_num = 14;

int C[16][4] = {{3,0,17,0},
                 {17,0,20,4},
                 {20,4,20,7},
                 {20,7,17,7},
                 {17,7,15,3},
                 {15,3,4,3},
                 {4,3,4,18},
                 {4,18,17,18},
                 {17,18,17,15},
                 {17,15,20,15},
                 {20,15,20,17},
                 {20,17,17,20},
                 {17,20,4,20},
                 {4,20,0,17},
                 {0,17,0,3},
             	 {0,3,3,0}};
int dotC[2] = {5,2};
int C_num = 16;

int D[10][4] = {{0,0,17,0},
                 {17,0,20,4},
                 {20,4,20,17},
                 {20,17,17,20},
                 {17,20,0,20},
                 {0,20,0,0},
                 {5,4,15,4},
                 {15,4,15,17},
                 {15,17,5,17},
                 {5,17,5,4}	
                 };
int dotD[2] = {2,3};
int D_num = 10;

int E[12][4] = {{0,0,20,0},
                 {20,0,20,4},
                 {20,4,4,4},
                 {4,4,4,9},
                 {4,9,20,9},
                 {20,9,20,12},
                 {20,12,4,12},
                 {4,12,4,17},
                 {4,17,20,17},
                 {20,17,20,20},
                 {20,20,0,20},
                 {0,20,0,0}	
                 };
int dotE[2] = {2,3}; 
int E_num = 12;

int F[10][4] = {{0,0,20,0},
                 {20,0,20,4},
                 {20,4,4,4},
                 {4,4,4,9},
                 {4,9,13,9},
                 {13,9,13,12},
                 {13,12,4,12},
                 {4,12,4,20},
                 {4,20,0,20},
                 {0,20,0,0}	
                 };
int dotF[2] = {2,3};
int F_num = 10;

int G[18][4] = {{4,0,17,0},
                 {17,0,20,4},
                 {20,4,20,6},
                 {20,6,16,6},
                 {16,6,16,3},
                 {16,3,6,3},
                 {6,3,6,17},
                 {6,17,15,17},
                 {15,17,15,14},
                 {15,14,12,14},
                 {12,14,12,12},
                 {12,12,20,12},
                 {20,12,20,17},
                 {20,17,17,20},
                 {17,20,4,20},
             	 {4,20,0,17},
             	 {0,17,0,4},
             	 {0,4,4,0}};
int dotG[2] = {10,3};
int G_num = 18;

int H[12][4] = {{0,0,5,0},
                 {5,0,5,9},
                 {5,9,16,9},
                 {16,9,16,0},
                 {16,0,20,0},
                 {20,0,20,20},
                 {20,20,16,20},
                 {16,20,16,12},
                 {16,12,5,12},
                 {5,12,5,20},
                 {5,20,0,20},
                 {0,20,0,0}};
int dotH[2] = {10,10};
int H_num = 12;

int I[4][4] = {{6,0,15,0},
                 {15,0,15,20},
                 {15,20,6,20},
                 {6,20,6,0}};
int dotI[2] = {7,1};
int I_num = 4;

void clear_screen(int x_length, int y_length);
void draw_one_pixel(int x, int y, color* c);
void draw_line_low(int x0, int y0, int x1, int y1);
void draw_line_high(int x0, int y0, int x1, int y1);
void draw_line(int x0, int y0, int x1, int y1);
void floodFill(int x,int y,color* oldcolor,color* newcolor);
void drawChar(char c, int pos);

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

    // Getting user input
    char input[50], ch;
    int w = 0;
    while (ch != '\n') {
            ch = getchar();
            input[w] = ch;
            w++;
    }
    input[w-1] = '\0';
    // Filling algorithm
    w = 0;
    int pos = 0;
    clear_screen(800,300);
    while (input[w] != '\0') {
        drawChar(input[w], pos);
        w++;
        pos += 20;
    }
    // draw_one_pixel(10,1,&new);
    munmap(fbp, screenSize);
    close(fbfd);
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

void drawChar(char c, int pos) {
    color old;
    old.r = -1;
    old.g = -1;
    old.b = -1;
    old.a = 0;
    color new;
    new.r = 255;
    new.g = 0;
    new.b = 0;
    new.a = 0;

    switch (c) {
        case 'A' : {
            for (int i = 0; i < A_num; i++) {
                draw_line(A[i][0]+pos, A[i][1], A[i][2]+pos, A[i][3]);
            }

            floodFill(dotA[0]+pos,dotA[1],&old,&new);
        };
        break;

        case 'B' : {
            for (int i = 0; i < B_num; i++) {
                draw_line(B[i][0]+pos, B[i][1], B[i][2]+pos, B[i][3]);
            }

            floodFill(dotB[0]+pos,dotB[1],&old,&new);
        };
        break;

        case 'C' : {
            for (int i = 0; i < C_num; i++) {
                draw_line(C[i][0]+pos, C[i][1], C[i][2]+pos, C[i][3]);
            }

            floodFill(dotC[0]+pos,dotC[1],&old,&new);
        };
        break;

        case 'D' : {
            for (int i = 0; i < D_num; i++) {
                draw_line(D[i][0]+pos, D[i][1], D[i][2]+pos, D[i][3]);
            }

            floodFill(dotD[0]+pos,dotD[1],&old,&new);
        };
        break;

        case 'E' : {
            for (int i = 0; i < E_num; i++) {
                draw_line(E[i][0]+pos, E[i][1], E[i][2]+pos, E[i][3]);
            }

            floodFill(dotE[0]+pos,dotE[1],&old,&new);
        };
        break;

        case 'F' : {
            for (int i = 0; i < F_num; i++) {
                draw_line(F[i][0]+pos, F[i][1], F[i][2]+pos, F[i][3]);
            }

            floodFill(dotF[0]+pos,dotF[1],&old,&new);
        };
        break;

        case 'G' : {
            for (int i = 0; i < G_num; i++) {
                draw_line(G[i][0]+pos, G[i][1], G[i][2]+pos, G[i][3]);
            }

            floodFill(dotG[0]+pos,dotG[1],&old,&new);
        };
        break;

        case 'H' : {
            for (int i = 0; i < H_num; i++) {
                draw_line(H[i][0]+pos, H[i][1], H[i][2]+pos, H[i][3]);
            }

            floodFill(dotH[0]+pos,dotH[1],&old,&new);
        };
        break;

        case 'I' : {
            for (int i = 0; i < I_num; i++) {
                draw_line(I[i][0]+pos, I[i][1], I[i][2]+pos, I[i][3]);
            }

            floodFill(dotI[0]+pos,dotI[1],&old,&new);
        };
        break;

        default : {
            printf("Sorry. Font is not added yet. \n");
        };
        break;
    }
}