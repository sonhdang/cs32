//
//  BballRoster.h
//  Project2
//
//  Created by Son Dang on 7/16/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#ifndef BballRoster_h
#define BballRoster_h
#include <string>

typedef std::string SomeType;

struct Node
{
    std::string m_firstName;
    std::string m_lastName;
    SomeType m_value;
    Node* next;
    Node* previous;
};

class BballRoster
{
public:
    BballRoster(); // Create an empty BballRoster list
    BballRoster(const BballRoster& rhs);
    ~BballRoster();
    const BballRoster& operator=(const BballRoster& rhs);
    bool rosterEmpty() const; // Return true if the BballRoster list // is empty, otherwise false.
    int howManyPlayers() const; // Return the number of players in // the BballRoster list.
    bool signPlayer(const std::string& firstName, const std::string& lastName, const SomeType& value);
    // If the full name (both the first and last name) is not equal // to any full name currently in the list then add it and return // true. Elements should be added according to their last name. // Elements with the same last name should be added according to // their first names. Otherwise, make no change to the list and // return false (indicating that the name is already in the
    // list).
    bool resignPlayer(const std::string& firstName, const std::string& lastName, const SomeType& value);
    // If the full name is equal to a full name currently in the // list, then make that full name no longer map to the value it // currently maps to, but instead map to the value of the third // parameter; return true in this case. Otherwise, make no
    // change to the list and return false.
    
    bool signOrResign(const std::string& firstName, const std::string& lastName, const SomeType& value);
    // If full name is equal to a name currently in the list, then // make that full name no longer map to the value it currently // maps to, but instead map to the value of the third parameter; // return true in this case. If the full name is not equal to // any full name currently in the list then add it and return // true. In fact, this function always returns true.
    bool renouncePlayer(const std::string& firstName, const std::string& lastName);
    // If the full name is equal to a full name currently in the
    // list, remove the full name and value from the list and return // true. Otherwise, make no change to the list and return
    // false.
    bool playerOnRoster(const std::string& firstName, const std::string& lastName) const;
    // Return true if the full name is equal to a full name
    // currently in the list, otherwise false.
    bool lookupPlayer(const std::string& firstName, const std::string& lastName, SomeType& value) const;
    // If the full name is equal to a full name currently in the // list, set value to the value in the list that that full name // maps to, and return true. Otherwise, make no change to the // value parameter of this function and return false.
    bool choosePlayer(int i, std::string& firstName, std::string& lastName, SomeType& value) const;
    // If 0 <= i < size(), copy into firstName, lastName and value // parameters the corresponding information of the element at // position i in the list and return true. Otherwise, leave the // parameters unchanged and return false. (See below for details // about this function.)
    void swapRoster(BballRoster& other);
    // Exchange the contents of this list with the other one.
    
private:
    
    Node* head;
    int m_size;
};



bool joinRosters(const BballRoster & bbOne,
                 const BballRoster & bbTwo,
                 BballRoster & bbJoined);

void checkRoster (const std::string& fsearch,
                  const std::string& lsearch,
                  const BballRoster& bbOne,
                  BballRoster& bbResult);



#endif /* BballRoster_h */
