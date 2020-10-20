#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include "CImg.h"

#define ROUND( a ) ( ( (a) < 0 ) ? (int) ( (a) - 0.5 ) : (int) ( (a) + 0.5 ) )

using namespace std;
using namespace cimg_library;

CImg<float> reversedct(const CImg<float>& crop_image, const CImg<>& Q) {

	float ci, cj, coef, cosx, cosy, somme = 0.0;
	CImg<float> tmp(8,8,1,1,0);

	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			somme = 0.0;

			for(int i = 0; i < 8; i++) {
				for(int j = 0; j < 8; j++) {
					if (i == 0) {
						ci = 1.0/sqrt(2);
					} else if (i > 0) {
						ci = 1.0;
					}
					if (j == 0) {
						cj = 1.0/sqrt(2);
					} else if (j > 0) {
						cj = 1.0;
					}
					coef = 0.25 * ci * cj;
					cosx = cos(((2.0*x + 1.0) * i * M_PI)/16.0);
					cosy = cos(((2.0*y + 1.0) * j * M_PI)/16.0);
					somme += coef * crop_image(i,j) * cosx * cosy * Q(i,j);
				}
			}
			tmp(x, y) = somme;
		}
	}
	return tmp;
}

CImg<float> dct(const CImg<float>& crop_image) {

	float ci, cj, coef, cosx, cosy, somme;
	CImg<float> tmp(8,8,1,1,0);
	// DCT
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			somme = 0.0;
			if (i == 0 ) {
				ci = 1.0/sqrt(2);
			} else if (i > 0) {
				ci = 1.0;
			}
			if (j == 0 ) {
				cj = 1.0/sqrt(2);
			} else if (j > 0) {
				cj = 1.0;
			}

			coef = 0.25 * ci * cj;
			for(int x = 0; x < 8; x++) {
				for(int y = 0; y < 8; y++) {
					cosx = cos(((2.0*x + 1.0) * i * M_PI)/16.0);
					cosy = cos(((2.0*y + 1.0) * j * M_PI)/16.0);
					somme += (crop_image(x,y)-128) * cosx * cosy;
				}
			}
			tmp(i,j) = coef * somme;
		}
	}
	return tmp;
}

CImg<float> quantization(CImg<float> image, const CImg<>& Q) {
	CImg<float> imageQuant(image.width(), image.height(), 1,1,0);
	for(int i = 0; i < image.width(); i = i + 8) {
		for(int j = 0; j < image.height(); j = j+ 8) {
			CImg<float> block = image.get_crop(i*8,j*8, i*8+7, j*8+7);
			for(int k = 0; k < 8; k++) {
				for(int l = 0; l < 8; l++) {
					imageQuant(i*8+k,j*8+l) = ROUND(block(k,l) / Q(k,l));
				} 
			}
		}
	}
	return imageQuant;
}


CImg<float> JPEGEncoder(CImg<unsigned char> image, const CImg<>& Q, float quality) {
	image = (CImg<float>) image;
	CImg<float> dctImage(image.width(),image.height(),1,1,0);
	CImg<float> reversedctImage(image.width(),image.height(),1,1,0);
	for(int i = 0; i < 64; i++) {
		for(int j = 0; j < 64; j++) {
			CImg<float> crop_image = image.get_crop(i*8, j*8, i*8 +7, j*8 +7);
			crop_image = dct(crop_image);
			crop_image = quantization(crop_image, Q);
			for(int k = 0; k < 8; k++) {
				for(int l = 0; l < 8; l++) {
					dctImage(i*8 + k, j*8 + l) = crop_image(k,l);
				}
			}
		}
	}
	return dctImage;
}

CImg<float> JPEGDecoder(const CImg<float>& dctImage, const CImg<>& Q, float quality) {
	CImg<float> reversedctImage(dctImage.width(),dctImage.height(),1,1,0);

	for(int i = 0; i < 64; i++) {
		for(int j = 0; j < 64; j++) {
			CImg<float> crop_image = dctImage.get_crop(i*8, j*8, i*8 +7, j*8 +7);
			crop_image = reversedct(crop_image, Q);
			for(int k = 0; k < 8; k++) {
				for(int l = 0; l < 8; l++) {
					reversedctImage(i*8 + k, j*8 + l) = crop_image(k,l);
				}
			}
		}
	}

	return reversedctImage;
}


double distorsion(CImg<unsigned char> image, const CImg<>& Q, float quality) {
	CImg<float> floatImage = (CImg<float>) image;
	double distorsion = 0;
	CImg<float> newImage = JPEGEncoder(image, Q, quality);
	for(int i = 0; i < image.width(); i++) {
		for(int j = 0; j < image.height(); j++) {
			distorsion += pow(abs(floatImage(i,j)-128 - newImage(i,j)) ,2);
		}
	}
	distorsion = distorsion / (double) (image.width() * image.height());
	return distorsion;
}

void zigzag(CImg<> Z){
	int i = 0;
	int j = 0;
	int max = Z.height() - 1;
	bool isCroissant = false;
	while(i<Z.width() && j<Z.height()) {
		cout << Z(i,j) << endl;
		if(i==0 || i== max) {
			if (j==max) {
				i++;
				j--;
			}
			j++;
			cout << Z(i,j) << endl;
		} else {
			if (j==0 || j== max) {
				if(i==max) {
					j++;
					i--;
				}
				i++;
				cout << Z(i,j) << endl;
			}
		}
		if (i == 0 || j == max) { 
			isCroissant = false;
		}
		if (j == 0 || i == max) {
			isCroissant = true;
		}
		if (isCroissant) {
			i--;
			j++;
		} else {
			i++;
			j--;
		}
	}
}


int main() {
	// Read the image "lena.bmp"
	CImg<unsigned char> my_image("lena.bmp");

	// Take the luminance information 
	my_image.channel(0);

	// Quantization matrix
	CImg<> Q(8,8);
	CImg<> Z(8,8);
	int choix;
	float quality=1.0;
	Q(0,0)=16;   Q(0,1)=11;   Q(0,2)=10;   Q(0,3)=16;   Q(0,4)=24;   Q(0,5)=40;   Q(0,6)=51;   Q(0,7)=61;
	Q(1,0)=12;   Q(1,1)=12;   Q(1,2)=14;   Q(1,3)=19;   Q(1,4)=26;   Q(1,5)=58;   Q(1,6)=60;   Q(1,7)=55;
	Q(2,0)=14;   Q(2,1)=13;   Q(2,2)=16;   Q(2,3)=24;   Q(2,4)=40;   Q(2,5)=57;   Q(2,6)=69;   Q(2,7)=56;
	Q(3,0)=14;   Q(3,1)=17;   Q(3,2)=22;   Q(3,3)=29;   Q(3,4)=51;   Q(3,5)=87;   Q(3,6)=80;   Q(3,7)=62;
	Q(4,0)=18;   Q(4,1)=22;   Q(4,2)=37;   Q(4,3)=56;   Q(4,4)=68;   Q(4,5)=109;  Q(4,6)=103;  Q(4,7)=77;
	Q(5,0)=24;   Q(5,1)=35;   Q(5,2)=55;   Q(5,3)=64;   Q(5,4)=81;   Q(5,5)=104;  Q(5,6)=113;  Q(5,7)=92;
	Q(6,0)=49;   Q(6,1)=64;   Q(6,2)=78;   Q(6,3)=87;   Q(6,4)=103;  Q(6,5)=121;  Q(6,6)=120;  Q(6,7)=101;
	Q(7,0)=72;   Q(7,1)=92;   Q(7,2)=95;   Q(7,3)=98;   Q(7,4)=112;  Q(7,5)=100;  Q(7,6)=103;  Q(7,7)=99;


	Z(0,0)=0;    Z(0,1)=1;    Z(0,2)=5;    Z(0,3)=6;    Z(0,4)=14;   Z(0,5)=15;   Z(0,6)=27;   Z(0,7)=28;
  	Z(1,0)=2;    Z(1,1)=4;    Z(1,2)=7;    Z(1,3)=13;   Z(1,4)=16;   Z(1,5)=26;   Z(1,6)=29;   Z(1,7)=42;
  	Z(2,0)=3;    Z(2,1)=8;    Z(2,2)=12;   Z(2,3)=17;   Z(2,4)=25;   Z(2,5)=30;   Z(2,6)=41;   Z(2,7)=43;
  	Z(3,0)=9;    Z(3,1)=11;   Z(3,2)=18;   Z(3,3)=24;   Z(3,4)=31;   Z(3,5)=40;   Z(3,6)=44;   Z(3,7)=53;
  	Z(4,0)=10;   Z(4,1)=19;   Z(4,2)=23;   Z(4,3)=32;   Z(4,4)=39;   Z(4,5)=45;   Z(4,6)=52;   Z(4,7)=54;
  	Z(5,0)=20;   Z(5,1)=22;   Z(5,2)=33;   Z(5,3)=38;   Z(5,4)=46;   Z(5,5)=51;   Z(5,6)=55;   Z(5,7)=60;
  	Z(6,0)=21;   Z(6,1)=34;   Z(6,2)=37;   Z(6,3)=47;   Z(6,4)=50;   Z(6,5)=56;   Z(6,6)=59;   Z(6,7)=61;
  	Z(7,0)=35;   Z(7,1)=36;   Z(7,2)=48;   Z(7,3)=49;   Z(7,4)=57;   Z(7,5)=58;   Z(7,6)=62;   Z(7,7)=63;

	std::cout << "Choississez une action à effectuer : " << std::endl;

	std::cout << "1. Appliquer la DCT et afficher le résultat. Vous devez fournir un facteur de qualité." << std::endl;
	std::cout << "2. Appliquer la DCT puis la DCT inverse et afficher l'image decompressée. Vous devez fournir un facteur de qualité." << std::endl;
	std::cout << "3. Calculer le taux de distorsion en fonction du facteur de qualité. Vous devez fournir un facteur de qualité minium, un facteur de qualité maximum et un pas." << std::endl;
	std::cout << "4. Parcours en zigzag de la matrice Z." << std::endl;
	std::cout << "Votre choix : ";
	std::cin >> choix;
	if (choix > 0 && choix < 3) {
		std::cout << "Facteur de qualité (Exemple 1.0) : " << std::endl;
		std::cin >> quality;
	}

	CImgDisplay main_disp(my_image,"Initial Image");
	CImg<float> image;
	CImgDisplay display;
	std::vector<double> tauxDistorsion;
	switch (choix) {
		case 1:
			Q *= quality;
			image = JPEGEncoder(my_image, Q, quality);
			display = CImgDisplay(image,"DCT Image");
			break;
		case 2:
			Q *= quality;
			image = JPEGEncoder(my_image, Q, quality);
			image = JPEGDecoder(image, Q, quality);
			display = CImgDisplay(image,"Decompressed image");
			break;
		case 3:
			
			double qualityMin, qualityMax, pas, tmpTauxDistorsion;
			std::cout << "Entrer un facteur de qualité minium" << std::endl;
			std::cin >> qualityMin;
			std::cout << "Entrer un facteur de qualité maximum" << std::endl;
			std::cin >> qualityMax;
			std::cout << "Entrer un pas" << std::endl;
			std::cin >> pas;

			for (double i = qualityMin; i <= qualityMax; i = i + pas) {
				tauxDistorsion.push_back(distorsion(my_image, Q * i, i));
			}
			for(auto it = tauxDistorsion.begin(); it != tauxDistorsion.end(); ++it) {
				std::cout << "Facteur : " << qualityMin << " = " << *it << std::endl; 	
				qualityMin += pas;
			}
			break;
		case 4:
			// Parcours en zigzag
			zigzag(Z);
			break;
		default: 
			break;
	}
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}
