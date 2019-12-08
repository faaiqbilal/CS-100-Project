#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\System.hpp"
#include <vector>


using namespace sf;


class Bullet
{
public:
	Sprite shape;
	Bullet(Texture *texture, Vector2f pos)

	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f,0.1f);
		this->shape.setPosition(pos.x + 30, pos.y);
	}
	~Bullet()
	{}
};

class Player
{
public:
	Sprite shape;
	Texture* texture;

	std::vector<Bullet> bullets;

	Player(Texture *texture, Vector2u windowSize)
	{
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
		this->shape.setPosition(windowSize.x/2, windowSize.y);
	}

	~Player()
	{}

};

class Enemy
{
public:
	Sprite shape;
	Texture* texture;

	Enemy(Texture *texture, Vector2u windowSize)
	{
		this->texture = texture;
		this->shape.setTexture(*texture);

		this->shape.setScale(0.03f, 0.03f);
		// Bug: stuff still spawning at the edges of window. For reference check part 3 of video series
		this->shape.setPosition(rand()%(int)(windowSize.x - this->shape.getGlobalBounds().width), shape.getGlobalBounds().height);
		this->shape.setRotation(180.f);
	}
	~Enemy()
	{}

};

class Asteroid
{
public:
	Sprite shape;
	Texture* texture;

	Asteroid(Texture *texture, Vector2f pos)
	{
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f,0.1f);
		this->shape.setPosition(pos);
	}

	~Asteroid()
	{}
};


int main()
{
	//Score Counter
	int Score_Count=0;


	// For everything random
	srand(time(NULL));

	// create window with framerate stuff
	RenderWindow window(VideoMode(800,1000),"Space Shooter",Style::Default);
	window.setFramerateLimit(60);

	//Firing rate for bullet
	int fireRate = 50;

	//Spawn timer for Enemies
	int SpawnTimerEnemies = 80;

	//Initializing text fonts after window is created
	Font font;
	font.loadFromFile("Fonts/Roboto-Light.ttf");
	//font.loadFromFile("Fonts/Roboto-Medium.ttf");

	//loading textures
	Texture playerTexture;
	playerTexture.loadFromFile("Textures/player-spaceship.png");

	Texture bulletTexture;
	bulletTexture.loadFromFile("Textures/bullet.png");

	Texture asteroidTexture;
	asteroidTexture.loadFromFile("Textures/asteroid.png");

	Texture enemyTexture;
	enemyTexture.loadFromFile("Textures/enemy-spacecraft.png");

	//Loading player
	Player player(&playerTexture, window.getSize());

	//Loading text
	Text Score_Display;
	Text Health_Display;
	Text Game_Over_Display;
	Text Final_Score_Display;
	Text Enter_Display;
	Text Space_Bar_Display;
	Text Right_Ctrl_Display;
	Text Left_Ctrl_Display;
	Text Up_Ctrl_Display;
	Text Down_Ctrl_Display;
	Text Title_Display;

	//TitleDisplay text
	Title_Display.setFillColor(Color::White);
	Title_Display.setFont(font);
	Title_Display.setPosition(180.f,50.f);
	Title_Display.setCharacterSize(60);
	Title_Display.setString("SPACE SHOOTER");

	//Downctrl display
	Down_Ctrl_Display.setFillColor(Color::White);
	Down_Ctrl_Display.setFont(font);
	Down_Ctrl_Display.setPosition(100.f, 250.f);
	Down_Ctrl_Display.setCharacterSize(40);
	Down_Ctrl_Display.setString("Press 'S' to move down");

	//Upctrl display
	Up_Ctrl_Display.setFillColor(Color::White);
	Up_Ctrl_Display.setFont(font);
	Up_Ctrl_Display.setPosition(100.f, 200.f);
	Up_Ctrl_Display.setCharacterSize(40);
	Up_Ctrl_Display.setString("Press 'W' to move up");

	//Leftctrl display
	Left_Ctrl_Display.setFillColor(Color::White);
	Left_Ctrl_Display.setFont(font);
	Left_Ctrl_Display.setPosition(100.f, 300.f);
	Left_Ctrl_Display.setCharacterSize(40);
	Left_Ctrl_Display.setString("Press 'A' to move left");

	//Rightctrl display
	Right_Ctrl_Display.setFillColor(Color::White);
	Right_Ctrl_Display.setFont(font);
	Right_Ctrl_Display.setPosition(100.f, 350.f);
	Right_Ctrl_Display.setCharacterSize(40);
	Right_Ctrl_Display.setString("Press 'D' to move right");

	//EnterDisplay Text
	Enter_Display.setFillColor(Color::White);
	Enter_Display.setFont(font);
	Enter_Display.setPosition(150.f, 550.f);
	Enter_Display.setCharacterSize(50);
	Enter_Display.setString("Press 'Enter' to start!");

	//SpaceBar Text
	Space_Bar_Display.setFillColor(Color::White);
	Space_Bar_Display.setFont(font);
	Space_Bar_Display.setPosition(100.f,400.f);
	Space_Bar_Display.setCharacterSize(40);
	Space_Bar_Display.setString("Press 'Space' to shoot");

	//Score text
	Score_Display.setFillColor(Color::White);
	Score_Display.setFont(font);
	
	//Player health
	int player_health=5;
	Health_Display.setFillColor(Color::White);
	Health_Display.setFont(font);
	
	//Game Over Text
	Game_Over_Display.setFillColor(Color::White);
	Game_Over_Display.setFont(font);
	Game_Over_Display.setPosition(230.f, 200.f);
	Game_Over_Display.setCharacterSize(60);
	Game_Over_Display.setString("GAME OVER!");

	//FinalScoreDisplay Text
	Final_Score_Display.setFillColor(Color::White);
	Final_Score_Display.setFont(font);
	Final_Score_Display.setPosition(220.f, 400.f);
	Final_Score_Display.setCharacterSize(60);

	//Loading enemy
	std::vector<Enemy> enemies;	

	//UPDATES
	bool Start_Screen = true;
	bool Game_Over = false;

		while (window.isOpen())
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			window.clear(sf::Color::Black);
			window.display();
			
			while( Start_Screen==true && Game_Over==false && player_health>0)
			{ 
				window.draw(Title_Display);
				window.draw(Enter_Display);
				window.draw(Space_Bar_Display);
				window.draw(Up_Ctrl_Display);
				window.draw(Down_Ctrl_Display);
				window.draw(Left_Ctrl_Display);
				window.draw(Right_Ctrl_Display);
				window.display();
			if (Keyboard::isKeyPressed(Keyboard::Enter))
			{
				Start_Screen = false;	
			}
			}

			while (player_health > 0 && Start_Screen==false && Game_Over==false)
			{


				//Update counter for bullet time
				if (fireRate < 20)
					fireRate++;

				//Score text display
				Score_Display.setPosition(650.f, 20.f);
				Score_Display.setString("Score: " + std::to_string(Score_Count));

				//Score text display
				Health_Display.setPosition(player.shape.getPosition().x , player.shape.getPosition().y - Health_Display.getGlobalBounds().height);
				Health_Display.setString( std::to_string(player_health) + " / 5" );


				// Updating values
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (fireRate >= 20))
				{
					//Spacebar is pressed: shoot
					player.bullets.push_back(Bullet(&bulletTexture, player.shape.getPosition()));
					fireRate = 0;
				}

				//Player movement
				if (Keyboard::isKeyPressed(Keyboard::W))
					player.shape.move(0.f, -10.f);
				if (Keyboard::isKeyPressed(Keyboard::A))
					player.shape.move(-10.f, 0.f);
				if (Keyboard::isKeyPressed(Keyboard::S))
					player.shape.move(0.f, 10.f);
				if (Keyboard::isKeyPressed(Keyboard::D))
					player.shape.move(10.f, 0.f);

				//Collision of player with window
				if (player.shape.getPosition().x <= 0) // Left side
				{
					player.shape.setPosition(0.f, player.shape.getPosition().y);
				}

				if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) //Right side
				{
					player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);
				}

				if (player.shape.getPosition().y <= 0) //Top side
				{
					player.shape.setPosition(player.shape.getPosition().x, 0.f);
				}

				if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) //Bottom side
				{
					player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);
				}

				for (size_t i = 0; i < player.bullets.size(); i++)
				{

					//Bullet movement with bullet speed defined
					player.bullets[i].shape.move(0.f, -30.f);

					//Reaching end of window
					if (player.bullets[i].shape.getPosition().y > window.getSize().y)
					{
						player.bullets.erase(player.bullets.begin() + i);
						break;
					}

					//Collision of bullet with enemies
					for (size_t k = 0; k < enemies.size(); k++)
					{
						if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
						{
							enemies.erase(enemies.begin() + k);
							player.bullets.erase(player.bullets.begin() + i);
							Score_Count++;
							break;
						}
					}

				}

				//Enemy Spawner timer update
				if (SpawnTimerEnemies < 80)
				{
					SpawnTimerEnemies++;
				}

				if (SpawnTimerEnemies >= 50)
				{
					enemies.push_back(Enemy(&enemyTexture, window.getSize()));
					SpawnTimerEnemies = 0;
				}


				//Update Enemy position
				for (size_t i = 0; i < enemies.size(); i++)
				{
					//Enemy Speed defined
					enemies[i].shape.move(0, 5);

					//Enemy collision with walls
					if (enemies[i].shape.getPosition().y <= enemies[i].shape.getGlobalBounds().height)
					{
						enemies.erase(enemies.begin() + i);
					}
					//bool player_interacting_enemy=false;


					//Enemy collision with player
					if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
					{
						enemies.erase(enemies.begin() + i);
						player_health--;

					}


				}

				// Draw
				window.clear();

				//Player
				window.draw(player.shape);


				//Using size_t everywhere because it is unsigned size of value. Question raised: Why doesn't it work if int is used instead of size_t?

				//Bullets
				for (size_t i = 0; i < player.bullets.size(); i++)
				{
					window.draw(player.bullets[i].shape);
				}

				//Enemy		
				for (size_t i = 0; i < enemies.size(); i++)
				{
					window.draw(enemies[i].shape);
				}
				window.draw(Health_Display);
				window.draw(Score_Display);
				window.display();

				
			}
			Game_Over = true;
			while(player_health <= 0 && Start_Screen == false && Game_Over == true)
	        {
				for (int i = 0; i < 100; i++)
				{
					window.clear(sf::Color::Black);
			        window.draw(Game_Over_Display);
		            Final_Score_Display.setString("Final Score: " + std::to_string(Score_Count));
		            window.draw(Final_Score_Display);
		            window.display();
		        }
		        window.close();

	        }
		
		}
	
	
		
    return 0;
}
