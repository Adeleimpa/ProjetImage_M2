

struct Image
{
	OCTET* data; // contenu de l'image

	int nH; // hauteur
	int nW; // largeur
	int nTaille; // nre de pixels
	int nTaille3; // nre de pixels quand RGB

	char * name_ppm;
	int index_img;

	void calculTailles(){

		if(nH != 0 and nW != 0){
			nTaille = nH*nW;
			nTaille3 = 3*nTaille;
		}else{
			std::cout << "nH ou nW vaut 0." << std::endl;
		}
		
	}
};