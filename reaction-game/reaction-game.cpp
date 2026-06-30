#include "splashkit.h"
#include "utilities.h"

const int MIN_TARGET_RADIUS = 30;
const int MAX_TARGET_RADIUS = 100;

// A single target: where it is and how big it is.
struct target_data
{
    double x;
    double y;
    double radius;
};

// All the state for one round of the game, including the running stats.
struct reaction_game_data
{
    target_data target;
    int hits; // targets hit so far
    int total_targets; // how many to hit this round
    int total_time; // sum of all reaction times (ms), used for the average
    int best_time; // fastest single reaction (ms)
    int worst_time; // slowest single reaction (ms)
};

target_data new_target(double x, double y, double radius)
{
    target_data my_target = {x, y, radius};

    return my_target;
}

target_data random_target()
{
    double radius = rnd(MAX_TARGET_RADIUS - MIN_TARGET_RADIUS) + MIN_TARGET_RADIUS;
    double x = rnd(radius, screen_width() - radius);
    double y = rnd(radius, screen_height() - radius);

    return new_target(x, y, radius);
}

void draw_target(target_data target)
{
    fill_circle(COLOR_LIGHT_BLUE, target.x, target.y, target.radius);
}

bool mouse_over_target(target_data target)
{
    return point_in_circle(mouse_x(), mouse_y(), target.x, target.y, target.radius);
}

reaction_game_data new_reaction_game(int total_targets)
{
    reaction_game_data new_game;

    new_game.target = random_target();
    new_game.hits = 0;
    new_game.total_targets = total_targets;
    new_game.total_time = 0;
    new_game.best_time = 0;
    new_game.worst_time = 0;

    return new_game;
}

void draw_game(reaction_game_data game)
{
    clear_screen(COLOR_WHITE);

    // Progress: the target being aimed at is hits + 1.
    draw_text("Target: " + to_string(game.hits + 1) + " / " + to_string(game.total_targets), COLOR_BLACK, 5, 10);

    // Live timer for the current target, read straight from the running timer.
    draw_text("Time: " + to_string(int(timer_ticks("reaction"))) + " ms", COLOR_BLACK, 5, 30);

    draw_target(game.target);

    refresh_screen(60);
}

// Returns a rating based on the average reaction time (ms).
string performance_tier(int average_time)
{
    if (average_time < 400)
    {
        return "INSANE - are you even human?!";
    }
    else if (average_time < 600)
    {
        return "Professional gamer reflexes!";
    }
    else if (average_time < 850)
    {
        return "Sharp - nicely done.";
    }
    else if (average_time < 1200)
    {
        return "Solid - room to sharpen up.";
    }
    else
    {
        return "Keep practicing - you'll get faster!";
    }
}

int main()
{
    int total_targets = read_integer("How many targets would you like to hit? ", 1, 20);
    write_line("Click " + to_string(total_targets) + " targets as fast as you can!");

    open_window("Reaction Timer - By Khubaib Ejaz", 800, 600);

    reaction_game_data game = new_reaction_game(total_targets);

    // Timer measures how long the current target has been on screen.
    create_timer("reaction");
    start_timer("reaction");

    while (!quit_requested() && game.hits < game.total_targets)
    {
        process_events();

        if (mouse_clicked(LEFT_BUTTON) && mouse_over_target(game.target))
        {
            // How long this target took, in milliseconds.
            int reaction = timer_ticks("reaction");

            game.hits++;
            game.total_time += reaction;

            // On the first hit there's nothing to compare against, so this
            // reaction becomes both the best and the worst. After that, just
            // check whether it beats the current best or worst.
            if (game.hits == 1)
            {
                game.best_time = reaction;
                game.worst_time = reaction;
            }
            else
            {
                if (reaction < game.best_time)
                {
                    game.best_time = reaction;
                }
                if (reaction > game.worst_time)
                {
                    game.worst_time = reaction;
                }
            }

            // Next target, and restart the timer for it.
            game.target = random_target();
            reset_timer("reaction");
        }

        draw_game(game);
    }

    // Round over (or the player quit early). Print the summary to the terminal.
    write_line("");

    if (game.hits > 0)
    {
        write_line("All done! Here are your results:");
        write_line("Targets hit:  " + to_string(game.hits));
        write_line("Average time: " + to_string(game.total_time / game.hits) + " ms");
        write_line("Best time:    " + to_string(game.best_time) + " ms");
        write_line("Worst time:   " + to_string(game.worst_time) + " ms");
        write_line("Rating:       " + performance_tier(game.total_time / game.hits));
    }
    else
    {
        write_line("No targets were hit this time. Maybe next round!");
    }

    write_line("Thanks for playing!");

    return 0;
}