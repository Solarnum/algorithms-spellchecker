/*
  Intro to Algorithms
  Project 1 - Spell Checker
  Author: Charlie Beckwith
  Date: 3/1/14

  Description: Reads in a text file to build a dictionary, then can load a file and spell check words against it. 
*/

#include "spellChecker.h"

//using namespace std;
static Dictionary dict;
static string DictionaryFile = "proj4650.txt";
static string spellCheckerFile = "mis4650.txt";
static bool debug = false;

unordered_map<char, int> refineList = { 
  {'!',1},{',',2},{'?',3},{':',4},{';',5},{'.',6}};

/**
 *refineString
 *param checkStr : String to be checked against the refinement list. 
 *
 *If the string contains one of the defined characters at the begininng or
 * front of the word, the character will be removed and the function will
 *recursively call itself (given that its length is > 0
 */
string refineString(string checkStr)
{
  string tempString = checkStr;
  char tempChar = checkStr.substr(0,1)[0];
  unordered_map<char,int>::const_iterator checker = refineList.find (tempChar);
  if(checker == refineList.end()){}
  else
    {
      tempString = checkStr.substr(1,checkStr.length());
      if(debug)
	cout << "deleted '" << tempChar << "' from \"" << checkStr << "\"" << endl;
      if(tempString.length() > 0)
	{
	  //refineString again
	  tempString = refineString(tempString);
	}
      else
	return tempString;
    }
  if(tempString.length() > 0)
    {
      tempChar = tempString.substr(tempString.length()-1,tempString.length())[0];
      if(debug)
	cout << tempChar << endl;
      checker = refineList.find (tempChar);
      if(checker == refineList.end())
	{
	  //no unwanted character found, do nothing
	}
      else
	{
	  //save tempstring as a substring of itself omiting the first character
	  tempString = tempString.substr(0,tempString.length()-1);
	  if(debug) 
	    cout << "deleted '" << tempChar << "' from " << "\"" << checkStr << "\"" << endl;
	  if(tempString.length() > 0)    
	    tempString = refineString(tempString);
	} 
    }
  return tempString;
}
  
  
/**
 *fillDictionary
 *param: string fname
 *Fills the dictionary hash table with words found in the file given by fname.
 *Words are refined using function refineString, and then added to the dictionary
 *by addDefinition.
 */
bool Dictionary::fillDictionary(string fname)
{
  if(fname == "")
    return false;
  fstream inStream;
  inStream.open(fname.c_str(), ios::in);
  if(inStream.fail() || inStream.eof())
    {
      cout << "file not found or eof:" << fname << endl;
      return false;
    }
  while(!inStream.eof())
    {
      string tempString;
      inStream >> tempString;
      tempString = refineString(tempString);
    
      if(addDefinition(tempString))
	{
	  if(debug)
	    cout << "added word \"" << tempString << "\"" << endl;
	}
      else
	{
	  if(debug) 
	    cout << "word eliminated" << endl;
	}
    }
  inStream.close();
  cout << fname << endl;
  return true;
}

/**
 *checkSpelling
 *param string fname
 *Loads a specified file and checks the words within the file against words stored
 *in the generated dictionary.
 */
void Dictionary::checkSpelling(string fname)
{
  if(fname == "")
    return;
  fstream inStream;
  inStream.open(fname.c_str(), ios::in);
  if(inStream.fail() || inStream.eof())
    {
      cout << "file not found or eof: " <<  fname << endl;
      return;
    }
  while(!inStream.eof())
    {
      string tempString;
      inStream >> tempString;
      tempString = refineString(tempString);
      if(tempString.length() > 0)
	searchDictionary(tempString,false);
    }
  inStream.close();
}

/**
 * addDefinition
 * param string entry
 * Creates a new Definition which is then added to the dictionary hash table.
 */
bool Dictionary::addDefinition(string entry)
{
  Definition Entry(entry);
  if(entry.length() > 0)
    {
      char entryChar = entry.substr(0,1)[0];
      dictionary.emplace(Entry,entryChar);  
    }
  else
    return false;
}
/**
 * searchDictionary
 * param string word : word to search for
 * param report : Whether or not to report that the word was found
 * Searches in dictionary hash table for a specifc word
 */    
void Dictionary:: searchDictionary(string word, bool report)
{
  unordered_map<Definition, char, DefinitionHash>::const_iterator got = dictionary.find(word);
  if(got == dictionary.end())
    cout <<"Word '"<<  word << "' not found" << endl;
  else
    {
      if(report) 
	cout << "Found: " << got->first.Word  << endl;
    }
}


int main()
{
  printBanner();
  //Fill dictionary using provided file.
  dict.fillDictionary(DictionaryFile);
  cout << "Type '-help' for more options" << endl;

  string input = "";
  while(1)
    {
      cout << ":";
      getline(std::cin, input);
      if(input == "-exit")
	return 0;
      else if(input == "-help")
	printHelp();
      else if(input == "-add")
	{
	  cout << "Add Word: ";
	  getline(std::cin, input);
	  if(dict.addDefinition(input))
	    cout << "Word '"<< input <<"' added to Dictionary" << endl;
	  else 
	    cout << "Entry failed" << endl;
	}
      else if(input == "-batch")
	{
	  cout << "Filepath: ";
	  getline(std::cin, input);
	  dict.fillDictionary(input);
	}
      else if(input == "-debug")
	{
	  if(debug)
	    debug = false;
	  else
	    debug = true;
	}
      else if(input.length() == 0)
	{
	  dict.checkSpelling(spellCheckerFile);
	}
      else
	dict.searchDictionary(input, true);
    }
  return 0;
}
