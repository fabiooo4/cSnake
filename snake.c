#define _DEFAULT_SOURCE // to use usleep

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int x;
  int y;
} pos;

int main() {
  initscr(); // Initialize curses mode

  // Window settings
  WINDOW *win;

  int height = 20;
  int width = 40;

  // Window position to middle of the screen
  int winX = (COLS - width) / 2;
  int winY = (LINES - height) / 2;

  win = newwin(height + 4, width + 2, winY, winX);

  keypad(win, true);
  // nodelay(win, true);
  noecho();
  curs_set(0);

  pos snake;
  snake.x = width / 2;
  snake.y = height / 2;

  pos food;
  food.x = 10;
  food.y = 10;

  pos dir;
  dir.x = 1;
  dir.y = 0;

  int points = 0;
  int input;

  while ((input = wgetch(win)) != 'q') {

    // Input checks
    if (input == KEY_DOWN && dir.y != -1) {
      dir.x = 0;
      dir.y = 1;
    }

    if (input == KEY_UP && dir.y != 1) {
      dir.x = 0;
      dir.y = -1;
    }

    if (input == KEY_RIGHT && dir.x != -1) {
      dir.x = 1;
      dir.y = 0;
    }

    if (input == KEY_LEFT && dir.x != 1) {
      dir.x = -1;
      dir.y = 0;
    }

    // Snake movement
    snake.x += dir.x;
    snake.y += dir.y;

    // Border looping
    if (snake.x == width + 1) {
      snake.x = 1;
    }

    if (snake.x == 0) {
      snake.x = width;
    }

    if (snake.y == height + 1) {
      snake.y = 1;
    }

    if (snake.y == 0) {
      snake.y = height;
    }

    werase(win);

    // Draw board
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwhline(win, height + 1, 1, 0, width);
    mvwaddch(win, height + 1, 0, ACS_LTEE);
    mvwaddch(win, height + 1, width + 1, ACS_RTEE);

    // Bottom board
    char pointsStr[height * width];
    sprintf(pointsStr, "Points: %d", points);
    mvwaddstr(win, height + 2, 2, pointsStr);

    char x[height * width];
    sprintf(x, "X: %d", snake.x);
    mvwaddstr(win, height + 2, 11, x);

    char y[height * width];
    sprintf(y, "Y: %d", snake.y);
    mvwaddstr(win, height + 2, 17, y);

    // Draw Snake + Food
    mvwaddch(win, snake.y, snake.x, 'X');
    mvwaddch(win, food.y, food.x, '@');

    // Food collection
    if (snake.x == food.x && snake.y == food.y) {
      points++;
      // Spawn new food (not on borders)
      food.x = 1 + rand() % (width - 1);
      food.y = 1 + rand() % (height - 1);
    }

    usleep(100000);
  }

  endwin();
}
