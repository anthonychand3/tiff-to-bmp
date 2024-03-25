//
//  main.c
//  tiff
//
//  Created by anthony chand on 6/1/18.
//  Copyright © 2018 anthony chand. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
/* BITMAP CONSTANTS */
const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;


/* TIF CONSTANTS*/
const int ImageWidth                 = 256;
const int ImageLength                = 257;
const int BitPerSample               = 258;
const int Compression                = 259;
const int PhotometricInterprestation = 262;
const int StripOffsets               = 273;
const int SamplePerPixel             = 277;
const int RowPerStrip                = 278;
const int StripByteCount             = 279;
const int XResolution                = 282;
const int YResolution                = 283;
const int ResolutionUnit             = 296;


int TifImageWidth                 = 0;
int TifImageLength                = 0;
int TifBitPerSample               = 0;
int TifCompression                = 0;
int TifPhotometricInterprestation = 0;
int TifStripOffsets               = 0;
int TifSamplePerPixel             = 0;
int TifRowPerStrip                = 0;
int TifStripByteCount             = 0;
int TifXResolution                = 0;
int TifYResolution                = 0;
int TifResolutionUnit             = 0;


u_int32_t StripOffSetCount = 0;

typedef struct {
    
    unsigned char R;
    unsigned char G;
    unsigned char B;
    
} RGB;

typedef struct{
    
    u_int16_t byte_order;
    u_int16_t tif_number;
    u_int32_t offset;
    
}HEADER;

typedef struct{
    
    u_int16_t tag;
    u_int16_t type;
    u_int32_t count;
    u_int32_t value_or_offset;
    
}DirectoryEntry;

typedef struct {
    
    unsigned char R;
    unsigned char G;
    unsigned char B;
    
}Pixel;


void generateBitmapImage( RGB **image , int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width);
unsigned char* createBitmapInfoHeader(int height, int width);
void freeStruct(RGB **image, int height);

int j =0;

int main(int argc, const char * argv[]) {
   

    //IFD
    u_int16_t num_of_directory;
    HEADER Header;
    DirectoryEntry directoryentry;
    Pixel pixel;
    RGB *  test;
    RGB ** image;
    
    FILE* TIF = fopen("/Users/anthonychand/Desktop/workspace/tiff/tiff/en.lproj/286045.tif", "rb");
    
    fread(&Header, sizeof(Header), 1, TIF);
    printf("%ld\n",sizeof(Header));
    printf("%x\n",Header.byte_order);
    printf("%x\n", Header.tif_number);
    printf("%x\n", Header.offset);
    
    fseek(TIF, Header.offset, SEEK_SET);
    fread(&num_of_directory, sizeof(u_int16_t), 1, TIF);
    printf("number of Directorys %d\n\n",num_of_directory);
    
    for (int i = 0; i < num_of_directory; i++ ){
        
        fread(&directoryentry, sizeof(directoryentry), 1, TIF);

        //printf("tag: %d\n", directoryentry.tag);
        //printf("type: %d\n", directoryentry.type);
        //printf("count: %d\n", directoryentry.count);
        //printf("value or offset: %d\n", directoryentry.value_or_offset);
        
        
        if (directoryentry.tag == ImageWidth){
            TifImageWidth = directoryentry.value_or_offset;
            printf("ImageWidth = %d\n\n", TifImageWidth);
        }
        if (directoryentry.tag == ImageLength){
            TifImageLength = directoryentry.value_or_offset;
            printf("ImageLength = %d\n\n",TifImageLength);
        }
        if (directoryentry.tag == BitPerSample){
            TifBitPerSample = directoryentry.value_or_offset;
            printf("BitPerSample = %d\n\n",directoryentry.count);
        
        }
        if (directoryentry.tag == Compression){
            TifCompression = directoryentry.value_or_offset;
            printf("Compression = %d\n\n",directoryentry.count);
        }
        if (directoryentry.tag == PhotometricInterprestation){
            TifPhotometricInterprestation = directoryentry.value_or_offset;
            printf("PhotometricInterprestation = %d\n\n",TifPhotometricInterprestation);
        }
        if (directoryentry.tag == StripOffsets){
            StripOffSetCount = directoryentry.count;
            TifStripOffsets = directoryentry.value_or_offset;
            printf("StripOffsets = %d\n\n", TifStripOffsets);
        }
        if (directoryentry.tag == SamplePerPixel){
            TifSamplePerPixel = directoryentry.value_or_offset;
            printf("SamplePerPixel = %d\n\n", TifSamplePerPixel);
        }
        if (directoryentry.tag == RowPerStrip){
            TifRowPerStrip = directoryentry.value_or_offset;
            printf("RowPerStrip = %d\n\n", TifRowPerStrip);
        }
        if (directoryentry.tag == StripByteCount){
            TifStripByteCount= directoryentry.value_or_offset;
            printf("StripByteCount = %d\n\n", TifStripByteCount);
        }
        if (directoryentry.tag == XResolution){
            TifXResolution = directoryentry.value_or_offset;
            printf("XResolution = %d\n\n", TifXResolution);
        }
        if (directoryentry.tag == YResolution){
            TifYResolution = directoryentry.value_or_offset;
            printf("YResolution = %d\n\n", TifYResolution);
        }
        if (directoryentry.tag == ResolutionUnit){
            TifResolutionUnit = directoryentry.value_or_offset;
            printf("ResolutionUnit = %d\n\n", TifResolutionUnit);
        }
    }
    
    test = (RGB *)malloc(sizeof(RGB) * (TifStripByteCount/3));
    printf(" current pointer %ld\n",ftell(TIF));
    fseek(TIF, TifStripOffsets, SEEK_SET);
     printf(" current pointer %ld\n",ftell(TIF));
    int i,j = 0;
    for(j=0; j< (TifStripByteCount/3); j++){
            fread(&pixel, sizeof(pixel), 1, TIF);
            test[j].R = pixel.R;
            test[j].G = pixel.G;
            test[j].B = pixel.B;
        
    }
    
    image = (RGB **)malloc(sizeof(RGB*) * (TifImageLength));
    for (i=0; i < TifImageLength; i++) {
        image[i] = (RGB *)malloc(sizeof(RGB) * (TifImageWidth));
    }
    
    
    i = 0;
    for (j=0; j< (TifStripByteCount/3); j++)
    {
        if((j % TifRowPerStrip) == 0)
        {
            image[i][j % TifRowPerStrip].R = test[j].R;
            image[i][j % TifRowPerStrip].G = test[j].G;
            image[i][j % TifRowPerStrip].B = test[j].B;
            
        }
        else
        {
            image[i][j % TifRowPerStrip].R = test[j].R;
            image[i][j % TifRowPerStrip].G = test[j].G;
            image[i][j % TifRowPerStrip].B = test[j].B;
        }
        if(((j % TifRowPerStrip)) >= (TifRowPerStrip-1)){
            i++;
        }
    }
    free(test);
    char* imageFileName = "/Users/anthonychand/Desktop/workspace/tiff/testing.bmp";
    generateBitmapImage(image,TifImageLength,TifImageWidth,imageFileName);
    freeStruct(image, TifImageLength);
    printf("done writing tif\n");
    return 0;
}



/********
 BITMAP SECTION
 *******/

void generateBitmapImage(RGB ** image , int height, int width, char* imageFileName){
    
    unsigned char* fileHeader = createBitmapFileHeader(height, width);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    FILE* imageFile = fopen(imageFileName, "wb");
    
    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);
    
    int i,j;
    for(i=0; i<height; i++){
        for(j=0; j<width; j++)
        {
            fwrite(&image[i][j].B, 1, 1, imageFile);
            fwrite(&image[i][j].G, 1, 1, imageFile);
            fwrite(&image[i][j].R, 1, 1, imageFile);
        }
    }
    fclose(imageFile);
}

unsigned char* createBitmapFileHeader(int height, int width){
    
    int fileSize = fileHeaderSize + infoHeaderSize + bytesPerPixel*height*width;
    static unsigned char fileHeader[fileHeaderSize] = {0};
    
    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);
    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[infoHeaderSize] = {0};
    
    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);
    return infoHeader;
}


void freeStruct(RGB **image, int height)
{
    int i;
    for(i = 0; i < height; i++)
    {
        free(image[i]);
    }
}
