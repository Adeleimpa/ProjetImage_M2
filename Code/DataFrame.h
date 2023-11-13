#include <fstream>
#include <iostream>


struct DataFrame
{
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> data;


    void readLines(const std::string &filename, int num_lines)
    {
        std::ifstream file(filename);

        if (file.is_open())
        {

            std::string line;

            int lines_read = 0;

            while (std::getline(file, line) && lines_read < num_lines)
            {
                // saute la ligne 0 

                // Lire la première ligne (noms de colonnes)
                if (lines_read == 1){
                    std::istringstream ss(line);
                    std::string cell;

                    while (std::getline(ss, cell, ' ')) {
                        columns.push_back(cell);
                    }
                }

                // Lire les lignes de données suivantes (data)
                if (lines_read >= 2)
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

                    data.push_back(row);
                }
                lines_read++;
            }

            file.close();
        }
        else
        {
            std::cerr << "Unable to open the file." << std::endl;
        }

    }

    void printDataFrame() {

        // Imprimer les noms de colonnes
        std::cout << "---- COLUMNS ----" << std::endl;
        for (std::string column : columns) {
            std::cout << column << " | ";
        }
        std::cout << "\n";

        // Imprimer les lignes de données (5 premières lignes)
        std::cout << "---- DATA ----" << std::endl;
        size_t numRowsToPrint = std::min(data.size(), static_cast<size_t>(5)); // Limiter à 5 lignes au maximum
        for (size_t i = 0; i < numRowsToPrint; ++i) {
            for (std::string cell : data[i]) {
                std::cout << cell << " | ";
            }
            std::cout << "\n";
        }
    }

    // TODO adapt -> pas de colonne 'index'
    /*std::vector<std::string> findRowByColumnValue(const DataFrame &df, const std::string &target_column, const std::string &target_value)
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
    }*/

    // donne la valeur du dataframe pour l'index de l'image donnee et suivant le nom de la colonne donné
    // valeurSelonImage
    int getValue(int rang, std::string nomColonne){
        size_t tailleDf = data.size() ;
        size_t nbColumns = columns.size(); //ATTENTION : il manque la colonne correspondant aux noms d'images

        int valColumn = -1 ;
        for(size_t i = 0 ; i < nbColumns ; i++){
            if(columns[i] == nomColonne){ // colonne trouvée
                valColumn = i+1 ; // on rajoute 1 pour compter cette colonne manquante
                break;
            }
        }
        if(valColumn == -1){
            printf("Mauvais nom de colonne donné ! ");
            exit(EXIT_FAILURE);
        }

        if(rang > tailleDf){
            printf("Mauvais rang donné ! ");
            exit(EXIT_FAILURE);
        }
        return std::stoi(data[rang][valColumn]); //string to integer a la position trouvee
    }

    // parametresImage
    std::vector<std::string> getRang(int rang){
        std::vector<std::string> contenu;

        size_t nbColumns = columns.size();
        for(size_t i = 1 ; i < nbColumns +1 ; i++){ // sauter valeur 0 (i.e. nom de l'image)
            contenu.push_back(data[rang][i]);
        }

        return contenu;
    }

    
};






