# runewars
rock paper scissors but it's complicated

## How to play
(the list of moves is available via typing `help`)

### Basics

In this unfinished game, 2 players `You :.: CPU` face each other, each starting with 20 health and 0 mana.

to help you understand better, let's simplify the game into just 3 moves that are available: Charge, Tackle, Shield

(also please note that MP and mana are used interchangably)

Charge gives you +1 MP, this allows you to use Tackle which costs 1 MP, example inputs: 

`.:.: c`

`.:.: t`

which will display:

`Charge :.: (some move)`

`Tackle :.: (some move)`

To defend against weak attacks like Tackle you can cast Shield (doesn't cost MP), example:

`Charge :.: Charge`

`Shield :.: Tackle`

`Tackle :.: Shield`

In that scenario, no one receives any damage.

(P.S. the `:.:` is just a stylized "vs" kinda like "Shield vs Tackle" or something idk. It looks cool. UI is hard.)

Note: if def power >= atk power, damage is mitigated. For more info on the move stats (e.g. Tackle) just type `help tackle` or just `help t` in the game.

### Parrying

If both players *attack* with the same move, no damage is dealt and each gets an additional 2 MP.

Special interaction: Mega Punch parries Punch. Don't ask why.

### Basics 2

Now in this hypothetical game with only 3 moves, there is a core gameplay flaw where you can cast Shield indefinitely.

So let's introduce 2 more moves: 
- Ember (3 MP) which can pierce Shield.
- Block (1 MP) which can negate Ember.

scenario:

`1. Charge :.: Charge`

`2. Shield :.: Charge`

`3. Shield :.: Charge`

Let's look at your options for this scenario:

`4. Shield :.: Ember`
This would be good if the opponent decides to use Tackle instead. Unfortunately, they didn't. You will take full damage.

`4. Block :.: Ember`
This is good for you since it resets the battle. No one receives any damage here.

`4. Charge :.: Ember`
Ouch. It's a good time to tell you a core mechanic in this game: 

### Mana Disruption

All attacks disrupt *some* of the opponent's mana and *all* of the opponent's stored focus (except when they disrupt 0 mana which obviously doesn't disrupt mana).
The amount of mana disrupted is the total damage given divided by 3.

In the case of `Charge :.: Ember`, even though you've casted Charge, you will lose your mana again due to the strong attack.

## あとがき

There are other moves and other mechanics that haven't been covered yet but I'm sure you'll figure it out by playing. Have fun!
