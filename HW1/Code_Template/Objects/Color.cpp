#include "Color.hpp"
Color::Color(){
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue) {
    r = red;
    g = green;
    b = blue;
    // this -> normalizeColor();
}

Color::Color(const Color &param) {
    r = param.r;
    g = param.g;
    b = param.b;
    // this -> normalizeColor();
}
void Color::normalizeColor(){

    if (r > 255.0f) {
        r = (unsigned char) 255;
    }
    if (g > 255.0f) {
        g = (unsigned char) 255;
    }
    if (b > 255.0f) {
        b = (unsigned char) 255;
    }
}

Color::~Color() {

}