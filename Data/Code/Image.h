#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>


class Image
{
    public:
        Image(int id, std::string nom_fich, int w, int h);
        void print();
        virtual ~Image();
    protected:
    private:
        int identity;
        std::string nom_fichier;
        int width;
        int height;
};

#endif // IMAGE_H
