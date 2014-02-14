#include "iostream"
#include "vector"

//include local libraries
#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"
#include "bwt.h"

using namespace std;

int main(){
	using namespace mscds;
/*
	BitArray b = BitArrayBuilder::create(4);
	b.setbit(0, true);

	
	vector<uint64_t> v(10,5);
	cout << "Created vector of size:" << v.size() << endl;

	for (long index = 0; index < 3; index ++){
		cout << "Vector element " << index << v.at(index) << endl;
	}
*/
	// Stub input. Will read input from Bed file later
	//string input ="mississippi";
	string input = "abaaba";

	BWT_Query bq;
	BWT_Builder bd;
	bd.build(input, &bq);
}
