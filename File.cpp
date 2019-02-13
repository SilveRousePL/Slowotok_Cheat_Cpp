//
// Created by darek on 11.02.19.
//

#include <iostream>
#include "File.h"

File::File(QString filename) {
    handle.setFileName(filename);
    if (!handle.open(QIODevice::ReadWrite | QIODevice::Text))
        throw std::exception();
}

File::~File() {
    if(handle.isOpen())
        handle.close();
}

std::vector<QString> File::readList() {
    std::vector<QString> result;
    QTextStream in(&handle);
    while (!in.atEnd()) {
        result.push_back(in.readLine());
    }
    return result;
}

void File::writeList(std::vector<QString> list) {
    QTextStream out(&handle);
    for(auto i : list) {
        out << i << "\n";
    }
}