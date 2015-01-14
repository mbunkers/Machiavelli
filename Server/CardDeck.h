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
#include <vector>
#include "Card.h"
#include <memory>

using namespace std;

template <typename T>
class CardDeck {
private:
    unique_ptr<vector<shared_ptr<Card>>> mCards;
public:
    CardDeck<T>(){
    }

    ~CardDeck(){

    }

    CardDeck(const CardDeck& other){

    }

    CardDeck<T>& operator=(const CardDeck& other){

    }

    CardDeck(CardDeck&&){

    }

    void addCard(shared_ptr<Card> card){
        mCards->push_back(card);
    }
};

#endif /* defined(__Machiavelli__CardDeck__) */
