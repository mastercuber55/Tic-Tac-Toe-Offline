#include <AST.hpp>

class SceneGame : public AST::Scene {

	std::vector<AST::Rect*> TitleWindow;
	std::vector<AST::Rect*> ContentWindow;
	std::vector<AST::Rect*> Tiles;
	std::vector<int> TilesData;
	
	TTF_Font * Font;
	AST::Text X;
	AST::Text O;
	AST::Text Info;

	AST::Rect RestartButton;

	bool PlayerXTurn = true;

	std::pair<int, std::vector<int>> CheckWinner();

	public:
		SceneGame();
		void Event(SDL_Event&) override;
		void Loop() override;
		~SceneGame();
};