#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "globals.h"

class Player; // forward declaration

class EnemyManager {
public:
    std::vector<Enemy> enemies;

    void spawn();
    void update();
    void draw(const Player& player) const;
    bool is_colliding(Vector2 pos);
    void remove_colliding(Vector2 pos);
};

#endif // ENEMY_H