// Compile Instructions: skm clang++ click-game.cpp utilities.cpp -o click-game
// ./click-game

#include "splashkit.h"
#include "utilities.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int TARGET_RADIUS = 75;

const double BAR_X = 50.0;
const double BAR_Y = 50.0;
const double BAR_WIDTH = 700.0;
const double BAR_HEIGHT = 50.0;
const double BAR_BORDER = 2.0;

/**
 * Returns true if the left mouse button was clicked inside the target this frame.
 */
bool target_hit(double target_x, double target_y, int target_size)
{
    return  mouse_clicked(LEFT_BUTTON) && 
            point_in_circle(point_at(mouse_x(), mouse_y()), circle_at(target_x, target_y, target_size));
}

/**
 * Draws a progress bar: a gray background with a green fill growing from the
 * left in proportion to bar_percent (0.0 to 1.0).
 */
void draw_progress(double bar_x, double bar_y, double bar_width, double bar_height, double bar_percent)
{
    draw_rectangle(COLOR_GRAY, bar_x, bar_y, bar_width, bar_height);

    double progress_width = bar_width * bar_percent;

    // Only draw the fill once it is wide enough to sit inside the border;
    // otherwise the inset would make the width negative.
    if (progress_width > BAR_BORDER * 2)
    {
        fill_rectangle(COLOR_GREEN,
                       bar_x + BAR_BORDER, bar_y + BAR_BORDER,
                       progress_width - BAR_BORDER * 2, bar_height - BAR_BORDER * 2);
    }
}

int main()
{
    int target_number = read_integer("How many targets would you like to hit? ", 1, 100);
    write_line("Click " + to_string(target_number) + " targets to end the game.");

    open_window("Click Game - By Khubaib Ejaz", WINDOW_WIDTH, WINDOW_HEIGHT);

    int hits = 0;
    double percent = 0.0;
    double player_x = rnd(TARGET_RADIUS, WINDOW_WIDTH - TARGET_RADIUS);
    double player_y = rnd(TARGET_RADIUS, WINDOW_HEIGHT - TARGET_RADIUS);

    while (!quit_requested() && hits < target_number)
    {
        process_events();

        clear_screen(COLOR_WHITE);
        fill_circle(COLOR_RED, player_x, player_y, TARGET_RADIUS);
        draw_progress(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, percent);

        if (target_hit(player_x, player_y, TARGET_RADIUS))
        {
            hits++;

            player_x = rnd(TARGET_RADIUS, WINDOW_WIDTH - TARGET_RADIUS);
            player_y = rnd(TARGET_RADIUS, WINDOW_HEIGHT - TARGET_RADIUS);

            percent = (double)hits / target_number;
        }

        refresh_screen(60);
    }

    print_line(50);
    write_line("Congratulations!! You hit all the targets.");

    return 0;
}