#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>


class Image
{
    public:
        Image(int num, std::string id, int w, int h);
        void print();
        virtual ~Image();
    protected:
    private:
        int numero;
        std::string identity;
        int width;
        int height;
};

#endif // IMAGE_H
