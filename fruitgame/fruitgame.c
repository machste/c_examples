#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>


#define FRUITS_PER_TREE 10
#define RAVEN_COUNT 9
#define FRUITS_PER_BASKET 2
#define GAMES 1000000

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


typedef enum {
	DICE_APPEL = 0,
	DICE_PEAR,
	DICE_PLUM,
	DICE_CHERRY,
	DICE_RAVEN,
	DICE_BASKET,
	DICE_N_SIDES
} DiceResult;


const char *dice_result_strs[] = {
	[DICE_APPEL] = "Apple",
	[DICE_PEAR] = "Pear",
	[DICE_PLUM] = "Plum",
	[DICE_CHERRY] = "Cherry",
	[DICE_RAVEN] = "Raven",
	[DICE_BASKET] = "Basket"
};
int fruit_trees[] = {
	[DICE_APPEL] = FRUITS_PER_TREE,
	[DICE_PEAR] = FRUITS_PER_TREE,
	[DICE_PLUM] = FRUITS_PER_TREE,
	[DICE_CHERRY] = FRUITS_PER_TREE
};
int players_wins = 0;
int raven_wins = 0;
float rolls_avg;

const char *dice_result_to_str(DiceResult dice_result)
{
	assert(dice_result < DICE_N_SIDES && dice_result >= 0);
	return dice_result_strs[dice_result];
}

DiceResult dice_roll(void)
{
	return rand() % DICE_N_SIDES;
}

void pick_fruit()
{
	int max_tree_idx = 0;
	for (int i = 1; i < ARRAY_SIZE(fruit_trees); i++) {
		if (fruit_trees[max_tree_idx] < fruit_trees[i]) {
			max_tree_idx = i;
		}
	}
	if (fruit_trees[max_tree_idx] > 0) {
		fruit_trees[max_tree_idx]--;
	}
}

int main(int argc, char *argv[])
{
	time_t t;
	unsigned int seed;
	// Init random number generator
	seed = (unsigned int)clock() + time(&t);
	srand(seed);
	// Start main program
	printf(" Fruit Game Analyser\n---------------------\n\n");
	for (int g = 0; g < GAMES; g++) {
		// Reset Game variables
		bool game_finished = false;
		int rolls = 0;
		int raven_count = 0;
		int basket_count = 0;
		for (int i = 0; i < ARRAY_SIZE(fruit_trees); i++) {
			fruit_trees[i] = FRUITS_PER_TREE;
		}
		// Start game
		while(!game_finished) {
			DiceResult dice_result = dice_roll();
			switch(dice_result) {
			case DICE_APPEL:
			case DICE_PEAR:
			case DICE_PLUM:
			case DICE_CHERRY:
				if (fruit_trees[dice_result] > 0) {
					fruit_trees[dice_result]--;
				}
				break;
			case DICE_RAVEN:
				raven_count++;
				break;
			case DICE_BASKET:
				basket_count++;
				for (int i = 0; i < FRUITS_PER_BASKET; i++) {
					pick_fruit(fruit_trees);
				}
				break;
			default:
				assert(true);
			}
			// Check raven
			if (raven_count >= RAVEN_COUNT) {
				raven_wins++;
				game_finished = true;
			}
			// Check fruit trees
			int i;
			for (i = 0; i < ARRAY_SIZE(fruit_trees); i++) {
				if (fruit_trees[i] > 0) {
					break;
				}
			}
			if (i >= ARRAY_SIZE(fruit_trees)) {
				players_wins++;
				game_finished = true;
			}
			rolls++;
		}
		if (g == 0) {
			rolls_avg = rolls;
		} else {
			rolls_avg = (rolls_avg + rolls) / 2;
		}
	}
	printf("Players Wins: %u\n", players_wins);
	printf("Raven Wins: %u\n", raven_wins);
	printf("Total games: %u\n", GAMES);
	printf("Rolls avg. per game: %f\n", rolls_avg);

	return EXIT_SUCCESS;
}
