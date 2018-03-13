#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<linux/input.h>
#include<unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "./Font_Huruf/font.cpp"
#include "./Area_Filling/area_fill.cpp"
#include "./Garis_dan_Pesawat/pesawat_garis.cpp"
#include "./Moving_Plane/moving_plane.cpp"
#include "./Moving_Plane_Clipping/moving_plane_clipping.cpp"
#include "./Peta_ITB/petaITB.cpp"

using namespace std;
#define MOUSEFILE "/dev/input/mice\0"

// char getch() {
//       char c; // This function should return the keystroke
//       system("stty raw");    // Raw input - wait for only a single keystroke
//       system("stty -echo");  // Echo off
//       c = getchar();
//       system("stty cooked"); // Cooked input - reset
//       system("stty echo");   // Echo on - Reset
//       return c;
// }

int showMenu(int choosed) {
    system("clear");
    printf("=======================================================================\n");
    printf("Pilihan Menu :\n");
    switch (choosed) {
        case 1 : {
            printf("> 1. Font\n");
            printf("  2. Area Filling\n");
            printf("  3. Shooting Plane\n");
            printf("  4. Moving Plane\n");
            printf("  5. Moving Plane with Clipping\n");
            printf("  6. ITB Map\n");
            printf("  7. Crosshair\n");
        };
        break;
        case 2 : {
            printf("  1. Font\n");
            printf("> 2. Area Filling\n");
            printf("  3. Shooting Plane\n");
            printf("  4. Moving Plane\n");
            printf("  5. Moving Plane with Clipping\n");
            printf("  6. ITB Map\n");
            printf("  7. Crosshair\n");
        };
        break;
        case 3 : {
            printf("  1. Font\n");
            printf("  2. Area Filling\n");
            printf("> 3. Shooting Plane\n");
            printf("  4. Moving Plane\n");
            printf("  5. Moving Plane with Clipping\n");
            printf("  6. ITB Map\n");
            printf("  7. Crosshair\n");
        };
        break;
        case 4 : {
            printf("  1. Font\n");
            printf("  2. Area Filling\n");
            printf("  3. Shooting Plane\n");
            printf("> 4. Moving Plane\n");
            printf("  5. Moving Plane with Clipping\n");
            printf("  6. ITB Map\n");
            printf("  7. Crosshair\n");
        };
        break;
        case 5 : {
            printf("  1. Font\n");
            printf("  2. Area Filling\n");
            printf("  3. Shooting Plane\n");
            printf("  4. Moving Plane\n");
            printf("> 5. Moving Plane with Clipping\n");
            printf("  6. ITB Map\n");
            printf("  7. Crosshair\n");
        };
        break;
        case 6 : {
            printf("  1. Font\n");
            printf("  2. Area Filling\n");
            printf("  3. Shooting Plane\n");
            printf("  4. Moving Plane\n");
            printf("  5. Moving Plane with Clipping\n");
            printf("> 6. ITB Map\n");
            printf("  7. Crosshair\n");
        };
        break;
        case 7 : {
            printf("  1. Font\n");
            printf("  2. Area Filling\n");
            printf("  3. Shooting Plane\n");
            printf("  4. Moving Plane\n");
            printf("  5. Moving Plane with Clipping\n");
            printf("  6. ITB Map\n");
            printf("> 7. Crosshair\n");
        };
        break;
    };

    printf("Press x to exit the program\n");
    printf("=======================================================================\n");
    char input = getch();
    int output;
    switch(input) {
        case 's' : {
            choosed++;
            if (choosed == 8) choosed = 1;
            output = showMenu(choosed);
        }
        break;
        case 'w' : {
            choosed--;
            if (choosed == 0) choosed = 7;
            output = showMenu(choosed);
        }
        break;
        case '\r' : 
            output = choosed;
        break;
        case 'x' : {
            output = -1;
        }
        break;
        default : {
            output = showMenu(choosed);
        }
    }

    return output;
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
void clearscreen() {
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0, timer = 0;
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

    for (y = 0; y < vinfo.yres - 10; y++) {
        for (x = 0; x < vinfo.xres; x++) {
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;
            if (*(fbp + location) != 0 &&
                *(fbp + location + 1) != 0 &&
                *(fbp + location + 2) != 255 &&
                *(fbp + location + 3) == 0) {
                *(fbp + location) = 0;        // Some blue
                *(fbp + location + 1) = 0;     // A little green
                *(fbp + location + 2) = 0;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
    }

    munmap(fbp, screensize);
    close(fbfd);
}

void mouse() {
    system("clear");
    int fd;
    struct input_event ie;
    unsigned char *ptr = (unsigned char*)&ie;
    unsigned char button, bLeft, bMiddle, bRight;
    char x, y;
    int absolute_x, absolute_y;

    if ((fd = open(MOUSEFILE, O_RDONLY | O_NONBLOCK)) == -1) {
        printf("Non Blocking %s open Erorr\n", MOUSEFILE);
        exit(EXIT_FAILURE);
    }
    printf("Right click to start coloring the screen\n");
    printf("Left+Right Click to Exit\n");
    printf("Press Left Click to change the color to red, green , or blue\n");
    while(1) {
        if(read(fd, &ie, sizeof(struct input_event)) != -1) {
            clearscreen();
            button = ptr[0];
            bLeft = (button & 0x1) > 0;
            bMiddle = (button & 0x4) > 0;
            bRight = (button & 0x2) > 0;
            x = (char) ptr[1];
            y = (char) ptr[2];
            if(bLeft & bRight) {
                printf("EXIT\n");
                break;
            }

            absolute_x += x;
            absolute_y -= y;

            if (bRight == 1) {
                absolute_x = 0;
                absolute_y = 0;
            }

            if (bLeft == 1) {
                drawPixel(absolute_x, absolute_y, 255,0, 0);
            }


            // printf("X%i Y%i\n", absolute_x, absolute_y);
            drawPixel(absolute_x+2, absolute_y, 255,255, 255);
            drawPixel(absolute_x+3, absolute_y, 255,255, 255);
            drawPixel(absolute_x+4, absolute_y, 255,255, 255);
            drawPixel(absolute_x-2, absolute_y, 255,255, 255);
            drawPixel(absolute_x-3, absolute_y, 255,255, 255);
            drawPixel(absolute_x-4, absolute_y, 255,255, 255);

            drawPixel(absolute_x, absolute_y+2, 255,255, 255);
            drawPixel(absolute_x, absolute_y+3, 255,255, 255);          
            drawPixel(absolute_x, absolute_y+4, 255,255, 255);
            drawPixel(absolute_x, absolute_y-2, 255,255, 255);
            drawPixel(absolute_x, absolute_y-3, 255,255, 255);          
            drawPixel(absolute_x, absolute_y-4, 255,255, 255);
            
            fflush(stdout);
        }
    }
    close(fd);
}
void runChoice(int choosed) {
    switch (choosed) {
        case -1 : {
            printf("Thank you for using our Application\n");
        }
        break;
        case 1 : {
            Font();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
        case 2 : {
            AreaFill();
            getch();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
        case 3 : {
            PesawatGaris();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
        case 4 : {
            MovingPlane();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
        case 5 : {
            MVClipping();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
        case 6 : {
            PetaITB();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
        case 7 : {
            mouse();
            usleep(1000);
            runChoice(showMenu(1));
        };
        break;
    }
}
int main() {
    char choosed;
    
    choosed = showMenu(1);
    runChoice(choosed);
    
    return 0;
}