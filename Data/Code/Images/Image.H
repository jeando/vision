#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <Pixels.H>

namespace Images {

    using namespace Pixels;

    template <typename TYPE>
    class Image {

        void destroy() {
            if (data)
                delete[] data;
            data = 0;
        }

    public:

        Image(): data(0) { dims[0] = dims[1] = 0; }

        Image(const char* file) throw(const std::string): data(0) {
            std::ifstream ifs(file);
            try {
                ifs >> *this;
            } catch(const std::string& message) {
                throw std::string(file)+":"+message;
            }
        }

        Image(const unsigned sx,const unsigned sy): data(0) { resize(sx,sy); }

        Image(const Image& im): data(0) {
            resize(im.dimx(),im.dimy());
            std::copy(&im(0,0),&im(im.dimx()-1,im.dimy()-1)+1,data);
        }

        ~Image() { destroy(); }

        //  Retaillage (attention les données sont perdues).

        void resize(const unsigned sx,const unsigned sy) {
            destroy();
            data = new TYPE[sx*sy];
            dims[0] = sx;
            dims[1] = sy;
        }

        //  Acces au pixel de coordonnees (i,j).

        const TYPE& operator()(const unsigned i,const unsigned j) const { return data[i*dimx()+j]; }
              TYPE& operator()(const unsigned i,const unsigned j)       { return data[i*dimx()+j]; }

        //  Obtention des dimensions de l'image.

        unsigned dim(const unsigned i) const { return dims[i-1]; }

        unsigned dimx() const { return dim(1); }
        unsigned dimy() const { return dim(2); }

    private:

        unsigned dims[2];
        TYPE*    data;
    };

    template <typename TYPE>
    std::istream& operator>>(std::istream& is,Image<TYPE>& im) throw(const std::string) {

        //  Read the tag identifying the image.

        std::string str;
        is >> str;
        if (str!=Pixels::TAG<TYPE>::value)
            throw std::string("Bad magic tag for this image type.");

        //  Ignoring comments.

        is >> std::ws;
        while (is.peek()=='#')
            is.ignore(std::numeric_limits<int>::max(),'\n');

        //  Reading the image size.

        unsigned sx,sy,depth;
        is >> sx >> sy >> depth >> std::ws;
        
        if (depth!=Pixels::TAG<TYPE>::depth)
            throw std::string("Bad depth size for this image type.");
            
        //  Resizing the image and reading the data.

        im.resize(sx,sy);
        const unsigned size = im.dimx()*im.dimy()*sizeof(TYPE);
        is.read(reinterpret_cast<char*>(&im(0,0)),size);

        return is;
    }

    template <typename TYPE>
    std::ostream& operator<<(std::ostream& os,const Image<TYPE>& im) throw(const std::string) {
        //  Write the identifying tag and the image sizes.

        os << Pixels::TAG<TYPE>::value << std::endl;
        os << im.dimx() << ' ' << im.dimy() << std::endl
           << Pixels::TAG<TYPE>::depth << std::endl;
        const unsigned size = im.dimx()*im.dimy()*sizeof(TYPE);
        os.write(reinterpret_cast<const char*>(&im(0,0)),size);

        return os;
    }
}

#endif  //  ! IMAGE_H
