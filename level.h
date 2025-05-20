#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "enemy.h"
#include "player.h"

class Level {
public:
    void reset_index();
    void load(int offset = 0, EnemyManager* enemyManager = nullptr);
    void unload();

    bool is_inside(int row, int col) const;
    bool is_colliding(Vector2 pos, char look_for = '#') const;
    char& get_cell(size_t row, size_t col);
    char get_cell(size_t row, size_t col) const;
    void set_cell(size_t row, size_t col, char value);
    char& get_collider(Vector2 pos, char look_for);

    size_t get_index() const { return level_index_; }
    size_t get_rows() const { return current_level.rows; }
    size_t get_cols() const { return current_level.columns; }

private:
    int level_index_ = 0;
};

#endif // LEVEL_H