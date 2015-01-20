//
//  CardDeck.h
//  Machiavelli
//
//  Created by Marc Bunkers on 17/12/14.
//  Copyright (c) 2014 Marc Bunkers. All rights reserved.
//

#ifndef __Machiavelli__CardDeck__
#define __Machiavelli__CardDeck__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Card.h"
#include <memory>
#include <string>
#include "BuildingCard.h"
#include "CharacterCard.h"
#include <algorithm>
#include <time.h>

#include "CardFactory.h"

using namespace std;

template <typename T>
class CardDeck {
private:
    vector<shared_ptr<Card>> mCards;
    bool mIsLoaded;
public:
    unique_ptr<T> make_unique(string name){
        return (unique_ptr<T>(new T(name)));
    }

    CardDeck<T>(string name){
        mIsLoaded = loadCards(name);
    }

    ~CardDeck(){

    }

    CardDeck(const CardDeck& other){

    }

    CardDeck<T>& operator=(const CardDeck& other){

    }

    CardDeck(CardDeck&&){

    }

    bool isLoaded(){
        return mIsLoaded;
    }

    void resetCharacters(){
		for (size_t i = 0; i < mCards.size(); i++){
            shared_ptr<CharacterCard> card = card = static_pointer_cast<CharacterCard>(mCards.at(1));
            card->setIsTaken(false);
            card->setOwner(nullptr);
        }
    }

    bool allCardsHaveOwners(){
		for (size_t i = 0; i < mCards.size(); i++){
            shared_ptr<CharacterCard> card = card = static_pointer_cast<CharacterCard>(mCards.at(1));
            if (!card->hasOwner()){
                return false;
            }
        }
        return true;
    }

    bool allCardsTaken(){
        int takenCards = 0;
        for (size_t i = 0; i < mCards.size(); i++){
            shared_ptr<CharacterCard> card = static_pointer_cast<CharacterCard>(mCards.at(i));
            if (card->isTaken()){
                takenCards++;
            }
        }
        return takenCards == mCards.size();
    }

    vector<shared_ptr<Card>> allCards(){
        return mCards;
    }

    shared_ptr<Card> cardAtIndex(size_t index){
        shared_ptr<Card> card = mCards.at(index);
        return card;
    }

    void addCard(shared_ptr<Card> card){
        mCards.push_back(card);
    }

    size_t size(){
        return mCards.size();
    }

    void removeCard(shared_ptr<Card> card){
        auto it = std::find(mCards.begin(), mCards.end(), card);
        if(it != mCards.end())
            mCards.erase(it);
    }

    shared_ptr<Card> drawCard(){
        shared_ptr<Card> card = mCards.at(mCards.size() - 1);
        mCards.pop_back();
        return card;
    }

    bool loadCards(string name){
        string line;
        ifstream cardsFile(name);
        if (cardsFile.is_open()){
            CardFactory factory = CardFactory();
            while (cardsFile.good()){
                getline(cardsFile, line);
                if (line == ""){
                    break;
                }
                shared_ptr<Card> card = factory.createCard(line);
                addCard(card);
            }
            cardsFile.close();
        }
        else{
            return false;
        }
        return true;
    }

    vector<string> splittedString(const string line, char delim){
        vector<string> elems;
        stringstream ss(line);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    void shuffle(){
		srand(static_cast<int>(time(0)));
        random_shuffle ( mCards.begin(), mCards.end());
    }
};

#endif /* defined(__Machiavelli__CardDeck__) */
