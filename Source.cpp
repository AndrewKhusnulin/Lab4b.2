#include "iostream"
#include "mutex"
#include "thread"
#include "windows.h"
using namespace std;
class Figure
{
protected:
	int x, y;
	int vx, vy;
public:

	Figure(int x, int y, int vx, int vy);
	int get_X() { return x; }
	int get_Y() { return y; }
	virtual void step();
};

Figure::Figure(int x, int y, int vx, int vy)
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
}
void Figure::step()
{

	this->x += vx; this->y += vy;
	(x > 100) || (x < 0) ? vx = -vx : NULL;
	(y > 100) || (y < 0) ? vy = -vy : NULL;
};
class rectangle : public Figure
{
protected:
	int height;
	int length;
public:
	rectangle(int x, int y, int vx, int vy, int a, int b);
	~rectangle() {};
};
rectangle::rectangle(int x, int y, int vx, int vy, int a, int b) : Figure(x, y, vx, vy)
{
	this->height = a;
	this->length = b;
};
class circle : public Figure
{
protected:
	int radius;
public:
	circle(int x, int y, int vx, int vy, int a);
	~circle() {};
};
circle::circle(int x, int y, int vx, int vy, int a) :Figure(x, y, vx, vy)
{
	this->radius = a;
}
mutex m;
HANDLE hEvent;
void Animation(Figure *p, int n)
{
	WaitForSingleObject(hEvent, INFINITE);
	//SetEvent(hEvent);
	while (WaitForSingleObject(hEvent,0)==WAIT_OBJECT_0)
	{
		m.lock();
		cout << "Координаты f[" << n << "] = (" << p->get_X() << ";" << p->get_Y() << ")" << endl;
		m.unlock();
		p->step();
		if (GetAsyncKeyState(VK_CONTROL)) break;
	}
	//ResetEvent(hEvent);
}

Figure *pF[4];

void main()
{
	setlocale(LC_ALL, "Russian");
	hEvent = CreateEvent(0, 1, 0, L"Animation");

	pF[0] = new rectangle(10, 20, 1, -1, 5, 10);
	pF[1] = new rectangle(20, 40, -1, 1, 10, 20);
	pF[2] = new circle(20, 30, 1, 1, 5);
	pF[3] = new circle(30, 60, -1, -1, 10);
	thread s0(Animation, pF[0], 0);
	thread s1(Animation, pF[1], 1);
	thread s2(Animation, pF[2], 2);
	thread s3(Animation, pF[3], 3);
	s0.join();
	s1.join();
	s2.join();
	s3.join();
	system("pause");
};
