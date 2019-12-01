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
		//Bug: Bullets come out of side of player instead of centre
		this->shape.setPosition(pos);
	}
	~Bullet()
	{}
};

class Player
{
public:
	Sprite shape;
	Texture* texture;

	int HP = 10;

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
	font.loadFromFile("Fonts/Roboto-Medium.ttf");

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

	//Loading enemy
	std::vector<Enemy> enemies;

	//Loading asteroid
	

	//UPDATES

	while(window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		
		//Update counter for bullet time
		if (fireRate <20)
			fireRate++;

		// Updating values
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (fireRate >=20))
		{
			//Spacebar is pressed: shoot
			player.bullets.push_back(Bullet(&bulletTexture, player.shape.getPosition()));
			fireRate = 0;
		}

		//Player movement
		if(Keyboard::isKeyPressed(Keyboard::W))
			player.shape.move(0.f, -10.f);
		if(Keyboard::isKeyPressed(Keyboard::A))
			player.shape.move(-10.f, 0.f);	
		if(Keyboard::isKeyPressed(Keyboard::S))
			player.shape.move(0.f, 10.f);
		if(Keyboard::isKeyPressed(Keyboard::D))
			player.shape.move(10.f, 0.f);

		//Collision of player with window
		if(player.shape.getPosition().x <= 0) // Left side
			{player.shape.setPosition(0.f, player.shape.getPosition().y);}

		if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) //Right side
			{player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);}		

		if (player.shape.getPosition().y <= 0) //Top side
			{player.shape.setPosition(player.shape.getPosition().x, 0.f);}

		if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) //Bottom side
			{player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);}

		for (size_t i =0; i<player.bullets.size(); i++)
		{

		//Bullet movement with bullet speed defined
			player.bullets[i].shape.move(0.f,-30.f);

		//Reaching end of window
			if(player.bullets[i].shape.getPosition().y > window.getSize().y)
				{
					player.bullets.erase(player.bullets.begin() + i); 
					break;
				}

		//Collision of bullet with enemies
			for (size_t k=0; k < enemies.size(); k++)
			{
				if(player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + k);
					player.bullets.erase(player.bullets.begin() + i); 
					break;
				}
			}			

		}

		//Enemy Spawner timer update
		if (SpawnTimerEnemies < 80)
			{SpawnTimerEnemies++;}

		if (SpawnTimerEnemies >= 50)
			{
				enemies.push_back(Enemy(&enemyTexture, window.getSize()));
				SpawnTimerEnemies=0;
			}

			
		//Update Enemy position
		for (size_t i = 0; i<enemies.size();i++)
		{
			//Enemy Speed defined
			enemies[i].shape.move(0, 5);

			//Enemy collision with walls
			if(enemies[i].shape.getPosition().y <= enemies[i].shape.getGlobalBounds().height)
				{
					enemies.erase(enemies.begin() + i);
				}

			//Enemy collision with player
			if(enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin()+i);
					player.HP--;
				}
		}

		// Draw
		window.clear();

		//Player
		window.draw(player.shape);


		//Using size_t everywhere because it is unsigned size of value. Question raised: Why doesn't it work if int is used instead of size_t?
		
		//Bullets
		for (size_t i = 0; i<player.bullets.size(); i++)
		{
			window.draw(player.bullets[i].shape);
		}

		//Enemy		
		for (size_t i = 0; i<enemies.size(); i++)
		{
			window.draw(enemies[i].shape);
		}

		window.display();
	}
    return 0;
}
