/**********************************************
* CardSet.cpp - Contains implementation of class CardSet
* Shannon Archer, 4220912, sja998, 9/9/12
***********************************************/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "CardSet.h"
using namespace std;

int cMaxCards = 150;

// CONSTRUCTOR
CardSet::CardSet()
{
	Card = new int[cMaxCards];
	nCards = 0;
}

CardSet::CardSet(int n)
{
	Card = new int[cMaxCards];
	nCards = n;

	for (int i = 0; i < n && i < cMaxCards; i++)
	{
		Card[i] = i % 52;
	}
}

// DESTRUCTOR
CardSet::~CardSet()
{
	delete [] Card;
}

// SIZE GETTER
int CardSet::Size() const
{
	return nCards;
}

// RETURN TRUE IF EMPTY
bool CardSet::IsEmpty() const
{
	return (nCards <= 0);
}

// SHUFFLE THE CARDS RANDOMLY
void CardSet::Shuffle()
{
    srand(time(NULL));

	for (int i = 0; i < nCards; i++)
	{
		int j = (rand() % nCards);

		int t = Card[i];
		Card[i] = Card[j];
		Card[j] = t;
	}
}

// DEAL OUT A CARD
int CardSet::Deal()
{
	if (nCards <= 0)
	{
		cerr << "Not enough cards exist to complete this deal." << endl;
		exit(1);
	}

	int t = *Card;

    for (int i = 1; i < nCards; i++)
        Card[i - 1] = Card[i];
	nCards--;

	delete &Card[nCards];

	return t;
}

// DEAL OUT TO OTHER CARD SETS
void CardSet::Deal(int n, CardSet &set1, CardSet &set2)
{
	if (nCards - n*2 < 0)
	{
		cerr << "Not enough cards exist to complete this deal." << endl;
		return;
	}

	for (int i = 0; i < n; i++)
	{
		set1.AddCard(Deal());
		set2.AddCard(Deal());
	}
}

// DEAL OUT TO FOUR OTHER CARD SETS
void CardSet::Deal(int n, CardSet &set1, CardSet &set2, CardSet &set3, CardSet &set4)
{
	if (nCards - n*4 < 0)
	{
		cerr << "Not enough cards exist to complete this deal." << endl;
		return;
	}

	for (int i = 0; i < n; i++)
	{
		set1.AddCard(Deal());
		set2.AddCard(Deal());
		set3.AddCard(Deal());
		set4.AddCard(Deal());
	}
}

// ADD CARD TO CURRENT HAND
void CardSet::AddCard(int n)
{
	nCards++;

	Card[nCards-1] = n;
}

// MERGE SHUFFLE
void CardSet::MergeShuffle(CardSet &set1)
{
	int *newCards = new int[cMaxCards];
	int newSize = Size() + set1.Size();

	for (int i = 0; i < newSize; i++)
	{
		if (!IsEmpty())
			newCards[i] = Deal();
		if (!set1.IsEmpty())
			newCards[++i] = set1.Deal();
	}

	delete [] Card;
	Card = newCards;
	nCards = newSize;
}

// PRINT SET FIVE CARDS TO A LINE
void CardSet::Print() const
{
	for (int i = 0; i < nCards;)
	{
	    for (int j = 0; j < 5 && i < nCards; j++, i++)
	    {
            PrintCard(Card[i]);
            cout << ' ';
	    }
	    cout << endl;
	}
}




// Private function to print out usual representation of playing card.
// Input is integer from 0 to 51.  There is no error checking.

void CardSet::PrintCard(int c) const
{
	int Rank = c%13;
	int Suit = c/13;
	const char NameSuit[5] = "SCDH";
	const char NameRank[14] = "23456789XJQKA";
	cout << NameRank[Rank] << NameSuit[Suit];
}
