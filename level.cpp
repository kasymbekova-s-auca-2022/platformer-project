#include "level.h"
#include "graphics.h"

void Level::reset_index() {
    level_index_ = 0;
}

void Level::load(int offset, EnemyManager* enemyManager) {
    level_index_ += offset;

    if (level_index_ >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index_ = 0;
        return;
    }

    size_t rows = LEVELS[level_index_].rows;
    size_t cols = LEVELS[level_index_].columns;
    current_level_data = new char[rows * cols];

    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < cols; ++col)
            current_level_data[row * cols + col] = LEVELS[level_index_].data[row * cols + col];

    current_level = {rows, cols, current_level_data};

    if (enemyManager) enemyManager->spawn();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void Level::unload() {
    delete[] current_level_data;
}

bool Level::is_inside(int row, int col) const {
    return row >= 0 && row < current_level.rows && col >= 0 && col < current_level.columns;
}

bool Level::is_colliding(Vector2 pos, char look_for) const {
    Rectangle hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = int(pos.y) - 1; row <= int(pos.y); ++row) {
        for (int col = int(pos.x) - 1; col <= int(pos.x); ++col) {
            if (!is_inside(row, col)) continue;
            if (get_level_cell(row, col) == look_for) {
                Rectangle block = {(float) col, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(hitbox, block)) return true;
            }
        }
    }
    return false;
}

char& Level::get_cell(size_t row, size_t col) {
    return current_level.data[row * current_level.columns + col];
}

char Level::get_cell(size_t row, size_t col) const {
    return current_level.data[row * current_level.columns + col];
}

void Level::set_cell(size_t row, size_t col, char value) {
    current_level.data[row * current_level.columns + col] = value;
}

char& Level::get_collider(Vector2 pos, char look_for) {
    Rectangle hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = int(pos.y) - 1; row <= int(pos.y); ++row) {
        for (int col = int(pos.x) - 1; col <= int(pos.x); ++col) {
            if (!is_inside(row, col)) continue;
            if (get_cell(row, col) == look_for) {
                Rectangle block = {(float) col, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(hitbox, block)) return get_cell(row, col);
            }
        }
    }
    return get_cell(pos.y, pos.x);
}
