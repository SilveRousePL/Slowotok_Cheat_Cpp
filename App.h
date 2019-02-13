//
// Created by darek on 10.02.19.
//

#ifndef SLOWOTOK_CHEAT_CPP_APP_H
#define SLOWOTOK_CHEAT_CPP_APP_H

#include <memory>
#include "Finder.h"

class App {
public:
    std::unique_ptr<Finder> finder;

    App();
    virtual ~App();
};


#endif //SLOWOTOK_CHEAT_CPP_APP_H
