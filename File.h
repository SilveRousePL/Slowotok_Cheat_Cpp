//
// Created by darek on 11.02.19.
//

#ifndef SLOWOTOK_CHEAT_CPP_FILE_H
#define SLOWOTOK_CHEAT_CPP_FILE_H

#include <vector>
#include <string>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

class File {
    QFile handle;

public:
    File(QString filename);
    virtual ~File();

    std::vector<QString> readList();
    void writeList(std::vector<QString> list);

private:
    QString read();
    void write(QString object);
};


#endif //SLOWOTOK_CHEAT_CPP_FILE_H
