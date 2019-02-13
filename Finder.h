//
// Created by darek on 10.02.19.
//

#ifndef SLOWOTOK_CHEAT_CPP_FINDER_H
#define SLOWOTOK_CHEAT_CPP_FINDER_H


#include <QtCore/QObject>
#include <string>
#include <thread>
#include "File.h"

class Point {
public:
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    int getX() { return x; }
    int getY() { return y; }
    std::vector<int> getXY() { std::vector<int> res; res.reserve(2);
        res.push_back(x); res.push_back(y); return res; }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setXY(int x, int y) { this->x = x; this->y = y; }
};

class Finder : public QObject {
    Q_OBJECT

signals:
    void findStarted();
    void findEnded();
    void progressChanged(int new_progress);
    void wordAdded(QString new_word);

private:
    std::vector<QString> dictionary_words;
    std::vector<QChar> char_list;
    std::vector<std::vector<int>> neighbours_list;
    std::vector<int> visited_list;
    bool is_found;
    std::vector<QString> found_words;

    std::thread searching;

public:
    Finder();
    virtual ~Finder();

    void startFind(std::vector<QChar> char_list);

private:
    void run();

    void updateProgress(int counter);
    void updateWordList(QString word);

    void generateAllPath();
    void recursion(QString& current_word, QString generated_word, int current_char_index, int depth);

    Point indexToXY(int index);
    int XYToIndex(int x, int y);
    std::vector<int> getNeighboursByXY(int x, int y);
    std::vector<int> getNeighboursByIndex(int index);
    std::vector<int> getExcludeVisited(std::vector<int>& neighbours_list);
};


#endif //SLOWOTOK_CHEAT_CPP_FINDER_H
