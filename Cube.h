
#define checkImageWidth 64
#define checkImageHeight 64
#define Cube_h
#include <iostream>
#include<GL/freeglut.h>
#include<GL/gl.h>
class Grass{
public:
    Grass(float i,float j,float q){
        glBegin(GL_QUADS); // construct a pyramid
        //右
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 0);
        glVertex3f(i+0.75f, j+0.75f, q+0.5f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(0, 0);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        //前
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f, j+0.75f, q+0.0f);

        //后
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.5f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.5f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f, j+0.75f, q+0.5f);
        
        //左
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f,j+ 0.25f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.25f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.25f,j+ 0.75f, q+0.5f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.25f, j+0.25f, q+0.5f);
        //上
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.25f, j+0.75f, q+0.5f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.75f,q+ 0.5f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);

        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.25f, j+0.25f, q+0.5f);
        glTexCoord2d(0,1);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(0, 0);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        
        
        //右
        glTexCoord2d(0, 0);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.5f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        glEnd();
    }
};
class Stone{
public:
    Stone(float i,float j,float q){
        glBegin(GL_QUADS); // construct a pyramid
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.25f, j+0.25f, q+0.5f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(0,0);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        //上
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.25f, j+0.75f, q+0.5f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.75f,q+ 0.5f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);
        //左
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f,j+ 0.25f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.25f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.25f,j+ 0.75f, q+0.5f);
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.5f);
        //前
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f, j+0.75f, q+0.0f);
        //后
        glTexCoord2d(1, 0);
        glVertex3f(i+0.25f, j+0.25f, q+0.5f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.5f);
        glTexCoord2d(0, 0);
        glVertex3f(i+0.25f, j+0.75f, q+0.5f);
        //右
        glTexCoord2d(0, 0);
        glVertex3f(i+0.75f, j+0.75f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+0.75f, j+0.75f, q+0.5f);
        glTexCoord2d(1, 1);
        glVertex3f(i+0.75f, j+0.25f, q+0.5f);
        glTexCoord2d(1, 0);
        glVertex3f(i+0.75f, j+0.25f, q+0.0f);
        glEnd();
    }
};
class BStone{
public:
    BStone(float i,float j,float q){
        glBegin(GL_QUADS); // construct a pyramid
    
        //上
        glTexCoord2d(1, 0);
        glVertex3f(i+1.0f, j+3.0f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+1.0f, j+3.0f, q+2.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+3.0f, j+3.0f,q+ 2.0f);
        glTexCoord2d(0, 0);
        glVertex3f(i+3.0f, j+3.0f, q+0.0f);
        
        glTexCoord2d(0, 0);
        glVertex3f(i+1.0f, j+1.0f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+1.0f, j+1.0f, q+2.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+3.0f, j+1.0f, q+2.0f);
        glTexCoord2d(1,0);
        glVertex3f(i+3.0f, j+1.0f, q+0.0f);
        
        //左
        glTexCoord2d(1, 0);
        glVertex3f(i+1.0f,j+ 1.0f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+1.0f, j+3.0f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+1.0f,j+ 3.0f, q+2.0f);
        glTexCoord2d(0, 0);
        glVertex3f(i+1.0f, j+1.0f, q+2.0f);
        
        //前
        glTexCoord2d(0, 0);
        glVertex3f(i+1.0f, j+1.0f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+3.0f, j+1.0f, q+0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+3.0f, j+3.0f, q+0.0f);
        glTexCoord2d(1, 0);
        glVertex3f(i+1.0f, j+3.0f, q+0.0f);
        
        
        //后
        glTexCoord2d(1, 0);
        glVertex3f(i+1.0f, j+1.0f, q+2.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+3.0f, j+1.0f, q+2.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+3.0f, j+3.0f, q+2.0f);
        glTexCoord2d(0, 0);
        glVertex3f(i+1.0f, j+3.0f, q+2.0f);
        //右
        glTexCoord2d(0, 0);
        glVertex3f(i+3.0f, j+3.0f, q+0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(i+3.0f, j+3.0f, q+2.0f);
        glTexCoord2d(1, 1);
        glVertex3f(i+3.0f, j+1.0f, q+2.0f);
        glTexCoord2d(1, 0);
        glVertex3f(i+3.0f, j+1.0f, q+0.0f);

         glEnd();
        
    }
};
class BBStone{
public:
	BBStone(float i, float j, float q,int w){
		glBegin(GL_QUADS); // construct a pyramid
		//上
		glTexCoord2d(1, 0);
		glVertex3f(i + w*4.0f, j + w*12.0f, q + w*0.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + w*4.0f, j + w*12.0f, q + w*8.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + w*12.0f, j + w*12.0f, q + w*8.0f);
		glTexCoord2d(0, 0);
		glVertex3f(i + w*12.0f, j + w*12.0f, q + w*0.0f);

		glTexCoord2d(0, 0);
		glVertex3f(i + w*4.0f, j + w*4.0f, q + w*0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + w*4.0f, j + w*4.0f, q + w*8.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + w*12.0f, j + w*4.0f, q + w*8.0f);
		glTexCoord2d(1, 0);
		glVertex3f(i + w*12.0f, j + w*4.0f, q + w*0.0f);

		//左
		glTexCoord2d(1, 0);
		glVertex3f(i + w*4.0f, j + w*4.0f, q + w*0.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + w*4.0f, j + w*12.0f, q + w*0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + w*4.0f, j + w*12.0f, q + w*8.0f);
		glTexCoord2d(0, 0);
		glVertex3f(i + w*4.0f, j + w*4.0f, q + w*8.0f);

		//前
		glTexCoord2d(0, 0);
		glVertex3f(i + w*4.0f, j + w*4.0f, q + w*0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + w*12.0f, j + w*4.0f, q + w*0.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + w*12.0f, j + w*12.0f, q + w*0.0f);
		glTexCoord2d(1, 0);
		glVertex3f(i + w*4.0f, j + w*12.0f, q + w*0.0f);


		//后
		glTexCoord2d(1, 0);
		glVertex3f(i + w*4.0f, j + w*4.0f, q + w*8.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + w*12.0f, j + w*4.0f, q + w*8.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + w*12.0f, j + w*12.0f, q + w*8.0f);
		glTexCoord2d(0, 0);
		glVertex3f(i + w*4.0f, j + w*12.0f, q + w*8.0f);
		//右
		glTexCoord2d(0, 0);
		glVertex3f(i + w*12.0f, j + w*12.0f, q + w*0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + w*12.0f, j + w*12.0f, q + w*8.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + w*12.0f, j + w*4.0f, q + w*8.0f);
		glTexCoord2d(1, 0);
		glVertex3f(i + w*12.0f, j + w*4.0f, q + w*0.0f);

		glEnd();

	}
};
class Sky{
public:
	Sky(float i, float j, float q){
		glBegin(GL_QUADS); // construct a pyramid

		//上
		glTexCoord2d(1, 0);
		glVertex3f(i + 100.0f, j + 300.0f, q + 0.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + 100.0f, j + 300.0f, q + 200.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + 300.0f, j + 300.0f, q + 200.0f);
		glTexCoord2d(0, 0);
		glVertex3f(i + 300.0f, j + 300.0f, q + 0.0f);

		glTexCoord2d(0, 0);
		glVertex3f(i + 100.0f, j + 100.0f, q + 0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + 100.0f, j + 100.0f, q + 200.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + 300.0f, j + 100.0f, q + 200.0f);
		glTexCoord2d(1, 0);
		glVertex3f(i + 300.0f, j + 100.0f, q + 0.0f);

		//左
		glTexCoord2d(1, 0);
		glVertex3f(i + 100.0f, j + 100.0f, q + 0.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + 100.0f, j + 300.0f, q + 0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + 100.0f, j + 300.0f, q + 200.0f);
		glTexCoord2d(0, 0);
		glVertex3f(i + 100.0f, j + 100.0f, q + 200.0f);

		//前
		glTexCoord2d(0, 0);
		glVertex3f(i + 100.0f, j + 100.0f, q + 0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + 300.0f, j + 100.0f, q + 0.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + 300.0f, j + 300.0f, q + 0.0f);
		glTexCoord2d(1, 0);
		glVertex3f(i + 100.0f, j + 300.0f, q + 0.0f);


		//后
		glTexCoord2d(1, 0);
		glVertex3f(i + 100.0f, j + 100.0f, q + 200.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + 300.0f, j + 100.0f, q + 200.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + 300.0f, j + 300.0f, q + 200.0f);
		glTexCoord2d(0, 0);
		glVertex3f(i + 100.0f, j + 300.0f, q + 200.0f);
		//右
		glTexCoord2d(0, 0);
		glVertex3f(i + 300.0f, j + 300.0f, q + 0.0f);
		glTexCoord2d(0, 1);
		glVertex3f(i + 300.0f, j + 300.0f, q + 200.0f);
		glTexCoord2d(1, 1);
		glVertex3f(i + 300.0f, j + 100.0f, q + 200.0f);
		glTexCoord2d(1, 0);
		glVertex3f(i + 300.0f, j + 100.0f, q + 0.0f);

		glEnd();

	}
};

