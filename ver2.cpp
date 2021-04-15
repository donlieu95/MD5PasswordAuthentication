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
//Convert plaintext string to binary
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
//Given binary password string, add salt
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

	//Add 64-bit representation of salted password length to end
	//Convert salted password length to binary
	int endValue = orgLength;
	string sixFour = "";
	while (endValue > 0)
	{
		if ( (endValue % 2) == 1)
		{
			sixFour.push_back('1');
		}
		else
		{
			sixFour.push_back('0');
		}
		endValue /= 2;
	}
	reverse(sixFour.begin(), sixFour.end());
	//Add 0s to the beginning of the 64 digit binary value (the converted binary length will most likely not span 64 digits)
	for (int k = (paddedLength-64); k < ( paddedLength - sixFour.length() ); k++)
	{
		pString[k] = '0';
	}
	//Fill in the rest with the converted binary value
	int endPosition = 0;
	for (int l = (paddedLength - sixFour.length() ); l < paddedLength; l++)
	{
		pString[l] = sixFour[endPosition];
		endPosition++;
	} 

	cout << paddedLength << endl;
	pString[paddedLength] = '\0';
	return pString;	
}

string md5Hash(string pass)
//Placeholder function for hash function to be written.  Currently returns the exact same string
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

	//Return same string for now.  Change this to 128-bit hash code later when algorithm is written
	return pass;
}

string toHex(string pass)
//Converts a binary string to hexadecimal string.  Must input a string with length that is a multiple of 4, or this will not work correctly.
{
	int hexLength = pass.length() / 4;
	char hexString[hexLength];

	for (int i = 0; i < hexLength; i++)
	{
		char hexBin[4], hexChar;
		int binPosition = 0, hexValue = 0;
		//Get next 4 bits
		for (int j = (4*i); j < (4 * (i+1) ); j++)
		{
			hexBin[binPosition] = pass[j];
			binPosition++;
		}
		//Get int value of 4 bits
		if (hexBin[0] == '1')
		{
			hexValue += 8;
		}
		if (hexBin[1] == '1')
		{
			hexValue += 4;
		}
		if (hexBin[2] == '1')
		{
			hexValue += 2;
		}
		if (hexBin[3] == '1')
		{
			hexValue += 1;
		}
		//Convert int value into hex
		if (hexValue < 10)
		{
			hexChar = '0'+hexValue;
		}
		else if (hexValue = 10)
		{
			hexChar = 'a';
		}
		else if (hexValue = 11)
		{
			hexChar = 'b';
		}
		else if (hexValue = 12)
		{
			hexChar = 'c';
		}
		else if (hexValue = 13)
		{
			hexChar = 'd';
		}
		else if (hexValue = 14)
		{
			hexChar = 'e';
		}
		else
		{
			hexChar = 'f';
		}
		//Insert hexadecimal digit into hex string
		hexString[i] = hexChar;
	}
	hexString[hexLength] = '\0';
	return hexString;
}

void showMenu()
{
	cout << "1. Add new user with password." << endl;
	cout << "2. Verify user and password." << endl;
	cout << "3. Erase all users in file." << endl;
	cout << "4. Perform rainbow table attack." << endl;
	cout << "5. Quit." << endl;
}

int main()
{
	int saltLength = 48;
	char salt[saltLength];
	string password, binaryPassword, saltedPassword, paddedPassword, binHashCode, hexHashCode, verPassword, filler;
	int padding, paddedLength, choice, userId, verfyId;
	
	ofstream passwordWrite;
	ifstream userFile;
	bool found = false;
	
	cout << "Welcome to the MD5 password simulator! Please choose an option." << endl;
	showMenu();
	cin >> choice;
	
	while(choice != 5)
	{
		// add new user.
		if (choice == 1)
		{
			cout << "Please enter user id number: ";
			cin >> userId;
			cout << "Please enter user's password, no spaces allowed: ";
			cin >> password;
			
			// generate random salt value
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
			
			// hashing steps to encrypt password. couts for troubleshooting
			password = strToBin(password);
			//cout << "Password in bin.. \n";
			saltedPassword = saltString(password, salt);
			//cout << "Password now salty.\n";
			paddedPassword = padString(saltedPassword, saltedPassword.length());
			//cout << "padding done.\n";
			binHashCode = md5Hash(paddedPassword);
			//cout << "now hashed.\n";
			hexHashCode = toHex(binHashCode);
			//cout << "sucessfully hexed.\n";
			
			// open password file and add new user id, salt, and hash digest to new line
			cout << "Opening file..." << endl;
			passwordWrite.open("password.txt", ios::app );

			passwordWrite << userId << " " << salt << " " << hexHashCode << endl;

			passwordWrite.close();
		}
		
		if (choice == 2)
		{
			cout << "Enter the id number you wish to verify: ";
			cin >> verfyId;
			userFile.open("password.txt")
			// try to find matching id in file
			while(!userFile.eof() || !found)
			{
				userFile >> userId;
				if (verfyId.compare(userId) == 0)
				{
					userFile >> salt >> hexHashCode;
					found = true;
				}
				// move past salt and hash if no match
				else
					userFile >> filler >> filler;
			}
			
			if(found)
			{
				cout << "Enter password for user " << userId << " to begin verification: ";
				cin >> verPassword;
				// run entered password though encryption to compare to hash digest
				binaryPassword = strToBin(verPassword);
				saltedPassword = saltString(binaryPassword, salt);
				paddedPassword = padString(saltedPassword, saltedPassword.length());
				binHashCode = md5Hash(paddedPassword);
				verPassword = toHex(binHashCode);
				
				if(verPassword.compare(hexHashCode) == 0)
					cout << "password successfully verified!" << endl;
				else
					cout << "Sorry, password does not match." << endl;
			}
			else
				cout << "No such user Id exists in our database." << endl;
		}
		
		if (choice == 3)
		{
			passwordWrite.open("password.txt");
			passwordWrite.close();
			cout << "Password file Erased." << endl;
		}
		
		if (choice == 4)
			//Perform Rainbow Table attack on selected userID
		{
			cout << "Performing rainbow table attack..." << endl;
			//Open outfile
			//Input text file of all dictionary words
			//Given length of generated salt value, find all possible salt values.  This will be 2^(saltLength+1) possible combinations.
			//For every dictionary word:
				//Output dictionary word to outfile with endline
				//Convert dictionary word to binary string
				//For every salt value:
					//Combine converted binary string with salt value
					//Run the result through padding function
					//Run the result through MD5 hashing algorithm
					//Run the result through hex conversion function
					//Output result to outfile with endline

			//This will populate an enormous rainbow table with 2^(saltLength+1) possible hashcode entries for every dictionary word
			//Hashcodes in the password file can be tested against the rainbow table (select the hashcode corresponding to a user account, search for the hashcode in the rainbow table, and find matching entries.  The dictionary word(s) corresponding to match(es) can be tested using the Password Authentication option from choice 2.

			//(If there exists an efficient way to store this rainbow table within the program itself, a function can be added to select a user's hash value and parse the rainbow table for matches.  This can output corresponding dictionary words for the attacker to try... However, this may be unnecessarily complicated.)

		}
		else
		{
			cout << "ERROR. Invalid choice." << endl;
		}
		// repeat menu until exited.
		showMenu();
		cout << "Please enter a number to select your choice:  ";
		cin >> choice;
	}
}