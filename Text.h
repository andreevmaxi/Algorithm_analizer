#pragma once

void DrawText(Point Start, const std::string& Text, Color TColor, int WinHeight, int WinWidth, static const size_t FHieght){
    TColor.peek();

    static const size_t FWidth = 0;
    if (FHieght == 15) {
        static const size_t FWidth = 9;
    }
    else if (FHieght == 13){
        static const size_t FWidth = 8;
    }
    size_t BitmapWidth = FWidth * Text.size();

    Start.x = Start.x - (static_cast<double>(BitmapWidth)) / WinHeight;
    Start.y = Start.y - (static_cast<double>(FHieght) / 2) / WinWidth;
    Start.pos();

    for (const char& i : Text) {
        if (FHieght == 15) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, i);
        }
        else if (FHieght == 13) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, i);
        }
    }
}