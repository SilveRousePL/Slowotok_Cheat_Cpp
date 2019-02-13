//
// Created by darek on 10.02.19.
//

#include <cmath>
#include <iostream>
#include "Finder.h"

Finder::Finder() {
    File f("words.txt");
    this->dictionary_words = f.readList();

    //std::vector<int> visited_list; // lista odwiedzonych (może stack i modyfikacja sprawdzenia wykluczonych)

    for(auto i = 0; i < 16; ++i) {
        neighbours_list.push_back(getNeighboursByIndex(i));
    }
    visited_list.reserve(20);
}

Finder::~Finder() {

}

void Finder::startFind(std::vector<QChar> char_list) {
    this->found_words.clear();
    this->char_list = char_list;
    this->searching = std::thread(&Finder::run, this);
}

void Finder::run() {
    emit findStarted();
    this->generateAllPath();
    this->searching.detach();
    emit findEnded();
}

void Finder::updateProgress(int counter) {
    int now = (counter + 0.0) / this->dictionary_words.size() * 100;
    int earlier = (counter - 1.0) / this->dictionary_words.size() * 100;
    if(earlier != now) {
        std::cout << now << std::endl;
        emit progressChanged(now);
    }
}

void Finder::updateWordList(QString word) {
    this->found_words.push_back(word);
    emit wordAdded(word);
}

void Finder::generateAllPath() {
    for(auto i = 0; i < this->dictionary_words.size(); ++i) {
        this->is_found = false;
        for(auto j = 0; j < 16; ++j) {
            this->visited_list.push_back(j);
            this->recursion(this->dictionary_words[i], QString() + char_list[j], j, 0);
            this->visited_list.pop_back();
            if(this->is_found)
                break;
        }
        updateProgress(i);
    }
}

void Finder::recursion(QString& current_word, QString generated_word, int current_char_index, int depth) {
    if(current_word[depth] != generated_word[depth])
        return;
    if(current_word == generated_word) {
        this->is_found = true;
        this->updateWordList(generated_word);
        return;
    }

    std::vector<int> available_list = this->getExcludeVisited(this->neighbours_list[current_char_index]);

    if(available_list.empty())
        return;
    for(auto i : available_list) {
        this->visited_list.push_back(i);
        this->recursion(current_word, generated_word + this->char_list[i], i, depth + 1);
        this->visited_list.pop_back();
        if(this->is_found)
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
        for(auto j : this->visited_list) {
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
