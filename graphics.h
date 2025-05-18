#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "level.h"

inline void draw_text(Text &text) {
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    Vector2 pos = {
            (screen_size.x * text.position.x) - (0.5f * dimensions.x),
            (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };
    DrawTextEx(*text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

inline void derive_graphics_metrics_from_loaded_level() {
    screen_size = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    cell_size = screen_size.y / current_level.rows;
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;

    float large = std::max(screen_size.x, screen_size.y);
    background_size = screen_size.x > screen_size.y ? Vector2{large, large / 16 * 10} : Vector2{large / 10 * 16, large};
    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

inline void draw_parallax_background(const Player& player) {
    float offset = -(player.pos.x * PARALLAX_PLAYER_SCROLLING_SPEED + game_frame * PARALLAX_IDLE_SCROLLING_SPEED);
    float bg_offset = fmod(offset * background_size.x, background_size.x);
    float mid_offset = fmod(bg_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE, background_size.x);
    float fg_offset = fmod(mid_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE, background_size.x);

    draw_image(background, {bg_offset + background_size.x, background_y_offset}, background_size.x, background_size.y);
    draw_image(background, {bg_offset, background_y_offset}, background_size.x, background_size.y);
    draw_image(middleground, {mid_offset + background_size.x, background_y_offset}, background_size.x, background_size.y);
    draw_image(middleground, {mid_offset, background_y_offset}, background_size.x, background_size.y);
    draw_image(foreground, {fg_offset + background_size.x, background_y_offset}, background_size.x, background_size.y);
    draw_image(foreground, {fg_offset, background_y_offset}, background_size.x, background_size.y);
}

inline void draw_game_overlay(const Player& player) {
    const float ICON_SIZE = 48.0f * screen_scale;
    float offset = 8.0f * screen_scale;

    for (int i = 0; i < player_lives; ++i)
        draw_image(heart_image, {ICON_SIZE * i + 4.0f * screen_scale, offset}, ICON_SIZE);

    Vector2 time_dim = MeasureTextEx(menu_font, std::to_string(timer / 60).c_str(), ICON_SIZE, 2.0f);
    DrawTextEx(menu_font, std::to_string(timer / 60).c_str(), {(screen_size.x - time_dim.x) / 2, offset}, ICON_SIZE, 2.0f, WHITE);

    Vector2 score_dim = MeasureTextEx(menu_font, std::to_string(player.get_total_score()).c_str(), ICON_SIZE, 2.0f);
    DrawTextEx(menu_font, std::to_string(player.get_total_score()).c_str(), {screen_size.x - score_dim.x - ICON_SIZE, offset}, ICON_SIZE, 2.0f, WHITE);
    draw_sprite(coin_sprite, {screen_size.x - ICON_SIZE, offset}, ICON_SIZE);
}

inline void draw_level(const Player& player, const Level& level) {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t col = 0; col < current_level.columns; ++col) {
            Vector2 pos = {(float(col) - player.pos.x) * cell_size + horizontal_shift, float(row) * cell_size};
            char cell = level.get_cell(row, col);
            switch (cell) {
                case WALL: draw_image(wall_image, pos, cell_size); break;
                case WALL_DARK: draw_image(wall_dark_image, pos, cell_size); break;
                case SPIKE: draw_image(spike_image, pos, cell_size); break;
                case COIN: draw_sprite(coin_sprite, pos, cell_size); break;
                case EXIT: draw_image(exit_image, pos, cell_size); break;
                default: break;
            }
        }
    }
}

inline void draw_menu() { draw_text(game_title); draw_text(game_subtitle); }
inline void draw_pause_menu() { draw_text(game_paused); }
inline void draw_game_over_menu() { draw_text(game_over_title); draw_text(game_over_subtitle); }

inline void draw_death_screen(const Player& player, const EnemyManager& enemyManager, const Level& level) {
    draw_parallax_background(player);
    draw_level(player, level);
    player.draw();
    enemyManager.draw(player);
    draw_game_overlay(player);
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), {0, 0, 0, 100});
    draw_text(death_title);
    draw_text(death_subtitle);
}

inline void draw_victory_menu_background() {
    for (auto &ball : victory_balls)
        DrawCircleV({ ball.x, ball.y }, ball.radius, VICTORY_BALL_COLOR);
}

inline void draw_victory_menu() {
    DrawRectangle(0, 0, screen_size.x, screen_size.y, {0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY});
    animate_victory_menu_background();
    draw_victory_menu_background();
    draw_text(victory_title);
    draw_text(victory_subtitle);
}

inline void create_victory_menu_background() {
    // Временная заглушка. Настоящая реализация может создавать анимации, например шары победы.
}

inline void animate_victory_menu_background() {
    // Временная заглушка. Здесь обычно двигаются объекты на фоне.
}

#endif // GRAPHICS_H