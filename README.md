# SymasymVisionDatasetBuilder

## Introduction
symasym.c is a command-line program that generates bitmap images with symmetric or asymmetric lines. It provides flexibility in customizing various parameters such as line orientation, line symmetry, image size, and the number of
generated images. It can generate thousands of images within seconds, which
is very useful for creating datasets for Machine Learning tasks such as image
classification. This user manual will guide you through the installation, usage,
and available options of the symasym.c program.

## Compiling
symasym.c requires a C compiler to be installed on your system. Follow the
steps below to compile the program:
- Download the symasym.c source code file to your computer (it is located in src/ folder of this archive).
- Open a terminal or command prompt.
- Navigate to the directory where symasym.c is located.
- Run the following command to compile the program:
`$ gcc −o symasym symasym . c`
- Once the compilation is successful, an executable file named ”symasym”
will be generated in the same directory.

## Usage
symasym.c is a command-line program that accepts various options and arguments. The basic usage syntax is: `$ . / symasym`
or `$ . / symasym [options]`

## Options
- -a: Sets the line symmetry to asymmetric.
- -s: Sets the line symmetry to symmetric.
- -v: Sets the line orientation to vertical.
- -h: Sets the line orientation to horizontal.
- -o [filename]: Specifies the name prefix for the generated image file(s).
- -seed [value]: Sets the seed value for randomization. If not provided, a default seed based on the current time will be used.
- -size [value]: Specifies the size of the generated image(s). The value should be an integer representing the width and height of the image in pixels. The default size is 32.
- -n [value]: Specifies the number of images to generate. The value should be an integer. The default is 1

*Note: The options can be provided in any order.*

*Note: If not specified, the orientation and the symmetry will be dedused randomly from the seed.*

*Note: The same seeds (if the orientation and the symmetry parameters are not specified or the same) produce the same images.*

## Examples
Generate a single image with default options:
`$ . / symasym`

Create 10000 randomly vertical or horizontal or symmetric or asymmetric images with file name starting with ”xxx”:
`$ . / symasym −n 10000 −o xxx`

Creates an image ”hs12345678.bmp” of seed *12345678*:
`$ . / symasym −seed 1234567`

Generate a symmetric horizontal line with a specific seed:
`$ . / symasym −h −s −seed 9876`

## File Output
The generated images will be saved as BMP files in the same directory where
symasym.c is located. The file names will be based on the provided name prefix,
line orientation, line symmetry, and seed value.

![Horizontal asymmetric image](https://github.com/Muradmustafayev-03/SymasymVisionDatasetBuilder/blob/main/example_ha1684187644525988.bmp)
![Horizontal symmetric image](https://github.com/Muradmustafayev-03/SymasymVisionDatasetBuilder/blob/main/example_hs1684187644522845.bmp)
![Vertical symmetric image](https://github.com/Muradmustafayev-03/SymasymVisionDatasetBuilder/blob/main/example_vs1684187644519671.bmp)


![Vertical asymmetric image](https://github.com/Muradmustafayev-03/SymasymVisionDatasetBuilder/blob/main/example_va1684187673783142.bmp)

## Conclusion
The symasym.c program allows you to generate bitmap images with symmetric
or asymmetric lines. By customizing the various parameters, you can create
unique visual patterns. Experiment with different options to explore the possibilities of line generation.
