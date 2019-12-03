#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define CHERRYCOUNT 4


#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;
// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.







struct Player	// Structure Definition
{

	// Data to represent Pacman
	Vector2* Position;
	Rect* SourceRect;
	Texture2D* Texture;
	float Speed;
	int score;
	int Direction;
	int Frame;
	int CurrentFrameTime;
	float speedMultiplier; 
	bool dead;
	int lives;

};

struct Edibles
{

	int FrameCount;
	Rect* Rect;
	Texture2D* Texture;
	Vector2* Position;
	int CurrentFrameTime;
	int Frame;
	float cherryBuff;
	Texture2D* BlueTexture;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};




class Pacman : public Game
{
private:

	const int _cPacmanFrameTime;
	const float _cPacmanSpeed;
	const int _cCherryFrameTime;
	const int _cMunchieFrameTime;

	SoundEffect* _pop;
	SoundEffect* _nom;
	SoundEffect* _death;
	SoundEffect* _cherryPickup;
	SoundEffect* _victory;


	//Data For Pause Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _escKeyDown;

	//Data for start menu
	Texture2D* _startMenuBackground;
	Rect* _startMenuRectangle;
	Vector2* _startMenuStringPosition;
	bool _startMenuActive;
	bool _endlife;
	bool _winConditions;





	Vector2* _stringPosition;

	

	Player *_pacman;
	Edibles* _cherry[CHERRYCOUNT], * _munchie[MUNCHIECOUNT]	; 
	MovingEnemy* _ghosts[GHOSTCOUNT];

	// Data to represent Cherry & Animation





public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

private:

	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState );

	void CheckPaused(int elapsedTime, Input::KeyboardState* state);

	void CheckStarted(int elapsedTime, Input::KeyboardState* state);

	void EndLives(Input::KeyboardState* state);

	void CheckViewportCollision();

	void UpdatePacman(int elapsedTime);

	void UpdateMunchie(Edibles* _munchie ,int elapsedTime);

	void UpdateCherry(int elapsedTime);

	void CheckCherryCollisions();

	void CheckGhostCollisions();

	void UpdateGhost(MovingEnemy*, int elapsedTime);

	void CheckMunchieCollisions();

	void WinConditions(Input::KeyboardState* state);
};