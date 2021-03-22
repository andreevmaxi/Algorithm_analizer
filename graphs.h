#pragma once

#define ResourcesSize 1000

long long* UsedRes[ResourcesSize];
int UsedNow = 0;

template<typename XType, typename YType>
struct GraphLine {
    XType* XArr;
    int XSize;
    YType* YArr;
    int YSize;
    Color LineColor;
};

template<typename XType, typename YType>
class Graph : WindowNode {
    Point Start;
    Point Final;
    std::vector<GraphLine<XType, YType>> GLinesBuffer;
    Color Background;
    Color Arrow;

    bool GraphBackground() {
        Background.peek();
        QuadDraw(Start, Final);

        Arrow.peek();
        LineDraw( Start, {Final.x, Start.y});
        //glVertex2f(x + 0.03 * scale, y + 0.03 * scale);
        //glVertex2f(x + 0.97 * scale, y + 0.03 * scale);


        Arrow.peek();
        TriangleDraw({Final.x, Start.y},
            {Final.x - xdelta, Start.y + ydelta},
            { Final.x - xdelta, Start.y - ydelta });
        //glVertex2f(x + 0.97 * scale, y + 0.03 * scale);
        //glVertex2f(x + 0.95 * scale, y + 0.02 * scale);
        //glVertex2f(x + 0.95 * scale, y + 0.04 * scale);
        

        Arrow.peek();
        LineDraw(Start, { Start.x, Final.y });
        //glVertex2f(x + 0.03 * scale, y + 0.03 * scale);
        //glVertex2f(x + 0.03 * scale, y + 0.97 * scale);

        Arrow.peek();
        TriangleDraw({ Start.x, Final.y },
            { Start.x - xdelta, Final.y + ydelta },
            { Start.x - xdelta, Final.y - ydelta });
        //glVertex2f(x + 0.03 * scale, y + 0.97 * scale);
        //glVertex2f(x + 0.02 * scale, y + 0.95 * scale);
        //glVertex2f(x + 0.04 * scale, y + 0.95 * scale);

        return 0;
    }

    template<typename YType, typename XType>
    bool GraphLines(float x, float y, float scale, XType* XArr, int XSize, YType* YArr, int YSize, Color GraphColor) {
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

        GraphColor.set();
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < XSize; ++i) {
            glVertex2f(x + 0.03 * scale + (0.97 - 0.03) * ((double)*(XArr + i) / (double)XMaxVal) * scale,
                y + 0.03 * scale + (0.97 - 0.03) * ((double)*(YArr + i) / (double)YMaxVal) * scale);
        }
        glEnd();
        return 0;
    }
public:
    Point Start;
    Point Final;

    Graph(Point Start, Point Final, Color Background = { 1, 1, 1 }, Color Arrow = { 0, 0, 0 }) {

    }
    virtual void Draw() override {
        GraphBackground();

    }
};