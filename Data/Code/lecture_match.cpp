#include "lecture_match.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include<memory>


using namespace std;

lecture_match::lecture_match(string file)
{
	cout << "test" << endl;

	fstream fichier(file.c_str());
	int nb_img;
	std::map<int,std::unique_ptr<Image>> img;
//	std::unique_ptr<Image> ptr_img;
	if(fichier)
	{
		string ligne;
		int x1,y1,x2,y2;
		string id1,id2;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            if(ligne[0]!='#')
            {
                if(ligne[0]==' ')//donnees d'appariement
                {
                    stringstream line(ligne);
                    line>>id1>>x1>>y1>>id2>>x2>>y2;
                }else{
                    if(ligne.length()==1)
                    {
                        nb_img=ligne[0];
                    }else{
                        int prec = 2;
                        int cpt=0;
                        int fich=ligne[0]-'0';
                        string id; int w; int h;
                        for(int i=3;i<ligne.length();i++)
                        {
                            if(ligne[i]==' '|| ligne[i]=='x' || ligne[i]=='\t' || ligne[i]=='\n')
                            {
                                switch(cpt)
                                {
                                    case 0:
                                        id=ligne.substr(prec,i-prec);
                                        break;
                                    case 1:
                                        stringstream(ligne.substr(prec,i-prec)) >> w;
                                        break;
                                }
                                cpt++;
                                prec=i;
                            }
                        }
                        stringstream(ligne.substr(prec+1)) >> h;
                        Image new_img(fich,id, w,h);
                        new_img.print();
                        img[fich]= std::unique_ptr<Image>(new Image(fich,id,w,h));//new_img;
                    }
                }//donnees taille image
            }//else commentaire
        }
	}else{
		cout << "impossible d'ouvrir le fichier" << endl;
	}
};

void lecture_match::test()
{
	cout << "!!!!!!!!!" << endl;
}


