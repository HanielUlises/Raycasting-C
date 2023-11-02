#include "utils.h"

int main(int argc, char** argv) {
	int gameRunning = initializeWindow();

	while (gameRunning) {
		processInput(&gameRunning);
		update();
		render();
	}

	destroyWindow();

	return 0;
}