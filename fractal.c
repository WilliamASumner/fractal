#include <string.h> // stringcmp
#include <stdio.h> // all io
#include <stdlib.h> // for abs
#include <unistd.h> // for timing
#include <math.h> // for fabs
#include <complex.h> // complex numbers

#define WHITE   "\x1b[37m"
#define RED     "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define CYAN    "\x1b[36m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

#define ITERMAX 256
#define MINISPIRALS 0
#define ELEPHANT 1
#define TENTACLES 2
#define CENTER 3
#define SHIP 4
#define RANDOM 5
#define SPIRAL 6

    /* void write_ppm(int width, int height, int grid[177][47])
    {
        FILE * openFile;
        openFile = fopen("output.ppm","w");
        if(openFile != NULL)
        {
            fprintf(openFile,"P3\n%d %d\n%d\n",width,height,255); // print ppm header
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    fprintf(openFile,"%d %d %d ",grid[i][j]%256,grid[i][j]%256,grid[i][j]%256);
                }
                fprintf(openFile,"\n");
            }
        }
        else
        {
            printf("Error writing file");
            exit(1);
        }
        printf("Successfully wrote output.ppm\n");
    } */

int main(int argc, char *argv[]) {
    double places[10][2];
    double ship[10][2];
    FILE *linescols;
    int lines = 0, cols = 0;
    linescols = popen("tput cols;tput lines","r");
    fscanf(linescols,"%d %d",&cols,&lines);
    pclose(linescols);
    double aspectRatio = (double) lines/cols;

    int spot = CENTER,width=cols/1.2,height=lines/1.2, keyval = 0;
    int grid[177][47] = { 0 }; // set to max dimensions of the terminal
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
    places[6][0] = -2.052465525;
    places[6][1] = -0.007233462;
    char colors[7][9] = {WHITE,CYAN,BLUE,GREEN,YELLOW,RED,MAGENTA};
    char syms[] = ".oO0+-#@:c";//"+-.o0&^=z@ua";
    int symLength= 9;
    int itermax = ITERMAX;
    float startt = 8.0;
    float endt = 1e-14;
    double step = 0.9;
    int iter = 0;
    double complex o = 0.0 + 0.0*I;
    int mode = 1;
    int interactive = 0;

    switch (argc) {
        case 3:
            {
                if (strcmp(argv[2],"elephant")==0) {
                    spot = ELEPHANT;
                }
                else if (strcmp(argv[2],"minispirals")==0) {
                    spot = MINISPIRALS;
                }
                else if (strcmp(argv[2],"spiral")==0) {
                    spot = SPIRAL;
                }
                else if (strcmp(argv[2],"tentacles")==0) {
                    spot = TENTACLES;
                }
                else if (strcmp(argv[2],"still")==0) {
                   spot = MINISPIRALS;
                   endt = startt;
                }
                else if (strcmp(argv[2],"-i")==0) {
                    interactive = 1;
                    endt = 1e-20;
                }
                else {
                    printf("usage: ./prog.out fractal [location]\n");
                    printf("possible locations include elephant, minispirals, spiral, tentancles, still, or -i (for interactive)\n");
                    return 0;
                }
            }
        case 2:
        {
            if (strcmp(argv[1],"mandelbrot")==0)
            {
                spot = ELEPHANT;
                mode = 1;
            }
            else if (strcmp(argv[1],"rotated")==0) { // rotated mandelbrot
               spot = MINISPIRALS;
               mode = 2;
            }
            else if (strcmp(argv[1],"ship")==0) { // burning ship
               spot = SHIP;
               mode = 3;
            }
            else if (strcmp(argv[1],"random")==0) { // a random fractal I came up with
                spot = RANDOM;
                mode = 4;
            }
            else {
                printf("usage: ./prog.out fractal [location]\n");
                printf("possible options for fractals are mandelbrot, rotated, ship, or random.\n");
                return 0;
            }
            break;
        }
        default:
        {
            printf("usage: ./prog.out fractal [location]\n");
            return 0;
        }
    }

    puts("\033[2J"); // clear screen
    for (double t = startt; t >= endt; t*=step) {
        puts("\033[1H"); // go back to the top
        for (int i = 0; i < height; i++) {
            for (int k = 0; k < abs(cols-width)/2;k++)
            {
                printf(" ");
            }
            for (int j = 0; j < width;j++) {
                double cReal = ((j - width*0.5)*t/width)*aspectRatio+places[spot][0];
                double cImag = ((i - height*0.5)*t/height)*aspectRatio+places[spot][1];
                iter = 0;
                double complex z0 = 0.0 + 0.0 * I;
                double complex c = cReal + cImag * I;
                c += o; // add the origin for interactivity purposes
                switch (mode)
                {
                    case 4: // random fractal
                        {
                            while (cabs(z0) < 2 && iter < itermax) {
                                z0 = cimag(z0)*cimag(z0) + creal(z0)*I;
                                z0 = z0*z0*z0 + c;
                                iter++;
                            }
                            break;
                        }
                    case 3: // burning ship fractal
                        {
                            while (cabs(z0) < 2 && iter < itermax)
                            {
                                z0 = fabs(creal(z0)) + fabs(cimag(z0))*I;
                                z0 = z0*z0 + c;
                                iter++;
                            }
                            break;
                        }
                    case 2: // rotated mandelbrot
                        {
                            while (cabs(z0) < 2 && iter < itermax)
                            {
                                z0 = z0*z0 + c*(1+-0.8*I); // this rotates the mandlebrot
                                iter++;
                            }
                            break;
                        }
                    default: // regular mandelbrot
                        {
                            while(cabs(z0)< 2 && iter < itermax) {
                                z0 = (z0*z0) + c;
                                iter++;
                            }
                            break;
                        }
                }
                if (iter < itermax)
                    putchar(syms[iter%symLength]);
                else
                    putchar(' ');

                grid[i][j] = iter; // save the iters
            }
            putchar('\n');
        }
        if (interactive)
        {
            printf("\nlocation: %.9lf, %.9lf\nitermax: %d, scale %.8lf\n",creal(o)+places[spot][0],cimag(o)+places[spot][1],itermax,t);
            keyval = getchar(); // get the next character
            switch (keyval)
            {
                case 'a': // left
                    o -= t*0.1;
                    step = 1.0;
                    break;
                case 'w': // up
                    o -= t*0.1*I;
                    step = 1.0;
                    break;
                case 'd': // right
                    o += t*0.1;
                    step = 1.0;
                    break;
                case 's': // down
                    o += t*0.1*I;
                    step = 1.0;
                    break;
                case 'z': // zoom in
                    step = 0.5;
                    break;
                case 'x': // zoom out
                    step = 2;
                    break;
              /*  case 'f': // f
                    write_ppm(width,height,grid);
                    break; */
                case 'r':
                    step = 1.0;
                    t = startt;
                    break;
                case EOF:
                    return 0; // the program has been run on a file, quit
                    break;
                case 'i':
                    itermax += 100;
                    break;
                case 'k':
                    itermax -= 100;
                    break;
                default:
                    step = 1.0;
            }
        }
        else
        {
            usleep(wait);
        }
    }
    return 0;
}
