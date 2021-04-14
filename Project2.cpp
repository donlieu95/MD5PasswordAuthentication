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
struct Node
{
	int id;
	string saltCode;
	string hashCode;
	struct Node *next;
};

struct Node* first = NULL;

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
	cout << "1)  Create a new user and set up password" << endl;
	cout << "2)  Log in with userID and password" << endl;
	cout << "3)  Perform a Rainbow Table attack on a selected userID" << endl;
	cout << "4)  Quit" << endl;
}

int main ()
{
	int saltLength = 48;
	char salt[saltLength];
	string password, binaryPassword, saltedPassword, paddedPassword, binHashCode, hexHashCode;
	int padding, paddedLength, choice, userId;
	bool done = false;

	ifstream infile;
	ofstream outfile;

	cout << "\nWelcome to the MD5 Password Simulator!" << endl;
	showMenu();
	cout << "Please enter a number to select your choice:  ";
	cin >> choice;

	while (choice != 4)
	{
		if (choice == 1)
		//Create new userID and password
		{
			int setupChoice;
			cout << "Do you wish to load from a previous password file?" << endl;
			cout << "1) Yes. Load from an existing password file." << endl;
			cout << "2) No. Create a new password and overwrite any existing password file." << endl;
			cout << "Please enter a number to select your choice:  ";
			cin >> setupChoice;

			if (setupChoice == 1)
			//Loading from file not working yet
			{
				infile.open("password.txt");
				if(!infile)
				{
					cout << "No password file found.  Creating new password file." << endl;
					infile.close();
				}
				else
				{
					while ( !infile.eof() )
					//Load id, salt, and hash from file
					{
						int i;
						string s, h;
						infile >> i;
						infile >> s;
						infile >> h;
						Node *u = new Node;
						u -> id = i;
						u -> saltCode = s;
						u -> hashCode = h;
						u -> next = first;
						first = u;
					}
					cout << "Password file loaded successfully!" << endl;
				}
			}
			else if (setupChoice == 2)
			{
				cout << "Creating new password file..." << endl;
			}
			else
			{
				cout << "ERROR.  Invalid choice.  Returning to main menu..." << endl;
				done = true;
			}

			outfile.open("password.txt");
			while (!done)
			{
				cout << "Please enter an integer number for the new userID:  ";
				cin >> userId;
				cout << "Please enter the password for this user.  Spaces are not allowed." << endl;
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
				cout << "Padded password: " << paddedPassword << endl;

				//Input padded password into hash algorithm

				binHashCode = md5Hash(paddedPassword);
				cout << "Hash code in binary:  " << binHashCode << endl;

				//Convert binary hash code to hexidecimal
				
				hexHashCode = toHex(binHashCode);
				cout << "Final hexidecimal hash code:  " << hexHashCode << endl;

				Node *u = new Node;
				u -> id = userId;
				u -> saltCode = salt;
				u -> hashCode = hexHashCode;
				u -> next = first;
				first = u;
				cout << "User setup successful!" << endl;
				
				
				//Prompt for another user setup
				cout << "Set up another user?" << endl;
				cout << "1) Yes" << endl;
				cout << "2) No" << endl;
				cout << "Please enter a number to select your choice:  ";
				cin >> setupChoice;

				if (setupChoice == 1)
				{
					done = false;
				}
				else if (setupChoice == 2)
				{
					Node *p = first;
					while (p != NULL)
					{
						outfile << p -> id << " ";
						outfile << p -> saltCode << " ";
						outfile << p -> hashCode << endl;
						outfile << endl;
						p = p -> next;
					}
					cout << "Password file written successfully!" << endl;
					done = true;
				}
				else
				{
					cout << "ERROR.  Invalid choice.  Returning to main menu..." << endl;
					done = true;
				}
			}

			//Main menu
			outfile.close();
			showMenu();
			cout << "Please enter a number to select your choice:  ";
			cin >> choice;
		}
		else if (choice == 2)
		//Log in with userID and password
		{
			cout << "Logging in..." << endl;

			//Main menu
			showMenu();
			cout << "Please enter a number to select your choice:  ";
			cin >> choice;
		}
		else if (choice == 3)
		//Perform Rainbow Table attack on selected userID
		{
			cout << "Performing rainbow table attack..." << endl;

			//Main menu
			showMenu();
			cout << "Please enter a number to select your choice:  ";
			cin >> choice;
		}
		else
		{
			cout << "ERROR. Invalid choice." << endl;
			showMenu();
			cout << "Please enter a number to select your choice:  ";
			cin >> choice;
		}
	}
	cout << "Exiting system..." << endl;
	return 0;
	
}
