#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 20
#define COLS (WINDOW_WIDTH / CELL_SIZE)
#define ROWS (WINDOW_HEIGHT / CELL_SIZE)

typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER } GameState;
typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

typedef struct {
    int x, y;
} Cell;

typedef struct {
    Cell *body;
    int length;
    Direction dir;
} Snake;

Cell food;

void snake_init(Snake *snake) {
    snake->length = 5;
    snake->body = malloc(sizeof(Cell) * COLS * ROWS);
    int startX = COLS / 2;
    int startY = ROWS / 2;
    for (int i = 0; i < snake->length; i++) {
        snake->body[i].x = startX - i;
        snake->body[i].y = startY;
    }
    snake->dir = DIR_RIGHT;
}

void spawn_food(Snake *snake) {
    bool onSnake;
    do {
        onSnake = false;
        food.x = rand() % COLS;
        food.y = rand() % ROWS;
        for (int i = 0; i < snake->length; i++) {
            if (snake->body[i].x == food.x && snake->body[i].y == food.y) {
                onSnake = true; break;
            }
        }
    } while (onSnake);
}

bool snake_update(Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    Cell *head = &snake->body[0];
    switch (snake->dir) {
        case DIR_UP: head->y--; break;
        case DIR_DOWN: head->y++; break;
        case DIR_LEFT: head->x--; break;
        case DIR_RIGHT: head->x++; break;
    }
    if (head->x < 0 || head->x >= COLS || head->y < 0 || head->y >= ROWS) return false;
    for (int i = 1; i < snake->length; i++) {
        if (head->x == snake->body[i].x && head->y == snake->body[i].y) return false;
    }
    if (head->x == food.x && head->y == food.y) {
        snake->length++;
        snake->body[snake->length - 1] = snake->body[snake->length - 2];
        spawn_food(snake);
    }
    return true;
}

SDL_Texture* render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    srand((unsigned)time(NULL));

    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);

    GameState state = STATE_MENU;
    Snake snake;
    bool running = true;
    SDL_Event e;
    Uint32 lastMove = SDL_GetTicks();
    const Uint32 moveDelay = 100;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                if (state == STATE_MENU) {
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        snake_init(&snake);
                        spawn_food(&snake);
                        state = STATE_PLAYING;
                    } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                } else if (state == STATE_PLAYING) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP: if (snake.dir != DIR_DOWN) snake.dir = DIR_UP; break;
                        case SDLK_DOWN: if (snake.dir != DIR_UP) snake.dir = DIR_DOWN; break;
                        case SDLK_LEFT: if (snake.dir != DIR_RIGHT) snake.dir = DIR_LEFT; break;
                        case SDLK_RIGHT: if (snake.dir != DIR_LEFT) snake.dir = DIR_RIGHT; break;
                    }
                } else if (state == STATE_GAMEOVER) {
                    if (e.key.keysym.sym == SDLK_RETURN) state = STATE_MENU;
                    else if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if (state == STATE_PLAYING && now - lastMove > moveDelay) {
            if (!snake_update(&snake)) {
                state = STATE_GAMEOVER;
            }
            lastMove = now;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state == STATE_MENU) {
            SDL_Color white = {255,255,255,255};
            SDL_Texture *title = render_text(renderer, font, "SNake Game", white);
            SDL_Texture *info = render_text(renderer, font, "Press Enter to Start, Esc to Quit", white);
            int tw, th;
            SDL_QueryTexture(title, NULL, NULL, &tw, &th);
            SDL_Rect tr = { (WINDOW_WIDTH-tw)/2, WINDOW_HEIGHT/3, tw, th };
            SDL_RenderCopy(renderer, title, NULL, &tr);
            SDL_QueryTexture(info, NULL, NULL, &tw, &th);
            SDL_Rect ir = { (WINDOW_WIDTH-tw)/2, WINDOW_HEIGHT/2, tw, th };
            SDL_RenderCopy(renderer, info, NULL, &ir);
            SDL_DestroyTexture(title);
            SDL_DestroyTexture(info);
        } else if (state == STATE_PLAYING) {
            int centerX = food.x * CELL_SIZE + CELL_SIZE / 2;
            int centerY = food.y * CELL_SIZE + CELL_SIZE / 2;
            int radius = CELL_SIZE / 2 - 1;
            filledCircleRGBA(renderer, centerX, centerY, radius, 255, 0, 0, 255);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for (int i = 0; i < snake.length; i++) {
                SDL_Rect r = { snake.body[i].x * CELL_SIZE, snake.body[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &r);
            }
        } else if (state == STATE_GAMEOVER) {
            SDL_Color red = {255,0,0,255};
            SDL_Texture *go = render_text(renderer, font, "Game Over! Press Enter to Menu or Esc to Quit", red);
            int tw, th;
            SDL_QueryTexture(go, NULL, NULL, &tw, &th);
            SDL_Rect gr = { (WINDOW_WIDTH-tw)/2, WINDOW_HEIGHT/2, tw, th };
            SDL_RenderCopy(renderer, go, NULL, &gr);
            SDL_DestroyTexture(go);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (state == STATE_PLAYING) free(snake.body);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}