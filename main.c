#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


// Rozdzielczość zafiksowana, nie zmieniać
#define RES_X 1920
#define RES_Y 1080

// Rozdzielczość siatki dobrana jest optymalnie, lepiej nie zmieniać
#define CELL_RES_X 128
#define CELL_RES_Y 72

#define GRID_OFFSET_X (RES_X / CELL_RES_X)
#define GRID_OFFSET_Y (RES_Y / CELL_RES_Y)
#define GRID_THIC 2


typedef struct {
    unsigned char blue, green, red;
} Pixel;

static const unsigned char HEADER[] = {
    0x42, 0x4D, 0x36, 0xEC, 0x5E, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
    0x80, 0x07, 0x00, 0x00, 0x38, 0x04, 0x00, 0x00, 0x01,
    0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEC,
    0x5E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static Pixel FRAME_BUFFER[RES_Y][RES_X];
static char CELLS_A[CELL_RES_Y][CELL_RES_X];
static char CELLS_B[CELL_RES_Y][CELL_RES_X];
static char SWAP_POINTER = 0;
static char FILE_PATH[256];

void initialize_board(double fraction) {
    double chance;
    srand(time(NULL));

    for (int y = 1; y < CELL_RES_Y - 1; y++) {
        for (int x = 1; x < CELL_RES_X - 1; x++) {
            chance = (double) rand() / (double) RAND_MAX;

            if (chance < fraction) {
                CELLS_A[y][x] = 1;
            } else {
                CELLS_A[y][x] = 0;
            }
        }
    }
}

int save_frame(char* path, int frame_number) {
    sprintf(FILE_PATH, "%s/frame_%d.bmp", path, frame_number);

    FILE *output = fopen(FILE_PATH, "wb");

    if (output == NULL) {
        return -1;
    }

    fwrite(HEADER, sizeof(unsigned char), sizeof(HEADER), output);
    fwrite(FRAME_BUFFER, sizeof(Pixel), RES_X * RES_Y, output);

    fclose(output);

    return 0;
}

void draw_cells() {
    char (*cells_buffer)[][CELL_RES_X];

    if (SWAP_POINTER == 0) {
        cells_buffer = &CELLS_A;
    } else {
        cells_buffer = &CELLS_B;
    }

    int index_x, index_y;

    for (int y = 0; y < RES_Y; y++) {
        for (int x = 0; x < RES_X; x++) {
            index_x = x / GRID_OFFSET_X;
            index_y = y / GRID_OFFSET_Y;

            if ((*cells_buffer)[index_y][index_x] == 1) {
                FRAME_BUFFER[y][x] = (Pixel) { .red = 128, .green = 255, .blue = 0 };
            } else {
                FRAME_BUFFER[y][x] = (Pixel) { .red = 64, .green = 64, .blue = 64 };
            }
        }
    }
}

void draw_grid() {
    for (int y = 0; y < RES_Y; y++) {
        for (int x = 0; x < RES_X; x++) {
            if ((x % GRID_OFFSET_X < GRID_THIC) || (x % GRID_OFFSET_X > GRID_OFFSET_X - GRID_THIC - 1) 
                || (y % GRID_OFFSET_Y < GRID_THIC) || (y % GRID_OFFSET_Y > GRID_OFFSET_Y - GRID_THIC - 1)) {
                FRAME_BUFFER[y][x] = (Pixel) { .red = 0, .green = 0, .blue = 0 };
            }
        }
    }
}

void simulate_next_steep() {
    char (*next_steep)[][CELL_RES_X];
    char (*prev_steep)[][CELL_RES_X];

    if (SWAP_POINTER == 0) {
        next_steep = &CELLS_B;
        prev_steep = &CELLS_A;

        SWAP_POINTER = 1;
    } else {
        next_steep = &CELLS_A;
        prev_steep = &CELLS_B;

        SWAP_POINTER = 0;
    }

    int neighbors;

    for (int y = 1; y < CELL_RES_Y - 1; y++) {
        for (int x = 1; x < CELL_RES_X - 1; x++) {
            neighbors = 0;

            neighbors += (*prev_steep)[y + 1][x - 1];
            neighbors += (*prev_steep)[y + 1][x];
            neighbors += (*prev_steep)[y + 1][x + 1];

            neighbors += (*prev_steep)[y][x - 1];
            neighbors += (*prev_steep)[y][x + 1];

            neighbors += (*prev_steep)[y - 1][x - 1];
            neighbors += (*prev_steep)[y - 1][x];
            neighbors += (*prev_steep)[y - 1][x + 1];

            if (((*prev_steep)[y][x] == 0) && (neighbors == 3)) {
                (*next_steep)[y][x] = 1;
            } else if ((*prev_steep)[y][x] == 1) {
                if ((neighbors == 2) || (neighbors == 3)) {
                    (*next_steep)[y][x] = 1;
                } else {
                    (*next_steep)[y][x] = 0;
                }
            } else {
                (*next_steep)[y][x] = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int frames_number;
    double fraction_alive;

    if (argc == 4) {
        frames_number = atoi(argv[2]);
        fraction_alive = atof(argv[3]);

        printf(
            "Uruchomiono z parametrami:\n\t- nazwa katalogu: %s\n\t- liczba klatek: %d\n\t- frakcja komorek zywych: %f\n\n",
            argv[1], frames_number, fraction_alive
        );
    } else {
        printf("Nie podano potrzebnych argumentow: {nazwa katalogu} {liczba klatek} {frakcja komorek zywych}\n");
        return -1;
    }

    initialize_board(fraction_alive);

    for (int frame = 1; frame < frames_number + 1; frame++) {
        simulate_next_steep();
        draw_cells();
        draw_grid();

        if (save_frame(argv[1], frame) != 0) {
            printf("Problem z zapisem pliku! Upewnij sie, ze katalog: '%s' istnieje.\n", argv[1]);
            return -1;
        }

        printf("Progres: %d / %d\r", frame, frames_number);
        fflush(stdout);
    }

    printf("Wygenerowano %d klatek.\n", frames_number);

    return 0;
}
