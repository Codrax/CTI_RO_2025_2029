#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Type
typedef uint8_t t_bit;

// Util
uint8_t in_array(int* array, int count, int element) {
    for (int i=0; i<count; i++)
        if (array[i] == element)
            return 1;
    return 0;
}
void* do_alloc(void* array, const int nmem, const int size) {
    void* new = NULL;
    if (!array)
        new = malloc(nmem * size);
    else
        new = realloc(array, nmem*size);
    if (!new) {
        perror("allocation error");
        if (array)
            free(array);
        exit(0xFF);
    }
    return new;
}

// Prog
void print(t_bit* table, int table_y, int table_x) {
    for (int y=0; y<table_y; y++) {
        for (int x=0; x<table_x; x++)
            printf("%d ", table[table_x * y + x]);
        printf("\n");
    }
}
void print_array(int* array, int count) {
    for (int i=0; i<count; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void find_recurse(t_bit* table, int table_y, int table_x,
    int** room, int* room_count, int y, int x) {
    int this = y*table_x + x;

    // Wall
    if (table[this] == 1)
        return;

    // Already part of room
    if (in_array(*room, *room_count, this))
        return;

    // Add self
    (*room)[*room_count] = this;
    (*room_count)++;

    // Occupy self
    table[this] = 1;

    // Add neighbours
    //printf("  X:%d Y:%d\n", x, y);
    if (x > 0)
        find_recurse(table, table_y, table_x, room, room_count, y, x-1);
    if (x < table_x-1)
       find_recurse(table, table_y, table_x, room, room_count, y, x+1);
    if (y > 0)
        find_recurse(table, table_y, table_x, room, room_count, y-1, x);
    if (y < table_y-1)
        find_recurse(table, table_y, table_x, room, room_count, y+1, x);
}

//
int main() {
    char* path_in = "m.txt";
    FILE* file_in = fopen(path_in, "r");
    if (!file_in) {
        perror("Invalid input");
        exit(0x01);
    }

    // Table
    int table_size = 64, table_y=0, table_x=0;
    t_bit* table = do_alloc(NULL, table_size, sizeof(t_bit));

    // Reading
    int position=0;
    char *s, *tmp;
    size_t s_n;
    while ((int)(getline(&s, &s_n, file_in)) != -1) {
        tmp = s;
        int increment_x = table_x == 0;
        while (*tmp) {
            if (*tmp != ' ') {
                table[position] = *tmp == '1';
                if (increment_x)
                    table_x++;

                //
                tmp++;
                position++;

                //
                if (position+1 > table_size) {
                    table_size *= 2;
                    table = do_alloc(table, table_size, sizeof(t_bit) );
                }
            }
            tmp++;
        }

        //
        table_y++;
    }
    free(s); s = NULL;

    // Test
    //print(table, table_y, table_x);

    // Find tables
    int camera=1;
    int* room=NULL;
    int room_count=0;

    room = do_alloc(room, table_x*table_y, sizeof(int*));

    for (int y=0; y<table_y; y++)
        for (int x=0; x<table_x; x++) {
            // Verify it's not a WALL
            if (table[y*table_x + x] == 1)
                continue;

            // Prepare new table
            room_count = 0;

            // Start recurse
            find_recurse(table, table_y, table_x,
                &room,
                &room_count,
                y,
                x
            );

            printf("Camera %d: %d\n", camera, room_count);

            //
            camera++;
        }

    // Free
    free(room);
    free(table);
}
