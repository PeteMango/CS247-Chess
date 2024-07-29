#ifndef LEVELFOUR_H
#define LEVELFOUR_H

#include "player/computer.h"

class LevelFour : public Computer {


public:
    LevelFour(std::shared_ptr<Game> game, Color color);
    ~LevelFour();

    void move() override;
};

#endif
