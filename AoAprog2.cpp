/******************************************************************************
* Program: prog2.cpp
* 
* Author: Leif Torgersen
* 
* Class: CSC 372 Analysis of Algorithms
* 
* Instructor: Dr A. Logar
* 
* Date Due: Nov. 1 2015
* 
* Dates Work on:
* 	Oct.  26th - Wrote main and file read functions
* 	Oct.  31st - Wrote all functions except the bisection function
* 	Nov.  1st  - Wrote and error checked the bisection funtion
* 
* Description: This program reads in an integer and a series of paired double
* 			precision numbers. It then finds out whenther they points for a 
* 			convex polygon. If not, no solution is output to the file. If
*			so, then a line is found which bisects the polygon and is then
*			adjusted until it perfectly splits the area of the polygon in 2.
* 
* Complining: g++ -std=c++11 [sourcefile] -o [executable file]
*			The file containg the input and the name of the desired destination
*			should be provided at runtime on the command line.
* 
* Bugs: This program has no known bugs
*
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

struct point
{
	double x;
	double y;
};

bool readIn(int vertNum, point p[], ifstream& fin, ofstream& fout);

double cross(point a, point b);

double direction(point a, point b, point c);

double area(point p[], int n);

bool isConvex(point p[], int vertNum);

double biSection(point p[], double max, double min, int vertNum);

double areaSplit(point p[], int vertNum);

void writeOut(double answer, ofstream& fout, int caseNum);


/******************************************************************************
* Function: main
* 
* Description: This function calls readIn which contains my main program loop
*			and calls the write fuction. It also handles the opening and 
*			closing of the files.
* 
* Parameters: 
* 	command line arguments
* 
* 	Output none
* 
******************************************************************************/


int main(int argc, char **argv)
{
	ifstream fin;
	ofstream fout;
	string fname = argv[1];
	int index = fname.find_last_of(".");
	fname = fname.substr(0, index);
	string outFile = fname + ".out";

	fin.open(argv[1]);
	fout.open(outFile);
	int vertNum = 0;
	point p [100];
	
	readIn(vertNum, p, fin, fout);
	fin.close();
	fout.close();
}


/******************************************************************************
* Function: readIn
* 
* Description: This funtion reads in to a string one line at a time from the 
* input file.
* 
* Parameters: 
*	int vertNum     -  The number of verticies in a polygon
*	point p[]       -  The array of point structs conatining my verticies
* 	ifstream& fin	-  The file that is read in from
*	ofstream& fout	-  The file that is written out to
* 
* 	Output Bool		-  This returns whether or not the read was sucessful
* 
******************************************************************************/


bool readIn(int vertNum, point p [], ifstream& fin, ofstream& fout)
{
	bool convex = true;
	int caseNum = 0;

	while(fin>> vertNum)
	{
		int i = 0;
		caseNum++;
		double answer = 0;
		while(i<vertNum)
		{
			fin >> p [i].x;	
			fin >> p [i].y;
			i++;
		}
		convex = isConvex(p, vertNum);
		if(convex == true)
		{
			answer = areaSplit(p, vertNum);
		}
		writeOut(answer, fout, caseNum);
	}
}


/******************************************************************************
* Function: direction
* Author: Dr. E Corwin & Dr. A Logar
* 
******************************************************************************/


double direction(point a, point b, point c)
{
  point ab;
  point bc;
  double result;

  ab.x = b.x - a.x;
  ab.y = b.y - a.y;
  bc.x = c.x - b.x;
  bc.y = c.y - b.y;
  result =  cross(ab, bc);
  if (fabs(result) < 1.0e-6)
    result = 0.0;
  return result;
}


/******************************************************************************
* Function: cross
* Author: Dr. E Corwin & Dr. A Logar
* 
******************************************************************************/


double cross(point a, point b)
{
  return a.x * b.y - a.y * b.x;
}


/******************************************************************************
* Function: area
* Author: Dr. E Corwin & Dr. A Logar
* 
******************************************************************************/


double area(point p[], int n)
{
  int i;
  int j;
  double result;

  result = 0;
  for (i = 0; i < n; i++)
    {
    j = (i + 1) % n;
    result += p[i].x * p[j].y;
    result -= p[i].y * p[j].x;
    }
  return fabs(result / 2);
}


/******************************************************************************
* Function: isConvex
* 
* Description: This funtion checks the polygon to make sure it is convex.
* 
* Parameters: 
*	int vertNum     -  The number of verticies in a polygon
*	point p[]       -  The array of point structs conatining my verticies
* 
* 	Output Bool		-  This returns whether or not the polygon is convex
* 
******************************************************************************/


bool isConvex(point p[], int vertNum)
{
	int i = vertNum - 1;
	double turn = 0;
	bool wasLast = false;
	bool wasOne = false;
	bool firstTurn = false;
	bool isNegative = false;
	point a;
	point b;
	point c;

	while(i>=0)
	{
		a.x = p[i].x;
		a.y = p[i].y;
		if(i==0)
		{
			i = vertNum;
			wasLast = true;
		}	
		b.x = p[i-1].x;
		b.y = p[i-1].y;
		if(i==1)
		{
			i = vertNum;
			wasOne = true;
		}	
		c.x = p[i-2].x;
		c.y = p[i-2].y;
		turn = direction(a, b, c);
		if(wasLast == true)
		{
			i = 0;
			wasLast = false;
		}
		else if(wasOne == true)
		{
			i = 1;
			wasOne = false;
		}
		if(i == vertNum-1)
		{
			if(turn < 0)
			{
				firstTurn = true;
			}
		}
		if(turn < 0)
		{
			isNegative = true;
		}
		if(firstTurn != isNegative)
		{
			return false;
		}
		i--;
	}
	return true;
}


/******************************************************************************
* Function: areaSplit
* 
* Description: This funtion finds an initial midpoint to bisect with.
* 
* Parameters: 
*	int vertNum     -  The number of verticies in a polygon
*	point p[]       -  The array of point structs conatining my verticies
* 
* 	Output double	-  This returns the final x value of the bisection.
* 
******************************************************************************/


double areaSplit(point p[], int vertNum)
{
	double min = 9999999999999999;
	double max = 0;

	for(int i=0; i<vertNum; i++)
	{
		if(p[i].x < min)
			min = p[i].x;
		if(p[i].x > max)
			max = p[i].x;
	}
	return biSection(p, max, min, vertNum);
}


/******************************************************************************
* Function: biSection
* 
* Description: This funtion finds the final x value of the bisection.
* 
* Parameters: 
*	int vertNum     -  The number of verticies in a polygon
*	point p[]       -  The array of point structs conatining my verticies
*	double max		-  This is used as the new max with each loop to fine the 
*						new middle
*	double min		-  This is used as the new min with each loop to fine the 
*						new middle
* 
* 	Output double	-  This returns the final x value of the bisection.
* 
******************************************************************************/


double biSection(point p[], double max, double min, int vertNum)
{
	double middle = 0;
	point bisectUp;
	point bisectDown;
	point areaTest[100];
	bool equal = false;
	int i = 0;
	double slope = 0;
	double intercept = 0;

	while(equal == false)
	{
		middle = (max-min)/2 + min;
		bisectUp.x = middle;
		bisectUp.y = 100;
		bisectDown.x = middle;
		bisectDown.y = -10;
		p[vertNum].x = p[0].x;
		p[vertNum].y = p[0].y;
		int j = 0;

		for(i = 0; i<vertNum; i++)
		{
			if(p[i].x < middle)
			{
				areaTest[j] = p[i];
				j++;
			}
			if(p[i].x < middle && p[i+1].x > middle)
			{
				slope = (p[i+1].y - p[i].y)/(p[i+1].x - p[i].x);
				intercept = p[i].y - slope * p[i].x;
				bisectUp.y = slope * middle +intercept;
				areaTest[j] = bisectUp;
				j++;
			}
			if(p[i].x > middle && p[i+1].x < middle)
			{
				slope = (p[i+1].y - p[i].y)/(p[i+1].x - p[i].x);
				intercept = p[i].y - slope * p[i].x;
				bisectDown.y = slope * middle +intercept;
				areaTest[j] = bisectDown;
				j++;
			}
		}
		if((area(areaTest, j) > area(p, vertNum)/2) && 
			(fabs(area(areaTest, vertNum) - area(p, vertNum)/2) > 1e-6))
		{
			max = middle;
		}
		if((area(areaTest, j) < area(p, vertNum)/2) && 
			(fabs(area(areaTest, vertNum) - area(p, vertNum)/2) > 1e-6))
		{
			min = middle;
		}
		if(fabs(area(areaTest, j) - area(p, vertNum)/2) < 1e-6)
		{
			equal = true;
		}
	}
	return middle;
}


/******************************************************************************
* Function: writeOut
* 
* Description: This funtion writes my results out to a file.
* 
* Parameters: 
*	int caseNum     -  The number cases solved
*	ofstream& fout	-  The file that is written out to
*	double answer	-  This is the final x value for the bisector
* 
* 	Output double	-  This returns the final x value of the bisection.
* 
******************************************************************************/


void writeOut(double answer, ofstream& fout, int caseNum)
{
	if( answer == 0)
		fout << "Case "<< caseNum << ": " << "No solution\n";
	else
		fout <<"Case " << caseNum << ": "<< fixed << setprecision(5) 
			<< answer << endl;
}

