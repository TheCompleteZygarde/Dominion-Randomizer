#include <iostream>
#include <fstream>
#include <string>
#include <array>

struct card {
	std::string sorting = "";
	std::string expansion = "";
	std::string types = "";
	std::string name = "";
	int cost = 0;
	int actions = 0;
	int cards = 0;
	int coins = 0;
	int buys = 0;
	bool special = false;
	bool potion = false;
	int debt = 0;
};

const int expansionsOwned = 7;
const std::array<std::string, expansionsOwned> expansionsNamed = { "Dominion", "Intrigue", "Dark Ages", "Alchemy", "Adventures", "Empires", "Nocturne"}; //For naming the expansions

int main(){
    card temp;
    std::string line, expansion, name, types;
    std::array<int, 8> nums;
    int n;
    char character;
    std::ifstream theDeck ("deck.txt");
    if (theDeck.is_open()){
        getline(theDeck, line);
        getline(theDeck, expansion, '\t');

        for(int i = 0; i < 7; i++){
            while(expansion == expansionsNamed[i]){
                while (theDeck.get(character)){
                    if (character != '\t'){
                        break;
                    }
                }
                getline(theDeck, name, '\t');
                while (theDeck.get(character)){
                    if (character != '\t'){
                        break;
                    }
                }
                getline(theDeck, types, '\t');
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
                temp.sorting = std::to_string(i) + expansion + name;
                temp.cost = nums[0];
                temp.actions = nums[1];
                temp.cards = nums[2];
                temp.coins = nums[3];
                temp.buys = nums[4];
                temp.special = bool(nums[5]);
                temp.potion = bool(nums[6]);
                temp.debt = nums[7];
                getline(theDeck, expansion, '\t');
                //std::cout << std::endl << expansion << std::endl;
            }
            
        }

        /*

        getline(myfile, line, '\t');
        std::cout << line << std::endl;
        while (myfile.get(character)){
            if (character != '\t'){
                break;
            }
        }
        getline(myfile, line, '\t');
        std::cout << character + line << std::endl;
        while (myfile.get(character)){
            if (character != '\t'){
                break;
            }
        }
        getline(myfile, line, '\t');
        std::cout << character + line << std::endl;*/
    }
    theDeck.close();
    return 0;
}