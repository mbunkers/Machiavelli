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

using namespace std;

template <typename T>
class CardDeck {
private:
    vector<shared_ptr<Card>> mCards;
public:
    unique_ptr<T> make_unique(string name){
        return (unique_ptr<T>(new T(name)));
    }

    CardDeck<T>(string name){
        if (loadCards(name)){
            
        }
    }

    ~CardDeck(){

    }

    CardDeck(const CardDeck& other){

    }

    CardDeck<T>& operator=(const CardDeck& other){

    }

    CardDeck(CardDeck&&){

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
            while (cardsFile.good()){
                getline(cardsFile, line);
                if (line == ""){
                    break;
                }
                if (name == "Bouwkaarten.csv"){
                    shared_ptr<BuildingCard> card = createBuildingCard(line);
                    addCard(card);
                }
                if (name == "Karakterkaarten.csv"){
                    shared_ptr<CharacterCard> card = createCharacterCard(line);
                    addCard(card);
                }
            }
            cardsFile.close();
        }
        else{
            return false;
        }
        return true;
    }

    shared_ptr<BuildingCard> createBuildingCard(string line){
        string name;
        int value;
        enum CardColor color = UNKNOWN;
		string flavorText = "";
		int specialFunction = 0;

        vector<string> data = splittedString(line, ';');

        name = data.at(0);
        value = atoi(data.at(1).c_str());
        if (data.size() > 2){
            color = colorForString(data.at(2));
        }
		if (data.size() > 3){
			flavorText = data.at(3);
		}
		if (data.size() > 4){
			specialFunction = atoi(data.at(4).c_str());
		}

        return make_shared<BuildingCard>(name, value, color, flavorText, specialFunction);
    }

    shared_ptr<CharacterCard> createCharacterCard(string line){
        string name;
        int priority;
        enum CardColor color = UNKNOWN;

        vector<string> data = splittedString(line, ';');

        priority = atoi(data.at(0).c_str());
        name = data.at(1);
        if (data.size() > 2){
            color = colorForString(data.at(2));
        }

        return make_shared<CharacterCard>(name, priority, color);
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

    CardColor colorForString(string stringColor){
        if (stringColor == "geel"){
            return YELLOW;
        }
        if (stringColor == "blauw"){
            return BLUE;
        }
        if (stringColor == "lila"){
            return LILA;
        }
        if (stringColor == "groen"){
            return GREEN;
        }
        if (stringColor == "rood"){
            return RED;
        }
        return UNKNOWN;
    }

    void shuffle(){
		srand(static_cast<int>(time(0)));
        random_shuffle ( mCards.begin(), mCards.end());
    }
};

#endif /* defined(__Machiavelli__CardDeck__) */
