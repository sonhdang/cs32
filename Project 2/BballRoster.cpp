//
//  BballRoster.cpp
//  Project2
//
//  Created by Son Dang on 7/16/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#include <stdio.h>
#include "BballRoster.h"

using namespace std;

BballRoster::BballRoster():head(nullptr), m_size(0)
{}

BballRoster::BballRoster(const BballRoster& rhs)
{
    if (rhs.head == nullptr)    //if the rhs BballRoster is empty, no need to copy
        return;
    else
    {
        head = new Node();    //Creating the first Node and linking it to head pointer
        head->m_firstName = rhs.head->m_firstName;
        head->m_lastName = rhs.head->m_lastName;
        head->m_value = rhs.head->m_value;
        head->next = nullptr;       //initializing the next pointer with nullptr
        head->previous = nullptr;   //the first node's previous ptr is a nullptr
        Node* p = head;
        Node* q = rhs.head;
        q = q->next;
        
        while(q != nullptr)         //coyping the rest of the list with a while loop
        {
            Node* temp = new Node();
            temp->m_firstName = q->m_firstName;
            temp->m_lastName = q->m_lastName;
            temp->m_value = q->m_value;
            temp->next = nullptr;
            temp->previous = p;
            p->next = temp;
            p = p->next;            //update p which is a pointer that moves across this BballRoster
            q = q->next;            //update q which is a pointer that moves across rhs BballRoster
        }
        
    }
    
    m_size = rhs.m_size;            //update the size
}

BballRoster::~BballRoster()
{
    if (head == nullptr)        //if BballRoster is empty, no need to destruct
        return;
    else
    {
        Node* p = head;         //p pointer points to the node that will get deleted
        Node* q = p;            //q pointer traverses BballRoster untill the end
        while (p != nullptr)
        {
            q = p->next;
            delete p;
            p = nullptr;
            p = q;
        }
    }
    head = nullptr;
    m_size = 0;                 //the size is zero after BballRoster is destructed
}

const BballRoster& BballRoster::operator=(const BballRoster& rhs)
{
    if(&rhs == this)
        return *this;
    BballRoster temp(rhs);
    this->swapRoster(temp);
    return *this;
}

bool BballRoster::rosterEmpty() const
{
    return m_size == 0;     //return true if BballRoster is empty, false otherwise
}

int BballRoster::howManyPlayers() const
{
    return m_size;          //return the private member m_size
}

bool BballRoster::signPlayer(const string& firstName, const string& lastName,const SomeType& value)
{
    if (rosterEmpty())      //if BballRoster is empty, no need to sort or check for duplicates
    {
        head = new Node();
        head->m_firstName = firstName;
        head->m_lastName = lastName;
        head->m_value = value;
        head->next = nullptr;
        head->previous = nullptr;
        m_size++;
        return true;
    }
    else
    {
        Node* p = head;
        while (p->next != nullptr)    //iterate through the roster until p reaches the end of the roster
        {
            if (p->m_lastName == lastName)  //check if the lastName is the same as the current lastName
            {
                if (p->m_firstName == firstName)    //check the firstName if the lastNames are the same
                    return false;                   //return false if the name already exists
                else if (p->m_firstName > firstName)    //if firstName is to the left of the current
                    break;                              //firstName, we have found the appropriate position
                else
                    p = p->next;                        //if not, move to the next player
            }
            else if (p->m_lastName > lastName)      //if lastName is to the left of the current lastName,
                break;                              //we have foudn the appropriate position
            else
                p = p->next;                        //if not, move to the next player
        }
        
        if (p->m_lastName == lastName && p->m_firstName == firstName) //Check if the last element of the
            return false;       //roster is already in the list because the loop stopped before checking
        
        Node* temp = new Node();                    //creating new Node
        temp->m_firstName = firstName;
        temp->m_lastName = lastName;
        temp->m_value = value;
        if(p->next == nullptr && p->previous == nullptr)    //when there is only one player
        {
            if (p->m_lastName < lastName || (p->m_lastName == lastName && p->m_firstName < firstName))
            {
                temp->next = nullptr;
                temp->previous = p;
                p->next = temp;
            }
            else
            {
                temp->next = p;
                temp->previous = nullptr;
                p->previous = temp;
                head = temp;
            }
        }
        else if (p->next == nullptr)
        {                                    //check if the current position is the end of the roster
            if (p->m_lastName < lastName || (p->m_lastName == lastName && p->m_firstName < firstName))
            {
                temp->next = nullptr;
                temp->previous = p;
                p->next = temp;
            }
            else
            {
                temp->next = p;             //adding the node in the middle of the roster
                temp->previous = p->previous;
                (p->previous)->next = temp;
                p->previous = temp;
            }
        }
        else if (p->previous == nullptr)    //check if the current position is the beginning of the roster
        {
            temp->next = p;
            temp->previous = nullptr;
            p->previous = temp;
            head = temp;
        }
        else
        {
            temp->next = p;             //adding the node in the middle of the roster
            temp->previous = p->previous;
            (p->previous)->next = temp;
            p->previous = temp;
        }
        m_size++;
        return true;
    }
}

bool BballRoster::resignPlayer(const string& firstName, const string& lastName, const SomeType& value)
{
    Node* p = head;
    while (p != nullptr && lastName <= p->m_lastName)   //iterate through the roster until the end of the roster
    {                                                   //if not passing a bigger lastName in the roster
        if(p->m_firstName == firstName && p->m_lastName == lastName)
        {
            p->m_value = value;
            return true;    //return true when the player is found and value's is mapped to value
        }
        else
            p = p->next;    //move to the next player on the roster
    }
    return false;
}

bool BballRoster::signOrResign(const string& firstName, const string& lastName, const SomeType& value)
{
    if (rosterEmpty())      //if BballRoster is empty, no need to sort or check for duplicates
    {
        head = new Node();
        head->m_firstName = firstName;
        head->m_lastName = lastName;
        head->m_value = value;
        head->next = nullptr;
        head->previous = nullptr;
        m_size++;
        return true;
    }
    else
    {
        Node* p = head;
        while (p->next != nullptr)    //iterate through the roster until p reaches the end of the roster
        {
            if (p->m_lastName == lastName)  //check if the lastName is the same as the current lastName
            {
                if (p->m_firstName == firstName)
                {                                         //check the firstName if the lastNames are the same
                    p->m_value = value;                   //return false if the name already exists
                    return true;
                }
                else if (p->m_firstName > firstName)    //if firstName is to the left of the current
                    break;                              //firstName, we have found the appropriate position
                else
                    p = p->next;                        //if not, move to the next player
            }
            else if (p->m_lastName > lastName)      //if lastName is to the left of the current lastName,
                break;                              //we have foudn the appropriate position
            else
                p = p->next;                        //if not, move to the next player
        }
        
        if (p->m_lastName == lastName && p->m_firstName == firstName) //Check if the last element of the
        {                                                             //roster is already in the list
            p->m_value = value;                                       //because the loop stopped before
            return true;                                              //checking
        }
        
        Node* temp = new Node();                    //creating new Node
        temp->m_firstName = firstName;
        temp->m_lastName = lastName;
        temp->m_value = value;
        if(p->next == nullptr && p->previous == nullptr)    //when there is only one player
        {
            if (p->m_lastName < lastName || (p->m_lastName == lastName && p->m_firstName < firstName))
            {
                temp->next = nullptr;
                temp->previous = p;
                p->next = temp;
            }
            else
            {
                temp->next = p;
                temp->previous = nullptr;
                p->previous = temp;
                head = temp;
            }
        }
        else if (p->next == nullptr)
        {                                    //check if the current position is the end of the roster
            if (p->m_lastName < lastName || (p->m_lastName == lastName && p->m_firstName < firstName))
            {
                temp->next = nullptr;
                temp->previous = p;
                p->next = temp;
            }
            else
            {
                temp->next = p;             //adding the node in the middle of the roster
                temp->previous = p->previous;
                (p->previous)->next = temp;
                p->previous = temp;
            }
        }
        else if (p->previous == nullptr)    //check if the current position is the beginning of the roster
        {
            temp->next = p;
            temp->previous = nullptr;
            p->previous = temp;
            head = temp;
        }
        else
        {
            temp->next = p;             //adding the node in the middle of the roster
            temp->previous = p->previous;
            (p->previous)->next = temp;
            p->previous = temp;
        }
        m_size++;
        return true;
    }
}

bool BballRoster::renouncePlayer(const string& firstName, const string& lastName)
{
    if (rosterEmpty())
        return false;
    
    Node* p = head;
    while (p != nullptr)   //iterate through the roster until the end of the roster
    {                      //if not passing a bigger lastName in the roster
        if(p->m_firstName == firstName && p->m_lastName == lastName)
            break;
        p = p->next;    //move to the next player on the roster
    }

    if (p == nullptr)       //the player is not on the roster; therefore return false
        return false;
    else if (howManyPlayers() == 1)
    {
        delete p;
        head = nullptr;
    }
    else if (p->previous == nullptr)        //the player is at the beginning of the roster the roster
    {
        head = p->next;
        head->previous = nullptr;
        delete p;
    }
    else if (p->next == nullptr)            //the player is at the end of
    {
        (p->previous)->next = nullptr;
        delete p;
        
    }
    else                                //the player is in the middle of the roster
    {
        (p->next)->previous = p->previous;
        (p->previous)->next = p->next;
        delete p;
    }
    m_size--;
    return true;
}

bool BballRoster::playerOnRoster(const string& firstName, const string& lastName) const
{
    if (rosterEmpty())      //check if the roster is empty
        return false;
    Node* p = head;
    while (p != nullptr && p->m_lastName <= lastName)   //iterate through the roster and compare the full name
    {
        if (p->m_lastName == lastName && p->m_firstName == firstName)
            return true;
        p = p->next;
    }
    return false;
    
}

bool BballRoster::lookupPlayer(const string& firstName, const string& lastName, SomeType& value) const
{
    if (rosterEmpty())      //check if the roster is empty
        return false;
    Node* p = head;
    while (p != nullptr && p->m_lastName <= lastName)   //iterate through the roster to find the player
    {
        if (p->m_lastName == lastName && p->m_firstName == firstName)
        {
            value = p->m_value;                         //change the value of the player when found
            return true;
        }
        p = p->next;
    }
    return false;
}

bool BballRoster::choosePlayer(int i, string& firstName, string& lastName, SomeType& value) const
{
    if (rosterEmpty() || i < 0 || i >= howManyPlayers())     //check if i is invalid or the roster is empty
        return false;
    
    Node* p = head;
    while (i > 0)    //move the pointer to position i;
    {
        p = p->next;
        i--;
    }

    firstName = p->m_firstName;
    lastName = p->m_lastName;
    value = p->m_value;
    return true;
}

void BballRoster::swapRoster(BballRoster& other)
{
    Node* temp = head;      //swapping the head pointers of two rosters
    head = other.head;
    other.head = temp;
    
    int tempSize = m_size;  //swapping the sizes of the two rosters
    m_size = other.m_size;
    other.m_size = tempSize;
}

bool joinRosters(const BballRoster& bbOne,const BballRoster& bbTwo, BballRoster& bbJoined)
{
    bool ret = true;            //setting the return value to true unless it is set to
                                //false because ther is a player with two different values
                                //clearing bbJoined to prevent memory leak
    
    if (&bbOne != &bbJoined || &bbTwo != &bbJoined)
        bbJoined.~BballRoster();
    
    for (int i = 0; i < bbOne.howManyPlayers(); i++)    //adding all the players in bbOne to bbJoined
    {
        string firstName = "";
        string lastName = "";
        SomeType value;
        bbOne.choosePlayer(i, firstName, lastName, value);
        bbJoined.signPlayer(firstName, lastName, value);
    }
    
    for (int i = 0; i < bbTwo.howManyPlayers(); i++)   //adding all the players in bbTwo to bbJoined
    {
        string firstName = "";
        string lastName = "";
        SomeType value;
        bbTwo.choosePlayer(i, firstName, lastName, value);
        SomeType testValue;
        if (bbJoined.lookupPlayer(firstName, lastName, testValue))  //check if the player is already in
        {                                                           //bbJoined
            if (value == testValue)         //if the player that are in both rosters' values are the same
                continue;                   //then move on to the next player of bbTwo
            else                            //if the player who are in both rosters' values are different
            {                               //then set the return value to false and delete the player
                bbJoined.renouncePlayer(firstName, lastName);
                ret = false;
            }
        }
        else
            bbJoined.signPlayer(firstName, lastName, value);    //if the player is not in bbJoined yet
    }
    return ret;
}

void checkRoster(const string& fsearch, const string& lsearch, const BballRoster& bbOne, BballRoster& bbResult)
{
    BballRoster bbTemp(bbOne);
    bbResult.~BballRoster();
    
    if (fsearch == "*" && lsearch == "*")   //when both fsearch and lsearch are "*" then bbResult
        bbResult = bbTemp;                   //is a deep copy of bbTemp
    else
    {
        if (fsearch == "*")                 //when fseach is a wildcard "*"
        {
            for (int i = 0; i < bbTemp.howManyPlayers(); i++)
            {
                string firstName = "";
                string lastName = "";
                SomeType value;
                bbTemp.choosePlayer(i, firstName, lastName, value);
                if (lastName == lsearch)
                    bbResult.signPlayer(firstName, lastName, value);
            }
        }
        else if (lsearch == "*")            //when lsearch is a wildcard "*"
        {
            for (int i = 0; i < bbTemp.howManyPlayers(); i++)
            {
                string firstName = "";
                string lastName = "";
                SomeType value;
                bbTemp.choosePlayer(i, firstName, lastName, value);
                if (firstName == fsearch)
                    bbResult.signPlayer(firstName, lastName, value);
            }
        }
        else                                //when both lsearch and fsearch are last name and first name
        {
            SomeType value;
            if (bbTemp.lookupPlayer(fsearch, lsearch, value))
                bbResult.signPlayer(fsearch, lsearch, value);
        }
    }
    
}
