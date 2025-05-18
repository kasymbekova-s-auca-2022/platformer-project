#include "level.h"
#include "player.h"
#include "enemy.h"
#include "graphics.h"

void Level::reset_index() {
    index = 0;
}

void Level::load(int offset, EnemyManager* enemyManager) {
    index += offset;

    if (index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        index = 0;
        return;
    }

    size_t rows = LEVELS[index].rows;
    size_t cols = LEVELS[index].columns;
    current_data = new char[rows * cols];

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            current_data[r * cols + c] = LEVELS[index].data[r * cols + c];
        }
    }

    current = {rows, cols, current_data};
    spawn_player();
    if (enemyManager) {
        enemyManager->spawn();
    }

    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void Level::unload() {
    delete[] current_data;
}

char& Level::get_cell(size_t row, size_t column) {
    return current.data[row * current.columns + column];
}

void Level::set_cell(size_t row, size_t column, char chr) {
    get_cell(row, column) = chr;
}

bool is_inside_level(int row, int column) {
    if (row < 0 || row >= current_level.rows) return false;
    if (column < 0 || column >= current_level.columns) return false;
    return true;
}

bool is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& get_collider(Vector2 pos, char look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }
    return get_level_cell(pos.x, pos.y);
}