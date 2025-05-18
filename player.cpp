#include "player.h"
#include "enemy.h"
#include "level.h"

void Player::reset() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void Player::spawn() {
    y_velocity = 0;
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);
            if (cell == PLAYER) {
                pos.x = column;
                pos.y = row;
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill() {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void Player::move_horizontally(float delta) {
    float next_x = pos.x + delta;
    if (!is_colliding({next_x, pos.y}, WALL)) {
        pos.x = next_x;
    } else {
        pos.x = roundf(pos.x);
        return;
    }
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void Player::update_gravity() {
    if (is_colliding({pos.x, pos.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }
    pos.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    on_ground = is_colliding({pos.x, pos.y + 0.1f}, WALL);
    is_player_on_ground = on_ground;
    if (on_ground) {
        y_velocity = 0;
        pos.y = roundf(pos.y);
    }
}

void Player::update(EnemyManager& enemyManager) {
    update_gravity();

    if (is_colliding(pos, COIN)) {
        get_collider(pos, COIN) = AIR;
        PlaySound(coin_sound);
        player_level_scores[level_index]++;
    }

    if (is_colliding(pos, EXIT)) {
        if (timer > 0) {
            timer -= 25;
            time_to_coin_counter += 5;
            if (time_to_coin_counter / 60 > 1) {
                PlaySound(coin_sound);
                player_level_scores[level_index]++;
                time_to_coin_counter = 0;
            }
        } else {
            load_level(1);
            PlaySound(exit_sound);
        }
    } else {
        if (timer >= 0) timer--;
    }

    if (is_colliding(pos, SPIKE) || pos.y > current_level.rows) kill();

    if (enemyManager.is_colliding(pos)) {
        if (y_velocity > 0) {
            enemyManager.remove_colliding(pos);
            PlaySound(kill_enemy_sound);
            player_level_scores[level_index]++;
            y_velocity = -BOUNCE_OFF_ENEMY;
        } else {
            kill();
        }
    }
}

void Player::draw() {
    float horizontal_shift = (screen_size.x - cell_size) / 2;
    Vector2 draw_pos = { horizontal_shift, pos.y * cell_size };

    if (game_state == GAME_STATE) {
        if (!is_player_on_ground) {
            draw_image((is_looking_forward ? player_jump_forward_image : player_jump_backwards_image), draw_pos, cell_size);
        } else if (is_moving) {
            draw_sprite((is_looking_forward ? player_walk_forward_sprite : player_walk_backwards_sprite), draw_pos, cell_size);
            is_moving = false;
        } else {
            draw_image((is_looking_forward ? player_stand_forward_image : player_stand_backwards_image), draw_pos, cell_size);
        }
    } else {
        draw_image(player_dead_image, draw_pos, cell_size);
    }
}