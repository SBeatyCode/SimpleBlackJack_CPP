// BlackJack.cpp by Stephen Beaty - created as a learning project

#include <iostream>
#include <conio.h> //needed for _getch()
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <string>


//A Class that defines a 'Card' object, the Cards that you need to play the game
class Card
{
public:
	std::string cardName; //The name of the card, such as '3 of Clubs'
	int cardValue; //What is the card's point value? 1-13
	const char* cardSuite; //What suite (Hearts, Clubs, ect) is this Card?

	Card(std::string name, int value, const char* suite)
		: cardName(name), cardValue(value), cardSuite(suite) {};
};

//Defines a 'CardHolder' object, something that can hld ontp Card objects. Used for the Player, and the Dealer
class CardHolder
{
private:
	std::vector<Card> cardsHeld; //The CardHolder's hand of cards - which cards they currently have

public:
	//Returns the Value of the Held Card at a given Index
	int GetValueOfHeldCard(int position)
	{
		return cardsHeld[position].cardValue;
	}

	//Returns the value of all of this Cardholder's held cards
	int GetTotalHeldCardValue()
	{
		int totalCardValue = 0;

		for (Card playingCard : cardsHeld)
			totalCardValue += playingCard.cardValue;

		return totalCardValue;
	}

	//Returns the Name of the Held Card at the given position
	std::string GetNameOfHeldCard(int position)
	{
		return cardsHeld[position].cardName;
	}

	//Adds a new Card to the CardHolder's Hand, the cardsHeld vector
	void AddCardToHand(Card cardToAdd)
	{
		cardsHeld.push_back(cardToAdd);
	}
};

//Method that creates a Deck of Cards as a std::vector
std::vector<Card> CreateCardDeck(std::vector<Card> cardDeck)
{
	//Define a Map that gives a value for the Face Cards
	std::map<char, int> faceCardValueMap;
	faceCardValueMap['A'] = 1;
	faceCardValueMap['J'] = 11;
	faceCardValueMap['Q'] = 12;
	faceCardValueMap['K'] = 13;

	//Define Card Suites and Face Values
	const char* suites[4] = { "Diamonds", "Spades", "Clubs", "Hearts" };
	char cardValues[13] = { 'A', 2, 3, 4, 5, 6, 7, 8, 9, 10, 'J', 'Q', 'K' };

	//Fill vector with cards
	for (int i = 0; i < 13; i++) //Loops through each number
	{
		for (int j = 0; j < 4; j++) //Loops through each Suite
		{
			int cardValue;
			const char* cardSuite;

			cardSuite = suites[j];
			std::string cardName;

			//Check if cardValue is A, J, Q, or K, and if so get the value of that FaceCard
			if (faceCardValueMap.contains(cardValues[i]))
				cardValue = faceCardValueMap[cardValues[i]];
			else
				cardValue = cardValues[i];

			//std::string cardName = cardValue;
			// 
			char valueChar = cardValue;
			//std::string 
			cardName = std::to_string(cardValues[i]); //std::to_string(valueChar);// valueChar;
			cardName += " of ";
			cardName += cardSuite;

			Card newCard = Card(cardName, cardValue, cardSuite);
			cardDeck.push_back(newCard);
		}
	}
	return cardDeck;
}

//Method that shuffle the Card Deck - randomize the order of the cardDeck std::Vector
void ShuffleDeck(std::vector<Card>& cardDeck)
{
	auto randomSeed = std::random_device{};
	auto rng = std::default_random_engine{ randomSeed() };
	std::shuffle(cardDeck.begin(), cardDeck.end(), rng);
}

//Draws a Card from the std::vector cardDeck, and returns it
Card DrawCardFromDeck(std::vector<Card>& cardDeck)
{
	Card drawnCard = (cardDeck[cardDeck.size() -1]);
	cardDeck.pop_back();

	return drawnCard;
}

//Checks if the Player has won by having 21 points, if so returns TRUE. Also shows the point total of the dealer
bool PlayerHas21Points(int playerPointTotal, CardHolder dealer)
{
	if (playerPointTotal == 21)
	{
		std::cout << "You point total is " << playerPointTotal << ", you have exactly 21! You Won!\n";
		std::cout << "The Dealer had a point total of " << dealer.GetTotalHeldCardValue() << " points.\n";
		std::cout << "Please play again sometime!" << std::endl;
		return true;
	}
	else return false;
}

//Checks if the Player has lost/bust by having over 21 points, if so returns TRUE. Also shows the point total of the dealer
bool PlayerHasBust(int playerPointTotal, CardHolder dealer)
{
	if (playerPointTotal > 21)
	{
		std::cout << "You point total is " << playerPointTotal << ", which is over 21!\nYou BUST\n";
		std::cout << "The Dealer had a point total of " << dealer.GetTotalHeldCardValue() << " points.\n";
		std::cout << "Game Over. Please try again!" << std::endl;
		return true;
	}
	else return false;
}

//The Main Game Loop. Returns False if it does NOT need to be run again and the game should end, returns True if it should be run again.
//Needs CardHolder objects to represent the Player and the Dealer, as well as the std::vector<Card> cardDeck
bool MainGameLoop(CardHolder& player, CardHolder& dealer, std::vector<Card>& cardDeck)
{
	int playerPointTotal = player.GetTotalHeldCardValue();

	//If User's Cards are perfectally 21, they instantly win
	if (PlayerHas21Points(playerPointTotal, dealer))
		return false;
	//If User's hand is over 21, Game Over
	else if (PlayerHasBust(playerPointTotal, dealer))
		return false;
	//If under 21, prompt Player if they would like to Draw or end the game, and compare their cards to the dealer's
	else
	{
		std::cout << "Your current total is " << playerPointTotal << ". Would you like to draw a card, or finish the game?\n";
		std::cout << "Press the 'd' key to draw a card.\nPress the 'f' key to finish drawing, and compare your cards to the Dealer's.\n" << std::endl;

		bool WaitingForPlayerInput = true;

		//Prompt the Player to press the 'd' or the 'f' key - other inputs will just restart this loop until one is selected
		while (WaitingForPlayerInput)
		{
			char playerInput = _getch();

			if (playerInput == 'd')
			{
				Card newCard = DrawCardFromDeck(cardDeck);
				player.AddCardToHand(newCard);
				std::cout << "You drew " << newCard.cardName << ", which is worth " << newCard.cardValue << " points.\n" << std::endl;
				WaitingForPlayerInput = false;
			}
			else if (playerInput == 'f')
			{
				std::cout << "Ok, let's compare your card total to the Dealers' card total.\n";
				std::cout << "Your current total is " << playerPointTotal << " points.\nPress any key to continue.";
				_getch();
				std::cout << "The Dealer's second card is " << dealer.GetNameOfHeldCard(1) << ", which is worth " << dealer.GetValueOfHeldCard(1) << " points.\n";
				std::cout << "The Dealer had a point total of " << dealer.GetTotalHeldCardValue() << " points.\n";

				//If User's Cards are perfectally 21, they instantly win
				if (PlayerHas21Points(playerPointTotal, dealer))
					return false;
				//If User's hand is over 21, Game Over
				else if (PlayerHasBust(playerPointTotal, dealer))
					return false;
				//If Player has more points than the Dealer
				else if (playerPointTotal <= 21 && playerPointTotal > dealer.GetTotalHeldCardValue())
				{
					std::cout << "You beat the dealer! Congratulations, you won! Please play again some time!" << std::endl;
				}
				//If the Dealer had more points, but they bust
				else if (dealer.GetTotalHeldCardValue() > 21)
				{
					std::cout << "The dealer BUST! Congratulations, you won! Please play again some time!" << std::endl;
				}
				//If the Dealer has more points than the Player
				else
				{
					std::cout << "You were beat by the dealer! Sorry, you lose this time! Please try again some time!" << std::endl;
				}

				WaitingForPlayerInput = false;
				return false;
			}
			else
			{
				std::cout << "You did not select to either draw, or to finish the game. Please select from one of the following options:\n";
				std::cout << "Press the 'd' key to draw a card.\nPress the 'f' key to finish drawing, and compare your cards to the Dealer's.\n" << std::endl;
			}
		}
	}
	return true;
}

int main()
{
	//Generate Card Deck
	std::vector<Card> cardDeck = CreateCardDeck(cardDeck);

	//shuffle the cards
	ShuffleDeck(cardDeck);

	//Make Objects to represent the Player and the Dealer
	CardHolder player;
	CardHolder dealer;

	/*
	for (Card playingCard : cardDeck)
	{
		std::cout << playingCard.cardName << "\n";
	}
	*/

	//Greet User and explain the basic rules
	std::cout << "Welcome to Blackjack!\n" << "Try to get to 21 points without going over, and try to beat the Dealer's hand!\n";
	std::cout << "Number cards are all worth their face-value. For example, a 2 of Hearts is worth 2 points.\n";
	std::cout << "A Jack is 11\nA Queen is 12\nA King is 13\nAn Ace is 1\n" << "\n";
	std::cout << "Ties go to the Player, so if the Player gets 21 points at any time, they win the game!\n" << std::endl;
	std::cout << "Press any key to begin" << std::endl;

	//After getting user input, begin the game
	_getch();
	std::cout << "\n\n\nLet's begin!\nDealing cards to Dealer . . .\n" << std::endl;

	//Shuffle the Card Deck again to be sure they're nice and random
	ShuffleDeck(cardDeck);

	//Draw two cards for the Dealer
	dealer.AddCardToHand(DrawCardFromDeck(cardDeck));
	dealer.AddCardToHand(DrawCardFromDeck(cardDeck));

	//Draw first two cards for the Player
	std::cout << "Dealing cards to Player . . .\n" << std::endl;
	player.AddCardToHand(DrawCardFromDeck(cardDeck));
	player.AddCardToHand(DrawCardFromDeck(cardDeck));

	std::cout << "The Player and the Dealer have each been dealt 2 cards.\n";

	//Show 1 of Dealer's Cards and give its value
	std::cout << "The Dealer's first card is '" << dealer.GetNameOfHeldCard(0) << "' which is worth ";
	std::cout << dealer.GetValueOfHeldCard(0) << " points.\n" << std::endl;

	//Show each of the Player's Cards, and give the combined Value
	std::cout << "The Player's first card is '" << player.GetNameOfHeldCard(0) << "' which is worth ";
	std::cout << player.GetValueOfHeldCard(0) << " points." << std::endl;
	std::cout << "The Player's second card is '" << player.GetNameOfHeldCard(1) << "' which is worth ";
	std::cout << player.GetValueOfHeldCard(1) << " points." << std::endl;
	std::cout << "This means the Player's hand is worth " << player.GetTotalHeldCardValue() << " points total.\n" << std::endl;

	//Wait for Player input to continue
	std::cout << "Press any key to continue. . ." << std::endl;
	_getch();


	bool playerIsPlaying = true;

	//Run the Main Game Loop untill something causes it to end
	while (playerIsPlaying)
	{
		playerIsPlaying = MainGameLoop(player, dealer, cardDeck);
	}

	return 0;
};