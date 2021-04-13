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

string saltString(string pass, string salt)
{
	int saltedLength = pass.length() + salt.length();
	char saltString[saltedLength];
	
	//Copy password string
	for(int i = 0; i < pass.length(); i++)
	{
		saltString[i] = pass[i];
	}

	//Add salt
	int saltPosition = 0;
	for(int j = pass.length(); j < saltedLength; j++)
	{
		saltString[j] = salt[saltPosition];
		saltPosition++;
	}

	saltString[saltedLength] = '\0';
	return saltString;
}

string padString(string s, int orgLength)
{
	int paddedLength, padding;

	//Find length of padded binary string
	if ( (orgLength % 512) == 0)
	{
		paddedLength = orgLength + 512;
	}
	else
	{
		padding = 512 - (orgLength % 512);
		paddedLength = orgLength + padding;
	}

	char pString[paddedLength];
	//Copy original string
	for (int i = 0; i < orgLength; i++)
	{
		pString[i] = s[i];
	}
	//Pad string
	pString[orgLength] = '1';

	for (int j = orgLength+1; j < (paddedLength-64); j++)
	{
		pString[j] = '0';
	}

	//Add 64-bit representation of salted password to end
	int orgPosition = 0;
	for (int k = (paddedLength-64); k < paddedLength; k++)
	{
		if (orgPosition < orgLength)
		{
			pString[k] = s[orgPosition];
			orgPosition++;
		}
		else
		{
			orgPosition = 0;
			pString[k] = s[orgPosition];
			orgPosition++;
		}
	}

	pString[paddedLength] = '\0';
	return pString;	
}
string md5Hash(string pass)
{
	string A, B, C, D;
	//A is hex 10:32:54:76
	A = "00010000001100100101010001110110";
	//B is hex FE:DC:89:AB
	B = "11111110110111001000100110101011";
	//C is hex 98:BA:EF:CD
	C = "10011000101110101110111111001101";
	//D is hex 67:65:23:01
	D = "01100111011001010010001100000001";
}
int main ()
{
	int saltLength = 48;
	char salt[saltLength];
	string password, binaryPassword, saltedPassword, paddedPassword;
	int padding, paddedLength;

	cout << "Please enter your password.  Spaces are not allowed." << endl;
	cin >> password;
	cout << "Password in plaintext is:  " << password << endl;

	binaryPassword = strToBin(password);

	cout << "Password in binary is:  " << binaryPassword << endl;

	//Generate salt with random binary string
	srand (time(NULL));
	for (int i = 0; i < saltLength; i++)
	{
		int randGen = rand() % 2;
		if (randGen == 1)
		{
			salt[i] = '1';
		}
		else
		{
			salt[i] = '0';
		}
	}
	salt[saltLength] = '\0';

	cout << "Salt generated:  " << salt << endl;

	//Combine password with salt
	saltedPassword = saltString(binaryPassword, salt);

	cout << "Salted password:  " << saltedPassword << endl;
	
	//Pseudo-random number generator seeded with current time

	paddedPassword = padString(saltedPassword, saltedPassword.length());
	
	//cout << "Original binary length:  " << binaryPassword.length() << endl;
	//cout << "Padded binary length:  " << paddedLength << endl;
	cout << "Padded password: " << paddedPassword << endl;
	
}
