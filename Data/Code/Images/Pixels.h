#ifndef PIXELS_H
#define PIXELS_H

namespace Pixels {

    typedef unsigned char Grey;

    template <typename TYPE=unsigned char>
    struct RGB {
        TYPE values[3];
    };

    template <typename T> struct TAG{ };

    template <> struct TAG<Grey> {
        static const unsigned depth = 255;
        static const char value[];
    };
    const char TAG<Grey>::value[] = "P5";

    template <> struct TAG<RGB<> > {
        static const unsigned depth = 255;
        static const char value[];
    };
    const char TAG<RGB<> >::value[] = "P6";
}

#endif  //  !PIXELS_H
