#include "A3_GameplayScene.h"
#include "A3_GameoverScene.h"
#include "UtilityMath.h"

// #include "A3_GameoverScene.h"
#include <iostream>

float A3_GameplayScene::screenWidth = 0.0f; // screen width
float A3_GameplayScene::screenHeight = 0.0f; // screen height

// constructor; if no screenWidth or height is provided, screen wrapping is disabled
// A3_GameplayScene::A3_GameplayScene() {}

Scene * A3_GameplayScene::createScene()
{
	// 'scene' is an autorelease object
	Scene* scene = Scene::create(); // create without physics
	// Scene* scene = Scene::createWithPhysics(); // create with physics
	A3_GameplayScene* layer = A3_GameplayScene::create();

	scene->addChild(layer);
	// Vec2 winSize = Director::getInstance()->getWinSizeInPixels();
	return scene;
}

// Creates the scene, setting the scene's width and height as well.
Scene * A3_GameplayScene::createScene(float width, float height) 
{
	screenWidth = width;
	screenHeight = height;
	return createScene();
}

// What we do on enter. This activates base functionality.
void A3_GameplayScene::onEnter() { Scene::onEnter(); }

// Initalizer function
bool A3_GameplayScene::init()
{
	// Ensure the parent init function was called first. If it wasn't, exit this one.
	if (!Scene::init())
		return false;

	director = Director::getInstance();

	// Initialize the event handlers
	initListeners();

	//Init the Sprites
	initSprites();

	// Initialize the pause menu
	// initPauseMenu();

	// Allows for the update() function to be called by cocos
	this->scheduleUpdate();

	//Let cocos know that the init function was successful
	return true;	
}

// initializes liseners
void A3_GameplayScene::initListeners()
{
	// Init the contact listener
	// initContactListener();
	initKeyboardListener();
}

// void A3_GameplayScene::initMouseListener()
// {

// }

// Listens for keyboard inptu
void A3_GameplayScene::initKeyboardListener()
{
	//Create the keyboard listener
	keyboardListener = EventListenerKeyboard::create();

	//Setting up callbacks
	keyboardListener->onKeyPressed = CC_CALLBACK_2(A3_GameplayScene::keyDownCallback, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(A3_GameplayScene::keyUpCallback, this);

	//Add the keyboard listener to the dispatcher
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

// initContactListeners
/*
void A3_GameplayScene::initContactListener()
{
	//Create the contact listener
	EventListenerPhysicsContact* contactListener = EventListenerPhysicsContact::create();

	// create onContactBeginCallback
	//Assign callbacks
	contactListener->onContactBegin = CC_CALLBACK_1(A3_GameplayScene::onContactBeginCallback, this);

	//Add the listener to the event dispatcher
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

}
*/

// Callbacks; automatically called

// Pressing a Key
void A3_GameplayScene::keyDownCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		std::cout << "Space Bar Was Pressed!" << std::endl;
		spaceBar = true;
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		// pShip->sprite->setPosition(Vec2 (pShip->sprite->getPositionX() + 3.0f, pShip->sprite->getPositionY()));
		std::cout << "Up Key or 'W' Key Pressed!" << std::endl;
		upKey = true;

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		std::cout << "Down Key or 'S' was pressed" << std::endl;
		downKey = true;
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		std::cout << "Left Key or 'L' Key Pressed!" << std::endl;
		leftKey = true;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		std::cout << "Right Key or 'D' Pressed!" << std::endl;
		rightKey = true;
	}
	// Toggles on and off rotations
	if (keyCode == EventKeyboard::KeyCode::KEY_CAPS_LOCK)
	{
		std::cout << "Rotation Toggled!" << std::endl;
		pShip->rotateLR = !pShip->rotateLR;
	}
	// Toggles the hud on/off
	if (keyCode == EventKeyboard::KeyCode::KEY_H)
	{
		std::cout << "Display Toggled!" << std::endl;
		displayHud = !displayHud;
		// Since the layer order needs tobe changed one-by-one, this is done elsewhere.
		// If the hud is on, it is put on layer 2. If it's off, it's put on layer -1, which is behind everything.
	}
	
	std::cout << std::endl;
}

// Letting Go of a Key
void A3_GameplayScene::keyUpCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		std::cout << "Space Bar Was Released!" << std::endl;
		spaceBar = false;
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		// keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW ? std::cout << "Up Key Pressed!" << std::endl : std::cout << "W Key Pressed!" << std::endl;
		std::cout << "Up Key or 'W' Key Released!" << std::endl;
		upKey = false;

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW || keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		std::cout << "Down Key or 'S' Key Released!" << std::endl;
		downKey = false;
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		std::cout << "Left Key or 'L' Key Released!" << std::endl;
		leftKey = false;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		std::cout << "Right Key or 'D' Released!" << std::endl;
		rightKey = false;
	}

	std::cout << std::endl;
}

// Tells cocos if we want two things to collide, and what to do if there is collision. We should be using our own collision math.
// bool A3_GameplayScene::onContactBeginCallback(PhysicsContact& contact) { return false; }

// Initializes sprites
void A3_GameplayScene::initSprites()
{
	Enemy* enemy; // generic enemy object

	// Image starts in the 'resource' folder.
	// Init the background sprite.
	spr_BG = Sprite::create("images/space_bg1.jpg"); //Load the handle
	
	spr_BG->setAnchorPoint(Vec2(0.5f, 0.5f)); //Ensure the middle of the background is the anchor point
	spr_BG->setPosition(director->getWinSizeInPixels().width / 2.0f, director->getWinSizeInPixels().height / 2.0f); // centre background; ensure the middle of the background is the anchor point
	this->addChild(spr_BG); // Add the sprite, pushed way to the back
	// spr_BG2 = Sprite::create("UA/Background/spr_Background.jpg"); //Load the handle
	// spr_BG2->setAnchorPoint(Vec2(0.5f, 0.5f)); //Ensure the middle of the background is the anchor point
	// spr_BG2->setPosition(director->getWinSizeInPixels().width / 2.0f, director->getWinSizeInPixels().height / 2.0f); // centre background; ensure hte middle of the background is the anchor point

	// Setting the wrapping points for all entities
	Entity::setWrapPointsMin(Vec2(0.0f, 0.0f));
	Entity::setWrapPointsMax(Vec2(screenWidth, screenHeight));

	// Creates the player ship; all values beyond the constructor are set in the player class
	pShip = new Player(Vec2(director->getWinSizeInPixels().width / 2.0f, director->getWinSizeInPixels().height / 2.0f), "images/player_ship_ss.png");
	pShip->getSprite()->setGlobalZOrder(2);
	// smallAsteroid = new Enemy(1);
	// largeAsteroid = new Enemy(2);

	this->addChild(pShip->getSprite());

	// Starts off with random asteroids of varying sizes
	
	for (int i = 0; i < 3; i++)
	{
		spawnEnemy(rand() % 2 + 1);
	}
	
	spawnEnemy(7);
	
	// HUD; drawn on Layer 3
	hud = new DrawNode();
	hud->setName("hud"); // used to identify the node
	hud->setGlobalZOrder(3); // putting it on the second layer, which places everything part of it above everything else.
	
	// Labels
	// *scoreStr = "0"; // starting score string value.
	scoreLabel = Label::create("0", "fonts/arial.ttf", 25.0F, Size(screenWidth, screenHeight), TextHAlignment::CENTER, TextVAlignment::TOP);
	scoreLabel->setGlobalZOrder(3);
	scoreLabel->setName("score");
	
	scoreLabel->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
	// scoreLabel->setString("Ultron"); // used to change text
	scoreLabel->enableShadow();
	hud->addChild(scoreLabel);

	// *lifeStr = "LIVES: " + std::to_string(pShip->getLives()) + " ";
	lifeLabel = Label::create("LIVES: " + std::to_string(pShip->getLives()) + " ", "fonts/arial.ttf", 25.0F, Size(screenWidth, screenHeight), TextHAlignment::RIGHT, TextVAlignment::TOP);
	lifeLabel->setGlobalZOrder(3);
	lifeLabel->setName("lives");
	lifeLabel->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
	lifeLabel->setPosition(lifeLabel->getPosition() - Vec2(5.0F, 0.0F));
	lifeLabel->enableShadow();
	hud->addChild(lifeLabel);
	
	/*
	// Tutorial: https://www.youtube.com/watch?v=iBfggFTvldw

	// hpNode->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width, screenHeight - 5.0F), Color4F::BLACK); // background health
	// To set a pre-existing color, use Color4F::COLOUR_NAME. e.g. Color4F::ORANGE
	// RGBA = (Red, Green, Blue, Alpha) https://en.wikipedia.org/wiki/RGBA_color_space
	// 'Alpha' measures how opague each pixel is. The higher the 'alpha' value, the more opague the colour is.

	// https://cocos2d-x.org/reference/native-cpp/V3.0alpha0/d2/d77/structcocos2d_1_1_color4_f.html#a9b6e45d9600d22cba771844689366dd8

	// can be done by four corner points, or starting point and ending point.
	// hpNode->drawSolidRect(Vec2(49.0F, 324.0F), Vec2(200.0F, 400.0F), Color4F(50.0F, 200.0F, 150.0F, 1.00F));  // test rectangle
	// hpNode->drawRect(Vec2(1.0F, screenHeight - 41), Vec2(200, screenHeight - 1.0), Color4F(1.00F, 1.00F, 1.00F, 1.00F));
	*/

	// Background Health
	hpBar[0]->setGlobalZOrder(3);
	hpBar[0]->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width, screenHeight - 5.0F), Color4F::BLACK);
	
	// The actual health bar
	hpBar[1]->setGlobalZOrder(3);
	hpBar[1]->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width * pShip->getHealth() / pShip->getMaxHealth(), screenHeight - 5.0F), Color4F::GREEN);
	// hpBar[1]->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width * pShip->getHealth() / pShip->getMaxHealth(), screenHeight - 5.0F), Color4F::GREEN);
	hpBar[1]->setAnchorPoint(Vec2(1.0F, 0.0F)); // this is the only shape that gets altered, so the anchour point needs to be defined.
	

	// Makes three lines seperating the sections on the health bar. Goes through indexes 2 - 4
	for (int i = 2; i < 5; i++)
	{
		hpBar[i]->setGlobalZOrder(3);
		hpBar[i]->drawLine(Vec2(5.0F + BARSIZE.width / 4 * (i - 1), screenHeight - 5.0F), Vec2(5.0F + BARSIZE.width / 4 * (i - 1), screenHeight - BARSIZE.height - 5.0F), Color4F::GRAY);
	}
		
	
	hpBar[5]->setGlobalZOrder(3);
	hpBar[5]->drawRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width, screenHeight - 5.0F), Color4F(Color3B(240.0F, 240.0F, 240.0F), 1.0F)); // the outline. Color3B is used for compatibility with traditional RGB
	
	for (int i = 0; i < BARLEN; i++) // adds all hud assets to the 'hud' node.
	{
		hud->addChild(hpBar[i]);
	}
	// hud->addChild(hpBar); // adds the hp nodes
	// hud->setParent()
	
	if(displayHud) // if the hud is set to be displayed.
		this->addChild(hud);
	// this->addChild(hpNode);

}

// Spawns a random enemy if no enemy is passed
void A3_GameplayScene::spawnEnemy(Enemy * enemy)
{
	this->addChild(enemy->getSprite()); // adds the sprite to the draw list
	enemies.push_back(enemy); // adds the enemy to the enemy vector
}

// Spawns a specific enemy
void A3_GameplayScene::spawnEnemy(unsigned short int type)
{
	Enemy * enemy = new Enemy(type);
	spawnEnemy(enemy); // passes the enemy to be spawned.
}

// Main update loop
void A3_GameplayScene::update(float deltaTime)
{
	static float spawnTimer; // the timer used for spawning enemies
	static bool transition = false; // checks to see if there is a transition happening.

	timer += deltaTime; // gets delta time across 'x' period of time
	spawnTimer += deltaTime;

	// updates screen size incase something has changed
	screenWidth = director->getWinSizeInPixels().width;
	screenHeight = director->getWinSizeInPixels().height;

	if(spawnTimer > 4.0F)// when 2 second(s) has passed, an enemy is spawned.
	{
		spawnEnemy(); // spawns the enemy
		spawnTimer = 0; // puts the timer back at 0.
		// spawn = deltaTime; // gets current timer
	}

	updatePlayer(deltaTime);
	updateEnemies(deltaTime);
	updateProjectiles(deltaTime);

	collisions(deltaTime); // goes through collisions
	// updateHud(); // updates the HUD information; now only updates whenever necessary
	// std::cout << "pShip Theta - D: " << umath::radiansToDegrees(pShip->theta) << " R: " << pShip->theta << std::endl;

	// if the global 'z' order of the hud is -1, but displayHud is on, that means the layering needs to be changed.
	if (displayHud && hud->getGlobalZOrder() < 0)
	{
		// Puts the hud in front of everything.
		hud->setGlobalZOrder(3);
		scoreLabel->setGlobalZOrder(3);
		lifeLabel->setGlobalZOrder(3);
		
		for (int i = 0; i < sizeof(hpBar) / sizeof(*hpBar); i++)
		{
			hpBar[i]->setGlobalZOrder(3);
		}
	}
	else if (!displayHud && hud->getGlobalZOrder() >= 0) // if 'displayHud' is turned off, but the global order
	{
		// puts the HUD behind everything.
		hud->setGlobalZOrder(-1);
		scoreLabel->setGlobalZOrder(-1);
		lifeLabel->setGlobalZOrder(-1);

		for (int i = 0; i < sizeof(hpBar) / sizeof(*hpBar); i++)
		{
			hpBar[i]->setGlobalZOrder(-1);
		}
	}

	// Moves over to the game over screen if the player's health is less than or equal to '0'.
	if (pShip->getHealth() <= 0 && !transition)
	{
		if (pShip->getLives() > 0) // if the player still has lives left
		{
			pShip->setHealth(pShip->getMaxHealth()); // restoring health.
			pShip->setLives(pShip->getLives() - 1); // decreases the life count by '1'.
			pShip->setPosition(Vec2(director->getWinSizeInPixels().width / 2, director->getWinSizeInPixels().height / 2)); // puts the player back at te middle of the screen
			updateHud(1); // updates health bar
			updateHud(3); // updates lives
		}
		else // if the player is having a game over.
		{
			Scene *newScene = A3_GameoverScene::createScene();
			Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0F, newScene));
			// Director::getInstance()->replaceScene(newScene);
			transition = true;
		}

	}
}

// updates related to the player
void A3_GameplayScene::updatePlayer(float deltaTime)
{
	// Sets whether pressing left and right rotates, or moves the ship.
	// The force that gets applied.
	const float force(pShip->getAppliedForce());
	static float locTime = 0.5F; // prevents a projectile from being fired for (x) amount of time
	const float R_SPD = 0.045F; // controls how fast the player rotates

	Projectile * proj;

	if (spaceBar)
	{
		if (locTime > 0.3F) // has a delay between when a projectile can be fired again.
		{
			// creates a new projectile that lasts for 'x' amount of milliseconds
			proj = new Projectile(pShip->getPosition(), "images/projectile2.png", 500.0F, 7.50F, pShip->theta, 1.0F, 2.5F); // at a time of 3.5F milliseconds, the projectile just barely does a full horizontal loop of the screen.
			projectiles.push_back(proj);
			this->addChild(projectiles.at(projectiles.size() - 1)->getSprite());
			locTime = 0;

		}
		else
		{
			locTime += deltaTime;
		}
		
	}

	// Going Up
	if (upKey)
	{
		// If true, an 'x' force and 'y' force since the force is going to be rotated.
		(pShip->rotateLR) ? pShip->addForce(0, force) : pShip->addForce(0, force);
		// pShip->setPositionY(pShip->getPositionY() + 1);
	}
	// Going Down
	else if (downKey)
	{
		(pShip->rotateLR) ? pShip->addForce(0, -force) : pShip->addForce(0, -force);
		// pShip->setPositionY(pShip->getPositionY() - 1);
	}


	// Going Left
	if (leftKey)
	{
		(pShip->rotateLR) ? pShip->theta -= R_SPD  : pShip->addForce(-force, 0);
	}
	// Going Right
	else if (rightKey)
	{
		(pShip->rotateLR) ? pShip->theta += R_SPD : pShip->addForce(force, 0);
	}

	pShip->update(deltaTime);
}

// Updates enemies
void A3_GameplayScene::updateEnemies(float deltaTime)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i)->update(deltaTime); // calls individual updates

		// If the enemy has 0 health or less, it is deleted.
		if (enemies.at(i)->getHealth() <= 0)
		{
			enemies.at(i)->getSprite()->runAction(RemoveSelf::create()); // removes the sprite from the draw list
			enemies.erase(enemies.begin() + i); // erases the enemy from the enemy vector
			i--;
		}

		// umath::collision(pShip, enemies.at(i)); // collision check
	}
}

// updates related to the projectiles.
void A3_GameplayScene::updateProjectiles(float deltaTime)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles.at(i)->update(deltaTime); // calls individual updates
		if (projectiles.at(i)->getTime() >= projectiles.at(i)->LIFESPAN)
		{
			projectiles.at(i)->getSprite()->runAction(RemoveSelf::create()); // removes the sprite from the draw list
			projectiles.erase(projectiles.begin() + i);
			i--;
		}
		// umath::collision(pShip, projectiles.at(i)); // collision check
	}
}

// collisions
void A3_GameplayScene::collisions(float deltaTime)
{
	Enemy * enemy; // temporary enemy object
	
	Vec2 distVec; // the distance between two objects as a vector
	float distScl; // the distance between two objects as a scalar
	Vec2 force; // a variable used to hold the force being applied to an entity.
	float angle; // a variable used to store the angle of an operation.
	Rect section; // saves a section of the sprite sheet, which is used to change sprites for some enemies.

	// bool statUpdate = false; // tells the program to update the hud
				   
	// Player colission with enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		// If the secondary radius of the enemy is not equal to '0', then it needs to be checked for collision.
		if (enemies.at(i)->getSecondRadius() != 0.0F)
		{
			// If the player is within the range of the second radius
			if (umath::collision(pShip->getPosition(), pShip->getScaledRadius(), enemies[i]->getPosition(), enemies[i]->getScaledSecondRadius()))
			{
				switch (enemies[i]->getType())
				{
				// Black Hole; the player is drawn towards it.
				case 3:
					// gets the distance between the ship and the enemy as a vector
					distVec = Vec2(pShip->getPositionX() - enemies[i]->getPositionX(), pShip->getPositionY() - enemies[i]->getPositionY());

					force = enemies[i]->getForce2(); // gets the force that will be applied to the player.

					// applies force to the ship (divided by the ship's mass) in the direction of the normalize distance between the ship and the enemy. It is also multiplied by the blackhole's bass, and deltaTime.
					pShip->setVelocity(pShip->getVelocity() - Vec2(distVec.getNormalized().x * force.x * enemies[i]->getMass() / pShip->getMass(), distVec.getNormalized().y * force.y * enemies[i]->getMass() / pShip->getMass()) * deltaTime);
					break;

				// Planet; the player is shot at by applying a rotation factor to the shot.
				case 4:
					if (enemies[i]->actionAvailable()) // if the planet is ready to fire a shot.
					{
						enemy = new Enemy(8); // creates a projectile
						this->addChild(enemy->getSprite());
						enemies.push_back(enemy);
						
						distVec = Vec2(pShip->getPosition() - enemies[i]->getPosition()); // gets a vector that represents the distance between the planet and the player.
						distScl = distVec.length();
						enemy->setPosition(enemies[i]->getPosition()); // sets the position of the projectile
						enemy->setScale(1.25F); // sets the scale of the projectile up for the planet.

						enemy->theta = umath::degreesToRadians(90 - umath::radiansToDegrees(umath::getRightTriangleAngle(distVec.x, distVec.y, distScl, 'c')));
						

						if (distVec.y < 0) // if the player is 'below' the planet, then 'theta' is offset by 180 degrees.
							enemy->theta = umath::degreesToRadians(180) - enemy->theta;

						// enemy->theta = umath::degreesToRadians(rand() % 360 + 1);
						enemies[i]->actionOcurred(); // tells the enemy that the action has been performed.
						
						// std::cout << "enemy->theta: " << umath::radiansToDegrees(enemy->theta) << std::endl;
						// std::cout << "DirVec: (" << distVec.x << ", " << distVec.y << ") D = " << distScl << std::endl;
						// std::cout << "DirVec ANGLE: " << umath::radiansToDegrees(umath::getRightTriangleAngle(distVec.x, distVec.y, distScl, 'c')) << "\n" << std::endl;
						// std::cout << "Theta: " << enemy->theta << std::endl;
					}
					break;
				
				// Shooter Ship; rotates and fires, but doesn't move.
				case 5:
					distVec = Vec2(pShip->getPosition() - enemies[i]->getPosition()); // getting a vector to represent the distance between the player and the enemy (x, y).
					distScl = distVec.length(); // getting the length of the vector between the player and the ship, which would be the hypotenuse of the triangle.

					// saves the angle that the enemy must rotate to in order to be aligned with the player's ship.
					angle = umath::degreesToRadians(90 - umath::radiansToDegrees(umath::getRightTriangleAngle(distVec.x, distVec.y, distScl, 'c')));

					if (distVec.y < 0) // if the player is 'below' the ship, then 'theta' is offset by 180 degrees.
					{
						angle = umath::degreesToRadians(180) - angle;
					}

					if (abs(umath::radiansToDegrees(enemies[i]->theta - angle)) <= 2.0F || (abs(umath::radiansToDegrees(enemies[i]->theta - angle)) >= 358.0F))
					{
						enemies[i]->theta = angle;
					}
					else
					{
						enemies[i]->theta = (enemies[i]->theta < angle) ? enemies[i]->theta + umath::degreesToRadians(7) : enemies[i]->theta - umath::degreesToRadians(7); // controls how fast the enemy ship rotates when it sees the player.
					}

					if (enemies[i]->actionAvailable()) // if the ship is able to fire.
					{
						enemy = new Enemy(8); // creates a projectile to be fired at the player.
						this->addChild(enemy->getSprite());
						enemies.push_back(enemy);

						enemy->setPosition(enemies[i]->getPosition());
						enemy->theta = enemies[i]->theta; // takes the enemy's theta and applies it to the projectile.
						enemy->setForce1(Vec2(0.0F, 70.0F));

						enemies[i]->actionOcurred(); // tells the enemy that the action has occurred.
					}

					break;

				// Kamikaze Ship
				case 6:
					section = enemies[i]->getCrop();
					if (enemies[i]->getSprite()->getTextureRect().getMinX() != section.getMaxX()) // if the first sprite is being displayed, it switches to the other frame.
					{
						enemies[i]->getSprite()->setTextureRect(Rect(section.getMaxX(), 0.0F, section.getMaxX(), section.getMaxY()));
					}

					distVec = Vec2(pShip->getPosition() - enemies[i]->getPosition()); // getting a vector to represent the distance between the player and the enemy (x, y)
					distScl = distVec.length(); // getting the length of the vector between the player and the ship, which would be the hypotenuse of the triangle.

					// saves the angle that the enemy must rotate to in order to be aligned with the player's ship.
					angle = umath::degreesToRadians(90 - umath::radiansToDegrees(umath::getRightTriangleAngle(distVec.x, distVec.y, distScl, 'c')));
					
					if (distVec.y < 0) // if the player is 'below' the ship, then 'theta' is offset by 180 degrees.
					{
						angle = umath::degreesToRadians(180) - angle;

						// std::cout << "Theta Difference (Degrees): " << umath::radiansToDegrees(enemies[i]->theta - angle) << std::endl;
						// std::cout << "E-THETA: " << umath::radiansToDegrees(enemies[i]->theta) << "\n" << std::endl;
					}

					// If the enemy's angle is less than the angle they must be at to hit the player, then it is increased by the the desired angle / 3.
					// if the enemy's angle is greater than the anele they must be at to hit the player, it is decreased by the desired angle / 3.

					// There is an issue where the angle of the enemy cannot reach a specific point. In these cases, the enemy will rotate the long way around since it can't go passed this point.
					// This happenes when the enemy reaches an angle of -90 degrees or 270 degrees. In order to get around this issue, the enemy must be put at the angle it's aiming to achieve.
					// After this point, the enemy will stay on the player until they leave the zone of sight.
					if (abs(umath::radiansToDegrees(enemies[i]->theta - angle)) <= 2.0F || (abs(umath::radiansToDegrees(enemies[i]->theta - angle)) >= 358.0F))
					{
						enemies[i]->theta = angle;
					}
					else
					{
						enemies[i]->theta = (enemies[i]->theta < angle) ? enemies[i]->theta + umath::degreesToRadians(7) : enemies[i]->theta - umath::degreesToRadians(7); // controls how fast the enemy ship rotates when it sees the player.
					}

					enemies[i]->addForce(0.0F, 60.0F); // makes the enemy go faster so that it can more easily catch the player.
					break;

				// BOSS
				case 7:
					distVec = Vec2(pShip->getPosition() - enemies[i]->getPosition()); // getting a vector to represent the distance between the player and the enemy (x, y)
					distScl = distVec.length(); // getting the length of the vector between the player and the ship, which would be the hypotenuse of the triangle.

					// saves the angle that the enemy must rotate to in order to be aligned with the player's ship.
					angle = umath::degreesToRadians(90 - umath::radiansToDegrees(umath::getRightTriangleAngle(distVec.x, distVec.y, distScl, 'c')));

					if (distVec.y < 0) // if the player is 'below' the ship, then 'theta' is offset by 180 degrees.
					{
						angle = umath::degreesToRadians(180) - angle;
					}

					// Sets the enemy to the proper angle if the difference between its current angle and the desigend angle is small enough.
					if (abs(umath::radiansToDegrees(enemies[i]->theta - angle)) <= 2.0F || (abs(umath::radiansToDegrees(enemies[i]->theta - angle)) >= 358.0F))
					{
						enemies[i]->theta = angle;
					}
					// has the 'boss' turn in order to lock on to the player.
					else
					{
						enemies[i]->theta = (enemies[i]->theta < angle) ? enemies[i]->theta + umath::degreesToRadians(5) : enemies[i]->theta - umath::degreesToRadians(5); // controls how fast the enemy ship rotates when it sees the player.
					}

					// force = enemies[i]->getForce2() * enemies[i]->getMass(); // gets the force that will be applied to the player so that they can be drawn in.
					// drwas the player towards the boss. The amount of force is effected by the boss's mass. This slightly differs from the calculations used for the black hoel.
					// pShip->setVelocity(pShip->getVelocity() - Vec2(distVec.getNormalized().x * force.x / pShip->getMass(), distVec.getNormalized().y * force.y / pShip->getMass()) * deltaTime);
					
					// if the boss can fire.
					if (enemies[i]->actionAvailable())
					{
						
						

						if (enemies[i]->getHealth() >= 2.0F && enemies[i]->getHealth() < 3.0f) // if the enemy has 2 health
						{
							for (int j = -2; j <= 2; j++) // fires '5' projectiles
							{
								enemy = new Enemy(9); // projectile 2
								enemy->setScale(enemy->getScale() * enemies[i]->getHealth() / enemies[i]->getMaxHealth()); // changes the scale based on how much health the boss has
								enemy->setMass(enemy->getMass() * enemies[i]->getHealth() / enemies[i]->getMaxHealth()); // sets the mass based on the boss's health

								enemy->setPosition(enemies[i]->getPosition()); // sets the position of the enemy to the position of the boss.
								enemy->theta = enemies[i]->theta + umath::degreesToRadians(20.0F) * j; // fires shots that are 20 degrees apart; one projectile is guaranteed to go for the player.
								this->addChild(enemy->getSprite()); // adds the enemy's sprite to the draw list
								enemies.push_back(enemy); // adds the enemy to the vector
							}
						}
						else if (enemies[i]->getHealth() >= 1.0F && enemies[i]->getHealth() < 2.0f) // if the enemy has 1 health
						{
							for (int j = -9; j <= 9; j++) // fires '18' projectiles, essentially going in all directions
							{
								enemy = new Enemy(9); // projectile 2
								enemy->setScale(enemy->getScale() * enemies[i]->getHealth() / enemies[i]->getMaxHealth()); // changes the scale based on how much health the boss has
								enemy->setMass(enemy->getMass() * enemies[i]->getHealth() / enemies[i]->getMaxHealth()); // sets the mass based on the boss's health

								enemy->setPosition(enemies[i]->getPosition()); // sets the position of the enemy to the position of the boss.
								enemy->theta = enemies[i]->theta + umath::degreesToRadians(20.0F) * j; // fires shots that are 20 degrees apart; one projectile is guaranteed to go for the player.
								this->addChild(enemy->getSprite()); // adds the enemy's sprite to the draw list
								enemies.push_back(enemy); // adds the enemy to the vector
							}
							
						}
						else // the enemy has maximum health, or no health
						{
							enemy = new Enemy(9); // projectile 2
							enemy->setScale(enemy->getScale() * enemies[i]->getHealth() / enemies[i]->getMaxHealth()); // changes the scale based on how much health the boss has
							enemy->setMass(enemy->getMass() * enemies[i]->getHealth() / enemies[i]->getMaxHealth()); // sets the mass based on the boss's health

							enemy->setPosition(enemies[i]->getPosition()); // sets the position of the enemy to the position of the boss.
							enemy->theta = enemies[i]->theta; // makes the shot go in the direction the enemy is facing
							this->addChild(enemy->getSprite()); // adds the enemy's sprite to the draw list
							enemies.push_back(enemy); // adds the enemy to the vector
						}

						enemies[i]->actionOcurred();
					}
					break;
				}
			}
			else // if the player is outside of the range.
			{
				switch (enemies[i]->getType())
				{
				// kamikaze ship
				case 6:
					if (enemies[i]->getSprite()->getTextureRect().getMinX() != 0.0F) // if the first image isn't already shown, it switches back to it.
					{
						enemies[i]->getSprite()->setTextureRect(enemies[i]->getCrop());
					}
				}

			}
		}

		// If there is collision, the player loses health. This collision is with the enemy's main hitcircle.
		if (enemies.at(i)->getRadius() != 0.0F && umath::collision(pShip, enemies[i]))
		{
			if (pShip->getKillable()) // if the player's able to take damage.
			{
				pShip->setHealth(pShip->getHealth() - enemies.at(i)->getDamage());
				pShip->playerHit(); // triggers invincibility frames
				updateHud(1); // updates the health bar
			}

			// statUpdate = true; // the hud needs to be updated
			break; // the player can only take damage from one enemy per frame.
		}
	}

	// Projectile Collision with Enemies
	for (int i = 0; i < projectiles.size(); i++)
	{
		for (int j = 0; j < enemies.size(); j++)
		{
			// If there is collision
			if (umath::collision(projectiles[i], enemies[j]))
			{
				// Large Asteroid splits into 2-4 small asteroids.
				if (enemies[j]->getKillable() && enemies[j]->getType() == 2)
				{
					for (int k = rand() % 3 + 2; k > 0; k--) // spawns two to four small asteroids
					{
						enemy = new Enemy(1);
						enemy->setPosition(enemies[j]->getPosition()); // gives the new small asteroid the big asteroid's original position

						enemies.push_back(enemy); // pushes the enemy onto the enemy vector
						this->addChild(enemies.at(enemies.size() - 1)->getSprite()); // adds the sprite to the draw list
					}
				}
				else
				{
					switch (enemies[j]->getType())
					{
					case 1: // small asteroid
						pShip->increaseScore(10); // adds '10' to the player score
						updateHud(2);
						break;

					case 3: // black holes
					case 4: // planets
					case 8:
					case 9:
						break;

					case 5: // shooter
					case 6: // kamikaze
						pShip->increaseScore(20); // adds '20' to the player score
						updateHud(2);
						break;

					case 7: // boss
						if (enemies[j]->getShield() == 0) // the boss is actually taking damage, and not just its shield.
						{
							pShip->increaseScore(1000); // adds '1000' to the player score
							updateHud(2);
						}
						break;

					default: // everything else
						pShip->increaseScore(10); // adds '10' to the player score
						updateHud(2); // updates the score
						// statUpdate = true; // updates the display.
					}
					
				}

				if (enemies.at(j)->getKillable() && enemies.at(j)->getShield() <= 0) // if the enemy can be killed and it has no shield, it loses health.
				{
					enemies.at(j)->setHealth(enemies.at(j)->getHealth() - projectiles.at(i)->getDamage()); // In the enemy update, an enemy is deleted if it has a health of '0' or less.
					
					// if it's the boss that took damage, then its shields are put back up.
					if (enemies[j]->getType() == 7 && enemies[j]->getHealth() > 0)
					{
						enemies.at(j)->setShield(enemies.at(j)->getMaxShield()); // puts the boss's shield back up
						enemies.at(j)->setScale(enemies.at(j)->getScale() / 2.0F); // makes the enemy smaller
						enemies.at(j)->setMass(enemies.at(j)->getMass() / 2.0F);
					}
				}
				else if (enemies.at(j)->getKillable() && enemies.at(j)->getShield() > 0) // if the enemy can be killed and it has a shield, the shield takes damage.
				{
					enemies.at(j)->setShield(enemies.at(j)->getShield() - projectiles.at(i)->getDamage());
				}
				
				// enemies.at(j)->getSprite()->runAction(RemoveSelf::create()); // removes the sprite from the draw list
				// enemies.erase(enemies.begin() + j); // erases the enemy from the enemy vector
				
				// Deleting the projectile
				projectiles.at(i)->getSprite()->runAction(RemoveSelf::create()); // removes the sprite from the draw list
				projectiles.erase(projectiles.begin() + i); // removes the sprite from the vector list
				i--;
				
				break;
			}
		}

		if (i >= projectiles.size())
			break;
	}

	// if (statUpdate) // updates the hud if something has changed, as determined by statUpdate
		// updateHud();
}

// updates the HUD; 0 updates all, 1 updates health, 2 updates score, and 3 updates lives.
void A3_GameplayScene::updateHud(int stat)
{
	if (displayHud == false) // if the hud display is off, the hud isn't updated.
		return;

	if (stat == 0 || stat == 1) // update health bar
	{
		// changed from a scale operation since it kept screwing up the formatting.
		hpBar[1]->clear();
		hpBar[1]->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width * pShip->getHealth() / pShip->getMaxHealth(), screenHeight - 5.0F), Color4F::GREEN);
		
		// The index of the actual health bar is '1'.
		// hpBar[1]->setScale(pShip->getHealth() / pShip->getMaxHealth(), 1.00F); // the size of the hp bar depends on the percentage.
		// hpBar[1]->setPosition(hpBar[1]->getPosition() + Vec2(1.0, 0.0));
		// std::cout << hpBar[1]->getPosition().x << ", " << hpBar[1]->getPosition().y << std::endl;
		// hpBar[1]->clear();

	}

	if (stat == 0 || stat == 2) // update score
		scoreLabel->setString(std::to_string(pShip->getScore())); // updates score
	
	if (stat == 0 || stat == 3) // update life count		
		lifeLabel->setString("LIVES: " + std::to_string(pShip->lives) + " "); // updates life count


	/*
	// Re-calculating the health bar
	hpNode->clear();
	
	hpNode->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width, screenHeight - 5.0F), Color4F::BLACK); // background health
	hpNode->drawSolidRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width * pShip->getHealth() / pShip->getMaxHealth(), screenHeight - 5.0F), Color4F::GREEN); // the actual health bar

	// Makes three lines seperating the sections on the health bar
	for (int i = 1; i <= 3; i++)
		hpNode->drawLine(Vec2(5.0F + BARSIZE.width / 4 * i, screenHeight - 5.0F), Vec2(5.0F + BARSIZE.width / 4 * i, screenHeight - BARSIZE.height - 5.0F), Color4F::GRAY);
	

	hpNode->drawRect(Vec2(5.0F, screenHeight - BARSIZE.height - 5.0F), Vec2(5.0F + BARSIZE.width, screenHeight - 5.0F), Color4F(Color3B(240.0F, 240.0F, 240.0F), 1.0F)); // the outline. Color3B is used for compatibility with traditional RGB
	*/
	
	// only the size of the bar needs to be changed. This is at index 1.
	// *lifeStr = "3";
	
	// this->addChild(hud); // adds the hud at the end of the draw list
}

// void A3_GameplayScene::onExit() {}

/*
// Updates both mouse and keyboard inputs.
void A3_GameplayScene::updateInputs()
{
	updateKeyboardInputs();
}

void A3_GameplayScene::updateKeyboardInputs()
{
	// if (endMenuDisplayed) return; // prevents use if end screen is active
	
}
*/