//
//  GUS.cpp
//  helloGL
//
//  Created by tomato on 1/30/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include <stdio.h>
#include "GUS.h"



char* GUS::readShader(string file) {
    FILE *fptr;
    long length;
    char *buf;
    
    fptr = fopen( file.c_str() , "r" ); 
    if ( !fptr ) {
    	printf("FAILED TO READ SHADER SOURCE!!\n");
        return NULL;
    }
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);  
    fread(buf, length, 1, fptr); 
    fclose(fptr); 
    buf[length] = 0;
    return buf;
}

GLboolean  GUS::loadTgaImage(const char* filename, GUS::GLtgaimage_t* tgaimage) {
	FILE*				file;
	GLubyte				type;
	GLubyte				bitsPerPixel;
    
	if ( !filename || !tgaimage ) {
        printf("[GUS][loadTgaImage] FAILED TO LOAD IMAGE %s\n", filename);
		return GL_FALSE;
	}
    
	file = fopen(filename, "rb");
	if ( !file ) {
        printf("[GUS][loadTgaImage] FAILED TO LOAD IMAGE %s\n", filename);
        return GL_FALSE;
	}
 	fseek(file, 2, SEEK_CUR);
 	fread(&type, 1, 1, file);
	if ( type != 2  &&  type != 10  &&  type != 11 ) {
		fclose(file);
		return GL_FALSE;
	}
 	fseek(file, 9, SEEK_CUR);
 	fread(&tgaimage->width, 2, 1, file);
 	fread(&tgaimage->height, 2, 1, file);
	fread(&bitsPerPixel, 1, 1, file);
	if ( bitsPerPixel != 8  &&  bitsPerPixel != 24  &&  bitsPerPixel != 32 ) {
		fclose(file);
		return GL_FALSE;
	} else {
		tgaimage->format = ALPHA;
		if ( bitsPerPixel == 24 ) {
			tgaimage->format = RGB;
		}
		else if ( bitsPerPixel == 32 ) {
			tgaimage->format = RGBA;
		}
	}
    
	fseek(file, 1, SEEK_CUR);
	tgaimage->data = (GLubyte*)malloc(tgaimage->width*tgaimage->height*bitsPerPixel/8);
    
	if (!tgaimage->data) {
		fclose(file);
		return GL_FALSE;
	}
    
	if (type == 2) {
		fread(tgaimage->data, 1, tgaimage->width*tgaimage->height*bitsPerPixel/8, file);
	} else {
		GLuint pixelsRead = 0;
        
		while ( pixelsRead < (GLuint)tgaimage->width*tgaimage->height ) {
			GLubyte amount;
			fread(&amount, 1, 1, file);
			if ( amount & 0x80 ) {
				GLuint			i;
				GLuint			k;
				amount&=0x7F;
				amount++;
				fread(&tgaimage->data[pixelsRead*bitsPerPixel/8], 1, bitsPerPixel/8, file);
				for (i = 1; i < amount; i++) {
					for (k = 0; k < (GLuint)bitsPerPixel/8; k++) {
						tgaimage->data[(pixelsRead+i)*bitsPerPixel/8+k] = tgaimage->data[pixelsRead*bitsPerPixel/8+k];
					}
				}
			} else {
				amount&=0x7F;
				amount++;
				fread(&tgaimage->data[pixelsRead*bitsPerPixel/8], 1, (GLuint)amount*bitsPerPixel/8, file);
			}
			pixelsRead += amount;
		}
	}
	if ( bitsPerPixel == 24 || bitsPerPixel == 32 ) {
        GUS::swapColorChannel(tgaimage->width, tgaimage->height, tgaimage->format, tgaimage->data);
	}
	fclose(file);
    //printf("[GUS][loadTgaImage] SUCCESS TO LOAD IMAGE\n");
	return GL_TRUE;
}

GLvoid GUS::swapColorChannel(GLuint width, GLuint height, GLenum format, GLubyte* data) {
	GLuint i;
	GLubyte temp;
	GLubyte bytesPerPixel = 3;
	if ( format == RGBA ) {
		bytesPerPixel = 4;
	}
	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for ( i = 0  ;  i < width*height*bytesPerPixel  ;  i+=bytesPerPixel ) {
		temp = data[i];
		data[i] = data[i+2];
		data[i+2] = temp;
	}
}

GLvoid GUS::destroyTgaImage(GUS::GLtgaimage_t* tgaimage) {
	if (!tgaimage) {
		return;
	}
	if (tgaimage->data) {
		free(tgaimage->data);
		tgaimage->data = 0;
	}
	tgaimage->width = 0;
	tgaimage->height = 0;
	tgaimage->format = 0;
}
