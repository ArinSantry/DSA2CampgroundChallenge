#include <fstream>
#include <iostream>
using namespace std;

// struct to store stone value and weight
struct stone
{
	int value;
	int weight;
};


class Stones
{
private:
	int n; // total number of stones
	int capacity; // maximum weight capacity of the wheelbarrow
	int numStonesSelected = 0; // number of stones selected to be put in the wheelbarrow
	stone* array; // array of all stones
	stone* wheelbarrow; // array of stones in the wheelbarrow

public:
	// constructor to initialize the total number of stones and the stone array
	Stones(int size)
	{
		n = size;
		array = new stone[n];
	}
	
	// constructor to read in text from the input file
	Stones(string inputFile)
	{
		ifstream inFile;
		inFile.open(inputFile, ios::in);
		
		if (inFile.is_open())
		{
			int size;
			// first two ints of the file are the size and capacity
			inFile >> size;
			inFile >> capacity;
			if (size > 0)
			{
				// initialize size and array of stones
				n = size;
				array = new stone[n];
				// initialize wheelbarrow size to its maximum
				wheelbarrow = new stone[n];
				
				int weight, val;
				int stoneNum = 0; // keep track of the amount of stones	added			
				while (inFile >> weight >> val)
				{
					cout << "Adding stone of weight " << weight << " and value " << val << endl;
					addStone(weight, val, stoneNum);
					stoneNum++;
				}
				
				inFile.close();
			}
			else
			{
				cerr << "Invalid number of stones" << endl;
			}
		}
		else
		{
			cerr << "Cannot open file" << endl;
		}
	}

	void addStone(int weight, int val, int num);
	void displayStones();
	void findBestStones();
	void addToWheelbarrow(int weight, int val, int num);
	void displayWheelbarrow();
};

// add a stone to the array of total stones
void Stones::addStone(int weight, int val, int num)
{
	stone newStone;
	newStone.weight = weight;
	newStone.value = val;
	array[num + 1] = newStone; // there is no 0th stone (trips up the best stone algorithm)
}

// add a stone to the wheelbarrow
void Stones::addToWheelbarrow(int weight, int val, int num) 
{
	stone newStone;
	newStone.weight = weight;
	newStone.value = val;
	wheelbarrow[num] = newStone;
	numStonesSelected++;
}

// find the best stones to put in the wheelbarrow to maximuze value
void Stones::findBestStones()
{
	// d is a 2D array to keep track of previous answers (dynamic programming)
	// pointers because we don't yet know how big it needs to be until the file is read
	int** d;
	d = new int*[n];
	for (int i = 0; i < n; ++i)
		d[i] = new int[capacity];

	//initialize the first row and column to be filled with 0s
	for (int i = 0; i < capacity; i++)
	{
		d[0][i] = 0;
	}
	for (int j = 1; j < n; j++)
	{
		d[j][0] = 0;
	}

	// find the best value for the first stone at each weight, the second, and so on
	for (int k = 1; k < n; k++)
	{
		for (int w = 0; w < capacity + 1; w++)
		{
			if (array[k].weight <= w)
			{
				if (array[k].value + d[k - 1][w - array[k].weight] > d[k - 1][ w])
				{
					// if it fits and has a better value than the previous, add it
					d[k][w] = array[k].value + d[k - 1][w - array[k].weight];
				}
				else
				{
					d[k][w] = d[k - 1][w];
				}
			}
			else
			{
				d[k][w] = d[k - 1][w];
			}
		}
	}

	// find which stones are to be added
	int i = n - 1;
	int k = capacity - 1;
	while (i > 0 && k > 0)
	{
		if (d[i][k] != d[i - 1][k])
		{
			addToWheelbarrow(array[i].weight, array[i].value, i);
			i = i - 1;
			k = k - array[i].weight;
		}
		else
			i = i - 1;
	}
}

// display all stones
void Stones::displayStones()
{
	for (int i = 1; i < n + 1; i++)
	{
		cout << "Stone " << i << ": Weight = " << array[i].weight << ", Value = " << array[i].value << endl;
	}
}

// display stones in the wheelbarrow
void Stones::displayWheelbarrow()
{
	for (int i = 1; i < numStonesSelected + 1; i++)
	{
		cout << "Stone " << i << ": Weight = " << wheelbarrow[i].weight << ", Value = " << wheelbarrow[i].value << endl;
	}
}

int main()
{
	Stones myStones("input.txt");
	cout << endl << "All stones: " << endl;
	myStones.displayStones();
	cout << endl << "Selected stones: " << endl;
	myStones.findBestStones();
	myStones.displayWheelbarrow();

	// so it doesn't close the window right away
	cout << "\nPress enter to continue";
	cin.ignore();
	return 0;
}