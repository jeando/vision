#include <iostream>
#include "mosaic.h"

#include<map>
#include<memory>
#include<vector>
typedef double TYPE;

int main(int argc, char** argv)
{
	
	mosaic<TYPE> m;
	m.lecture_appariement("test.matchs");
	//m.compute_mosaic(Draw);
	m.init_compute_mosaic();
	
	m.compute_next_mosaic();
	std::cout << m.size()<<std::endl;
}


