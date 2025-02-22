//
//  main.cpp
//  Dominion randomizer #2
//
//  Created by Ola Eklund on 2020-04-27.
//  Copyright � 2020 Ola Eklund. All rights reserved.
//

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <fstream>

using namespace std;

struct card {
    string sorting = "";
    string expansion = "";
    string types = "";
    string name = "";
    int cost = 0;
    int actions = 0;
    int cards = 0;
    int coins = 0;
    int buys = 0;
    bool special = false;
    bool potion = false;
    int debt = 0;
};

const int nrOfVariables = 8; //number of variables represented by either a number or a bool for each card
const int expansionsOwned = 7; //number of expansions owned
const int nrOfCards = 10; //number of different cards used each game
const array<string, expansionsOwned> expansionsNamed = { "Dominion", "Intrigue", "Dark Ages", "Alchemy", "Adventures", "Empires", "Nocturne"}; //For naming the expansions


void setList(array<vector<card>, expansionsOwned>& expansions);
// Creates a complete list of all cards owned
// expansions (array(vector(card))) : the list created

void randomExpansions(vector<int>& expansionsUsed);
//For random choice of the expansions that are used
//expansionsUsed (vector (int)) : the list of expansions chosen

void chosenExpansions(vector<int>& expansionsUsed, array<string, expansionsOwned> expansionsNamed);
//For manual choice of the expansions that are used
//expansionsUsed (vector (int)) : the list of expansions chosen
//expansionsNamed (array (string)) : the list of the names of expansions owned

void manualRandomExpansions(vector<int>& expansionsUsed, array<string, expansionsOwned> expansionsNamed);
//For more manual random choice of the expansions that are used
//expansionsUsed (vector (int)) : the list of expansions chosen
//expansionsNamed (array (string)) : the list of the names of expansions owned

void randomCards(array<card, nrOfCards>& cardsUsed, vector<int>& expansionsUsed, array<vector<card>, expansionsOwned>& expansions);
//For random choice of the cards used
//cardsUsed (array(card)) : the list of cards chosen
//expansionsUsed (vector(int)) : the list of expansions chosen between
//expansions (array(vector(card))) : the list of all cards owned

void manualRandomCards(array<card, nrOfCards>& cardsUsed, vector<int>& expansionsUsed, array<vector<card>, expansionsOwned>& expansions);
//For more manual random choice of the cards used
//cardsUsed (array(card)) : the list of cards chosen
//expansionsUsed (vector(int)) : the list of expansions chosen between
//expansions (array(vector(card))) : the list of all cards owned

void sortCards(array<card, nrOfCards>& cardsUsed); //Legacy, not used anymore
//For sorting the list of cards
//cardsUsed (array(card)) : the list of cards used

bool condition(card one, card two);
//The condition for the sorting function

int main() {
    vector<card> Dominion, Adventures, Empires, Intrigue, Alchemy, DarkAges, Nocturne;
    array<vector<card>, expansionsOwned> expansions = { Dominion, Intrigue, DarkAges, Alchemy, Adventures, Empires, Nocturne };
    setList(expansions); // creates the list of cards owned
    int choice;
    char YN;
    vector<int> expansionsUsed;
    array<card, nrOfCards> cardsUsed;
    bool reserveCheck = false, debtCheck = false, VPCheck = false, potionCheck = false, buyCheck = false, actionCheck = false, ruinsCheck = false, spoilsCheck = false, sheltersCheck = false, boonsCheck = false, hexesCheck = false, spiritsCheck = false, wishCheck = false;
    srand(time(0));

    cout << "How do you want to choose versions?" << endl << "1 for completely random" << endl << "2 for completely manual" << endl << "3 for advanced randomization" << endl;
    cin >> choice;

    while (choice < 1 || choice > 3) {
        cout << "Invalid choice. Choose again." << endl << "How do you want to choose versions?" << endl << "1 for completely random" << endl << "2 for completely manual" << endl << "3 for advanced randomization" << endl;
        cin >> choice;
    }

    if (choice == 1) {
        randomExpansions(expansionsUsed);
    }
    else if (choice == 2) {
        chosenExpansions(expansionsUsed, expansionsNamed);
    }
    else if (choice == 3) {
        manualRandomExpansions(expansionsUsed, expansionsNamed);
    }

    sort(expansionsUsed.begin(), expansionsUsed.end());

    cout << endl << "The versions used are:" << endl;
    for (int i = 0; i < expansionsUsed.size(); i++) {
        cout << "-" << expansionsNamed[expansionsUsed[i]] << endl;
    }

    cout << endl << "How do you want to choose cards?" << endl << "1 for completely random" << endl << "2 for advanced randomization" << endl;
    cin >> choice;

    while (choice < 1 || choice > 2) {
        cout << "Invalid choice. Choose again." << endl << "How do you want to choose cards?" << endl << "1 for completely random" << endl << "2 for advanced randomization" << endl;
        cin >> choice;
    }

    if (choice == 1)
    {
        randomCards(cardsUsed, expansionsUsed, expansions);
    }

    else if (choice == 2)
    {
        manualRandomCards(cardsUsed, expansionsUsed, expansions);
    }

    if (cardsUsed[(nrOfCards - 1)].expansion == "Dark Ages")
    {
        sheltersCheck = true;
    }
    //sortCards(cardsUsed);
    sort(cardsUsed.begin(), cardsUsed.end(), condition);

    for (int i = 0; i < nrOfCards; i++)//checking for each extra thing needed
    {
        if (cardsUsed[i].types.find("Reserve") != string::npos || cardsUsed[i].name == "Miser" || cardsUsed[i].name == "Peasant + upgrades")
        {
            reserveCheck = true;
        }

        if (cardsUsed[i].actions > 1 || cardsUsed[i].name.find("Page") != string::npos || cardsUsed[i].name == "Throne Room")
        {
            actionCheck = true;
        }

        if (cardsUsed[i].debt > 0 || cardsUsed[i].name == "Capital" || cardsUsed[i].name.find("Fortune") != string::npos)
        {
            debtCheck = true;
        }

        if (cardsUsed[i].name == "Chariot Race" || cardsUsed[i].name.find("Plunder") != string::npos || cardsUsed[i].name == "Farmer's Market" || cardsUsed[i].name == "Groundskeeper" || cardsUsed[i].name.find("Emporium") != string::npos || cardsUsed[i].name == "Sacrifice" || cardsUsed[i].name == "Temple" || cardsUsed[i].name == "Wild Hunt" || cardsUsed[i].name == "Castles")
        {
            VPCheck = true;
        }

        if (cardsUsed[i].buys > 0)
        {
            buyCheck = true;
        }

        if (cardsUsed[i].potion)
        {
            potionCheck = true;
        }

        if (cardsUsed[i].types.find("Looter") != string::npos) 
        {
            ruinsCheck = true;
        }

        if (cardsUsed[i].name == "Pillage" || cardsUsed[i].name == "Marauder" || cardsUsed[i].name == "Bandit Camp")
        {
            spoilsCheck = true;
        }

        if (cardsUsed[i].types.find("Fate") != string::npos)
        {
            boonsCheck = true;
            spiritsCheck = true;
        }
        else if (cardsUsed[i].name == "Devil's Workshop" || cardsUsed[i].name == "Tormentor" || cardsUsed[i].name == "Cemetery" || cardsUsed[i].name == "Exorcist")
        {
            spiritsCheck = true;
        }

        if (cardsUsed[i].types.find("Doom") != string::npos)
        {
            hexesCheck = true;
        }

        if (cardsUsed[i].name == "Leprechaun" || cardsUsed[i].name == "Secret Cave")
        {
            wishCheck = true;
        }
    }


    cout << endl << "The cards used are:" << endl;
    for (int i = 0; i < nrOfCards; i++)
    {
        cout << cardsUsed[i].expansion << ": " << cardsUsed[i].name << endl;
    }

    cout << endl << "You will need:" << endl;
    if (reserveCheck)
    {
        cout << "-Tavern Mat" << endl;
    }
    if (debtCheck)
    {
        cout << "-Debt counters" << endl;
    }
    if (VPCheck)
    {
        cout << "-Victory Point counters" << endl;
    }
    if (potionCheck)
    {
        cout << "-Potion cards" << endl;
    }
    if (ruinsCheck)
    {
        cout << "-Ruins cards" << endl;
    }
    if (spoilsCheck)
    {
        cout << "-Spoils cards" << endl;
    }
    if (boonsCheck)
    {
        cout << "-Boons" << endl;
    }
    if (hexesCheck)
    {
        cout << "-Hexes" << endl;
    }
    if (spiritsCheck)
    {
        cout << "-Spirit cards" << endl;
    }
    if (wishCheck)
    {
        cout << "-Wish cards" << endl;
    }

    if (sheltersCheck)
    {
        cout << endl << "You should play with Shelters." << endl;
    }

    cout << endl << "Do you want to randomize who goes first? (y/n)" << endl;
    cin >> YN;
    if (YN == 'y' || YN == 'Y')
    {
        cout << "How many players are you?" << endl;
        cin >> choice;
        cout << "The player to begin is player " << (1 + rand() % choice) << "." << endl;
    }

    cout << endl << "Enter anything when you are done." << endl;
    cin >> choice;
}

void setList(array<vector<card>, expansionsOwned>& expansions)
{
    /* //Legacy, not used anymore
    vector<string> AdventuresTypes = { "Action, Duration", "Action", "Action, Duration, Attack", "Action, Duration, Reaction",  "Treasure, Reserve", "Action, Reserve, Victory", "Action, Duration", "Action, Reserve", "Action, Duration", "Action, Attack", "Action, Reserve", "Action, Duration, Attack", "Action, Duration", "Action", "Action", "Action", "Action", "Action", "Action", "Action, Reserve", "Action", "Treasure, Attack", "Action, Reserve", "Action", "Action, Duration, Attack", "Action, Reserve", "Treasure", "Action, Reserve", "Action, Traveller", "Action, Traveller" }, AlchemyTypes = { "Action", "Action", "Action", "Action, Attack", "Action", "Action", "Treasure", "Action", "Action, Attack", "Action", "Action", "Victory" }, DarkAgesTypes = { "Action", "Action", "Action, Command", "Action", "Action, Reaction", "Action", "Action", "Treasure", "Action, Attack, Looter", "Action, Looter", "Victory", "Action", "Action", "Action", "Action", "Action", "Action", "Action", "Action, Attack, Looter", "Action, Reaction", "Action", "Action, Attack", "Action", "Action", "Action", "Action", "Action, Attack", "Action", "Action", "Action", "Action", "Action, Attack", "Action", "Action", "Action, Attack, Knight" }, DominionTypes = { "Action", "Action, Attack", "Action", "Action", "Action", "Action", "Action", "Action", "Action", "Action", "Action, Attack", "Action", "Action, Reaction", "Action", "Action", "Action", "Action, Attack", "Action, Attack", "Action", "Action", "Action, Attack", "Action", "Action", "Victory" }, EmpiresTypes = { "Action, Duration", "Treasure", "Action, Attack", "Action", "Treasure", "Action", "Action, Treasure", "Action", "Action, Duration, Attack", "Action", "Action, Gathering", "Action", "Action", "Action", "Action, Attack", "Action", "Action, Command", "Action", "Action", "Action", "Action, Gathering", "Action", "Action, Gathering", "Victory, Castle" }, IntrigueTypes = { "Action", "Action", "Action", "Action", "Action", "Action, Reaction", "Action", "Action", "Action", "Action, Victory", "Action", "Action, Attack", "Action", "Action", "Action, Attack", "Action", "Action", "Action", "Action, Attack", "Action, Attack", "Action", "Action", "Action", "Action, Victory", "Treasure, Victory", "Victory" }, NocturneTypes = { "Action, Fate", "Action, Fate", "Victory", "Night", "Night, Duration", "Action", "Night, Duration", "Action, Doom", "Night, Duration", "Night", "Action, Fate", "Night", "Action, Reaction", "Action, Fate", "Night, Duration", "Night, Duration", "Treasure, Attack, Fate", "Action, Doom", "Night", "Action", "Night", "Action, Fate", "Action", "Night, Duration, Attack", "Action, Fate", "Action, Duration", "Action", "Action, Attack, Doom", "Action, Attack, Doom", "Action, Fate", "Action", "Night, Attack, Doom", "Action, Night, Attack, Doom"};
    array<vector<string>, expansionsOwned> cardsTypes = { DominionTypes, IntrigueTypes, DarkAgesTypes, AlchemyTypes, AdventuresTypes, EmpiresTypes, NocturneTypes }; //For giving types to all cards

    vector<string> AdventuresNames = { "Amulet", "Artificer", "Bridge Troll", "Caravan Guard",  "Coin of the Realm", "Distant Lands", "Dungeon", "Duplicate", "Gear", "Giant", "Guide", "Haunted Woods", "Hireling", "Lost City", "Magpie", "Messenger", "Miser", "Port", "Ranger", "Ratcatcher", "Raze", "Relic", "Royal Carriage", "Storyteller", "Swamp hag", "Transmogrify", "Treasure trove", "Wine Merchant", "Page + upgrades", "Peasant + upgrades" }, AlchemyNames = { "Alchemist", "Apothecary", "Apprentice", "Familiar", "Golem", "Herbalist", "Philosopher's Stone", "Possession", "Scrying Pool", "Transmute", "University", "Vineyard" }, DarkAgesNames = { "Altar", "Armory", "Band of Misfits", "Bandit Camp", "Beggar", "Catacombs", "Count", "Counterfeit", "Cultist", "Death Cart", "Feodum", "Forager", "Fortress", "Graverobber", "Hermit", "Hunting Grounds", "Ironmonger", "Junk Dealer", "Marauder", "Market Square", "Mystic", "Pillage", "Poor House", "Procession", "Rats", "Rebuild", "Rogue", "Sage", "Scavenger", "Squire", "Storeroom", "Urchin", "Vagrant", "Wandering Minstrel", "Knights" }, DominionNames = { "Adventurer", "Bureaucrat", "Cellar", "Chancellor", "Chapel", "Council Room", "Feast", "Festival", "Laboratory", "Market", "Militia", "Mine", "Moat", "Moneylender", "Remodel", "Smithy", "Spy", "Thief", "Throne Room", "Village", "Witch", "Woodcutter", "Workshop", "Gardens" }, EmpiresNames = { "Archive", "Capital", "Catapult/Rocks", "Chariot Race", "Charm", "City Quarter", "Crown", "Encampment/Plunder", "Enchantress", "Engineer", "Farmers' Market", "Forum", "Gladiator/Fortune", "Groundskeeper", "Legionary", "Royal Blacksmith", "Overlord", "Patrician/Emporium", "Sacrifice", "Settlers/Bustling Village", "Temple", "Villa", "Wild Hunt", "Castles" }, IntrigueNames = { "Baron", "Bridge", "Conspirator", "Courtier", "Courtyard", "Diplomat", "Ironworks", "Lurker", "Masquerade", "Mill", "Mining Village", "Minion", "Patrol", "Pawn", "Replace", "Secret Passage", "Shanty Town", "Steward", "Swindler", "Torturer", "Trading Post", "Upgrade", "Wishing Well", "Nobles", "Harem", "Duke" }, NocturneNames = { "Bard", "Blessed Village", "Cemetery", "Changeling", "Cobbler", "Conclave", "Crypt", "Cursed Village", "Den of Sin", "Devil's Workshop", "Druid", "Exorcist", "Faithful Hound", "Fool", "Ghost Town", "Guardian", "Idol", "Leprechaun", "Monastery", "Necromancer", "Night Watchman", "Pixie", "Pooka", "Raider", "Sacred Grove", "Secret Cave", "Shepherd", "Skulk", "Tormentor", "Tracker", "Tragic Hero", "Vampire", "Werewolf" };
    array<vector<string>, expansionsOwned> cardsNamed = { DominionNames, IntrigueNames, DarkAgesNames, AlchemyNames, AdventuresNames, EmpiresNames, NocturneNames }; //For naming all the cards

    vector<int> AdventuresCost = { 3, 5, 5, 3, 2, 5, 3, 4, 3, 5, 3, 5, 6, 5, 4, 4, 4, 4, 4, 2, 2, 5, 5, 5, 5, 4, 5, 5, 2, 2 }, AlchemyCost = { 3, 2, 5, 3, 4, 2, 3, 6, 2, 0, 2, 0 }, DarkAgesCost = { 6, 4, 5, 5, 2, 5, 5, 5, 5, 4, 4, 3, 4, 5, 3, 6, 4, 5, 4, 3, 5, 5, 1, 4, 4, 5, 5, 3, 4, 2, 3, 3, 2, 4, 5 }, DominionCost = { 6, 4, 2, 3, 2, 5, 4, 5, 5, 5, 3, 5, 2, 4, 4, 4, 4, 4, 4, 3, 5, 3, 3, 4 }, EmpiresCost = { 5, 5, 3, 3, 5, 0, 5, 2, 3, 0, 3, 5, 3, 5, 5, 0, 0, 2, 4, 2, 4, 4, 5, 3 }, IntrigueCost = { 4, 4, 4, 5, 2, 4, 4, 2, 3, 4, 4, 5, 5, 2, 5, 4, 3, 3, 3, 5, 5, 5, 3, 6, 6, 5 }, NocturneCost = { 4, 4, 4, 3, 5, 4, 5, 5, 5, 4, 2, 4, 2, 3, 3, 2, 5, 3, 2, 4, 3, 2, 5, 6, 5, 3, 4, 4, 5, 2, 5, 5, 5 };
    array<vector<int>, expansionsOwned> cardsCost = { DominionCost, IntrigueCost, DarkAgesCost, AlchemyCost, AdventuresCost, EmpiresCost, NocturneCost }; //For adding cost to all the cards

    vector<int> AdventuresActions = { 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 2, 1, 0, 0, 2, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0 }, AlchemyActions = { 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 2, 0 }, DarkAgesActions = { 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 2, 0 }, DominionActions = { 0, 0, 1, 0, 0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0 }, EmpiresActions = { 1, 0, 0, 1, 0, 2, 0, 2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0 }, IntrigueActions = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0 }, NocturneActions = { 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 };
    array<vector<int>, expansionsOwned> cardsActions = { DominionActions, IntrigueActions, DarkAgesActions, AlchemyActions, AdventuresActions, EmpiresActions, NocturneActions }; //For adding actions all cards give

    vector<int> AdventuresCards = { 0, 1, 0, 1, 0, 0, 2, 0, 2, 0, 1, 0, 0, 2, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }, AlchemyCards = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, DarkAgesCards = { 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 4, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 }, DominionCards = { 0, 0, 0, 0, 0, 4, 0, 0, 2, 1, 0, 0, 2, 0, 0, 3, 1, 0, 0, 1, 2, 0, 0, 0 }, EmpiresCards = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 1, 0, 5, 0, 1, 0, 1, 0, 0, 0, 0 }, IntrigueCards = { 0, 0, 0, 0, 3, 2, 0, 0, 2, 1, 1, 0, 3, 0, 0, 2, 0, 0, 0, 3, 0, 1, 1, 0, 0, 0 }, NocturneCards = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0 };
    array<vector<int>, expansionsOwned> cardsCards = { DominionCards, IntrigueCards, DarkAgesCards, AlchemyCards, AdventuresCards, EmpiresCards, NocturneCards }; //For adding cards all cards give

    vector<int> AdventuresCoins = { 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 2, 4, 0, 1 }, AlchemyCoins = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, DarkAgesCoins = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 4, 0, 0, 0, 2, 0, 2, 1, 0, 0, 0, 0, 0 }, DominionCoins = { 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0 }, EmpiresCoins = { 0, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0 }, IntrigueCoins = { 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0 }, NocturneCoins = { 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 2, 1, 0, 0, 0 };
    array<vector<int>, expansionsOwned> cardsCoins = { DominionCoins, IntrigueCoins, DarkAgesCoins, AlchemyCoins, AdventuresCoins, EmpiresCoins, NocturneCoins }; //For adding coins all cards give

    vector<int> AdventuresBuys = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 }, AlchemyBuys = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, DarkAgesBuys = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, DominionBuys = { 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }, EmpiresBuys = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }, IntrigueBuys = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, NocturneBuys = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };
    array<vector<int>, expansionsOwned> cardsBuys = { DominionBuys, IntrigueBuys, DarkAgesBuys, AlchemyBuys, AdventuresBuys, EmpiresBuys, NocturneBuys }; //For adding buys all cards give

    vector<bool> AdventuresSpecial = { true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, false, false }, AlchemySpecial = { true, true, true, true, true, true, true, true, true, true, true, true }, DarkAgesSpecial = { true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true }, DominionSpecial = { true, true, true, true, true, true, true, false, false, false, true, true, true, true, true, false, true, true, true, false, true, false, true, true }, EmpiresSpecial = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true }, IntrigueSpecial = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true }, NocturneSpecial = { true, true, true, true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true  };
    array<vector<bool>, expansionsOwned> cardsSpecial = { DominionSpecial, IntrigueSpecial, DarkAgesSpecial, AlchemySpecial, AdventuresSpecial, EmpiresSpecial, NocturneSpecial }; //For adding if each card has additional effects

    vector<bool> AdventuresPotion = { false }, AlchemyPotion = { true, true, false, true, true, false, true, true, true, true, true, true }, DarkAgesPotion = { false }, DominionPotion = { false }, EmpiresPotion = { false }, IntriguePotion = { false }, NocturnePotion = { false };
    array<vector<bool>, expansionsOwned> cardsPotion = { DominionPotion, IntriguePotion, DarkAgesPotion, AlchemyPotion, AdventuresPotion, EmpiresPotion, NocturnePotion }; //For adding if each card has a potion in the cost

    vector<int> AdventuresDebt = { 0 }, AlchemyDebt = { 0 }, DarkAgesDebt = { 0 }, DominionDebt = { 0 }, EmpiresDebt = { 0, 0, 0, 0, 0, 8, 0, 0, 0, 4, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0 }, IntrigueDebt = { 0 }, NocturneDebt = { 0 };
    array<vector<int>, expansionsOwned> cardsDebt = { DominionDebt, IntrigueDebt, DarkAgesDebt, AlchemyDebt, AdventuresDebt, EmpiresDebt, NocturneDebt }; //For adding the debt (if any) each card gives when bought

    card temp;

    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsTypes[i].size(); n++) {
            expansions[i].push_back(temp);
            expansions[i][n].types = cardsTypes[i][n];
            expansions[i][n].expansion = expansionsNamed[i];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsNamed[i].size(); n++) {
            expansions[i][n].name = cardsNamed[i][n];
            expansions[i][n].sorting = to_string(i) + expansions[i][n].expansion + expansions[i][n].name;
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsCost[i].size(); n++) {
            expansions[i][n].cost = cardsCost[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsActions[i].size(); n++) {
            expansions[i][n].actions = cardsActions[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsCards[i].size(); n++) {
            expansions[i][n].cards = cardsCards[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsCoins[i].size(); n++) {
            expansions[i][n].coins = cardsCoins[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsBuys[i].size(); n++) {
            expansions[i][n].buys = cardsBuys[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsSpecial[i].size(); n++) {
            expansions[i][n].special = cardsSpecial[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsPotion[i].size(); n++) {
            expansions[i][n].potion = cardsPotion[i][n];
        }
    }
    for (int i = 0; i < expansionsOwned; i++) {
        for (int n = 0; n < cardsDebt[i].size(); n++) {
            expansions[i][n].debt = cardsDebt[i][n];
        }
    }*/

    card temp;
    string line, expansion, name, types;
    array<int, nrOfVariables> nums;
    int n;
    char character;
    ifstream theDeck ("deck.txt");
    if (!theDeck.is_open()){
        cout << "The file couldn't be opened" << endl;
        return;
    }
    getline(theDeck, line);
    getline(theDeck, expansion, '\t');

    for(int i = 0; i < expansionsOwned; i++){
        while(expansion == expansionsNamed[i]){
            while (theDeck.get(character)){
                if (character != '\t'){
                    break;
                }
            }
            getline(theDeck, name, '\t');
            name = character + name;
            while (theDeck.get(character)){
                if (character != '\t'){
                    break;
                }
            }
            getline(theDeck, types, '\t');
            types = character + types;
            n = 0;
            while (theDeck.get(character)){
                if (character == '\n'){
                    break;
                }
                else if (character != '\t')
                {
                    nums[n] = character - '0';
                    n++;
                }
            }
            temp.name = name;
            temp.expansion = expansion;
            temp.sorting = to_string(i) + expansion + name;
            temp.types = types;
            temp.cost = nums[0];
            temp.actions = nums[1];
            temp.cards = nums[2];
            temp.coins = nums[3];
            temp.buys = nums[4];
            temp.special = bool(nums[5]);
            temp.potion = bool(nums[6]);
            temp.debt = nums[7];

            expansions[i].push_back(temp);
            getline(theDeck, expansion, '\t');
            //std::cout << std::endl << expansion << std::endl;
        }
    }

    theDeck.close();

    //Debug code:
    cout << expansions[2][34].name << endl << expansions[2][34].expansion << endl << expansions[2][34].types << endl << expansions[2][34].cost << endl << expansions[2][34].actions << endl << expansions[2][34].cards << endl << expansions[2][34].coins << endl << expansions[2][34].buys << endl << expansions[2][34].debt << endl << expansions[2][34].potion << endl << expansions[2][34].special << endl << expansions[2][34].sorting << endl;
}

void randomExpansions(vector<int>& expansionsUsed) {

    int randMax, placeholder;

    cout << "How many versions of Dominion are to be used? (Enter 0 for random)" << endl;
    cin >> randMax;

    while (randMax < 1 || randMax > expansionsOwned) {

        if (randMax == 0) {
            randMax = rand() % expansionsOwned + 1;
            cout << "The number of expansions used is " << randMax << "." << endl;
            break;
        }

        cout << "Wrong number entered. Try again:" << endl;
        cin >> randMax;
    }

    while (expansionsUsed.size() < randMax) {
        int i;

        placeholder = rand() % expansionsOwned;

        for (i = 0; i < expansionsUsed.size(); i++) {
            if (placeholder == expansionsUsed[i]) {
                i = -1;
                break;
            }
        }
        if (i != -1) {
            expansionsUsed.push_back(placeholder);
        }
    }
}

void chosenExpansions(vector<int>& expansionsUsed, array<string, expansionsOwned> expansionsNamed) {
    int choice = 1;
    array<bool, expansionsOwned> checklist;
    for (int i = 0; i < expansionsOwned; i++) {
        checklist[i] = true;
    }

    while (true) {
        cout << endl << "Enter the number for the expansion you want to use: (Enter 0 when done)" << endl;

        for (int i = 0; i < expansionsOwned; i++) {
            if (checklist[i]) {
                cout << i + 1 << ": " << expansionsNamed[i];
            }
            cout << endl;
        }
        cin >> choice;
        choice--;
        if (choice == -1) {
            break;
        }
        while (!checklist[choice]) {
            cout << endl << "Invalid choice. Choose again." << endl << "Enter the number for the expansion you want to use: (Enter 0 when done)" << endl;

            for (int i = 0; i < expansionsOwned; i++) {
                if (checklist[i]) {
                    cout << i + 1 << ": " << expansionsNamed[i];
                }
                cout << endl;
            }
            cin >> choice;
            choice--;
            if (choice == -1) {
                break;
            }
        }
        if (choice == -1) {
            break;
        }
        checklist[choice] = false;
    }

    for (int i = 0; i < expansionsOwned; i++) {
        if (!checklist[i]) {
            expansionsUsed.push_back(i);
        }
    }
}

void manualRandomExpansions(vector<int>& expansionsUsed, array<string, expansionsOwned> expansionsNamed) {
    int choice = 1, randMax, placeholder;
    vector<int> randChoice;
    array<bool, expansionsOwned> checklist;
    for (int i = 0; i < expansionsOwned; i++) {
        checklist[i] = true;
    }

    while (true) {
        cout << endl << "It will be randomly chosen from among these expansions:" << endl;

        for (int i = 0; i < expansionsOwned; i++) {
            if (checklist[i]) {
                cout << i + 1 << ": " << expansionsNamed[i];
            }
            cout << endl;
        }
        cout << "Enter the number for the expansion you DO want to use: (Enter 0 when done)" << endl;
        cin >> choice;
        if (choice == 0) {
            break;
        }
        choice--;
        while (!checklist[choice]) {
            cout << endl << "Invalid choice. Choose again." << endl << "It will be randomly chosen from among these expansions:" << endl;

            for (int i = 0; i < expansionsOwned; i++) {
                if (checklist[i]) {
                    cout << i + 1 << ": " << expansionsNamed[i];
                }
                cout << endl;
            }
            cout << "Enter the number for the expansion you DO want to use: (Enter 0 when done)" << endl;
            cin >> choice;
            choice--;
            if (choice == -1) {
                break;
            }
        }
        if (choice == -1) {
            break;
        }
        checklist[choice] = false;
    }
    for (int i = 0; i < expansionsOwned; i++) {
        if (!checklist[i]) {
            expansionsUsed.push_back(i);
        }
    }

    choice = 1;

    while (true) {
        cout << endl << "It will be randomly chosen from among these expansions:" << endl;

        for (int i = 0; i < expansionsOwned; i++) {
            if (checklist[i]) {
                cout << i + 1 << ": " << expansionsNamed[i];
            }
            cout << endl;
        }
        cout << "Enter the number for the expansion you DON'T want to use: (Enter 0 when done)" << endl;
        cin >> choice;
        if (choice == 0) {
            break;
        }
        choice--;
        while (!checklist[choice]) {
            cout << endl << "Invalid choice. Choose again." << endl << "It will be randomly chosen from among these expansions:" << endl;

            for (int i = 0; i < expansionsOwned; i++) {
                if (checklist[i]) {
                    cout << i + 1 << ": " << expansionsNamed[i];
                }
                cout << endl;
            }
            cout << "Enter the number for the expansion you DON'T want to use: (Enter 0 when done)" << endl;
            cin >> choice;
            choice--;
            if (choice == -1) {
                break;
            }
        }
        if (choice == -1) {
            break;
        }
        checklist[choice] = false;
    }
    for (int i = 0; i < expansionsOwned; i++) {
        if (checklist[i]) {
            randChoice.push_back(i);
        }
    }

    cout << "How many versions of Dominion are to be used? (Enter 0 for random)" << endl;
    cin >> randMax;

    while (randMax < 1 || randMax > randChoice.size()) {

        if (randMax == 0) {
            randMax = rand() % randChoice.size() + 1;
            cout << "The number of expansions used is " << randMax << "." << endl;
            break;
        }

        cout << "Wrong number entered. Try again:" << endl;
        cin >> randMax;
    }

    while (expansionsUsed.size() < randMax) {
        int i;

        placeholder = randChoice[rand() % randChoice.size()];

        for (i = 0; i < expansionsUsed.size(); i++) {
            if (placeholder == expansionsUsed[i]) {
                i = -2;
            }
        }
        if (i != -1) {
            expansionsUsed.push_back(placeholder);

        }
    }
}

void randomCards(array<card, nrOfCards>& cardsUsed, vector<int>& expansionsUsed, array<vector<card>, expansionsOwned>& expansions) 
{
    int placeholder, placeholder2;
    bool check;
    card card;
    for (int i = 0; i < nrOfCards; i++) {
        placeholder = expansionsUsed[rand() % expansionsUsed.size()];
        placeholder2 = rand() % expansions[placeholder].size();
        card = expansions[placeholder][placeholder2];

        check = true;
        for (int n = 0; n < nrOfCards; n++) {
            if (card.name == cardsUsed[n].name) {
                i--;
                check = false;
                break;
            }
        }
        if (check) {
            cardsUsed[i] = card;
        }
    }
}

void manualRandomCards(array<card, nrOfCards>& cardsUsed, vector<int>& expansionsUsed, array<vector<card>, expansionsOwned>& expansions) 
{
    vector<string> typesNeg;
    int minCost = 0, maxCost = -1, CostTemp = -1, maxActions = -1, nrOfMinActions = 0, minActions = 0, maxCards = -1, nrOfMinCards = 0, minCards = 0, maxCoins = -1, nrOfMinCoins = 0, minCoins = 0, maxBuys = -1, nrOfMinBuys = 0, minBuys = 0, maxDebt = -1, nrOfMinDebt = 0, minDebt = 0, minSpec = 0, maxSpec = -1, minPot = 0, maxPot = -1;
    map<string, int> typesPos, typesPosCount;
    map<int, int> costSpec, costSpecCount;
    int choice, choice2;
    string strChoice = "";

    while (true)
    {
        cout << "Choose an effect to set special rules for: (Enter 0 when done)" << endl;
        cout << "1: Types" << endl;
        cout << "2: Cost" << endl;
        cout << "3: Potions" << endl;
        cout << "4: Debt" << endl;
        cout << "5: Actions" << endl;
        cout << "6: Cards" << endl;
        cout << "7: Coins" << endl;
        cout << "8: Buys" << endl;
        cout << "9: Special effects" << endl;
        cin >> choice;

        if (choice == 0)
        {
            break;
        }
        else if (choice == 1) //Types
        {

            cout << "Enter the types you do NOT want to use, enter 0 when you are done" << endl;
            while (strChoice != "0")
            {
                cin >> strChoice;
                typesNeg.push_back(strChoice);
            }

            while (true)
            {
                cout << "Enter any type you want to specify the MINIMUM amount to use, enter 0 if you are done:" << endl;
                cin >> strChoice;
                if (strChoice == "0") {
                    break;
                }
                cout << "At least how many do you want to use?" << endl;
                cin >> choice;
                typesPos.insert( pair<string, int>(strChoice, choice) );
                typesPosCount.insert(pair<string, int>(strChoice, 0));
            }
        }
        else if (choice == 2) //Cost
        {
            cout << "Do you want to set a minimum cost? For yes, enter 1." << endl;
            cin >> choice;
            while (choice == 1)
            {
                cout << "Enter the minimum cost for cards." << endl;
                cin >> minCost;
            }

            cout << "Do you want to set a maximum cost? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter the maximum cost for cards." << endl;
                cin >> maxCost;
            }

            while (true)
            {
                cout << "Enter any cost you want to specify how many you want of. Enter -1 when you are done." << endl;
                cin >> choice;
                if (choice == -1)
                {
                    break;
                }
                cout << "Enter at least how many you want of that cost." << endl;
                cin >> choice2;
                costSpec.insert(pair<int, int>(choice, choice2));
                costSpecCount.insert(pair<int, int>(choice, 0));
            }
        }
        else if (choice == 3) //Potions
        {
            cout << "Do you want to set a minimum number of cards with a potion cost? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "At least how many cards should have a potion cost?" << endl;
                cin >> minPot;
            }
            cout << "Do you want to set a maximum number of cards with a potion cost? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "At most how many cards should have a potion cost?" << endl;
                cin >> maxPot;
            }
        }
        else if (choice == 4) //Debt
        {
            cout << "Do you want to set a maximum amount of debt each card cost? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter the maximum debt cost for each card." << endl;
                cin >> maxBuys;
            }
            cout << "Do you want to set a minimum amount of debt on a specified number of cards? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "How many cards should cost at least the specified number of debt?" << endl;
                cin >> nrOfMinDebt;
                cout << "At least how much debt should they cost?" << endl;
                cin >> minDebt;
            }
        }
        else if (choice == 5) //Actions
        {
            cout << "Do you want to set a maximum number of actions each card gives? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter the maximum number of actions each card may give." << endl;
                cin >> maxActions;
            }
            cout << "Do you want to set a minimum number of actions on a specified number of cards? For yes, enter 1." << endl; 
            cin >> choice;
            if (choice == 1) 
            {
                cout << "How many cards should give at least the specified number of actions?" << endl;
                cin >> nrOfMinActions;
                cout << "At least how many actions should they give?" << endl;
                cin >> minActions;
            }
        }
        else if (choice == 6) //Cards
        {
            cout << "Do you want to set a maximum number of cards each card gives? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter the maximum number of cards each card may give." << endl;
                cin >> maxCards;
            }
            cout << "Do you want to set a minimum number of cards on a specified number of cards? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "How many cards should give at least the specified number of cards?" << endl;
                cin >> nrOfMinCards;
                cout << "At least how many cards should they give?" << endl;
                cin >> minCards;
            }
        }
        else if (choice == 7) //Coins
        {
            cout << "Do you want to set a maximum number of coins each card gives? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter the maximum number of coins each card may give." << endl;
                cin >> maxCoins;
            }
            cout << "Do you want to set a minimum number of coins on a specified number of cards? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "How many cards should give at least the specified number of coins?" << endl;
                cin >> nrOfMinCoins;
                cout << "At least how many coins should they give?" << endl;
                cin >> minCoins;
            }
        }
        else if (choice == 8) //Buys
        {
            cout << "Do you want to set a maximum number of buys each card gives? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter the maximum number of buys each card may give." << endl;
                cin >> maxBuys;
            }
            cout << "Do you want to set a minimum number of buys on a specified number of cards? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "How many cards should give at least the specified number of buys?" << endl;
                cin >> nrOfMinBuys;
                cout << "At least how many buys should they give?" << endl;
                cin >> minBuys;
            }
        }
        else if (choice == 9) //Special effects
        {
            cout << "Do you want to set a minimum number of cards with a special effect? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "At least how many cards should have a special effect?" << endl;
                cin >> minSpec;
            }
            cout << "Do you want to set a maximum number of cards with a special effect? For yes, enter 1." << endl;
            cin >> choice;
            if (choice == 1)
            {
                cout << "At most how many cards should have a special effect?" << endl;
                cin >> maxSpec;
            }
        }
    }

    int nrOfActionsMin = 0, nrOfCardsMin = 0, nrOfCoinsMin = 0, nrOfBuysMin = 0, nrOfSpec = 0, nrOfDebtMin = 0;
    int placeholder, placeholder2;
    bool check;
    card card;
    int counter = 0;
    int i = 0;
    
    while (i < nrOfCards) {
        counter++;
        if (counter > 1000) {
            cout << "Program failed, restart it. Too many loops." << endl;
            break;
        }
        placeholder = expansionsUsed[rand() % expansionsUsed.size()];
        placeholder2 = rand() % expansions[placeholder].size();
        card = expansions[placeholder][placeholder2];

        check = true;

        if (card.cost < minCost || (maxCost != -1 && card.cost > maxCost) || (costSpec.count(card.cost)) || (maxActions != -1 && card.actions > maxActions) || (maxCards != -1 && card.cards > maxCards) || (maxCoins != -1 && card.coins > maxCoins) || (maxBuys != -1 && card.buys > maxBuys) || (maxDebt != -1 && card.debt > maxDebt) || (nrOfActionsMin < nrOfMinActions && card.actions < minActions) || (nrOfCardsMin < nrOfMinCards && card.cards < minCards) || (nrOfCoinsMin < nrOfMinCoins && card.coins < minCoins) || (nrOfBuysMin < nrOfMinBuys && card.buys < minBuys) || (maxSpec != -1 && nrOfSpec >= maxSpec && card.special) || (nrOfSpec < minSpec && !card.special) || (nrOfDebtMin < nrOfMinDebt && card.debt < minDebt))
        {
            check = false;
            continue;
        }
        for (int n = 0; n < typesNeg.size(); n++) {
            if (card.types.find(typesNeg[n]) != string::npos) {
                check = false;
                break;
            }
        }
        for (int n = 0; n < nrOfCards; n++) {
            if (card.name == cardsUsed[n].name) {
                check = false;
                break;
            }
        }
        if (check) {
            cardsUsed[i] = card;
            i++;
            if (card.actions >= minActions) {
                nrOfActionsMin++;
            }
            if (card.cards >= minCards) {
                nrOfCardsMin++;
            }
            if (card.coins >= minCoins) {
                nrOfCoinsMin++;
            }
            if (card.buys >= minBuys) {
                nrOfBuysMin++;
            }
            if (card.special) {
                nrOfSpec++;
            }
        }
    }
}

void sortCards(array<card, nrOfCards>& cardsUsed) //Legacy, not used anymore
{
    bool check = false;
    card placeholder;
    while (!check)
    {
        check = true;
        for (int i = 0; i < nrOfCards-1; i++)
        {
            if (cardsUsed[i].sorting > cardsUsed[i+1].sorting) 
            {
                check = false;
                placeholder = cardsUsed[i];
                for (int n = nrOfCards-1; n > i; n--)
                {
                    if (placeholder.sorting > cardsUsed[n].sorting) 
                    {
                        for (int m = i; m < n; m++)
                        {
                            cardsUsed[m] = cardsUsed[m + 1];
                        }
                        cardsUsed[n] = placeholder;
                        break;
                    }
                }
                i--;
            }
        }
    }
}

bool condition(card one, card two) {
    return (one.sorting < two.sorting);
}
