//
// Created by roagen on 08.09.2021.
//

#include "Model.h"

Model::Model(const char *file) {
    std::string text = get_file_contents(file);
    JSON = json::parse(text);

    this->file = file;
    data = getData();
}

std::vector<unsigned char> Model::getData() {

    std::string bytesText;
    std::string uri = JSON["buffers"][0]["uri"];

    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of("/") + 1);
    bytesText = get_file_contents((fileDirectory + uri).c_str());
    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());

    return data;
}
