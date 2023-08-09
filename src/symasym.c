#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define LINE_THICKNESS 3

// structure for representing a pixel
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

// constants for white and black pixels
const Pixel WHITE_PIXEL = {255, 255, 255};
const Pixel BLACK_PIXEL = {0, 0, 0};

void createImage(long long seed, int isHorizontal, int isSymmetric, char* namePreffix, int size);
void createCanvas(int size, Pixel canvas[][size]);
void drawLine(int size, Pixel canvas[][size], int isHorizontal, int isSymmetric);
void createBitmapHeaders(FILE *file, int size);

int main(int argc, char *argv[]) {
  // declare file-wise parameters
  long long nSeed = -1;
  int fIsHorizontal = -1;
  int fIsSymmetric = -1;

  // declare batch-wise parameters
  int nImageSize = 32;
  int nNumberOfFiles = 1;
  char sFileName[64] = {};
  
  // get parameters from arguments
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-a") == 0) fIsSymmetric = 0;
    if (strcmp(argv[i], "-s") == 0) fIsSymmetric = 1;
    if (strcmp(argv[i], "-v") == 0) fIsHorizontal = 0;
    if (strcmp(argv[i], "-h") == 0) fIsHorizontal = 1;
    if (strcmp(argv[i], "-o") == 0) strcpy(sFileName, argv[i + 1]);
    if (strcmp(argv[i], "-seed") == 0) nSeed = atoi(argv[i + 1]);
    if (strcmp(argv[i], "-size") == 0) nImageSize = atoi(argv[i + 1]);
    if (strcmp(argv[i], "-n") == 0) nNumberOfFiles = atoi(argv[i + 1]);
  }

  for (int i = 0; i < nNumberOfFiles; i++) {
    createImage(nSeed, fIsHorizontal, fIsSymmetric, sFileName, nImageSize); 
  }
}

void createImage(long long seed, int isHorizontal, int isSymmetric, char* namePreffix, int size) {
  // set default seed
  if (seed == -1) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // get number of microseconds since January 1, 1970
    seed = (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
  }

  // set default flags for orientation and symmetry
  if (isHorizontal == -1) isHorizontal = (seed % 4 < 2) ? 1 : 0;
  if (isSymmetric == -1) isSymmetric = seed % 2;

  // generate filename
  char name[64] = {};
  strcpy(name, namePreffix);
  if (isHorizontal == 1) strcat(name, "h");
  else strcat(name, "v");
  if (isSymmetric == 1) strcat(name, "s");
  else strcat(name, "a");
  char str[32];
  sprintf(str, "%lli", seed);
  strcat(name, str);
  strcat(name, ".bmp");

  srand(seed); // set seed to the random function
  Pixel image[size][size]; // create a bits matrix
  createCanvas(size, image); // get an empty canvas to draw the line
  drawLine(size, image, isHorizontal, isSymmetric); // draw the line

  FILE *file = fopen(name, "wb");
  if (file == NULL) {
    printf("Error opening file\n");
  }
  createBitmapHeaders(file, size);

  // write image data to file
  for (int j = 0; j < size; j++) {
    for (int i = 0; i < size; i++) {
      fwrite(&image[i][j].blue, sizeof(uint8_t), 1, file);  // B
      fwrite(&image[i][j].green, sizeof(uint8_t), 1, file);  // G
      fwrite(&image[i][j].red, sizeof(uint8_t), 1, file);  // R
    }
  }
  fclose(file);
}

void createCanvas(int size, Pixel canvas[][size]) {
    // fill canvas with white pixels
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            canvas[i][j] = WHITE_PIXEL;
        }
    }
    // draw the black frame with horizontal and vertical lines of width 1
    for (int i = 0; i < size; i++) {
        canvas[i][0] = BLACK_PIXEL;
        canvas[i][size - 1] = BLACK_PIXEL;
    }
    for (int j = 0; j < size; j++) {
        canvas[0][j] = BLACK_PIXEL;
        canvas[size - 1][j] = BLACK_PIXEL;
    }
}

void drawLine(int size, Pixel canvas[][size], int isHorizontal, int isSymmetric){
  // declaring variables for coordinates, motion in corresponding directions, and number of iterations
  int x; int y; int xMotion; int yMotion; int nIter;

  // number of iterations depends on whether the image is symmetric
  if (isSymmetric) nIter = (int) (size / 2) + size % 2; // half size rounded to upper int
  else nIter = size;

  // generating a random coordinate for the starting point
  int startPoint = (int) (LINE_THICKNESS / 2) + rand() % (size - LINE_THICKNESS);
  
  if (isHorizontal == 1) { // horizontal line
    x = 0; y = startPoint; // initial position
    xMotion = 1;  // motion in x direction is uniform
    for (int i = 0; i < nIter; i++) {
        // y is the central coordinate of the line is of specified thickness
        int yInit = y - (int) (LINE_THICKNESS / 2);
        int yFin = yInit + LINE_THICKNESS;
        for (int yCurrent = yInit; yCurrent < yFin; yCurrent++) {
            canvas[x][yCurrent] = BLACK_PIXEL; // painting the pixel to black
            if (isSymmetric) { canvas[size-x-1][yCurrent] = BLACK_PIXEL; } // painting the symmetrical pixel
        }
        yMotion = rand() % (2 * LINE_THICKNESS + 1) - LINE_THICKNESS;
        if (y + yMotion <= 0 || y + yMotion >= size-1) yMotion *= -1; // inverting the direction to stay inside
        x += xMotion; y += yMotion;  // updating coordinates         
    }
  } else { // vertical line
    x = startPoint; y = 0;  // initial position
    yMotion = 1;  // motion in y direction is uniform
    for (int i = 0; i < nIter; i++) {
        // x is the central coordinate of the line is of specified thickness
        int xInit = x - (int) (LINE_THICKNESS / 2);
        int xFin = xInit + LINE_THICKNESS;
        for (int xCurrent = xInit; xCurrent < xFin; xCurrent++) {
            // painting the pixel to black
            canvas[xCurrent][y] = BLACK_PIXEL;  // painting the pixel to black
            if (isSymmetric) { canvas[xCurrent][size-y-1] = BLACK_PIXEL; } // painting the symmetrical pixel
        }
        xMotion = rand() % (2 * LINE_THICKNESS + 1) - LINE_THICKNESS;
        if (x + xMotion <= 0 || x + xMotion >= size-1) xMotion *= -1; // inverting the direction to stay inside
        x += xMotion; y += yMotion;  // updating coordinates         
    }
  }
}

void createBitmapHeaders(FILE *file, int size) {
  // file header (14 bytes)
  uint16_t bfType = 0x4D42;  // "BM"
  uint32_t bfSize = 54 + 3 * size * size;  // Total file size
  uint16_t bfReserved1 = 0;
  uint16_t bfReserved2 = 0;
  uint32_t bfOffBits = 54;  // Offset to image data

  // info header (40 bytes)
  uint32_t biSize = 40;  // Size of BITMAPINFOHEADER structure
  int32_t biWidth = size;
  int32_t biHeight = size;   // Negative height for top-down image
  uint16_t biPlanes = 1;       // Number of color planes
  uint16_t biBitCount = 24;    // Bits per pixel (RGB)
  uint32_t biCompression = 0;  // No compression
  uint32_t biSizeImage = 3 * size * size;  // Image data size
  int32_t biXPelsPerMeter = 0;
  int32_t biYPelsPerMeter = 0;
  uint32_t biClrUsed = 0;       // Number of colors used
  uint32_t biClrImportant = 0;  // All colors are important

  fwrite(&bfType, sizeof(bfType), 1, file);
  fwrite(&bfSize, sizeof(bfSize), 1, file);
  fwrite(&bfReserved1, sizeof(bfReserved1), 1, file);
  fwrite(&bfReserved2, sizeof(bfReserved2), 1, file);
  fwrite(&bfOffBits, sizeof(bfOffBits), 1, file);

  fwrite(&biSize, sizeof(biSize), 1, file);
  fwrite(&biWidth, sizeof(biWidth), 1, file);
  fwrite(&biHeight, sizeof(biHeight), 1, file);
  fwrite(&biPlanes, sizeof(biPlanes), 1, file);
  fwrite(&biBitCount, sizeof(biBitCount), 1, file);
  fwrite(&biCompression, sizeof(biCompression), 1, file);
  fwrite(&biSizeImage, sizeof(biSizeImage), 1, file);
  fwrite(&biXPelsPerMeter, sizeof(biXPelsPerMeter), 1, file);
  fwrite(&biYPelsPerMeter, sizeof(biYPelsPerMeter), 1, file);
  fwrite(&biClrUsed, sizeof(biClrUsed), 1, file);
  fwrite(&biClrImportant, sizeof(biClrImportant), 1, file);
}
