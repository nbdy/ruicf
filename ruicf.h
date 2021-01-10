//
// Created by nbdy on 06.01.21.
//

#ifndef RUICF_RUICF_H
#define RUICF_RUICF_H

#include <ctime>
#include <string>
#include <utility>
#include <vector>
#include <utility>

#include <raylib.h>

#define CBFUNC []()
typedef void (*cbFunc)();
typedef std::vector<std::string> strVec;

class UIItem {
protected:
    cbFunc onClickFunction = CBFUNC {};
    cbFunc onLongClickFunction = CBFUNC {};

    float x = 0;
    float y = 0;
    float width = 1;
    float height = 1;

    unsigned long longClickThreshold = 420;

    time_t firstClicked {};
    time_t clicked {};

public:
    UIItem() = default;

    UIItem(float x, float y, float width, float height): x(x), y(y), width(width), height(height) {};

    UIItem(float x, float y, float width, float height, cbFunc onClickFunction): x(x), y(y), width(width), height(height), onClickFunction(onClickFunction) {};

    UIItem(float x, float y, float width, float height, cbFunc onClickFunction, cbFunc onLongClickFunction):
    x(x), y(y), width(width), height(height), onClickFunction(onClickFunction), onLongClickFunction(onLongClickFunction) {};

    UIItem(float x, float y, float width, float height, unsigned long longClickThreshold): x(x), y(y), width(width), height(height), longClickThreshold(longClickThreshold) {};

    ~UIItem() = default;

    static unsigned long getTimestampLong() {
        struct timespec t {};
        timespec_get(&t, TIME_UTC);
        return 1000000000 * t.tv_sec + t.tv_nsec;
    }

    static Texture2D getTexture(const std::string &path, int w, int h) {
        if(FileExists(path.c_str())) {
            auto icon = LoadImage(path.c_str());
            ImageResize(&icon, w, h);
            auto r = LoadTextureFromImage(icon);
            UnloadImage(icon);
            return r;
        }
        return Texture2D {};
    }
    
    virtual void draw() {
        auto wc = wasClicked();
        if(wc) {
            if(firstClicked == 0) firstClicked = getTimestampLong();
            else {
                clicked = getTimestampLong();
                if(wasLongClicked()) onLongClicked();
            }
        } else {
            if(firstClicked != 0) onClicked();
            firstClicked = 0;
            clicked = 0;
        }
    };

    virtual bool wasClicked() {
        auto mp = GetMousePosition();
        return (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
                (mp.x > x && mp.x < x + width && mp.y > y && mp.y < y + height));
    };

    [[nodiscard]] virtual bool wasLongClicked() const {
        auto r = clicked - firstClicked;
        return r >= longClickThreshold;
    };

    [[nodiscard]] bool isClicked() const {
        return clicked > 0;
    }

    [[nodiscard]] float getHeight() const {
        return height;
    };

    void setHeight(float v) {
        height = v;
    };

    [[nodiscard]] float getWidth() const {
        return width;
    };

    void setWidth(float v) {
        width = v;
    };

    [[nodiscard]] float getX() const {
        return x;
    };

    void setX(float v) {
        x = v;
    };

    [[nodiscard]] float getY() const {
        return y;
    };

    void setY(float v) {
        y = v;
    };

    [[nodiscard]] Vector2 getTopLeft() const {
        return Vector2 {x, y};
    };

    [[nodiscard]] Vector2 getBottomRight() const {
        return Vector2 {x + width, y + height};
    };

    [[nodiscard]] Vector2 getCenter() const {
        return Vector2 {x + width / 2, y + height / 2};
    }

    [[nodiscard]] Rectangle getBounds() const {
        return Rectangle {x, y, width, height};
    };

    virtual void onClicked() {
        onClickFunction();
    };

    virtual void onLongClicked() {
        onLongClickFunction();
    };
};


class Button : public UIItem {
protected:
    int segments = 4;
    float roundness = 0.2;
    int fontSize = 16;
    Color buttonColor = GREEN;
    Color buttonClickedColor = DARKGREEN;
    Color textColor = BLACK;
    std::string text = "Button";

public:
    Button(float x, float y, float w, float h, std::string text): UIItem(x, y, w, h), text(std::move(text)) {};

    Button(float x, float y, float w, float h, std::string text, cbFunc onClickFunction): UIItem(x, y, w, h, onClickFunction), text(std::move(text)) {};

    Button(float x, float y, float w, float h, std::string text, Color buttonColor, Color buttonClickedColor, Color textColor):
    UIItem(x, y, w, h), text(std::move(text)), buttonColor(buttonColor), buttonClickedColor(buttonClickedColor), textColor(textColor) {};

    Button(float x, float y, float w, float h, std::string text, Color buttonColor, Color buttonClickedColor, Color textColor, cbFunc onClickFunction):
    UIItem(x, y, w, h, onClickFunction), text(std::move(text)), buttonColor(buttonColor), buttonClickedColor(buttonClickedColor), textColor(textColor) {};

    void draw() override {
        UIItem::draw();

        auto btnC = (clicked != 0) ? buttonClickedColor : buttonColor;

        drawBackground(btnC);
        drawForeground(textColor);
    }

    virtual void drawBackground(Color color) {
        DrawRectangleRounded(Rectangle {x, y, width, height}, roundness, segments, color);
    }

    virtual void drawForeground(Color color) {
        DrawText(text.c_str(),x + width / 4, y + height / 4, fontSize, color);
    }
};

class OutlineButton : public Button {
protected:
    int lineThickness = 1;

public:
    OutlineButton(float x, float y, float w, float h, std::string text): Button(x, y, w, h, std::move(text), GREEN, BLACK, GREEN) {};

    OutlineButton(float x, float y, float w, float h, std::string text, int lineThickness): Button(x, y, w, h, std::move(text), GREEN, BLACK, GREEN), lineThickness(lineThickness) {};

    OutlineButton(float x, float y, float w, float h, std::string text, Color buttonColor, Color buttonClickedColor, Color textColor):
    Button(x, y, w, h, std::move(text), buttonColor, buttonClickedColor, textColor) {};

    OutlineButton(float x, float y, float w, float h, std::string text, Color buttonColor, Color buttonClickedColor, Color textColor, int lineThickness):
    Button(x, y, w, h, std::move(text), buttonColor, buttonClickedColor, textColor), lineThickness(lineThickness) {};

    void drawBackground(Color color) override {
        DrawRectangleRoundedLines(Rectangle {x, y, width, height}, roundness, segments, lineThickness, color);
    }
};

class ImageButton : public Button {
protected:
    Texture2D texture;
public:
    ImageButton(float x, float y, Texture2D texture): Button(x, y, texture.width, texture.height, ""), texture(texture) {}

    ImageButton(float x, float y, Texture2D texture, cbFunc onClickFunction): Button(x, y, texture.width, texture.height, "", onClickFunction), texture(texture) {}

    ImageButton(float x, float y, float w, float h, Texture2D texture): Button(x, y, w, h, ""), texture(texture) {}

    ImageButton(float x, float y, float w, float h, Texture2D texture, cbFunc onClickFunction): Button(x, y, w, h, "", onClickFunction), texture(texture) {}

    ImageButton(float x, float y, Texture2D texture, Color buttonColor, Color buttonClickedColor, Color textureColor):
    Button(x, y, texture.width, texture.height, "", buttonColor, buttonClickedColor, textureColor), texture(texture) {};

    ImageButton(float x, float y, Texture2D texture, Color buttonColor, Color buttonClickedColor, Color textureColor, cbFunc onClickFunction):
    Button(x, y, texture.width, texture.height, "", buttonColor, buttonClickedColor, textureColor, onClickFunction), texture(texture) {};

    void drawForeground(Color color) override {
        DrawTexture(texture, x + width / 2 - texture.width / 2, y + height / 2 - texture.height / 2, color);
    }

    void drawBackground(Color color) override {
        Button::drawBackground(color);
    }
};

class OutlineImageButton : public ImageButton {
protected:
    int lineThickness = 1;

public:
    OutlineImageButton(float x, float y, Texture2D texture): ImageButton(x, y, texture, GREEN, DARKGREEN, GREEN) {};

    OutlineImageButton(float x, float y, Texture2D texture, cbFunc onClickFunction): ImageButton(x, y, texture, GREEN, DARKGREEN, GREEN, onClickFunction) {};

    OutlineImageButton(float x, float y, Texture2D texture, Color buttonColor, Color buttonClickedColor, Color textureColor):
    ImageButton(x, y, texture, buttonColor, buttonClickedColor, textureColor) {};

    OutlineImageButton(float x, float y, Texture2D texture, Color buttonColor, Color buttonClickedColor, Color textureColor, cbFunc onClickFunction):
    ImageButton(x, y, texture, buttonColor, buttonClickedColor, textureColor, onClickFunction) {};

    void drawBackground(Color color) override {
        DrawRectangleRoundedLines(Rectangle {x, y, width, height}, roundness, segments, lineThickness, color);
    }
};

class DropdownButton : public Button { // todo
protected:
    strVec items;

public:
    DropdownButton(float x, float y, float w, float h, std::string text, strVec items):
    Button(x, y, w, h, std::move(text)), items(std::move(items)) {};
};


class CheckBox : public UIItem {
protected:
    int fontSize = 15;
    bool checked = false;
    int lineThickness = 1;
    float roundness = 0.2;
    int segments = 4;

    Color color = GREEN;
    std::string text = "CheckBox";

public:
    CheckBox(float x, float y, float w, float h, std::string text): UIItem(x, y, w, h), text(std::move(text)) {};

    CheckBox(float x, float y, float w, float h, std::string text, cbFunc onClickFunction): UIItem(x, y, w, h, onClickFunction), text(std::move(text)) {};

    CheckBox(float x, float y, float w, float h, std::string text, Color color): UIItem(x, y, w, h), text(std::move(text)), color(color) {};

    CheckBox(float x, float y, float w, float h, std::string text, Color color, cbFunc onClickFunction): UIItem(x, y, w, h, onClickFunction), text(std::move(text)), color(color) {};

    void draw() override {
        UIItem::draw();
        DrawRectangleRoundedLines(Rectangle {x, y, height, height}, roundness, segments, lineThickness, color);
        DrawText(text.c_str(), x + height + 8, y + height / 4, fontSize, color);
        if(checked) drawCheckMark();
    }

    virtual void drawCheckMark() {
        DrawLine(x, y, x + height, y + height, color);
        DrawLine(x, y + height, x + height, y, color);
    }

    void onClicked() override {
        UIItem::onClicked();
        checked = !checked;
    }

    [[nodiscard]] bool isChecked() const {
        return checked;
    }
};


class SliderKnob : public UIItem {
protected:
    Color color = GREEN;
    Color clickedColor = DARKGREEN;

public:
    SliderKnob(float x, float y, float r): UIItem(x, y, r * 2, r * 2) {};
    SliderKnob(float x, float y, float w, float h): UIItem(x, y, w, h) {};

    void draw() override {
        UIItem::draw();
        DrawCircle(x, y, width / 2, isClicked() ? clickedColor : color);
    }
};

class HSliderKnob : public SliderKnob {
public:
    HSliderKnob(float x, float y, float r): SliderKnob(x, y, r) {};
    HSliderKnob(float x, float y, float w, float h): SliderKnob(x, y, w, h) {};

    void onLongClicked() override {
        x = (float) GetMouseX();
    }
};

class VSliderKnob : public SliderKnob {
public:
    VSliderKnob(float x, float y, float r): SliderKnob(x, y, r) {};
    VSliderKnob(float x, float y, float w, float h): SliderKnob(x, y, w, h) {};

    void onLongClicked() override {
        y = (float) GetMouseY() - height / 2;
    }
};


// todo draw value on top or bottom
// todo use min
template<typename KNOB>
class Slider : public UIItem {
protected:
    float roundness = 0.4;
    int lineThickness = 2;
    int segments = 4;
    Color color = GREEN;

    float min = 0;
    float max = 100;

    KNOB knob;

public:
    Slider(float x, float y, float w, float h, KNOB knob): UIItem(x, y, w, h), knob(std::move(knob)) {};

    void draw() override {
        UIItem::draw();
        drawRail();
        drawKnob();
    }

    virtual void drawKnob() {
        knob.draw();
    };

    virtual void drawRail(){};

    void onClicked() override {}

    [[nodiscard]] virtual float getValue() const {
        return -1;
    }
};

class HSlider : public Slider<HSliderKnob> {
public:
    HSlider(float x, float y, float w, float h): Slider<HSliderKnob>(x, y, w, h, HSliderKnob(x, y + h / 2, 8)) {};
    HSlider(float x, float y, float w, float h, float knobRadius): Slider<HSliderKnob>(x, y, w, h, HSliderKnob(x, y + h / 2, knobRadius)) {};

    void drawRail() override {
        DrawRectangleRoundedLines(Rectangle {x, y + height / 2 - height / 8, width, height / 8}, roundness, segments, lineThickness, color);
    }

    void drawKnob() override {
        auto kc = knob.getCenter();
        if(kc.x > x + width) knob.setX(x + width);
        if(kc.x < x) knob.setX(x);
        knob.draw();
    }

    void onClicked() override {
        auto xc = GetMouseX();
        auto kx = knob.getX();
        if(xc < kx) knob.setX(kx - 10);
        else if(xc > kx) knob.setX(kx + 10);
    }

    [[nodiscard]] float getValue() const override {
        auto s = width / max;
        auto v = knob.getCenter().x - x - 8; // todo remove 8
        return v / s;
    }
};

class VSlider : public Slider<VSliderKnob> {
public:
    VSlider(float x, float y, float w, float h): Slider<VSliderKnob>(x, y, w, h, VSliderKnob(x + w / 2, y, 8)) {};
    VSlider(float x, float y, float w, float h, float knobRadius): Slider<VSliderKnob>(x, y, w, h, VSliderKnob(x + w / 2, y, knobRadius)) {};

    void drawRail() override {
        DrawRectangleRoundedLines(Rectangle {x + width / 2, y, width / 8, height}, roundness, segments, lineThickness, color);
    }

    void drawKnob() override {
        auto kc = knob.getCenter();
        if(kc.y > y + height) knob.setY(y + height);
        if(kc.y < y) knob.setY(y);
        knob.draw();
    }

    void onClicked() override {
        auto yc = GetMouseY();
        auto ky = knob.getY();
        if(yc < ky) knob.setY(ky - 10);
        else if(yc > ky) knob.setY(ky + 10);
    }
};

#endif //RUICF_RUICF_H
