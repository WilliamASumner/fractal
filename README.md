# Fractals
Fractals are some of the coolest things to come out of mathematics ever in my humble opinion and so I wanted to write something that could visualize them.
This repo includes two versions of the same program.
The first is a fully featured fractal viewer that can show a number of different fractal types such as the burning ship, the mandelbrot set, and others.
The second is my attempt to make it as small as possible while retaining basic functionality.

# To Use
Compile either of the .c files, neither of them need any special libraries. The obfuscated version works with a fixed screen size so that is something to be aware of.
The other version works perfectly fine and resizes to the terminal window size. Running this version with no inputs gives a usage message, too.

# Interactive Mode
By choosing a fractal and then adding a -i flag, the program will allow you to enter a series of WASD (for moving), ZX (for zooming), or a space followed by an enter
to move around and explore the fractal in depth. I thought it would be best to not include any additional libraries that I didn't need, so that's why you need to hit
enter to use it. It has the added benefit of allowing you to fine tune your movement and even writing down a series of movements into a file and then read it!
Lastly, with the f key you can save your screen into a high resolution ppm named "output.ppm".
Enjoy!
