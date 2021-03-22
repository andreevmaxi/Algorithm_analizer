#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <string>
#include <queue>
#include <GL/freeglut.h>
#include "KeyDefines.h"
#include "BasicClasses.h"
#include "Text.h"

class EventManager {
	GLFWwindow* window;

	Point getMousePos(GLFWwindow* window) {
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		MousePos.x = 2 * (x - WindowWidth / 2) / WindowWidth;
		MousePos.y = (2 * (WindowHeight / 2 - y) ) / WindowHeight;
		return MousePos;
	}

	static void KeysCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Events.push({ MousePos, key, action });
	}

	static void MouseKeysCallback(GLFWwindow* window, int button, int action, int mods) {
		Events.push({ MousePos, button, action });
	}

	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		MousePos = { xpos, ypos };
		Events.push({ MousePos, -1, 0 });
	}

public:
	int WindowHeight;
	int WindowWidth;
	static Point MousePos;
	static std::queue<EventInfo> Events;

	EventManager(GLFWwindow* window, int WindowWidth, int WindowHeight) :
		window(window),	
		WindowWidth(WindowWidth),	
		WindowHeight(WindowHeight) {
		MousePos;
		glfwSetKeyCallback(window, KeysCallback);
		glfwSetMouseButtonCallback(window, MouseKeysCallback);
		glfwSetCursorPosCallback(window, CursorPositionCallback);
	}

	EventInfo getEvent() {
		EventInfo Stat;
		if (!Events.empty()) {
			Stat.KeyId = Events.front().KeyId;
			Events.pop();
		}
		Stat.MousePos = getMousePos(window);
		return Stat;
	}
};

std::queue<EventInfo> EventManager::Events;
Point EventManager::MousePos;

class WindowsTree;

class WindowNode {
public:
	WindowsTree* MainWindow;
	AbstractWindow* Window;
	std::vector<WindowNode*> ChildWindows;

	WindowNode(AbstractWindow* NewWindow, WindowsTree* MainWindow) : Window(NewWindow), MainWindow(MainWindow) {};

	~WindowNode() {
		for (int i = 0; i < ChildWindows.size(); ++i)
			ChildWindows[i]->~WindowNode();
	}

	void Draw() {
		Window->Draw();
		for (int i = 0; i < ChildWindows.size(); ++i)
			ChildWindows[i]->Draw();
	}

	void PollEvents(const EventInfo& Event) {
		Window->GetEvent(Event);
		for (int i = 0; i < ChildWindows.size(); ++i)
			ChildWindows[i]->PollEvents(Event);
	}

	WindowNode* NewChild(AbstractWindow* NewWindow) {
		WindowNode* Child = nullptr;
		ChildWindows.push_back(Child = new WindowNode(NewWindow, MainWindow));

		return Child;
	}
};

class WindowsTree {
public:
	EventManager Events;
	GLFWwindow* Window;
	Color WindowColor;
	int WindowHeight;
	int WindowWidth;
	std::vector<WindowNode*> ChildWindows;

	WindowsTree(GLFWwindow* Window, Color WindowColor, int WindowHeight, int WindowWidth) :
		Window(Window),
		WindowColor(WindowColor),
		WindowHeight(WindowHeight),
		WindowWidth(WindowWidth), Events(Window, WindowWidth, WindowHeight) {}

	~WindowsTree() {
		for (int i = 0; i < ChildWindows.size(); ++i)
			ChildWindows[i]->~WindowNode();
	}
	WindowsTree(const WindowsTree& other) = delete;
	WindowsTree(WindowsTree&& other) = delete;
	WindowsTree& operator= (const WindowsTree& other) = delete;
	WindowsTree& operator= (WindowsTree&& other) = delete;

	WindowNode* NewChild(AbstractWindow* NewWindow) {
		WindowNode* Child = nullptr;
		ChildWindows.push_back(Child = new WindowNode(NewWindow, this));

		return Child;
	}

	void Draw() {
		WindowColor.Clear();
		for (int i = 0; i < ChildWindows.size(); ++i)
			ChildWindows[i]->Draw();
		glfwSwapBuffers(Window);
	}

	void ProcedureEvents() {
		glfwPollEvents();
	}

	void PollEvents(const EventInfo& Event) {
		for (int i = 0; i < ChildWindows.size(); ++i)
			ChildWindows[i]->PollEvents(Event);
	}

	bool ShouldClose() {
		return glfwWindowShouldClose(Window);
	}
};

GLFWwindow* CreateContextWindow(int argc, char** argv, const char* name, int hieght, int width) {
	glutInit(&argc, argv);
	GLFWwindow* window;

	if (!glfwInit()) {
		fprintf(stderr, "Error of initialisation of KOROBKA");
		return nullptr;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(width, hieght, name, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "KOROBKA can't open the window");
		glfwTerminate();
		return nullptr;
	}

	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);

	return window;
}

WindowsTree* KorobkaInit(int argc, char** argv, Color color, const char* name, int hieght, int width) {
	GLFWwindow* window = CreateContextWindow(argc, argv, name, hieght, width);

	if (window == nullptr)
		return nullptr;

	return new WindowsTree(window, color, hieght, width);
}

template <int Corners>
class Shape : WindowNode {
public:
	Point coord[Corners];

public:
	Shape() = delete;

	Shape(const Point points[Corners]) {
		for (int i = 0; i < Corners; ++i)
			coord[i] = points[i];
	}

	virtual void GetEvent(const EventInfo& status) = 0;

	virtual void draw() override {

		glBegin(GL_POLYGON);
		for (int i = 0; i < 4; ++i)
			coord[i].set();
		glEnd();
		glFlush();
	}
};

#include "Buttons.h"