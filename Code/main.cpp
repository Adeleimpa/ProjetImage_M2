#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <sstream>
#include <regex>
#include <cmath>
#include "DataFrame.h"
#include "Image.h"


// retrouve le nom .pgm du fichier correspondant au fichier .jpg 
// imageJPG : nom total (exemple : 000001.jpg)
// extensionCible : "pgm" (pour l'instant)
// dossier : chemin menant vers la base de donnees d'images pgm ("./.../")
char *nomImageCorrespondante(const char *imageJPG, const char *extensionCible, const char *dossier)
{
    size_t longueur = strlen(imageJPG);
    size_t longueurDossier = strlen(dossier);
    size_t longueurNom = longueur - 3; // extension "jpg" : 3 caractères

    // Allocation de la mémoire (de taille chemin + nom)
    char *nomImage = (char *)malloc(longueurDossier + longueur);

    if (nomImage != nullptr)
    {
        // Copier le dossier
        strncpy(nomImage, dossier, longueurDossier);

        // Copier le nom de l'image (sans l'extension)
        strncat(nomImage, imageJPG, longueurNom);

        // Ajouter l'extension cible
        strcat(nomImage, extensionCible);
    }
    return nomImage;
}


// retrouve l'ensemble des images jpg suivant un label
// condition = colonne
std::vector<std::string> imagesParLabel(const DataFrame &df, std::string nomCondition, int valeurCondition){
    std::vector<std::string> images ;

    size_t nbColumns = df.columns.size() ;
    size_t nbImages = df.data.size() ;

    int val = -1 ;
    for(size_t i = 0 ; i<nbColumns ; i++){
        if(df.columns[i] == nomCondition){
            val = i+1 ;
            break;
        }
    }
    if(val == -1){
        printf("Mauvaise condition donnee ! ");
	    exit(EXIT_FAILURE);
    }
    else{
        for(size_t i = 1 ; i < nbImages ; i++){
            if(df.data[i][val] == std::to_string(valeurCondition)){
                images.push_back(df.data[i][0]); //rajout des images
            }
        }
    }
    return images ;
}


// retrouve une image proche suivant :
// df : dataframe des donnees (list_landmarks_align_celeba.txt)
// imagesPossibles : ensemble des images candidat
// ensemblesParametre : donnees de l'image de depart (position des yeux, bouche, ...)
std::vector<std::string> imagesProches(DataFrame df, std::vector<std::string> imagesPossibles, std::vector<std::string> ensembleParametres){
    std::vector<std::string> meilleuresImages ;

    size_t nbImages = imagesPossibles.size() ;
    size_t tailleDf = df.data.size() ;
    size_t tailleParam = ensembleParametres.size();

    std::vector<float> distances(nbImages);

    for(size_t i = 1 ; i<tailleDf ; i++){
        for(size_t j = 0 ; j<nbImages ; j++){
            if(df.data[i][0] == imagesPossibles[j]){
                distances[j] = 0 ;
                for(size_t k = 1 ; k<tailleParam ; k++){
                    distances[j] += (std::stoi(ensembleParametres[k])-std::stoi(df.data[i][k])) * (std::stoi(ensembleParametres[k])-std::stoi(df.data[i][k])); //distance euclidienne
                    //std::cout<<df.data[i][k]<<std::endl ;
                    //std::cout<<k<<std::endl ;
                }
            }
        }
    }
    
    for(size_t i = 0 ; i<nbImages ; i++){
        if(distances[i] < 10){ //images proches 
            meilleuresImages.push_back(imagesPossibles[i]);
        }
    }
    int indice ;
    if(!meilleuresImages.empty()){
        indice = rand() % meilleuresImages.size() ;
        meilleuresImages = {meilleuresImages[indice]}; // On selectionne une image parmi les images proches
    } 
    else{ // s'il n'y a aucune image proche
        float minDist = FLT_MAX ; // recuperation de l'image la plus proche
        std::string imageOpti ;
        for(size_t i = 0 ; i<nbImages ; i++){
            if(distances[i]<minDist){
                minDist = distances[i] ;
                imageOpti = imagesPossibles[i] ;
            }
        }
        meilleuresImages = {imageOpti} ;
    }
    
    return meilleuresImages ;
}

int getIndexFromImgName(const char* filename){
    // Convertit le nom de fichier en une chaîne de caractères
    std::string strFilename(filename);

    // Trouve la position du premier caractère non zéro
    size_t start = strFilename.find_first_not_of("0");

    // Obtient la position du point avant l'extension
    size_t end = strFilename.find_last_of('.');

    // Extrait la sous-chaîne contenant le numéro de l'image
    std::string numberStr = strFilename.substr(start, end - start);

    // Convertit la sous-chaîne en entier
    int imageNumber = std::stoi(numberStr);

    return imageNumber;
}

void swapVisages(OCTET * imgSwap, std::vector<std::string> df_data, OCTET *img, OCTET *img2, int nH, int nW){

    // data of image 1
    int x_lefteye = std::stoi(df_data[1]);
    int y_lefteye = std::stoi(df_data[2]);
    int x_righteye = std::stoi(df_data[3]);
    int y_righteye = std::stoi(df_data[4]);
    int x_nose = std::stoi(df_data[5]);
    int y_nose = std::stoi(df_data[6]);
    int x_leftmouth = std::stoi(df_data[7]);
    int y_leftmouth = std::stoi(df_data[8]);
    int x_rightmouth = std::stoi(df_data[9]);
    int y_rightmouth = std::stoi(df_data[10]);

    // trouver centre de l'ovale de découpe
    int x_dist_eyes = x_righteye - x_lefteye;
    int a = std::floor(x_dist_eyes * 1.); // largeur ovale

    int x_center_eyes = x_lefteye + std::floor(x_dist_eyes/2);
    int x_dist_mouth = x_rightmouth - x_leftmouth;
    int x_center_mouth = x_leftmouth + std::floor(x_dist_mouth/2);
    int x_center = std::floor((x_center_eyes + x_center_mouth) / 2);

    int y_dist_eyes;
    int y_center_eyes;
    if(y_righteye > y_lefteye){
        y_dist_eyes = y_righteye - y_lefteye;
        y_center_eyes = y_lefteye + std::floor(y_dist_eyes/2);
    }else{
        y_dist_eyes = y_lefteye - y_righteye;
        y_center_eyes = y_righteye + std::floor(y_dist_eyes/2);
    }
    int y_dist_mouth;
    int y_center_mouth;
    if(y_rightmouth > y_leftmouth){
        y_dist_mouth = y_rightmouth - y_leftmouth;
        y_center_mouth = y_leftmouth + std::floor(y_dist_mouth/2);
    }else{
        y_dist_mouth = y_leftmouth - y_rightmouth;
        y_center_mouth = y_rightmouth + std::floor(y_dist_mouth/2);
    }

    int y_center = std::floor((y_center_eyes + y_center_mouth) / 2);
    int b = std::floor(std::abs(y_center_mouth - y_center_eyes) * 1.3); // hauteur ovale

    /*std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "x_center: " << x_center << std::endl;
    std::cout << "y_center: " << y_center << std::endl;*/

    for(unsigned int i = 0; i < nH*nW; i++){
        // get x and y of image
        int x = i % nW;
        int y = std::floor(i / nW);

        // check if x y is in ovale (formula)
        if( (pow(x-x_center,2) / pow(a,2)) +   (pow(y-y_center,2) / pow(b,2)) <= 1.){
            // if yes -> imgSwap fileld with img2
            imgSwap[3*i] = img2[3*i];
            imgSwap[3*i+1] = img2[3*i+1];
            imgSwap[3*i+2] = img2[3*i+2];
        } else{
            imgSwap[3*i] = img[3*i];
            imgSwap[3*i+1] = img[3*i+1];
            imgSwap[3*i+2] = img[3*i+2];
        }
    }

}

std::vector<std::string> intersection(std::vector<std::string> vect1, std::vector<std::string> vect2){
    size_t tailleVect1 = vect1.size() ;
    size_t tailleVect2 = vect2.size() ;
    std::vector<std::string> result ;
    for(size_t i = 0 ; i<tailleVect1 ; i++){
        for(size_t j = 0 ; j<tailleVect2 ; j++){
            if(vect1[i] == vect2[j]){
                result.push_back(vect1[i]);
            }
        }
    }
    return result ;
}

std::vector<std::string> imagesPossiblesSelonParametres(DataFrame df, const char* nomImage, std::vector<std::string> param, std::vector<std::string> &intersect){
    //std::vector<std::string> inter = intersect ;

    if(!param.empty()){
        int index_img = getIndexFromImgName(nomImage);
        int val = df.getValue(index_img, param[0]);

        std::vector<std::string> imagesSelonCritere = imagesParLabel(df, param[0], val);
        intersect = intersection(intersect, imagesSelonCritere);
        param.erase(param.begin());
        std::cout<<intersect.size()<<std::endl ;

        if(intersect.size() > 50)
            imagesPossiblesSelonParametres(df, nomImage, param, intersect);
        else
            std::cout<<"ici ! "<<std::endl ;
            return intersect;
    } else{
        std::cout << "attention, ne devrait pas imprimer ceci." << std::endl;
        return intersect;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    char method_key[250];

    char name_img_1[250], name_img_out[250], name_img_swap[250];

    if (argc != 5)
    {
        printf(" wrong usage \n");
        exit(1);
    }

    sscanf(argv[1], "%s", method_key);
    sscanf(argv[2], "%s", name_img_1);
    sscanf(argv[3], "%s", name_img_out);
    sscanf(argv[4], "%s", name_img_swap);

    // --------------------------------------------------------------------------------------------
    if(method_key[0] == 'A'){ // methode 1 -> méthode simple

        // --------Créer dataframes (attributs des images de la base de données)--------
        DataFrame df;
        df.readLines("list_attr_celeba.txt", 10002);
        //df.printDataFrame();
        DataFrame dfParametres; 
        dfParametres.readLines("list_landmarks_align_celeba.txt", 10002);
        //dfParametres.printDataFrame();
        // ----------------------------------------------------------------------------


        // -----------------Lecture de l'image d'entrée-------------------------------
        Image image_entree;
        //OCTET *img;
        //int nH, nW, nTaille;
        image_entree.name_ppm = nomImageCorrespondante(name_img_1, (char *)"ppm", (char *)"./basePPM/");
        lire_nb_lignes_colonnes_image_ppm(image_entree.name_ppm, &image_entree.nH, &image_entree.nW);
        image_entree.calculTailles();
        //nTaille = nH * nW * 3;
        image_entree.index_img = getIndexFromImgName(name_img_1);
        // On re-génère l'image d'entrée pour pouvoir la visualiser facilement
        allocation_tableau(image_entree.data, OCTET, image_entree.nTaille3);
        lire_image_ppm(image_entree.name_ppm, image_entree.data, image_entree.nTaille);
        char nomFichierTest[250];
        sprintf(nomFichierTest, "%d_image_originale.ppm", image_entree.index_img);
        ecrire_image_ppm(nomFichierTest, image_entree.data, image_entree.nH, image_entree.nW);
        // ----------------------------------------------------------------------------


        // ----------------------------------------------------------------------------
        // Trouver les images de genre opposé
        // Les filtrer selon certains critères
        // Trouver les images les plus proches (en terme de positions des yeux, nez et bouche)
        // ----------------------------------------------------------------------------
        // valeur suivant le genre et le nom de l'image
        int valeur = df.getValue(image_entree.index_img, "Male");
        // on veut les images de genre opposé
        std::vector<std::string> images_opposees = imagesParLabel(df, "Male", -valeur);

        // filtrer selon critères
        std::vector<std::string> criteres = {"Arched_Eyebrows", "Attractive", "Bags_Under_Eyes", "Big_Lips", "Big_Nose", "Black_Hair", "Blond_Hair", "Brown_Hair", "Bushy_Eyebrows", "Chubby", "Double_Chin", "Eyeglasses", "Gray_Hair", "Heavy_Makeup", "High_Cheekbones", "Mouth_Slightly_Open", "Narrow_Eyes", "Oval_Face", "Pale_Skin", "Pointy_Nose", "Receding_Hairline", "Rosy_Cheeks", "Smiling", "Straight_Hair", "Wavy_Hair", "Wearing_Earrings", "Wearing_Hat", "Young"};
        std::vector<std::string> imagesPossibles = imagesPossiblesSelonParametres(df, name_img_1, criteres, images_opposees);

        // position des elements du visage sur l'image d'entree
        std::vector<std::string> parametres = dfParametres.getRang(image_entree.index_img);

        // images proches suivant ces parametres
        std::vector<std::string> imgProches = imagesProches(dfParametres, imagesPossibles, parametres);
        // ----------------------------------------------------------------------------
        

        
         // -----------------Lecture de l'image opposée-------------------------------
        Image image_opposee;
        image_opposee.name_ppm = nomImageCorrespondante(imgProches[0].c_str(), (char *)"ppm", (char *)"./basePPM/");
        lire_nb_lignes_colonnes_image_ppm(image_opposee.name_ppm, &image_opposee.nH, &image_opposee.nW);
        image_opposee.calculTailles();
        allocation_tableau(image_opposee.data, OCTET, image_opposee.nTaille3);
        lire_image_ppm(image_opposee.name_ppm, image_opposee.data, image_opposee.nTaille);
        ecrire_image_ppm(name_img_out, image_opposee.data, image_opposee.nH, image_opposee.nW);
        // ----------------------------------------------------------------------------


        // -----------------swap les visages-------------------------------
        Image image_swap;
        // /!\ en supposant les images ppm ont la meme taille
        image_swap.nH = image_opposee.nH;
        image_swap.nW = image_opposee.nW;
        image_swap.calculTailles();
        allocation_tableau(image_swap.data, OCTET, image_swap.nTaille3);
        swapVisages(image_swap.data, dfParametres.data[image_entree.index_img], image_entree.data, image_opposee.data, image_swap.nH, image_swap.nW);
        ecrire_image_ppm(name_img_swap, image_swap.data, image_swap.nH, image_swap.nW);
        // ----------------------------------------------------------------------------

        // cleaning
        free(image_entree.data);
        free(image_opposee.data);
        free(image_swap.data);

    }
    // --------------------------------------------------------------------------------------------
    else{
       printf("first argument is incorrect \n");
    }

    return 1;
}