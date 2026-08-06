#ifndef SOLVER_H
#define SOLVER_H

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;
typedef enum Action {LEFT, FORWARD, RIGHT, IDLE} Action;

//Queue variables
typedef struct {
  int x, y;
} Point;

//Maze Variables
typedef struct {
  int dist;
  int walls[4];  // indexed by Heading: NORTH=0, EAST=1, SOUTH=2, WEST=3
  int visited;
} Cell;


void queue_clear();
int queue_empty();
void enqueue(int x, int y);
Point dequeue();


void init_maze();
void init_mouse();
void flood_maze();
void display_distances();
void set_wall(Heading h);
int sense_walls();


Heading best_neighbor();
Action turn_toward(Heading h);


Action solver();
Action leftWallFollower();
Action floodFill();

#endif