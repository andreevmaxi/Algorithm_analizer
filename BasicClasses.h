#pragma once

struct Point {
	double x;
	double y;

	Point() = default;
	Point(const Point & other) = default;
	Point(Point && other) = default;
	Point& operator=(const Point & other) = default;
	Point& operator=(Point&&) = default;
	Point(double x, double y) : x(x), y(y) {}

	void set() const {
		glVertex2d(x, y);
	}

	void pos() const {
		glRasterPos2f(x, y);
	}
};

struct KeyEvent {
	KeyEvent() = default;
	KeyEvent(const KeyEvent& other) = default;
	KeyEvent(KeyEvent&& other) = default;
	KeyEvent& operator=(const KeyEvent& other) = default;
	KeyEvent& operator=(KeyEvent&&) = default;

	int key = 0;
	int action = 0;
};

enum KeyStat {
	PRESS = GLFW_PRESS,
	RELEASE = GLFW_RELEASE
};

enum OtherKeyNum {
	ESCAPE = 033	
};

struct EventInfo {
	Point MousePos;
	KeyEvent KeyId;

	EventInfo() = default;
	EventInfo(const Point MPos, const KeyEvent NewEv) : MousePos(MPos), KeyId(NewEv) {}
	EventInfo(const Point MPos, const int Key, const int Act) : MousePos(MPos), KeyId({ Key, Act }) {}
	EventInfo(const double MPosx, const double MPosy, const int Key, const int Act) : 
		MousePos({ MPosx, MPosy }), KeyId({ Key, Act }) {}
};

struct AbstractWindow {
	virtual void Draw() = 0;
	virtual void GetEvent(const EventInfo& status) = 0;
};

class ViewPort : AbstractWindow {
	int x, y, width, height;
public:
	void Change(Point LeftBottomPoint, Point RightTopPoint, int WindowHeight, int WindowWidth) {
		x = 2 * ((LeftBottomPoint.x + 1) * WindowWidth * 2);
		y = 2 * ((1 - LeftBottomPoint.y) * WindowHeight * 2);
		width  = ((RightTopPoint.x - LeftBottomPoint.x) * WindowWidth);
		height = ((RightTopPoint.y - LeftBottomPoint.y) * WindowHeight);
	}

	void SetView() {
		glViewport(x, y, width, height);
	}
};

struct Color {
	double red;
	double green;
	double blue;

	Color() = default;
	Color(double red, double green, double blue) : red(red), green(green), blue(blue) {}
	Color(const Color& other) = default;

	void peek() const {
		glColor3d(red, green, blue);
	}

	void Clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(red, green, blue, 0);
	}
};

void QuadDraw(Point LeftBottom, Point RightTop) {
	glBegin(GL_QUAD_STRIP);
		LeftBottom.set();
		glVertex2f(LeftBottom.x, RightTop.y);
		glVertex2f(RightTop.x, LeftBottom.y);
		RightTop.set();
	glEnd();
	glFlush();
}

void LineDraw(Point A, Point B) {
	glBegin(GL_LINES);
	A.set();
	B.set();
	glEnd();
}

void TriangleDraw(Point A, Point B, Point C) {
	glBegin(GL_TRIANGLES);
	A.set();
	B.set();
	C.set();
	glEnd();
}
