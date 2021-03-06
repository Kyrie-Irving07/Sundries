// 图形学作业5_简单光线跟踪_黄熠华.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <gl/glut.h>
#define THRESHOLD 0.01
#define MAXDEPTH 100

enum color
{
	RED,
	GREEN,
	BLUE,
	PINK,
	YELLOW,
	NONE
};

typedef struct rgb
{
	float r;
	float g;
	float b;
	rgb()
	{
		r = 0.f;
		g = 0.f;
		b = 0.f;
	}
	rgb(float rr, float gg, float bb)
	{
		r = rr;
		g = gg;
		b = bb;
	}
	rgb operator +(const rgb & rgb0)
	{
		rgb temp;
		temp.r = r + rgb0.r;
		temp.g = g + rgb0.g;
		temp.b = b + rgb0.b;
		return temp;
	}
	rgb operator *(float mul)
	{
		rgb temp;
		temp.r = r * mul;
		temp.g = g * mul;
		temp.b = b * mul;
		return temp;
	}
} rgb;

typedef struct point
{
	float x, y, z;
	int flag;
	point()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		flag = 0;
	}
	point(const point& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		flag = p.flag;
	}
	point(float xx, float yy, float zz, int ff)
	{
		x = xx;
		y = yy;
		z = zz;
		flag = ff;
	}
	point(float xx, float yy, float zz)
	{
		x = xx;
		y = yy;
		z = zz;
		flag = 0;
	}
	point operator*(float mul)
	{
		point temp;
		temp.x = x * mul;
		temp.y = y * mul;
		temp.z = z * mul;
		return temp;
	}

	point operator/(float div)
	{
		point temp;
		temp.x = x / div;
		temp.y = y / div;
		temp.z = z / div;
		return temp;
	}

	float operator*(const point & p)
	{
		float temp = 0;
		temp += x * p.x;
		temp += y * p.y;
		temp += z * p.z;
		return temp;
	}
	point operator=(const point& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		flag = p.flag;
		return *this;
	}
	point operator+(const point& p)
	{
		point temp;
		temp.x = x + p.x;
		temp.y = y + p.y;
		temp.z = z + p.z;
		return temp;
	}
	point operator-(const point& p)
	{
		point temp(x - p.x, y - p.y, z - p.z, 0);
		return temp;
	}
	float length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
	float sum()
	{
		return x + y + z;
	}
} point;

typedef struct ray
{
	point position;
	point direction;
	float length;
	float strength;
	rgb ray_rgb;
	ray()
	{
		position = point();
		direction = point();
		length = 0.;
		strength = 0.;
	}
	ray(point p, point d, float l, float s)
	{
		position = p;
		direction = d;
		length = l;
		strength = s;
	}
} ray;

typedef struct ball
{
	float r;         // 半径
	point center;    // 球心
	rgb c;
	float n;         // 折射率
	float reflect;   // 反射率
	float cast;      // 透射率
	ball()
	{
		r = 0.;
		center = point();
		c = rgb();
		n = 0.;
		reflect = 0.;
		cast = 0.;
	}
	ball(float rr, point cc, rgb rgbb, float ref)
	{
		r = rr;
		center = cc;
		c = rgbb;
		reflect = ref;
	}
} ball;

typedef struct cube
{
	// boundary fore
	float x0;
	float x1;
	// boundary side
	float y0;
	float y1;
	// boundary up
	float z0;
	float z1;
	// color
	color fore;
	color up;
	color side;
} cube;

ball ball1, ball2, ball3;

void ChangeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float size = 7.;

	//设置裁剪空间
	glOrtho(-size, size, -size, size, -size, size);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void show_point(point p, rgb color, float size)
{
	glColor3f(color.r, color.g, color.b);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.z);
	glEnd();
}

point obj_ball(ray R, ball B, int flag)
{
	point answer = point();
	point p = B.center - R.position;
	float cosine = ((p * R.direction) / p.length() / R.direction.length());
	float d = p.length() * sqrtf(1 - pow(cosine, 2.));
	if (d > B.r)
		return answer;
	float length = sqrtf(pow(p.length(), 2.) - pow(d, 2.)) - sqrtf(pow(B.r, 2.) - pow(d, 2.));
	answer = R.position + ((R.direction / R.direction.length()) * length);
	answer.flag = flag;
	return answer;
}

point obj_cube(ray R, cube C, int flag)
{
	point answer = point();
	return answer;
};

point obj(ray R)
{
	point p = point();
	point mid = point();
	mid = obj_ball(R, ball1, 1);
	if (mid.flag == 1)
		p = mid;
	mid = obj_ball(R, ball2, 2);
	if (mid.flag == 2 && ((mid - R.position).length() < (p - R.position).length()))
		p = mid;
	mid = obj_ball(R, ball3, 3);
	if (mid.flag == 3 && ((mid - R.position).length() < (p - R.position).length()))
		p = mid;
	return p;
}

rgb RayTrace(ray R, int k)
{
	rgb value = rgb(0., 0., 0.);
	rgb ref_rgb;
	if (R.strength < THRESHOLD)
	{
		value = rgb(0., 0., 0.);
		return value;
	}
	if(R.length > MAXDEPTH)
	{
		value = rgb(0., 0., 0.);
		return value;
	}
	point p = obj(R);
	ray NewR = ray();
	point ref_dir = point();
	if (p.flag == 0)
		return value;
	else if (p.flag == 1)
	{
		value = ball1.c;
		point n = p - ball1.center;
		n = n / n.length();
		ref_dir = R.direction - (n * (n * R.direction) * 2.);
		NewR = ray(p, ref_dir, R.length + (p - R.position).length(), R.strength * ball1.reflect);
		ref_rgb = RayTrace(NewR, k+1);
	}
	else if (p.flag == 2)
	{
		value = ball2.c;
		point n = p - ball2.center;
		n = n / n.length();
		ref_dir = R.direction - (n * (n * R.direction) * 2.);
		NewR = ray(p, ref_dir, R.length + (p - R.position).length(), R.strength * ball2.reflect);
		ref_rgb = RayTrace(NewR, k + 1);
	}
	else if (p.flag == 3)
	{
		value = ball3.c;
		point n = p - ball3.center;
		n = n / n.length();
		ref_dir = R.direction - (n * (n * R.direction) * 2.);
		NewR = ray(p, ref_dir, R.length + (p - R.position).length(), R.strength * ball3.reflect);
		ref_rgb = RayTrace(NewR, k + 1);
	}
	if (k > 0)
	{
		int a = 0;
	}
	value = (value * R.strength * exp(-0.3 * NewR.length)) + ref_rgb;
	return value;
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float width = 5.;
	float height = 5.;
	
	//绘制绿色线框
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(-1.f, -1.f);
	glVertex2f(width+1.f, -1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(width + 1.f, -1.f);
	glVertex2f(width + 1.f, height + 1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(width + 1.f, height + 1.f);
	glVertex2f(-1.f, height + 1.f);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-1.f, -1.f);
	glVertex2f(-1.f, height + 1.f);
	glEnd();

	point eye = point(2.5, -2.5, 2.5);
	rgb ball1_rgb = rgb(1., 0., 1.);
	rgb ball2_rgb = rgb(0., 0.5, 0.);
	rgb ball3_rgb = rgb(0., 0., 1.);
	point center1 = point(4., 3.5, 3.);
	point center2 = point(2., 2., 2.5);
	point center3 = point(1.5, 1., 1.5);

	ball1 = ball(1., center1, ball1_rgb, 0.75);
	ball2 = ball(1.5, center2, ball2_rgb, 0.75);
	ball3 = ball(1., center3, ball3_rgb, 0.75);

	float dist = .01;
	point pix;
	rgb pix_rgb = rgb(1., 0., 0.);
	for (float i = 0.; i < width; i += dist)
	{
		for (float j = 0.; j < height; j += dist)
		{
			if (i > 2.4 && i < 2.6 && j > 2.4 && j < 2.6)
			{
				int a = 0;
			}
			pix = point(i, 0., j);
			ray eye_sight = ray(eye, (pix - eye) / (pix - eye).length(), 0, 1.);
			pix_rgb = RayTrace(eye_sight, 0);
			show_point(pix, pix_rgb, 2.5);
		}
	}
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("简单光线跟踪");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(10000, 10000);
	glutMainLoop();

	return 0;
}

