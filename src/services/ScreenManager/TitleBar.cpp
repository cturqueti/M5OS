#include "TitleBar.h"

TitleBar::TitleBar(M5GFX* display)
    : display(display),
      canvas(display),
      text(""),
      x0(0),
      y0(0),
      width(0),
      height(0),
      borderColor(TFT_WHITE) {
    canvas.fillSprite(TFT_BLACK);
    canvas.setTextColor(TFT_WHITE);
}

TitleBar::~TitleBar() {
    canvas.deleteSprite();
}

void TitleBar::begin(int x0, int y0, int x1, int y1) {
    this->x0 = x0;
    this->y0 = y0;
    this->width = x1 - x0;
    this->height = y1 - y0;
    canvas.createSprite(width, height);
}

void TitleBar::configColors(int bgColor, int textColor, int borderColor) {
    this->borderColor = borderColor;
    canvas.fillSprite(bgColor);
    canvas.setTextColor(textColor);
}

void TitleBar::draw() {
    // Define border parameters
    int radius = 10;  // Radius for the rounded corners

    // Draw the top horizontal line with rounded corners
    canvas.drawLine(radius, 0, width - radius - 1, 0, borderColor);

    // Draw the vertical sides
    canvas.drawLine(0, radius, 0, height - 1, borderColor);                  // Left side
    canvas.drawLine(width - 1, radius, width - 1, height - 1, borderColor);  // Right side

    // Draw the bottom horizontal line
    // canvas.drawLine(0, height - 1, width - 1, height - 1, borderColor);

    // Draw the top-left rounded corner
    canvas.drawCircleHelper(radius, radius, radius, 1, borderColor);

    // Draw the top-right rounded corner
    canvas.drawCircleHelper(width - radius - 1, radius, radius, 2, borderColor);

    // Draw the text
    canvas.setTextSize(1);
    canvas.setCursor(5, (height - 1) / 2);
    canvas.print(text);
}
