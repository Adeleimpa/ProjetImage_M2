#include <stdio.h>
#include <string>
#include "image_ppm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <fstream>
#include <sstream>

struct DataFrame {
    std::vector<std::string> columns;
    std::vector<std::vector<std::string> > data;
};

DataFrame readLines(const std::string& filename, int num_lines) {
    DataFrame df;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;

        // Lire la première ligne (noms de colonnes)
        if (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ' ')) {
                df.columns.push_back(cell);
            }
        } else {
            std::cerr << "File is empty." << std::endl;
            return df;
        }

        // Lire les lignes de données suivantes
        int lines_read = 0;
        while (std::getline(file, line) && lines_read < num_lines) {
            std::vector<std::string> row;
            std::istringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ' ')) {
                row.push_back(cell);
            }

            df.data.push_back(row);
            lines_read++;
        }

        file.close();
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }

    return df;
}

std::vector<std::string> findRowByColumnValue(const DataFrame& df, const std::string& target_column, const std::string& target_value) {
    std::vector<std::string> result;

    // Find the index of the target column
    int target_column_index = -1;
    //std::cout <<"df.columns.size(); " << df.columns.size() << std::endl;
    for (int i = 0; i < df.columns.size(); i++) {
        if (df.columns[i] == target_column) {
            target_column_index = i;
            break;
        }
    }

    if (target_column_index != -1) {
        // Search for the target value in the specified column
        for (const std::vector<std::string>& row : df.data) {
            if (row.size() > target_column_index && row[target_column_index] == target_value) {
                result = row;
                break;
            }
        }
    }

    return result;
}



int main(int argc, char* argv[]){
    srand(time(NULL));

    char method_key[250];

	char name_img_1[250], name_img_2[250], name_img_out[250];
	int nH1, nW1, nTaille1;
    int nH2, nW2, nTaille2;

	if (argc != 5) {
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

        DataFrame df = readLines("list_attr_celeba.txt", 2);
        std::cout << "Columns: ";
        for (const std::string& col : df.columns) {
            std::cout << col << "\t";
        }
        std::cout << std::endl;

        std::cout << "Data: " << std::endl;
        for (const std::vector<std::string>& row : df.data) {
            for (const std::string& cell : row) {
                std::cout << cell << "\t";
            }
            std::cout << std::endl;
        }

        // Search for a row based on the data in a specified column
        const std::string target_column = "index";
        const std::string target_value = "000001.jpg";
        std::vector<std::string> result = findRowByColumnValue(df, target_column, target_value);

        if (!result.empty()) {
            std::cout << "Row found: ";
            for (const std::string& cell : result) {
                std::cout << cell << "\t";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Row not found for value: " << target_value << " in column: " << target_column << std::endl;
        }



    }
// --------------------------------------------------------------------------------------------
    else{
       printf("first argument is incorrect \n"); 
    }


    free(ImgIn1); free(ImgIn2); free(ImgOut);

	return 1;
}