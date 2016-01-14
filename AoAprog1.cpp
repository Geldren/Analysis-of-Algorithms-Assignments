/******************************************************************************
* Program: prog1.cpp
* 
* Author: Leif Torgersen
* 
* Class: CSC 372 Analysis of Algorithms
* 
* Instructor: Dr A. Logar
* 
* Date Due: Oct. 4 2015
* 
* Dates Work on:
* 	Sept. 24th - Wrote declarations and main
* 	Sept. 27th - wrote input and output functions
* 	Oct.  1st  - Began writing recursive function
* 	Oct.  2nd  - Finished Writing Recursion
* 	Oct.  3rd  - Added dynamic programming and error checked
* 	Oct.  4th  - Wrote documentation and submitted
* 
* Description: This program is designed to read in from a file called pairs.in
* one line at a time into a string. That string is then processed to see if 
* there is any combination  of splitting into supbtrings in which it is 
* entirtely comprised of adjacent matching pairs. Then number of the line that
* was being tested and whether or not it passed is output to the file
* pairs.out.
* 
* Complining: This Program has no special compilation or linking instructions
* 
* Bugs: This program has no known bugs
*
******************************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

bool isPair[5000] = {true,false}; //Keeps track of the state of all tested
								  //substrings

bool looked[5000] = {true,false}; //keeps track if we checked this substring

bool readIn(string& input, ifstream& fin);

bool pairFind(string input);

void printResult(ofstream& fout, bool wholePair, int stringNum);


/******************************************************************************
* Function: Main
* 
* Description: This function's purpose is to declare variables and to call the 
* other functions. Main contains my promary loop to read, process, and print 
* each sucessive line.
* 
* Parameters: 
* 	This function takes in no parameters.
* 
* 	This function outputs a 0 if the program ran successfully
* 
******************************************************************************/


int main()
	{
	int i = 0;
	int currentSize = 0;
	string input;
	int totalSize = 0;
	int stringNum = 0;
	bool wholePair = false;
	ifstream fin;
	ofstream fout;

	fin.open("pairs.in");
	fout.open("pairs.out");

	//This if the primary loop to read and process each string
	while(readIn(input, fin))
		{
		totalSize = input.size();

		//This skips the recursion if the sting is of odd length
		if(totalSize%2 == 0)
			{
			wholePair = pairFind(input);
			}
		else
			{
			wholePair = false;
			}

		//This Resets My dynamic programming tables after each string
		for(i = 1; i < input.size()+1; i++)
			{
			looked[i] = false;
			isPair[i] = false;
			}
		stringNum ++;

		printResult(fout, wholePair, stringNum);
		}

	fin.close();
	fout.close();

	return 0;
	}


/******************************************************************************
* Function: readIn
* 
* Description: This funtion reads in to a string one line at a time from the 
* input file.
* 
* Parameters: 
* 	ifstream& fin	-  The file that is read in from
* 	string& input	-  The string that is read into
* 
* 	Output Bool		-  This returns whether or not the read was sucessful
* 
******************************************************************************/


bool readIn(string& input, ifstream& fin)
	{
	if(getline(fin, input))
		return true;

	return false;
	}


/******************************************************************************
* Function: pairFind
* 
* Description: This function recursively splits a string into subpairs and 
* then tests the members of thos pairs to see if they are equal. When it finds
* a match, the rest of the string without that pair is passed down to be tested
* again
* 
* Parameters: 
* 	string& input	-  The string that contains the data to be tested.
* 
* 	Output Bool		-  This returns whether or not the string could be split
* 					   into only pairs
* 
******************************************************************************/


bool pairFind(string input)
	{
	string temp1;
	string temp2;
	string down;
	bool subPair = false;
	int currentSize = 0;

	//base case. If a string of length 0 is passed, then the result is true
	if(input.size() == 0)
		return true;

	//This checks if thestring of this size has been checked
	if(looked[input.size()])
		{
		return isPair[input.size()];
		}

	//If not, the It is set to be checks because it is now
	looked[input.size()] = true;

	//Loops through all possibly pair lengths up to half the length of the 
	//total string.
	while(subPair == false)
		{
		currentSize ++;

		if(input.size() < currentSize*2)
			return false;

		temp1 = input.substr(0,currentSize);
		temp2 = input.substr(currentSize,currentSize);

		//If a pair is found, then recusion of the rest of the string is called
		if(temp1 == temp2)
			{
			down = input.substr(currentSize*2);
			subPair = pairFind(down);
			}
		}

	isPair[input.size()] = subPair;
	return subPair;
	}


/******************************************************************************
* Function: printResult
* 
* Description: This function prints to the output file The number of strings 
* read in and which is associated with whicjh string. It then prints a yes if
* the pair matching succeeded and no if it did not.
* 
* Parameters: 
* 	ofstream& fout	-  This is the file to be output to.
* 	bool wholePair	-  This is the final result pf the pair match for each line
* 	int stringNum	-  This is the number of the string, whether it was 1st, 
* 					-  2nd, or 3rd ect.
* 
* 	This funtion return no values
* 
******************************************************************************/


void printResult(ofstream& fout, bool wholePair, int stringNum)
	{
	string Y = "yes\n";
	string N = "no\n";

	fout << stringNum << ' ';

	if(wholePair)
		fout << Y;
	else
		fout << N;
	}
