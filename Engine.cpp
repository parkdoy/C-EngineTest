#include <iostream>
#include <fstream>
#include <conio.h>
#include <algorithm>

#include "World.h"
#include "Engine.h"
#include "Wall.h"
#include "Player.h"
#include "Goal.h"
#include "Floor.h"
#include "Monster.h"


int Engine::KeyCode = 0;

Engine* Engine::Instance = nullptr;

Engine::Engine()
{
	Instance = this;
	Initilize();
}

Engine::~Engine()
{
	Terminate();
}

void Engine::Initilize()
{
	bRunning = true;
	MyWorld = new World();
}

void Engine::Load(string MapFilename)
{
	//Load
	ifstream MapFile(MapFilename);

	int Y = 0;
	while (MapFile.peek() != EOF)
	{
		char Buffer[1024] = { 0, };
		MapFile.getline(Buffer, 1024);

		for (size_t X = 0; X < strlen(Buffer); ++X)
		{
			char Cursor = Buffer[X];
			switch (Cursor)
			{
			case '#':
				MyWorld->SpawnActor(new AWall((int)X, Y, '#', true));
				break;
			case 'P':
				MyWorld->SpawnActor(new APlayer((int)X, Y, 'P', true));
				break;
			case 'G':
				MyWorld->SpawnActor(new AGoal((int)X, Y, 'G', false));
				break;
			case 'M':
				MyWorld->SpawnActor(new AMonster((int)X, Y, 'M', false));
				break;
			}


			MyWorld->SpawnActor(new AFloor((int)X, Y, ' ', false));
		}


		Y++;
	}


	//그리는 순서를 변경
	sort(MyWorld->MyActors.begin(), MyWorld->MyActors.end(), AActor::Compare);

	MapFile.close();
}

void Engine::Run()
{
	//Run
	while (bRunning) //1 Frame
	{
		Input();
		MyWorld->Tick();
		MyWorld->Render();
	}
}

void Engine::Terminate()
{
	delete MyWorld;
	MyWorld = nullptr;
}

void Engine::Input()
{
	Engine::KeyCode = _getch();
}


