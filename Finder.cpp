//
// Created by darek on 10.02.19.
//

#include <cmath>
#include <iostream>
#include "Finder.h"

Finder::Finder() {
    openDictionaryFile("words.txt");

    //Pomysł na optymatlizację:
    //Lista wykluczonych wprowadzana jako arg. funkcji rekurencyjnej i rezygnacja z visited_list

    for(auto i = 0; i < 16; ++i) {
        neighbours_list.push_back(getNeighboursByIndex(i));
    }
    visited_list.reserve(20);
}

Finder::~Finder() {

}

void Finder::openDictionaryFile(QString filename) {
    File f(filename);
    dictionary_words = f.readList();
}

void Finder::startFind(std::vector<QChar> char_list) {
    found_words.clear();
    this->char_list = char_list;
    thread = std::thread(&Finder::run, this);
}

void Finder::run() {
    emit findStarted();
    generateAllPath();
    thread.detach();
    emit findEnded();
}

void Finder::updateProgress(int counter) {
    int now = (counter + 1.0) / dictionary_words.size() * 100;
    int earlier = (counter) / dictionary_words.size() * 100;
    if(earlier != now) {
        emit progressChanged(now);
    }
}

void Finder::updateFoundWordList(QString word) {
    found_words.push_back(word);
    emit wordAdded(word);
}

void Finder::generateAllPath() {
    for(auto i = 0; i < dictionary_words.size(); ++i) {
        is_found = false;
        for(auto j = 0; j < 16; ++j) {
            visited_list.push_back(j);
            recursion(dictionary_words[i], QString() + char_list[j], j, 0);
            visited_list.pop_back();
            if(is_found)
                break;
        }
        updateProgress(i);
    }
}

void Finder::recursion(QString& current_word, QString generated_word, int current_char_index, int depth) {
    if(current_word[depth] != generated_word[depth])
        return;
    if(current_word == generated_word) {
        is_found = true;
        updateFoundWordList(generated_word);
        return;
    }

    std::vector<int> available_list = getExcludeVisited(neighbours_list[current_char_index]);

    if(available_list.empty())
        return;
    for(auto i : available_list) {
        visited_list.push_back(i);
        recursion(current_word, generated_word + char_list[i], i, depth + 1);
        visited_list.pop_back();
        if(is_found)
            return;
    }
}

Point Finder::indexToXY(int index) {
    if(index < 0 || index > 15)
        return Point(-1, -1);
    int x = floor(index / 4);
    int y = floor(index % 4);
    return Point(x, y);
}

int Finder::XYToIndex(int x, int y) {
    if(x < 0 || y < 0 || x > 3 || y > 3)
        return -1;
    return x * 4 + y % 4;
}

std::vector<int> Finder::getNeighboursByXY(int x, int y) {
    std::vector<int> res = {
            XYToIndex(x - 1, y - 1),
            XYToIndex(x - 1, y),
            XYToIndex(x - 1, y + 1),
            XYToIndex(x, y - 1),
            XYToIndex(x, y + 1),
            XYToIndex(x + 1, y - 1),
            XYToIndex(x + 1, y),
            XYToIndex(x + 1, y + 1)
    };
    for(auto i = 0; i < res.size(); ++i) {
        if(res[i] < 0 || res[i] > 15)
            res.erase(res.begin() + i);
    }
    return res;
}

std::vector<int> Finder::getNeighboursByIndex(int index) {
    Point xy = indexToXY(index);
    return getNeighboursByXY(xy.x, xy.y);
}

std::vector<int> Finder::getExcludeVisited(std::vector<int>& neighbours_list) {
    std::vector<int> result;
    result.reserve(neighbours_list.size()+1);
    for(auto i : neighbours_list) {
        bool is_visited = false;
        for(auto j : visited_list) {
            if(i == j) {
                is_visited = true;
                break;
            }
        }
        if(!is_visited)
            result.push_back(i);
    }
    return result;
}
