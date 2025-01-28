#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// define a type for the cells of a sudoku
// a cell has a row number, a column number, and a value
typedef struct sudoku_cell{
	
	int row;
	int column;
	int val;

} Cell;

// define a type for the sudoku grid
// a sudoku grid is a list of pointers to cells
// 	pointers because we would like to modify these cells
typedef struct sudoku_grid{
	
	Cell **cells;
	int size;
	int cap;

} Grid;

// helper function to print the sudoku grid
// returns 0 indicating success
int print_grid(Grid g)
{
        // find how many rows/columns there are
        int dimensions = (int)sqrt(g.size);

        for (int i=0; i<g.size; i++)
        {

                if ((i+1) % dimensions == 0)
                {
                        if (g.cells[i]->val == -1)
                        {
                                printf("_\n");
                        }
                        else
                        {
                                printf("%d\n", g.cells[i]->val);
                        }
                }
                else
                {
                        if (g.cells[i]->val == -1)
                        {
                                printf("_ ");
                        }
                        else
                        {
                                printf("%d ", g.cells[i]->val);
                        }
                }
        }

        return 0;
}


// function that initializes the sudoku grid
// initial values of the puzzle are entered, empty cells have value -1
// takes a grid pointer, returns int indicating success/failure
int init_grid(Grid *g){
	
	int i = 0;

	for (int r=0; r<9; r++)
	{
		for (int c=0; c<9; c++)
		{
			// ----------------------------------
			Cell *cur_cell = malloc(sizeof(Cell));
			// ----------------------------------
			// question: why did we allocated dynamic memory to cur_cell??	
			// cur_cell is a Cell pointer, so dereferencing it give a Cell. That's why we can say cur_cell->row, this initializes the row field of the cell that cur_cell points to. As for why we allocated dynamic memory, it's because each time the loop executes that instruction, a new memory space is allocated to the new cell. If we didn't allocate dynamic memory and wrote Cell cur_cell; instead, then every time the loop executes that instruction, the same memory address will be overwritten by the new values assigned to each field of the cell.
			cur_cell->row = r;
			cur_cell->column = c;

			if ((r==0 && c==7) || (r==2 && c==1) || 
					(r==5 && c==3) || (r==7 && c==5))
			{
				cur_cell->val = 2;
			}
			else if ((r==1 && c==8) || (r==8 && c==6))
			{
				cur_cell->val = 5;
			}
			else if ((r==3 && c==2) || (r==6 && c==4))
			{
				cur_cell->val = 0;
			}
			else
			{
				cur_cell->val = -1;
			}

			// resize cells array as needed
			if (g->size == g->cap)
			{
				g->cap *= 2;
				Cell **new_arr = realloc(g->cells, g->cap*sizeof(Cell*));
				g->cells = new_arr;
			}
			
			g->cells[i] = cur_cell;
			// free(cur_cell);
			// question: why shouldn't we free cur_cell????
			// because when we free cur_cell, IDK
			g->size++;
			i++;
		}
	}

	return 0; // 0 for success
}

// function to determine if assigning a given value to a cell is legal
// returns a boolean value (0 for safe, 1 for unsafe)
int is_legal(Grid *grid, Cell *cell){

	//check if num exists in the row
	//iterate over the columns of a row
	for (int i=0; i<9; i++)
	{
		if (grid->cells[(cell->row)*9 + i]->val == cell->val)
		{
			return 1;
		}
	}
	//check the column
	for (int i=0; i<9; i++)
	{
		if (grid->cells[(cell->column) + i*9]->val == cell->val)
		{
			return 1;
		}
	}

	//check the subgrid
	int start_row = cell->row - cell->row % 3;
	int start_column = cell->column - cell->column % 3;

	for (int r=0; r<3; r++)
	{
		for (int c=0; c<3; c++)
		{
			if (grid->cells[((start_row+r)*9)+(start_column+c)]->val == cell->val)
			{
				return 1;
			}
		}
	}


	return 0;
	
}

// a function that writes the given cell on the grid
// the index of a cell in the cells list is calculated as:
// 	row*9 + column
// returns int to signal success (it's always going to be successful anyways)
// takes in pointer to a grid (to modify grid)
void apply_cell(Grid *grid, Cell cell)
{
	// go to the location in the grid specified in the cell
	// assign the value of the given cell in the grid as the
	// one specified in the cell
	grid->cells[cell.row*9 + cell.column]->val = cell.val;
}

// function that solves the puzzle recursively
// returns 0 for valid solution, 1 for invalid solution
// PROBLEM WITH BACKTRACKING:
// 	- doesn't properly update the wrong value when future
// 	  steps don't work
// 	- in the current version of the code, when the last 
// 	  value (3) is applied on the grid, the funcion needs
// 	  to realize that it doesn't work because it needs to 
// 	  put 2 to either one of the last two spots but this is
// 	  not legal, so needs to backtract
// 	- the for loop exhausts all the values and non of them are
// 	  legal (but not sure because the program terminates from
// 	  inside the nested if statement in the for loop)
//
int solve_puzzle(Grid *grid, int cur_row, int cur_column)
{

	if (cur_row > 8)
	{
		return 0;
	}

	if (cur_column > 8)
	{
		cur_row++;
		cur_column = 0;
	}

	// check if the cell in the location (cur_row, cur_column)
	// is full
	// the index of the cell in cells list is 
	// 	row*9 + column
	if (grid->cells[cur_row*9 + cur_column]->val > -1)
	{
		return solve_puzzle(grid, cur_row, cur_column+1);
	}

	for (int i=0; i<9; i++)
	{
		// CHANGED DYNAMIC ALLOCATION OF CELL
		Cell cell;
		cell.row = cur_row;
		cell.column = cur_column;
		cell.val = i;

		if (is_legal(grid, &cell) == 0)
		{
			apply_cell(grid, cell);

			//printf("\n-------------\n");
			//int dumb= print_grid(grid);

			if (solve_puzzle(grid, cur_row, cur_column + 1) == 0)
			{
				return 0;
			}
		}
		
		grid->cells[cur_row*9 + cur_column]->val = -1;

		//free(cell);

	}

	return 0;

}

// now the real puzzle begins lol ... but i should give some break
// BEFORE CONTINUING, answer the question in lines 40 and 73!!!!

int main(int argc, char *argv[]){
	
	// initialize the grid
	Grid sudo_grid;
	sudo_grid.cells = malloc(5*sizeof(Cell*));
	sudo_grid.size = 0;
	sudo_grid.cap = 5;
	int s1 = init_grid(&sudo_grid);

	if (s1 != 0)
	{
		printf("There was a problem while initializing the grid.\n");
		return 1;
	}

	int s2 = print_grid(sudo_grid);

	if (s2 != 0)
	{
		printf("There was a problem while printing the grid.\n");
		return 1;
	}

	int sol = solve_puzzle(&sudo_grid, 0, 0);

	if (sol == 0)
	{
		printf("\n\n");
		int s3 = print_grid(sudo_grid);
	}
	else
	{
		printf("There's no solution!");
	}

	free(sudo_grid.cells);
	return 0;
}
