// pocker_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include "test.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>

int const NUMBER_OF_TYPES_OF_CARDS = 8;
int const NUMBER_OF_SUITS = 4;
int const NUMBER_OF_ALL_CARDS = 32; // 4 suits * 8 types of cards
int const NUMBER_OF_CARDS_IN_A_HAND = 3;
int const CHIP_VALUE = 10;
int const ARRAY_SIZE = 32; //the maximum number of possible cards is 32
int const MAX_NUMBER_OF_PLAYERS = 9;

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
            players[i].balance -= CHIP_VALUE;
            pot += CHIP_VALUE;
        }
    }
}
bool raise_bigger_than_players_balance(player players[], int number_of_players, int last_raise) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].balance < last_raise) {
            return 1;
        }
    }
    return 0;
}

void validation_raise(int& raise, int last_bet, player players[], int number_of_players) {
    std::cout << "Your raise is: ";
    std::cin >> raise;
    while (std::cin.fail() || (raise < last_bet) || (raise_bigger_than_players_balance(players, number_of_players, raise)))
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');//moving the pointer
        std::cout << "You should enter a valid raise! ";
        std::cin >> raise;
    }

}
void given_raise(int number_of_players, player players[], int last_raise, int& pot) {
    for (int i = 0;i < number_of_players;i++) {
        if (players[i].is_playing) {
            players[i].balance -= last_raise;
            pot += last_raise;
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
    std::cout << "Pot: " << pot;

}
int main()
{
 

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



    //Now we are going to eavluate the result - count of points
    for (int i = 0;i < number_of_players;i++)
    {
        players[i].result = result_of_points(players[i].cards_in_hand);
    }


    /*
    for (int i = 0;i < number_of_players;i++) {
        for (int j = 0;j < 3;j++) {
            std::cout<<players[i].cards_in_hand[j].number <<" " << players[i].cards_in_hand[j].suit<<'\n';
        }
        std::cout << players[i].result<<'\n';
    }*/

    int pot = 0;
    beggining_of_the_game(players, pot, number_of_players);
  /*  for (int i = 0;i < number_of_players;i++) {
        std::cout << players[i].balance<<' ';

    }
    std::cout << pot;*/
    int last_bet = 0;
    for (int i = 0;i < number_of_players;i++) {
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

        int raise = 0;
        if (decision == 'r') {
            validation_raise(raise, last_bet, players, number_of_players);
            given_raise(number_of_players, players, raise, pot);
            status_of_players(number_of_players, players, raise, pot);
        }
        else if (decision == 'f') {
            players[i].is_playing = false;
        }

    }
    
        return 0;
}

