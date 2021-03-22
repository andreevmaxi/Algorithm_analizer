#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <random>
#include <string>
#include <ctime>
#include <GL/freeglut.h>
#include "src/Sortings.h"

#define GLFW_HAND_CURSOR   0x00036004

GLFWcursor* HandCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
int NowWWidth  = 640;
int NowWHeight = 480;

struct Color {
    float R;
    float G;
    float B;
};

long long CountSort::CounterCmp = 0;
long long CountSort::CounterSwap = 0;

const long long MaxSortSize = 300;
const long long SortStep = 1;
const long long ResourcesSize = 102;
const long long ButtonsSize = 100;


struct Button {
    int id;
    float LeftX;
    float LeftY;
    float RightX;
    float RightY;
    Color BColor; //Button color
    Color VColor; //Visited button color
    Color PColor; //Pressed button color
    Color TColor; // Text color
    int PressMode;
    std::string ButtonText;
    void (*BindedFunc) (void);

    bool ButtonPaint() {
        switch (PressMode) 
        {
        case(0): {
            glColor3f(BColor.R, BColor.G, BColor.B);
        }
           break;
        case(1): {
            glColor3f(VColor.R, VColor.G, VColor.B);
        }
            break;
        case(2): {
            glColor3f(PColor.R, PColor.G, PColor.B);
        }
            break;
        }
        glBegin(GL_QUAD_STRIP);
        glVertex2f(LeftX, LeftY);
        glVertex2f(LeftX, RightY);
        glVertex2f(RightX, LeftY);
        glVertex2f(RightX, RightY);
        glEnd();

        glColor3f(TColor.R, TColor.G, TColor.B);

        static const size_t FHeight = 15,
            FWidth = 9;

        size_t BitmapWidth = FWidth * ButtonText.size();

        glRasterPos2f((RightX + LeftX) / 2 - (static_cast<double>(BitmapWidth)) / NowWWidth, 
                      (RightY + LeftY) / 2 - (static_cast<double>(FHeight) / 2) / NowWHeight) ;

        for (char& i : ButtonText) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, i);
        }

        return 0;
    }
};

long long* UsedRes[ResourcesSize];
Button* UsedButtons[ButtonsSize];
Color UsedColors[ResourcesSize/3];
int UsedNow = 0;
int UsedB = 0;

bool Calculation(int NowSize, int NowSorting, long long* AnsCmp, long long* AnsSwap);

template<typename YType, typename XType>
bool Graph(float x, float y, float scale, XType* XArr, int XSize, YType* YArr, int YSize, Color GraphColor);

bool GraphBackgroung(float x, float y, float scale);

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void BubbleGraphCalculation(); 

void QuickGraphCalculation();

void MergeGraphCalculation();

void EraseGraph();

int main(int argc, char** argv){
    glutInit(&argc, argv);
    srand(static_cast<unsigned int>(time(0)));

    GLFWwindow* window;

    if (!glfwInit()) {
        fprintf(stderr, "Error of initialisation of GLFWn");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); 

    window = glfwCreateWindow(640, 480, "Algorithm Compare", NULL, NULL);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    if (window == NULL) {
        fprintf(stderr, "GLFW can't open the window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback); 
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    Button MergeSButton = { 0, -0.975, -0.4, -0.025, -0.10, {0, 0, 0.9},
    {0.2, 0.2, 0.9}, {0.35, 0.35, 0.9}, {0, 0, 0}, 0, "Merge Sort", MergeGraphCalculation };
    UsedButtons[MergeSButton.id] = &MergeSButton;
    ++UsedB;
    Button QuickSButton = { 1, 0.025, -0.4, 0.975, -0.10, {0, 0.9, 0},
    {0.2, 0.9, 0.2}, {0.35, 0.9, 0.35}, {0, 0, 0}, 0, "Quick Sort", QuickGraphCalculation};
    UsedButtons[QuickSButton.id] = &QuickSButton;
    ++UsedB;
    Button BubbleSButton = { 2, -0.975, -0.8, -0.025, -0.50, {0.9, 0, 0},
    {0.9, 0.2, 0.2}, {0.9, 0.35, 0.35}, {0, 0, 0}, 0, "Bubble Sort", BubbleGraphCalculation };
    UsedButtons[BubbleSButton.id] = &BubbleSButton;
    ++UsedB;
    Button EraseSButton = { 3, 0.025, -0.8, 0.975, -0.50, {0, 0, 0},
    {0.2, 0.2, 0.2}, {0.35, 0.35, 0.35}, {1, 1, 1}, 0, "Erase Graphs", EraseGraph};
    UsedButtons[EraseSButton.id] = &EraseSButton;
    ++UsedB;

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.95, 0.79, 0.59, 0);

        GraphBackgroung(-0.975, -0.05, 0.95);
        GraphBackgroung(0.025, -0.05, 0.95);
        for (int i = 0; i < UsedNow; i += 3){ 
            Graph(-0.975, -0.05, 0.95, UsedRes[i], (MaxSortSize / SortStep + 1),
                UsedRes[i + 1], (MaxSortSize / SortStep + 1), UsedColors[i/3]);
            Graph(0.025, -0.05, 0.95, UsedRes[i], (MaxSortSize / SortStep + 1),
                UsedRes[i+2], (MaxSortSize / SortStep + 1), UsedColors[i/3]);
        }
        for (int i = 0; i < UsedB; ++i) {
            UsedButtons[i]->ButtonPaint();
        }
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    if (UsedRes[UsedNow] != 0) {
        for (int i = 0; i < UsedNow; ++i)
            delete UsedRes[i];
    }
    glfwDestroyCursor(HandCursor);
    glfwTerminate();
    return 0;
}

bool Calculation(int NowSize, int NowSorting, long long* AnsCmp, long long* AnsSwap) {
    CountSort::CounterCmp = 0;
    CountSort::CounterSwap = 0;

    int* SortingArr = new int[NowSize];
    for (int i = 0; i < NowSize; ++i) {
        *(SortingArr + i) = rand() % 1000;
    }

    switch (NowSorting)
    {
    case 1: {
        BubbleSort(SortingArr, SortingArr + NowSize - 1, CmpWithCount);
    }
          break;
    case 2: {
        QuickSort(SortingArr, SortingArr + NowSize - 1, CmpWithCount);
    }
          break;
    case 3: {
        MergeSort(SortingArr, SortingArr + NowSize - 1, CmpWithCount);
    }
          break;
    default:
        break;
    }

    delete[] SortingArr;

    *AnsCmp = CountSort::CounterCmp;
    *AnsSwap = CountSort::CounterSwap;
    return 1;
}

bool GraphBackgroung(float x, float y, float scale) {
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 1 * scale, y);
    glVertex2f(x + 1 * scale, y + 1 * scale);
    glVertex2f(x, y + 1 * scale);
    glEnd();

    glColor3f(0, 0, 0);

    glBegin(GL_LINES);
    glVertex2f(x + 0.03 * scale, y + 0.03 * scale);
    glVertex2f(x + 0.97 * scale, y + 0.03 * scale);
    glEnd();

    glColor3f(0, 0, 0);

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0.97 * scale, y + 0.03 * scale);
    glVertex2f(x + 0.95 * scale, y + 0.02 * scale);
    glVertex2f(x + 0.95 * scale, y + 0.04 * scale);
    glEnd();

    glColor3f(0, 0, 0);

    glBegin(GL_LINES);
    glVertex2f(x + 0.03 * scale, y + 0.03 * scale);
    glVertex2f(x + 0.03 * scale, y + 0.97 * scale);
    glEnd();

    glColor3f(0, 0, 0);

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0.03 * scale, y + 0.97 * scale);
    glVertex2f(x + 0.02 * scale, y + 0.95 * scale);
    glVertex2f(x + 0.04 * scale, y + 0.95 * scale);
    glEnd();

    return 0;
}

template<typename YType, typename XType>
bool Graph(float x, float y, float scale, XType* XArr, int XSize, YType* YArr, int YSize, Color GraphColor) {
    glColor3f(1, 1, 1);
    
    if (XSize != YSize) {
        return 1; // error
    }

    YType YMaxVal = *YArr;
    XType XMaxVal = *XArr;
    for (int i = 1; i < XSize; ++i) {
        if (*(XArr + i) > XMaxVal) {
            XMaxVal = *(XArr + i);
        }
    }
    for (int i = 1; i < YSize; ++i) {
        if (*(YArr + i) > YMaxVal) {
            YMaxVal = *(YArr + i);
        }
    }

    glColor3f(GraphColor.R, GraphColor.G, GraphColor.B);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < XSize; ++i) {
        glVertex2f(x + 0.03 * scale + (0.97 - 0.03) * ((double)*(XArr + i) / (double)XMaxVal) * scale,
            y + 0.03 * scale + (0.97 - 0.03) * ((double)*(YArr + i) / (double)YMaxVal) * scale);
    }
    glEnd();
    return 0;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        BubbleGraphCalculation();
    }
    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        QuickGraphCalculation();
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        MergeGraphCalculation();
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        void EraseGraph();
    }

    //if (key == GLFW_PRESS) {
    //    
    //}
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    for (int i = 0; i < UsedB; ++i) {
        if ((UsedButtons[i]->LeftX <= 2 * (xpos - NowWWidth / 2) / NowWWidth ) && 
            (2 * (xpos - NowWWidth / 2) / NowWWidth <= UsedButtons[i]->RightX)) {
            if (UsedButtons[i]->LeftY <= (2 * (NowWHeight / 2 - ypos) / NowWHeight) &&
                (2 * (NowWHeight / 2 - ypos) / NowWHeight) <= UsedButtons[i]->RightY) {
                UsedButtons[i]->ButtonPaint();
                if (UsedButtons[i]->PressMode != 2) {
                    UsedButtons[i]->PressMode = 1;
                }
                glfwFocusWindow(window);
                glfwSetCursor(window, HandCursor);
            }
            else {
                UsedButtons[i]->PressMode = 0;
            }
        }
        else {
            UsedButtons[i]->PressMode = 0;
        }
    }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        for (int i = 0; i < UsedB; ++i) {
            if ((UsedButtons[i]->LeftX <= 2 * (xpos - NowWWidth / 2) / NowWWidth) &&
                (2 * (xpos - NowWWidth / 2) / NowWWidth <= UsedButtons[i]->RightX)) {
                if (UsedButtons[i]->LeftY <= (2 * (NowWHeight / 2 - ypos) / NowWHeight) &&
                    (2 * (NowWHeight / 2 - ypos) / NowWHeight) <= UsedButtons[i]->RightY) {
                    UsedButtons[i]->PressMode = 2;
                    UsedButtons[i]->ButtonPaint();
                    UsedButtons[i]->BindedFunc();
                }
                else {
                    UsedButtons[i]->PressMode = 0;
                }
            }
            else {
                UsedButtons[i]->PressMode = 0;
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        for (int i = 0; i < UsedB; ++i) {
            if ((UsedButtons[i]->LeftX <= 2 * (xpos - NowWWidth / 2) / NowWWidth) &&
                (2 * (xpos - NowWWidth / 2) / NowWWidth <= UsedButtons[i]->RightX)) {
                if (UsedButtons[i]->LeftY <= (2 * (NowWHeight / 2 - ypos) / NowWHeight) &&
                    (2 * (NowWHeight / 2 - ypos) / NowWHeight) <= UsedButtons[i]->RightY) {
                    UsedButtons[i]->ButtonPaint();
                    UsedButtons[i]->PressMode = 1;
                }
                else {
                    UsedButtons[i]->PressMode = 0;
                }
            }
            else {
                UsedButtons[i]->PressMode = 0;
            }
        }

    }
}

void BubbleGraphCalculation() {
    long long* SortingSizes = new long long[(MaxSortSize / SortStep + 1)];
    long long* SortingSizeCmp = new long long[(MaxSortSize / SortStep + 1)];
    long long* SortingSizeSwap = new long long[(MaxSortSize / SortStep + 1)];
    Color GraphColor;

    if (UsedNow >= ResourcesSize) {
        for (int i = 0; i < UsedNow; ++i) {
            delete UsedRes[i];
        }
        UsedNow = 0;
    }
    UsedRes[UsedNow] = SortingSizes;
    UsedRes[UsedNow + 1] = SortingSizeCmp;
    UsedRes[UsedNow + 2] = SortingSizeSwap;
    UsedColors[UsedNow / 3] = { 1, 0, 0 };
    UsedNow += 3;

    SortingSizes[0] = 0;
    SortingSizeCmp[0] = 0;
    SortingSizeSwap[0] = 0;

    for (long long i = 1; i <= (MaxSortSize / SortStep); ++i) {
        SortingSizes[i] = i * (SortStep);
        Calculation(SortingSizes[i], 1, (SortingSizeCmp + i), (SortingSizeSwap + i));
    }
}

void QuickGraphCalculation() {
    long long* SortingSizes = new long long[(MaxSortSize / SortStep + 1)];
    long long* SortingSizeCmp = new long long[(MaxSortSize / SortStep + 1)];
    long long* SortingSizeSwap = new long long[(MaxSortSize / SortStep + 1)];
    Color GraphColor;

    if (UsedNow >= ResourcesSize) {
        for (int i = 0; i < UsedNow; ++i) {
            delete UsedRes[i];
        }
        UsedNow = 0;
    }
    UsedRes[UsedNow] = SortingSizes;
    UsedRes[UsedNow + 1] = SortingSizeCmp;
    UsedRes[UsedNow + 2] = SortingSizeSwap;
    UsedColors[UsedNow / 3] = { 0, 1, 0 };
    UsedNow += 3;

    SortingSizes[0] = 0;
    SortingSizeCmp[0] = 0;
    SortingSizeSwap[0] = 0;

    for (long long i = 1; i <= (MaxSortSize / SortStep); ++i) {
        SortingSizes[i] = i * (SortStep);
        Calculation(SortingSizes[i], 2, (SortingSizeCmp + i), (SortingSizeSwap + i));
    }
}

void MergeGraphCalculation() {
    long long* SortingSizes = new long long[(MaxSortSize / SortStep + 1)];
    long long* SortingSizeCmp = new long long[(MaxSortSize / SortStep + 1)];
    long long* SortingSizeSwap = new long long[(MaxSortSize / SortStep + 1)];
    Color GraphColor;

    if (UsedNow >= ResourcesSize) {
        for (int i = 0; i < UsedNow; ++i) {
            delete UsedRes[i];
        }
        UsedNow = 0;
    }
    UsedRes[UsedNow] = SortingSizes;
    UsedRes[UsedNow + 1] = SortingSizeCmp;
    UsedRes[UsedNow + 2] = SortingSizeSwap;
    UsedColors[UsedNow / 3] = { 0, 0, 1 };
    UsedNow += 3;

    SortingSizes[0] = 0;
    SortingSizeCmp[0] = 0;
    SortingSizeSwap[0] = 0;

    for (long long i = 1; i <= (MaxSortSize / SortStep); ++i) {
        SortingSizes[i] = i * (SortStep);
        Calculation(SortingSizes[i], 2, (SortingSizeCmp + i), (SortingSizeSwap + i));
    }
}

void EraseGraph() {
    for (int i = 0; i < UsedNow; ++i) {
        delete UsedRes[i];
    }
    UsedNow = 0;
}