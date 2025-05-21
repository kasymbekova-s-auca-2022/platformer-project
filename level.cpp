#include "level.h"
#include "graphics.h"
#include "rle_loader.h"

inline std::vector<RLELevel> loaded_levels; //временно.

void Level::reset_index() {
    level_index_ = 0;
}

void Level::load(int offset, EnemyManager* enemyManager) {
    if (loaded_levels.empty()) {
        loaded_levels = load_rle_levels("data/levels.rll");
    }

    level_index_ += offset;

    if (level_index_ >= (int)loaded_levels.size()) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index_ = 0;
        return;
    }

    RLELevel& l = loaded_levels[level_index_];
    current_level_data = new char[l.rows * l.cols];
    std::copy(l.data.begin(), l.data.end(), current_level_data);
    current_level = {l.rows, l.cols, current_level_data};

    if (enemyManager) enemyManager->spawn();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void Level::unload() {
    delete[] current_level_data;
    current_level_data = nullptr;
}

bool Level::is_inside(int row, int col) const {
    return row >= 0 && row < static_cast<int>(current_level.rows) &&
           col >= 0 && col < static_cast<int>(current_level.columns);
}

bool Level::is_colliding(Vector2 pos, char look_for) const {
    int row = static_cast<int>(floor(pos.y));
    int col = static_cast<int>(round(pos.x));

    if (!is_inside(row, col)) return false;

    if (get_cell(row, col) == look_for) {
        Rectangle block = {(float)col, (float)row, 1.0f, 1.0f};
        Rectangle hitbox = {pos.x, pos.y, 1.0f, 1.0f};
        return CheckCollisionRecs(hitbox, block);
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

    for (int row = static_cast<int>(pos.y) - 1; row <= static_cast<int>(pos.y); ++row) {
        for (int col = static_cast<int>(pos.x) - 1; col <= static_cast<int>(pos.x); ++col) {
            if (!is_inside(row, col)) continue;

            if (get_cell(row, col) == look_for) {
                Rectangle block = {(float) col, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(hitbox, block)) {
                    return get_cell(row, col);
                }
            }
        }
    }

    return get_cell(static_cast<size_t>(pos.y), static_cast<size_t>(pos.x));
}
