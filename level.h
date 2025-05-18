#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "globals.h"

class EnemyManager;

class Level {
public:
    level current;
    char* current_data = nullptr;
    int index = 0;

    void reset_index();
    void load(int offset = 0, EnemyManager* enemyManager = nullptr);
    void unload();

    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char chr);
};

bool is_inside_level(int row, int column);
bool is_colliding(Vector2 pos, char look_for);
char& get_collider(Vector2 pos, char look_for);

#endif