typedef struct {
    char* squares;
    int size;
    int capacity;
} row;

void init_row(row* r){
    r->size = 0;
    r->capacity = INITIAL_CAPACITY;
    r->squares = malloc(r->capacity * sizeof(char));
    assert(r->squares != NULL);
}

void reallocate_row(row* r){
    r->capacity = r->capacity * GROWTH_FACTOR;
    r->squares = realloc(r->squares, r->capacity * sizeof(char));
    assert(r->squares != NULL);
}

void add_square(row* r, char square){
    if(r->size == r->capacity){
        reallocate_row(r);
    }
    r->squares[r->size] = square;
    r->size++;
}

void dispose_row(row* r){
    free(r->squares);
}

typedef struct {
    row** rows;
    int size;
    int capacity;
} board;

void init_board(board* b){
    b->size = 0;
    b->capacity = INITIAL_CAPACITY;
    b->rows = malloc(b->capacity * sizeof(row*));
    assert(b->rows != NULL);
}

void reallocate_board(board* b){
    b->capacity = b->capacity * GROWTH_FACTOR;
    b->rows = realloc(b->rows, b->capacity * sizeof(row*));
    assert(b->rows != NULL);
}

void add_row(board* b, row* r){
    if(b->size == b->capacity){
        reallocate_board(b);
    }
    b->rows[b->size] = r;
    b->size++;
}

void dispose_board(board* b){
    for(int i = 0; i < b->size; i++){
        dispose_row(b->rows[i]);
    }
    free(rows);
}
