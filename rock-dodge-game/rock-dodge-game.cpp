#include "splashkit.h"

const double SCREEN_WIDTH = 1280;
const double SCREEN_HEIGHT = 720;
const int MARGIN = 10;
const int MAX_ROCKS = 100;
const int PLAYER_SIZE = 30;
const int START_LIVES = 3;
const int PLAYER_SPEED = 3;

struct rock_data
{
    point_2d position;
    int size;
    int speed;
};

struct game_data
{
    rock_data rocks[MAX_ROCKS];
    int rock_count;
    int score;
    point_2d player;
    int lives;
};

/**
 * Function: new_rock
 * Purpose: Generates a random size, speed and x position within their thresholds
 * and creates a new rock starting just above the top of the screen
 *
 * @return the new rock created
 */
rock_data new_rock()
{
    rock_data rock;
    rock.size = rnd(20, 200);
    rock.speed = rnd(1, 5);
    rock.position.x = rnd(0, SCREEN_WIDTH);
    rock.position.y = - rock.size;

    return rock;
}

/**
 * Function: new_game
 * Purpose: Sets all variables to their initial values to generate the start of the game
 *
 * @return the new game created
 */
game_data new_game()
{
    game_data game;
    game.rock_count = 0;
    game.player.x = SCREEN_WIDTH / 2;
    game.player.y = SCREEN_HEIGHT - PLAYER_SIZE - MARGIN;
    game.score = 0;
    game.lives = START_LIVES;

    return game;
}

/**
 * Procedure: remove_rock
 * Purpose: Removes the rock at the given index by moving the last rock into its
 * slot and shrinking the count (a fast way to remove from an array)
 *
 * @param game the game to remove the rock from
 * @param index the index of the rock to remove
 */
void remove_rock(game_data &game, int index)
{
    game.rocks[index] = game.rocks[game.rock_count - 1];
    game.rock_count--;
}

/**
 * Procedure: add_rock
 * Purpose: Checks if the number of rocks is less than the max number,
 * if so, creates a new rock and adds it to the game, incrementing rock_count and score
 *
 * @param game the game to add the rock to
 */
void add_rock(game_data &game)
{
    if (game.rock_count < MAX_ROCKS)
    {
        game.rocks[game.rock_count] = new_rock();
        game.rock_count++;
        game.score++;
    }
}

/**
 * Procedure: update_game
 * Purpose: Moves every rock, removes the ones that leave the screen (rewarding
 * the player), checks for collisions, and occasionally spawns a new rock
 *
 * @param game the game to update
 */
void update_game(game_data &game)
{
    // Move the rocks down (falling effect)
    // and remove any rocks that fall off the screen
    int i = 0;
    while (i < game.rock_count)
    {
        game.rocks[i].position.y += game.rocks[i].speed;

        // Check if the rock is offscreen
        if (game.rocks[i].position.y - game.rocks[i].size > SCREEN_HEIGHT)
        {
            // Reward for dodging: bigger rocks are worth more
            game.score += game.rocks[i].size;
            remove_rock(game, i);
        }
        else
        {
            i++;
        }
    }

    // Check rock collision
    i = 0;
    while (i < game.rock_count)
    {
        // Distance between the rock and the player
        double dx = game.rocks[i].position.x - game.player.x;
        double dy = game.rocks[i].position.y - game.player.y;

        // Use Pythagoras' theorem to check whether the two circles collide
        double distance_squared = dx * dx + dy * dy; // c^2 = a^2 + b^2
        double radii = game.rocks[i].size + PLAYER_SIZE;

        if (distance_squared < radii * radii)
        {
            remove_rock(game, i);
            game.lives--;
        }
        else
        {
            i++;
        }
    }

    // 3% chance of spawning a new rock each frame
    if (rnd(100) <= 2)
    {
        add_rock(game);
    }
}

/**
 * Procedure: handle_input
 * Purpose: Moves the player according to user input, wrapping around the screen
 * if the player goes offscreen
 *
 * @param game the game whose player is being moved
 */
void handle_input(game_data &game)
{
    // Player movement
    if (key_down(LEFT_KEY))
    {
        game.player.x -= PLAYER_SPEED;
    }
    if (key_down(RIGHT_KEY))
    {
        game.player.x += PLAYER_SPEED;
    }

    // Wrap around if offscreen
    if (game.player.x - PLAYER_SIZE > SCREEN_WIDTH)
    {
        game.player.x = -PLAYER_SIZE;
    }
    if (game.player.x + PLAYER_SIZE < 0)
    {
        game.player.x = SCREEN_WIDTH + PLAYER_SIZE;
    }
}

/**
 * Procedure: draw_game
 * Purpose: Draws the game elements (score, lives, player and rocks) onto the window
 *
 * @param game the game to draw
 */
void draw_game(const game_data &game)
{
    clear_screen(COLOR_DARK_SLATE_BLUE);

    draw_text("Score: " + to_string(game.score), COLOR_WHITE, 10, 10);
    draw_text("Lives: " + to_string(game.lives), COLOR_WHITE, 10, 30);

    fill_circle(COLOR_BROWN, game.player.x, game.player.y, PLAYER_SIZE);
    draw_circle(COLOR_WHITE, game.player.x, game.player.y, PLAYER_SIZE);

    for (int i = 0; i < game.rock_count; i++)
    {
        fill_circle(COLOR_GRAY, game.rocks[i].position.x, game.rocks[i].position.y, game.rocks[i].size);
        draw_circle(COLOR_WHITE, game.rocks[i].position.x, game.rocks[i].position.y, game.rocks[i].size);
    }

    refresh_screen(60);
}

int main()
{
    game_data game = new_game();

    open_window("Rock Dodge - by Khubaib Ejaz", SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!quit_requested() && game.lives > 0)
    {
        process_events();

        handle_input(game);
        update_game(game);
        draw_game(game);
    }

    // Game over screen
    clear_screen(COLOR_BLACK);
    draw_text("GAME OVER", COLOR_RED, 590, 340);
    draw_text("Final score: " + to_string(game.score), COLOR_WHITE, 590, 370);
    refresh_screen(60);
    delay(3000);

    return 0;
}