#pragma once
#include <vector>
#include <string>
enum specialMoveConditions {
    none = 0,
    bb = 1,
    e,
    wp,
    rv = 7,
    rf = 11,
    lm,
    mp = 21,
    p
};
struct Move {
    std::string _name = "";
    double _cost = 0;
    double _power = 0;
    specialMoveConditions _special = none;
    std::string _desc = "";

    Move() : _name(""), _cost(0), _power(0), _special(none), _desc("") {}
    Move(std::string n, double c, double p, specialMoveConditions s, std::string d) : _name(n), _cost(c), _power(p), _special(s), _desc(d) {}

    std::string getName() const {
        return _name;
    }
    double getCost() const {
        return _cost;
    }
    double getPower() const {
        return _power;
    }
    specialMoveConditions getSpecial() const {
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
int whoWins(Move a, Move b, int p1, int p2);
static std::vector<std::pair<std::string, Move>> moves_vector = {
{"f", {"Focus", 0, 0, none, "Focus and add +1.5 to the bank. The next Charge will include the amount stored in the bank."}},
{"c", {"Charge", 0, 0, none, "Charge and add +1 MP (+Focus stored) to use abilities that have cost. Higher CPU difficulty means CPU gets more MP charged per cast."}},
{"s", {"Shield", 0, -5, none, "The most basic defense."}},
{"p", {"Punch", 0.5, 1, p, "Cost-efficient attack."}},
{"t", {"Tackle", 1, 3, none, "The most basic attack."}},
{"b", {"Block", 1, -8, none, "Mid-tier defense. Effective against BB/E/WP"}},
{"sk", {"Scratch", 1.5, 4, none, "Better than tackle."}},
{"mp", {"Mega Punch", 1, 2, mp, "Crits against reflector types, useless otherwise."}},
{"g", {"Guard", 1.5, -20, none, "High-tier but very niche defense."}},
{"bb", {"Brick Break", 3, 7, bb, "Decent attack. RPS: This wins against Water Pulse."}},
{"e", {"Ember", 3, 7, e, "Decent attack. RPS: This wins against Brick Break."}},
{"wp", {"Water Pulse", 3, 7, wp, "Decent attack. RPS: This wins against Ember... I didn't come up with these move names."}},
{"rf", {"Reflect", 2, -14, rf, "Reflects attacks weaker than this move's defense. Needs a nerf."}},
{"tb", {"Thunderbolt", 4, 12, none, "Strong attack."}},
{"eb", {"Electro Ball", 5, 16, none, "Very strong attack."}},
{"rv", {"Revive", 3, 0, rv, "Revive to full health if you get attacked and have <25% health afterwards"}},
{"lm", {"Light Mirror", 3.5, -100, lm, "The strongest reflect; Uno reverse card."}},
{"sb", {"Solar Beam", 7, 50, none, "The strongest attack."}}
};
std::vector<std::pair<std::string, Move>> createFullMoveNameVector(std::vector<std::pair<std::string, Move>>& moves_vector);


