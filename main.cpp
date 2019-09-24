//
//  main.cpp
//  Hogwarts
//
//  Created by Johnlin on 2016/12/24.
//  Copyright © 2016年 Johnlin. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
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
    
    // ¼ÆËãÏñËØÊý¾ÝµÄÊµ¼Ê³¤¶È
    i = WindowWidth * 3;   // µÃµ½Ã¿Ò»ÐÐµÄÏñËØÊý¾Ý³¤¶È
    while (i % 4 != 0)      // ²¹³äÊý¾Ý£¬Ö±µ½iÊÇµÄ±¶Êý
        ++i;               // ±¾À´»¹ÓÐ¸ü¿ìµÄËã·¨£¬
    // µ«ÕâÀï½ö×·ÇóÖ±¹Û£¬¶ÔËÙ¶ÈÃ»ÓÐÌ«¸ßÒªÇó
    PixelDataLength = i * WindowHeight;
    
    // ·ÖÅäÄÚ´æºÍ´ò¿ªÎÄ¼þ
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if (pPixelData == 0)
        exit(0);
    
   pDummyFile = fopen("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/dummy.bmp", "rb");
    if (pDummyFile == 0)
        exit(0);
    
    pWritingFile = fopen("grab.bmp", "wb");
    if (pWritingFile == 0)
        exit(0);
    
    // ¶ÁÈ¡ÏñËØ
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
    
    // °Ñdummy.bmpµÄÎÄ¼þÍ·¸´ÖÆÎªÐÂÎÄ¼þµÄÎÄ¼þÍ·
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = WindowWidth;
    j = WindowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);
    
    // Ð´ÈëÏñËØÊý¾Ý
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);
    
    // ÊÍ·ÅÄÚ´æºÍ¹Ø±ÕÎÄ¼þ
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}

/* º¯Êýpower_of_two
 * ¼ì²éÒ»¸öÕûÊýÊÇ·ñÎª2µÄÕûÊý´Î·½£¬Èç¹ûÊÇ£¬·µ»Ø1£¬·ñÔò·µ»Ø0
 * Êµ¼ÊÉÏÖ»Òª²é¿´Æä¶þ½øÖÆÎ»ÖÐÓÐ¶àÉÙ¸ö£¬Èç¹ûÕýºÃÓÐ1¸ö£¬·µ»Ø1£¬·ñÔò·µ»Ø0
 * ÔÚ¡°²é¿´Æä¶þ½øÖÆÎ»ÖÐÓÐ¶àÉÙ¸ö¡±Ê±Ê¹ÓÃÁËÒ»¸öÐ¡¼¼ÇÉ
 * Ê¹ÓÃn &= (n-1)¿ÉÒÔÊ¹µÃnÖÐµÄ¼õÉÙÒ»¸ö£¨¾ßÌåÔ­Àí´ó¼Ò¿ÉÒÔ×Ô¼ºË¼¿¼£©
 */
int power_of_two(int n)
{
    if (n <= 0)
        return 0;
    return (n & (n - 1)) == 0;
}

/* º¯Êýload_texture
 * ¶ÁÈ¡Ò»¸öBMPÎÄ¼þ×÷ÎªÎÆÀí
 * Èç¹ûÊ§°Ü£¬·µ»Ø0£¬Èç¹û³É¹¦£¬·µ»ØÎÆÀí±àºÅ
 */
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint texture_ID = 0;
    GLint last_texture_ID;
    // ´ò¿ªÎÄ¼þ£¬Èç¹ûÊ§°Ü£¬·µ»Ø
    FILE* pFile = fopen(file_name, "rb");
    if (pFile == 0)
        return 0;
    
    // ¶ÁÈ¡ÎÄ¼þÖÐÍ¼ÏóµÄ¿í¶ÈºÍ¸ß¶È
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);
    
    // ¼ÆËãÃ¿ÐÐÏñËØËùÕ¼×Ö½ÚÊý£¬²¢¸ù¾Ý´ËÊý¾Ý¼ÆËã×ÜÏñËØ×Ö½ÚÊý
    {
        GLint line_bytes = width * 3;
        while (line_bytes % 4 != 0)
            ++line_bytes;
        total_bytes = line_bytes * height;
    }
    
    // ¸ù¾Ý×ÜÏñËØ×Ö½ÚÊý·ÖÅäÄÚ´æ
    pixels = (GLubyte*)malloc(total_bytes);
    if (pixels == 0)
    {
        fclose(pFile);
        return 0;
    }
    
    // ¶ÁÈ¡ÏñËØÊý¾Ý
    if (fread(pixels, total_bytes, 1, pFile) <= 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    // ÔÚ¾É°æ±¾µÄOpenGLÖÐ
    // Èç¹ûÍ¼ÏóµÄ¿í¶ÈºÍ¸ß¶È²»ÊÇµÄÕûÊý´Î·½£¬ÔòÐèÒª½øÐÐËõ·Å
    // ÕâÀï²¢Ã»ÓÐ¼ì²éOpenGL°æ±¾£¬³öÓÚ¶Ô°æ±¾¼æÈÝÐÔµÄ¿¼ÂÇ£¬°´¾É°æ±¾´¦Àí
    // ÁíÍâ£¬ÎÞÂÛÊÇ¾É°æ±¾»¹ÊÇÐÂ°æ±¾£¬
    // µ±Í¼ÏóµÄ¿í¶ÈºÍ¸ß¶È³¬¹ýµ±Ç°OpenGLÊµÏÖËùÖ§³ÖµÄ×î´óÖµÊ±£¬Ò²Òª½øÐÐËõ·Å
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!power_of_two(width)
            || !power_of_two(height)
            || width > max
            || height > max)
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // ¹æ¶¨Ëõ·ÅºóÐÂµÄ´óÐ¡Îª±ß³¤µÄÕý·½ÐÎ
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            
            // ¼ÆËãÃ¿ÐÐÐèÒªµÄ×Ö½ÚÊýºÍ×Ü×Ö½ÚÊý
            new_line_bytes = new_width * 3;
            while (new_line_bytes % 4 != 0)
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            
            // ·ÖÅäÄÚ´æ
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if (new_pixels == 0)
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            
            // ½øÐÐÏñËØËõ·Å
            gluScaleImage(GL_RGB,
                          width, height, GL_UNSIGNED_BYTE, pixels,
                          new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
            
            // ÊÍ·ÅÔ­À´µÄÏñËØÊý¾Ý£¬°ÑpixelsÖ¸ÏòÐÂµÄÏñËØÊý¾Ý£¬²¢ÖØÐÂÉèÖÃwidthºÍheight
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    
    // ·ÖÅäÒ»¸öÐÂµÄÎÆÀí±àºÅ
    glGenTextures(1, &texture_ID);
    if (texture_ID == 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    // °ó¶¨ÐÂµÄÎÆÀí£¬ÔØÈëÎÆÀí²¢ÉèÖÃÎÆÀí²ÎÊý
    // ÔÚ°ó¶¨Ç°£¬ÏÈ»ñµÃÔ­À´°ó¶¨µÄÎÆÀí±àºÅ£¬ÒÔ±ãÔÚ×îºó½øÐÐ»Ö¸´
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
    //ÔÚ·ÖÅäÁËÎÆÀí¶ÔÏó±àºÅºó£¬Ê¹ÓÃglBindTextureº¯ÊýÀ´Ö¸¶¨¡°µ±Ç°ËùÊ¹ÓÃµÄÎÆÀí¶ÔÏó¡±
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, last_texture_ID);
    
    // Ö®Ç°Îªpixels·ÖÅäµÄÄÚ´æ¿ÉÔÚÊ¹ÓÃglTexImage2DÒÔºóÊÍ·Å
    // ÒòÎª´ËÊ±ÏñËØÊý¾ÝÒÑ¾­±»OpenGLÁíÐÐ±£´æÁËÒ»·Ý£¨¿ÉÄÜ±»±£´æµ½×¨ÃÅµÄÍ¼ÐÎÓ²¼þÖÐ£©
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
void renderScene(void)
{
    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
              x, y, 1,
              x + lx, y + ly, 1.0,
              0.0, 0.0, 1.0);
    glTranslatef(tans_x, 0.0f, -1.5f);
    glScalef(0.5,0.5,0.5);
    glRotatef(rote_x, 1.0, 0.0, 0.0);
    glRotatef(0.0, 0.0, 1.0, 0.0);
    glRotatef(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glBindTexture(GL_TEXTURE_2D, texGround);
    for (float j = -50; j<120; j = j + 2)
        for (float i = -50; i<130; i = i + 2)
            BStone::BStone(j, -1, i);
    glBindTexture(GL_TEXTURE_2D, texSnow);
    for (float j = 36; j<54; j = j + 2)
        for (float i = 19; i<80; i = i +2)
            BStone::BStone(j, -2, i);
        glBindTexture(GL_TEXTURE_2D, texCwall);
        //chengbao
    for(float p=-0.0;p<30;p=p+2)
        for (float j = -8; j<36; j = j + 2)
            BStone::BStone(j, p, 18.5);
    for(float p=-0.0;p<30;p=p+2)
        for (float j = 54; j<100; j = j + 2)
            BStone::BStone(j, p, 18.5);
    for(float p=20;p<26;p=p+2)
        for (float j = 30; j<54; j = j + 2)
            BStone::BStone(j, p, 18.5);
    for(float p=-2.0;p<30;p=p+2)
        for (float j = 18.5; j<120; j = j + 2)
            BStone::BStone(-8, p, j);
    for(float p=-2.0;p<30;p=p+2)
        for (float j = 18.5; j<120; j = j + 2)
            BStone::BStone(100, p, j);
    glBindTexture(GL_TEXTURE_2D, texTop);
    for(float p=0.5;p<30;p=p+2)
        for (float j = -5; j<36; j = j + 2)
            Stone::Stone(j, p, 18.25);
    for(float p=0.5;p<30;p=p+2)
        for (float j = 55; j<100; j = j + 2)
            Stone::Stone(j, p, 18.25);
    

    glBindTexture(GL_TEXTURE_2D, texTop);
    //house top
    for (int i = 9.75; i<20; i = i + 1)
        for (float j = 10; j <= 20; j = j + 1)
            Ractangular::Ractangular(j, 9, i);
    for (int i = 10.25; i<19.5; i = i + 1)
        for (float j = 10.5; j <= 19.5; j = j + 1)
            Ractangular::Ractangular(j, 9.5, i);
    for (int i = 10.75; i<19.0; i = i + 1)
        for (float j = 11; j <= 19; j = j + 1)
            Ractangular::Ractangular(j, 10, i);
    for (int i = 11.25; i<18.5; i = i + 1)
        for (float j = 11.5; j <= 18.5; j = j + 1)
            Ractangular::Ractangular(j, 10.5, i);
    for (int i = 11.75; i<18; i = i + 1)
        for (float j = 12; j <= 18; j = j + 1)
            Ractangular::Ractangular(j, 11, i);
    for (int i = 12.25; i<17.5; i = i + 1)
        for (float j = 12.5; j <= 17.5; j = j + 1)
            Ractangular::Ractangular(j, 11.5, i);
    for (int i = 12.75; i<17; i = i + 1)
        for (float j = 13; j <= 17; j = j + 1)
            Ractangular::Ractangular(j, 12, i);
    for (int i = 13.25; i<16.5; i = i + 1)
        for (float j = 13.5; j <= 16.5; j = j + 1)
            Ractangular::Ractangular(j, 12.5, i);
    for (int i = 13.75; i<16; i = i + 1)
        for (float j = 14; j <= 16; j = j + 1)
            Ractangular::Ractangular(j, 13, i);
    for (int i = 14.25; i<15.5; i = i + 1)
        for (float j = 14.5; j <= 15.5; j = j + 1)
            Ractangular::Ractangular(j, 13.5, i);
    for (int i = 14.75; i<15; i = i + 1)
        for (float j = 15; j <= 15; j = j + 1)
            Ractangular::Ractangular(j, 14, i);
    
    glBindTexture(GL_TEXTURE_2D, texTop);
    //house decorate
    for (float j = 3; j<9; j = j + 1)
        Ractangular::Ractangular(20, j, 9.75);
    for (float j = 3; j<9; j = j + 2)
        Ractangular::Ractangular(10, j, 9.75);
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

    glFlush();  // drawing the object to screen
    grab();
}



void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(70.0, ratio, 0.1, 100.0); // perspective transformation
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
    }
}

void releaseSpecialKey(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP: deltaMove = 0.0; break;
        case GLUT_KEY_DOWN: deltaMove = 0.0; break;
        case GLUT_KEY_LEFT: tans_x -= 1.0; break;
        case GLUT_KEY_RIGHT: tans_x += 1.0; break;
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
void init(void)
{
    glLineWidth(5);
    glClearColor(0.0, 0.0, 20.0, 1.0);  // select background color: yellow
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluPerspective(75.0f, (float)h/ w, 6.0f, 1000.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glMatrixMode(GL_PROJECTION); // places the camera at (0,0,0) and faces it along(0,0,-1).
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, w / 2, -w / 2); // specify clipping volume
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("JohnLin");
    init();
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
    texGround = load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/Grass1.bmp");
    texWall = load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/stone.bmp");
    texTop =load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/Stone1.bmp");
    texSnow =load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/Snow.bmp");
    texDoor =load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/door.bmp");
     texDoorBar =load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/doorbar.bmp");
    texCwall =load_texture("/Users/Johnlin/Documents/C++/Hogwarts/Hogwarts/ChengbaoWall.bmp");
    glutMainLoop();
    return 0;
}

