#include <iostream>
#include <windows.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include "main.h"

using std::cout;
using std::cin;
using std::endl;

std::string trim(std::string str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
        }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
        }).base(), str.end());
    return str;
}

std::vector<std::pair<std::string, Move>> getLegalMoves(Player p, std::vector<std::pair<std::string, Move>> moves_vector) {
    std::vector<std::pair<std::string, Move>> legalMoves_vector;
    std::copy_if(moves_vector.begin(), moves_vector.end(), std::back_inserter(legalMoves_vector), [p](std::pair<std::string, Move> i) {
        Move m = i.second;
        return m.getCost() <= p._charges;
        });
    return legalMoves_vector;
}

int catchRecursion = 0; // better safe than sorry
int whoWins(Move a, Move b, int p1, int p2) {
    catchRecursion++;
    if (catchRecursion > 10) {
        exit(catchRecursion);
    }
    if (a.getSpecial() == bb) { // brick break
        if (b.getSpecial() == e)
            return p2;
        if (b.getSpecial() == wp)
            return p1;
    }
    if (a.getSpecial() == e) { // ember
        if (b.getSpecial() == bb)
            return p1;
        if (b.getSpecial() == wp)
            return p2;
    }
    if (a.getSpecial() == wp) { // water pulse
        if (b.getSpecial() == bb)
            return p2;
        if (b.getSpecial() == e)
            return p1;
    }
    if (b.getSpecial() != mp && b.getPower() > 0) {
        if (a.getSpecial() == rf) // reflect
            return (b.getPower() < -a.getPower()) ? p1 : p2;
        else if (a.getSpecial() == lm) { // light mirror
            return p1;
        }
    }
    if (a.getSpecial() == mp) { //mega punch
        if (b.getSpecial() == p)
            return -1; //tied with punch
        else if (b.getSpecial() == rf || b.getSpecial() == lm) {
            return p1;
        }
    }
    if (a.getPower() <= 0 && b.getPower() <= 0) { // if both are charge/shields
        return 0; //nothing
    }
    if (b.getSpecial() == rf || b.getSpecial() == lm) {
        return whoWins(b, a, p2, p1); //check everything again with values swapped
    }
    if (a == b && a.getPower() > 0) {
        return -1; //tie
    }
    else if (a.getPower() > 0 && b.getPower() < 0) { // if attack vs shield
        if (-b.getPower() >= a.getPower()) { // if shield is stronger
            return 0;
        }
        else { // if weaker then attack wins
            return p1;
        }
    }
    else if (a.getPower() > b.getPower() && b.getPower() >= 0) {
        return p1;
    }
    else {
        return whoWins(b, a, p2, p1); //check everything again with values swapped
    }
}

std::vector<std::pair<std::string, Move>> createFullMoveNameVector(std::vector<std::pair<std::string, Move>>& moves_vector) {
    std::vector<std::pair<std::string, Move>> fullMoveName_vector;
    for (auto const& move : moves_vector) {
        std::string lowercaseName = move.second.getName();
        std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);
        fullMoveName_vector.emplace_back(lowercaseName, move.second);
    }
    return fullMoveName_vector;
}

int main()
{
    std::vector<std::pair<std::string, Move>> fullMoveName_vector = createFullMoveNameVector(moves_vector);
    int startingHP = 20;
    Player p1(0, 0, startingHP, 0);
    Player p2(0, 0, startingHP, 0);

    cout << ":::  Rune  :::" << endl;
    cout << ":::. Wars .:::" << endl << endl;
    cout << ">>> Hint: type help [movename]" << endl << endl;
    cout << ":::: HP: " << p1._health << " | MP: " << p1._charges << " (" << p1._focusPower << ")" << " ::: " << "HP: " << p2._health << " | MP: " << p2._charges << " (" << p2._focusPower << ")" << " ::::" << endl;
    cout << ".:.: ";

    while (true) {
        std::string currentAbbvMove;
        //auto runCommand = [&currentAbbvMove, p1, fullMoveName_vector](std::string input) {
        //    
        //};
        
        // Get player input
        std::string input = "";
        currentAbbvMove = "";
        std::getline(std::cin, input);
        runCommand(input, fullMoveName_vector, currentAbbvMove, p1, p2);
        input = currentAbbvMove;
        std::unordered_map<std::string, Move> moves;
        for (auto kv_pair : moves_vector) {
            moves[kv_pair.first] = kv_pair.second;
        }
        std::unordered_map<std::string, Move> fullMoveName;
        for (auto kv_pair : fullMoveName_vector) {
            fullMoveName[kv_pair.first] = kv_pair.second;
        }

        if (moves.find(input) == moves.end() && fullMoveName.find(input) == fullMoveName.end()) {
            cout << ".:.: ";
            cin.clear();
            fflush(stdin);
            continue;
        }
        if (moves[input].getCost() > p1._charges) {
            cout << ">>> Not enough charges" << endl;
            cout << ".:.: ";
            cin.clear();
            fflush(stdin);
            continue;
        }
        else {

        /*std::vector<std::pair<std::string, Move>> legalMoves_vector;
        std::copy_if(moves_vector.begin(), moves_vector.end(), std::back_inserter(legalMoves_vector), [p2](std::pair<std::string, Move> i) {
            Move m = i.second;
            return m.getCost() <= p2._charges;
            });*/

            // Get random CPU input
            std::random_device rd;
            std::uniform_int_distribution<int> uni(0, getLegalMoves(p2, moves_vector).size() - 1);
            int rngIndex = uni(rd);
            std::string input2 = getLegalMoves(p2, moves_vector)[rngIndex].first;

            // Update player states
            Actions(moves, startingHP, input, input2, p1, p2);
            cout << ":::: HP: " << p1._health << " | MP: " << p1._charges << " (" << p1._focusPower << ")" << " ::: " << "HP: " << p2._health << " | MP: " << p2._charges << " (" << p2._focusPower << ")" << " ::::" << endl;
            cout << ".:.: ";
        }
    }
}

void Actions(std::unordered_map<std::string, Move>& moves, int startingHP, std::string& input, std::string& input2, Player& p1, Player& p2)
{
    p2._charges -= moves[input2].getCost();
    double chargePower = 1;

    if (input == "f") {
        p1._focusPower += 1.5;
    }
    if (input2 == "f") {
        p2._focusPower += 1.5;
    }
    if (input == "c") {
        p1._charges += chargePower + p1._focusPower;
        if (p1._focusPower > 0) {
            p1._focusPower = 0;
        }
    }
    double handicap = p1.scores - p2.scores;
    handicap = handicap < 0 ? 0 : handicap;
    if (input2 == "c") {
        p2._charges += chargePower + p2._focusPower + handicap;
        if (p2._focusPower > 0) {
            p2._focusPower = 0;
        }
    }

    Move i1 = moves[input];
    Move i2 = moves[input2];
    p1._charges -= i1.getCost();
    cout << ":.:. " << input2 << endl;
    
    int winner = whoWins(i1, i2, 1, 2);

    std::string winnerSymbolThing;
    switch (winner) {
    case 1:
        winnerSymbolThing = " > ";
        break;
    case 2:
        winnerSymbolThing = " < ";
        break;
    case -1:
        winnerSymbolThing = " = ";
        break;
    case 0:
        winnerSymbolThing = " . ";
        break;
    }

    cout << ":::. " << i1.getName() << winnerSymbolThing << i2.getName() << " .:::" << endl;

    catchRecursion = 0;

    double extraDamage = 0;
    if (i1.getSpecial() == mp && (i2.getSpecial() == rf || i2.getSpecial() == lm)) { //if mega punch then crit reflectors
        extraDamage = 16;
    }
    if (i2.getSpecial() == mp && (i1.getSpecial() == rf || i1.getSpecial() == lm)) {
        extraDamage = 16;
    }
    double totaldamage1 = i1.getPower() + extraDamage;
    double totaldamage2 = i2.getPower() + extraDamage;
    double disrupt1 = round(totaldamage1 / 3);
    double disrupt2 = round(totaldamage2 / 3);

    if (winner == 1) {
        cout << ">>> Player 1 ";
        if (i1.getSpecial() == rf || i1.getSpecial() == lm) { //deflected
            p2._health -= totaldamage2;
            cout << "deflected " << totaldamage2 << " damage";
            if (disrupt2 > 0) {
                cout << ", disrupting " << disrupt2 << " mana!" << endl;
            }
            else {
                cout << "." << endl;
            }
            p2._charges -= disrupt2;
        }
        else { //standard
            p2._health -= abs(totaldamage1);
            cout << "dealt " << totaldamage1 << " damage";
            if (disrupt1 > 0) {
                cout << ", disrupting " << disrupt1 << " mana!";
                if (extraDamage > 0) {
                    cout << " (Crit!)" << endl;
                }
                else {
                    cout << endl;
                }
            }
            else {
                cout << "." << endl;
            }
            p2._charges -= disrupt1; //disrupt charges
        }
        p2._focusPower = 0; //disrupt focus
    }
    if (winner == 2) {
        cout << ">>> Player 2 ";
        if (i2.getSpecial() == rf || i2.getSpecial() == lm) {
            p1._health -= totaldamage1;
            cout << "deflected " << totaldamage1 << " damage";
            if (disrupt1 > 0) {
                cout << ", disrupting " << disrupt1 << " mana!" << endl;
            }
            else {
                cout << "." << endl;
            }
            p1._charges -= disrupt1;
        }
        else {
            p1._health -= abs(totaldamage2);
            cout << "dealt " << totaldamage2 << " damage";
            if (disrupt2 > 0) {
                cout << ", disrupting " << disrupt2 << " mana!";
                if (extraDamage > 0) {
                    cout << " (Crit!)" << endl;
                }
                else {
                    cout << endl;
                }
            }
            else {
                cout << "." << endl;
            }
            p1._charges -= disrupt2;
        }
        p1._focusPower = 0;
    }
    if (winner == -1) {
        cout << ">>> Parry! (+2 each)" << endl;
        p1._charges += 2;
        p2._charges += 2;
    }
    if (i1.getSpecial() == rv) {
        if (i2.getPower() > 0) {
            if (p1._health <= startingHP / 4) {
                p1._health = startingHP;
                cout << ">>> Player 1 revived!" << endl;
            }
        }
    }
    if (i2.getSpecial() == rv) {
        if (i1.getPower() > 0) {
            if (p2._health <= startingHP / 4) {
                p2._health = startingHP;
                cout << ">>> Player 2 revived!" << endl;
            }
        }
    }
    if (p1._charges <= 0) {
        p1._charges = 0;
    }
    if (p2._charges <= 0) {
        p2._charges = 0;
    }
    if (p2._health <= 0) {
        p1.scores++;
        cout << ">>> Player 1 wins! (" << p1.scores << ":" << p2.scores << ")\n";
    }
    else if (p1._health <= 0) {
        p2.scores++;
        cout << ">>> Player 2 wins! (" << p1.scores << ":" << p2.scores << ")\n";
    }
    if (p1._health <= 0 || p2._health <= 0) {
        handicap = p1.scores - p2.scores;
        std::cout << ">>> Difficulty set to: " << (chargePower + handicap) << endl;
        p1.reset();
        p2.reset();
    }
}

void runCommand(std::string& input, std::vector<std::pair<std::string, Move>>& fullMoveName_vector, std::string& currentAbbvMove, Player& p1, Player& p2)
{
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    for (int i = 0; i < moves_vector.size(); i++) {
        std::string move = moves_vector[i].first;
        std::string fullmove = fullMoveName_vector[i].first;
        if (input.substr(0, 5) == "help ") {
            std::string helpInput = input.substr(5);
            std::transform(helpInput.begin(), helpInput.end(), helpInput.begin(), ::tolower);
            if (helpInput == move || helpInput == fullmove) {
                Move currMove = moves_vector[i].second;
                cout << endl << ">>> " << currMove.getName() << " [" << move << "] - cost: " << currMove.getCost();
                if (currMove.getPower() > 0) {
                    cout << ", atk power: " << currMove.getPower() << endl;
                }
                else if (currMove.getPower() < 0) {
                    cout << ", def power: " << -currMove.getPower() << endl;
                }
                else {
                    cout << endl;
                }
                cout << ">>> " << currMove.getDesc() << endl << endl;
            }
        }
        if (input == move || input == fullmove) {
            currentAbbvMove = move;
        }
    }
    if (input == "moves") {
        cout << endl;
        cout << ">>> Player 1 legal moves:" << endl;
        cout << ">>> [";
        bool first = true;
        for (const auto& move : getLegalMoves(p1, moves_vector)) {
            if (!first) {
                cout << ", ";
            }
            cout << move.first;
            first = false;
        }
        cout << "]";
        cout << endl;
        cout << ">>> [";
        first = true;
        for (const auto& move : getLegalMoves(p1, fullMoveName_vector)) {
            if (!first) {
                cout << ", ";
            }
            cout << move.first;
            first = false;
        }
        cout << "]";
        cout << endl;
        cout << endl << ">>> Player 2 legal moves:" << endl;
        cout << ">>> [";
        first = true;
        for (const auto& move : getLegalMoves(p2, moves_vector)) {
            if (!first) {
                cout << ", ";
            }
            cout << move.first;
            first = false;
        }
        cout << "]";
        cout << endl;
        cout << ">>> [";
        first = true;
        for (const auto& move : getLegalMoves(p2, fullMoveName_vector)) {
            if (!first) {
                cout << ", ";
            }
            cout << move.first;
            first = false;
        }
        cout << "]";
        cout << endl << endl;
    }
    if (input == "help") {
        cout << endl;
        cout << ">>> Here are the list of moves, you can also type \"moves\" for a list of legal moves." << endl;
        cout << ">>> [";
        bool first = true;
        for (const auto& move : moves_vector) {
            if (!first) {
                cout << ", ";
            }
            cout << move.first;
            first = false;
        }
        cout << "]";
        cout << endl;
        cout << ">>> [";
        first = true;
        for (const auto& fullmove : fullMoveName_vector) {
            if (!first) {
                cout << ", ";
            }
            cout << fullmove.first;
            first = false;
        }
        cout << "]";
        cout << endl << endl;
    }
}

    