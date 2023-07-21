#define SDL_MAIN_HANDLED
#define AST_IMPLEMENTATION

#include <AST.hpp>
#include "Scenes/Scenes.h"


int main(int argc, char const *argv[]) {
	AST::Init("Tic Tac Toe By Cube Nerd", {-1, -1, 512, 512 + 512/3});
 
	while(AST::Render(new SceneGame()) == 0) continue;

	AST::Quit();
	return 0;
}