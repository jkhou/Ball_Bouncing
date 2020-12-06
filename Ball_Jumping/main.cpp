/*
����һ����״STLģ�ͣ��뾶5mm������XOY���ϣ�ͨ�����ͻȻ�ƶ���������ٶȺ�����ǣ�
�������ٶ�Ϊ-Z����ÿ���䵽XOY���ϵ��𣬵���ǰ����ٶ�˥��ϵ��Ϊ0.3��
ֱ���ٶȾ���ֵ�ӽ����ٶȵ�1/100����ֹ��������ά������ʵ�ָù��̣�����ʾ���ĵĹ켣����
*/
#include <GL/glut.h>
#include <cmath>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <time.h>

using namespace std;
//ɫ��ȫ�ֳ���
GLfloat WHITE[] = { 1, 1, 1 };    //��ɫ
GLfloat RED[] = { 1, 0, 0 };    //��ɫ
GLfloat GREEN[] = { 0, 1, 0 };    //��ɫ
GLfloat MAGENTA[] = { 1, 0, 1 };  //���

int x11 = 0;
int x22 = 0;
int y11 = 0;
int y22 = 0;

clock_t time1;
clock_t time2;

double mouseX1;
double mouseY1;

double mouseX2;
double mouseY2;

double mouseVX;
double mouseVY;

int flag = 0;

//С�����ĵ�λ�ã�ȫ�ֱ�����
vector<vector<double>> centerList;

//������ࣺˮƽ�ƶ��뾶Ϊ10�������¼���ֱ�ƶ�
class Camera {
public:
	double theta;      //ȷ��x��z��λ��
	double y;          //yλ��
	double dTheta;     //�Ƕ�����
	double dy;         //����y����
public:
	//�๹�캯����Ĭ�ϳ�ʼ���÷�
	Camera() : theta(0), y(4), dTheta(0.04), dy(0.2) {}
	//�෽��
	double getX() { return 30 * cos(theta); }
	double getY() { return y; }
	double getZ() { return 30 * sin(theta); }
	void moveRight() { theta += dTheta; }
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy) y -= dy; }
};
 
//���ඨ��
//�뾶����ɫ�����߶�
//x��z�̶�
//��lame bouncing algorithm
//ÿ֡�����ƶ�0.05��λ

float gx = 0;
float gy = 3;
float gz = 0;


class Ball {
	//�������
	double radius;
	GLfloat* color;
	double maximumHeight;
	double x;
	double y;
	double z;
	double vx;
	double vy;
	int direction;   //����
public:
	//���캯��
	Ball(double r, GLfloat* c, double h, double x, double z) :
		radius(r), color(c), maximumHeight(h), direction(-1),
		y(h), x(x), z(z), vx(0.3), vy(0) {
	}

	double getX()
	{
		return x;
	}

	double getY()
	{
		return y;
	}

	double getZ()
	{
		return z;
	}

	void updateX(double mx)
	{
		x = mx;
	}

	void updateY(double my)
	{
		y = my;
	}

	void updateZ(double mz)
	{
		z = mz;
	}

	void updateVX(double mvx)
	{
		vx = mvx;
	}

	void updateVY(double mvy)
	{
		vy = mvy;
	}

	void updateDirection(int direct)
	{
		direction = direct;
	}
	//���ºͻ��Ʒ���
	void update() {
		//�����˶�
		x += vx;
		vy += direction * 0.0098;
		y += vy;
		gx = x;
		gy = y;
		gz = z;
		if (vy > 0 && vy < 0.005) {

			direction = -1;
		}
		else if (y < radius) {
			y = radius;
			vy = -vy;
			vy = vy * 0.80;
			vx = vx * 0.85;
		}

		//cout << "x = " << x << endl;
		//cout << "y = " << y << endl;
		//cout << "z = " << z << endl;

		glPushMatrix();
		//��������ÿ����Ĳ��ʲ���
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		//������
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();

	}
};

//���̸���x��zƽ��ֲ�
//���Դλ������Ϊ(4, 3, 7).
class Checkerboard {
	int displayListId;
	int width;
	int depth;
public:
	//���캯��
	Checkerboard(int width, int depth) : width(width), depth(depth) {}

	//����X
	double centerx() { return width / 2; }

	//����Y
	double centerz() { return depth / 2; }

	//��������
	void create() {
		displayListId = glGenLists(1);     //ÿ����ʾ�б��Ӧ1����š�����������
										   //�½�������
		glNewList(displayListId, GL_COMPILE);   //����������װ����ʾ�б�����ʾ

		//��Դλ�ò���
		GLfloat lightPosition[] = { 4, 3, 7, 1 };
		
		//���ù�Դλ��
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		//��ʼ�����ı���
		glBegin(GL_QUADS);

		//����������
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) {
				//����ÿ�����ӵĲ�������
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
					(x + z) % 2 == 0 ? RED : WHITE);
				//�ı��ε�4��������
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}
	//���б��Ż������̸�
	void draw() {
		glCallList(displayListId);
	}
};

//ȫ�ֱ��������̸������3���������
Checkerboard checkerboard(100, 100);
Camera camera;
//����3��С�������

double initBallH = 8;
double initBallX = 5;
double initBallZ = 15;

Ball balls[] = {

	Ball(0.5, WHITE, initBallH, initBallX, initBallZ)
};


//�Զ����ʼ������
void init() {
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	//����ɢ��;�����Ϊ�׹�
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	//����ǰ����ĸ߹⾵����Ϊ�׹�
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	//����ǰ����ɢ��ⷴ��ϵ��
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	//����ƹ�
	glEnable(GL_LIGHTING);
	//��0#��
	glEnable(GL_LIGHT0);
	//�������̸�
	checkerboard.create();
}

//�Զ�����ƺ���,ͨ������Ƹ�����display����������Լ�
void display() {
	//���ǰһ֡��ͼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//װ�뵥λ��
	glLoadIdentity();
	
	//�����ӽǡ������������
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),    //�����λ��
		gx,  3 , gz,   //��������
		0.0, 1.0, 0.0);   //�������������ʸ��
	
	//��������
	checkerboard.draw();

	//����С��
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
		if (flag == 0)
		{
			//���û�����Ķ���������С��һֱ����������λ��
			balls[i].updateX(initBallX);
			balls[i].updateY(initBallH);
			balls[i].updateZ(initBallZ);
			balls[i].updateDirection(0);
			balls[i].update();
		}
		else if (flag == 1)
		{
			//����λ�ò���ͼ
			balls[i].updateDirection(-1);
			balls[i].update();

			//����С��ĳ�ʼ�ٶ�Ϊ�����ƶ��ٶ�
			//balls[i].updateVX(mouseVX);
			//balls[i].updateVY(mouseVY);

			//����һ��vector�����浱ǰʱ��С���xyz
			vector<double> cent;
			cent.clear();
			cent.push_back(balls[i].getX());
			cent.push_back(balls[i].getY());
			cent.push_back(balls[i].getZ());

			//��ӵ�ȫ�ֱ���centerList��
			centerList.push_back(cent);

			//����С���˶��Ĺ켣
			glColor3f(1.0f, 0.0f, 0.0f);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			for (int i = 0; i < centerList.size(); i++)
			{
				//����Բ�ĵ�
				glVertex3f(centerList[i][0], centerList[i][1], centerList[i][2]);
			}
			glEnd();

		}

	}
	glutSwapBuffers();
}

//���ڵ�����Сʱ���õĺ���
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

//�Զ����ʱ������
void timer(int v) {
	//����ʱ������ʱ�����õĺ���
	glutPostRedisplay();
	//������һ�μ�ʱ���Ĳ���
	glutTimerFunc(1000 / 60, timer/*������*/, v);
}

//���̴�����
void onKey(int key, int, int) {
	//��������������
	switch (key) {
	case GLUT_KEY_LEFT: camera.moveLeft(); break;
	case GLUT_KEY_RIGHT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
}

//ע������¼�����ʱ�Ļص�����
void mouseCB(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		time1 = clock(); //��ȡ��갴��ʱ��ʱ�䣨���룩
		cout << "time1:" << time1 << endl;
		mouseX1 = x;
		mouseY1 = y;
		cout << "1 Mouse x:" << x << endl;
		cout << "1 Mouse y:" << y << endl;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		time2 = clock(); //��ȡ����ɿ�ʱ��ʱ�䣨���룩
		cout << "time2:" << time2 << endl;
		mouseX2 = x;
		mouseY2 = y;
		cout << "2 Mouse x:" << x << endl;
		cout << "2 Mouse y:" << y << endl;

		//���������x��y������϶��ٶ�
		mouseVX = (mouseX1 - mouseX2) / (time2 * 1.0 - time1 * 1.0);
		mouseVY = (mouseY1 - mouseY2) / (time2 * 1.0 - time1 * 1.0);

		mouseVX = mouseVX / 2;
		mouseVY = mouseVY / 2;

		cout << "mouseVX = " << mouseVX << endl;
		cout << "mouseVY = " << mouseVY << endl;

		flag = 1;
		balls[0].updateVX(mouseVX);
		balls[0].updateVY(mouseVY);
	}

	glutPostRedisplay();
}

//��������ƶ�ʱ��Ļص�����, x��y����ǰ��������
void mouseMove(int x, int y)
{
	//mouseX = x;
	//mouseY = y;
	//cout << "Current Mouse x:" << x << endl;
	//cout << "Current Mouse y:" << y << endl;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("��Ծ����");
	glutDisplayFunc(display);   //������ʾ����
	glutReshapeFunc(reshape);   //���ô��ڵ�����С�ĺ���
	glutSpecialFunc(onKey);   //���ð���������
	glutMouseFunc(mouseCB);   //ע������¼�����ʱ�Ļص�����
	glutMotionFunc(mouseMove);  //��������ƶ�ʱ��Ļص�����
	glutTimerFunc(100, timer, 0);  //���ü�ʱ������--ÿ100ms������1��
	init();//�Զ����ʼ������
	glutMainLoop();//����opengl��ѭ��

	return 0;
}
