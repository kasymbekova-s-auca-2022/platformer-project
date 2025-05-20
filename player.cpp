#include "player.h"
#include "enemy.h"
#include "level.h"

void Player::reset() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < 20; ++i) {
        level_scores[i] = 0;
    }
}

void Player::spawn(Level& level) {
    y_velocity = 0;
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t col = 0; col < current_level.columns; ++col) {
            if (level.get_cell(row, col) == PLAYER) {
                pos = {static_cast<float>(col), static_cast<float>(row)};
                level.set_cell(row, col, AIR);
                return;
            }
        }
    }
}

void Player::kill(Level& level) {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    level_scores[level.get_index()] = 0;
}

void Player::move_horizontally(float delta, const Level& level) {
    float next_x = pos.x + delta;
    if (!level.is_colliding({next_x, pos.y}, WALL)) {
        pos.x = next_x;
    } else {
        pos.x = roundf(pos.x);
    }

    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void Player::update_gravity(const Level& level) {
    if (level.is_colliding({pos.x, pos.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }

    pos.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    if (level.is_colliding({pos.x, pos.y + 0.1f}, WALL)) {
        y_velocity = 0;
        pos.y = roundf(pos.y);
    }
}

void Player::update(EnemyManager &enemyManager, Level &level) {
    update_gravity(level);

    Vector2 center = {pos.x + 0.5f, pos.y + 0.5f};

    if (level.is_colliding(center, COIN)) {
        level.set_cell((size_t)pos.y, (size_t)pos.x, AIR);
        PlaySound(coin_sound);
        level_scores[level.get_index()]++;
    }

    if (level.is_colliding(center, EXIT)) {
        PlaySound(exit_sound);
        level.load(1, &enemyManager);
        return;
    }

    if (level.is_colliding(center, SPIKE) || pos.y > level.get_rows()) {
        kill(level);
        return;
    }

    if (enemyManager.is_colliding(pos)) {
        if (y_velocity > 0) {
            enemyManager.remove_colliding(pos);
            PlaySound(kill_enemy_sound);
            level_scores[level.get_index()]++;
            y_velocity = -BOUNCE_OFF_ENEMY;
        } else {
            kill(level);
        }
    }
}

void Player::draw() const {
    float shift = (screen_size.x - cell_size) / 2;
    Vector2 draw_pos = {shift, pos.y * cell_size};

    if (game_state == GAME_STATE) {
        if (!is_player_on_ground)
            draw_image(is_looking_forward ? player_jump_forward_image : player_jump_backwards_image, draw_pos, cell_size);
        else if (is_moving)
            draw_sprite(is_looking_forward ? player_walk_forward_sprite : player_walk_backwards_sprite, draw_pos, cell_size);
        else
            draw_image(is_looking_forward ? player_stand_forward_image : player_stand_backwards_image, draw_pos, cell_size);
        is_moving = false;
    } else {
        draw_image(player_dead_image, draw_pos, cell_size);
    }
}

int Player::get_total_score() const {
    int sum = 0;
    for (int i = 0; i < 20; ++i) {
        sum += level_scores[i];
    }
    return sum;
}
