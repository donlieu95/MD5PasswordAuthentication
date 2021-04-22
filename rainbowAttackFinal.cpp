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
#include "MD5.cpp"

using namespace std;

int showMenu()
//Displays menu
{
	cout << endl;
	cout << "MAIN MENU:" << endl;
	cout << "1)  Create a new user and set up password" << endl;
	cout << "2)  Log in with userID and password" << endl;
	cout << "3)  Generate a rainbow table" << endl;
	cout << "4)  Attack userID using rainbow table" << endl;
	cout << "5)  Quit" << endl;
	cout << "Enter your choice:  ";
	int choice;
	cin >> choice;
	return choice;
}

void isEmpty()
//Checks if password file is empty
{
    ifstream in;
    in.open("passwords.txt");
    if (in.peek() == std::ifstream::traits_type::eof())
    {
        cout << "warning: passwords.txt is empty" << endl;
        cout << "please add users before beginning the attack" << endl;
    }
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
//Converts an integer to binary
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
//Generates a random salt of the inputted salt length
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
//Pad string according to MD5 algorithm specifications
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
//Generate a hashcode using a password that has been salted and padded
{
    md5.hashBlock(formattedPassword);
    md5.genDigest();
    return md5.getDigest();
}
string formatPassword(string password, int saltLength, string& salt, bool genRandomFlag)
//Salts and pads a plaintext password
{
    if (genRandomFlag)
        salt = genRandomSalt(saltLength);
    password = strToBin(password);
    password = saltString(password, salt);
    password = padString(password, password.length());
    return password;
}
bool findUser(int userId)
//Finds a given User ID
{
    ifstream in;
    in.open("passwords.txt");
    string line;
    bool found = false;
    int inputId;
    string userSalt, userHash;
    while(getline(in, line))
    {
        stringstream lineStream(line);

        lineStream >> inputId;
        lineStream >> userSalt;
        lineStream >> userHash;

        if (inputId == userId)
        {
            found = true;
	    in.close();
            return true;
        }
    }

    if (found == false)
    {
	in.close();
        return false;
    }
	
}
void createNewUser(int saltLength, MD5 md5)
//Create a new User ID and pair it with a password
{
    int userId;
    cout << "User ID: ";
    cin >> userId;
    if ( !findUser(userId) )
    {
	    ofstream out;
	    out.open("passwords.txt", ios::app);
	    string password;
	    cout << "Password: ";
	    cin >> password;

	    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	    string salt;
	    password = formatPassword(password, saltLength, salt, 1);

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
    else
    {
             cout << "ERROR.  User already exists in system.  Aborting..." << endl;
    }

}

void verifyUser(int saltLength, MD5 md5)
//Given a user ID and password combination, verify access
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
            cout << "User ID found" << endl;
            break;
        }
    }

    if (found == false)
    {
        cout << "User ID does not exist" << endl;
    }

    string inputPassword;
    cout << "Enter password: ";
    cin >> inputPassword;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    inputPassword = formatPassword(inputPassword, saltLength, userSalt, 0);

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

void verifyUser(MD5 md5, string rainbowUserId, string rainbowPassword, string rainbowSalt)
//For a rainbow attack, given a user ID and password combination, verify access
{
    ifstream in;
    in.open("passwords.txt");

    cout << "Attacking user ID: " << rainbowUserId << endl;

    string line;
    bool found = false;
    string userId, userSalt, userHash;
    while(getline(in, line))
    {
        stringstream lineStream(line);

        lineStream >> userId;
        lineStream >> userSalt;
        lineStream >> userHash;

        if (rainbowUserId.compare(userId) == 0)
        {
            found = true;
            break;
        }
    }

    if (found == false)
    {
        cout << "UserID does not exist" << endl;
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int saltLength;
    saltLength = rainbowSalt.length();
    string formattedRainbowPassword;
    formattedRainbowPassword = formatPassword(rainbowPassword, saltLength, userSalt, 0);

    string rainbowHash;
    rainbowHash = genHash(md5, formattedRainbowPassword);

    bool correctPassword = false;
    if (rainbowHash.compare(userHash) == 0)
        correctPassword = true;

    if (correctPassword == true)
        cout << "Password successfully verified.  ATTACK SUCCESSFUL." << endl;
    else
        cout << "Invalid password entered.  ATTACK FAILED." << endl;

    int duration;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    cout << "Execution time: " << duration << " milliseconds." << endl;

    in.close();
}

void generateRainbowTable(int saltLength, MD5 md5)
//Generates a rainbow table
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

void attackUserId(MD5 md5)
//Perform a Rainbow Table attack a selected User ID
{
    ifstream userHashIn;
    userHashIn.open("passwords.txt");
    ifstream rainbowHashIn;
    rainbowHashIn.open("rainbowTable.txt");

    string userId;
    cout << "User ID to attack: ";
    cin >> userId;

    string line, lineSegment, passwordsFileHash;
    while (getline(userHashIn, line))
    {
        //lineSegment = line.substr(0, line.find(" "));
        //cout << "lineSegment: " << lineSegment << endl;
        if (line.find(userId, 0) != string::npos)
        {
            stringstream passwordsLineStream(line);
            passwordsLineStream >> passwordsFileHash >>
                passwordsFileHash >> passwordsFileHash;
            break;
        }
    }

    string rainbowFilePassword, rainbowFileSalt;
    while(getline(rainbowHashIn, line))
    {
        if (line.find(passwordsFileHash, 0) != string::npos)
        {
            stringstream rainbowLineStream(line);
            rainbowLineStream >> rainbowFilePassword >> rainbowFileSalt;
            break;
        }
    }

    userHashIn.close();
    cout << "We believe the user's login information is:" << endl;
    cout << "   ID: " << userId << endl;
    cout << "   Salt: " << rainbowFileSalt << endl;
    cout << "   Password: " << rainbowFilePassword << endl;
    cout << "Launching login sequence using attack setup" << endl;
    verifyUser(md5, userId, rainbowFilePassword, rainbowFileSalt);
}

int main()
{
    MD5 md5;
    const int SALTLENGTH = 1; //Max length of 48
    srand (time(NULL));

    int userChoice = 0;
    bool quit = false;
    while (!quit)
    {
        isEmpty();
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
                attackUserId(md5);
                break;
            case 5:
                quit = true;
                break;
        }
    }

}
