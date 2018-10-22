
#ifndef FUNCTIONS_H

#define FUNCTIONS_H

Deck newCard(true);
Deck discards(false);
Deck newDeck(true);

Player human;
Player computer;
Player emptyHand;

map<int, string> suitMap;
map<int, string> valueMap;

string winner;
bool gameOver;
bool rounds;
bool victory;
int cardRemain;

struct Location
{
	//Used throughout program, stores a location via suit (0-3) and value (0-12)
	// Suit 0, Clubs: Suit 1, Diamonds: Suit 2, Hearts: Suit 3, Spades; Alphabetical Order to have better functionality with map function if needed
	int suit;  //stores a suit number
	int value;  //stores a vard value
};


int checkForGin(Player);
void computersTurn(void);
void humansTurn(void);
void firstHand(void);
void print(Player, int);
void checkCardCount(Deck);
void playGame(void);



#endif