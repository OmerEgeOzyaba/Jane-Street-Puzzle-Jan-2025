puzzle_main : puzzle_main.c dummy_puzz.c
	gcc puzzle_main.c -o puzzle_main -lm
	gcc dummy_puzz.c -o dummy -lm
