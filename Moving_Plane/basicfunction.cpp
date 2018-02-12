#ifndef BASICFUNCTION
#define BASICFUNCTION

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

class Basicfunction {
    public :
    static void clearscreen() {
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

                *(fbp + location) = 0;        // Some blue
                *(fbp + location + 1) = 0;     // A little green
                *(fbp + location + 2) = 0;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }

        munmap(fbp, screensize);
        close(fbfd);
    }
};

#endif