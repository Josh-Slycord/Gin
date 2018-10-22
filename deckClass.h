
#ifndef DECKCLASS_H

#define DECKCLASS_H

using namespace std;

class Deck
	{
	//used to create a face up and a face down deck, named newCard (facedown) and discards (faceup)
	public: 
		Deck(bool fill);  //deck consturctor, fill sets all values to true or false: IE) all cards start in face down so 1, and no cards in discard 0
		Location drawFromDeck();  //picks a card that is in the deck and stores that location in a struct
		void addToDeck(Location);  //adds a card back to a deck (only used with discards) via a struct location
		Location getLastCard() { return lastCard; };  //keeps track of last card added to deck (only needed with discards) to track which face up card is available
		const vector<vector<bool>>& getDeck() const { return cards; } //returns a pointer to players current hand


	private:

		Location lastCard;  //remembers last card added to the deck IE) the face up card in the discard pile
		vector<vector<bool>> cards;  //resized in constructor to be of size cards[4][13], the first variable keeps track of suit, the second keeps track of value, stores these slots as true/false
	
	};

#endif