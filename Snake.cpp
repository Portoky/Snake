// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <ctime>
#include <cstdlib>
#include <time.h>
using namespace std;
using namespace std::chrono_literals;

struct sSnakeSegment
{
	int x, y;
};
int start = clock();
double diff = 0; 
int masod = 7; //7 masodpercenkent eldonti, hogy tegyen-e le pokot vagy se
int pok = 0; // 1 tegyen le pokot, kulonben ne (0 < pok < 6)
int ScreenWidth = 120; //x of screen	
int ScreenHeight = 45; //y of screen
int FieldWidth = 20;
int FieldHeight = 15;
unsigned char *pField = NULL;
int main()
{
	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	for (int i = 0; i < ScreenHeight * ScreenWidth; ++i)
		screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytesWritten = 0;
	SetConsoleActiveScreenBuffer(hConsole);

	/*pField = new unsigned char[ScreenWidth * ScreenHeight];
	for(int x = 0; x < FieldWidth; ++x)
		for(int y = 0; y < FieldHeight; ++y)
			pField[y * FieldWidth + x] = (x == 0 || x == FieldWidth - 1 || y == FieldHeight - 1) ? 9 : 0;*/

	list<sSnakeSegment> snake = { {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15} };
	int FoodX = 40;
	int FoodY = 15; 
	int Score = 0;
	int SnakeDirection = 3;
	bool Dead = false;
	bool KeyLeft = false, KeyRight = false, KeyUp = false, KeyDown = false;
	int coordx = 0, coordxBefore = 0;
	int coordy = 0, coordyBefore = 0;
	string KeyBefore = " ";
	int pokX = 0, pokY = 0;

	//mfw game loop
	srand(time(0));
	while (!Dead)
	{
		
		diff = ((clock() - start) / (double)(CLOCKS_PER_SEC));//!!! nem jo
		if (diff >= masod)
		{
			start = clock();
			if (pokX == 0)
			{
				pok = 1 + (rand() % 6);
				diff = 0;
				if (pok == 1)//lesz pok
				{
					pokX = 1 + (rand() % 118);
					pokY = 5 + (rand() % 34);
				}
			}
			else
			{
				diff = 0;
				pokY = pokX = 0;
			}
		}
		//Timing && input
		this_thread::sleep_for(75ms); //Game Tick
		
		//Game Logic

		if(GetAsyncKeyState(VK_LEFT) & 0x0001 && KeyBefore != "KeyRight")
		{
			KeyLeft = true;
			KeyRight = false;
			KeyUp = false;
			KeyDown = false;
		}
 		if (GetAsyncKeyState(VK_RIGHT) & 0x0001 && KeyBefore != "KeyLeft")
		{
			KeyLeft = false;
			KeyRight = true;
			KeyUp = false;
			KeyDown = false;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001 && KeyBefore != "KeyUp")
		{
			KeyLeft = false;
			KeyRight = false;
			KeyUp = false;
			KeyDown = true;
		}
		if (GetAsyncKeyState(VK_UP) & 0x0001 && KeyBefore != "KeyDown")
		{
			KeyLeft = false;
			KeyRight = false;
			KeyUp = true;
			KeyDown = false;
		}
		list <sSnakeSegment> ::iterator it = snake.begin();

		if (KeyLeft && KeyBefore != "KeyRight")
		{

			if (screen[(*it).y * ScreenWidth + (*it).x - 1] == L'%')
			{	
				++Score;
				snake.push_back({ (*it).y * ScreenWidth +(*it).x - 1, (*it).y }); // ezeknel nem tudom miert nem siman csak (*it)x, (*it).y), de
				FoodX = 1 + (rand() % 118);										  // igy is mukodik, sot smoothabb is igy
				FoodY = 5 + (rand() % 34);
			}
			else if (screen[(*it).y * ScreenWidth + (*it).x - 1] == L'#')
			{
				Score += 3;
				snake.push_back({ (*it).y * ScreenWidth + (*it).x - 1, (*it).y });
				diff = 0;
				pokY = pokX = 0;
			}
			else if (screen[(*it).y * ScreenWidth + (*it).x - 1] != L' ')
				Dead = true;
			coordx = (*it).x;
			coordy = (*it).y;
			--(*it).x;
			++it;
			for (; it != snake.end(); ++it)
			{
				coordxBefore = (*it).x;
				coordyBefore = (*it).y;
				(*it).x = coordx;
				(*it).y = coordy;
				coordx = coordxBefore;
				coordy = coordyBefore;
			}
			KeyBefore = "KeyLeft";
		}
		else if (KeyRight && KeyBefore != "KeyLeft")
		{
			if (screen[(*it).y * ScreenWidth + (*it).x + 1] == L'%')
			{
				++Score;
				snake.push_back({ (*it).y * ScreenWidth + (*it).x + 1, (*it).y });
				FoodX = 1 + (rand() % 118);
				FoodY = 5 + (rand() % 34);
			}
			else if (screen[(*it).y * ScreenWidth + (*it).x + 1] == L'#')
			{
				Score += 3;
				snake.push_back({ (*it).y * ScreenWidth + (*it).x + 1, (*it).y });
				diff = 0;
				pokY = pokX = 0;
			}
			else if (screen[(*it).y * ScreenWidth + (*it).x + 1] != L' ')
				Dead = true;
			coordx = (*it).x;
			coordy = (*it).y;
			++(*it).x;
			++it;
			for (; it != snake.end(); ++it)
			{
				coordxBefore = (*it).x;
				coordyBefore = (*it).y;
				(*it).x = coordx;
				(*it).y = coordy;
				coordx = coordxBefore;
				coordy = coordyBefore;
			}
			KeyBefore = "KeyRight";
		}
		else if (KeyUp && KeyBefore != "KeyDown")
		{
			if (screen[((*it).y - 1) * ScreenWidth + (*it).x] == L'%')
			{
				++Score;
				snake.push_back({ ((*it).y + 1) * ScreenWidth + (*it).x, (*it).y });
				FoodX = 1 + (rand() % 118);
				FoodY = 5 + (rand() % 34);
			}
			else if (screen[((*it).y - 1) * ScreenWidth + (*it).x] == L'#')
			{
				Score += 3;
				snake.push_back({ ((*it).y + 1) * ScreenWidth + (*it).x, (*it).y });
				diff = 0;
				pokY = pokX = 0;
			}
			else if (screen[((*it).y - 1) * ScreenWidth + (*it).x] != L' ')
				Dead = true;
			coordx = (*it).x;
			coordy = (*it).y;
			--(*it).y;
			++it;
			for (; it != snake.end(); ++it)
			{
				coordxBefore = (*it).x;
				coordyBefore = (*it).y;
				(*it).x = coordx;
				(*it).y = coordy;
				coordx = coordxBefore;
				coordy = coordyBefore;
			}
			KeyBefore = "KeyUp";
		}
		else if (KeyDown && KeyBefore != "KeyUp")
		{
			if (screen[((*it).y + 1) * ScreenWidth + (*it).x] == L'%')
			{
				++Score;
				snake.push_back({ ((*it).y - 1) * ScreenWidth + (*it).x, (*it).y });
				FoodX = 1 + (rand() % 118);
				FoodY = 5 + (rand() % 34);
			}
			else if (screen[((*it).y + 1) * ScreenWidth + (*it).x] == L'#')
			{
				Score += 3;
				snake.push_back({((*it).y - 1) * ScreenWidth + (*it).x, (*it).y });
				diff = 0;
				pokY = pokX = 0;
			}
			else if (screen[((*it).y + 1) * ScreenWidth + (*it).x] != L' ')
				Dead = true;
			coordx = (*it).x;
			coordy = (*it).y;
			++(*it).y;
			++it;
			for (; it != snake.end(); ++it)
			{
				coordxBefore = (*it).x;
				coordyBefore = (*it).y;
				(*it).x = coordx;
				(*it).y = coordy;
				coordx = coordxBefore;
				coordy = coordyBefore;
			}
			KeyBefore = "KeyDown";
		}
		//Display to player
		//Clear screen
		for (int i = 0; i < ScreenHeight * ScreenWidth; ++i)
			screen[i] = L' ';
		//Draw stats and border
		for (int i = 0; i < ScreenWidth; ++i)
		{
			screen[i] = L'=';
			screen[2 * ScreenWidth + i] = L'=';
			screen[42 * ScreenWidth + i] = L'=';
		}
		for (int i = 0; i < ScreenWidth; i += 119)
		{
			for (int j = 3; j < ScreenHeight; ++j)
			{
				screen[j * ScreenWidth + i] = L'|';
			}
		}
		wsprintf(&screen[ScreenWidth + 5], L" SCORE : %d", Score);
		wsprintf(&screen[ScreenWidth + 30], L"Made by JanosAkosMarton");

		//Draw Snake
		for (auto i : snake) // ilyenkor az iterator pointer
		{
			screen[i.y * ScreenWidth + i.x] = Dead ? L'+' : L'O';
		}
		//Draw head of the snake
		screen[snake.front().y * ScreenWidth + snake.front().x] = Dead ? L'X' : L'@';
		//Draw Food
		screen[FoodY * ScreenWidth + FoodX] = L'%';
		//Draw Snake
		if (pokX != 0)
			screen[pokY * ScreenWidth + pokX] = L'#';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	while(1)
	WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth* ScreenHeight, { 0,0 }, & dwBytesWritten);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
