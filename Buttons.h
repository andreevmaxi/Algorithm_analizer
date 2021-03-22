#pragma once

struct Button : AbstractWindow {
    int id;
    Point Left;
    Point Right;
    Color BColor; //Button color
    Color VColor; //Visited button color
    Color PColor; //Pressed button color
    Color TColor; // Text color
    int PressMode;
    std::string ButtonText;
    WindowNode* Place; // place of our butto
    void (*BindedFunc) (void);

    bool ButtonPaint() {
        switch (PressMode)
        {
        case(0): {
            BColor.peek();
        }
               break;
        case(1): {
            VColor.peek();
        }
               break;
        case(2): {
            PColor.peek();
        }
               break;
        }
        QuadDraw(Left, Right);

        DrawText({(Right.x + Left.x)/2, (Right.y + Left.y)/2}, 
            ButtonText, 
            TColor, 
            Place->MainWindow->Events.WindowHeight,
            Place->MainWindow->Events.WindowWidth,
            9);

        return 0;
    }
};