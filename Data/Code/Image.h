#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include<vector>
#include<map>
#include"correspondance.h"
class Image
{
    public:
        Image(int _id, std::string nom_fich, int w, int h);
        void print();
        virtual ~Image();
		std::vector<correspondance<double>>& operator[](int i);

    protected:
    private:
        int id;
        std::string name;
		std::map<int,std::vector<correspondance<double>>> assoc;
        int width;
        int height;
};

#endif // IMAGE_H

