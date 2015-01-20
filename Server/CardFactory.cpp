//
//  CardFactory.cpp
//  Machiavelli
//
//  Created by Marc Bunkers on 20/01/15.
//  Copyright (c) 2015 Marc Bunkers & Leon van Tuijl. All rights reserved.
//

#include "CardFactory.h"

CardFactory::CardFactory(){

}

shared_ptr<Card> CardFactory::createCard(string line){
    vector<string> data = splittedString(line, ';');
    if (data.at(0) == "Building"){
        return createBuildingCard(data);
    }
    if (data.at(0) == "Character"){
        return createCharacterCard(data);
    }
    return nullptr;
}

shared_ptr<Card> CardFactory::createBuildingCard(vector<string> data){
    string name;
    int value;
    enum CardColor color = UNKNOWN;
    string flavorText = "";
    int specialFunction = 0;

    name = data.at(1);
    value = atoi(data.at(2).c_str());
    if (data.size() > 3){
        color = colorForString(data.at(3));
    }
    if (data.size() > 4){
        flavorText = data.at(4);
    }
    if (data.size() > 5){
        specialFunction = atoi(data.at(5).c_str());
    }

    return make_shared<BuildingCard>(name, value, color, flavorText, specialFunction);
}

shared_ptr<Card> CardFactory::createCharacterCard(vector<string> data){
    string name;
    int priority;
    enum CardColor color = UNKNOWN;

    name = data.at(1);
    priority = atoi(data.at(2).c_str());
    if (data.size() > 3){
        color = colorForString(data.at(3));
    }

    if (name == "Moordenaar"){
        return createAssassinCard(name, priority, color);
    }
    if (name == "Dief"){
        return createThiefCard(name, priority, color);
    }
    if (name == "Magier"){
        return createMagicianCard(name, priority, color);
    }
    if (name == "Koning"){
        return createKingCard(name, priority, color);
    }
    if (name == "Prediker"){
        return createPreacherCard(name, priority, color);
    }
    if (name == "Koopman"){
        return createMerchantCard(name, priority, color);
    }
    if (name == "Bouwmeester"){
        return createArchitectCard(name, priority, color);
    }
    if (name == "Condottiere"){
        return createCondottiereCard(name, priority, color);
    }

    throw exception();
    return nullptr;
}

shared_ptr<Card> CardFactory::createAssassinCard(string name, int priority, CardColor color){
    return make_shared<Assassin>(name, priority, color);
}

shared_ptr<Card> CardFactory::createThiefCard(string name, int priority, CardColor color){
    return make_shared<Thief>(name, priority, color);
}

shared_ptr<Card> CardFactory::createMagicianCard(string name, int priority, CardColor color){
    return make_shared<Magician>(name, priority, color);
}

shared_ptr<Card> CardFactory::createKingCard(string name, int priority, CardColor color){
    return make_shared<King>(name, priority, color);
}

shared_ptr<Card> CardFactory::createPreacherCard(string name, int priority, CardColor color){
    return make_shared<Preacher>(name, priority, color);
}

shared_ptr<Card> CardFactory::createMerchantCard(string name, int priority, CardColor color){
    return make_shared<Merchant>(name, priority, color);
}

shared_ptr<Card> CardFactory::createArchitectCard(string name, int priority, CardColor color){
    return make_shared<Architect>(name, priority, color);
}

shared_ptr<Card> CardFactory::createCondottiereCard(string name, int priority, CardColor color){
    return make_shared<Condottiere>(name, priority, color);
}

vector<string> CardFactory::splittedString(const string line, char delim){
    vector<string> elems;
    stringstream ss(line);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

CardColor CardFactory::colorForString(string stringColor){
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
