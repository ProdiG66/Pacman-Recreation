#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include "maze.h"
#include "gameSound.h"
enum GhostStatus
{
	Attack,
	Scater,
	Blue,
	Death
}ghostStatus;

enum GameStatus
{
	Demo,Play
}gameStatus;


 
class GameObject 
{
public:
	enum GhostStatus
	{
		Attack,
		Scater,
		Blue,
		Death
	}thisGhostStatus;

	static int winW,winH;	
	static sf::Vector2f *pacPosition;
	static sf::Vector2f blinkyPosition;
	static bool pacIsDead;
	static bool isBlueGhost;
	static int ghostBonus;
	static int score;
	static int level;
	static int dotsEat;
	static bool gameRun;
	sf::Vector2f pacPos;
	GameSound gameSound;
	int sprW = 0, sprH = 0, sprOfsX = 0, sprOfsY = 0;
	int demoPos[5][2] = { {6,11}, {6,6},{6,7},{6,8},{6,9} };
	int modulX, modulY;	
	int dotBonus=10;
	float moveX = 0, moveY = 0;
	float speed=0;
	bool stopMove = false;
	bool isInHome;
	sf::Vector2f *targetPos;
	sf::Vector2f edgePos;
	sf::VideoMode videoMode;
	int delayLoop = 15;
	string Name;
	int Id;	  		
	Maze *maze;
	 

	GameObject()
	{
		maze = Maze::instance;		 
		winW = videoMode.getDesktopMode().width;
		winH = videoMode.getDesktopMode().height;
	}

	//---
	bool virtual checkMazeGrid(int x, int y)
	{
		string str = maze->mazeGrid[x][y];
		return str == '#' || str=="-";
	}

	//---
	void changeGhostState()
	{			
		switch (ghostStatus)
		{
		case Attack:		
			thisGhostStatus = Attack;
			break;
		case Scater:
			thisGhostStatus = Scater;
			break;
		case Blue:
			thisGhostStatus = Blue;
			break;
		case Death:
			thisGhostStatus = Death;
			break;
		}

		if (Id == 1 && thisGhostStatus == Scater)// for Blinky
		{		 
			if ((maze->dotsCount - GameObject::dotsEat) < 30)
			{
				thisGhostStatus = Attack;				 
			}		  
		}

		if ( thisGhostStatus == Attack) 
		{		  	
			targetPos = GameObject::pacPosition; 
		}
		else if (thisGhostStatus == Scater || thisGhostStatus==Blue) { targetPos = &edgePos; }
		
		if (thisGhostStatus != Blue) 
		{
			sprOfsY = 30 + sprW * (Id - 1);
		}
		if (isInHome && GameObject::gameRun ) { run(); }
	}
	string getMazeStr(int x, int y)
	{
		return maze->mazeGrid[x][y];		 
	}
	bool virtual checkWall(sf::Vector2f sprPos)
	{
		return (sprPos.x) + sprW / 2 >= GameObject::winW ||
			   (sprPos.y) + sprH/2 >= GameObject::winH ||
			   (sprPos.x) < (sprW/2) ||
			   (sprPos.y) < (sprH/2);		
	}


	 
	//---
	void run()
	{		
		stopMove = false;			
	}

	//---
	void stop()
	{
		moveX = moveY = 0;
		stopMove = true;
	}
	void endGame()
	{

	}
};
 
int GameObject::winW=800; 
int GameObject::winH=800; 
int GameObject::ghostBonus = 100;
int GameObject::score=0;
int GameObject::level = 0;
int GameObject::dotsEat = 0;
bool GameObject::pacIsDead = false;
bool GameObject::isBlueGhost = false;
bool GameObject::gameRun = false;
sf::Vector2f *GameObject::pacPosition=0;
sf::Vector2f GameObject::blinkyPosition;
