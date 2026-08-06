#include "solver.h"
#include "walls.h"
#include "controller.h"

#define WIDTH  16
#define HEIGHT 16
#define QUEUE_SIZE 256

/* Horizontal walls: h_walls[x][y] is the wall on the south edge of cell (x,y).
   h_walls[x][0] = south boundary, h_walls[x][HEIGHT] = north boundary. */
static int h_walls[WIDTH][HEIGHT + 1];

/* Vertical walls: v_walls[x][y] is the wall on the west edge of cell (x,y).
   v_walls[0][y] = west boundary, v_walls[WIDTH][y] = east boundary. */
static int v_walls[WIDTH + 1][HEIGHT];

static int dist[WIDTH][HEIGHT];

static Point queue[QUEUE_SIZE];
static int q_head = 0;
static int q_tail = 0;

static int mx_pos, my_pos;
static Heading mouse_heading;


/* ---------- queue ---------- */

void queue_clear() { q_head = q_tail = 0; }
int  queue_empty() { return q_head == q_tail; }

void enqueue(int x, int y) {
    queue[q_tail].x = x;
    queue[q_tail].y = y;
    q_tail = (q_tail + 1) % QUEUE_SIZE;
}

Point dequeue() {
    Point p = queue[q_head];
    q_head = (q_head + 1) % QUEUE_SIZE;
    return p;
}


/* ---------- wall helpers ---------- */

static int wall_north(int x, int y) { return h_walls[x][y + 1]; }
static int wall_south(int x, int y) { return h_walls[x][y];     }
static int wall_east (int x, int y) { return v_walls[x + 1][y]; }
static int wall_west (int x, int y) { return v_walls[x][y];     }


/* ---------- maze init ---------- */

void init_maze() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            h_walls[x][y] = 0;
            v_walls[x][y] = 0;
            dist[x][y]    = 999;
        }
        h_walls[x][HEIGHT] = 0;
    }
    for (int y = 0; y < HEIGHT; y++) {
        v_walls[WIDTH][y] = 0;
    }

    /* boundary walls */
    for (int x = 0; x < WIDTH; x++) {
        h_walls[x][0]      = 1;   /* south boundary */
        h_walls[x][HEIGHT] = 1;   /* north boundary */
    }
    for (int y = 0; y < HEIGHT; y++) {
        v_walls[0][y]     = 1;    /* west boundary  */
        v_walls[WIDTH][y] = 1;    /* east boundary  */
    }

    /* goal cells (centre 2x2) start at distance 0 */
    // dist[4][4] = 0;
    dist[7][7] = 0;
    dist[7][8] = 0;
    dist[8][7] = 0;
    dist[8][8] = 0;
}

void init_mouse() {
    mx_pos = 0;
    my_pos = 0;
    mouse_heading = NORTH;
}


/* ---------- flood fill ---------- */

void flood_maze() {
    /* reset distances only, keep discovered walls */
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            dist[x][y] = 999;

    dist[7][7] = 0;
    dist[7][8] = 0;
    dist[8][7] = 0;
    dist[8][8] = 0;
    // dist[4][4] = 0;

    queue_clear();
    enqueue(7, 7);
    enqueue(7, 8);
    enqueue(8, 7);
    enqueue(8, 8);
    // enqueue(4, 4);

    while (!queue_empty()) {
        Point p = dequeue();
        int x = p.x, y = p.y;
        int d = dist[x][y];

        if (!wall_north(x, y) && dist[x][y + 1] > d + 1) { dist[x][y + 1] = d + 1; enqueue(x, y + 1); }
        if (!wall_south(x, y) && dist[x][y - 1] > d + 1) { dist[x][y - 1] = d + 1; enqueue(x, y - 1); }
        if (!wall_east (x, y) && dist[x + 1][y] > d + 1) { dist[x + 1][y] = d + 1; enqueue(x + 1, y); }
        if (!wall_west (x, y) && dist[x - 1][y] > d + 1) { dist[x - 1][y] = d + 1; enqueue(x - 1, y); }
    }
}


/* ---------- wall sensing ---------- */

void set_wall(Heading h) {
    int x = mx_pos, y = my_pos;
    switch (h) {
        case NORTH:
            h_walls[x][y + 1] = 1;
            break;
        case SOUTH:
            h_walls[x][y] = 1;
            break;
        case EAST:
            v_walls[x + 1][y] = 1;
            break;
        case WEST:
            v_walls[x][y] = 1;
            break;
    }
}

int sense_walls() {
    int wf = wallFront();
    int wl = wallLeft();
    int wr = wallRight();
    int found = 0;

    Heading front = mouse_heading;
    Heading left  = (Heading)((mouse_heading + 3) % 4);
    Heading right = (Heading)((mouse_heading + 1) % 4);

    if (wf) { found = 1; set_wall(front); }
    if (wl) { found = 1; set_wall(left);  }
    if (wr) { found = 1; set_wall(right); }

    return found;
}


/* ---------- navigation ---------- */

Heading best_neighbor() {
    int x = mx_pos, y = my_pos;
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    Heading min_dir = mouse_heading;
    int min_val = 999;

    for (int dir = NORTH; dir <= WEST; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;

        /* skip if wall blocks this direction */
        if (dir == NORTH && wall_north(x, y)) continue;
        if (dir == SOUTH && wall_south(x, y)) continue;
        if (dir == EAST  && wall_east (x, y)) continue;
        if (dir == WEST  && wall_west (x, y)) continue;

        if (dist[nx][ny] < min_val) {
            min_dir = (Heading)dir;
            min_val = dist[nx][ny];
        }
    }

    if (min_val == 999) return mouse_heading;  /* all neighbors unreachable — hold position */
    return min_dir;
}

Action turn_toward(Heading h) {
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    if (mouse_heading == h) {
        mx_pos += dx[h];
        my_pos += dy[h];
        return FORWARD;
    }
    if ((mouse_heading + 1) % 4 == h) {
        mouse_heading = h;
        return RIGHT;
    }
    if ((mouse_heading + 3) % 4 == h) {
        mouse_heading = (Heading)((mouse_heading + 3) % 4);
        return LEFT;
    }
    /* 180° — turn left, next call finishes the rotation */
    mouse_heading = (Heading)((mouse_heading + 3) % 4);
    return LEFT;
}


/* ---------- solvers ---------- */

Action solver() {
    return floodFill();
}

Action leftWallFollower() {
    if (wallFront()) {
        if (wallLeft()) return RIGHT;
        return LEFT;
    }
    return FORWARD;
}

Action floodFill() {
    static int initialized = 0;
    if (!initialized) {
        init_maze();
        init_mouse();
        flood_maze();
        initialized = 1;
    }

    if (dist[mx_pos][my_pos] == 0) return IDLE;

    //sense for walls around
    int new_walls = sense_walls();

    //if there are walls flood maze
    if(new_walls) {
        flood_maze();
    }

    //get the closest neighbor
    Heading best = best_neighbor();


    //turn towards closest neighbor(return that value)
    return turn_toward(best);
}
