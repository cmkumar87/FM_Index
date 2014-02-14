#include "iostream"
#include "vector"
#include "algorithm"

//include headers from local libraries

#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"
#include "bwt.h"
#include "framework/archive.h"

using namespace std;


/* Function to write the ouput object to disk
void BWT_Builder::build(const std::string& input, OutArchive& out){

}
*/


void BWT_Builder::build(const std::string& input, BWT_Query *out){

	vector<string> bwt;

	cout << "Input string: " << input << endl;

	//Append end of string char
	string appended = input + "$";
	cout << "Appended with $: " << appended << endl;
	//count << "Original length:" << input.length();
	//count << "Appended string length:" << appended.length();

	vector<BWT_Builder::suffix_pair> suffixes = suffix(appended);

	
	//sort the suffixes using custom sort predicate
	sort(suffixes.begin(),suffixes.end(),
		[](const pair<int,string> &left, const pair<int,string> &right)->bool{
			return left.second < right.second;
		});		

	
	cout << "Sorted Suffixes:" <<endl;
	printVector(suffixes);

	for (int index = 0; index < appended.length(); ++index){
		suffix_pair p = suffixes[index];
		if( p.first == 0 )
			cout << "$" << endl;
		else
			cout<< appended[p.first - 1]<< endl;

		 	//bwt.push_back(appended[p.first - 1]);
	}

}


/*
  Finds all the suffixes for the string.
  Returns suffixes with their indices.
*/
vector<BWT_Builder::suffix_pair> 
	BWT_Builder::suffix (const string& input){

	vector<BWT_Builder::suffix_pair> suffixes;

	for (unsigned int index = 0; index < input.length(); index++){
	   suffixes.push_back(suffix_pair(index,input.substr(index)));
	}

	printVector(suffixes);

	return suffixes;
}

/*
bool BWT_Builder::sort_suffixes( suffix_pair& left, 
				  suffix_pair& right){
	return left.second < right.second;
}
*/

void BWT_Builder::printVector(vector<BWT_Builder::suffix_pair>& input){
       for (vector<suffix_pair>::iterator iter = input.begin();
                                        iter != input.end(); ++iter){
            int index = (*iter).first; 
	    cout << index << ":";  

	    string suffix =(*iter).second;
	    cout << suffix << endl;
        }  

}

// count the number of times the pattern occur in the input
unsigned int BWT_Query::count(const std::string& pattern){
}

// return the location of any occurrence of the pattern in the input
unsigned int BWT_Query::first_occ(const std::string& pattern){

}


/*
	BitArray b = BitArrayBuilder::create(4);
	b.setbit(0, true);

	
	vector<uint64_t> v(10,5);
	cout << "Created vector of size:" << v.size() << endl;

	for (long index = 0; index < 3; index ++){
		cout << "Vector element " << index << v.at(index) << endl;
	}
*/

