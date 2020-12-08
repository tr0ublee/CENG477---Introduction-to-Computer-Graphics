#ifndef COLOR_HPP
#define COLOR_HPP
class Color {

    public:
        unsigned char r;
        unsigned char g;
        unsigned char b;

        Color();
        Color(unsigned char red, unsigned char green, unsigned char blue);
        Color(const Color &param);
        void normalizeColor();
        ~Color();
};

#endif