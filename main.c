#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define STEP_HARDCORE 15

char  *read_line()
{
  ssize_t  ret;
  char     *buff;

  if ((buff = malloc(sizeof(*buff) * (50 + 1))) == NULL)
    return (NULL);
  if ((ret = read(0, buff, 50)) > 1)
  {
    buff[ret - 1] = '\0';
    return (buff);
  }
  buff[0] = '\0';
  return (buff);
}

typedef struct {
  int score;
  char map[5][5];
  int y, x;
} game_t;

int update(game_t *game, int y, int x)
{
  int newy = game->y + y;
  int newx = game->x + x;
  if (newy < 0 || newx < 0 || newy > 4 || newx > 4)
    return 0;
  game->map[game->y][game->x] = ' ';
  game->map[game->y = newy][game->x = newx] = '*';
  return 1;
}

void show(game_t *game)
{
  printf("-----\n");
  for (int y = 0; y < 5; ++y)
  {
    for (int x = 0; x < 5; ++x)
      printf("%c", game->map[y][x]);
    printf("\n");
  }
  printf("-----\n");
}

int act(game_t* game)
{
  printf("-> ");
  fflush(stdout);
  char *input = read_line();
  if (!strcmp(input, "up"))
    return update(game, -1, 0);
  else if (!strcmp(input, "down"))
    return update(game, 1, 0);
  else if (!strcmp(input, "still"))
    return update(game, 0, 0);
  return act(game);
}

void spawn_monster(game_t *game)
{
  int y = rand() % 5;
  int x = 3 + rand() % 2;
  game->map[y][x] = '#';
}

void play_monster(game_t *game)
{
  for (int y = 0; y < 5; ++y)
    for (int x = 0; x < 5; ++x)
      if (game->map[y][x] == '#')
      {
        game->map[y][x] = ' ';
        if (x)
          game->map[y][x - 1] = '#';
      }
  spawn_monster(game);
  if (game->score > STEP_HARDCORE)
    spawn_monster(game);
}

void start(game_t *game)
{
  while (1)
  {
    show(game);
    if (act(game))
    {
      game->score += 1;
      play_monster(game);
      if (game->map[game->y][game->x] == '#')
      {
        printf("You lost! You ended with %d points.\n", game->score);
        return;
      }
    }
    else
      printf("Wrong move!\n");
  }
}

void fill_empty(game_t *game)
{
  for (int y = 0; y < 5; ++y)
    for (int x = 0; x < 5; ++x)
      game->map[y][x] = ' ';
}

int main(int argc, char **argv)
{
  if (argc != 3 || strcmp(argv[1], "-n"))
  {
    printf("usage: %s -n [login]", argv[0]);
    return (1);
  }
  srand(time(NULL));
  printf("Bienvenue dans helicobyte, %s.\n\n", argv[2]);
  game_t game = {.score = 0, .map = {}, .y = 2, .x = 0};
  fill_empty(&game);
  game.map[game.y][game.x] = '*';
  start(&game);
  return (0);
}
