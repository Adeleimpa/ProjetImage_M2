

struct Face {
    int x_lefteye, y_lefteye;
    int x_righteye, y_righteye;
    int x_nose, y_nose;
    int x_leftmouth, y_leftmouth;
    int x_rightmouth, y_rightmouth;
};


Face setFaceCaracteristics(const std::vector<std::string>& df_data) {
    return Face {
        std::stoi(df_data[1]), std::stoi(df_data[2]),
        std::stoi(df_data[3]), std::stoi(df_data[4]),
        std::stoi(df_data[5]), std::stoi(df_data[6]),
        std::stoi(df_data[7]), std::stoi(df_data[8]),
        std::stoi(df_data[9]), std::stoi(df_data[10])
    };
}

int getA(const Face& face_caract) {
    int x_dist_eyes = face_caract.x_righteye - face_caract.x_lefteye;
    return std::floor(x_dist_eyes * 1.);
}

int getB(const Face& face_caract) {
    int y_dist_eyes = std::abs(face_caract.y_righteye - face_caract.y_lefteye);
    int y_dist_mouth = std::abs(face_caract.y_rightmouth - face_caract.y_leftmouth);

    int y_center_eyes = std::min(face_caract.y_lefteye, face_caract.y_righteye) + std::floor(y_dist_eyes/2);
    int y_center_mouth = std::min(face_caract.y_leftmouth, face_caract.y_rightmouth) + std::floor(y_dist_mouth/2);

    int max_y_dist = std::max(y_dist_eyes, y_dist_mouth);

    return std::floor(std::abs(y_center_mouth - y_center_eyes) * 1.3);
}

int getXCenter(const Face& face_caract) {
    int x_dist_eyes = face_caract.x_righteye - face_caract.x_lefteye;
    int x_center_eyes = face_caract.x_lefteye + std::floor(x_dist_eyes / 2);
    int x_dist_mouth = face_caract.x_rightmouth - face_caract.x_leftmouth;
    int x_center_mouth = face_caract.x_leftmouth + std::floor(x_dist_mouth / 2);
    return std::floor((x_center_eyes + x_center_mouth) / 2);
}

int getYCenter(const Face& face_caract) {
    int y_center_eyes = (face_caract.y_righteye > face_caract.y_lefteye) ?
        face_caract.y_lefteye + std::floor(std::abs(face_caract.y_righteye - face_caract.y_lefteye) / 2) :
        face_caract.y_righteye + std::floor(std::abs(face_caract.y_lefteye - face_caract.y_righteye) / 2);
    int y_center_mouth = (face_caract.y_rightmouth > face_caract.y_leftmouth) ?
        face_caract.y_leftmouth + std::floor(std::abs(face_caract.y_rightmouth - face_caract.y_leftmouth) / 2) :
        face_caract.y_rightmouth + std::floor(std::abs(face_caract.y_leftmouth - face_caract.y_rightmouth) / 2);
    return std::floor((y_center_eyes + y_center_mouth) / 2);
}