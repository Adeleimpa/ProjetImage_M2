#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>



int main(int argc, char* argv[]){
    srand(time(NULL));

    char method_key[250];

	char name_img_1[250], name_img_2[250], name_img_out[250];
	int nH1, nW1, nTaille1;
    int nH2, nW2, nTaille2;

	if (argc != 4) {
        printf(" wrong usage \n");
        exit(1);
    }

    sscanf(argv[1], "%s", method_key);
    sscanf(argv[2], "%s", name_img_1);
    sscanf(argv[3], "%s", name_img_2);
    sscanf(argv[4], "%s", name_img_out);

    OCTET *ImgIn1, *ImgIn2, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(name_img_1, &nH1, &nW1);
    nTaille1 = nH1 * nW1 * 3;

    lire_nb_lignes_colonnes_image_ppm(name_img_2, &nH2, &nW2);
    nTaille2 = nH2 * nW2 * 3;

    

    allocation_tableau(ImgIn1, OCTET, nTaille1);
    allocation_tableau(ImgIn2, OCTET, nTaille2);
    allocation_tableau(ImgOut, OCTET, nTaille1);


// --------------------------------------------------------------------------------------------
    if(method_key[0] == 'A'){ // methode 1 -> face swap

        
// --------------------------------------------------------------------------------------------
    else{
       printf("first argument is incorrect \n"); 
    }


    ecrire_image_pgm(name_img_out, ImgOut, nH, nW);
    free(ImgIn); free(ImgOut);

	return 1;
}