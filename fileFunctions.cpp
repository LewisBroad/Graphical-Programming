#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "fileFunctions.h"

// Returns a pointer to the bitmap image of the bitmap specified
//  by filename. Also returns the bitmap header information.
//  No support for 8-bit bitmaps.
unsigned char* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;				            // the file pointer
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap file header
	unsigned char* bitmapImage;			// bitmap image data
	int imageIdx = 0;		                        // image index counter
	unsigned char		tempRGB;		// swap variable
	// open filename in "read binary" mode
	errno_t err = fopen_s(&filePtr, filename, "rb");
	if (err != NULL)
		return NULL;
	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	// read in the bitmap image data
	fread(bitmapImage, sizeof(BYTE), bitmapInfoHeader->biSizeImage, filePtr);


	// swap the R and B values to get RGB since the bitmap color format is in BGR
	// Added -3 to stop Heap Error being generated when freeing bitmap  in VS2012
	for (imageIdx = 0; imageIdx < (bitmapInfoHeader->biSizeImage - 3); imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;

	}
	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}
