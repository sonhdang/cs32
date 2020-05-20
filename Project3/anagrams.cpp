//
//  anagrams.cpp
//  Project3
//
//  Created by Son Dang on 7/31/19.
//  Copyright © 2019 Son Dang. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <string>
#include <cassert>
using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

int loadWords(istream &dictfile, string dict[], int i)
{
    string str;
    if(i >= MAXDICTWORDS || !getline(dictfile, str))    //stop reading in more words when reaches MAXDICTWORDS
        return i;                                       //or no words left to read in (aka base case)
    dict[i] = str;
    i++;
    return loadWords(dictfile, dict, i);              //recursive call
}

int loadWords(istream &dictfile, string dict[])
{
    return loadWords(dictfile, dict, 0);                //call the actual recursive function
}

bool isAnagram(string str1, string str2)        //function checking for anagram
{
    if (str2.size() == 1)                       //when there is only one letter left
    {
        if(str2[0] == str1[0])                  //compare that one letter and return true if equals
            return true;
        else
            return false;
    }
    unsigned long indexOf = str1.find(str2[0]); //find the character at position i of str2
    if (indexOf == string::npos)                //return false if the character does not exist
        return false;
    else
    {
        char temp = str1[0];                    //swaping the character at indexOf to the right position
        str1[0] = str1[indexOf];
        str1[indexOf] = temp;
        return isAnagram(str1.substr(1,str1.size()), str2.substr(1,str2.size()));
    }
}

bool checkDuplicates(string results[], const int &size, const string &word, int i)
{
    if (i >= size)                      //base case is when position greater than the size of the array
        return true;
    if (results[i] == word)             //compare word with every single string in results
        return false;
    else
    {
        i++;
        return checkDuplicates(results, size, word, i); //recursive call
    }
}

int recBlends(string word, const string dict[], int size, string results[], int resSize, unsigned int i)
{
    if (resSize >= MAXRESULTS || i >= size)     //if the number of anagrams exceeds MAXRESULTS
        return resSize;                         //or there is no words left in dict then return
                                                // current number of resSize (aka base case)
    if (dict[i].size() != word.size())
    {                                           //comparing the size of the word
        i++;
        return recBlends(word, dict, size, results, resSize, i);
    }
    else
    {
        string currentWord = dict[i];
        if (isAnagram(dict[i], word) && checkDuplicates(results, resSize, currentWord, 0)) //check
        {                                       //if the word is an anagram and if the word is already
            results[resSize] = dict[i];         //in results
            resSize++;
            i++;
            return recBlends(word, dict, size, results, resSize, i);    //recursive call
        }
        else
        {
            i++;
            return recBlends(word, dict, size, results, resSize, i);    //recursive call
        }
    }
}

int recBlends(string word, const string dict[], int size, string results[])
{
    
    if (word == "")
        return 0;
    return recBlends(word, dict, size, results, 0, 0);
    
}

void showResults(const string results[], int size)
{
    if (size <= 0)      //base case when there is nothing to print
        return;
    showResults(results + 1, size - 1);     //recursive call
    cout << "Matching word " << results[0] << endl;
}


int main()
{
    string results[MAXRESULTS];
    string dict[MAXDICTWORDS];
    ifstream dictfile;         // file containing the list of words
    int nwords;                // number of words read from dictionary
    string word;
    
    dictfile.open("words.txt");
    if (!dictfile) {
        cout << "File not found!" << endl;
        return (1);
    }
    
    nwords = loadWords(dictfile, dict);

    cout << "Please enter a string for an anagram: ";
    cin >> word;
    
    int numMatches = recBlends(word, dict, nwords, results);
    //assert(numMatches == 20);
    if (!numMatches)
        cout << "No matches found" << endl;
    else
        showResults(results, numMatches);
    
    //check the number of words being loaded
    assert(nwords == 25143);
    
    //check if the word is the same as one of the word in dict
    string results1[MAXRESULTS];
    assert(recBlends("rat", dict, nwords, results1) == 3);
    assert(results1[0] == "art");
    assert(results1[1] == "rat");
    assert(results1[2] == "tar");
    
    //check if the word is not the same as any of the word in dict
    string results2[MAXRESULTS];
    assert(recBlends("oplo", dict, nwords, results2) == 3);
    assert(results2[0] == "loop");
    assert(results2[1] == "polo");
    assert(results2[2] == "pool");
    
    //when there is no such word in dict
    string results3[MAXRESULTS];
    assert(recBlends("regardless", dict, nwords, results3) == 0);
    
    //when there is only one word in dict
    string results4[MAXRESULTS];
    assert(recBlends("hello", dict, nwords, results4) == 1);
    assert(results4[0] == "hello");
    
    //check for case sensitivity
    string results5[MAXRESULTS];
    assert(recBlends("RaT", dict, nwords, results5) == 0);
    
    //check for empty string
    string results6[MAXRESULTS];
    assert(recBlends("", dict, nwords, results6) == 0);
}
