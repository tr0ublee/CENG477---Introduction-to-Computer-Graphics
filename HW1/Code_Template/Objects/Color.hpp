#pragma once
class Color {

    public:
        int r;
        int g;
        int b;

        Color();
        Color(int red, int green, int blue);
        Color(Color &param);
        inline void normalizeColor(){
            if (r > 255) {
                r = 255;
            }
            if (g > 255) {
                g = 255;
            }
            if (b > 255) {
                b = 255;
            }
        }
        ~Color();
};