#ifndef _spellChecker
#define _spellChecker

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <unordered_map>

using namespace std;

class Definition
{
 public:
  string Word;

  Definition(string def)
    {
      Word = def;
    }

  bool operator==(const Definition &definition) const{
    return Word == definition.Word;
  }
};

class DefinitionHash
{ 
 public:

  size_t operator()(const Definition &definition)const
  {
    return (hash<string>()(definition.Word));
  }
};


class Dictionary
{
 public:
  
  unordered_map<Definition, char, DefinitionHash> dictionary;

  Dictionary(){};
  bool fillDictionary(string fname);
  bool addDefinition(string entry);
  void searchDictionary(string word, bool report);
  void checkSpelling(string fname);
};

void printHelp(){
  cout << "Instructions for Usage" << endl;
  cout << "-exit : exits program" << endl << "-add : add word to dictionary"
       << endl << "-help : prints this help box"<< endl << "-batch : load another file into the dictionary" << endl << "-debug : toggles debug mode on/off" << endl << "default : perform spellcheck (leave field blank)" << endl;
}

void printBanner(){
  cout << "(c) Charlie Beckwith, Intro to Algorithms COEN 4650, Spring 2014" << endl << "Project 1: SpellChecker" << endl;
}
#endif
