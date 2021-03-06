#include "pch.h"
#include<gl/glut.h>
#include<stdio.h>
#include<math.h>
#define PI 3.1415926535

typedef float point[3];

typedef struct triangle
{
	point p1;
	point p2;
	point p3;
} triangle;

typedef float matrix[3][3];

void matrix_m(matrix a_matrix,  point b, point c)
// c = a*b
{
	float temp;
	int cx, bx;
	for (cx = 0; cx < 3; cx++)
	{
		temp = 0.;
		for (bx = 0; bx < 3; bx++)
		{
			temp += a_matrix[cx][bx] * b[bx];
		}
		c[cx] = temp;
	}
}

void rotation(triangle * obj_addr, float angle)
{
	matrix rot =
	{ { cos(angle * PI / 180.), -sin(angle * PI / 180.),    0.},
	  { sin(angle * PI / 180.),  cos(angle * PI / 180.),    0.},
	  { 0.,                                          0.,    1.}
	};
	point temp;
	for (int i = 0; i < 3; i++)
		temp[i] = obj_addr->p1[i];
	matrix_m(rot, temp, obj_addr->p1);
	for (int i = 0; i < 3; i++)
		temp[i] = obj_addr->p2[i];
	matrix_m(rot, temp, obj_addr->p2);
	for (int i = 0; i < 3; i++)
		temp[i] = obj_addr->p3[i];
	matrix_m(rot, temp, obj_addr->p3);
}

void translation(triangle * obj_addr, float tx, float ty)
{
	matrix rot =
	{ { 1., 0.,  tx},
	  { 0., 1., ty },
	  { 0., 0., 1. }
	};
	point temp;
	for (int i = 0; i < 3; i++)
		temp[i] = obj_addr->p1[i];
	matrix_m(rot, temp, obj_addr->p1);
	for (int i = 0; i < 3; i++)
		temp[i] = obj_addr->p2[i];
	matrix_m(rot, temp, obj_addr->p2);
	for (int i = 0; i < 3; i++)
		temp[i] = obj_addr->p3[i];
	matrix_m(rot, temp, obj_addr->p3);
}

void scaling(triangle * obj_addr, float s1, float s2, float s3)
{
	obj_addr->p1[2] *= s1;
	obj_addr->p2[2] *= s2;
	obj_addr->p3[2] *= s3;
}

void init_triangle(triangle * obj_addr, float p11, float p12, float p21, float p22, float p31, float p32, float s)
{
	obj_addr->p1[0] = p11;
	obj_addr->p1[1] = p12;
	obj_addr->p2[0] = p21;
	obj_addr->p2[1] = p22;
	obj_addr->p3[0] = p31;
	obj_addr->p3[1] = p32;
	obj_addr->p1[2] = obj_addr->p2[2] = obj_addr->p3[2] = 1.;
}


void SetupRC(void)
{
	//设置窗口背景颜色为白色
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//绘制三角形
void DrawTriangle(triangle obj)
{
	glBegin(GL_TRIANGLES);
	glVertex2f(obj.p1[0] * obj.p1[2], obj.p1[1] * obj.p1[2]);
	glVertex2f(obj.p2[0] * obj.p2[2], obj.p2[1] * obj.p2[2]);
	glVertex2f(obj.p3[0] * obj.p3[2], obj.p3[1] * obj.p3[2]);
	glEnd();
}

void ChangeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//设置裁剪空间
	if (w <= h)
		glOrtho(-200.0f, 200.0f, -200.0f*h / w, 200.0f*h / w, -200.0f, 200.0f);
	else
		glOrtho(-200.0f*w / h, 200.0f*w / h, -200.0f, 100.0f, -200.0f, 200.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//设置当前操作矩阵为模型视图矩阵，并复位为单位矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//绘制黑色的坐标轴
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(-200.0f, 0.0f);
	glVertex2f(200.0f, 0.0f);
	glVertex2f(0.0f, -200.0f);
	glVertex2f(0.0f, 200.0f);
	glEnd();

	//绘制出第一个红色的三角形
	triangle a;
	init_triangle(&a, 0., 0., 40., 0., 20., 40., 1.);
	glColor3f(1.0f, 0.0f, 0.0f);
	DrawTriangle(a);

	//先逆时针旋转200度角，颜色设置为绿色
	rotation(&a, 200.);
	glColor3f(0.0f, 1.0f, 0.0f);
	DrawTriangle(a);

	//继续沿x轴负反向平移60单位,沿y轴平移20单位，颜色设置为黄色
	translation(&a, -60., 20.);
	glColor3f(1.0f, 1.0f, 0.0f);
	DrawTriangle(a);


	//最后三角形的以两倍的尺度放大，颜色设置为蓝色
	scaling(&a, 2., 2., 2.);
	glColor3f(0.0f, 0.0f, 1.0f);
	DrawTriangle(a);



	//交换两个缓冲区的指针
	//功能：防止复杂绘制时屏幕闪烁
	glutSwapBuffers();

}

int main(int argc, char *argv[])
{

	//glutInit is used to initialize the GLUT library
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("作业2 常见图形变换表示");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);


	SetupRC();
	glutMainLoop();

	return 0;
}
