#include <vector>
#include <iostream>

#include "ruicf.h"

std::vector<Button*> buildButtons(int x = 10, int y = 10) {
    std::vector<Button*> r;
    int w = 100;
    int h = 30;
    const char* text = "don't";

    r.emplace_back(new Button(x, y, w, h, text));
    y += h + 16;
    r.emplace_back(new Button(x, y, w, h, text, YELLOW, ORANGE, BLACK));
    y += h + 16;
    r.emplace_back(new Button(x, y, w, h, text, CBFUNC {std::cout << "only look" << std::endl;}));
    y += h + 16;
    r.emplace_back(new OutlineButton(x, y, w, h, text));
    y += h + 16;
    r.emplace_back(new OutlineButton(x, y, w, h, text, YELLOW, ORANGE, YELLOW));
    y += h + 16;
    r.emplace_back(new OutlineButton(x, y, w, h, text, 4));
    y += h + 16;
    r.emplace_back(new OutlineButton(x, y, w, h, text, YELLOW, ORANGE, YELLOW, 4));
    y += h + 16;
    r.emplace_back(new ImageButton(x, y, UIItem::getTexture("/home/nbdy/CLionProjects/ruicf/icons/gpsFixed.png", 32 ,32 )));
    x += 48;
    r.emplace_back(new ImageButton(x, y, UIItem::getTexture("/home/nbdy/CLionProjects/ruicf/icons/gpsOff.png", 32, 32), GREEN, DARKGREEN, BLACK));
    y += 48;
    x -= 48;
    r.emplace_back(new ImageButton(x, y, UIItem::getTexture("/home/nbdy/CLionProjects/ruicf/icons/gpsFixed.png", 32, 32), CBFUNC {std::cout << "hey" << std::endl;}));
    x += 48;
    r.emplace_back(new ImageButton(x, y, UIItem::getTexture("/home/nbdy/CLionProjects/ruicf/icons/gpsOff.png", 32, 32), GREEN, DARKGREEN, BLACK, CBFUNC {std::cout << "stop" << std::endl;}));

    return r;
}

std::vector<CheckBox*> buildCheckboxes(int x = 140, int y = 10) {
    std::vector<CheckBox*> r;
    int w = 100;
    int h = 30;
    const char* text = "don't.";

    r.emplace_back(new CheckBox(x, y, w, h, text));
    y += h + 16;
    r.emplace_back(new CheckBox(x, y, w, h, text, YELLOW));
    y += h + 16;
    r.emplace_back(new CheckBox(x, y, w, h, text, CBFUNC {std::cout << "don't touch" << std::endl;}));
    y += h + 16;
    r.emplace_back(new CheckBox(x, y, w, h, text, YELLOW, CBFUNC {std::cout << "maybe a little" << std::endl;}));

    return r;
}

std::vector<HSlider*> buildHSliders(int x = 140, int y = 200) {
    std::vector<HSlider*> r;
    r.emplace_back(new HSlider(x, y, 200, 20));
    y += 36;
    r.emplace_back(new HSlider(x, y, 200, 20, 16));
    return r;
}

std::vector<VSlider*> buildVSliders(int x = 400, int y = 200) {
    std::vector<VSlider*> r;
    r.emplace_back(new VSlider(x, y, 20, 200));
    x += 36;
    r.emplace_back(new VSlider(x, y, 20, 200, 16));
    return r;
}

template<typename T> void draw(std::vector<T> lst) {
    for(auto *i : lst) i->draw();
}

template<typename T> void clean(std::vector<T> lst) {
    for(auto *i : lst) delete i;
}

int main() {
    InitWindow(500, 500, "demo");
    SetTargetFPS(420);

    auto btns = buildButtons();
    auto chkb = buildCheckboxes();
    auto hsdr = buildHSliders();
    auto vsdr = buildVSliders();

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        draw(btns);
        draw(chkb);
        draw(hsdr);
        draw(vsdr);

        EndDrawing();
    }

    CloseWindow();

    clean(btns);
    clean(chkb);
    clean(hsdr);
    clean(vsdr);

    return 0;
}
