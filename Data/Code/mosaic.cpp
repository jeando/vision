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

			do{
				ss >> ptmp.id >> ptmp.p.x >> ptmp.p.y;
				vtmp.push_back(ptmp);
			}while(ss.ignore());
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
/*
template<typename T>
vec3<T> operator*(const cv::Mat_<T>& m, const vec2<T>& v)
{
	return vec3<T>(m[0][0]*v.x+m[0][1]*v.y+m[0][2],m[1][0]*v.x+m[1][1]*v.y+m[1][2],m[2][0]*v.x+m[2][1]*v.y+m[2][2]);
}

//*/
/*
template<typename T>
vec2<T> operator*(const cv::Mat_<T>& m, const vec2<T>& v)
{
	T z = m[2][0]*v.x+m[2][1]*v.y+m[2][2];
	return vec2<T>((m[0][0]*v.x+m[0][1]*v.y+m[0][2])/z,(m[1][0]*v.x+m[1][1]*v.y+m[1][2])/z);
}
//*/
template<typename T>
void mosaic<T>::init_compute_mosaic()
{
	int val = -1;
	int indice = -1;
	int tmp_val;
	for(const auto& a : img)
	{
		tmp_val = (*a.second)[a.second->get_id_closest_img()].size();
		if(tmp_val>val)
		{
			val = tmp_val;
			indice = a.first;
			std::cerr << indice << " " << tmp_val << std::endl;
		}
	}
//	result;//ini result
	image<T> tmp;
	for(const image<T>& i : img[indice]->get_img())
	{
		tmp.name = i.name;
		for(int j=0; j<4; j++)
		{
			tmp[j] = i[j];
		}
		result.add_tex(tmp);
	}
	for(const auto& a : img[indice]->get_assoc())
	{
		if(a.first == result.get_id())
			continue;
		img[a.first]->erase(indice);
		for(const correspondance<T>& c : a.second)
		{
			result[a.first].push_back(c);
			(*img[a.first])[result.get_id()].push_back(correspondance<T>(c.p2,c.p1));
		}
	}
	std::cerr << indice << std::endl;
	img.erase(indice);
}
template<typename T>
void mosaic<T>::compute_next_mosaic()
{
	int indice = -1;
	if(!img.size())
	{
		return;
	}
		indice = result.get_id_closest_img();
		if(indice<0)
		{
			std::cerr << "erreur:\n\tfile: " << __FILE__ << "\n\tline: " << __LINE__<<std::endl;
			std::cerr << "0 appariement ????" << std::endl;
			img.clear();
			return;
		}
		std::unique_ptr<Image<T>>& ptr = img[indice];
		//compute homographie
		//applied homographie to first part of appariement of img[indice]
		//add appariement of img[indice] to result
		//set coordinate of texture of img[indice] to the 4 vertex
		
		cv::Mat_<T> h;
	std::cerr << "start compute_homographie " << indice << " " << result.get_id()<<std::endl;
		compute_homographie(result[indice],h);
	std::cerr << "end compute_homographie " << std::endl;

		//add the image indice in the result
		image<T> tmp;
		for(const image<T>& i : ptr->get_img())
		{
			tmp.name = i.name;
			for(int j=0; j<4; j++)
			{
				tmp[j] = h * i[j];
				std::cout << i[j] << " " << tmp[j]<<std::endl;
			}
			result.add_tex(tmp);
		}

		//mise a jour des correspondance dans les deux sens
		correspondance<T> c2;
		std::cout << "indice "<< indice<<std::endl;
		result.erase(indice);
		std::cout << "indice "<< indice<<std::endl;
		for(const auto& a : ptr->get_assoc())
		{
			if(a.first == result.get_id())
				continue;
			img[a.first]->erase(indice);
			for(const correspondance<T>& c : a.second)
			{
				c2.p1 = h * c.p1;
				c2.p2 = c.p2;
				result[a.first].push_back(c2);
				(*img[a.first])[result.get_id()].push_back(correspondance<T>(c.p2,c.p1));
			}
		}
//??erreur		
		img.erase(indice);
}
template<typename T>
void mosaic<T>::compute_homographie(const std::vector<correspondance<T>>& c, cv::Mat_<T>& h)
{
	const size_t size = c.size();
	//const size_t size = 3;
	h = cv::Mat_<T>(9, 1);
	cv::Mat_<T> A(3*size, 9);
	T* ptr = A[0]-1;
	typename std::vector<correspondance<T>>::const_iterator it = c.begin();
	const typename std::vector<correspondance<T>>::const_iterator it_end = c.end();
	//const typename std::vector<correspondance<T>>::const_iterator it_end = it+3;
	T x,y,z,t,tx,ty,zx,zy;
	for(;it!=it_end; it++)
	{
		x=it->p2.x;
		y=it->p2.y;
		z=it->p1.x;
		t=it->p1.y;

		tx=t*x;
		ty=t*y;
		zx=z*x;
		zy=z*y;
	
		*++ptr = 0;
		*++ptr = 0;
		*++ptr = 0;

		*++ptr = -x;
		*++ptr = -y;
		*++ptr = -1;
		
		*++ptr = tx;
		*++ptr = ty;
		*++ptr = t;
		

		*++ptr = x;
		*++ptr = y;
		*++ptr = 1;
		
		*++ptr = 0;
		*++ptr = 0;
		*++ptr = 0;
		
		*++ptr = -zx;
		*++ptr = -zy;
		*++ptr = -z;
		

		*++ptr = -tx;
		*++ptr = -ty;
		*++ptr = -t;
		
		*++ptr = zx;
		*++ptr = zy;
		*++ptr = z;
		
		*++ptr = 0;
		*++ptr = 0;
		*++ptr = 0;
	}
	A/=10000;//to have "good" value
	ptr=A[0];
	for(it=c.begin();it!=it_end; it++)
	{
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<std::endl;
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<std::endl;
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<" ";
		std::cout << *ptr++<<std::endl;
	}
	cv::SVD::solveZ(A,h);
	std::cout << (A*h) << std::endl;
	h=h.reshape(0,3);
	std::cout << h << std::endl;
	std::cout << h.rows << " " << h.cols << std::endl;
	for(const correspondance<T>& cc : c)
	{
		std::cout << cc.p1 << " " << vec2<T>(h*cc.p2)<<std::endl;
		std::cout << cc.p2 << " " << vec2<T>(h*cc.p1)<<std::endl;
		std::cout << cc.p1 << " " << vec3<T>(h*cc.p2)<<std::endl;
		std::cout << cc.p2 << " " << vec3<T>(h*cc.p1)<<std::endl<<std::endl;;
	}
}


