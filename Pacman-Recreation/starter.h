
#include <iostream>
#include <thread>
#include<chrono>
#include "pac.h"
#include "ghost.h"
#include "maze.h"
#include "gameText.h"
#include "fruit.h"

using namespace std;
using namespace sf;

class Starter
{
 public:	 		 		 
	 Event sfEvt;
	 Maze maze;
	 Pac<Starter>* pac;
	 Ghost<Starter> *Ghost1, *Ghost2, *Ghost3,*Ghost4;	 
	 thread ghStatusThread;
	 thread loopThread;
	 int attackInterval=15;
	 int scaterInterval=7;
	 int blueInterval = 6;	
	 int delay;
	 int curTime;
	 Texture backText;
	 Texture backFlashText;
         	 
	 Sprite backSpr;	 
	 bool intro = false;
	 bool isCollid = false;
	 bool lifeWin = false;
	 bool toNextLevel = false;
	 GameSound *gmSound;	
	 GameText *gameText;
	 Fruit* fruit;	
	 RenderWindow* window;	
	
	 Starter( RenderWindow *win, Texture* sprTexture)
	 {
		 window = win;
		 gmSound = GameSound::getInstance();		 
		 gameStatus = Demo;
		 backText.loadFromFile("texture/map1.png");
		 backFlashText.loadFromFile("texture/mapB.png");
		 backSpr.setTexture(backText);
		 maze.initMaze();		
		 pac = new Pac<Starter>(sprTexture,this);
		 Ghost1 = new Ghost<Starter>(sprTexture, 1,this);
		 Ghost2 = new Ghost<Starter>(sprTexture, 2,this);
		 Ghost3 = new Ghost<Starter>(sprTexture, 3,this);
		 Ghost4 = new Ghost<Starter>(sprTexture, 4,this);
		 gameText = new GameText();
		 fruit = new Fruit(sprTexture);		  
		 
		 loopThread = thread ( &Starter::loop,this,win );
		 while (win->isOpen())
		 {
			 if ( GameObject::dotsEat == maze.dotsCount) {nextLevel();}
			 while (win->pollEvent(sfEvt))
			 {
				 if (sfEvt.type == Event::Closed) { win->close(); }
				 else if (sfEvt.type == Event::KeyPressed)
				 {
					 if (gameStatus == Demo && sfEvt.key.code == Keyboard::Enter)
					 {
						 startGame();
					 }
					 pac->rotation(sfEvt.key.code);
				 }
			 }
		 }        
	 };

	~Starter() 
	{
		if (loopThread.joinable()) { loopThread.detach(); }
		if (ghStatusThread.joinable()) { ghStatusThread.detach(); }
		delete  Ghost1; 
		delete Ghost2;
		delete Ghost3;
		delete Ghost4;
	}

	void drawLife( RenderWindow *win)
	{		
		if (pac->pacLife < 0) { return; }
		Sprite spr;
		for (int i = 0; i < pac->pacLife; i++)
		{
			spr = pac->getLifeSpr();
			spr.setPosition( Vector2f(30*i, 24*30) );
			(*win).draw( spr );
		}
	}

	void setBlueGhost()
	{
		if (GameObject::pacIsDead) { return; }
		if (ghStatusThread.joinable()) { ghStatusThread.detach(); }		
		sleep(milliseconds(20));
		ghostStatus = Blue;
		creatGhostThr();
		gmSound->play(GameSound::PlSound::Blue);
		gmSound->stop(GameSound::PlSound::Siren);
	}

	void collidToPac()
	{		
		GameObject::pacIsDead = true;
		pac->pacLife--;
		isCollid = true;		
		stopAll();
		if (pac->pacLife < 0) 
		{			
			gameOver();
		}		
	}

	void gameOver()
	{				 		
		gameStatus = Demo;
		GameObject::score = 0;
		GameObject::level = 0;
		lifeWin = false;
		gameText->scoreTxt.setString("SCORE: 0");
		pac->stop();
		Ghost1->stop();
		Ghost2->stop();
		Ghost3->stop();
		Ghost4->stop();	
		resetPacGhost();
		resetLevel();			
	}
	
private:
	void loop( RenderWindow* win)
	{				
		win->setActive(true);		
		while (win->isOpen())
		{			
			if (GameObject::score >= 10000 && !lifeWin)
			{
				lifeWin = true;
				gmSound->play(GameSound::PlSound::WinnLife);
				pac->pacLife++;
			}
			win->clear();			
			if (gameStatus == Play)
			{				
				drawLife(win);
				maze.drawWall(win);
				win->draw(backSpr);
				win->draw(gameText->gameOverTxt);
				win->draw(gameText->scoreTxt);
				win->draw(gameText->levelTxt);
				if (ghostStatus == Blue)
				{
					string dif = to_string(curTime - time(0));
					if (stoi(dif) == 0) { dif = ""; }
					gameText->countTxt.setString(dif);
					win->draw(gameText->countTxt);
				}				
				if (gameText->bonusTxt.getString() != "")
				{
					win->draw(gameText->bonusTxt);
				}
				if(fruit->getVisible() ){ win->draw(fruit->getSrpite());}
			}		
			else
			{
				win->draw(gameText->authorTxt);
				win->draw(gameText->enterTxt);
				win->draw( Ghost1->getNameTxt());
				win->draw(Ghost2->getNameTxt());
				win->draw(Ghost3->getNameTxt());
				win->draw(Ghost4->getNameTxt());
			}
			
			win->draw(pac->getSprite());
			win->draw(Ghost1->getSprite());
			win->draw(Ghost2->getSprite());
			win->draw(Ghost3->getSprite());
			win->draw(Ghost4->getSprite());
			win->display();				
			
		}
	 }

	void changeGhostState()
	{
			   delay =  scaterInterval;
			   if (ghostStatus == Blue) 
			   {
				   delay = blueInterval; 
				   GameObject::isBlueGhost = true;
			   }	  
			   changeStatus();
			   while ( ghStatusThread.joinable() )
			   {				   
				   curTime = time(0);
				   curTime += delay;			 
				  
				   while (true && ghStatusThread.joinable() )
				   {
					   if (curTime <= time(0))  { break; }
				   }
				    
				   sleep(milliseconds(10));
				   if (ghostStatus == Blue)
				   {	
					 GameObject::isBlueGhost = false;
					 GameObject::ghostBonus = 100;
					 gmSound->stop(GameSound::PlSound::Blue);
					 if(GameObject::gameRun)gmSound->play(GameSound::PlSound::Siren);
				   }
				   if (ghostStatus == Attack)
				   {
					   ghostStatus = Scater;
					   delay = scaterInterval;
				   }
				   else 
				   {			 
					   ghostStatus =  Attack;
					   delay = attackInterval;
				   }
				   changeStatus();
				  
				   if (( maze.dotsCount - GameObject::dotsEat) < 30)
				   {
					   gmSound->setPich(GameSound::Siren, 1.2);
				   }
				 
			   }		   
	}

	void changeStatus()
	{
		Ghost1->changeGhostState();
		Ghost2->changeGhostState();
		Ghost3->changeGhostState();
		Ghost4->changeGhostState();
	}

	void stopAll()
	{					
		pac->stop();
		Ghost1->stop();
		Ghost2->stop();
		Ghost3->stop();
		Ghost4->stop();	
	    GameObject::gameRun = false;
		gameText->stopThread();
		fruit->stop();
		gmSound->stopAll();
		if (ghStatusThread.joinable() ){ ghStatusThread.detach(); }	
	    resetLevel();	
		wait(2);
	    if (pac->pacLife >= 0) { startLevel(); }				
	}

	void startGame()
	{				
		maze.redrawDot();
		gameText->gameOverTxt.setString("");
		pac->pacLife = 2;		
		gameStatus = Play;
		resetPacGhost();		
		intro = true;
		GameObject::gameRun = true;
		GameObject::level=1;
		GameObject::score=0;
		GameObject::dotsEat = 0;
		blueInterval = 6; 
		fruit->setLevel(GameObject::level);
		gmSound->setPich(GameSound::Siren, 1);
		startLevel();
	}

	void startLevel()
	{		 
		if (intro)
		{
			intro = false;		 
			gmSound->play(GameSound::PlSound::Intro);	
		}
		isCollid = false;
		ghostStatus = Scater;
		gmSound->play(GameSound::PlSound::Siren);		
	    GameObject::gameRun = true;
		resetPacGhost();		 
		pac->run();	
		creatGhostThr();
		fruit->start();
		gameText->levelTxt.setString("LEVEL: " + to_string(GameObject::level));
	}

	void nextLevel()
	{
		gmSound->setPich(GameSound::Siren, 1);
		toNextLevel = true;		
	    GameObject::dotsEat = 0;
		stopAll();	
		maze.redrawDot();
		GameObject::level++;		
		gameText->levelTxt.setString("LEVEL: " + to_string(GameObject::level));
		fruit->setLevel(GameObject::level);
		if (GameObject::level > 2 && GameObject::level < 6) { blueInterval = 4; }
		else if (GameObject::level >= 6 && GameObject::level < 10){blueInterval = 2; }
		else if (GameObject::level >= 10) { blueInterval = 0; }
	}

	void creatGhostThr()
	{	
		while (ghStatusThread.joinable()) {}
		ghStatusThread = thread(&Starter::changeGhostState, this); 
	}

	void resetLevel()
	{		
		GameObject::ghostBonus = 100;
		if(pac->pacLife<0)
		{		 
			gameText->gameOverTxt.setString("GAME OVER"); 
		}
	}

	void resetPacGhost()
	{
		pac->reset();
		Ghost1->reset();
		Ghost2->reset();
		Ghost3->reset();
		Ghost4->reset();		
	}

	void wait(int delayInt)
	{
		auto curTime = time(0);
		int counter=0;
		curTime += delayInt;		
		while (true)
		{
			counter++;
			if (toNextLevel)
			{		
				if (counter % 30 == 0)
				{
				 backSpr.setTexture(backFlashText);
				}
				else if (counter % 30 == 15)
				{
				 backSpr.setTexture(backText);
				}				
			}
			if (curTime < time(0)) { break; }
			sleep(milliseconds(10));
		}
		toNextLevel = false;
		backSpr.setTexture(backText);
	}
	
};