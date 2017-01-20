/*
	K9 Web Protection Password Cracker v1.0 source code
	Copyright (c) 2014 izacktheyak.com. All rights reserved.
	Author: Isaac "Yak" L

	Released under GNU General Public License v3.0 - https://www.gnu.org/licenses/gpl-3.0.en.html
	
	Visit http://izacktheyak.com/projects/k9-web-protection-password-cracker/ for more information.

	Description:
	This tool allows the user to recover the password of their K9 Web Protection installation.

	The following tool makes use
	of dietrich's discovery about
	the password hashing algorithm
	of K9 Web Protection software,
	which he published at:

	http://insecurety.net/?p=148
*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include <math.h>


using namespace std;

//function prototypes
bool loadLicenseFile(string *);
void printLicenseFileContents();
char getOffsetValue();
void getHash(unsigned char *);
bool reverseHash(unsigned char *, unsigned char, string *);
int getHashLength();


//byte number 45 of the license file is the location of the amount to subtract from the password hash inorder to decypher the code
const unsigned int OFFSETLOC = 44;

//byte number 465 of the license file is the location of the password hash
const unsigned int PASSWORDHASHLOC = 464;

//byte number 181 of the license file is the location of the length of the store password hash
const unsigned int HASHLENGTHLOC = 180;

//the maximum length of the password hash
const unsigned int PASSWORDMAXLENGTH = 15;

//license file contents
unsigned char * licenseFileContents;

//size of the license file
ifstream::pos_type size;

//store default cout flags
const ios::fmtflags COUTFLAGDEFAULTS = cout.flags();




int main()
{
	//set colors
	system("color 0A");//set the background to black and foreground to light green

	//set window/buffer size
	system("mode con:cols=80 lines=40");

	//set the title of the console window
	SetConsoleTitle(L"K9 Web Protection Password Cracker");

	//print program info
	cout << "K9 Web Protection Password Cracker v1.0\n";
	cout << "Copyright (c) 2014 izacktheyak.com. All rights reserved.\n";
	cout << "Visit http://izacktheyak.com/projects/k9-web-protection-password-cracker/ for updates and more information about this program.\n";
	cout << "\n";
	cout << "NOTICE: This program is intended to help with the recovery of lost passwords. By using this program, you agree that the author is not liable for any lose or damage you incur from use or misuse of this program.\n";
	cout << "\n\n";

	//welcome the user!
	cout << "Welcome ";
	system("echo %username%!");

	//instruct the user
	cout << "\n";
	cout << "If you have K9 Web Protection installed on this computer, press any key to get started.\n";
	cout << "\n";
	cout << "-OR-\n";
	cout << "\n";
	cout << "If you would like to crack the password of a K9 Web Protection license file, simply place the license file in the same directory as this program, make sure it is named \"license\", and then press any key to get started.\n";
	
	//wait for the user to press any key
	_getch();

	//clear the screen
	system("cls");

	cout << "Loading the license file... ";//notify the user that the license file is loading

	string * licenseLoc = new string();//string to hold the location from which the license was loaded
	
	//if the license file loaded successfully
	if(loadLicenseFile(licenseLoc))
	{
		cout << "[Success]\n";//print success
		cout << "License File Location: " << *licenseLoc << "\n";//print the location that the license file was loaded from
		cout << "Size: " << size << "bytes\n";//print the size of the license file

		//print the contents of the license file
		//printLicenseFileContents();
	
		//load offset
		char offsetValue = getOffsetValue();//get value of offset
	
		//print the offset value for the user to see
		cout << "Offset Value: " << hex << setw(2) << setfill('0') << (unsigned int)offsetValue << endl;
	
		//load password hash
		unsigned char * passwordHash = new unsigned char[PASSWORDMAXLENGTH];//array for holding password hash
		getHash(passwordHash);//get the password hash
		
		//print the password hash for the user to see
		cout << "Password Hash: ";
	
		//move through the password hash array
		for( int i = 0 ; i < PASSWORDMAXLENGTH ; i++)
		{
			//print the current character in the password hash array
			cout << hex << setw(2) << setfill('0') << (unsigned int)passwordHash[i] << " " ;
		}
		cout << "\n";//move to next line
	
		//restore cout flags to default
		cout.flags(COUTFLAGDEFAULTS);

		if(getHashLength() > 1 && getHashLength() < 16)//if the predicted password length is in the allowed password length range
		{
			//print the probable length of the password
			cout << "Probable Password Length: " << getHashLength() << endl;
		}
	
		string crackedPassword = "";//variable for holding the password
	
		//notify the user that we are decyphering the hash aka cracking the password
		cout << "\n\nCracking password... ";
	
		//if the password is successfully reversed
		if(reverseHash(passwordHash,offsetValue,&crackedPassword))
		{
			cout << "[Success]\n";//print success
			cout << "\n";
			cout << "===============================================================================\n";
			cout << "|                             Possible Passwords                              |\n";
			cout << "===============================================================================\n";
			if(getHashLength() > 1 && getHashLength() < 16)//if the predicted password length is in the allowed password length range
			{
				cout << crackedPassword.substr(0,getHashLength()) << "\n";//print password
			}
			if(crackedPassword.substr(0,getHashLength()) != crackedPassword)//if the alternative password is not the same as the previous one
			{
				cout << crackedPassword << "\n";//print alternative password
			}
			cout << "\n";
			cout << "\n";
			cout << "\n";
			cout << "\n";
			cout << "[Note] Occasionally trailing characters may be left on the end of passwords. If the listed password(s) did not work, try removing characters one by one from the password(s), and test to see if one of those is the password.\n";
			
			//cout << "\n\nNOTE: Sometimes the cracked password has been seen including one or two characters at the end of it which are not part of the valid password. This is due to not knowing the length of the password before beginning the reverse hashing method. Hopefully this issue can be solved in the future.\n\nIf K9 Web Protection says that the cracked password is invalid, simply remove a character from the end of it, and see if that is the correct password. If necessary, continue to remove additional characters until the password is finally correct.\n";//notify the user of a known issue
		}
		else//password was not successfully reversed
		{
			cout << "[Failure]\n";//print failure
			cout << "\n\n";
			cout << "I'm sorry, but we were unable to crack your password.\n";
			cout << "This may simply mean there is no password set.\n";//notify the user of the possible reason for the failure
		}
		
		delete[] licenseFileContents;//free up the memory
	}
	else//else the license file could not be loaded
	{
		cout << "[Failure]\n";//print failure
		cout << "\n\n";
		cout << "I'm sorry, we were unable to locate your license file.\n";
		cout << "Check to see if K9 Web Protection is installed. If you would like to crack the password of a K9 Web Protection license file without installing K9 Web Protection, simply place the license file in the same directory as this program, and try again.\n";//attempt to give the user some help with the reason for failure
	}

	//add new lines
	cout << "\n";
	cout << "\n";

	cout << "Thank you for using this software!\n";
	cout << "Press any key to quit...";//notify the user of what they have to do to quit the program
	
	//wait for the user to press any key
	int ch = _getch();//capture user input
	
	return 0;//return no errors and end the program
}





//load the license file
bool loadLicenseFile(string * licenseLocation)
{
	//open license file to the end of the file
	ifstream licenseFile;
	
	//attempt to open license file from program directory
	licenseFile.open("license", ios::in|ios::binary|ios::ate);

	//check to see if the license file opened successfully
	if(!licenseFile.is_open())//if the file did not open successfully
	{
		//license file did not open

		//close the license file
		licenseFile.close();

		//attempt to open license file from default installation location
		licenseFile.open("C:\\Program Files\\Blue Coat K9 Web Protection\\license", ios::in|ios::binary|ios::ate);
	
		//check to see if the license file opened successfully
		if(!licenseFile.is_open())
		{
			//license file did not open
			
			//close the license file
			licenseFile.close();

			//the license file was not found in any of the locations
			return false;//return failure to open up the license file
		}
		else
		{
			//file loaded from the default installation location

			*licenseLocation = "[Default Installation Location] C:\\Program Files\\Blue Coat K9 Web Protection\\license";//pass the location from which the license was loaded
		}
	}
	else//if the file did open successfully
	{
		//file loaded from program directory

		*licenseLocation = "[Program Directory] .\\";//pass the location from which the license was loaded
	}

	size = licenseFile.tellg();//we are at the end of the file so read the size
	licenseFileContents = new unsigned char[size];//allocate the amount of space needed for the file in memory
    licenseFile.seekg (0, ios::beg);//move to the beginning of the file
    
	//read license file and check to see if this task succeeds
	if(!licenseFile.read((char *)licenseFileContents, size))
	{
		//close the license file
		licenseFile.close();

		//the read failed
		return false;
	}

	//close the license file
	licenseFile.close();

	//return true because the operation succeeded
	return true;
}






//prints the license file in hex
void printLicenseFileContents()
{
	cout << "\n= License File Contents =========================\n";//print header

	//move through the character array until we reach the size of the license file
	for(int i = 0 ; i < size ;)
	{
		cout << " ";//print a space before the rows

		//print in rows of 16 separated by spaces
		for(int j = 0 ; j < 16 ; j++, i++)
		{
			cout << hex << setw(2) << setfill('0') << (unsigned int)licenseFileContents[i];//print each character at 2 digits and fill empty space with 0's
			cout << " ";//print a space between characters
		}
		cout << "\n";//move to the next line
	}

	cout << "=================================================\n\n";//print footer

	//restore cout flags to default
	cout.flags(COUTFLAGDEFAULTS);
}





//get the offset char
char getOffsetValue()
{
	//return the character at the location of the offset value in the array
	return licenseFileContents[OFFSETLOC];
}





//get the hash
void getHash(unsigned char * passwordHashStor)
{
	//loop until we reach the PASSWORDMAXLENGTH
	for(int i = 0 ; i < PASSWORDMAXLENGTH ; i++)
	{
		//move through the passed array for storing the password hash in
		//and copy the values of the password hash from the
		//license file contents array to it
		passwordHashStor[i] = licenseFileContents[PASSWORDHASHLOC+i];
	}
}












//reverse the hash aka crack the password
bool reverseHash(unsigned char * thePsswdHash, unsigned char theOffStVal, string * theCrkdPsswd)
{
	//if there is no password for some reason
	if(((int)thePsswdHash[0]) == 0)
	{
		return false;//return false
	}

	//move through the password hash until we reach the PASSWORDMAXLENGTH
	for(int i = 0 ; i < PASSWORDMAXLENGTH ; i++)
	{
		if(((int)(thePsswdHash[i] - theOffStVal)) < 32 || ((int)(thePsswdHash[i] - theOffStVal)) > 126)//if an invalid character is reached (ascii decimal)
		{
			break;//break out of the loop
		}
		else
		{
			//subtract the offset value from the current character in the hash
			//and add it to the string for holding the cracked password
			*theCrkdPsswd += (thePsswdHash[i] - theOffStVal);
		}
	}

	//reversal of the hash was a success
	return true;
}









//get the length of the hash
int getHashLength()
{
	//variable that will hold the length of the hash
	int lengthOfHash = 0;

	//move through the 4 characters that are used to determine the password length
	for(int i = 0 ; i < 4 ; i++)
	{
		//calculate the password length
		lengthOfHash += ((unsigned int)licenseFileContents[HASHLENGTHLOC+i])*(pow(256,i));
	}
	
	//calculate the password length final step
	lengthOfHash /= (unsigned int)licenseFileContents[OFFSETLOC];

	//return the length of the password/hash
	return lengthOfHash;
}
