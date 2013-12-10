#include "lecture_match.h"
#include <iostream>
#include <fstream>

using namespace std;

lecture_match::lecture_match(string file)
{
	cout << "test" << endl;

	fstream fichier(file);

	if(fichier)
	{
		cout << "traitement du fichier" << endl;
	}else{
		cout << "impossible d'ouvrir le fichier" << endl;
	}

	cout << "fin test" << endl;
};

void lecture_match::test()
{
	cout << "!!!!!!!!!" << endl;
}


