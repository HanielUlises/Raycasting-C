#include "utils.h"

int main(int argc, char** argv) {
	int gameRunning = initializeWindow();
	
	playerConstructor();
	printf("Game running\n");
	while (gameRunning) {
		handleEvents(&gameRunning);
		update();
		render();
	}

	destroyWindow();

	return 0;
}