#include "generator.h"
#include "graphic.h"

int main(int argc, char** argv) {
    int row = 20;
    int col = 20;

    if (argc == 3) {
        int arg1 = atoi(argv[1]);
        int arg2 = atoi(argv[2]);

        row = arg1 < 5 ? row : arg1;
        col = arg2 < 5 ? col : arg2;
    }

    spacesMap spaces = generateMap(row, col);
    buildView(row, col, &spaces);

    return 0;
}