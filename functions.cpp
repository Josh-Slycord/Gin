#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <thread>
#include <algorithm>
#include "functions.h"

using namespace std;

void playGame(void)
{
		srand(time(NULL));//sets new random seed
		firstHand(); //deals 10 cards to each player and places top card in discard pile
		rounds = true;
		victory = false;
		gameOver = false;
		human.human = true;
		computer.human = false;
		checkCardCount(newCard);

		///////////////////////////////Debug

		
		while (rounds)
		{
			if (cardRemain > 3 && !victory)
			{
				human.setDeadwood(checkForGin(human));
				humansTurn();
				human.setDeadwood(checkForGin(human));
				if (!(human.getDeadwood()))
				{
					victory = true;
					winner = "Human Player ";
					human.gotGin(computer);
					break;
				}
				checkCardCount(newCard);
				computer.setDeadwood(checkForGin(computer));
				computersTurn();
				computer.setDeadwood(checkForGin(computer));
				if (!(computer.getDeadwood()))
				{
					victory = true;
					winner = "Computer Player ";
					human.gotGin(human);
					break;
				}
				checkCardCount(newCard);
			}
			else if (cardRemain <= 3 && !victory)
			{
				cout << "Round is a Draw! - Restarting" << endl;
				newCard = newDeck;
				human.clearHand();
				computer.clearHand();
				playGame();
			}
			else if (victory && !gameOver)
			{
				victory = false;
				cout << winner << "Wins! - Starting New Game" << endl;
				newCard = newDeck;
				human.clearHand();
				computer.clearHand();

				playGame();
			}
			else if (victory && gameOver)
			{
				cout << winner << "Wins The Game!" << endl;
				rounds = false;
			}

		}
		
	
		cout << "Press any key to exit:";
		cin;
}

void checkCardCount(Deck temp)
{
	vector<vector<bool>> deck = temp.getDeck();
	int numTrue = 0;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<13; j++)
		{
			if (deck[i][j])
				numTrue++;
		}
	}
	cardRemain = numTrue;
}

int checkForGin(Player temp)
	{
	//runs can be any length
	//check for sets that still leave runs intact
	//hand size 10
	// if first or last card of run is in set, put it in set if run >= 4 cards
	//if run is >= 7 cards, can use center card and end cards for sets
	// 7 cards = 4th card,
	// 8 cards = 4, 5th

	///////////////////////////////////////////////////////////////Variables

	vector<vector<bool>> hand = temp.getHand(); //get a copy of the hand from the current player
	vector<vector<bool>> deadwood = temp.getHand(); //get a copy of the hand from the current player to calculate deadwood
	
	/*
	/////////////////////////////////////////Debug
	vector<vector<bool>> hand;
	vector<vector<bool>> deadwood;

	hand.resize(4);
	for (int i = 0; i < 4; ++i) hand[i].resize(13);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			hand[i][j] = false;
		}
	}

	hand[2][8] = true;
	hand[2][9] = true;
	hand[2][10] = true;
	hand[2][11] = true;
	hand[2][12] = true;
	hand[2][7] = true;

	hand[1][7] = true;
	hand[3][7] = true;
	hand[1][4] = true;
	hand[3][5] = true;


	deadwood = hand;
	//////////////////////////////////////////Debug 
	*/

	vector<int> runsSuit;    //holds the suite of the run
	vector<int> runsLength; // holds the lengths of each run
	vector<int> runsEnd;   // holds the final value of each run
	vector<int> setValue; // holds the value of the card in a set
	vector<int> setSize; // holds the number of suits involved in the set

	//the vectors above will always be set in pairs: 
	//      EX) runsLength[1] and runsEnd[1] are about the same run
	//      EX) setValue[1] and setSize[1] are about the same set


	vector<int>::iterator itRuns; //used as iterator for run vectors
	vector<int>::iterator itSets; //used as iterator for set vectors

	int location = 0; //used as incrementing counters for loops
	int location2 = 0; //used as incrementing couters for loops
	int runLength = 1; //the default length of any run must always be at least 1
	int deadwoodTotal = 0;

	bool consecutive = false;  
	bool foundSets = false;
	bool foundRuns = false;

	///////////////////////////////////////////////////////////////Find Melds Including Conflicting Values
	try {
		for (int i = 0; i < 4; i++)
		{//this set of loops finds runs and fills the end location and the length into 2 vectors
			for (int j = 1; j < 13; j++)
			{
				if (hand[i][j - 1] && hand[i][j])
				{//if the one being checked and the last, then they must be consecutive
					consecutive = true;
					runLength++;//increase run length
					if (consecutive && (j == 12) && runLength >= 3)
					{
						runsLength.push_back(runLength); //puts the final length of the run in a vector
						runsEnd.push_back(j); //puts the previous number as the final value in the run
						runsSuit.push_back(i);
						foundRuns = true;
					}
				}
				else
				{//the one being check and the last are not consecutive
					consecutive = false;

					if ((runLength >= 3))
					{//if we get 3 or more consecutive numbers, and the next number is not consecutive, then we just exited a run
						runsLength.push_back(runLength); //puts the final length of the run in a vector
						runsEnd.push_back(j - 1); //puts the previous number as the final value in the run
						runsSuit.push_back(i);
						foundRuns = true;
					}
					runLength = 1; //reset the runLength counter to 1 to find the next run
				}

			}
			runLength = 1;
			consecutive = false;
		}

		for (int i = 0; i < 13; i++)
		{//this set of loops finds sets and fills the values and the size into 2 vectors
			if (hand[0][i] && hand[1][i] && hand[2][i] && hand[3][i])
			{//checks for 4 of a kind of value i
				setValue.push_back(i);
				setSize.push_back(4);
				foundSets = true;
			}
			else if (hand[0][i] && hand[1][i] && hand[2][i])
			{//checks for 3 of a kind of value i
				setValue.push_back(i);
				setSize.push_back(3);
				foundSets = true;
			}
			else if (hand[0][i] && hand[1][i] && hand[3][i])
			{//checks for 3 of a kind of value i
				setValue.push_back(i);
				setSize.push_back(2);
				foundSets = true;
			}
			else if (hand[0][i] && hand[2][i] && hand[3][i])
			{//checks for 3 of a kind of value i
				setValue.push_back(i);
				setSize.push_back(1);
				foundSets = true;
			}
			else if (hand[1][i] && hand[2][i] && hand[3][i])
			{//checks for 3 of a kind of value i
				setValue.push_back(i);
				setSize.push_back(0);
				foundSets = true;
			}
		}


		///////////////////////////////////////////////////////////////Correct Card Conflicts if they Exist
		//So here in the code, we have 4 vectors to use:
		// runsLength = length of run
		// runsEnd = location of last consecutive cards in run
		// setValue = location of card that is in the set
		// setSize = if it is a set of 3 or 4

		//To minimize deadwood scores, first we check if the last card in a run of 4 or greater is used in a set, and we place that card in the set
		//Then we check some specific cases were the central card can be used in a set, these cases are runs of 7, 8
		//for runs of 7, we can replace the 4th card with a set and create 2 runs
		//for runs of 8, we can replace the 4th or 5th card with a set and create 2 runs
		//then we check the first card it runs greater than 4 and see if they can be placed in a set, and place them there.
		//finaly we check for any remaining cards that are in sets and runs that did meet any of the criteria above, and remove the sets
		//By following these steps, we should always find what combination of cards in your hand create the lowest deadwood, which if it is 0, is gin!


		if ((foundSets) && (foundRuns))
		{

			location = 0;
			for (itRuns = runsLength.begin(); itRuns < runsLength.end(); itRuns++)
			{//this loops checks if a run of 4 or greater has a set that conflicts with the highest value card, and removes that card from the run

				if (runsLength[location] >= 4)
				{
					location2 = 0;
					for (itSets = setValue.begin(); itSets < setValue.end(); itSets++)
					{
						if (setValue[location2] == runsEnd[location])
						{
							runsEnd[location]--;
							runsLength[location]--;
						}
						location2++;
					}

				}
				location++;
			}

			location = 0;
			for (itRuns = runsLength.begin(); itRuns < runsLength.end(); itRuns++)
			{//this loop looks for cards used in sets and runs, and in some predefined cases it breaks the runs into 2 runs 
				bool breakLoop = false; //this is needed because this for loop actually modifies the topology of the vector, causing it to not work if it tries to iterate

				if (runsLength[location] == 7)
				{//if you have a run of length 7, then a set using the 4th card creates to 3 card runs, which is always the preferred case
					int center;
					center = runsEnd[location] - 3;
					location2 = 0;
					for (itSets = setValue.begin(); itSets < setValue.end(); itSets++)
					{
						if (center == setValue[location2])
						{
							runsLength[location] = 3;
							runsEnd[location] = setValue[location2] - 1;
							runsLength.push_back(3);
							runsEnd.push_back(center + 3);
							breakLoop = true;
						}
						location2++;
					}

				}
				if (runsLength[location] == 8)
				{//if you have a run of length 8, then a set using the 4th or 5th card creates 2 runs, which is a preferred case.
					int center;
					int center2;
					center = runsEnd[location] - 4;
					center2 = runsEnd[location] - 3;
					location2 = 0;
					for (itSets = setValue.begin(); itSets < setValue.end(); itSets++)
					{
						if (center == setValue[location2])
						{
							runsLength[location] = 3;
							runsEnd[location] = setValue[location2] - 1;
							runsLength.push_back(4);
							runsEnd.push_back(center + 4);
							breakLoop = true;
						}
						if (center2 == setValue[location2])
						{
							runsLength[location] = 4;
							runsEnd[location] = setValue[location2] - 1;
							runsLength.push_back(3);
							runsEnd.push_back(center2 + 3);
							breakLoop = true;
						}
						location2++;
					}
				}
				location++;
				if (breakLoop)
				{
					break;
				}
			}

			location = 0;
			for (itRuns = runsLength.begin(); itRuns < runsLength.end(); itRuns++)
			{//this loops checks if a run of 4 or greater has a set that conflicts with the lowest value card, and removes that card from the run

				if (runsLength[location] >= 4)
				{
					location2 = 0;
					for (itSets = setValue.begin(); itSets < setValue.end(); itSets++)
					{
						if (setValue[location2] == (runsEnd[location] - runsLength[location]+1))
						{
							runsLength[location]--;
						}
						location2++;
					}

				}
				location++;
			}

			location = 0;
			for (itRuns = runsLength.begin(); itRuns < runsLength.end(); itRuns++)
			{
				bool exitLoop = false;
				location2 = 0;
				for (itSets = setValue.begin(); itSets < setValue.end(); itSets++)
				{
					int firstInRun = runsEnd[location] - runsLength[location];
					for (int range = runsEnd[location]; range > firstInRun; range--)
					{
						if (range == setValue[location2])
						{
							if ((runsSuit[location] != setSize[location2]) && (setSize[location2] != 4))
							{

								setValue.erase(setValue.begin() + location2);
								setSize.erase(setSize.begin() + location2);
								exitLoop = true;

							}
							else if (setSize[location2] == 4)
							{
								setSize[location2] = runsSuit[location];
							}

						}
						if (exitLoop)
						{
							break;
						}
					}
					location2++;
					if (exitLoop)
					{
						break;
					}
				}
				location++;
			}
		}
		///////////////////////////////////////////////////////////////Removes runs and sets from deadwood vector


		if (foundSets)
		{
			//the value in setSize corresponds to one of the comments below, which determines which suits the cards were found in
			//this value is corresponds to the suit that is NOT in the set, except for 4 of a kind, which is 4 
			//4 = 4 of a kind
			//3 = 0, 1, 2  
			//2 = 0, 1, 3
			//1 = 0, 2, 3
			//0 = 1, 2, 3

			for (int i = 0; i < 4; i++)
			{
				location = 0;
				for (itSets = setValue.begin(); itSets < setValue.end(); itSets++)
				{
					if (hand[i][setValue[location]])
					{
						if (setSize[location] == 4)
						{
							deadwood[0][setValue[location]] = false;
							deadwood[1][setValue[location]] = false;
							deadwood[2][setValue[location]] = false;
							deadwood[3][setValue[location]] = false;
						}
						if (setSize[location] == 3)
						{
							deadwood[0][setValue[location]] = false;
							deadwood[1][setValue[location]] = false;
							deadwood[2][setValue[location]] = false;
						}
						if (setSize[location] == 2)
						{
							deadwood[0][setValue[location]] = false;
							deadwood[1][setValue[location]] = false;
							deadwood[3][setValue[location]] = false;
						}
						if (setSize[location] == 1)
						{
							deadwood[0][setValue[location]] = false;
							deadwood[2][setValue[location]] = false;
							deadwood[3][setValue[location]] = false;
						}
						if (setSize[location] == 0)
						{
							deadwood[1][setValue[location]] = false;
							deadwood[2][setValue[location]] = false;
							deadwood[3][setValue[location]] = false;
						}
					}
					location++;
				}
			}
		}

		if (foundRuns)
		{
			for (int i = 0; i < 4; i++)
			{
				location = 0;
				for (itRuns = runsLength.begin(); itRuns < runsLength.end(); itRuns++)
				{
					if (hand[i][runsEnd[location]])
					{
						for (int j = runsLength[location]; j > 0; j--)
						{
							if (runsSuit[location] == i)
								deadwood[i][runsEnd[location] - j+1] = false;
						}
					}
					location++;
				}
			}
		}
		///////////////////////////////////////////////////////////////Calculate deadwood by adding values of cards remaining in deadwood vector

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				if (deadwood[i][j])
				{
					if (j <= 8)
					{
						deadwoodTotal = deadwoodTotal + j + 1;
					}
					else
					{
						deadwoodTotal += 10;
					}
				}

			}
		}
	}
	catch (exception hand)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				cout << "[" << i << "]" << "[" << j << "]" << endl;
			}
		}
	}

	return deadwoodTotal;

	}

void humansTurn()
	{//humans players UI and logic
	 
		int deckChoice;
		int discardChoice;
		int validLoop = 1;
		Location temp;

		cout << "Enter X at any time to exit game." << endl;
		cout << "Players Turn! Current Scores: Human: " << human.getScore() << " Computer: " << computer.getScore() << endl;
		cout << "Current Deadwood " << human.getDeadwood() << " Cards Remaining: " << cardRemain << " Top of Discard Pile: ";
			print(human, 2); //prints card on top of discard pile
			cout << "Players Hand: " << endl;
			print(human, 1); //prints players hand
			cout << "Which deck do you want to draw from? 1)Face Down, 2) Face Up: ";
			cin >> deckChoice; // selects deck to draw from
			cout << endl; //
			while (validLoop)//Loop for determing what to do based on which deck is being drawn from
			{
				if (deckChoice == 2) //face up
				{
					human.getCardFromDeck(discards.getLastCard()); //get face up card from discard pile, place in hand
					print(human, 5);  //prints the card that was just drawn 
					validLoop = 0;  //exits loop
				}
				else if (deckChoice == 1) //face down
				{

					human.getCardFromDeck(newCard.drawFromDeck()); //get next face down card in deck, place in hand
					print(human, 4); //prints the card that was just drawn 
					validLoop = 0;  //exits loop
				}
				else if (deckChoice == 88)
				{
					rounds = false;
					validLoop = 0;
				}
				else //try again
				{
					cout << "Invalid Selection, try again: 1) Face Down, 2) Face Up: ";
					cin >> deckChoice; //keeps asking till the user inputs a 1 or a 2
				}
			}
			print(human, 3); //prints the players hand + the new card (11 total cards) and numbers them to choose which to discard
			cout << "Choose card to discard: ";
			cin >> discardChoice;//should be between 0-10


			validLoop = 1;
			while (validLoop)//loop to make sure proper input is put for dicard
			{
				if (discardChoice <= 10) validLoop = 0; //if input is 10 or below, exit loop
				else if (discardChoice == 88)
				{
					rounds = false;
					validLoop = 0;
				}
				else 
				{
					cout << "Invalid Selection, try again: "; //keep asking until proper input is used
					cin >> discardChoice; //between 0-10
				}
			}

			cout << endl;
			temp = human.discardFromHand(human, discardChoice); //gets the location for the card to be discarded
			human.discard(temp); //sets that location to false in the the players hand
			discards.addToDeck(temp); //sets the location to true in the discard deck

			return;
	}

void computersTurn()
	{//for now acts as a player 2
	int deckChoice;
	int discardChoice = 0;
	int lowestDeadwood;
	int currentDeadwood;
	Location temp;
	Player tempComputer;
	vector<vector<bool>>hand;

	cout << "Computers Turn! Current Scores: Human: " << human.getScore() << " Computer: " << computer.getScore() << endl;
	cout << "Cards Remaining: " << cardRemain << " Top of Discard Pile: ";
	print(computer, 2); //prints card on top of discard pile
	cout << endl;


	tempComputer = computer;

	temp = discards.getLastCard();

	tempComputer.getCardFromDeck(temp);
	currentDeadwood = checkForGin(tempComputer);
	if (currentDeadwood < computer.getDeadwood()) deckChoice = 2;
	else deckChoice = 1;
	
	if (deckChoice == 2) //face up
	{
		computer.getCardFromDeck(discards.getLastCard()); //get face up card from discard pile, place in hand
	}
	 if (deckChoice == 1) //face down
	{
		computer.getCardFromDeck(newCard.drawFromDeck()); //get next face down card in deck, place in hand
	}

	 tempComputer = computer;
	 lowestDeadwood = checkForGin(computer);
	
	 int defaultChoice = true;
	 int finalDiscardChoice;
	 for (int i = 0; i < 4; i++)
	 {
		 for (int j = 0; j < 13; j++)
		 {
			 hand = tempComputer.getHand();
			 if (hand[i][j])
			 {
				 temp.suit = i;
				 temp.value = j;
				 tempComputer.discard(temp);
				 currentDeadwood = checkForGin(tempComputer);
				 if(currentDeadwood < lowestDeadwood)
				 {
					 defaultChoice = false;
					 lowestDeadwood = currentDeadwood;
					 finalDiscardChoice = discardChoice;
				 }
				 tempComputer.getCardFromDeck(temp);
				 discardChoice++;
			 }
	   	 }
	 }

	 if (defaultChoice)
	 {
		 finalDiscardChoice = 10;
	 }

	temp = computer.discardFromHand(computer, finalDiscardChoice); //gets the location for the card to be discarded
	computer.discard(temp); //sets that location to false in the the Computers hand
	discards.addToDeck(temp); //sets the location to true in the discard deck
	
	return;
		
	}

void firstHand()
	{//used for setting up the first hand of a new game

		for (int i = 0; i < 10; i++)
		{  //deals 10 cards alternating to each player
			human.getCardFromDeck(newCard.drawFromDeck());
			computer.getCardFromDeck(newCard.drawFromDeck());;
		}
		discards.addToDeck(newCard.drawFromDeck());  //adds a card to the discard pile
	}

void print(Player temp, int choice)
	{
	//this function is a mess, try not to make changes to any thing that is here, just add a new command if needed.

	//Choice 1 = prints the players hand in ordered by suit by value
	//Choice 2 = prints the top card of the discard pile
	//Choice 3 = prints players hand with a number to left for selecting dicard: 0), 1), 2) ...... 10)
	//Choice 4 = prints last card drawn from face down deck deck
	//Choice 5 = prints last card drawn from dicard pile

		/////////////////////////////////////////
		suitMap[0] = "Clubs";
		suitMap[1] = "Diamonds";
		suitMap[2] = "Hearts";
		suitMap[3] = "Spades";
		/////////////////////////////////////////
		valueMap[0] = "Ace";
		valueMap[1] = "2";
		valueMap[2] = "3";
		valueMap[3] = "4";
		valueMap[4] = "5";
		valueMap[5] = "6";
		valueMap[6] = "7";
		valueMap[7] = "8";
		valueMap[8] = "9";
		valueMap[9] = "10";
		valueMap[10] = "Jack";
		valueMap[11] = "Queen";
		valueMap[12] = "King";
		////////////////////////////////////////

		if (choice == 1)
		{//prints players hand
		vector<vector<bool>>hand = temp.getHand();
		vector<vector<bool>>handPairOrganized;
		handPairOrganized.resize(13);
		for (int i = 0; i < 13; ++i) handPairOrganized[i].resize(4);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				handPairOrganized[j][i] = hand[i][j];
			}
		}

		int first = 1;
		int second = 0;
		int test1 = 1;
		int test2 = 1;
		int break1 = 0;
		int break2 = 0;
		int i = 0;
		int j = 0;
		int k = 0;
		int l = 0;
		while (!((k == 13) && (i == 4)))
		{
			while (first)
			{
				for (i; i<4; i+=0)
				{
					for (j; j<13; j++)
					{
						if (hand[i][j])
						{
							first = 0;
							second = 1;
							cout << setw(6) << right << valueMap.find(j)->second << setw(4) << " of " << setw(9) << left << suitMap.find(i)->second;
							break1 = 1;
							j++;
							break;
						}
						

						if (break1) break;
					}
					if (j == 13)
					{
						j = 0;
						i++;
					}

					if (break1)	break;

				}
				if (break1) break;
				first = 0;
				second = 1;
			}

			while (second)
			{
				for (k; k<13; k+=0)
				{
					for (l; l<4; l++)
					{
						if (handPairOrganized[k][l])
						{
							first = 1;
							second = 0;
							cout << setw(6) << right << valueMap.find(k)->second << setw(4) << " of " << setw(9) << left << suitMap.find(l)->second << endl;

							break2 = 1;
						
							l++;
							break;
						}

						if (break2)	break;
					}
					if (l == 4)
					{
						l = 0;
						k++;
					}
					if (break2) break;

				}
				if (break2)	break;
				second = 0;
				first = 1;
			}

			break1 = 0;
			break2 = 0;

		}

		}  //prints players hand 

		if (choice == 2)
		{//prints the top of dicard pile
			Location temp;
			temp = discards.getLastCard();
			cout << valueMap.find(temp.value)->second << " of " << suitMap.find(temp.suit)->second << endl;
		}

		if (choice == 3)
		{//prints the players hand with numbers beside: 1), 2), 3) .... 10) for choice of which card to discard
			int selection = 0;
			vector<vector<bool>>hand = temp.getHand();
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 13; j++)
				{
					if (hand[i][j])
					{
						cout << selection << ") " << valueMap.find(j)->second << " of " << suitMap.find(i)->second << endl;
						selection++;

					}

				}
			}
		}

		if (choice == 4)
		{//prints last card drawn from face down pile
			Location temp;
			temp = newCard.getLastCard();

			cout << "Card Drawn: " << valueMap.find(temp.value)-> second << " of " << suitMap.find(temp.suit)->second << endl;

		}

		if (choice == 5)
		{//prints last card drawn from discard pile
			Location temp;
			temp = discards.getLastCard();

			cout << "Card Drawn: " << valueMap.find(temp.value)->second << " of " << suitMap.find(temp.suit)->second << endl;

		}
	}
