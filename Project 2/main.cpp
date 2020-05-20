//
//  main.cpp
//  Project2
//
//  Created by Son Dang on 7/9/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#include <iostream>
#include "BballRoster.h"
#include <type_traits>
#include <cassert>
#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }
using namespace std;
static_assert(std::is_default_constructible<BballRoster>::value,
              "Map must be default-constructible.");
static_assert(std::is_copy_constructible<BballRoster>::value,
              "Map must be copy-constructible.");
void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&BballRoster::operator=, BballRoster&(BballRoster::*)(const BballRoster&));
    CHECKTYPE(&BballRoster::rosterEmpty, bool(BballRoster::*)() const);
    CHECKTYPE(&BballRoster::howManyPlayers, int(BballRoster::*)() const);
    CHECKTYPE(&BballRoster::signPlayer, bool (BballRoster::*)(const std::string&, const std::string&, const SomeType&));
    CHECKTYPE(&BballRoster::resignPlayer, bool
              (BballRoster::*)(const std::string&, const std::string&,
                               const SomeType&));
    CHECKTYPE(&BballRoster::signOrResign, bool
              (BballRoster::*)(const std::string&, const std::string&,
                               const SomeType&));
    CHECKTYPE(&BballRoster::renouncePlayer, bool (BballRoster::*)
              (const std::string&, const std::string&));
    CHECKTYPE(&BballRoster::playerOnRoster, bool
              (BballRoster::*)(const std::string&, const std::string&)
              const);
    CHECKTYPE(&BballRoster::lookupPlayer, bool (BballRoster::*)
              (const std::string&, const std::string&, SomeType&)
              const);
    CHECKTYPE(&BballRoster::choosePlayer, bool (BballRoster::*)
              (int, std::string&, std::string&, SomeType&)
              const);
    CHECKTYPE(&BballRoster::swapRoster, void
              (BballRoster::*)(BballRoster&));
    CHECKTYPE(joinRosters,  bool (*)(const BballRoster&, const
                                     BballRoster&, BballRoster&));
    CHECKTYPE(checkRoster, void (*)(const std::string&,
                                    const std::string&, const BballRoster&, BballRoster&));
}

void test()
{
    BballRoster mwp;
    assert(mwp.signPlayer("Adam", "Wright", 41));
    assert(mwp.signPlayer("Jason", "Falitz", 37));
    assert(mwp.howManyPlayers() == 2);
    string first, last;
    int a;
    assert(mwp.choosePlayer(0, first, last, a) && a == 37);
    assert(mwp.choosePlayer(1, first, last, a) && (first == "Adam" && a == 41));
    
    //test empty constructor
    BballRoster vietnam;
    
    //test rosterEmpty() function when roster is empty
    assert(vietnam.rosterEmpty());
    //test howManyPlayers() function when m_size = 0
    assert(vietnam.howManyPlayers() == 0);
    
    //test signPlayer() function
    assert(vietnam.signPlayer("Son", "Dang", 3));
    //test rosterEmpty() function when roster is not empty
    assert(!vietnam.rosterEmpty());
    //test howManyPlayers() signPlayer is called once
    assert(vietnam.howManyPlayers() == 1);
    //test signPlayer() when player is already on the roster with a different value
    assert(!vietnam.signPlayer("Son", "Dang", 1));
    //test signPlayer() when player is alrady on the roster with the same value
    assert(!vietnam.signPlayer("Son", "Dang", 3));
    assert(vietnam.howManyPlayers() == 1);
    
    SomeType value = 0;
    //test lookupPlayer() function
    assert(vietnam.lookupPlayer("Son", "Dang", value) && value == 3);
    
    value = 0;
    //test lookupPlayer() function's case sensitivity
    assert(!vietnam.lookupPlayer("Son", "dang", value));
    assert(!vietnam.lookupPlayer("son", "Dang", value));
    assert(value == 0);
    
    //test signPlayer() case-sensitivity
    assert(vietnam.signPlayer("son", "Dang", 3));
    assert(vietnam.lookupPlayer("son", "Dang", value) && value == 3);
    assert(vietnam.howManyPlayers() == 2);
    
    //test resignPlayer()
    assert(vietnam.resignPlayer("son", "Dang", 2));
    assert(vietnam.lookupPlayer("son", "Dang", value) && value == 2);
    //test resignPlayer() when the value passing is the same as the current value
    assert(vietnam.resignPlayer("Son", "Dang", 3));
    assert(vietnam.lookupPlayer("Son", "Dang", value) && value == 3);
    //test resignPlayer() when the player is not on the roster
    assert(!vietnam.resignPlayer("Nguyen", "Nguyen", 9));
    assert(!vietnam.lookupPlayer("Nguyen", "Nguyen", value) && value == 3);
    assert(vietnam.howManyPlayers() == 2);
    
    //test renouncePlayer()
    string firstName = "";
    string lastName = "";
    value = 0;
    vietnam.signPlayer("Sort", "Alphabetically", 33);
    vietnam.signPlayer("Sort", "Betabetically", 34);
    assert(vietnam.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Sort" && lastName == "Alphabetically" && value == 33);
    assert(vietnam.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Sort" && lastName == "Betabetically" && value == 34);
    assert(vietnam.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Son" && lastName == "Dang" && value == 3);
    assert(vietnam.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "son" && lastName == "Dang" && value == 2);
    assert(vietnam.renouncePlayer("Sort", "Betabetically"));
    assert(vietnam.renouncePlayer("Sort","Alphabetically"));
    assert(!vietnam.renouncePlayer("Son", "dang"));
    assert(vietnam.howManyPlayers() == 2);
    assert(vietnam.renouncePlayer("son", "Dang"));
    assert(!vietnam.lookupPlayer("son", "Dang", value) && value == 2);
    assert(vietnam.howManyPlayers() == 1);
    assert(vietnam.renouncePlayer("Son", "Dang"));
    assert(!vietnam.lookupPlayer("Son", "Dang", value) && value == 2);
    assert(vietnam.howManyPlayers() == 0);
    assert(vietnam.rosterEmpty());
    //test renouncePlayer() when the roster is empty
    assert(!vietnam.renouncePlayer("Dang", "Son"));
    assert(vietnam.rosterEmpty());
    assert(vietnam.howManyPlayers() == 0);
    
    //test signPlayer() when signing a player that was renounced before
    assert(vietnam.signPlayer("Son", "Dang", 3));
    assert(vietnam.howManyPlayers() == 1);
    
    //test signOrResign() when the roster is Empty()
    BballRoster america;
    assert(america.signOrResign("Lebron", "James", 23));
    assert(america.howManyPlayers() == 1);
    assert(america.lookupPlayer("Lebron", "James", value) && value == 23);
    //test signOrResign() when the roster is not empty()
    assert(america.signOrResign("Stephen", "Curry", 30));
    assert(america.howManyPlayers() == 2);
    assert(america.lookupPlayer("Stephen", "Curry", value) && value == 30);
    //test signOrResign() when the player already exists
    assert(america.signOrResign("Lebron", "James", 32));
    assert(america.lookupPlayer("Lebron", "James", value) && value == 32);
    
    //test the order of the players when use signOrResign()
    firstName = "";
    lastName = "";
    america.signOrResign("Seth", "Curry", 78);
    america.signOrResign("Bronny", "James", 45);
    america.signOrResign("Kevin", "Love", 24);
    assert(america.howManyPlayers() == 5);
    assert(america.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    assert(america.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    assert(america.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Bronny" && lastName == "James" && value == 45);
    assert(america.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Lebron" && lastName == "James" && value == 32);
    assert(america.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    
    //test choosePlayer() when i is invalid
    assert(!america.choosePlayer(5, firstName, lastName, value));
    assert(!america.choosePlayer(-1, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    //test playerOnRoster()
    assert(america.playerOnRoster("Bronny", "James"));
    assert(!america.playerOnRoster("bronny", "James"));
    assert(america.playerOnRoster("Kevin", "Love"));
    assert(america.playerOnRoster("Seth", "Curry"));
    
    //test swapRoster()
    firstName = "";
    lastName = "";
    value = 0;
    america.swapRoster(vietnam);
    assert(vietnam.howManyPlayers() == 5);
    assert(vietnam.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    assert(vietnam.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    assert(vietnam.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Bronny" && lastName == "James" && value == 45);
    assert(vietnam.choosePlayer(3, firstName, lastName, value));
    assert(vietnam.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Lebron" && lastName == "James" && value == 32);
    assert(vietnam.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    assert(america.howManyPlayers() == 1);
    assert(america.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Son" && lastName == "Dang" && value == 3);
    
    //test swapRoster() with an empty BballRoster
    firstName = "";
    lastName = "";
    value = 0;
    BballRoster china;
    vietnam.swapRoster(china);
    assert(china.howManyPlayers() == 5);
    assert(china.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    assert(china.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    assert(china.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Bronny" && lastName == "James" && value == 45);
    assert(china.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Lebron" && lastName == "James" && value == 32);
    assert(china.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    firstName = "";
    lastName = "";
    value = 0;
    assert(vietnam.howManyPlayers() == 0);
    assert(!vietnam.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "" && lastName == "" && value == 0);
    
    //test assigment operator overloading
    america = china;
    assert(america.howManyPlayers() == 5);
    assert(america.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    assert(america.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    assert(america.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Bronny" && lastName == "James" && value == 45);
    assert(america.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Lebron" && lastName == "James" && value == 32);
    assert(america.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    
    assert(china.howManyPlayers() == 5);
    assert(china.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    assert(china.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    assert(china.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Bronny" && lastName == "James" && value == 45);
    assert(china.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Lebron" && lastName == "James" && value == 32);
    assert(china.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    
    //test assigment operator overloading of a short roster to a longer one
    firstName = "";
    lastName = "";
    value = 0;
    vietnam.signPlayer("Son", "Dang", 3);
    vietnam = china;
    assert(vietnam.howManyPlayers() == 5);
    assert(china.howManyPlayers() == 5);
    assert(vietnam.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    firstName = "";
    lastName = "";
    value = 0;
    assert(china.choosePlayer(0,firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    
    //test copy constructor
    firstName = "";
    lastName = "";
    value = 0;
    BballRoster russia;
    russia = america;
    assert(russia.howManyPlayers() == 5);
    assert(russia.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Seth" && lastName == "Curry" && value == 78);
    assert(russia.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    assert(russia.choosePlayer(2, firstName, lastName, value));
    assert(firstName == "Bronny" && lastName == "James" && value == 45);
    assert(russia.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Lebron" && lastName == "James" && value == 32);
    assert(russia.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Love" && value == 24);
    
    //test joinRosters()
    BballRoster france;
    france.signPlayer("Michael", "Jordan", 89);
    france.signPlayer("Seth", "Curry", 78);
    france.signPlayer("Kevin", "Love", 24);
    
    BballRoster joined;
    assert(joinRosters(russia, france, joined));
    assert(joined.howManyPlayers() == 6);
    assert(joined.choosePlayer(4, firstName, lastName, value));
    assert(firstName == "Michael" && lastName == "Jordan" && value == 89);
    
    france.signPlayer("Bronny", "James", 89);
    assert(!joinRosters(russia, france, joined));
    assert(joined.howManyPlayers() == 5);
    assert(joined.choosePlayer(3, firstName, lastName, value));
    assert(firstName == "Michael" && lastName == "Jordan" && value == 89);
    
    BballRoster empty;
    BballRoster superEmpty;
    assert(joinRosters(empty,superEmpty, joined));
    assert(joined.howManyPlayers() == 0);
    
    //test checkRoster()
    checkRoster("*", "Curry", russia, empty);
    assert(empty.howManyPlayers() == 2);
    assert(empty.choosePlayer(1, firstName, lastName, value));
    assert(firstName == "Stephen" && lastName == "Curry" && value == 30);
    
    checkRoster("*", "Michael", russia, empty);
    assert(empty.howManyPlayers() == 0);
    
    checkRoster("Lebron", "*", russia, empty);
    assert(empty.howManyPlayers() == 1);
    
    checkRoster("*", "*", russia, empty);
    assert(empty.howManyPlayers() == 5);
    
    russia.signPlayer("Kevin", "Durant", 21);
    checkRoster("Kevin", "*", russia, joined);
    assert(joined.howManyPlayers() == 2);
    assert(joined.choosePlayer(0, firstName, lastName, value));
    assert(firstName == "Kevin" && lastName == "Durant" && value == 21);
    
    checkRoster("*", "James", russia, russia);
    assert(russia.howManyPlayers() == 2);
    
    
    
}

int main() {
    test();
    ThisFunctionWillNeverBeCalled();
    cout << "Passed all tests" << endl;
    return 0;
}
