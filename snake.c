#define _DEFAULT_SOURCE // to use usleep
#define _XOPEN_SOURCE 600

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NULL_POS -69
#define SNAKE_COLOR 1
#define FOOD_COLOR 2

typedef struct {
  int x;
  int y;
} pos;

int main() {
  setlocale(LC_ALL, "");
  initscr(); // Initialize curses mode

  // Colors
  if (has_colors()) {
    use_default_colors();
    start_color();
    init_pair(SNAKE_COLOR, COLOR_GREEN, -1);
    init_pair(FOOD_COLOR, COLOR_RED, -1);
  }

  // Window settings
  WINDOW *win;

  int height = 20;
  int width = 40;

  // Window position to middle of the screen
  int winX = (COLS - width) / 2;
  int winY = (LINES - height) / 2;

  win = newwin(height + 4, width + 2, winY, winX);

  keypad(win, true);
  nodelay(win, true);
  noecho();
  cbreak();
  curs_set(0);

  int time = 0;

  pos snake[width * height];
  snake[0].x = width / 2;
  snake[0].y = height / 2;

  for (int i = 1; i < width * height; i++) {
    snake[i].x = NULL_POS;
    snake[i].x = NULL_POS;
  }

  pos food;
  food.x = 10;
  food.y = 10;

  pos dir;
  dir.x = 1;
  dir.y = 0;

  int points = 0;
  int snakeLength;
  int input;
  bool gameOver = false;

  // Timer
  int seconds;
  int minutes;
  int hours;

  while (((input = wgetch(win)) != 'q') && !gameOver) {

    // Input checks
    if ((input == KEY_DOWN || input == 's' || input == 'S') && dir.y != -1) {
      dir.x = 0;
      dir.y = 1;
    }

    if ((input == KEY_UP || input == 'w' || input == 'W') && dir.y != 1) {
      dir.x = 0;
      dir.y = -1;
    }

    if ((input == KEY_RIGHT || input == 'd' || input == 'D') && dir.x != -1) {
      dir.x = 1;
      dir.y = 0;
    }

    if ((input == KEY_LEFT || input == 'a' || input == 'A') && dir.x != 1) {
      dir.x = -1;
      dir.y = 0;
    }

    // Snake movement
    snakeLength = points + 1;
    for (int i = snakeLength; i >= 0; i--) {
      // Game over check
      if (i != 0 && snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
        gameOver = true;
        break;
      }

      if (i == 0) {
        snake[0].x += dir.x;
        snake[0].y += dir.y;
      } else {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
      }
    }

    // Border looping
    if (snake[0].x == width + 1) {
      snake[0].x = 1;
    }

    if (snake[0].x == 0) {
      snake[0].x = width;
    }

    if (snake[0].y == height + 1) {
      snake[0].y = 1;
    }

    if (snake[0].y == 0) {
      snake[0].y = height;
    }

    werase(win);

    // Draw board
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwhline(win, height + 1, 1, 0, width);
    mvwaddch(win, height + 1, 0, ACS_LTEE);
    mvwaddch(win, height + 1, width + 1, ACS_RTEE);

    // Bottom board
    // Food points
    char pointsStr[height * width];
    sprintf(pointsStr, "%d", points);
    if (has_colors())
      wattron(win, COLOR_PAIR(FOOD_COLOR));

    mvwaddstr(win, height + 2, 2, "\u2B24");

    if (has_colors())
      wattroff(win, COLOR_PAIR(FOOD_COLOR));

    mvwaddstr(win, height + 2, 4, pointsStr);

    // Timer
    seconds = time / 10;
    minutes = seconds / 60;
    hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;

    char *timeStr = malloc(9 * sizeof(char));
    sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
    mvwaddstr(win, height + 2, width - 8, timeStr);

    // Draw food
    if (has_colors())

      wattron(win, COLOR_PAIR(FOOD_COLOR));
    mvwaddwstr(win, food.y, food.x, L"\u2B24");

    // Draw Snake
    if (has_colors())
      wattron(win, COLOR_PAIR(SNAKE_COLOR));

    for (int i = 0; i < width * height; i++) {
      mvwaddwstr(win, snake[i].y, snake[i].x, L"\u2588");
    }

    if (has_colors())
      wattroff(win, COLOR_PAIR(SNAKE_COLOR));

    if (has_colors())
      wattroff(win, COLOR_PAIR(FOOD_COLOR));

    // Food collection
    if (snake[0].x == food.x && snake[0].y == food.y) {
      points++;

      // Spawn new food (not on borders)
      food.x = 1 + rand() % (width - 1);
      food.y = 1 + rand() % (height - 1);
    }

    wrefresh(win);
    usleep(100000);
    time++;
  }

  endwin();
}
