#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include <map>
#include "correspondance.h"

class Image
{
    public:
        Image(int _id, std::string nom_fich, int w, int h);
        Image(std::string line);
        void print()const;
        virtual ~Image();
		std::vector<correspondance<double>>& operator[](int i);
		int get_id(){return id;}
		void add_correspondance(const point_i& p1, const point_i& p2);
		friend std::ostream& operator<< (std::ostream& os, const Image& i);

    protected:

    private:
        int id;
        std::string name;
		std::map<int,std::vector<correspondance<double>>> assoc;
        int width;
        int height;
};

#endif // IMAGE_H

