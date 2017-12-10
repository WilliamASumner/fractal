#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>

#define WHITE   "\x1b[37m"
#define RED     "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define CYAN    "\x1b[36m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

#define ITERMAX 1000
#define MINISPIRALS 0
#define ELEPHANT 1
#define TENTACLES 2
#define CENTER 3

// usage for command line arguments ./fractal.out PLACE WIDTH HEIGHT ITERMAX

// (-0.71359190450499,-0.21478559053)     - Mini Spirals
// (0.2730000018,-.00700000235)            -Elephant Valley
// (0.1002,0.8383)                        - Double Scepter
// (-0.274,0.482)                        - Quad Spiral
// (0.75, 0.1)                             - Seahorse Valley
// (0.088,0.654)                        - Triple Spiral
// (1.108,0.230)                         - Scepter Varient
// (0.1592,-1.0317) - Another Mandlebrot

int main(int argc, char *argv[]) {
    double places[10][2];
    double ship[10][2];
    FILE *linescols;
    int lines = 0, cols = 0;
    linescols = popen("tput cols;tput lines","r");
    fscanf(linescols,"%d %d",&cols,&lines);
    pclose(linescols);
    double aspectRatio = (double) cols/lines;

    int spot,width=cols/1.2,height=lines/1.2;
    unsigned long wait = 50000;
    places[0][0] = -0.21478559053;
    places[0][1] = 0.71359190450499;
    places[1][0] = 0.2730000018;
    places[1][1] = .00700000235;
    places[2][0] = 0.10119090000003;
    places[2][1] = -0.6192;
    places[3][0] = 0;
    places[3][1] = 0;
    places[4][0] = -1.68;
    places[4][1] = 0.0;
    places[5][0] = -0.183995;
    places[5][1] = 0.95;
    char colors[7][9] = {WHITE,CYAN,BLUE,GREEN,YELLOW,RED,MAGENTA};
    char syms[] = "+-.o0&^=z@ua";
    int symLength= 12;
    int itermax = ITERMAX;
    float startt = 3.0;
    float endt = 1e-14;
    int mode = 1;
    switch (argc) {
        case 3:
            {
                itermax = atoi(argv[2]);
            }
        case 2:
        {
            if (strcmp(argv[1],"elephant")==0) {
                spot = ELEPHANT;
            }
            else if (strcmp(argv[1],"spirals")==0) {
                spot = MINISPIRALS;
            }
            else if (strcmp(argv[1],"tentacles")==0) {
                spot = TENTACLES;
            }
            else if (strcmp(argv[1],"still")==0) {
               spot = MINISPIRALS;
               endt = 1e-5;
               startt = endt;
            }
            else if (strcmp(argv[1],"rotated")==0) {
               spot = MINISPIRALS;
               mode = 2;
            }
            else if (strcmp(argv[1],"ship")==0) {
               spot = 4;
               mode = 3;
            }
            else if (strcmp(argv[1],"random")==0) {
                spot = 5;
                mode = 4;
            }
            else if (strcmp(argv[1],"carp")==0) {
                spot = 5;
                mode = 5;
            }
            else
                spot = CENTER;
            break;
        }
        default:
            {
                printf("usage: ./prog.out fractal [location]\n");
                return 0;
            }
    }

    puts("\033[2J");
    for (double t = startt; t >= endt; t*=0.9) {
        puts("\033[1H");
        for (int i = 0; i < height; i++) {
            for (int k = 0; k < abs(cols-width)/2;k++)
            {
                printf(" ");
            }
            for (int j = 0; j < width;j++) {
                double cReal = ((j - width*0.5)*t/width)*aspectRatio+places[spot][0];
                double cImag = ((i - height*0.5)*t/height)*aspectRatio+places[spot][1];
              //  printf("cr: %lf ci: %lf\n",cReal,cImag);
                double x = 0.0, y = 0.0, xtemp;
                int iter = 0;
                double complex z0 = 0.0 + 0.0 * I;
                double complex c = cReal + cImag * I;
                switch (mode)
                {
                default:
                    {
                        while(cabs(z0)< 2 && iter < itermax) {
                            z0 = (z0*z0) + c;
                            iter++;
                        }
                        break;
                    }

                case 5:
                    {
                        while (creal(z0) > 0 && cimag(z0) > 0 && iter < ITERMAX)
                        {
                            if ((int)creal(z0) %  3 < 1 && (int)cimag(z0) % 3 < 1)
                                iter = ITERMAX;
                            z0 = creal(z0)/3 + cimag(z0)/3*I;
                            iter++;
                        }
                    }
                    break;
                case 4:
                    {
                        while (cabs(z0) < 2 && iter < itermax) {
                            z0 = cimag(z0)*cimag(z0) + creal(z0)*I;
                            z0 = z0*z0*z0 + c;
                            iter++;
                        }
                        break;
                    }
                case 3:
                    {
                        while (cabs(z0) < 2 && iter < itermax)
                        {
                            z0 = fabs(creal(z0)) + fabs(cimag(z0))*I;
                            z0 = z0*z0 + c;
                            iter++;
                        }
                        break;
                    }

                case 2:
                    {
                        while (cabs(z0) < 2 && iter < itermax)
                        {
                            z0 = z0*z0 + c*(1+-0.8*I); // this rotates the mandlebrot
                            iter++;
                        }
                        break;
                    }

                }
                if (iter < ITERMAX)
                    putchar(syms[iter%symLength]);//33+iter%13
                else
                    putchar(' ');
            }
            putchar('\n');
        }
        usleep(wait);
    }
    return 0;
}
