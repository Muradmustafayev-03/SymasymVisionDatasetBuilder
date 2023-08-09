#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define LINE_THICKNESS 3

void createImage(long long seed, int isHorizontal, int isSymmetric, char* namePreffix, int size);
void createCanvas(int size, unsigned char canvas[][size][3]);
void drawLine(int size, unsigned char canvas[][size][3], int isHorizontal, int isSymmetric);
void createBitmapHeaders(FILE *file, int size);

int main(int argc, char *argv[]) {
  // declaring file-wise parameters
  long long nSeed = -1;
  int fIsHorizontal = -1;
  int fIsSymmetric = -1;

  // declaring batch-wise parameters
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
  // setting default seed
  if (seed == -1) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // getting number of microseconds since January 1, 1970
    seed = (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
  }

  // setting default flags for orientation and symmetry
  if (isHorizontal == -1) isHorizontal = (seed % 4 < 2) ? 1 : 0;
  if (isSymmetric == -1) isSymmetric = seed % 2;

  // generating filename
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

  srand(seed); // setting seed to the random function
  uint8_t image[size][size][3]; // Creating a bits matrix
  createCanvas(size, image); // getting an empty canvas to draw the line
  drawLine(size, image, isHorizontal, isSymmetric); // drawing the line

  FILE *file = fopen(name, "wb");
  if (file == NULL) {
    printf("Error opening file\n");
  }
  createBitmapHeaders(file, size);

  // Write image data to file
  for (int j = 0; j < size; j++) {
    for (int i = 0; i < size; i++) {
      fwrite(&image[i][j][2], sizeof(uint8_t), 1, file);  // B
      fwrite(&image[i][j][1], sizeof(uint8_t), 1, file);  // G
      fwrite(&image[i][j][0], sizeof(uint8_t), 1, file);  // R
    }
  }
  fclose(file);
}

void createCanvas(int size, unsigned char canvas[][size][3]) {
  // Filling canvas with white
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      canvas[i][j][0] = 255;  // R
      canvas[i][j][1] = 255;  // G
      canvas[i][j][2] = 255;  // B
    }
  }

  // Drawing the black frame with vertical and horizontal lines of width 1
  for (int i = 0; i < size; i++) {
    canvas[i][0][0] = 0;  // R
    canvas[i][0][1] = 0;  // G
    canvas[i][0][2] = 0;  // B

    canvas[i][size - 1][0] = 0;  // R
    canvas[i][size - 1][1] = 0;  // G
    canvas[i][size - 1][2] = 0;  // B
  }

  for (int j = 0; j < size; j++) {
    canvas[0][j][0] = 0;  // R
    canvas[0][j][1] = 0;  // G
    canvas[0][j][2] = 0;  // B

    canvas[size - 1][j][0] = 0;  // R
    canvas[size - 1][j][1] = 0;  // G
    canvas[size - 1][j][2] = 0;  // B
  }
}

void drawLine(int size, unsigned char canvas[][size][3], int isHorizontal, int isSymmetric) {
  // declaring variables for coordinates, motion in corresponding directions, and number of iterations
  int x; int y; int xMotion; int yMotion; int nIter;

  // number of iterations depends on whether the image is symmetric
  if (isSymmetric) nIter = (int) (size / 2) + size % 2; // half size rounded to upper int
  else nIter = size;

  // generating a random coordinate for the starting point
  int startPoint = (int) (LINE_THICKNESS / 2) + rand() % (size - LINE_THICKNESS);
  
  if (isHorizontal == 1) { // vertical line
    x = startPoint; y = 0; // initial position
    yMotion = 1;  // motion in y direction is uniform
    for (int i = 0; i < nIter; i++) {
        // x is the central coordinate of the line is of specified thickness
        int xInit = x - (int) (LINE_THICKNESS / 2);
        int xFin = xInit + LINE_THICKNESS;
        for (int xCurrent = xInit; xCurrent < xFin; xCurrent++) {
            // painting the pixel to black
            canvas[y][xCurrent][0] = 0;
            canvas[y][xCurrent][1] = 0;
            canvas[y][xCurrent][2] = 0;

            // painting the symmetrical pixel
            if (isSymmetric) {
                canvas[size-y-1][xCurrent][0] = 0;
                canvas[size-y-1][xCurrent][1] = 0;
                canvas[size-y-1][xCurrent][2] = 0;
            }
        }
        xMotion = rand() % (2 * LINE_THICKNESS + 1) - LINE_THICKNESS;
        if (x + xMotion <= 0 || x + xMotion >= size-1) xMotion *= -1; // inverting the direction to stay inside
        x += xMotion; y += yMotion;  // updating coordinates         
    }
  } else {
    x = 0; y = startPoint;  // initial position
    xMotion = 1;  // motion in x direction is uniform
    for (int i = 0; i < nIter; i++) {
        // y is the central coordinate of the line is of specified thickness
        int yInit = y - (int) (LINE_THICKNESS / 2);
        int yFin = yInit + LINE_THICKNESS;
        for (int yCurrent = yInit; yCurrent < yFin; yCurrent++) {
            // painting the pixel to black
            canvas[yCurrent][x][0] = 0;
            canvas[yCurrent][x][1] = 0;
            canvas[yCurrent][x][2] = 0;

            // painting the symmetrical pixel
            if (isSymmetric) {
                canvas[yCurrent][size-x-1][0] = 0;
                canvas[yCurrent][size-x-1][1] = 0;
                canvas[yCurrent][size-x-1][2] = 0;
            }
        }
        yMotion = rand() % (2 * LINE_THICKNESS + 1) - LINE_THICKNESS;
        if (y + yMotion <= 0 || y + yMotion >= size-1) yMotion *= -1; // inverting the direction to stay inside
        x += xMotion; y += yMotion;  // updating coordinates         
    }
  }
}

void createBitmapHeaders(FILE *file, int size) {
  // File header (14 bytes)
  uint16_t bfType = 0x4D42;  // "BM"
  uint32_t bfSize = 54 + 3 * size * size;  // Total file size
  uint16_t bfReserved1 = 0;
  uint16_t bfReserved2 = 0;
  uint32_t bfOffBits = 54;  // Offset to image data

  // Info header (40 bytes)
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
