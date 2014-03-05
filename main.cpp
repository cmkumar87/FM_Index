#include "iostream"
#include "vector"

//include local libraries
#include "bwt.h"
#include "bitarray/bitarray.h"
#include "wavarray/wat_array.h"
#include "bitarray/rrr.h"
#include "bitarray/rrr2.h"
#include "intarray/sdarray_sml.h"

using namespace mscds;

typedef WatQueryGen<RRR> WavQuery2;
typedef WatBuilderGen<RRR> WavBuilder2;


typedef WatQueryGen<RRR2> WavQuery4;
typedef WatBuilderGen<RRR2> WavBuilder4;

typedef WatQueryGen<SDRankSelectSml> WavQuery3;
typedef WatBuilderGen<SDRankSelectSml> WavBuilder3;

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
	string input = "abracadabra";
	vector<uint64_t> input_as_vector = {123};
	WavBuilder2 rrr;
	WavQuery2 rrrq;
	rrr.build(input_as_vector, &rrrq);

	uint64_t output = rrrq.rank(6,8);
	cout<< "Wavoutput "<< output <<endl;
/*
	BWT_Query bq;
	BWT_Builder bd;
	bd.build(input, &bq);
	bq.make_count_table(bd.appended, bd.suffix_indices);

	cout << "Example Occ calls"<< endl;
	cout << "------------------------"<< endl;
	cout << "Occ(a,1,2): " << bq.Occ('a',2)<< endl;
	cout << "Occ(b,1,3): " << bq.Occ('b',3)<< endl;
	cout << "Occ(r,1,4): " << bq.Occ('r',4)<< endl;
	cout << "Occ(r,1,1): " << bq.Occ('r',1)<< endl;
	cout << "Occ(d,1,5): " << bq.Occ('d',5)<< endl;
	
	
	cout<< "Example 1" << endl <<"--------------"<< endl;
	string pattern = "abra";
	cout <<"Pattern to count:"<< pattern << endl;
	int pattern_count = bq.count(pattern);
	cout << "Found " << pattern_count << " occurances" << endl;
	
	bq.locate(pattern, bd.suffix_indices);

	cout<< "Example 2" << endl <<"--------------"<< endl;
        pattern = "cad";
        cout <<"Pattern to count:"<< pattern << endl;
        pattern_count = bq.count(pattern);
        cout << "Found " << pattern_count << " occurances" << endl;

	bq.locate(pattern, bd.suffix_indices);


	cout<< "Example 3" << endl <<"--------------"<< endl;
        pattern = "abar";
        cout <<"Pattern to count:"<< pattern << endl;
        pattern_count = bq.count(pattern);
        cout << "Found " << pattern_count << " occurances" << endl;

	bq.locate(pattern, bd.suffix_indices);
*/
}
