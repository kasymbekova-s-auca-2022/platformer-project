#include "raylib.h"
#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "player.h"

bool is_inside_level(int row, int column) {
    return !(row < 0 || row >= current_level.rows || column < 0 || column >= current_level.columns);
}

bool is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float)column, (float)row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) return true;
            }
        }
    }
    return false;
}

char& get_collider(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float)column, (float)row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) return get_level_cell(row, column);
            }
        }
    }

    return get_level_cell(pos.y, pos.x);
}

char& get_level_cell(size_t row, size_t column) {
    return current_level.data[row * current_level.columns + column];
}

void set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}

void spawn_player() {
    player_y_velocity = 0;
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);
            if (cell == PLAYER) {
                player_pos = {(float)column, (float)row};
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}
