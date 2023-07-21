#include "Scenes.h"
#include <thread>

SceneGame::SceneGame() :
Font(TTF_OpenFont("Resources/TTF/Font.ttf", 48)),
TitleWindow(UI::CreateWindow("Window::", {0, 0, 6, 2}, 512/6)),
ContentWindow(UI::CreateWindow("Window::", {0, 512/3, 6, 6}, 512/6)),
RestartButton({512 - 48, 0, 48, 48}, "ButtonRound.png") {

	UI::LoadWindow("Window::", "Window.png", 100);

	SpriteManager::Load("ButtonWinner.png");
	SpriteManager::Load("ButtonSquarePressed.png");

	RestartButton.Layer = new AST::Rect({0, 0, 32, 32}, "IconRestart.png"); // It will be centerized by the SpriteManager::Render() so we can provide 0, 0 as its coordinates.

	for(int i = 0; i < 9; i++) {
		Tiles.push_back(new AST::Rect({0, 512/3, 512/4, 512/4}, "ButtonSquareNotPressed.png"));
		TilesData.push_back(-1);
	}
	UI::AlignRectInRects({0, 512/3, 6 * 512/6, 6 * 512/6}, Tiles, 3, 3);
	
	X.Set(Font, "X");
	O.Set(Font, "O");

	TTF_CloseFont(Font);
	Font = TTF_OpenFont("Resources/TTF/Font.ttf", 38);

	Info.Set(Font, "Turn: Player X");
	UI::CenterRectInRect(Info, {0, 0, 6 * 512/6, 2 * 512/6});
}

void SceneGame::Event(SDL_Event&) {
	if(AST::Clicked(&RestartButton)) AST::Loop = false;
	for(int i = 0; i < Tiles.size(); i++) if(AST::Clicked(Tiles[i]) && (Tiles[i]->ImageFile != "ButtonSquarePressed.png" && Tiles[i]->ImageFile != "ButtonWinner.png")) {
		Tiles[i]->ImageFile = "ButtonSquarePressed.png";
		Tiles[i]->Text = (PlayerXTurn ? &X : &O);
		TilesData[i] = (PlayerXTurn ? 0 : 1);
		PlayerXTurn = !PlayerXTurn;
		Info.Set(Font, "Turn:Player " + std::string(PlayerXTurn ? "X" : "O"));

		std::thread([this](){
			std::pair<int, std::vector<int>> Data = CheckWinner();
			if(Data.first == -1) return;
			Info.Set(Font, "Winner:Player " + std::string(Data.first == 0 ? "X" : "O"));
			for(int i = 0; i < Tiles.size(); i++) {
				SDL_Delay(1000/16);
				for(int j = 0; j < Data.second.size(); j++) if(i == Data.second[j]) Tiles[i]->ImageFile = "ButtonWinner.png";
			}
		}).detach();
	}
	if(AST::Events[SDL_QUIT]) {
		AST::Loop = false; 
		ReturnCode = -1;
	}
	AST::Keys[SDL_BUTTON_LEFT] = false;
}

void SceneGame::Loop() {
	for(auto& Part : TitleWindow) SpriteManager::Render(Part);
	for(auto& Part : ContentWindow) SpriteManager::Render(Part);
	for(auto& Tile : Tiles) SpriteManager::Render(Tile);
	SpriteManager::Render(&RestartButton);

	Info.Draw();
}

std::pair<int, std::vector<int>> SceneGame::CheckWinner() {
    // Define the winning combinations
    const std::vector<std::vector<int>> winningCombinations = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };

    // Check each winning combination
    for (const auto& combination : winningCombinations) {
        int firstTile = TilesData[combination[0]];
        if (firstTile != -1 && TilesData[combination[1]] == firstTile && TilesData[combination[2]] == firstTile) {
            return std::make_pair(firstTile, combination);  // Return the winning player and the tiles
        }
    }

    return std::make_pair(-1, std::vector<int>());  // No winner
}

SceneGame::~SceneGame() {}