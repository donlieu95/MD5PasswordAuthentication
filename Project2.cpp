#include <iostream> 
#include <list>
#include <bits/stdc++.h>
#include <fstream> 
#include <string> 
#include <cstring>  
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <time.h>

using namespace std;
string strToBin(string s)
{
	string binary = "";
	int n = s.length();
	//cout << n;
	//cout << " ";
	int binLength = (n*8);
	//cout << binLength;
	int binPosition;
	char binaryString[binLength];

	for (int i = 0; i < n; i++)
	{
		//Convert char to ASCII
		int ascii = int(s[i]);
		if (ascii < 128)
		{
			//Convert ASCII to binary
			while (ascii > 0)
			{
				if ( (ascii % 2) == 1)
				{
					binary.push_back('1');
				}
				else
				{
					binary.push_back('0');
				}
				ascii /= 2;
			}
			binary.push_back('0');
		}
		else
		{
			//Convert ASCII to binary
			while (ascii > 0)
			{
				if ( (ascii % 2) == 1)
				{
					binary.push_back('1');
				}
				else
				{
					binary.push_back('0');
				}
				ascii /= 2;
			}
		}
		reverse(binary.begin(), binary.end());
		
		binPosition = 0;
		for (int j = 8*i; j < (8* (i+1) ); j++)
		{
			binaryString[j] = binary[binPosition];
			binPosition++;
		}
	}
	binaryString[binLength] = '\0';
	return binaryString;
}
int main ()
{
	string password;
	string binaryPassword;

	cout << "Please enter your password.  Spaces are not allowed." << endl;
	cin >> password;
	cout << "Password in plaintext is:  " << password << endl;

	binaryPassword = strToBin(password);

	cout << "Password in binary is:  " << binaryPassword << endl;
	
}
