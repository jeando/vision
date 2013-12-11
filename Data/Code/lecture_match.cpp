#include "lecture_match.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

lecture_match::lecture_match(string file)
{
	cout << "test" << endl;

	fstream fichier(file.c_str());
	int nb_img;
	vector<Image> img;

	if(fichier)
	{
		string ligne;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            if(ligne[0]!='#')
            {
                if(ligne[0]==' ')//donnees d'appariement
                {

                }else{
                    if(ligne.length()==1)
                    {
                        nb_img=ligne[0];
                    }else{
                        int prec = 1;
                        int cpt=0;
                        int num=ligne[0]-'0';
                        string id; int w; int h;
                        for(int i=1;i<ligne.length();i++)
                        {
                            if(ligne[i]==' '|| ligne[i]=='x' || ligne[i]=='\t' || ligne[i]=='\n')
                            {
                                switch(cpt)
                                {
                                    case 0:
                                        id=ligne.substr(i+1,prec);
                                        break;
                                    case 1:
                                        stringstream(ligne.substr(i+1,prec)) >> w;
                                        break;
                                    case 2:
                                        stringstream(ligne.substr(i+1)) >> h;
                                        break;
                                }
                                cpt++;
                                prec=i;
                            }
                        }
                        Image new_img(num,id, w,h);
                        new_img.print();
                        img.push_back(new_img);
                    }
                }//donnees taille image
            }//else commentaire
        }
	}else{
		cout << "impossible d'ouvrir le fichier" << endl;
	}

	cout << "fin test" << endl;
};

void lecture_match::test()
{
	cout << "!!!!!!!!!" << endl;
}

