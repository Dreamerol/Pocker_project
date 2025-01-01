PROJECT POCKER 10
I use Visual Studio

I have one main cpp file with all the functions for the program.

The project is about playing simple poker, so I decided to arrange the functionality into few sections:

1.I create structs which represent a card and a player.

2.The I create a function for dealing the cards, using rand() for the indexes of already dealed cards.

3.Then come the functions for evaluating the result based on the cards in every player's hand.

4.I also have a few validations functions for entering answers, or number of players.

5.In the main() function - I use while cycle for the games until the user enter No, I have used bool flags - indicating for a new game or a tie - I have also a nested for cycle - when asking every player for (r/c/f).

6.If there is a tie - we go back at the beginning of the program - new dealing - new array of dealed and the same algorhitm is executed!
