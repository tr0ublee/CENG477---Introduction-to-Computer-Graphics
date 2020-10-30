#include "Color.hpp"
Color::Color(){
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(int red, int green, int blue) {
    r = red;
    g = green;
    b = blue;
}

Color::Color(Color &param) {
    r = param.r;
    g = param.g;
    b = param.b;
}
Color::~Color() {

}