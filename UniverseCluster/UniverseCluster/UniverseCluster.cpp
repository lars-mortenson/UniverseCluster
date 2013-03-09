// UniverseCluster.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "UniverseCluster.h"

/*
int _tmain(int argc, _TCHAR* argv[])
{
	ifstream file;

	string dataFilePath = "C:\\Users\\Fish\\Documents\\GitHub\\UniverseCluster\\data\\lars_full_double.csv";
	file.open(dataFilePath);
	string respondentDataRow;
	respondentGrid grid;
	respondentListMap problemMap;

	while (getline(file, respondentDataRow))
	{
		stringstream respondentDataRowStream(respondentDataRow);
		vector<bit_mask> row;
		long respondentDatum;

		int index = 0;
		int problemCount = 0;
		string binaryDataString = "";
		Respondent respondent;

		while (respondentDataRowStream >> respondentDatum)
		{
			index++;
			if (index == 1) //then this is the enty_id
			{
				respondent.enty_id = respondentDatum;
			}
			else //this is binary, comma delimited data
			{
				if (respondentDatum == 0)
				{
					binaryDataString += "0";
				}
				else if (respondentDatum == 1)
				{
					binaryDataString += "1";
					problemCount++;
				}
			}
			row.push_back(respondentDatum);

			if (respondentDataRowStream.peek() == ',')
				respondentDataRowStream.ignore();
		}
		respondent.mask = _strtoi64(binaryDataString.c_str(), NULL, 2);
		respondent.strMask = binaryDataString;
		respondent.problemCount = problemCount;
		vector<Respondent>* problemCountRespondentList;
		if (problemMap.count(problemCount) == 0)
		{
			vector<Respondent>* emptyRespondentList = new vector<Respondent>();
			problemCountRespondentList = emptyRespondentList;
			problemMap.insert(pair<int,vector<Respondent>*>(problemCount,emptyRespondentList));
		}
		else
		{
			problemCountRespondentList = problemMap.find(problemCount)->second;
		}
		problemCountRespondentList->push_back(respondent);
		grid.push_back(row);
	}
	for (int x = 0; x < 20; x++) 
	{
		if (problemMap.count(x) != 0)
		{
			cout << "Has " << x << " problems: " << problemMap.find(x)->second->size() <<endl;
		}
	}

	int64 time1 = GetTimeMs64();
	testIterator();
	int64 time2 = GetTimeMs64();
	cout << "Time: " <<  time2 - time1 << endl;

	time1 = GetTimeMs64();
	cout << calcBitCombosFact(10, 40) << endl;
	time2 = GetTimeMs64();
	cout << "Time: " <<  time2 - time1 << endl;

	system ("pause");
	return 0;
}
*/
void testIterator()
{
	// calculate total 
	cout << calcBitCombosFact(10,40) << endl;
	for (unsigned int i = 1; i < 100; i++)
	{
		string position = formatMask(getNthSet(i, 10, 40), 40);
		cout << position << endl;
	}
	//solveProblems(10, grid, problemMap);
}

/*
const unsigned long long calcBitCombos(unsigned int number, unsigned int breadth)
{
	if (number > 2)
	{
		unsigned long long total = 0;
		for (unsigned int i = 0; i <= breadth - number; i++)
		{
			total += calcBitCombos(number - 1, breadth - (i+1));
		}
		return total;
	}
	if (number == 2)
	{
		return ((breadth - 1) * breadth) / 2;
	}
	if (number == 1)
	{
		return breadth;
	}
	if (number == 0)
	{
		return 0;
	}
}
*/

string formatMask(bit_mask mask, int numberOfProblems) 
{
	bitset<maxBits> maskSet(mask);
	string tempMask = maskSet.to_string();
	return tempMask.substr(tempMask.size() - numberOfProblems, numberOfProblems);
}
/*
bool compareWithMask(bit_mask solution, bit_mask respondentMask)
{
	//totalComparisons++;
	bit_mask intermediateResult = solution ^ respondentMask;
	bit_mask finalResult = intermediateResult & respondentMask;
	return finalResult == 0;
}

int countMatchingRespondents(bit_mask mask, vector<Respondent> const & eligibleRespondents) 
{
	int respondentCount = 0;
	for (unsigned int i = 0; i < eligibleRespondents.size(); i++) {
		if(compareWithMask(mask, eligibleRespondents[i].mask)) {
			respondentCount++;
		}
	}
	return respondentCount;
}
*/
void incrementMask(bit_mask &currentMask)
{

	bit_mask t = currentMask | (currentMask - 1); // t gets v's least significant 0 bits set to 1
	cout << t << endl;
	// Next set to 1 the most significant bit to change, 
	// set to 0 the least significant ones, and add the necessary 1 bits.
	unsigned long l;
	_BitScanForward64(&l,currentMask);
	cout << l << endl;
	cout << ((~t & -~t) - 1) << endl;
	currentMask = ((t + 1) | ((~t & -~t) - 1) >> (l + 1));
}

unsigned long long factorial(unsigned int number)
{
	if (number == 0 || number == 1)
		return 1;

	unsigned long long total = 1;

	for(unsigned int i = number; i > 1; i--)
	{
		total *= i;
	}

	return total;
}

unsigned long long calcBitCombosFact(unsigned int numPicked, unsigned int numElements)
{
	unsigned long long total = 1;
	for (unsigned int i = numElements; i > (numElements - numPicked); i--)
	{
		total *= i;
	}

	return total / factorial(numPicked);
}

void accumulatePositions(unsigned long long n, unsigned int numPicked, unsigned int numElements, vector<unsigned int>& positions)
{
	unsigned int testPicked = numPicked;
	unsigned int testElements = numPicked;
	unsigned long long totalCombinations = calcBitCombosFact(testPicked, testElements);
	while (n > totalCombinations)
	{
		testElements++;
		totalCombinations = calcBitCombosFact(testPicked, testElements);
	}
	if (n != totalCombinations)
	{
		unsigned long long newN = n - calcBitCombosFact(testPicked, testElements - 1);
		positions.push_back(testElements);
		testPicked--;
		testElements--;
		accumulatePositions(newN, testPicked, testElements, positions);
	}
	else
	{
		while(testPicked > 0)
		{
			positions.push_back(testElements);
			testElements--;
			testPicked--;
		}
	}
}

unsigned long long getNthSet(unsigned long long n, unsigned int numPicked, unsigned int numElements)
{
	if (n == 0)
	{
		return 0;
	}
	vector<unsigned int> positions;
	accumulatePositions(n, numPicked, numElements, positions);

	unsigned long long result = 0;
	for (unsigned int i =0; i < positions.size(); i++) {
		result |= (1LL << positions[i] - 1);
	}

	return result;
}





/*
		function calcBitCombos(number, breadth, recurseBreadth)
		{
			if (number > recurseBreadth)
			{
				var total = 0;
				for (var i = 0; i <= (breadth - number); i++)
				{
					total += calcBitCombos(number - 1, breadth - (i + 1), recurseBreadth);
				}
				return total;
			}

			if (number == 4)
			{
				return ((-6 * breadth) + (11 * Math.pow(breadth,2)) - (6 * Math.pow(breadth,3)) + (Math.pow(breadth,4))) / 24;
			}
			if (number == 3)
			{
				return ((2 * breadth) - (3 * Math.pow(breadth,2)) + (Math.pow(breadth,3))) / 6;
			}
			if (number == 2)
			{
				return (-(3 * breadth) + 3*Math.pow(breadth,2)) / 6
			}
			if (number == 1)
			{
				return breadth;
			}
			if (number == 0)
			{
				return 0;
			}
		}
*/

void solveProblems(const int maxDepth, const respondentGrid& grid, const respondentListMap& problemMap)
{
	vector<bit_mask> masks;
	vector<Respondent> eligibleRespondents;
	int numberOfProblems = grid[0].size() - 1;

	//generate all the masks we'll be using
	for (int x = 0; x < numberOfProblems; x++) {
		masks.push_back(powl(2.0F,x));
	}
	masks.push_back(0);

	//find all eligible respondents
	for (int x = 1; x <= maxDepth; x++)
	{
		if (problemMap.count(x) > 0)
		{
			eligibleRespondents.insert(eligibleRespondents.end(), problemMap.find(x)->second->begin(), problemMap.find(x)->second->end());
		}
	}

	bit_mask currentMask = 0;
	//calculate the 'max mask' so we know when we're done looping
	bit_mask maxMask = 0;
	for (int i = 1; i <= maxDepth; i++)
	{
		maxMask = maxMask ^ bit_mask(pow(2.0F,(numberOfProblems - i)));
	}
	incrementMask(maxMask);

	int position = 0;
	int bestCount = 1;
	vector<bit_mask> maskArray;
	vector<long> positionArray;
	int currentDepth = maxDepth;

	for (int i = 0; i < maxDepth; i++)
	{
		positionArray.push_back(i);
	}

	bool done = false;
			
	currentMask = powl(2.0F,maxDepth) - 1;
	cout << formatMask(maxMask, numberOfProblems) << endl;
//	cout << "count" << countMatchingRespondents(maxMask, eligibleRespondents) << endl;

	while(currentMask != maxMask)
	{
		int count;
		//int count = countMatchingRespondents(currentMask, eligibleRespondents);
		if (count > bestCount) 
		{
			bestCount = count;
			cout << "Best set so far: " << formatMask(currentMask, numberOfProblems) << " with " << bestCount << endl;
			//listMatchingRespondents(currentMask, eligibleRespondents);
		}
		
		bit_mask t = currentMask | (currentMask - 1); // t gets v's least significant 0 bits set to 1
		// Next set to 1 the most significant bit to change, 
		// set to 0 the least significant ones, and add the necessary 1 bits.
		unsigned long l;
		_BitScanForward64(&l,currentMask);
		currentMask = ((t + 1) | ((~t & -~t) - 1) >> (l + 1));  
	}	
}

int64 GetTimeMs64()
{
#ifdef WIN32
 /* Windows */
 FILETIME ft;
 LARGE_INTEGER li;

 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
  * to a LARGE_INTEGER structure. */
 GetSystemTimeAsFileTime(&ft);
 li.LowPart = ft.dwLowDateTime;
 li.HighPart = ft.dwHighDateTime;

 uint64 ret = li.QuadPart;
 ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

 return ret;
#else
 /* Linux */
 struct timeval tv;

 gettimeofday(&tv, NULL);

 uint64 ret = tv.tv_usec;
 /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
 ret /= 1000;

 /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
 ret += (tv.tv_sec * 1000);

 return ret;
#endif
}
