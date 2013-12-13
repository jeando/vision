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
	std::unique_ptr<Image> ptr_image;
	int img_0;
	int	img_1;
	if(fichier)
	{
		string ligne;
		correspondance<double> corr;
		string id1,id2;

		std::getline(fichier, ligne);
		while(ligne[0]=='#')
			std::getline(fichier, ligne);
		stringstream(ligne)>>nb_img;
		std::cout << nb_img << std::endl;
		for(int i=0; i<nb_img; i++)
		{
			std::getline(fichier, ligne);
			if(ligne[0]=='#' || ligne.size()==0)// || ligne.size() == std::count(&ligne[0])
			{
				i--;
				continue;
			}
			ptr_image = std::unique_ptr<Image>(new Image(ligne));
			std::cout << ptr_image->get_id() << std::endl;
			img[ptr_image->get_id()] = std::move(ptr_image);
		}
		for(const auto& a : img)
		{
			//a.second->print();
			cout << a.second;
		}

		std::vector<point_i> vtmp;
		point_i ptmp;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu
		{
			if(ligne[0]=='#' || ligne.size()==0)// || ligne.size() == std::count(&ligne[0])
			{
				continue;
			}
			vtmp.clear();
			std::stringstream ss(ligne);

			while(ss.ignore())
			{
				ss>>ptmp.id>>ptmp.x>>ptmp.y;
				vtmp.push_back(ptmp);
			}
//			std::cout << vtmp.size()<<std::endl;
			const int size = vtmp.size();
			for(int i=0; i<size; i++)
			{
				int id = vtmp[i].id;
				for(int j=0; j<i; j++)
				{
					img[id]->add_correspondance(vtmp[i],vtmp[j]);
				}
				for(int j=i+1; j<size; j++)
				{
					img[id]->add_correspondance(vtmp[i],vtmp[j]);
				}
			}
		}
/*		for(int i : {0,1,2,3,4})
		{
			img[i]->print();
		}
		//*/

/*

        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            if(ligne[0]!='#')
            {
                if(ligne[0]==' ')//donnees d'appariement
                {
                    stringstream line(ligne);
                    line>>id1>>corr.x1>>corr.y1>>id2>>corr.x2>>corr.y2;

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
		//*/
	}else{
		cout << "impossible d'ouvrir le fichier" << endl;
	}
};

void lecture_match::test()
{
	cout << "!!!!!!!!!" << endl;
}


