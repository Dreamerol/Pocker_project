/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
*
* @author Mihaela Lachezareva Koseva
* @idnumber 6MI0600443
* @compiler VC / Visual Studio
*
* <main file with the functions, used for the project>
*
*/

//LINK GITHUB
// https://github.com/Dreamerol/Pocker_project
//#include "Functions.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
//#include "Functions.cpp"
//#include <ofstream>
#include <fstream>

//These are all the constants that we will need

int const NUMBER_OF_TYPES_OF_CARDS = 8; //there are cards from 7 to A;
int const NUMBER_OF_SUITS = 4;
int const NUMBER_OF_ALL_CARDS = 32; // 4 suits * 8 types of cards
int const NUMBER_OF_CARDS_IN_A_HAND = 3; // we know that every player deals three cards at the beginning
int const CHIP_VALUE = 10;
int const ARRAY_SIZE = 32; //the maximum number of possible cards is 32
int const MAX_NUMBER_OF_PLAYERS = 9;
int const MAX_GAINED_MONEY = 3 * 100 * CHIP_VALUE; // the count of all chips in the game

//I created constants with some of the types or suits of cards so to avoid using magical numbers in the code
int const ACE = 14; // number of ace card
int const SEVEN = 7;
char const CLUBS = 'C';
int const SEVEN_CLUBS = 11; //the points of the card seven clubs

//the points of essential combinations
int const POINTS_PAIR_SEVEN = 23; // pair of seven
int const POINTS_THREE_SEVEN = 34; //having three cards seven in a hand

//There are all the types of cards - I decided to use an int array and for cards J to A - I have 
//I have decided to make an int array of the types of cards to be easier for evaluating the result
//So 11 is the J
//11 - J
//12 - Q
//13 - K
//14 - A
int cards[NUMBER_OF_TYPES_OF_CARDS] = { 7, 8, 9, 10, 11, 12, 13, 14 };
//we also create a char array for the suits
char suits[NUMBER_OF_SUITS] = { 'C', 'D', 'H', 'S' };//clubs (♣), diamonds ( ), hearts (♥) and spades (♠)

//I decided for the cards to use structs - the number will represent the type of card - 
// for example 13 is K, the other variable is for the suit

struct card
{
    int number;
    char suit;

};

//The deck of cards will be an array of structs with constant size, because every game is played with 32 cards
card deck[NUMBER_OF_ALL_CARDS];   // we create an array of structs - each struct represent every card


//I also decided to represent each player with a struct
//Every player will have:
//id - from one to n
//an array of cards - the three cards that he will have in his hand
//balance - the value from all the chips he has
//result - which is the calculated points - evaluated based on the cards he has
//is_playing - a status whether the player is still in the game or has fold
//has_called - a status whether the player has called in the game
//given_virtual_points - how many points the player has already given by the beginning of the game

struct player
{
    int id;
    card cards_in_hand[NUMBER_OF_CARDS_IN_A_HAND];
    int balance = 100 * CHIP_VALUE;
    int result = 0;
    bool is_playing = true;
    bool has_called = false;
    int given_virtual_points = 0;

};
//The user will enter until he enters a valid number
//checks if the element is in the array
bool is_in_array(int indexes[], int& len_indexes, int idx) {
    for (int i = 0;i < len_indexes;i++) {
        if (idx == indexes[i]) {
            return 1;
        }
    }
    return 0;
}
//gives an idx of a card that isn't dealed yet
int generate_idx_for_deck(int indexes[], int& len_indexes) {
    int idx = rand() % NUMBER_OF_ALL_CARDS;
    while (is_in_array(indexes, len_indexes, idx))
    {
        idx = rand() % NUMBER_OF_ALL_CARDS;
    }
    indexes[len_indexes++] = idx;
    return idx;
}

//a function for dealing the three cards
void cards_deal_array(card hand_cards[NUMBER_OF_CARDS_IN_A_HAND], int indexes[], int& len_indexes) {
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
        int idx = generate_idx_for_deck(indexes, len_indexes);
        hand_cards[i].number = deck[idx].number;
        hand_cards[i].suit = deck[idx].suit;
    }

}

//These lines contains the functnionality behind calculating the result
void swap(card& a, card& b) {
    card help = a;
    a = b;
    b = help;
}
void swap_suit(char& a, char& b) {
    char help = a;
    a = b;
    b = help;
}
void swap_number(int& a, int& b) {
    int help = a;
    a = b;
    b = help;
}
void swap_players(player& a, player& b) {
    player help = a;
    a = b;
    b = help;
}

//Selection Sort functions
//I sort the cards as objects based on their suit or type
void sort_by_number(card cards_in_hand[]) {
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND - 1;i++) {
        int min_idx = i;
        for (int j = i;j < NUMBER_OF_CARDS_IN_A_HAND;j++) {
            if (cards_in_hand[min_idx].number > cards_in_hand[j].number) {
                min_idx = j;
            }
        }
        if (i != min_idx) {
            swap(cards_in_hand[min_idx], cards_in_hand[i]);
        }
    }
}

void sort_by_suit(card cards_in_hand[]) {
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND - 1;i++) {
        int min_idx = i;
        for (int j = i;j < NUMBER_OF_CARDS_IN_A_HAND;j++) {
            if (cards_in_hand[min_idx].suit > cards_in_hand[j].suit) {
                min_idx = j;
            }
        }
        if (i != min_idx) {
            swap(cards_in_hand[min_idx], cards_in_hand[i]);
        }
    }
}

void sort_by_balance(player players[], int number_of_players) {
    for (int i = 0;i < number_of_players - 1;i++) {
        int min_idx = i;
        for (int j = i + 1;j < number_of_players;j++) {
            if (players[min_idx].balance > players[j].balance) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap_players(players[min_idx], players[i]);
        }
    }
}

//checks if the player has three equal cards
bool three_equal_cards(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND - 1;i++) {
        if (cards_in_hand[i].number != cards_in_hand[i + 1].number) {
            return 0;
        }

    }
    return 1;
}
//checks if the player has three equal suits
bool three_equal_suits(card cards_in_hand[]) {
    sort_by_suit(cards_in_hand);
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND - 1;i++) {
        if (cards_in_hand[i].suit != cards_in_hand[i + 1].suit) {
            return 0;
        }

    }
    return 1;
}
//checks if the player has two equal cards/suits
bool two_equal_suits(card cards_in_hand[]) {
    sort_by_suit(cards_in_hand);
    return(cards_in_hand[1].suit == cards_in_hand[0].suit || cards_in_hand[1].suit == cards_in_hand[2].suit);

}


bool two_equal_cards(card cards_in_hand[], int card_type) {
    int count = 0;
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++)
    {
        if (cards_in_hand[i].number == card_type) {
            count++;
        }
    }
    return (count == 2);
}

//a function that converts the type of card into points
//from 11 to 13 - are J, Q and K - which are 10 points
//and 14 - A - is 11 points

int to_points_converter(int type_of_card)
{
    if (type_of_card > 10 && type_of_card < 14) {
        return 10;
    }
    else if (type_of_card > 13) {
        return 11;
    }
    else {
        return type_of_card;
    }

}
//it converts the int type of card into the well-known card chars - K. Q, A, J; 
char int_to_card_type_converter(int card) {
    if (card == 11) {
        return 'J';
    }
    else if (card == 12) {
        return 'Q';
    }
    else if (card == 13) {
        return 'K';

    }
    else {
        return 'A';
    }
}
//checks whether the player has seven clubs in his hand
bool seven_clubs_in_hand(card cards_in_hand[])
{
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++)
    {
        if (cards_in_hand[i].number == SEVEN && cards_in_hand[i].suit == CLUBS)
        {
            return 1;
        }
    }
    return 0;
}


//when we sort the cards in the hand - the equal ones will be one to another
bool two_equal_numbers_seven_clubs(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    //when we sort them by number - the 7 will be the firts , because 7 is lower than all the other types of cards
    return (cards_in_hand[1].number == cards_in_hand[2].number);
}

//Analogically when we sort by suit - the C is the character with the smallest ASCII value from the set of suits
//the same ones would be next to each other on 1 and 2 position
bool two_equal_suits_seven_clubs(card cards_in_hand[]) {
    sort_by_suit(cards_in_hand);
    return (cards_in_hand[1].suit == cards_in_hand[2].suit);
}

//returns the biggest card
int max_card(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    return cards_in_hand[2].number;
}
//when we have the case for three equal suits - the result is the sum from the points of each card
int sum_of_all_cards(card cards_in_hand[]) {
    int result = 0;
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
        result += to_points_converter(cards_in_hand[i].number);
    }
    return result;
}

//calculating the result
int result_of_points(card cards_in_hand[]) {
    int result = 0;
    //first we check if there are three equal cards
    if (three_equal_cards(cards_in_hand)) {
        if (cards_in_hand[0].number == SEVEN) {
            result = POINTS_THREE_SEVEN;//if the player has 3 sevens this is the max result - 34
        }
        else {
            result = 3 * to_points_converter(cards_in_hand[0].number);
        }
    }
    else if (seven_clubs_in_hand(cards_in_hand)) {
        //we sort the cards - the seven clubs will be at position 
        sort_by_number(cards_in_hand);
        if (two_equal_numbers_seven_clubs(cards_in_hand)) {
            result = 2 * to_points_converter(cards_in_hand[1].number);
        }
        else if (two_equal_suits_seven_clubs(cards_in_hand)) {
            result = to_points_converter(cards_in_hand[1].number) + to_points_converter(cards_in_hand[2].number);
        }
        else {
            //if there aren't two equal suits, or types of cards - we get the bigger card
            int bigger = (cards_in_hand[1].number > cards_in_hand[2].number) ? cards_in_hand[1].number : cards_in_hand[2].number;
            result = to_points_converter(bigger);
        }
        //finally we add the value of seven clubs
        result += SEVEN_CLUBS;
    }
    else {
        //if there isn't seven clubs we check whether there is a pair or equal suits
        //the pair gives points when is only formed by aces or sevens otherwise we check for the suits
        if (two_equal_cards(cards_in_hand, ACE)) result = 2 * to_points_converter(ACE);
        else if (two_equal_cards(cards_in_hand, SEVEN)) result = POINTS_PAIR_SEVEN;//we created a constant for the points each combination gives us
        else if (three_equal_suits(cards_in_hand)) result = sum_of_all_cards(cards_in_hand);
        else if (two_equal_suits(cards_in_hand)) {

            result = to_points_converter(cards_in_hand[1].number);
            result += to_points_converter(cards_in_hand[0].suit == cards_in_hand[1].suit ? cards_in_hand[0].number : cards_in_hand[2].number);
        }
        //finally if there are no combinations we just take the max card
        else result = to_points_converter(max_card(cards_in_hand));
    }
    return result;
}

//validator for the number of players
void validation_number_of_players(int& number_of_players) {
    std::cout << "How many players are going to play (2-9)? ";

    //the user must enter until he enter a valid number
    std::cin >> number_of_players;
    while (std::cin.fail() || (number_of_players > 9) || (number_of_players < 2))
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');//moving the pointer
        std::cout << "You should enter a number between 2 and 9! ";
        std::cin >> number_of_players;
    }
   
}

//validation for entering an answer, when the player can't raise - it should be c - call or f - fold
void validation_unable_to_raise(char& decision) {
    std::cin >> decision;
    while (std::cin.fail() || (decision != 'c' && decision != 'f'))
    {

        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "You should enter a valid character! ";
        std::cin >> decision;
    }

}

//if the raise is bigger
bool raise_bigger_than_players_balance(player players[], int number_of_players, int last_raise) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing && players[i].balance < last_raise) {
            return 1;
        }
    }
    return 0;
}

//in the beginning of every game - all the players give 1 CHIP, so we need to substract CHIP_VALUE from their balances
//Also we add the CHIP_VALUE to their virtual_points
void beggining_of_the_game(player players[], int& pot, int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            if (players[i].balance >= CHIP_VALUE) {
                players[i].balance -= CHIP_VALUE;
                pot += CHIP_VALUE;
                players[i].given_virtual_points = CHIP_VALUE;

            }
            else {
                players[i].is_playing = false;
                pot += players[i].balance;
                players[i].balance = 0;
            }
        }
    }
}

//returns the minimum balance in the game
int min_balance(player players[], int number_of_players) {
    int min = MAX_GAINED_MONEY;
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing && min > players[i].balance) {
            min = players[i].balance;
        }
    }
    return min;
}

//It removes the given raise from player's balance
void given_raise(int number_of_players, player players[], int last_raise, int& pot) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            players[i].balance -= last_raise;
            pot += last_raise;
            players[i].given_virtual_points += last_raise;
        }
    }

}
//It prints the balances of the players, who are still in the game
void status_of_players(int number_of_players, player players[], int last_raise, int& pot) {
    std::cout << "Last raise: " << last_raise << '\n';
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            std::cout << "Player" << players[i].id << " balance: " << players[i].balance << '\n';

        }
    }
    std::cout << "Pot: " << pot << '\n';

}

//Conditions for ending the game
bool all_players_has_called(player players[], int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            if (!players[i].has_called) {
                return 0;
            }
        }
    }
    return 1;
}
//Or just one player has left in the game
bool just_one_player_left(player players[], int number_of_players) {
    int count = 0;
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            count++;

        }
    }
    return (count == 1);
}

//Finding the winner based on the result
//calculating the max result
int max_result(player players[], int number_of_players) {
    int max = 0;
    for (int i = 0;i < number_of_players;i++) {
        if(players[i].is_playing){
        if (max < players[i].result) max = players[i].result;
    }}
    return max;
}
//returns the idx of the player with max result
int max_result_idx(player players[], int number_of_players) {
    int max = 0;
    int max_idx = -1;
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            if (max < players[i].result) {
                max = players[i].result;
                max_idx = i;
            }
        }
    }
    return max_idx;
}
//function checking if there is a tie
bool is_tie(player players[], int number_of_players) {
    int max = max_result(players, number_of_players);
    int count = 0;
    for (int i = 0;i < number_of_players;i++) {
        if (max == players[i].result && players[i].is_playing) count++;
    }
    return (count > 1);//if there are more than one person with a max result
}


void validation_raise(int& raise, int& last_bet, player players[], int number_of_players, int idx, bool& able_to_raise) {
    std::cout << "Your raise is: ";
    std::cin >> raise;
    int min_limit = last_bet + CHIP_VALUE;
    int max_limit = min_balance(players, number_of_players);
    //I create two variables for the raise conditions - the player can raise when the his raise is bigger than the last bet(raise) + CHIP_VALUE
    //The max limit for raise is to not get bigger than someone's balance(the lowest balance) in the game
    while (std::cin.fail() || (raise < min_limit) || (raise_bigger_than_players_balance(players, number_of_players, raise)))
    {
        //this is a corner case - for example when the smallest balance is 100, but last raise is 110 - a number 110 < number < 100 - doesn't exist
        //without this condition - (max_limit < min_limit) we wouldn't be able to get out the while cycle
        if (max_limit < min_limit) {
            std::cout << "You are not able to raise! Call or fold (c/f): ";
            able_to_raise = false;
            char decision;
            //when player is unable to raise - he can either fold or call
            validation_unable_to_raise(decision);
            if (decision == 'f') {
                players[idx].is_playing = false;
                //the player isn,t playing anymore
            }
            else {
                players[idx].has_called = true;
            }
            break;

        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        //if the raise isn't big enough - the player should enter a bigger number
        if ((raise < last_bet + CHIP_VALUE)) {
            std::cout << "You should enter a valid raise! Bigger than " << min_limit << ": ";
        }
        //if the raise is too big - he must enter a number smaller than min balance
        if ((raise_bigger_than_players_balance(players, number_of_players, raise))) {
            std::cout << "You should enter a valid raise! Smaller than " << max_limit << ": ";
        }

        std::cin >> raise;
    }

}

//Validating the answer for joining the tie - y or n;
void validation_of_joining_the_tie(char& answer) {

    std::cin >> answer;
    while (std::cin.fail() || (answer != 'y' && answer != 'n'))
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');//moving the pointer
        std::cout << "You should enter a valid answer! ";
        std::cin >> answer;
    }

}

void joining_the_tie(player players[], int& pot, int number_of_players) {
    int max = max_result(players, number_of_players);
    int value_to_pay_to_join_the_tie = pot / 2; // the player has to pay one half of the pot to join the tie
    for (int i = 0;i < number_of_players;i++) {
        //Those players who have the max result have joined the tie by default, but for those, whose result < max_result are forced to pay 
        //if they want to join the game
        //we want payment just from the players, who don't have the max result whether to join or not
        if (players[i].result != max) {
            std::cout << "Player" << players[i].id << " do you want to join the tie? You have to pay " << value_to_pay_to_join_the_tie << " (y / n) : ";

            char answer;
            validation_of_joining_the_tie(answer);

            if (answer == 'y') {
                if (players[i].balance < value_to_pay_to_join_the_tie) {
                    std::cout << "You don't have the money to join the tie!" << '\n';
                    players[i].is_playing = false;
                    //if the player doesn't have the money he can't join - so is_playing = false 
                }
                else {
                    //the player pays
                    players[i].balance -= value_to_pay_to_join_the_tie;
                    players[i].is_playing = true;
                }
            }
            //if the player has said no - he isn't playing anymore
            else {
                players[i].is_playing = false;
            }
        }

        else {
            //for the players with max result
            //if they have balance 0 - automatically are given 50 points to their balance
            if (players[i].balance == 0) {
                players[i].balance += 50;
            }
        }
    }
}

//asking the user for another game
void do_you_want_to_play_again(char& answer) {
    std::cout << "Do you want to play again? (y/n): ";

    std::cin >> answer;
    while (std::cin.fail() || (answer != 'y' && answer != 'n')) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "You must enter a valid character (y/n): ";
        std::cin >> answer;
    }

}

//In the end of the game - if the user have chosen - n - the balance and results of the player are written in a file; 
void write_results_in_file(player players[], int number_of_players) {
    sort_by_balance(players, number_of_players);
    std::ofstream MyFile("results.txt");
    if(!MyFile.is_open()){
        std::cerr<<"Cannot open file!";
    }
    for (int i = number_of_players - 1;i >= 0;i--) {
        MyFile << "Player" << players[i].id << " with balance " << players[i].balance << "!" << '\n';
    }
    MyFile.close();
}

//NEW GAME
//Restarting the players' status
void restart_players_status(player players[], int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].balance > 0) {
            players[i].has_called = false;
            players[i].is_playing = true; //all the players with balance > 0 are back in the game
        }
        else {
            players[i].is_playing = false;
        }
    }
}
void restart_players_status_when_tie(player players[], int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        //when there is a tie - all the players who are playing continue in the game
        //has_called - false - is again false - because in the new game the player hasn't still called 
        if (players[i].is_playing) {
            players[i].has_called = false;
        }
    }
}
int main() {



    std::cout << "WELCOME TO FMI CASINO!" << '\n';
    std::cout << "Are you tired of Mathematics? Why don't you try becoming a billionaire! ;)"<<'\n';
    std::cout << "Who is going to be the next billionaire? It's time to find out!" << '\n';



    //the creation of the deck
    int idx_of_deck = 0;
    for (int i = 0;i < NUMBER_OF_SUITS;i++) {
        for (int j = 0;j < NUMBER_OF_TYPES_OF_CARDS;j++) {
            deck[idx_of_deck].suit = suits[i];
            deck[idx_of_deck].number = cards[j];
            idx_of_deck++;
        }
    }
    //printing the cards of the players
    /* for (int i = 0;i < idx_of_deck;i++) {
         std::cout << deck[i].number << " " << deck[i].suit<<'\n';
     }
    */
    bool another_game = true;
    bool not_first_game = false;
    int number_of_players;

    validation_number_of_players(number_of_players);
    
 
    player* players = new player[number_of_players];
    int indexes[NUMBER_OF_ALL_CARDS];
    int len_indexes = 0;
    for (int i = 0;i < number_of_players;i++)
    {
        player pocker_player;
        pocker_player.id = i + 1;
        cards_deal_array(pocker_player.cards_in_hand, indexes, len_indexes);
        players[i] = pocker_player;


    }
    bool is_tie_flag = false;
    int pot = 0;
    int count = 0;

    while (another_game)
    {
        int last_bet = 0;
        //This is the case, when the user has asked for another game
        if (not_first_game) {
            pot = 0;
            //len_indexes = 0
            //we declare an array for the new dealed card indexes
            int indexes[NUMBER_OF_ALL_CARDS];
            int len_indexes = 0;

            //we restart the status - players with balance > 0 - is_playing = true;
            restart_players_status(players, number_of_players);
            for (int i = 0;i < number_of_players;i++) {
                std::cout << "Player" <<i + 1<< " " << players[i].balance << '\n';
            }
            //we deal them new three cards
            for (int i = 0;i < number_of_players;i++) {
                if (players[i].is_playing) {

                    cards_deal_array(players[i].cards_in_hand, indexes, len_indexes);
                }
            }

        }
          
        //If there is a tie
        if (is_tie_flag) {
            //again we declare an array for the new dealed card indexes
            int indexes[NUMBER_OF_ALL_CARDS];
            int len_indexes = 0;

            for (int i = 0;i < number_of_players;i++) {
                if (players[i].is_playing) {
                    cards_deal_array(players[i].cards_in_hand, indexes, len_indexes);
                }
            }
            is_tie_flag = false;// we don't know if there will be a tie in the new game
            //we restart the players' status - those who are still in the game
            restart_players_status_when_tie(players, number_of_players);
        }
        //Printing players' cards
       /* for (int i = 0;i < number_of_players;i++) {
            if (players[i].is_playing) {
                for (int j = 0;j < 3;j++) {
                    std::cout << players[i].cards_in_hand[j].number << " " << players[i].cards_in_hand[j].suit << '\n';
                }
            }
        }
        */
        //Now we are going to evaluate the result - count of points
        for (int i = 0;i < number_of_players;i++)
        {
            if (players[i].is_playing) {
                players[i].result = result_of_points(players[i].cards_in_hand);
                //std::cout<<players[i].result<<'\n';
            }
        }


        //edge case equal results
         /*if (count == 0) {
             for (int i = 0;i < number_of_players;i++)
             {
                 if (players[i].is_playing) {
                     players[i].result = 10;
                 }
             }
             players[0].result = 9;
             
         }
         else {
             for (int i = 0;i < number_of_players;i++)
             {
                 if (players[i].is_playing) {
                     players[i].result = i;
                 }
             }
         }*/


       //Printing the cards of the players
      /*  for (int i = 0;i < number_of_players;i++) {
            if (players[i].is_playing)
                for (int j = 0;j < 3;j++) {
                    std::cout << players[i].cards_in_hand[j].number << " " << players[i].cards_in_hand[j].suit << '\n';
                }
            std::cout << players[i].result << '\n';
        }*/



        /*  for (int i = 0;i < number_of_players;i++) {
              std::cout << players[i].balance<<' ';

          }
          std::cout << pot;*/

         //Now it's time to begin the game
        beggining_of_the_game(players, pot, number_of_players);
        

        //we are playing until one player is left or all the players have called
        while (!all_players_has_called(players, number_of_players) && (!just_one_player_left(players, number_of_players))) {
            int raise = 0;

            for (int i = 0;i < number_of_players;i++) {
                //we move through the players and ask each one of them - r, c, f
                if (just_one_player_left(players, number_of_players) || all_players_has_called(players, number_of_players)) {
                    //if one of the conditions - true - we break the cycle
                    break;
                }
                //if the player is still playing - he is asked - r,f,c
                if (players[i].is_playing)
                {
                    std::cout << "Player" << players[i].id << " raise, call or fold? (r/c/f): ";
                    char decision;
                    std::cin >> decision;
                    while (std::cin.fail() || (decision != 'c' && decision != 'r' && decision != 'f'))
                    {
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');//moving the pointer
                        std::cout << "You should enter a valid character (r/c/f)! ";
                        std::cin >> decision;
                    }
                    bool able_to_raise = true;

                    if (decision == 'r') {
                        raise = 0;
                        validation_raise(raise, last_bet, players, number_of_players, i, able_to_raise);
                        /* if (able_to_raise) {
                             given_raise(number_of_players, players, raise, pot);
                         }*/
                        players[i].given_virtual_points += raise; //we add this raise to player's given_virtual points
                        players[i].balance -= raise;//we substract from his balance the given raise
                        pot += raise; // the pot + given_raise
                        status_of_players(number_of_players, players, raise, pot);//we can print to see the players'
                    }
                    else if (decision == 'f') {
                        //when a player folds - he is out of the game
                        players[i].is_playing = false;
                    }
                    else {

                        //std::cout << raise << ' ' << players[i].given_virtual_points << '\n';
                        if (raise > players[i].given_virtual_points) {
                            //A player can call if the given raise is bigger than the given_virtual_points of the player
                            players[i].has_called = true;
                            players[i].given_virtual_points += raise;
                            players[i].balance -= raise;
                            pot += raise;

                        }
                        else {
                            std::cout << "You can not call!" << players[i].given_virtual_points << '\n';
                            //If the player can't call to the raise - he can't play - so he is out of the game
                            players[i].is_playing = false;
                        }
                        //std::cout << all_players_has_called(players, number_of_players);
                    }

                    last_bet = raise;

                }
            }
        }
        if (is_tie(players, number_of_players)) {
            std::cout << "IT IS A TIE :)" << '\n';
            joining_the_tie(players, pot, number_of_players);

            is_tie_flag = true;//flag indicating the tie in the game



        }

        else {
            //The winner gets the amount of money from the pot

            int max_idx = max_result_idx(players, number_of_players);//find the idx of the winner - player with max result
            players[max_idx].balance += pot;
            //printing the winner's status
            std::cout << "The winner is Player" << players[max_idx].id << " with " << players[max_idx].balance << "!" << '\n';
            for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
                //we convert the int card_types to the well-known - K, J, A, Q
                if (players[max_idx].cards_in_hand[i].number > 10) {
                    std::cout << int_to_card_type_converter(players[max_idx].cards_in_hand[i].number);
                }
                else {
                    std::cout << players[max_idx].cards_in_hand[i].number;

                }
                //printing the winner's cards combination
                std::cout << players[max_idx].cards_in_hand[i].suit << ' ';
            }
            //His reult
            std::cout << players[max_idx].result << '\n';
            char answer;

            //Asking the user for another game
            do_you_want_to_play_again(answer);
            if (answer == 'y') {
                not_first_game = true;
            }
            else {
                //we write the results and the status of the players in a file
                write_results_in_file(players, number_of_players);
                another_game = false; // there is no another game the flag = false
                //we get out from the while cycle with condition - another game
            }
        }

    }
    //we free the dynamic memory
    delete[] players;
    return 0;

}

