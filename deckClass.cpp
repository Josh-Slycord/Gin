#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <thread>
#include <algorithm>
#include "deckClass.h"

using namespace std;
	
Deck::Deck(bool fill)
	{
	//this function fills the cards double vector with all of whatever variable is passed as fill
		cards.resize(4);
		for (int i = 0; i < 4; ++i) cards[i].resize(13);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				cards[i][j] = fill;
			}
		}
	}

void Deck::addToDeck(Location temp)
	{//sets a card at the location given to true, so it is in the deck
		cards[temp.suit][temp.value] = true;
		lastCard = temp;
	}
	
Location Deck::drawFromDeck()
	{//draws a card at random from the remaining cards in the deck
		int randomNumber = rand();
		lastCard.suit = randomNumber % 4;
		lastCard.value = randomNumber % 13;
		if (cards[lastCard.suit][lastCard.value])
		{
			cards[lastCard.suit][lastCard.value] = false;
			return lastCard;
		}
		else drawFromDeck();
	}
	