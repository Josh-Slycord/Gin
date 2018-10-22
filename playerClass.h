
#ifndef PLAYERCLASS_H

#define PLAYERCLASS_H

using namespace std;

class Player
	{
	public:
		Player();
		void discard(Location temp);  //sets the card at location bool value to 0
		Location discardFromHand(Player, int); //gets the location of a card to be discarded from user input during the discard stage
		void getCardFromDeck(Location);  //gets a new card from deck
		const vector<vector<bool>>& getHand() const { return hand; } //returns a pointer to players current hand
		void setDeadwood(int deadwoodTotal) { deadwood = deadwoodTotal; }
		int getDeadwood(void) { return deadwood; }
		void clearHand();
		bool human;
		void gotGin(Player temp) { score = score + 25 + temp.getDeadwood(); }
		int getScore() { return score; }

	private:
		vector<vector<bool>> hand; //a 4x13 matrix that 
		int deadwood;
		int score;
	};

#endif