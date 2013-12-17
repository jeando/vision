#include "mosaic.h"
#include <iostream>
#include <fstream>
#include <sstream>


template<typename T>
mosaic<T>::mosaic()
{
}
template<typename T>
void mosaic<T>::lecture_appariement(std::string file)
{

	int nb_img;
	std::unique_ptr<Image<T>> ptr_image;
	
	std::fstream fichier(file.c_str());
	if(fichier)
	{
		std::string ligne;
		std::string id1,id2;

		std::getline(fichier, ligne);
		while(ligne[0]=='#')
			std::getline(fichier, ligne);
		std::stringstream(ligne)>>nb_img;
		std::cout << nb_img << std::endl;
		for(int i=0; i<nb_img; i++)
		{
			std::getline(fichier, ligne);
			if(ligne[0]=='#' || ligne.size()==0)// || ligne.size() == std::count(&ligne[0])
			{
				i--;
				continue;
			}
			ptr_image = std::unique_ptr<Image<T>>(new Image<T>(ligne));
			std::cout << ptr_image->get_id() << std::endl;
			img[ptr_image->get_id()] = std::move(ptr_image);
		}
		for(const auto& a : img)
		{
			std::cout << *a.second << std::endl;
		}

		std::vector<point_i<T>> vtmp;
		point_i<T> ptmp;
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
				ss >> ptmp.id >> ptmp.p.x >> ptmp.p.y;
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

		for(const auto& a : img)
		{
			std::cout << *a.second << std::endl;
		}
		fichier.close();
	}else{
		std::cerr << "impossible d'ouvrir le fichier" << std::endl;
	}
};

template<typename T>
void mosaic<T>::test()
{
	std::cout << "!!!!!!!!!" << std::endl;
}

template<typename T>
void mosaic<T>::compute_mosaic()
{
	int indice;
	while(img.size())
	{
		indice = result.get_id_closest_img();
		if(indice<0)
		{
			std::cerr << "erreur:\n\tfile: " << __FILE__ << "\n\tline: " << __LINE__<<std::endl;
			std::cerr << "0 appariement ????" << std::endl;
			img.clear();
			continue;
		}
		std::unique_ptr<Image<T>>& ptr = img[indice];
		//compute homographie
		//applied homographie to first part of appariement of img[indice]
		//add appariement of img[indice] to result
		//set coordinate of texture of img[indice] to the 4 vertex
		
		image<T> tmp;
		for(const image<T>& i : ptr->get_img())
		{
			tmp.name = i.name;
			for(int j=0; j<4; j++)
			{
//				tmp[i] = h * ptr->img[i];
			}
			result.add_tex(tmp);
		}
		//idem for correspondance

		img.erase(indice);
	}
}
