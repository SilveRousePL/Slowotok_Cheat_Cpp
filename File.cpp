//
// Created by darek on 11.02.19.
//

#include <iostream>
#include "File.h"

File::File(QString filename) {
    handle.setFileName(filename);
    if (!handle.open(QIODevice::ReadOnly | QIODevice::Text))
        std::cout << "Błąd otwarcia! " << filename.toStdString() << std::endl;
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

}

QString File::read() {


}

void File::write(QString object) {

}