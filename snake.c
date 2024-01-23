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

WINDOW *win;

int height = 20;
int width = 41;
int speed = 70000;

bool gameOver = false;
bool menuScr = true;

// Settings options
char *settingsOptions[4] = {"Size", "Speed", "Wall wrap", "Back"};
char *settingsSize[3] = {"Small", "Medium", "Large"};
char *settingsSpeed[3] = {"Slow", "Medium", "Fast"};
char *settingsWallWrap[2] = {"Off", "On"};
int sizeSelected = 1;
int speedSelected = 1;
int wallWrap = 0;

void menu(WINDOW *win) {
  width = 41;
  height = 20;

  bool settingsScr = false;
  int menuInput;
  int menuSelected = 0;
  while (menuScr) {
    werase(win);

    // Draw board
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    // Title
    if (has_colors())
      wattron(win, COLOR_PAIR(SNAKE_COLOR));
    mvwaddstr(win, 2, width / 2 - 14, " ____  __ _   __   __ _  ____");
    mvwaddstr(win, 3, width / 2 - 14, "/ ___)(  ( \\ / _\\ (  / )(  __)");
    mvwaddstr(win, 4, width / 2 - 14, "\\___ \\/    //    \\ )  (  ) _) ");
    mvwaddstr(win, 5, width / 2 - 14, "(____/\\_)__)\\_/\\_/(__\\_)(____)");
    if (has_colors())
      wattroff(win, COLOR_PAIR(SNAKE_COLOR));

    // Menu options
    char *menuOptions[3] = {"Start", "Options", "Exit"};

    for (int i = 0; i < 3; i++) {
      if (i == menuSelected) {
        wattron(win, A_REVERSE);
      }

      mvwaddstr(win, height / 2 + i + 5, width / 2 - 2, menuOptions[i]);

      if (i == menuSelected) {
        wattroff(win, A_REVERSE);
      }
    }

    // Menu input
    menuInput = wgetch(win);

    if (menuInput == KEY_DOWN || menuInput == 's' || menuInput == 'S') {
      menuSelected++;
      if (menuSelected > 2) {
        menuSelected = 0;
      }
    }

    if (menuInput == KEY_UP || menuInput == 'w' || menuInput == 'W') {
      menuSelected--;
      if (menuSelected < 0) {
        menuSelected = 2;
      }
    }

    if (menuInput == '\n') {
      // Start
      if (menuSelected == 0) {
        menuScr = false;
        gameOver = false;

        break;
      }

      // Options
      if (menuSelected == 1) {
        settingsScr = true;
        menuSelected = 0;

        while (settingsScr) {
          werase(win);

          // Draw board
          wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

          // Title
          if (has_colors())
            wattron(win, COLOR_PAIR(SNAKE_COLOR));
          mvwaddstr(win, 2, width / 2 - 14, " ____  __ _   __   __ _  ____");
          mvwaddstr(win, 3, width / 2 - 14, "/ ___)(  ( \\ / _\\ (  / )(  __)");
          mvwaddstr(win, 4, width / 2 - 14,
                    "\\___ \\/    //    \\ )  (  ) _) ");
          mvwaddstr(win, 5, width / 2 - 14,
                    "(____/\\_)__)\\_/\\_/(__\\_)(____)");
          if (has_colors())
            wattroff(win, COLOR_PAIR(SNAKE_COLOR));

          for (int i = 0; i < 4; i++) {
            if (i == menuSelected) {
              wattron(win, A_REVERSE);
            }

            mvwaddstr(win, height / 2 + i + 5, width / 2 - 9,
                      settingsOptions[i]);

            if (i == menuSelected) {
              wattroff(win, A_REVERSE);
            }
          }

          // Reverse selected value
          if (menuSelected == 0) {
            wattron(win, A_REVERSE);
          }

          mvwaddstr(win, height / 2 + 5, width / 2 + 7,
                    settingsSize[sizeSelected]);

          if (menuSelected == 0) {
            wattroff(win, A_REVERSE);
          }

          if (menuSelected == 1) {
            wattron(win, A_REVERSE);
          }
          mvwaddstr(win, height / 2 + 6, width / 2 + 7,
                    settingsSpeed[speedSelected]);

          if (menuSelected == 1) {
            wattroff(win, A_REVERSE);
          }

          if (menuSelected == 2) {
            wattron(win, A_REVERSE);
          }
          mvwaddstr(win, height / 2 + 7, width / 2 + 7,
                    settingsWallWrap[wallWrap]);

          if (menuSelected == 2) {
            wattroff(win, A_REVERSE);
          }

          // Settings input
          menuInput = wgetch(win);

          if (menuInput == KEY_DOWN || menuInput == 's' || menuInput == 'S') {
            menuSelected++;
            if (menuSelected > 3) {
              menuSelected = 0;
            }
          }

          if (menuInput == KEY_UP || menuInput == 'w' || menuInput == 'W') {
            menuSelected--;
            if (menuSelected < 0) {
              menuSelected = 3;
            }
          }

          if ((menuInput == KEY_RIGHT || menuInput == '\n' ||
               menuInput == 'd') &&
              menuSelected == 0) {
            sizeSelected++;
            if (sizeSelected > 2) {
              sizeSelected = 0;
            }
          }

          if ((menuInput == KEY_RIGHT || menuInput == '\n' ||
               menuInput == 'd') &&
              menuSelected == 1) {
            speedSelected++;
            if (speedSelected > 2) {
              speedSelected = 0;
            }
          }

          if ((menuInput == KEY_RIGHT || menuInput == '\n' ||
               menuInput == 'd') &&
              menuSelected == 2) {
            wallWrap++;
            if (wallWrap > 1) {
              wallWrap = 0;
            }
          }

          if ((menuInput == KEY_LEFT || menuInput == 'a') &&
              menuSelected == 0) {
            sizeSelected--;
            if (sizeSelected < 0) {
              sizeSelected = 2;
            }
          }

          if ((menuInput == KEY_LEFT || menuInput == 'a') &&
              menuSelected == 1) {
            speedSelected--;
            if (speedSelected < 0) {
              speedSelected = 2;
            }
          }

          if ((menuInput == KEY_LEFT || menuInput == 'a') &&
              menuSelected == 2) {
            wallWrap--;
            if (wallWrap < 0) {
              wallWrap = 1;
            }
          }

          if (menuInput == '\n' && menuSelected == 3) {
            // Back
            settingsScr = false;
            menuSelected = 0;
            continue;
          }

          if (menuSelected == 0 && menuInput == KEY_RIGHT) {
            continue;
          }
        }
      }

      // Exit
      if (menuSelected == 2) {
        endwin();
        exit(0);
      }
    }

    wrefresh(win);
  }
}

void resizeWin(WINDOW **win, int height, int width, int starty, int startx) {
  werase(*win);
  wrefresh(*win);
  *win = newwin(height + 4, width + 2, starty, startx);

  keypad(*win, true);
  nodelay(*win, true);
  noecho();
  cbreak();
  curs_set(0);
}

WINDOW *pauseWin(int starty, int startx) {
  WINDOW *pausew = newwin(5, 21, starty, startx);

  keypad(pausew, true);
  nodelay(pausew, true);
  noecho();
  cbreak();
  curs_set(0);
  wborder(pausew, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwaddstr(pausew, 1, 8, "Paused");
  mvwaddstr(pausew, 3, 2, "Press P to resume");

  wrefresh(pausew);

  return pausew;
}

WINDOW *gameOverWin(int width, int height, int starty, int startx) {
  WINDOW *gameOverw = newwin(height, width, starty, startx);

  keypad(gameOverw, true);
  nodelay(gameOverw, true);
  noecho();
  cbreak();
  curs_set(0);

  return gameOverw;
}

int kbhit(void) {
  int ch = getch();

  if (ch != ERR) {
    ungetch(ch);
    return 1;
  } else {
    return 0;
  }
}

void game(WINDOW *win) {
  if (sizeSelected == 0) {
    height = 10;
    width = 21;
    resizeWin(&win, height, width, (LINES - height) / 2, (COLS - width) / 2);
  } else if (sizeSelected == 1) {
    height = 15;
    width = 31;
    resizeWin(&win, height, width, (LINES - height) / 2, (COLS - width) / 2);
  } else if (sizeSelected == 2) {
    height = 20;
    width = 41;
    resizeWin(&win, height, width, (LINES - height) / 2, (COLS - width) / 2);
  }

  if (speedSelected == 0) {
    speed = 170000;
  } else if (speedSelected == 1) {
    speed = 120000;
  } else if (speedSelected == 2) {
    speed = 80000;
  }

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

  // Timer
  int seconds;
  int minutes;
  int hours;

  while (!gameOver) {

    input = wgetch(win);

    if (input == 'q') {
      menuScr = true;
      gameOver = true;
    }

    // Pause game
    if (input == 'p') {
      WINDOW *pausew = pauseWin((LINES - 3) / 2, (COLS - 19) / 2);
      while (true) {
        input = wgetch(pausew);
        if (input == 'p' || input == 'q') {
          break;
        }
      }
      delwin(pausew);
    }

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
        WINDOW *gameOverw = gameOverWin(1, 1, snake[0].y + (LINES - height) / 2,
                                        snake[0].x + (COLS - width) / 2);
        while (true) {
          input = wgetch(gameOverw);

          if (has_colors())
            wattron(gameOverw, COLOR_PAIR(FOOD_COLOR));

          mvwaddstr(gameOverw, 0, 0, "X");

          if (has_colors())
            wattroff(gameOverw, COLOR_PAIR(FOOD_COLOR));

          if (input == 'q' || input == '\n') {
            gameOver = true;
            menuScr = true;
            break;
          }
        }
        delwin(gameOverw);
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
    if (wallWrap) {
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
    } else {
      // Game over check
      if (snake[0].x == width + 1 || snake[0].x == 0 ||
          snake[0].y == height + 1 || snake[0].y == 0) {
        WINDOW *gameOverw = gameOverWin(1, 1, snake[0].y + (LINES - height) / 2,
                                        snake[0].x + (COLS - width) / 2);
        while (true) {
          input = wgetch(gameOverw);

          if (has_colors())
            wattron(gameOverw, COLOR_PAIR(FOOD_COLOR));

          mvwaddstr(gameOverw, 0, 0, "X");

          if (has_colors())
            wattroff(gameOverw, COLOR_PAIR(FOOD_COLOR));

          if (input == 'q' || input == '\n') {
            gameOver = true;
            menuScr = true;
            break;
          }
        }
        delwin(gameOverw);
      }
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

    if (has_colors())
      wattroff(win, COLOR_PAIR(FOOD_COLOR));

    // Draw Snake
    if (has_colors())
      wattron(win, COLOR_PAIR(SNAKE_COLOR));

    for (int i = 0; i < width * height; i++) {
      mvwaddwstr(win, snake[i].y, snake[i].x, L"\u2588");
    }

    if (has_colors())
      wattroff(win, COLOR_PAIR(SNAKE_COLOR));

    // Food collection
    if (snake[0].x == food.x && snake[0].y == food.y) {
      points++;

      // Spawn new food (not on borders and not on snake)
      bool foodSpawned = false;
      while (!foodSpawned) {
        food.x = rand() % (width - 1) + 1;
        food.y = rand() % (height - 1) + 1;

        for (int i = 0; i < width * height; i++) {
          if (food.x == snake[i].x && food.y == snake[i].y) {
            foodSpawned = false;
            break;
          } else {
            foodSpawned = true;
          }
        }
      }
    }

    if (gameOver) {
      break;
    }
    wrefresh(win);
    usleep(speed);
    time++;
  }
}

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

  // Window position to middle of the screen
  int winX = (COLS - width) / 2;
  int winY = (LINES - height) / 2;

  win = newwin(height + 4, width + 2, winY, winX);

  keypad(win, true);
  nodelay(win, true);
  noecho();
  cbreak();
  curs_set(0);

  while (true) {
    // Menu screen
    if (menuScr)
      menu(win);

    // Game loop
    if (!gameOver)
      game(win);
  }

  endwin();
}
