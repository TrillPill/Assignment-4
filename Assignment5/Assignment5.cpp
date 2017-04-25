// Assignment5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

//MYDIVES SUBCLASS
class FavoriteDives {
	string location;
	string diveDate;
	double maxDepth;
	string timeSubmerged;
public:
	FavoriteDives();
	FavoriteDives(string loc, string date, double depth, string timeSub);
	void CaptureMyDive();
	void DisplayMyDive();
	int SaveMyDive(ofstream &outfile);
};

//FavoriteDives Default Constructor
FavoriteDives::FavoriteDives() {
	location = "";
	diveDate = "";
	maxDepth = 0.0;
	timeSubmerged = "";
}

//FavoriteDives Overloaded Constructor
FavoriteDives::FavoriteDives(string loc, string date, double depth, string timeSub) {
	location = loc;
	diveDate = date;
	maxDepth = depth;
	timeSubmerged = timeSub;
}

//FavoriteDives Methods
void FavoriteDives::CaptureMyDive() {
	cout << "Location? -->";
	getline(cin, location);
	cout << "Date of dive?  -->";
	getline(cin, diveDate);
	cout << "Max depth of dive? -->";
	cin >> maxDepth;
	cout << "Time submerged? -->";
	cin >> timeSubmerged;
	cin.ignore();
	cin.clear();
}

//Favorite Dives Display
void FavoriteDives::DisplayMyDive() {
	cout << "Dive Location : " << location << " Dive Date : " << diveDate << " Max Depth : " << maxDepth << " Time Submerged : " << timeSubmerged << endl;
}

//FavoriteDives Save
int FavoriteDives::SaveMyDive(ofstream &outfile) {
	if (outfile.is_open())
	{
		outfile << location << "|" << diveDate << "|" << maxDepth << "|" << timeSubmerged << endl;
		return 0;
	}
	else
		return WRITEERROR;
}


//Favorite Dives List
class MyFavDivesList{
	FavoriteDives **DiveList;
	int numRecords;
	int listSize;
	int ReallocateArray();
public:
	MyFavDivesList();
	~MyFavDivesList();
	void GetUserInput();
	void DisplayFavDivesList();
	int SaveFavDivesList(string filename);
	int ReadFavDivesList(string filename);
};

//MyFavThingsList default constructor
MyFavDivesList :: MyFavDivesList() {
	DiveList = new FavoriteDives*[ARRAYSIZE];
	numRecords = 0;
	listSize = ARRAYSIZE;
}

//MyFavThingsList destructor
MyFavDivesList :: ~MyFavDivesList() {
	for (int i = 0; i < numRecords; i++)
	{
		delete DiveList[i];
	}
	delete[]DiveList;
}

//LIST METHODS
int MyFavDivesList::ReallocateArray() {
	FavoriteDives **temp;
	temp = new FavoriteDives*[listSize + ARRAYSIZE];
	listSize = listSize + ARRAYSIZE;
	for (int i = 0; i < numRecords; i++)
	{
		temp[i] = DiveList[i];
	}
	delete[]DiveList;
	DiveList = temp;
	return 0;
}

void MyFavDivesList::GetUserInput() {
	string answer = "Y";
	//
	cout << "Enter Item? Y/N: " << endl;
	getline(cin, answer);
	while ((answer == "y") || (answer == "Y")) {
		DiveList[numRecords] = new FavoriteDives();
		DiveList[numRecords]->CaptureMyDive();
		numRecords++;
		cout << "Enter another item? Y/N: " << endl;
		getline(cin, answer);
	}
}

void MyFavDivesList::DisplayFavDivesList() {
	for (int i = 0; i < numRecords; i++)
		DiveList[i]->DisplayMyDive();
}

int MyFavDivesList::SaveFavDivesList(string filename) {
	ofstream output(filename, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numRecords; i++)
		{
			DiveList[i]->SaveMyDive(output);
		}
		output.close();
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}

int MyFavDivesList::ReadFavDivesList(string filename) {
	string loc, dDate, depth, timeBelow;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}


	while (!infile.eof())
	{
		if (numRecords == listSize)
		{
			ReallocateArray();
		}
		getline(infile, loc, '|');
		if (!loc.empty())
		{
			getline(infile, dDate, '|');
			getline(infile, depth, '|');
			double dDepth = stod(depth);
			getline(infile, timeBelow);
			DiveList[numRecords] = new FavoriteDives(loc, dDate, dDepth, timeBelow);
			numRecords++;
		}
	}

	infile.close();
	return 0;
}


int main()
{
	string filename = "C:\\Users\MrMShearer\Documents\blah.txt";
	cout << "What is the full path of the file you would like to use? " << endl;
	MyFavDivesList diveList;
	string answer;
	int error;
	error = diveList.ReadFavDivesList(filename);
	if (error)
	{
		cout << "Cannot read favorites - continue creating new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}
	diveList.GetUserInput();
	diveList.SaveFavDivesList(filename);
	diveList.DisplayFavDivesList();

	return 0;

}