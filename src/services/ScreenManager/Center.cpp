#include "Center.h"

#include "esp_log.h"

const char* Center::TAG = "Center";

Center::Center(M5GFX* display)
    : display(display),
      canvas(display),
      x0(0),
      y0(0),
      width(0),
      height(0),
      maxLines(10),
      lineHeight(1),
      borderColor(TFT_WHITE),
      textParagraph(5) {
    canvas.fillSprite(TFT_BLACK);
    canvas.setTextColor(TFT_WHITE);
}

Center::~Center() {
    canvas.deleteSprite();
}

void Center::begin(int x0, int y0, int x1, int y1) {
    this->x0 = x0;
    this->y0 = y0;
    this->width = x1 - x0;
    this->height = y1 - y0;
    canvas.createSprite(width, height);
}

void Center::configColors(int bgColor, int textColor, int borderColor) {
    this->borderColor = borderColor;
    canvas.fillSprite(bgColor);
    canvas.setTextColor(textColor);
}

void Center::setText(const std::vector<std::pair<std::string, std::string>>& textVector) {
    this->textVector = textVector;
}

void Center::draw() {
    // Draw the border with rounded corners at the bottom
    int radius = 10;  // Radius for rounded corners

    // Draw the top and side parts of the border
    canvas.drawLine(0, 0, width - 1, 0, borderColor);                            // Top line
    canvas.drawLine(0, 0, 0, height - radius - 1, borderColor);                  // Left vertical line
    canvas.drawLine(width - 1, 0, width - 1, height - radius - 1, borderColor);  // Right vertical line

    // Draw the bottom horizontal line excluding corners
    canvas.drawLine(radius, height - 1, width - radius - 1, height - 1, borderColor);

    // Draw the bottom-left rounded corner
    canvas.drawCircleHelper(radius, height - radius - 1, radius, 8, borderColor);

    // Draw the bottom-right rounded corner
    canvas.drawCircleHelper(width - radius - 1, height - radius - 1, radius, 4, borderColor);

    // Draw the text
    canvas.setTextSize(1);
    lineHeight = canvas.fontHeight() + 1;
    maxLines = height / lineHeight;

    int y = height - lineHeight - 2;  // Y position for text
    int numItems = maxLines;

    if (textVector.size() <= numItems) {
        ESP_LOGI(TAG, "Número de itens no vetor: %d", static_cast<int>(textVector.size()));
        numItems = textVector.size();  // Ajusta o número de itens a serem exibidos
    }

    int endIdx = textVector.size() - numItems;

    for (int i = textVector.size() - 1; i >= endIdx; --i) {
        const std::string& key = textVector[i].first;
        const std::string& line = textVector[i].second;  // Obtém o texto da linha
        std::string output = key + ">" + line;

        // Quebra o texto em linhas com base na largura do canvas
        std::vector<std::string> wrappedLines = wrapText(output, width - textParagraph);

        // Desenha cada linha no canvas
        for (const std::string& wrappedLine : wrappedLines) {
            canvas.setCursor(textParagraph, y);
            canvas.print(wrappedLine.c_str());
            ESP_LOGI(TAG, "Texto impresso: %s", wrappedLine.c_str());
            y -= lineHeight;  // Move para cima na tela

            if (y < 0) {
                break;  // Sai do loop se não houver mais espaço na tela
            }
        }
    }
}

int Center::getTextWidth(const std::string& text) {
    return text.length() * canvas.fontWidth();  // Exemplo: 6 pixels por caractere
}

std::vector<std::string> Center::wrapText(const std::string& text, int maxWidth) {
    std::vector<std::string> lines;
    std::string currentLine;

    for (char c : text) {
        std::string testLine = currentLine + c;
        int lineWidth = getTextWidth(testLine);

        if (lineWidth > maxWidth) {
            if (!currentLine.empty()) {
                lines.insert(lines.begin(), currentLine);
            }
            currentLine = std::string(1, c);
        } else {
            currentLine += c;
        }
    }

    if (!currentLine.empty()) {
        lines.insert(lines.begin(), currentLine);
    }

    return lines;
}