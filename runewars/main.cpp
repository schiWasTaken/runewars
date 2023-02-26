#include <iostream>
#include <windows.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;

struct Move {
    std::string _name = "";
    double _cost = 0;
    double _power = 0;
    int _special = 0;
    std::string _desc = "";

    Move() : _name(""), _cost(0), _power(0), _special(0), _desc("") {}
    Move(std::string n, double c, double p, int s, std::string d) : _name(n), _cost(c), _power(p), _special(s), _desc(d) {}

    std::string getName() const {
        return _name;
    }
    double getCost() const {
        return _cost;
    }
    double getPower() const {
        return _power;
    }
    int getSpecial() const {
        return _special;
    }
    std::string getDesc() const {
        return _desc;
    }
    bool operator==(const Move& other) const {
        return (_name == other._name && _cost == other._cost && _power == other._power && _special == other._special && _desc == other._desc);
    }
    bool operator!=(const Move& other) const {
        return !(*this == other);
    }
};

class Player {
public:
    double _charges;
    double _focusPower;
    double _health;
    int scores;

    // Default constructor
    Player() : _charges(0), _focusPower(0), _health(20), scores(0) {}

    // Constructor with parameters
    Player(double charges, double focusPower, double health, int scores) : _charges(charges), _focusPower(focusPower), _health(health), scores(scores) {}

    void reset() {
        this->_charges = 0;
        this->_focusPower = 0;
        this->_health = 20;
    }
};

std::string trim(std::string str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
        }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
        }).base(), str.end());
    return str;
}

std::string currentAbbvMove;

void runCommand(std::string input, std::vector<std::pair<std::string, Move>> moves, std::vector<std::pair<std::string, Move>> fullMoveName) {
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    for (int i = 0; i < moves.size(); i++) {
        std::string move = moves[i].first;
        std::string fullmove = fullMoveName[i].first;
        if (input.substr(0, 5) == "help ") {
            std::string helpInput = input.substr(5);
            std::transform(helpInput.begin(), helpInput.end(), helpInput.begin(), ::tolower);
            if (helpInput == move || helpInput == fullmove) {
                Move currMove = moves[i].second;
                cout << currMove.getName() << " [" << move << "] - cost: " << currMove.getCost();
                if (currMove.getPower() > 0) {
                    cout << ", atk power: " << currMove.getPower() << endl;
                }
                else if (currMove.getPower() < 0) {
                    cout << ", def power: " << -currMove.getPower() << endl;
                }
                else {
                    cout << endl;
                }
                cout << currMove.getDesc() << endl;
            }
        }
        if (input == move || input == fullmove) {
            currentAbbvMove = move;
        }
    }
    if (input == "help") {
        cout << ">>> Here are the list of moves, hint: you can type help f or help focus for more info." << endl;
        cout << "[";
        bool first = true;
        for (const auto& move : moves) {
            if (!first) {
                cout << ", ";
            }
            cout << move.first;
            first = false;
        }
        cout << "]";
        cout << endl;
        cout << "[";
        first = true;
        for (const auto& fullmove : fullMoveName) {
            if (!first) {
                cout << ", ";
            }
            cout << fullmove.first;
            first = false;
        }
        cout << "]";
        cout << endl;
    }
}
int catchRecursion = 0; // i mean... better safe than sorry
int whoWins(Move a, Move b, int p1, int p2) {
    catchRecursion++;
    if (catchRecursion > 10) {
        exit(catchRecursion);
    }
    if (a.getSpecial() == 1) { //bb
        if (b.getSpecial() == 2)
            return p2;
        if (b.getSpecial() == 3)
            return p1;
    }
    if (a.getSpecial() == 2) { //ember
        if (b.getSpecial() == 1)
            return p1;
        if (b.getSpecial() == 3)
            return p2;
    }
    if (a.getSpecial() == 3) { //water
        if (b.getSpecial() == 1)
            return p2;
        if (b.getSpecial() == 2)
            return p1;
    }
    if (b.getSpecial() != 5 && b.getPower() > 0) {
        if (a.getSpecial() == 4) //reflect
            return (b.getPower() < -a.getPower()) ? p1 : p2;
        else if (a.getSpecial() == 6) { //light mirror
            return p1;
        }
    }
    if (a.getSpecial() == 5) { //mega punch
        if (b.getName() == "Punch")
            return -1; //tied with punch
        else if (b.getSpecial() == 4 || b.getSpecial() == 6) {
            return p1;
        }
    }
    if (a.getPower() <= 0 && b.getPower() <= 0) { // if both are charge/shields
        return 0; //nothing
    }
    if (b.getSpecial() == 4 || b.getSpecial() == 6) {
        if (a != b) { //reflect vs reflect = nothing
            return whoWins(b, a, 2, 1); //check everything again with values swapped
        }
        return 0; //if a == b
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
        return whoWins(b, a, 2, 1); //check everything again with values swapped
    }
}

int main()
{
    std::vector<std::pair<std::string, Move>> moves_vector;
    moves_vector = {
    {"f", {"Focus", 0, 0, 0, "Focus and add +1.5 to the bank. The next Charge will include the amount stored in the bank."}},
    {"c", {"Charge", 0, 0, 0, "Charge and add +1 MP (+Focus stored) to use abilities that have cost. Higher CPU difficulty means CPU gets more MP charged per cast."}},
    {"s", {"Shield", 0, -5, 0, "The most basic defense."}},
    {"p", {"Punch", 0.5, 1, 0, "Cost-efficient attack."}},
    {"t", {"Tackle", 1, 3, 0, "The most basic attack."}},
    {"b", {"Block", 1, -8, 0, "Mid-tier defense. Effective against BB/E/WP"}},
    {"sk", {"Scratch", 1.5, 4, 0, "Better than tackle."}},
    {"mp", {"Mega Punch", 1, 2, 5, "Crits against reflector types, useless otherwise."}},
    {"g", {"Guard", 1.5, -20, 0, "High-tier but very niche defense."}},
    {"bb", {"Brick Break", 3, 7, 1, "Decent attack. RPS: This wins against Water Pulse."}},
    {"e", {"Ember", 3, 7, 2, "Decent attack. RPS: This wins against Brick Break."}},
    {"wp", {"Water Pulse", 3, 7, 3, "Decent attack. RPS: This wins against Ember... I didn't come up with these move names."}},
    {"rf", {"Reflect", 2, -14, 4, "Reflects attacks weaker than this move's defense. Needs a nerf."}},
    {"tb", {"Thunderbolt", 4, 12, 0, "Strong attack."}},
    {"eb", {"Electro Ball", 5, 16, 0, "Very strong attack."}},
    {"rv", {"Revive", 3, 0, 7, "Revive to full health if you get attacked and have <25% health afterwards"}},
    {"lm", {"Light Mirror", 3.5, -100, 6, "The strongest reflect; Uno reverse card."}},
    {"sb", {"Solar Beam", 7, 50, 0, "The strongest attack."}}
    };

    std::vector<std::pair<std::string, Move>> fullMoveName_vector;
    for (auto const& move : moves_vector) {
        std::string lowercaseName = move.second.getName();
        std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(), ::tolower);
        fullMoveName_vector.emplace_back(lowercaseName, move.second);
    }

    int startingHP = 20;
    Player p1(0, 0, startingHP, 0);
    Player p2(0, 0, startingHP, 0);

    cout << ":  Rune  :" << endl;
    cout << ":. Wars .:" << endl;
    cout << ">>> Hint: type help [movename]" << endl;
    cout << "HP: " << p1._health << " | MP: " << p1._charges << " (" << p1._focusPower << ")" << " ::: " << "HP: " << p2._health << " | MP: " << p2._charges << " (" << p2._focusPower << ")" << endl;
    cout << ".:.: ";

    while (true) {
        // Get player input
        std::string input = "";
        currentAbbvMove = "";
        std::getline(std::cin, input);
        runCommand(input, moves_vector, fullMoveName_vector);
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
            cout << "Not enough charges" << endl;
            cout << ".:.: ";
            cin.clear();
            fflush(stdin);
            continue;
        }
        else {
            // Get random CPU input
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> uni(0, moves.size() - 1);
            int rngIndex = uni(rng);
            auto it = moves.begin();
            advance(it, rngIndex);
            std::string input2 = it->first;

            while (moves[input2].getCost() > p2._charges) {
                rngIndex = uni(rng);
                it = moves.begin();
                advance(it, rngIndex);
                input2 = it->first;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            //input = "mp";
            //input2 = "rf";
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            // Update player states
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
            cout << i1.getName() << " :.: " << i2.getName() << endl;
            int winner = whoWins(i1, i2, 1, 2);

            catchRecursion = 0;

            double extraDamage = 0;
            if (i1.getSpecial() == 5 && (i2.getSpecial() == 4 || i2.getSpecial() == 6)) { //if mega punch then crit reflectors
                extraDamage = 16;
            }
            if (i2.getSpecial() == 5 && (i1.getSpecial() == 4 || i1.getSpecial() == 6)) {
                extraDamage = 16;
            }
            double totaldamage1 = i1.getPower() + extraDamage;
            double totaldamage2 = i2.getPower() + extraDamage;
            double disrupt1 = round(totaldamage1 / 3);
            double disrupt2 = round(totaldamage2 / 3);

            if (winner == 1) {
                cout << "Player 1 ";
                if (i1.getSpecial() == 4 || i1.getSpecial() == 6) { //uno reverse card
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
                cout << "Player 2 ";
                if (i2.getSpecial() == 4 || i2.getSpecial() == 6) {
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
                cout << "Parry! (+2 each)" << endl;
                p1._charges += 2;
                p2._charges += 2;
            }
            if (winner == -2) {
                cout << "Can't determine winner" << endl;
                break;
            }
            if (i1.getSpecial() == 7) {
                if (i2.getPower() > 0) {
                    if (p1._health <= startingHP / 4) {
                        p1._health = startingHP;
                        cout << "Player 1 revived!" << endl;
                    }
                }
            }
            if (i2.getSpecial() == 7) {
                if (i1.getPower() > 0) {
                    if (p2._health <= startingHP / 4) {
                        p2._health = startingHP;
                        cout << "Player 2 revived!" << endl;
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
                cout << "Player 1 wins! (" << p1.scores << ":" << p2.scores << ")\n";
            }
            else if (p1._health <= 0) {
                p2.scores++;
                cout << "Player 2 wins! (" << p1.scores << ":" << p2.scores << ")\n";
            }
            if (p1._health <= 0 || p2._health <= 0) {
                handicap = p1.scores - p2.scores;
                std::cout << "Difficulty set to: " << (chargePower + handicap) << endl;
                p1.reset();
                p2.reset();
            }
            std::cout << "HP: " << p1._health << " | MP: " << p1._charges << " (" << p1._focusPower << ")" << " ::: " << "HP: " << p2._health << " | MP: " << p2._charges << " (" << p2._focusPower << ")" << std::endl;
            std::cout << ".:.: ";
        }
    }
}

    