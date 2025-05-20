#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"

class Level;
class EnemyManager;

class Player {
public:
    Vector2 pos{};
    float y_velocity = 0;

    void reset();
    void spawn(Level& level);
    void kill(Level& level);
    void move_horizontally(float delta, const Level& level);
    void update_gravity(const Level& level);
    void update(EnemyManager& enemyManager, Level& level);
    void draw() const;
    int get_total_score() const;

private:
    static constexpr int MAX_SCORES = 20;
    int level_scores[MAX_SCORES] = {};
};

#endif // PLAYER_H
