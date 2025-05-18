#include "enemy.h"
#include "player.h"
#include "level.h"

void EnemyManager::spawn() {
    enemies.clear();
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);
            if (cell == ENEMY) {
                enemies.push_back({ {static_cast<float>(column), static_cast<float>(row)}, true });
                set_level_cell(row, column, AIR);
            }
        }
    }
}

void EnemyManager::update() {
    for (auto &enemy : enemies) {
        float next_x = enemy.pos.x + (enemy.is_looking_right ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);
        if (::is_colliding({next_x, enemy.pos.y}, WALL)) {
            enemy.is_looking_right = !enemy.is_looking_right;
        } else {
            enemy.pos.x = next_x;
        }
    }
}

bool EnemyManager::is_colliding(Vector2 pos) {
    Rectangle hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (auto &enemy : enemies) {
        Rectangle e_hitbox = {enemy.pos.x, enemy.pos.y, 1.0f, 1.0f};
        if (CheckCollisionRecs(hitbox, e_hitbox)) return true;
    }
    return false;
}

void EnemyManager::remove_colliding(Vector2 pos) {
    Rectangle hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        Rectangle e_hitbox = {it->pos.x, it->pos.y, 1.0f, 1.0f};
        if (CheckCollisionRecs(hitbox, e_hitbox)) {
            enemies.erase(it);
            remove_colliding(pos);
            return;
        }
    }
}

void EnemyManager::draw(const Player& player) const {
    float h_shift = (screen_size.x - cell_size) / 2;
    for (auto &enemy : enemies) {
        Vector2 pos = {(enemy.pos.x - player.pos.x) * cell_size + h_shift, enemy.pos.y * cell_size};
        draw_sprite(enemy_walk, pos, cell_size);
    }
}