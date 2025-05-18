#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"

class EnemyManager;

class Player {
public:
    Vector2 pos;
    float y_velocity = 0;

    void reset();
    void spawn();
    void kill();
    void move_horizontally(float delta);
    void update(EnemyManager& enemyManager);
    void update_gravity();
    void draw();

private:
    bool on_ground = false;
};

#endif