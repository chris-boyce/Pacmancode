#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(125) ,_cMunchieFrameTime(250)
{

	//Variables
	_munchieFrameCount = 0;
	_paused = false;
	_escKeyDown = false;
	_startMenuActive = true;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0; 
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;
	_munchieFrame = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.png", true);
	_munchiePosition = new Vector2(0.0f, 0.0f);
	_munchieRect = new Rect(0.0f, 0.0f, 12, 12);

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

void Pacman::Update(int elapsedTime)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown)
	{
		_escKeyDown = true;

		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;
	if (keyboardState->IsKeyDown(Input::Keys::SPACE))

	{

		_startMenuActive = false;

	}


	

	if (!_startMenuActive)
	{
		if (!_paused) {

			


			// Checks if D key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::D))
			{
				_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
				_pacmanDirection = 0;
			}
				// Checks if A key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::A))
			{
				_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
				_pacmanDirection = 2;
			}

					// Checks if W key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::W))
			{
				_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
				_pacmanDirection = 3;
			}


						// Checks if S key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::S))
			{
				_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
				_pacmanDirection = 1;
			}

			//Changes the way Pacman looks
			_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
			_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
			_pacmanCurrentFrameTime += elapsedTime;
			_munchieCurrentFrameTime += elapsedTime;


			//Pacman timing for animation
			if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
			{
				_pacmanFrame++;
				if (_pacmanFrame >= 2)
				{
					_pacmanFrame = 0;

					_pacmanCurrentFrameTime = 0;
				}
			}
			if(_munchieCurrentFrameTime > _cMunchieFrameTime)
			{
				_munchieFrame++;
					if (_munchieFrameCount >= 2)
					{
						_munchieFrameCount = 0;

						_munchieCurrentFrameTime = 0;
					}

			}



			// Loops around edges

			if (_pacmanPosition->X + _pacmanSourceRect->Width > Graphics::GetViewportWidth())
			{
				_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
			}
			if (_pacmanPosition->X + _pacmanSourceRect->Width < 0)
			{
				_pacmanPosition->X = Graphics::GetViewportWidth() - _pacmanSourceRect->Width;
			}
			if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight())
			{
				_pacmanPosition->Y = 0 - _pacmanSourceRect->Height;
			}
			if (_pacmanPosition->Y + _pacmanSourceRect->Height < 0)
			{
				_pacmanPosition->Y = Graphics::GetViewportHeight() - _pacmanSourceRect->Height;
			}

		}
	}




}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_munchieFrameCount == 0)
	{
		SpriteBatch::Draw(_munchieBlueTexture, _munchiePosition, _munchieRect);



	}

	else
	{
		// Draw Blue Munchie
		//SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_munchieFrameCount++;

		if (_munchieFrameCount >= 60)
			_munchieFrameCount = 0;
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
	SpriteBatch::EndDraw(); // Ends Drawing
}