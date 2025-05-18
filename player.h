#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

class Player {
public:
    Vector2 pos{};
    float y_velocity = 0;

    void reset();
    void spawn();
    void kill();
    void move_horizontally(float delta);
    void update_gravity();
    void update(class EnemyManager &enemyManager, class Level &level);
    void draw() const;
    int get_total_score() const;
};

#endif // PLAYER_H