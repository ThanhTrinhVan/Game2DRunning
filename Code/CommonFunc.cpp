#include "pch.h"
#include "CommonFunc.h"

float PLAYER_SPEED_X = 5.0;
int Score = 0;
int Blood = LENGTH_BLOOD_MAX;

bool SDLCommonFunc::checkCollision(const SDL_Rect & object1, const SDL_Rect & object2)
{
	int left_a = object1.x;
	int right_a = object1.x + object1.w;
	int top_a = object1.y;
	int bottom_a = object1.y + object1.h;

	int left_b = object2.x;
	int right_b = object2.x + object2.w;
	int top_b = object2.y;
	int bottom_b = object2.y + object2.h;

	// Case 1: size object 1 < size object 2
	if (left_a > left_b && left_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (left_a > left_b && left_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	// Case 2: size object 1 < size object 2
	if (left_b > left_a && left_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (left_b > left_a && left_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	// Case 3: size object 1 = size object 2
	if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
	{
		return true;
	}

	return false;
}

bool SDLCommonFunc::checkFocus(const int & x, const int & y, const SDL_Rect & rect)
{
	if (x >= rect.x && x < (rect.x + rect.w) &&
		y >= rect.y && y < (rect.y + rect.h))
		return true;
	return false;
}

TypeMenu SDLCommonFunc::checkType(std::string txt)
{
	if (txt == "Play" || txt == "Play Again")
		return TypeMenu::Play;
	else if (txt == "Instruction")
		return TypeMenu::Instruction;
	else if (txt == "Documentation")
		return TypeMenu::Documentation;
	else if (txt == "Exit")
		return TypeMenu::Exit;
	else if (txt == "Go to StartMenu")
		return TypeMenu::GoToStart;
	else if (txt == "Continue")
		return TypeMenu::Continue;
	else if (txt == "Beginner")
		return TypeMenu::Beginner;
	else if (txt == "Professional")
		return TypeMenu::Professional;
	else if (txt == "Level")
		return TypeMenu::Level;
	else
		return TypeMenu::Exit;
	return TypeMenu::Exit;
}

