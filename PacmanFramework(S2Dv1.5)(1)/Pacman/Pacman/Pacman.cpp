#include "Pacman.h"
#include <iostream>
#include <sstream>
#include <random>
#include <time.h>
using namespace std; 
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv),_cPacmanSpeed(0.1), _cPacmanFrameTime(250), _cMunchieFrameTime(250),_cCherryFrameTime(250)
{

	_pacman = new Player();
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i] = new Edibles();
	}
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
	}
	int i;
	for (i = 0; i < MUNCHIECOUNT; i++)

	{
		_munchie[i] = new Edibles();
		_munchie[i]-> CurrentFrameTime = 0;
		_munchie[i]->FrameCount = rand() % 500 + 50;
		_munchie[i]->Frame = rand() % 1;
	}

	_pop = new SoundEffect();
	_nom = new SoundEffect();
	_death = new SoundEffect();
	_cherryPickup = new SoundEffect();
	_victory = new SoundEffect();


	//Set Bools For Menu
	_paused = false;
	_escKeyDown = false;
	_endlife = false;
	_startMenuActive = true;
	_winConditions = false;


	//Set Pacmans Variables
	_pacman->Direction = 0;
	_pacman->CurrentFrameTime = 0;
	_pacman->Frame = 0;
	_pacman->dead = false;
	_pacman->lives = 3;
	_pacman->score = 0;

	// Initialise Ghostcharacter
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;

	}
	//Sets cherrys variables
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i]->FrameCount = 0;
		_cherry[i]->CurrentFrameTime = 0;
		_cherry[i]->Frame = 0;
		_cherry[i]->cherryBuff = 1.25f;
	}

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();


}


Pacman::~Pacman()
{
	//Destroying Pointers
	delete _pacman->Texture;
	delete _pacman->SourceRect;
	delete _pacman->Position;

	delete _pop;
	delete _nom;
	delete _death;
	delete _cherryPickup;
	delete _victory;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchie[i]->BlueTexture;
		delete _munchie[i]->Rect;
	}
	for (int i = 0; i < CHERRYCOUNT; i++)
	{

		delete _cherry[i]->Texture;
		delete _cherry[i]->Rect;

	}
	delete _pacman;
}

void Pacman::LoadContent()
{
	// Load Pacman

	_pacman->Texture = new Texture2D();
	_pacman->Texture->Load("Textures/Pacman.png", false);
	_pacman->Position = new Vector2(350.0f, 350.0f);
	_pacman->SourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->speedMultiplier = 1.0f;

	//Loading Ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/Ghost1.png", true);
		_ghosts[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}


	//Loading Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.tga", true); 
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]-> Texture = munchieTex;
	}
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			_munchie[i]->BlueTexture = new Texture2D();
			_munchie[i]->BlueTexture->Load("Textures/Munchie.png", false);
			_munchie[i]->Position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
			_munchie[i]->Rect = new Rect(0.0f, 0.0f, 12, 12);
		}
	// Load Cherry
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			_cherry[i]->Texture = new Texture2D();
			_cherry[i]->Texture->Load("Textures/CherrySpriteSheet.png", false);
			_cherry[i]->Position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
			_cherry[i]->Rect = new Rect(0.0f, 0.0f, 12, 12);
		}

	//Loading Sounds
	_pop->Load("Sounds/pop.wav");
	_nom->Load("Sounds/nom.wav");
	_death->Load("Sounds/death.wav");
	_cherryPickup->Load("Sounds/cherry.wav");
	_victory->Load("Sounds/Victory.wav");


	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Set Start Menu Position
	_startMenuBackground = new Texture2D();
	_startMenuBackground->Load("Textures/Transparency.png", false);
	_startMenuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startMenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);



}


//Update Function
void Pacman::Update(int elapsedTime)
{
	//Checking input from keyboard and mouse
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Runnning Menu Functions
	Pacman::CheckStarted(elapsedTime, keyboardState);
	Pacman::CheckPaused(elapsedTime, keyboardState);

	if (!_startMenuActive)
	{
		//Death Function
		Pacman::WinConditions(keyboardState);
		if (!_winConditions)
		{
			Pacman::EndLives(keyboardState);
			if (!_endlife)
			{
				if (!_paused) {
					//Running rest of functions after pause menu is down
					Pacman::Input(elapsedTime, keyboardState, mouseState);
					for (int i = 0; i < MUNCHIECOUNT; i++)
					{
						Pacman::UpdateMunchie(_munchie[i], elapsedTime);
						Pacman::CheckMunchieCollisions();
					}
					Pacman::CheckViewportCollision();
					Pacman::UpdateCherry(elapsedTime);
					Pacman::UpdatePacman(elapsedTime);
					for (int i = 0; i < GHOSTCOUNT; i++)
					{
						Pacman::UpdateGhost(_ghosts[i], elapsedTime);
						Pacman::CheckGhostCollisions();
						Pacman::CheckCherryCollisions();

					}



				}
			}
		}
	}
	
}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	if (ghost->direction == 0) //Moves Right 
	{
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 1) //Moves Left 
	{
		ghost->position->X -= ghost->speed * elapsedTime;
	}
	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) //Hits Right edge 
	{
		ghost->direction = 1; //Change direction 
	}
	else if (ghost->position->X <= 0) //Hits left edge 
	{
		ghost->direction = 0; //Change direction 
	}
}
//Collision Function for ghosts
void Pacman::CheckGhostCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Populate variables with Ghost data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height; left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}
void Pacman::CheckMunchieCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		// Populate variables with Munchie data
		bottom2 = _munchie[i]->Position->Y + _munchie[i]->Rect->Height; left2 = _munchie[i]->Position->X;
		right2 = _munchie[i]->Position->X + _munchie[i]->Rect->Width;
		top2 = _munchie[i]->Position->Y;
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->score=_pacman->score + 1;
			Audio::Play(_nom);
			_munchie[i]->Position->Y = 4000;
			_munchie[i]->Position->X = 4000;
			i = MUNCHIECOUNT;
		}
	}

}
void Pacman::CheckCherryCollisions()
{	
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->Position->Y + _pacman->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->Position->X;
	int left2 = 0;
	int right1 = _pacman->Position->X + _pacman->SourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->Position->Y;
	int top2 = 0;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		// Populate variables with Munchie data
		bottom2 = _cherry[i]->Position->Y + _cherry[i]->Rect->Height; left2 = _cherry[i]->Position->X;
		right2 = _cherry[i]->Position->X + _cherry[i]->Rect->Width;
		top2 = _cherry[i]->Position->Y;
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			Audio::Play(_cherryPickup);
			_cherry[i]->cherryBuff = _cherry[i]->cherryBuff + 0.25;
			_cherry[i]->Position->Y = 4000;
			_cherry[i]->Position->X = 4000;

			i = MUNCHIECOUNT;
		}
	}

}


void Pacman::Input(int elapsedTime, Input::KeyboardState* state , Input::MouseState* mouseState)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; 
	// Check if mouse pressed
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		for (int i = 0; i < CHERRYCOUNT; i++)
		{

			_cherry[i]->Position->X = mouseState->X;
			_cherry[i]->Position->Y = mouseState->Y;
		}
	}

	// Moves cherry

	
		//srand(time(NULL));
		//int ranX = rand() % Graphics::GetViewportWidth();
		//int ranY = rand() % Graphics::GetViewportHeight();
	//	_cherry->Position = new Vector2(ranX, ranY);
	

	


	
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->Position->X += pacmanSpeed;  //Moves Pacman across X axis
		_pacman->Direction = 0;
	}
	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->Position->X -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->Direction = 2;
	}

	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->Position->Y -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->Direction = 3;
	}


	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->Position->Y += pacmanSpeed; //Moves Pacman across X axis
		_pacman->Direction = 1;
	}
	// Speed Multiplier
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		// Apply multiplier
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			_pacman->speedMultiplier = _cherry[i]->cherryBuff;
		}

	}
	else
	{
		// Reset multiplier
		_pacman->speedMultiplier = 1.0f;
	}

}


void Pacman::CheckStarted(int elapsedTime, Input::KeyboardState* state)
{
	if (state->IsKeyDown(Input::Keys::SPACE) && !_escKeyDown)
	{
		_startMenuActive = false;
	}
	
}
void Pacman::EndLives(Input::KeyboardState* state)
{
	if (_pacman->lives <= 0)
	{
		_endlife = true;
		if (state->IsKeyDown(Input::Keys::R))
		{
			_endlife = false;
			_pacman->lives = 3;
			_pacman->score = 0;
			for (int i = 0; i < CHERRYCOUNT; i++)
			{
				_cherry[i]->cherryBuff = 1.25;
			}
			Pacman::LoadContent();
			

			
		}
	}
}




void Pacman::CheckPaused(int elapsedTime, Input::KeyboardState* state)
{
	if (state->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown)
	{
		_escKeyDown = true;

		_paused = !_paused;
	}
	if (state->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;


}




void Pacman::CheckViewportCollision()
{
	// Loops around edges

	if (_pacman->Position->X + _pacman->SourceRect->Width > Graphics::GetViewportWidth())
	{
		_pacman->Position->X = 0 - _pacman->SourceRect->Width;
	}
	if (_pacman->Position->X + _pacman->SourceRect->Width < 0)
	{
		_pacman->Position->X = Graphics::GetViewportWidth() - _pacman->SourceRect->Width;
	}
	if (_pacman->Position->Y + _pacman->SourceRect->Height > Graphics::GetViewportHeight())
	{
		_pacman->Position->Y = 0 - _pacman->SourceRect->Height;
	}
	if (_pacman->Position->Y + _pacman->SourceRect->Height < 0)
	{
		_pacman->Position->Y = Graphics::GetViewportHeight() - _pacman->SourceRect->Height;
	}

}

void  Pacman::WinConditions(Input::KeyboardState* state)
{	
	if (_pacman->score >= 49)
	{
		Audio::Play(_victory);
		_winConditions = true;
		if (state->IsKeyDown(Input::Keys::R))
		{
			_endlife = false;
			_pacman->lives = 3;
			_pacman->score = 0;
			for (int i = 0; i < CHERRYCOUNT; i++)
			{
				_cherry[i]->cherryBuff = 1.25;
			}
			Pacman::LoadContent();
			_winConditions = false;
		}
	}
}

void  Pacman::UpdateMunchie(Edibles* Texture, int elapsedTime)
{

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]->CurrentFrameTime += elapsedTime;

		if (_munchie[i]->CurrentFrameTime > _cMunchieFrameTime)
		{
			_munchie[i]->Rect->Y = _munchie[i]->Rect->Height * 0;
			_munchie[i]->FrameCount++;
			if (_munchie[i]->FrameCount >= 300)
			{
				_munchie[i]->FrameCount = 0;
				_munchie[i]->Rect->Y = _munchie[i]->Rect->Height * 1;
				_munchie[i]->CurrentFrameTime = 0;
			}

		}
	}
}
void  Pacman::UpdateCherry(int elapsedTime)
{
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i]->CurrentFrameTime += elapsedTime;
		if (_cherry[i]->CurrentFrameTime > _cCherryFrameTime)
		{
			_cherry[i]->Rect->Y = _cherry[i]->Rect->Height * 1;
			_cherry[i]->Frame++;
			if (_cherry[i]->Frame >= 30)
			{
				_cherry[i]->Frame = 0;
				_cherry[i]->Rect->Y = _cherry[i]->Rect->Height * 0;
				_cherry[i]->CurrentFrameTime = 0;
			}
		}
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	//Pacman timing for animation
	_pacman->CurrentFrameTime += elapsedTime;
	if (_pacman->CurrentFrameTime > _cPacmanFrameTime)
	{
		_pacman->SourceRect->X = _pacman->SourceRect->Width * _pacman->Frame;
		_pacman->SourceRect->Y = _pacman->SourceRect->Height * _pacman->Direction;
		_pacman->Frame++;
		if (_pacman->Frame >= 3)
		{
			_pacman->Frame = 0;
			_pacman->CurrentFrameTime = 0;
		}
	}
}



void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Score: " << _pacman->score;

	SpriteBatch::BeginDraw(); // Starts Drawing
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->Texture, _pacman->Position, _pacman->SourceRect); // Draws Pacman
	}
	else
	{
		_pacman->Position->X = 350;
		_pacman->Position->Y = 350;
		Audio::Play(_pop);
		_pacman->lives = _pacman->lives-1;
		_pacman->dead = false;
	}
	int i;
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchie[i]->BlueTexture, _munchie[i]->Position, _munchie[i]->Rect); // Draw Munchie
	}
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		SpriteBatch::Draw(_cherry[i]->Texture, _cherry[i]->Position, _cherry[i]->Rect); //Draw Cherry
	}
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	if (_paused)

	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);


	}

	if (_startMenuActive)
	{
		std::stringstream startMenuStream;
		startMenuStream << "Start Menu";
		SpriteBatch::Draw(_startMenuBackground, _startMenuRectangle, nullptr);
		SpriteBatch::DrawString(startMenuStream.str().c_str(), _startMenuStringPosition, Color::Red);
	}

	if (_endlife)
	{

		std::stringstream startMenuStream;
		startMenuStream << "GAME OVER!!! Press R to reset"<<endl;
		SpriteBatch::Draw(_startMenuBackground, _startMenuRectangle, nullptr);
		SpriteBatch::DrawString(startMenuStream.str().c_str(), _startMenuStringPosition, Color::Red);
		_pacman->score = 0; 
	}
	if (_winConditions)
	{
		std::stringstream startMenuStream;
		startMenuStream << "Congratulations You Win. Press R to play again" << endl;
		SpriteBatch::Draw(_startMenuBackground, _startMenuRectangle, nullptr);
		SpriteBatch::DrawString(startMenuStream.str().c_str(), _startMenuStringPosition, Color::Red);
	}


	SpriteBatch::EndDraw(); // Ends Drawing
}

