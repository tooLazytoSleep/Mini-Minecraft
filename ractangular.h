//
//  ractangular.h
//  Hogwarts
//
//  Created by Johnlin on 2016/12/24.
//  Copyright © 2016年 Johnlin. All rights reserved.
//

#ifndef ractangular_h
#define ractangular_h
#include <iostream>
#include <gl/GL.h>
#include <gl/freeglut.h>
#include <cmath>
#endif /* ractangular_h */
class Ractangular{
public:
	Ractangular(float i, float j, float p, int r){
		glBegin(GL_TRIANGLE_FAN); // construct a pyramid
		glTexCoord2d(0, 0);
		glVertex3f(i + r*0.5f, j + r*1.75f, p + r*0.25f);
		glTexCoord2d(1, 0);
		glVertex3f(i + r*0.0f, j + r*0.75f, p + r*(-0.25f));
		glTexCoord2d(0, 1);
		glVertex3f(i + r*1.0f, j + r*0.75f, p + r*(-0.25f));
		glTexCoord2d(1, 0);
		glVertex3f(i + r*1.0f, j + r*0.75f, p + r*0.75f);
		glTexCoord2d(0, 1);
		glVertex3f(i + r*0.0f, j + r*0.75f, p + r*0.75f);
		glTexCoord2d(1, 1);
		glVertex3f(i + r*0.0f, j + r*0.75f, p + r*(-0.25f));
		glEnd();
	}
};
