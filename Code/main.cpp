#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <fstream>
#include <sstream>
#include <regex>

struct DataFrame
{
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> data;
};

DataFrame readLines(const std::string &filename, int num_lines)
{
    DataFrame df;
    std::ifstream file(filename);

    if (file.is_open())
    {
        std::string line;

        // Lire la première ligne (noms de colonnes)
        /*if (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ' ')) {
                df.columns.push_back(cell);
            }
        } else {
            std::cerr << "File is empty." << std::endl;
            return df;
        }
        */
        // Lire les lignes de données suivantes
        int lines_read = 0;
        while (std::getline(file, line) && lines_read < num_lines)
        {
            if (lines_read >= 1)
            {
                std::vector<std::string> row;

                // Utiliser une expression régulière pour diviser la ligne en fonction d'un ou plusieurs espaces
                std::regex regex("\\s+");
                std::sregex_token_iterator it(line.begin(), line.end(), regex, -1);
                std::sregex_token_iterator end;

                while (it != end)
                {
                    std::string cell = *it;
                    row.push_back(cell);
                    ++it;
                }

                df.data.push_back(row);
            }
            lines_read++;
        }

        file.close();
    }
    else
    {
        std::cerr << "Unable to open the file." << std::endl;
    }

    return df;
}

std::vector<std::string> findRowByColumnValue(const DataFrame &df, const std::string &target_column, const std::string &target_value)
{
    std::vector<std::string> result;

    // Find the index of the target column
    int target_column_index = -1;
    // std::cout <<"df.columns.size(); " << df.columns.size() << std::endl;
    for (int i = 0; i < df.columns.size(); i++)
    {
        if (df.columns[i] == target_column)
        {
            target_column_index = i;
            break;
        }
    }

    if (target_column_index != -1)
    {
        // Search for the target value in the specified column
        for (const std::vector<std::string> &row : df.data)
        {
            if (row.size() > target_column_index && row[target_column_index] == target_value)
            {
                result = row;
                break;
            }
        }
    }

    return result;
}

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

// donne la valeur du dataframe pour l'image donnee et suivant la condition donnee
int valeurSelonImage(DataFrame &df, std::string nomImage, std::string nomCondition){
    size_t tailleDf = df.data.size() ;
    size_t nbColumns = df.data[0].size(); //ATTENTION : il manque la colonne correspondant aux noms d'images
    int valColumn = -1 ;
    for(size_t i = 0 ; i<nbColumns ; i++){
        if(df.data[0][i] == nomCondition){
            valColumn = i+1 ; // on rajoute 1 pour compter cette colonne manquante
            break;
        }
    }
    if(valColumn == -1){
        printf("Mauvaise condition donnee ! ");
	    exit(EXIT_FAILURE);
    }
    int valLine = -1 ;
    for(size_t i = 0 ; i<tailleDf ; i++){
        if(df.data[i][0] == nomImage){
            valLine = i ;
            break;
        }
    }
    if(valLine == -1){
        printf("Mauvais nom d'image donne ! ");
	    exit(EXIT_FAILURE);
    }
    return std::stoi(df.data[valLine][valColumn]); //string to integer a la position trouvee
}


// retrouve l'ensemble des images jpg suivant un label
std::vector<std::string> imagesParLabel(const DataFrame &df, std::string nomCondition, int valeurCondition){
    std::vector<std::string> images ;
    size_t nbColumns = df.data[0].size() ;
    size_t nbImages = df.data.size() ;
    int val = -1 ;
    for(size_t i = 0 ; i<nbColumns ; i++){
        if(df.data[0][i] == nomCondition){
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

// retrouve les parametres suivant une image donnee
std::vector<std::string> parametresImage(DataFrame df, std::string nomImage){
    size_t tailleDf = df.data.size() ;
    int i = 0 ; 
    while(i<tailleDf && df.data[i][0] != nomImage){
        i++ ;
    }
    size_t nbColumns = df.data[i].size() ;
    std::vector<std::string> parametres(nbColumns) ;
    for(int j = 0 ; j < nbColumns ; j++){
        parametres[j] = df.data[i][j] ;
    }
    return parametres ;
}


// retrouve une image proche suivant :
// df : dataframe des donnees (list_landmarks_align_celeba.txt)
// imagesPossibles : ensemble des images candidat
// ensemblesParametre : donnees de l'image de depart (position des yeux, bouche, ...)
std::vector<std::string> imagesProches(DataFrame df, std::vector<std::string> imagesPossibles, std::vector<std::string> ensembleParametres){
    std::vector<std::string> meilleuresImages ;
    size_t nbImages = imagesPossibles.size() ;
    size_t tailleDf = df.data.size() ;
    size_t nbColumns = df.data[0].size() ;
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

int main(int argc, char *argv[])
{
    srand(time(NULL));

    char method_key[250];

    char name_img_1[250], name_img_out[250];

    if (argc != 4)
    {
        printf(" wrong usage \n");
        exit(1);
    }

    sscanf(argv[1], "%s", method_key);
    sscanf(argv[2], "%s", name_img_1);
    sscanf(argv[3], "%s", name_img_out);

    // --------------------------------------------------------------------------------------------
    if(method_key[0] == 'A'){ // methode 1 -> méthode simple

        DataFrame df = readLines("list_attr_celeba.txt", 10002);
        DataFrame dfParametres = readLines("list_landmarks_align_celeba.txt", 10002);

        // image jpg que l'on souhaite modifier
        OCTET *img;
        int nH, nW, nTaille;

        char *nomFichier = nomImageCorrespondante(name_img_1, (char *)"ppm", (char *)"./basePPM/");
        lire_nb_lignes_colonnes_image_ppm(nomFichier, &nH, &nW);
        nTaille = nH * nW * 3;
        // VERIFICATION 
        allocation_tableau(img, OCTET, nTaille);
        lire_image_ppm(nomFichier, img, nH * nW);
        ecrire_image_ppm((char *)"test1.ppm", img, nH, nW);

        // valeur suivant le genre et le nom de l'image
        int valeur = valeurSelonImage(df, std::string(name_img_1), "Male");
        // on veut les images de genre opposé
        std::vector<std::string> imgs = imagesParLabel(df, "Male", -valeur);
        // position des elements du visage sur l'image d'entree
        std::vector<std::string> parametres = parametresImage(dfParametres, std::string(name_img_1));
        // images proches suivant ces paramtres
        std::vector<std::string> imgProches = imagesProches(dfParametres, imgs, parametres);
        int nHres, nWres, nTailleRes ;

        // on teste si ça colle en l'affichant
        // TODO swap faces
        char *nomFichierRes = nomImageCorrespondante(imgProches[0].c_str(), (char *)"ppm", (char *)"./basePPM/");
        lire_nb_lignes_colonnes_image_ppm(nomFichierRes, &nHres, &nWres);
        nTailleRes = nH * nW * 3;
        OCTET *imgOut ;
        allocation_tableau(imgOut, OCTET, nTailleRes);
        lire_image_ppm(nomFichierRes, imgOut, nHres * nWres);
        ecrire_image_ppm(name_img_out, imgOut, nHres, nWres);

        free(img);
        free(imgOut);

    }
    // --------------------------------------------------------------------------------------------
    else{
       printf("first argument is incorrect \n");
    }

    return 1;
}