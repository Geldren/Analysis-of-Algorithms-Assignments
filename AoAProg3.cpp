/******************************************************************************
* Program: AoAProg3.cpp
* 
* Author: Leif Torgersen
* 
* Class: CSC 372 Analysis of Algorithms
* 
* Instructor: Dr A. Logar
* 
* Date Due: Dec. 1 2015
* 
* Dates Work on:
* 	Nov.  31st - Implemented Code on website
* 	Dec.  1st  - Wrote all functions
* 
* Description: This program reads in an N value, a K value, and a 
* 			series of other values representing a 
* 			of sin waves. It then performs a fast
* 			fourier transform on the data and 
* 			find the largest 5 amplitudes of the Waves.
* 
* Complining: g++ -std=c++11 [sourcefile] -o [executable file]
*			The file containg the input and the name of the 
* 			desired destination should be provided at runtime 
* 			on the command line.
* 
* Bugs: This program has no known bugs
*
******************************************************************************/

////////////////////////////////////////////////////////////////////
//           FFT based on code from CLRS Algorithms Text          //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <iomanip>

using namespace std;

const int MAX = 18096;
const double EPSILON = 1.0e-12;
const complex<double> I (0, 1);
enum  direction {FORWARD, INVERSE};
void findLargest5 (double a[], double b[], int n, double top[], int topIndex[]);

////////////////////////////////////////////////////////////////////
// complex_round - just set to zero values that are very small    //
//             makes output easier to read                        //
////////////////////////////////////////////////////////////////////

void complex_round(complex <double> a[], int n)
  {
  int    i;
  float  x;
  float  y;

  for (i = 0; i < n; i++)
    {
    x = a[i].real();
    y = a[i].imag();
    if (abs(x) < EPSILON)
      x = 0;
    if (abs(y) < EPSILON)
      y = 0;
    a[i] = complex<double>(x, y);
    }
  }

/******************************************************************************
* Function: print_polynomials
* 
* Description: This function prints the results of my find greates 5 to the file.
* 
* Parameters: 
* 	double a[]     - The array of 5 values that are my greatest amplitudes
* 	int topIndex[] - The array containing my indexes of the top 5
* 	ofstream fout  - The file stream to output to
* 
* 	Output none
* 
******************************************************************************/

void print_polynomial(double a[], int topIndex[], ofstream& fout)
{
	int i;
	for (i = 1; i < 6; i++)  // just print first 6
	{
		fout << fixed << setprecision(2) << topIndex[i] << " " << abs(a[i]) << "  " << endl;
	}
}

////////////////////////////////////////////////////////////////////
//                        FFT based on CLRS page 911              //
////////////////////////////////////////////////////////////////////

void fft(complex <double> a[], int n, complex <double> y[],
  direction dir)
{
	complex  <double> even[n];
	complex  <double> even_fft[n];
	int      i;
	int      j;
	int      n2;
	complex  <double>  x;
	complex  <double> odd[n];
	complex  <double> odd_fft[n];
	complex  <double> omega;
	complex  <double> omega_power;

	if (n == 1)
	{
		y[0] = a[0];
		return;
	}
  
	n2 = n / 2;
	j = 0;
	for (i = 0; i < n; i += 2)
	{
		even[j] = a[i];
		odd[j] = a[i + 1];
		j ++;
	}
    
	fft(even, n2, even_fft, dir);
	fft(odd, n2, odd_fft, dir);

	if (dir == FORWARD)
		omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);
	else
		omega = cos(2.0 * M_PI / n) + I * sin(2.0 * M_PI / n);
	omega_power = 1;
	for (i = 0; i < n2; i ++)
	{
		y[i] = even_fft[i] + omega_power * odd_fft[i];
		y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
		omega_power = omega * omega_power;
	}

}

/******************************************************************************
* Function: main
* 
* Description: This function reads in from the input. It then calls the fft
* 			function. Then if finds the normalized average fft
* 			values. With these values It calls the function to find
* 			the top 5 and with those calls the print function.
* 
* Parameters: 
* 	command line arguments
* 
* 	Output none
* 
******************************************************************************/

int main(int argc, char** argv)
  {
	double   a_real[MAX];
	complex  <double> a[MAX];
	complex  <double> b[MAX] = {0};
	int      i;
	ifstream inf;
	int      n;
	int      k;
	complex  <double> y[MAX] = {0};
	double   total[MAX] = {0};
	double   totalAdd[MAX] = {0};
	double   top[6] = {0};
	int      topIndex[6] = {0};
	double   a_next;
	complex  <double> omega;
	complex <double>  x;
	ofstream fout;

	inf.open(argv[1]);

	string fname = argv[1];
	int index = fname.find_last_of(".");
	fname = fname.substr(0, index);
	string outFile = fname + ".out";

	fout.open(outFile);

	inf >> n;
	inf >> k;

	for (i = 0; i < n + k; i++)
	{
		inf >> a_real[i];
		a[i] = a_real[i];
	}

  // Do forward FFT

	fft(a, n, y, FORWARD);

  // Clean up result by setting small values to zero

	omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);

	//This averages the many fft arrays

	for(i = 0; i < n; i++)
	{		
		total[i] = abs(y[i]);
	}

	for(int j = 0; j < k-1; j++)
	{
		x = 1;
		for(i = 0; i < n; i++)
		{
			y[i] = (y[i] - a[j] + a[n+j])/x;
			x = x * omega;
			total[i] += abs(y[i]);

		}
	}

	for(i = 0; i < n; i++)
	{
		total[i] = total[i] / k;
	}

	//This adds the 2 values before and after to the value

	for(i = 2; i < n-2; i++)
	{
		totalAdd[i] = total[i] + total[i-2] + total[i-1] + total[i+1] + total[i+2];
	}

	findLargest5(totalAdd, total, n, top, topIndex);

	print_polynomial(top, topIndex, fout);

	return 0;

  }

/******************************************************************************
* Function: findLargest5
* 
* Description: This function finds the greates 5 amplitudes of my frequencies.
* 			It also finds their respective indexes.
* 
* Parameters: 
* 	double a[]     - The array of my normalized total values
* 	double b[]     - The array of my not normalized total values
* 	int n          - The size of my arrays
* 	int top[]      - The array containing my values of the top 5
* 	int topIndex[] - The array containing my indexes of the top 5
* 
* 	Output none
* 
******************************************************************************/



void findLargest5(double a[], double b[], int n, double top[], int topIndex[])
{
	int      i;

	for(i = 0; i < n/2; i++)
	{
		if(top[0] < a[i]) //just find the largest value
		{
			top[0] = a[i];
			topIndex[0] = i;
		}
	}

	for(i = 0; i < n/2; i++)
	{
		if((top[1] < a[i]) && (top[0] > a[i])) //This makes sure values aren't repeated
			//The next if makes sure the right index is chosen as Totaladd has repeats
			if(b[i-1] < b[i] && b[i+1] < b[i] && b[i+2] < b[i] && b[i-2] < b[i])
			{
				top[1] = a[i];
				topIndex[1] = i;
			}
	}

	//The next loops use the same functionality as the previous one

	for(i = 0; i < n/2; i++)
	{
		if((top[2] < a[i]) && (top[1] > a[i]))
			if(b[i-1] < b[i] && b[i+1] < b[i] && b[i+2] < b[i] && b[i-2] < b[i])
			{
				top[2] = a[i];
				topIndex[2] = i;
			}
	}

	for(i = 0; i < n/2; i++)
	{
		if((top[3] < a[i]) && (top[2] > a[i]))
			if(b[i-1] < b[i] && b[i+1] < b[i] && b[i+2] < b[i] && b[i-2] < b[i])
			{
				top[3] = a[i];
				topIndex[3] = i;
			}
	}

	for(i = 0; i < n/2; i++)
	{
		if((top[4] < a[i]) && (top[3] > a[i]))
			if(b[i-1] < b[i] && b[i+1] < b[i] && b[i+2] < b[i] && b[i-2] < b[i])
			{
				top[4] = a[i];
				topIndex[4] = i;
			}
	}

	for(i = 0; i < n/2; i++)
	{
		if((top[5] < a[i]) && (top[4] > a[i]))
			if(b[i-1] < b[i] && b[i+1] < b[i] && b[i+2] < b[i] && b[i-2] < b[i])
			{
				top[5] = a[i];
				topIndex[5] = i;
			}
	}
}

