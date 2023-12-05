#pragma once

//  Game Rules
static constexpr int PILL_SCORE{ 10 };
static constexpr int ENEMY_KILLED_SCORE{ 50 };
static constexpr float CHARACTER_SPEED{ 100.0f };
static constexpr float CHARACTER_SLOWED_SPEED{ CHARACTER_SPEED * 0.75 };
static constexpr int PLAYER_LIFE_AMOUNT{ 3 };
static constexpr float POWER_PILL_DURATION{ 10.0f };

// Other Constants
static constexpr float CELL_SIZE{ 32.0f };  // constexpr specifies that the value of an object or a function can be evaluated at compile-time and the expression can be used in other constant expressions. Need this to set in other classes.

static constexpr int PACMAN_SIZE{ 13 };
static constexpr int ENEMY_SIZE{ 14 };
static constexpr int MAX_ITERATIONS{ 1000 };
static constexpr float ANIMATION_SPEED{ 0.25f };