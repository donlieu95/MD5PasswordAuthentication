#include <iostream>
#include <list>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <time.h>
#include <chrono>
#include "MD5.h"

using namespace std;

int showMenu()
{
	cout << endl;
	cout << "MAIN MENU:" << endl;
	cout << "1)  Create a new user and set up password" << endl;
	cout << "2)  Log in with userID and password" << endl;
	cout << "3)  Generate a Rainbow Table" << endl;
	cout << "4)  Quit" << endl;
	int choice;
	cin >> choice;
	return choice;
}

void isEmpty(ifstream& in)
{
    in.open("passwords.txt");
    if (in.peek() == std::ifstream::traits_type::eof())
       cout << "warning: passwords.txt is empty" << endl;
       cout << "please add users before beginning the attack" << endl;
    in.close();
}

string strToBin(string s)
//Convert plaintext string to binary
{
	string binary = "";
	int n = s.length();
	int binLength = (n*8);
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

string intToBinary(int n)
{
    string output;
    while (n != 0)
    {
        if (n % 2 == 0)
        {
            output.insert(0, "0");
        }
        else
        {
            output.insert(0, "1");
        }
        n /= 2;
    }
    return output;
}

string genRandomSalt(int saltLength)
{
    string salt;
    for (int i = 0; i < saltLength; i++)
    {
        int randGen = rand() % 2;
        if (randGen == 1)
        {
            salt.insert(0, "1");
        }
        else
        {
            salt.insert(0, "0");
        }
    }
    return salt;
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
	string sixFour = intToBinary(endValue);
	/*
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
	*/
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

	pString[paddedLength] = '\0';
	return pString;
}

string genHash(MD5 md5, string formattedPassword)
{
    md5.hashBlock(formattedPassword);
    md5.genDigest();
    return md5.getDigest();
}
/*
string formatPassword(string password, int saltLength)
{
    password = strToBin(password);
    string salt;
    salt = genRandomSalt(saltLength);
    password = saltString(password, salt);
    password = padString(password, password.length());
    return password;
}
*/
string formatPassword(string password, int saltLength, string& salt, bool genRandomFlag)
{
    if (genRandomFlag)
        salt = genRandomSalt(saltLength);
    password = strToBin(password);
    password = saltString(password, salt);
    password = padString(password, password.length());
    return password;
}

void createNewUser(int saltLength, MD5 md5)
{
    ofstream out;
    out.open("passwords.txt");
    int userId;
    cout << "User ID: ";
    cin >> userId;
    string password;
    cout << "Password: ";
    cin >> password;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    cout << "beginning hash function timer" << endl;

    string salt;
    password = formatPassword(password, saltLength, salt, 1);
    cout << "salted password: " << password << endl;

    string hexHash;
    hexHash = genHash(md5, password);

    out << userId << " "; //", "
    out << salt << " "; //", "
    out << hexHash << endl;

    out.close();

    int duration;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    cout << "User setup successful!" << endl;
    cout << "Execution time: " << duration << " milliseconds." << endl;

}

void verifyUser(int saltLength, MD5 md5)
{
    ifstream in;
    in.open("passwords.txt");

    int inputId;
    cout << "Enter ID to verify: ";
    cin >> inputId;

    cout << "Searching for userId" << endl;

    string line;
    bool found = false;
    int userId;
    string userSalt, userHash;
    while(getline(in, line))
    {
        stringstream lineStream(line);

        lineStream >> userId;
        lineStream >> userSalt;
        lineStream >> userHash;

        if (inputId == userId)
        {
            found = true;
            break;
        }
    }

    if (found == false)
    {
        cout << "UserID does not exist" << endl;
    }

    string inputPassword;
    cout << "Enter password: ";
    cin >> inputPassword;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    inputPassword = formatPassword(inputPassword, saltLength, userSalt, 0);
    cout << "salted password: " << inputPassword << endl;

    string inputHash;
    inputHash = genHash(md5, inputPassword);

    bool correctPassword = false;
    if (inputHash.compare(userHash) == 0)
        correctPassword = true;

    if (correctPassword == true)
        cout << "Password successfully verified.  ACCESS GRANTED." << endl;
    else
        cout << "Invalid password entered.  ACCESS DENIED." << endl;

    int duration;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    cout << "Execution time: " << duration << " milliseconds." << endl;

    in.close();
}

void generateRainbowTable(int saltLength, MD5 md5)
{
    ofstream out;
    ifstream in;

    out.open("rainbowTable.txt");
    in.open("dictionary.txt");

    cout << "Generating rainbow table..." << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    unsigned int maxSalt;
    maxSalt = (int)pow(2, saltLength);
    //cout << maxSalt << endl;

    string dictionaryPassword;
    while (getline(in, dictionaryPassword))
    {
        string formattedDictionaryPassword;
        //cout << dictionaryPassword << endl;
        for(int i = 0; i < maxSalt; i++)
        {
            out << dictionaryPassword << " "; //", "
            string salt;
            int saltValue = i;
            salt = intToBinary(saltValue);
            //cout << salt << endl;

            string leadingZero;
            int numberOfLeadingZeros;
            numberOfLeadingZeros = saltLength - salt.length();
            while (salt.length() < saltLength)
            {
                salt.insert(0, "0");
                //cout << salt << endl;
            }

            formattedDictionaryPassword = formatPassword(dictionaryPassword, saltLength, salt, 0);
            string dictionaryHash;
            dictionaryHash = genHash(md5, formattedDictionaryPassword);

            out << salt << " "; //", "
            out << dictionaryHash << endl;
        }
    }
    out.close();
    in.close();
    int duration;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "Rainbow Table generated successfully for salt length = " << saltLength << " in " << duration << " milliseconds." << endl;
}


int main()
{
    MD5 md5;
    const int SALTLENGTH = 32;
    srand (time(NULL));

    int userChoice = 0;
    bool quit = false;
    while (!quit)
    {
        userChoice = showMenu();
        switch(userChoice)
        {
            case 1:
                createNewUser(SALTLENGTH, md5);
                break;
            case 2:
                verifyUser(SALTLENGTH, md5);
                break;
            case 3:
                generateRainbowTable(SALTLENGTH, md5);
                break;
            case 4:
                quit = true;
                break;
        }
    }

}
