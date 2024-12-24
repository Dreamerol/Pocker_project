// pocker_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include "test.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
//#include "Functions.cpp"
//#include <ofstream>
#include <fstream>


int const NUMBER_OF_TYPES_OF_CARDS = 8;
int const NUMBER_OF_SUITS = 4;
int const NUMBER_OF_ALL_CARDS = 32; // 4 suits * 8 types of cards
int const NUMBER_OF_CARDS_IN_A_HAND = 3;
int const CHIP_VALUE = 10;
int const ARRAY_SIZE = 32; //the maximum number of possible cards is 32
int const MAX_NUMBER_OF_PLAYERS = 9;
int const MAX_GAINED_MONEY = 3 * 100 * CHIP_VALUE;

int const ACE = 14;
int const SEVEN = 7;
char const CLUBS = 'C';
int const SEVEN_CLUBS = 11;

int cards[NUMBER_OF_TYPES_OF_CARDS] = { 7, 8, 9, 10, 11, 12, 13, 14 };

int const POINTS_PAIR_SEVEN = 23;
int const POINTS_THREE_SEVEN = 4;
//I have decided to make an int array of the types of cards to be easier for evaluating the result
//So 11 is the J
//11 - J
//12 - Q
//13 - K
//14 - A

char suits[NUMBER_OF_SUITS] = { 'C', 'D', 'H', 'S' };//clubs (♣), diamonds ( ), hearts (♥) and spades (♠)


struct card
{
    int number;
    char suit;

};

card deck[NUMBER_OF_ALL_CARDS];   // we create an array of structs - each struct represent every card


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

bool is_in_array(int indexes[], int& len_indexes, int idx) {
    for (int i = 0;i < len_indexes;i++) {
        if (idx == indexes[i]) {
            return 1;
        }
    }
    return 0;
}

int generate_idx_for_deck(int indexes[], int& len_indexes) {
    int idx = rand() % NUMBER_OF_ALL_CARDS;
    while (is_in_array(indexes, len_indexes, idx))
    {
        idx = rand() % NUMBER_OF_ALL_CARDS;
    }
    indexes[len_indexes++] = idx;
    return idx;
}

void cards_deal_array(card hand_cards[NUMBER_OF_CARDS_IN_A_HAND], int indexes[], int& len_indexes) {
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
        int idx = generate_idx_for_deck(indexes, len_indexes);
        hand_cards[i].number = deck[idx].number;
        hand_cards[i].suit = deck[idx].suit;
    }

}

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

bool three_equal_cards(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND - 1;i++) {
        if (cards_in_hand[i].number != cards_in_hand[i+1].number) {
            return 0;
        }

    }
    return 1;
}
bool three_equal_suits(card cards_in_hand[]) {
    sort_by_suit(cards_in_hand);
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND - 1;i++) {
        if (cards_in_hand[i].suit != cards_in_hand[i + 1].suit) {
            return 0;
        }

    }
    return 1;
}

bool two_equal_suits(card cards_in_hand[]) {
    sort_by_suit(cards_in_hand);
    return(cards_in_hand[1].suit == cards_in_hand[0].suit || cards_in_hand[1].suit == cards_in_hand[2].suit);
    
}

bool two_equal_numbers_one(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    return(cards_in_hand[1].number == cards_in_hand[0].number || cards_in_hand[1].number == cards_in_hand[2].number);


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



bool two_equal_numbers_seven_clubs(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    return (cards_in_hand[1].number == cards_in_hand[2].number);
}


bool two_equal_suits_seven_clubs(card cards_in_hand[]) {
    sort_by_suit(cards_in_hand);
    return (cards_in_hand[1].suit == cards_in_hand[2].suit);
}


int max_card(card cards_in_hand[]) {
    sort_by_number(cards_in_hand);
    return cards_in_hand[2].number;
}
int sum_of_all_cards(card cards_in_hand[]) {
    int result = 0;
    for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
        result += to_points_converter(cards_in_hand[i].number);
    }
    return result;
}


int result_of_points(card cards_in_hand[]){
    int result = 0;
    if (three_equal_cards(cards_in_hand)) {
        if (cards_in_hand[0].number == SEVEN) {
            result = POINTS_THREE_SEVEN;
        }
        else {
            result = 3 * to_points_converter(cards_in_hand[0].number);
        }
    }
    else if (seven_clubs_in_hand(cards_in_hand)) {
        if (two_equal_numbers_seven_clubs(cards_in_hand)) {
            result = 2 * to_points_converter(cards_in_hand[1].number);
        }
        else if(two_equal_suits_seven_clubs(cards_in_hand)){
            result = to_points_converter(cards_in_hand[1].number) + to_points_converter(cards_in_hand[2].number);
        }
        else {
             int bigger = (cards_in_hand[1].number > cards_in_hand[2].number) ? cards_in_hand[1].number : cards_in_hand[2].number;
             result = to_points_converter(bigger);
        }
        result += SEVEN_CLUBS;
    }
    else {
        
        if (two_equal_cards(cards_in_hand, ACE)) result = 2 * to_points_converter(ACE);
        else if (two_equal_cards(cards_in_hand, SEVEN)) result = POINTS_PAIR_SEVEN;
        else if (three_equal_suits(cards_in_hand)) result = sum_of_all_cards(cards_in_hand);
        else if (two_equal_suits(cards_in_hand)) {
       
            result = to_points_converter(cards_in_hand[1].number);
            result += to_points_converter(cards_in_hand[0].suit == cards_in_hand[1].suit ? cards_in_hand[0].number : cards_in_hand[2].number);
        }
        else result = to_points_converter(max_card(cards_in_hand));
    }
    return result;
}

//int result_of_points(card cards_in_hand[]) {
//    int result = 0;
//    bool is_seven_clubs_in_hand = seven_clubs_in_hand(cards_in_hand);
//
//    if (three_equal_cards(cards_in_hand)) {
//        if (cards_in_hand[0].number == 7)
//        {
//            result = 34;
//        }
//        else
//        {
//            result = NUMBER_OF_CARDS_IN_A_HAND * to_points_converter(cards_in_hand[0].number);
//        }
//    }
//    else if (three_equal_suits(cards_in_hand))
//    {
//        for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
//            result += to_points_converter(cards_in_hand[i].number);
//        }
//    }
//    else if (two_equal_suits(cards_in_hand)) {
//        if (cards_in_hand[0].suit == cards_in_hand[1].suit) {
//            result += to_points_converter(cards_in_hand[0].number) + to_points_converter(cards_in_hand[1].number);
//        }
//        else {
//            result += to_points_converter(cards_in_hand[1].number) + to_points_converter(cards_in_hand[2].number);
//        }
//    }
//    {
//     
//
//    }
//    else if (two_equal_cards(cards_in_hand, seven))
//    {
//        result = 23;
//    }
//    else if (seven_clubs_in_hand(cards_in_hand)) {
//
//    }
//    {
//
//    }
//
//
//
//
//    return result;
//}

void beggining_of_the_game(player players[], int& pot, int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            if (players[i].balance >= CHIP_VALUE) {
                players[i].balance -= CHIP_VALUE;
                pot += CHIP_VALUE;
                players[i].given_virtual_points = CHIP_VALUE;
                /*players[i].given_virtual_points += CHIP_VALUE;
                players[i].given_virtual_points = 0;*/
            }
            else {
                players[i].is_playing = false;
                pot += players[i].balance;
                players[i].balance = 0;
            }
        }
    }
}
bool raise_bigger_than_players_balance(player players[], int number_of_players, int last_raise) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing && players[i].balance < last_raise) {
            return 1;
        }
    }
    return 0;
}
int min_balance(player players[], int number_of_players) {
    int min = MAX_GAINED_MONEY;
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing && min > players[i].balance) {
            min = players[i].balance;
        }
    }
    return min;
}
void validation_unable_to_raise(char& decision) {
    std::cin >> decision;
    while (std::cin.fail() ||(decision != 'c' && decision != 'f'))
    {
        
        std::cin.clear();
        std::cin.ignore(1000, '\n');//moving the pointer
        std::cout << "You should enter a valid character! ";
        std::cin >> decision;
    }

}

void validation_raise(int& raise, int& last_bet, player players[], int number_of_players, int idx, bool& able_to_raise) {
    std::cout << "Your raise is: ";
    std::cin >> raise;
    int min_limit = last_bet + CHIP_VALUE;
    int max_limit = min_balance(players, number_of_players);
    while (std::cin.fail() || (raise < min_limit) || (raise_bigger_than_players_balance(players, number_of_players, raise)))
    {
        if (max_limit < min_limit) {
            std::cout << "You are not able to raise! Call or fold (c/f): ";
            able_to_raise = false;
            char decision;
            validation_unable_to_raise(decision);
            if (decision == 'f') {
                players[idx].is_playing = false;
            }
            else {
                players[idx].has_called = true;
            }
            break;

        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');//moving the pointer
        if ((raise < last_bet + CHIP_VALUE)) {
            std::cout << "You should enter a valid raise! Bigger than "<< min_limit <<": ";
        }
        if ((raise_bigger_than_players_balance(players, number_of_players, raise))) {
            std::cout << "You should enter a valid raise! Smaller than "<< max_limit <<": ";
        }
        
        std::cin >> raise;
    }

}
void given_raise(int number_of_players, player players[], int last_raise, int& pot) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            players[i].balance -= last_raise;
            pot += last_raise;
            players[i].given_virtual_points += last_raise;
        }
    }
    
}
void status_of_players(int number_of_players, player players[], int last_raise, int& pot) {
    std::cout << "Last raise: " << last_raise<<'\n';
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            std::cout<<"Player"<<players[i].id<<" balance: " << players[i].balance<<'\n';
           
        }
    }
    std::cout << "Pot: " << pot<< '\n';

}
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
bool just_one_player_left(player players[], int number_of_players) {
    int count = 0;
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            count++;
            
        }
    }
    return (count == 1);
}
int max_result(player players[], int number_of_players) {
    int max = players[0].result;
    for (int i = 1;i < number_of_players;i++) {
        if (max < players[i].result) max = players[i].result;
    }
    return max;
}

int max_result_idx(player players[], int number_of_players) {
    int max = 0;
    int max_idx;
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
bool is_tie(player players[], int number_of_players) {
    int max = max_result(players, number_of_players);
    int count = 0;
    for (int i = 0;i < number_of_players;i++) {
        if (max == players[i].result) count++;
    }
    return (count > 1);
}

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
    int value_to_pay_to_join_the_tie = pot / 2;
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].result != max) {
            std::cout << "Player" << players[i].id << " do you want to join the tie? You have to pay "<< value_to_pay_to_join_the_tie <<" (y / n) : ";

            char answer;
            validation_of_joining_the_tie(answer);
            
            if (answer == 'y') {
                if (players[i].balance < value_to_pay_to_join_the_tie) {
                    std::cout << "You don't have the money to join the tie!" << '\n';
                    players[i].is_playing = false;
                }
                else {
                    players[i].balance -= value_to_pay_to_join_the_tie;
                    players[i].is_playing = true;
                }
            }
            else {
                players[i].is_playing = false;
            }
        }
        
        else {
            if (players[i].balance == 0) {
                players[i].balance += 50;
            }
        }
    }
}

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

void write_results_in_file(player players[], int number_of_players) {
    sort_by_balance(players, number_of_players);
    std::ofstream MyFile("results.txt");
    for (int i = number_of_players - 1;i>=0;i--) {
        MyFile << "Player"<<players[i].id << " with balance " << players[i].balance << "!" << '\n';
    }
    MyFile.close();
}

//NEW GAME
void restart_players_status(player players[], int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].balance > 0) {
            players[i].has_called = false;
            players[i].is_playing = true;
        }
        else {
            players[i].is_playing = false;
        }
    }
}
void restart_players_status_when_tie(player players[], int number_of_players) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            players[i].has_called = false;
        }
    }
}
//---------------------------------------------------------------------------------------------------------------

int main()
{
    std::cout << "WELCOME TO FMI CASINO!" << '\n';
    std::cout << "Who is going to be the next billionaire? It's time to find out!"<<'\n';
 


    //the creation of the deck
    int idx_of_deck = 0;
    for (int i = 0;i < NUMBER_OF_SUITS;i++) {
        for (int j = 0;j < NUMBER_OF_TYPES_OF_CARDS;j++) {
            deck[idx_of_deck].suit = suits[i]; 
            deck[idx_of_deck].number = cards[j];
            idx_of_deck++;
        }
    }
   /* for (int i = 0;i < idx_of_deck;i++) {
        std::cout << deck[i].number << " " << deck[i].suit<<'\n';
    }
   */
    bool another_game = true;
    bool not_first_game = false;
        std::cout << "How many players are going to play (2-9)? ";
        int number_of_players;

        std::cin >> number_of_players;

        while (std::cin.fail() || (number_of_players > 9) || (number_of_players < 2))
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');//moving the pointer
            std::cout << "You should enter a number between 2 and 9! ";
            std::cin >> number_of_players;
        }

        player players[MAX_NUMBER_OF_PLAYERS];
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
    while (another_game) {
        int last_bet = 0;
        if (not_first_game) {
            pot = 0;
            int indexes[NUMBER_OF_ALL_CARDS];
            int len_indexes = 0;
            restart_players_status(players, number_of_players);
            for (int i = 0;i < number_of_players;i++) {
                std::cout << players[i].is_playing<<" "<< players[i].balance<<'\n';
            }
            for (int i = 0;i < number_of_players;i++) {
                if (players[i].is_playing) {
                    cards_deal_array(players[i].cards_in_hand, indexes, len_indexes);
                }
            }
            //last_bet = 0;
        }
        if (is_tie_flag) {
            
            int indexes[NUMBER_OF_ALL_CARDS];
            int len_indexes = 0;
            for (int i = 0;i < number_of_players;i++) {
                if (players[i].is_playing) {
                    cards_deal_array(players[i].cards_in_hand, indexes, len_indexes);
                }
            }
            is_tie_flag = false;
            restart_players_status_when_tie(players, number_of_players);
        }
        /*for (int i = 0;i < number_of_players;i++) {
            if (players[i].is_playing) {
                for (int j = 0;j < 3;j++) {
                    std::cout << players[i].cards_in_hand[j].number << " " << players[i].cards_in_hand[j].suit << '\n';
                }
            }
        }*/
        /*for (int h = 0;h < number_of_players;h++) {
            for (int i = 0;i < 3;i++) {
                std::cout << players[h].cards_in_hand[i].number << ' ' << players[h].cards_in_hand[i].suit << '\n';
            }
        }*/

        /* for (int i = 0;i < 3;i++) {
                std::cout << players[0].cards_in_hand[i].number << ' '<< players[0].cards_in_hand[i].suit<<'\n';
            }*/
            /*sort_by_suit(players[1].cards_in_hand);
            for (int i = 0;i < 3;i++) {
                std::cout<<players[1].cards_in_hand[i].number <<' '<< players[1].cards_in_hand[i].suit << '\n';
            }*/


            //std::cout << pocker_player.cards_in_hand[0];
            /*for (int h = 0;h < 3;h++) {
                std::cout<<players[i].cards_in_hand[h].number<<' '<< players[i].cards_in_hand[h].suit<<'\n';
            }*/



        //Now we are going to evaluate the result - count of points
        for (int i = 0;i < number_of_players;i++)
        {
            if (players[i].is_playing) {
                players[i].result = result_of_points(players[i].cards_in_hand);
            }
        }

       /* if (count == 0) {
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

        
        for (int i = 0;i < number_of_players;i++) {
            if(players[i].is_playing)
            for (int j = 0;j < 3;j++) {
                std::cout<<players[i].cards_in_hand[j].number <<" " << players[i].cards_in_hand[j].suit<<'\n';
            }
            std::cout << players[i].result<<'\n';
        }



        /*  for (int i = 0;i < number_of_players;i++) {
              std::cout << players[i].balance<<' ';

          }
          std::cout << pot;*/

        
        beggining_of_the_game(players, pot, number_of_players);

        while (!all_players_has_called(players, number_of_players) && (!just_one_player_left(players, number_of_players))) {
            int raise = 0;
           

            for (int i = 0;i < number_of_players;i++) {
                if (just_one_player_left(players, number_of_players) || all_players_has_called(players, number_of_players)) {
                    break;
                }
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
                        players[i].given_virtual_points += raise;
                        players[i].balance -= raise; 
                        pot += raise;
                        status_of_players(number_of_players, players, raise, pot);
                    }
                    else if (decision == 'f') {
                        players[i].is_playing = false;
                    }
                    else {
                        std::cout<<raise<<' ' << players[i].given_virtual_points << '\n';
                        if (raise > players[i].given_virtual_points) {
                            players[i].has_called = true;
                            players[i].given_virtual_points += raise;
                            players[i].balance -= raise;
                            pot += raise;

                        }
                        else {
                            std::cout << "You can not call!" <<players[i].given_virtual_points<<'\n';
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
          
            is_tie_flag = true;
            //another_game = true;
            count = 1;
            

        }
        
        else {
            int max_idx = max_result_idx(players, number_of_players);
            players[max_idx].balance += pot;
            std::cout << "The winner is Player" << players[max_idx].id << " with " << players[max_idx].balance << "!" << '\n';
            for (int i = 0;i < NUMBER_OF_CARDS_IN_A_HAND;i++) {
                if (players[max_idx].cards_in_hand[i].number > 10) {
                    std::cout << int_to_card_type_converter(players[max_idx].cards_in_hand[i].number);
                }
                else {
                    std::cout << players[max_idx].cards_in_hand[i].number;
 
                }
                std::cout<< players[max_idx].cards_in_hand[i].suit << ' ';
            }
            std::cout << players[max_idx].result<<'\n';
            char answer;
            do_you_want_to_play_again(answer);
            if (answer == 'y') {
                not_first_game = true;
            }
            else {
                write_results_in_file(players, number_of_players);
                another_game = false;
            }
        }

    }
        return 0;
}
