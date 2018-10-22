
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <thread>
#include <algorithm>
#include "playerClass.h"

using namespace std;

Player::Player()
	{//functions just like the deck constructor, except it only fills value to 0 since the player will always start with an empty hand
			hand.resize(4);
			for (int i = 0; i < 4; ++i) hand[i].resize(13);
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 13; j++)
				{
					hand[i][j] = false;
				}
			}
	}
	
void Player::clearHand()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			hand[i][j] = false;
		}
	}
}

void Player::getCardFromDeck(Location temp)
	{//sets a location given by temp to true, which means it is now in the players hand
		hand[temp.suit][temp.value] = true;
	}

void Player::discard(Location temp)
	{//This function sets the card at the location to false 
		hand[temp.suit][temp.value] = false;
	}
	
Location Player::discardFromHand(Player temp, int discard)
{//this function is used when a player choices which card to discard from thier hand after drawing an 11th card. The int discard is the number which is input from the user
	Location tempLocation;
	int selection = 0;
	vector<vector<bool>>hand = temp.getHand();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (hand[i][j])
			{
				if (selection == discard)
				{
					tempLocation.suit = i;
					tempLocation.value = j;
				}
				selection++;
			}
		}
	}
	return tempLocation;
}