//  Compile with: g++ -O4 -I. -o Test Test.C
//  Run with: ./Test ../Mosaics/Eglise/i.25 toto.ppm ../Stereo/herveleft.pgm toto.pgm

#include <fstream>
#include <string>
#include <Image.H>

int main(const int argc,const char* argv[]) try {
    using namespace Images;
    Image<RGB<> > im(argv[1]);
    std::ofstream ofs(argv[2]);
    ofs << im;
    Image<Grey> im1(argv[3]);
    std::ofstream ofs1(argv[4]);
    ofs1 << im1;

} catch (const std::string& message) {
    std::cerr << message << std::endl;
}
