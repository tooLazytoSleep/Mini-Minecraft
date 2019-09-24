// Hogwarts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include"Cube.h"
#include"ractangular.h"
#include <stdio.h>
#include <stdlib.h>
#define BMP_Header_Length 54
using std::cout;   // dot.cpp needs this
#define checkImageWidth 64
#define checkImageHeight 64
#define _USE_MATH_DEFINES // for C++
#include <math.h>
#define ESC 27
using std::cout;   // dot.cpp needs this
#define WindowWidth  400
#define WindowHeight 400
#define checkImageWidth 64
#define checkImageHeight 64
#define _USE_MATH_DEFINES // for C++
GLfloat w = 1920;  // the size of the screen space, top-right: 800, 600
GLfloat h = 1080;  // bottom-left: 0, 0
float tans_x = 0;
float x = 30.5f, y = 1.5f; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move
// Camera direction
float lx = 0.5f, ly = 1.5f; // camera points initially along y-axis
float angle = 59.5; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
float isDragging = 2.5; // true when dragging
float xDragStart = 1.5; // records the x-coordinate when dragging starts
float yDragStart = 1.5;
float rote_x = 90;


void grab(void)
{
	FILE*    pDummyFile;
	FILE*    pWritingFile;
	GLubyte* pPixelData;
	GLubyte  BMP_Header[BMP_Header_Length];
	GLint    i, j;
	GLint    PixelDataLength;


	i = WindowWidth * 3;   
	while (i % 4 != 0)     
		++i;              

	PixelDataLength = i * WindowHeight;

	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	pDummyFile = fopen("dummy.bmp", "rb");
	if (pDummyFile == 0)
		exit(0);

	pWritingFile = fopen("grab.bmp", "wb");
	if (pWritingFile == 0)
		exit(0);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WindowWidth, WindowHeight,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);


	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);


	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);


	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}


int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}


GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint texture_ID = 0;
	GLint last_texture_ID;

	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;


	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}


	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}


	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; 
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	free(pixels);
	return texture_ID;
}

/* Á½¸öÎÆÀí¶ÔÏóµÄ±àºÅ
*/
GLuint texGround;
GLuint texWall;
GLuint texTop;
GLuint texSnow;
GLuint texDoor;
GLuint texDoorBar;
GLuint texWindow;
GLuint texCwall;
GLuint texSky;
GLuint texChwall;
GLuint texGubao;
GLuint texGubaod;
GLuint texGuTop;
GLuint texTree;
GLuint texTreeB;
GLuint texGudoor;
GLuint texGuW;
GLuint texSun;
GLuint texPerson;
float shift = 0;
void renderScene(void)
{
	// Clear color and depth buffers
	glClearColor(1.0, 1.0, 1.0, 1.0); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, 1,
		x + lx, y + ly, 1.0,
		0.0, 0.0, 1.0);
	glTranslatef(tans_x, 0.0f, shift-1.5f);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(rote_x, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texGround);
	for (float j = -80; j<150; j = j + 2)
	for (float i = -50; i<150; i = i + 2)
		BStone::BStone(j, -1, i);
	glBindTexture(GL_TEXTURE_2D, texSnow);
	for (float j = 36; j<60; j = j + 2)
	for (float i = 19; i<90; i = i + 2)
		BStone::BStone(j, -0.5, i);
	glBindTexture(GL_TEXTURE_2D, texCwall);
	//chengbao
	for (float p = -0.0; p<30; p = p + 2)
	for (float j = -8; j<36; j = j + 2)
		BStone::BStone(j, p, 18.5);
	for (float p = -0.0; p<30; p = p + 2)
	for (float j = 60; j<100; j = j + 2)
		BStone::BStone(j, p, 18.5);
	for (float p = 20; p<26; p = p + 2)
	for (float j = 30; j<60; j = j + 2)
		BStone::BStone(j, p, 18.5);
	for (float p = -2.0; p<30; p = p + 2)
	for (float j = 18.5; j<150; j = j + 2)
		BStone::BStone(-8, p, j);
	for (float p = -2.0; p<30; p = p + 2)
	for (float j = 18.5; j<150; j = j + 2)
		BStone::BStone(100, p, j);
	for (float j = -8; j < 36;j=j+6)
		BStone::BStone(j, 30, 18.5);
	for (float j = 60; j < 100; j = j + 6)
		BStone::BStone(j, 30, 18.5);
	for (float j = 18.5; j < 150; j = j + 6)
		BStone::BStone(100, 30, j);
	for (float j = 18.5; j < 150; j = j + 6)
		BStone::BStone(-8, 30, j);
	//in chengbao
	glBindTexture(GL_TEXTURE_2D, texGuTop);
	Ractangular::Ractangular(-8, 60, 95, 30);
	glBindTexture(GL_TEXTURE_2D, texGuW);
	for (float i = -13; i < 50; i = i + 20)
		BBStone::BBStone(-17, i, 140, 3);
	glBindTexture(GL_TEXTURE_2D, texGuTop);
	Ractangular::Ractangular(-8, 60, 145, 30);
	glBindTexture(GL_TEXTURE_2D, texGuW);
	for (float i = -13; i < 50; i = i + 20)
		BBStone::BBStone(-17, i, 90, 3);
	glBindTexture(GL_TEXTURE_2D, texGuTop);
	Ractangular::Ractangular(74, 60, 95, 30);
	glBindTexture(GL_TEXTURE_2D, texGuW);
	for (float i = -13; i < 50; i = i + 20)
		BBStone::BBStone(65, i, 140, 3);
	glBindTexture(GL_TEXTURE_2D, texGuTop);
	Ractangular::Ractangular(74, 60, 145, 30);
	glBindTexture(GL_TEXTURE_2D, texGuW);
	for (float i = -13; i < 50; i = i + 20)
		BBStone::BBStone(65, i, 90, 3);
	glBindTexture(GL_TEXTURE_2D, texGubao);
	for (int j = 115; j<140; j = j + 10)
	for (int i = -12; i<81; i = i + 10)
		BBStone::BBStone(i, -13, j, 2.5);
	for (int j = 3; j<60; j = j + 10)
	for (int i = 12; i<60; i = i + 10)
		BBStone::BBStone(i, j, 115, 2.5);
	for (int j = 3; j<60; j = j + 10)
	for (int i = 12; i<60; i = i + 10)
		BBStone::BBStone(i, j, 140, 2.5);
	//Chengbao door
	glBindTexture(GL_TEXTURE_2D, texTop);
	Ractangular::Ractangular(29.5, 11, 105, 40);
	glBindTexture(GL_TEXTURE_2D, texGubaod);
	for (int j = 108; j>87; j = j - 8)
	for (int i = -10; i<35; i = i + 8)
		BBStone::BBStone(22, i, j, 1);
	for (int j = 108; j>87; j = j - 8)
	for (int i = -10; i<35; i = i + 8)
		BBStone::BBStone(60, i, j, 1);
	glBindTexture(GL_TEXTURE_2D, texGudoor);
	BBStone::BBStone(17, -14, 111, 4);
	BBStone::BBStone(17, -5.3, 111.2, 4);
	glBindTexture(GL_TEXTURE_2D, texWindow);
	for (int i = 23; i < 65; i = i + 20)
	BBStone::BBStone(-1, i, 89.8, 1);
	for (int i = 23; i < 65; i = i + 20)
		BBStone::BBStone(81, i, 89.8, 1);
	glBindTexture(GL_TEXTURE_2D, texTop);
	for (float p = 0.5; p<30; p = p + 2)
	for (float j = -5; j<36; j = j + 2)
		Stone::Stone(j, p, 18.25);
	for (float p = 0.5; p<30; p = p + 2)
	for (float j = 62; j<100; j = j + 2)
		Stone::Stone(j, p, 18.25);
	glBindTexture(GL_TEXTURE_2D, texTop);
	//house top
	for (int i = 9.75; i<20; i = i + 1)
	for (float j = 10; j <= 20; j = j + 1)
		Ractangular::Ractangular(j, 9, i, 1);
	for (int i = 10.25; i<19.5; i = i + 1)
	for (float j = 10.5; j <= 19.5; j = j + 1)
		Ractangular::Ractangular(j, 9.5, i, 1);
	for (int i = 10.75; i<19.0; i = i + 1)
	for (float j = 11; j <= 19; j = j + 1)
		Ractangular::Ractangular(j, 10, i, 1);
	for (int i = 11.25; i<18.5; i = i + 1)
	for (float j = 11.5; j <= 18.5; j = j + 1)
		Ractangular::Ractangular(j, 10.5, i, 1);
	for (int i = 11.75; i<18; i = i + 1)
	for (float j = 12; j <= 18; j = j + 1)
		Ractangular::Ractangular(j, 11, i, 1);
	for (int i = 12.25; i<17.5; i = i + 1)
	for (float j = 12.5; j <= 17.5; j = j + 1)
		Ractangular::Ractangular(j, 11.5, i, 1);
	for (int i = 12.75; i<17; i = i + 1)
	for (float j = 13; j <= 17; j = j + 1)
		Ractangular::Ractangular(j, 12, i, 1);
	for (int i = 13.25; i<16.5; i = i + 1)
	for (float j = 13.5; j <= 16.5; j = j + 1)
		Ractangular::Ractangular(j, 12.5, i, 1);
	for (int i = 13.75; i<16; i = i + 1)
	for (float j = 14; j <= 16; j = j + 1)
		Ractangular::Ractangular(j, 13, i, 1);
	for (int i = 14.25; i<15.5; i = i + 1)
	for (float j = 14.5; j <= 15.5; j = j + 1)
		Ractangular::Ractangular(j, 13.5, i, 1);
	for (int i = 14.75; i<15; i = i + 1)
	for (float j = 15; j <= 15; j = j + 1)
		Ractangular::Ractangular(j, 14, i, 1);

	glBindTexture(GL_TEXTURE_2D, texTop);
	//house decorate
	for (float j = 3; j<9; j = j + 1)
		Ractangular::Ractangular(20, j, 9.75, 1);
	for (float j = 3; j<9; j = j + 2)
		Ractangular::Ractangular(10, j, 9.75, 1);
	for (float i = 3.5; i<9.5; i = i + 0.5)
		Stone::Stone(9.75, i, 9.75);
	for (float i = 3.5; i<9.5; i = i + 0.5)
		Stone::Stone(20.25, i, 9.75);
	//House bottom
	for (float j = 7; j<23.5; j = j + 0.5)
	for (float i = 7; i<19; i = i + 0.5)
		Stone::Stone(j, 0.5, i);
	for (float j = 7.5; j<23; j = j + 0.5)
	for (float i = 7.5; i<19; i = i + 0.5)
		Stone::Stone(j, 1.0, i);
	for (float j = 8; j<22.5; j = j + 0.5)
	for (float i = 8; i<19; i = i + 0.5)
		Stone::Stone(j, 1.5, i);
	for (float j = 8.5; j<22; j = j + 0.5)
	for (float i = 8.5; i<19; i = i + 0.5)
		Stone::Stone(j, 2, i);
	for (float j = 9; j<21.5; j = j + 0.5)
	for (float i = 9; i<19; i = i + 0.5)
		Stone::Stone(j, 2.5, i);
	for (float j = 9.5; j<21; j = j + 0.5)
	for (float i = 9.5; i<19; i = i + 0.5)
		Stone::Stone(j, 3, i);
	glBindTexture(GL_TEXTURE_2D, texWall);
	//House
	for (float j = 3.5; j<9; j = j + 0.5)
	for (float i = 10; i<19; i = i + 0.5)
		Stone::Stone(10, j, i);
	for (float j = 3.5; j<9; j = j + 0.5)
	for (float i = 10; i<19; i = i + 0.5)
		Stone::Stone(20, j, i);
	for (float j = 10; j<20.5; j = j + 0.5)
	for (float i = 10; i<19; i = i + 0.5)
		Stone::Stone(j, 9, i);
	for (float j = 10; j<20; j = j + 0.5)
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(j, i, 18.5);
	for (float j = 10; j<20; j = j + 0.5)
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(j, i, 18);
	for (float j = 10.5; j<13.5; j = j + 0.5)
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(j, i, 10);
	for (float j = 17; j<20; j = j + 0.5)
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(j, i, 10);
	glBindTexture(GL_TEXTURE_2D, texDoor);
	for (float j = 13.5; j<15; j = j + 0.5)
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(j, i, 10);
	for (float j = 15.5; j<17; j = j + 0.5)
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(j, i, 10);
	glBindTexture(GL_TEXTURE_2D, texDoorBar);
	for (float i = 3.5; i<9; i = i + 0.5)
		Stone::Stone(15, i, 10);
	// Reset transformations
	for (int i = 1; i<30; i = i + 5){
		glLoadIdentity();
		// Set the camera centered at (x,y,1) and looking along directional
		// vector (lx, ly, 0), with the z-axis pointing up
		gluLookAt(
			x, y, 1,
			x + lx, y + ly, 1.0,
			0.0, 0.0, 1.0);
		glTranslatef(tans_x + 15, -15.0f - i, shift-1.5f);
		glRotatef(rote_x, 1.0, 0.0, 0.0);
		glRotatef(0.0, 0.0, 1.0, 0.0);
		glRotatef(0.0, 0.0, 0.0, 1.0);
		glScalef(0.5, 0.5, 0.5);
		//tree
		glBindTexture(GL_TEXTURE_2D, texTree);
		Ractangular::Ractangular(0.5, 10.3, 0.2, 1);
		Ractangular::Ractangular(0, 8, 0, 2);
		Ractangular::Ractangular(-0.5, 5.5, -0.3, 3);
		Ractangular::Ractangular(-1, 3.0, -0.6, 4);
		Ractangular::Ractangular(-1.5, 0.0, -0.8, 5);
		glBindTexture(GL_TEXTURE_2D, texTreeB);
		Stone::Stone(0.5, 3, 0);
		Stone::Stone(0.5, 2.5, 0);
		Stone::Stone(0.5, 2, 0);
		Stone::Stone(0.5, 1.5, 0);
		Stone::Stone(0.5, 1, 0);
		Stone::Stone(0.5, 0.5, 0);
	}
	for (int i = 1; i<30; i = i + 5){
		glLoadIdentity();
		// Set the camera centered at (x,y,1) and looking along directional
		// vector (lx, ly, 0), with the z-axis pointing up
		gluLookAt(
			x, y, 1,
			x + lx, y + ly, 1.0,
			0.0, 0.0, 1.0);
		glTranslatef(tans_x + 33, -15.0f - i, shift-1.5f);
		glRotatef(rote_x, 1.0, 0.0, 0.0);
		glRotatef(0.0, 0.0, 1.0, 0.0);
		glRotatef(0.0, 0.0, 0.0, 1.0);
		glScalef(0.5, 0.5, 0.5);
		//tree
		glBindTexture(GL_TEXTURE_2D, texTree);
		Ractangular::Ractangular(0.5, 10.3, 0.2, 1);
		Ractangular::Ractangular(0, 8, 0, 2);
		Ractangular::Ractangular(-0.5, 5.5, -0.3, 3);
		Ractangular::Ractangular(-1, 3.0, -0.6, 4);
		Ractangular::Ractangular(-1.5, 0.0, -0.8, 5);
		glBindTexture(GL_TEXTURE_2D, texTreeB);
		Stone::Stone(0.5, 3, 0);
		Stone::Stone(0.5, 2.5, 0);
		Stone::Stone(0.5, 2, 0);
		Stone::Stone(0.5, 1.5, 0);
		Stone::Stone(0.5, 1, 0);
		Stone::Stone(0.5, 0.5, 0);
	}
	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, 1,
		x + lx, y + ly, 1.0,
		0.0, 0.0, 1.0);
	glTranslatef(tans_x + 33, -6.0f, shift-1.5f);
	glRotatef(rote_x, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	//tree
	glBindTexture(GL_TEXTURE_2D, texTree);
	Ractangular::Ractangular(0.5, 10.3, 0.2, 1);
	Ractangular::Ractangular(0, 8, 0, 2);
	Ractangular::Ractangular(-0.5, 5.5, -0.3, 3);
	Ractangular::Ractangular(-1, 3.0, -0.6, 4);
	Ractangular::Ractangular(-1.5, 0.0, -0.8, 5);
	glBindTexture(GL_TEXTURE_2D, texTreeB);
	Stone::Stone(0.5, 3, 0);
	Stone::Stone(0.5, 2.5, 0);
	Stone::Stone(0.5, 2, 0);
	Stone::Stone(0.5, 1.5, 0);
	Stone::Stone(0.5, 1, 0);
	Stone::Stone(0.5, 0.5, 0);
	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, 1,
		x + lx, y + ly, 1.0,
		0.0, 0.0, 1.0);
	glTranslatef(tans_x + 15, -6.0f, shift-1.5f);
	glRotatef(rote_x, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	//tree
	glBindTexture(GL_TEXTURE_2D, texTree);
	Ractangular::Ractangular(0.5, 10.3, 0.2, 1);
	Ractangular::Ractangular(0, 8, 0, 2);
	Ractangular::Ractangular(-0.5, 5.5, -0.3, 3);
	Ractangular::Ractangular(-1, 3.0, -0.6, 4);
	Ractangular::Ractangular(-1.5, 0.0, -0.8, 5);
	glBindTexture(GL_TEXTURE_2D, texTreeB);
	Stone::Stone(0.5, 3, 0);
	Stone::Stone(0.5, 2.5, 0);
	Stone::Stone(0.5, 2, 0);
	Stone::Stone(0.5, 1.5, 0);
	Stone::Stone(0.5, 1, 0);
	Stone::Stone(0.5, 0.5, 0);
	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, 1,
		x + lx, y + ly, 1.0,
		0.0, 0.0, 1.0);
	glTranslatef(tans_x, 0.0f, shift-1.5f);
	glScalef(0.6, 0.6, 0.6);
	glRotatef(rote_x, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texSky);
	Sky::Sky(-160, -100, -50);
	//Sun
	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, 1,
		x + lx, y + ly, 1.0,
		0.0, 0.0, 1.0);
	glTranslatef(tans_x-40, 20.0f, shift+0.5f);
	glScalef(0.6, 0.6, 0.6);
	glRotatef(rote_x, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texSun);
	glPushMatrix();
	//glutSolidTeapot(2.0);
	glutSolidSphere(14.0, 32, 32);
	glPopMatrix();

	glLoadIdentity();
	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, 1,
		x + lx, y + ly, 1.0,
		0.0, 0.0, 1.0);
	glTranslatef(tans_x+29, -6.0f, shift-2.0f);
	glScalef(0.6, 0.6, 0.6);
	glRotatef(rote_x, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//Person
	glBindTexture(GL_TEXTURE_2D, texTop);
	BStone::BStone(-0.1, 5.3, 0.1);
	BStone::BStone(0.1, 5.3, 0.1);
	glBindTexture(GL_TEXTURE_2D, texPerson);
	BStone::BStone(0, 5, 0);
	glBindTexture(GL_TEXTURE_2D, texTreeB);
	Stone::Stone(1.25, 5.5, 0.5);
	Stone::Stone(1.75, 5.5, 0.5);
	Stone::Stone(1.25, 5.5, 1.0);
	Stone::Stone(1.75, 5.5, 1.0);
	Stone::Stone(1.25, 5.0, 0.5);
	Stone::Stone(1.75, 5.0, 0.5);
	Stone::Stone(1.25, 5.0, 1.0);
	Stone::Stone(1.75, 5.0, 1.0);
	Stone::Stone(1.25, 4.5, 0.5);
	Stone::Stone(1.75, 4.5, 0.5);
	Stone::Stone(1.25, 4.5, 1.0);
	Stone::Stone(1.75, 4.5, 1.0);
	Stone::Stone(1.25, 4.0, 0.5);
	Stone::Stone(1.75, 4.0, 0.5);
	Stone::Stone(1.25, 4.0, 1.0);
	Stone::Stone(1.75, 4.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texTree);
	Stone::Stone(1.8, 3.5, 0.75);
	Stone::Stone(1.8, 3.0, 0.75);
	Stone::Stone(1.8, 2.5, 0.75);
	Stone::Stone(1.2, 3.5, 0.75);
	Stone::Stone(1.2, 3.0, 0.75);
	Stone::Stone(1.2, 2.5, 0.75);
	glBindTexture(GL_TEXTURE_2D, texTreeB);
	Stone::Stone(1.2, 2.0, 0.5);
	Stone::Stone(1.2, 2.0, 0.75);
	Stone::Stone(1.8, 2.0, 0.5);
	Stone::Stone(1.8, 2.0, 0.75);
	glBindTexture(GL_TEXTURE_2D, texTree);
	Stone::Stone(1.0, 4.0, 0.75);
	Stone::Stone(1.0, 4.5, 0.75);
	Stone::Stone(1.0, 5.0, 0.75);
	Stone::Stone(2.0, 4.0, 0.75);
	Stone::Stone(2.0, 4.5, 0.75);
	Stone::Stone(2.0, 5.0, 0.75);
	glBindTexture(GL_TEXTURE_2D, texTreeB);
	Stone::Stone(2.0, 6.75, -0.25);
	Stone::Stone(1.0, 6.75, -0.25);
	glFlush();  // drawing the object to screen
	grab();
}



void changeSize(int w, int h)
{
	float ratio = ((float)w) / ((float)h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(75.0, ratio, 0.1, 10000000000.0); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}
void update(void)
{
	if (deltaMove) { // update camera position
		x += deltaMove * lx * 0.1;
		y += deltaMove * ly * 0.1;
	}
	glutPostRedisplay(); // redisplay everything
}
void processNormalKeys(unsigned char key, int xx, int yy)
{
	if (key == ESC || key == 'q' || key == 'Q') exit(0);
}

void pressSpecialKey(int key, int xx, int yy)
{
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 2.0; break;
	case GLUT_KEY_DOWN: deltaMove = -2.0; break;
	case GLUT_KEY_LEFT: tans_x -= 1.0; break;
	case GLUT_KEY_RIGHT: tans_x += 1.0; break;
	case GLUT_KEY_PAGE_UP: shift -= 1.0; break;
	case GLUT_KEY_PAGE_DOWN: shift += 1.0; break;
	}
}

void releaseSpecialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.0; break;
	case GLUT_KEY_DOWN: deltaMove = 0.0; break;
	case GLUT_KEY_LEFT: tans_x -= 1.0; break;
	case GLUT_KEY_RIGHT: tans_x += 1.0; break;
	case GLUT_KEY_PAGE_UP: shift -= 1.0; break;
	case GLUT_KEY_PAGE_DOWN: shift += 1.0; break;

	}
}

void mouseMove(int x, int y)
{
	if (isDragging) { // only when dragging
		// update the change in angle
		deltaAngle = (x - xDragStart) * 0.005;
		// camera's direction is set to angle + deltaAngle
		lx = -sin(angle + deltaAngle);
		ly = cos(angle + deltaAngle);
	}

}
void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDragging = 1; // start dragging
			xDragStart = x; // save x where button first pressed
		}
		else  { /* (state = GLUT_UP) */
			angle += deltaAngle; // update camera turning angle
			isDragging = 0; // no longer dragging
		}
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("JohnLin");
	glutDisplayFunc(renderScene); // (re)display callback
	glutReshapeFunc(changeSize); // window reshape callback
	glutIdleFunc(update); // incremental update
	glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
	glutMouseFunc(mouseButton); // process mouse button push/release
	glutMotionFunc(mouseMove); // process mouse dragging motion
	glutKeyboardFunc(processNormalKeys); // process standard key clicks
	glutSpecialFunc(pressSpecialKey); // process special key pressed
	// Warning: Nonstandard function! Delete if desired.
	glutSpecialUpFunc(releaseSpecialKey); // process special key release
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LESS);
	texGround = load_texture("Grass1.bmp");
	texWall = load_texture("stone.bmp");
	texTop = load_texture("Stone1.bmp");
	texSnow = load_texture("Snow.bmp");
	texDoor = load_texture("door.bmp");
	texDoorBar = load_texture("doorbar.bmp");
	texCwall = load_texture("ChengbaoWall.bmp");
	texSky = load_texture("sky2.bmp");
	texChwall = load_texture("ChengbaoWall.bmp");
	texGubao = load_texture("Gubao1.bmp");
	texGubaod = load_texture("stone2.bmp");
	texGuTop = load_texture("Top.bmp");
	texTree = load_texture("tree.bmp");
	texTreeB = load_texture("tree1.bmp");
	texGudoor = load_texture("Gudoor.bmp");
	texGuW = load_texture("GuW.bmp");
	texSun = load_texture("Sun.bmp");
	texPerson = load_texture("Person.bmp");
	texWindow = load_texture("Window.bmp");
	glutMainLoop();
	return 0;
}


