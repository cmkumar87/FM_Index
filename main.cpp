#include "iostream"
#include "fstream"
#include  "time.h"
#include "vector"
#include "mem/info_archive.h"

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

int main(int argc, char *argv[]){

	if(argc < 5){
		std::cerr << "Error: Not enough arguments" << endl;	
		std::cerr << "Usage: ./bwt -f <sequence_file> -p <pattern_file>[debug][perf]" << endl;
		return 1;
	}

	bool debug = false, perf = false;
        std::string seqFile,patFile;

	for(unsigned int i= 1; i< argc; i++){
		  if(strcmp(argv[i],"-f") == 0 && (i+1) != argc){
			seqFile = argv[i+1];
			i++;
		  }
		  else if (strcmp(argv[i],"-p") == 0 && (i+1) != argc){
			patFile = argv[i+1];
			i++;
		  }
		  else if (strcmp(argv[i],"-d") == 0){
			cout << debug << endl;
			debug = true;
		  }
		  else if (strcmp(argv[i],"-perf") == 0){
			perf = true;
		  }
		  else{
		   	cout << "Usage: ./bwt -f <sequence_file> -p <pattern_file> [debug][perf]" << endl;
	           	exit(0);
	  	  }		  
	}

	using namespace mscds;

	clock_t t1,t2,t3,t4,t5,t6,t7,t8;

	std::string inputSeq, pattern;
	
	// Read input file containing gene sequence
	std::ifstream inputFile(seqFile,std::ios::in);
	if(inputFile){
		
		inputFile.seekg(0, std::ios::end);
		inputSeq.resize(inputFile.tellg());

		inputFile.seekg(0,std::ios::beg);
		inputFile.read(&inputSeq[0], inputSeq.size());
		inputSeq.erase( std::remove(inputSeq.begin(), inputSeq.end(), '\n'), inputSeq.end() );
		inputFile.close();
	}


	std::ifstream inputFile2(patFile,std::ios::in);
        if(inputFile2){

                inputFile2.seekg(0, std::ios::end);
                pattern.resize(inputFile2.tellg());

                inputFile2.seekg(0,std::ios::beg);
                inputFile2.read(&pattern[0], pattern.size());
                pattern.erase( std::remove(pattern.begin(), pattern.end(), '\n'), pattern.end() );
                inputFile2.close();
        }

	cout << "InputFiles read: " << seqFile << " | " << patFile << endl;
	//cout << "Input Strings read: " << inputSeq << endl << pattern << endl;

	/*
	vector<uint64_t> input_as_vector;
	

	if(inputFile2){
	    uint64_t value;
		input_as_vector.push_back(value);

	WavBuilder4 rrr;
	WavQuery4 rrrq;

	t1= clock();
	rrr.build(input_as_vector, &rrrq);
	t2 = clock();

	uint64_t output = rrrq.rank(6,8);
	cout<< "Wavoutput "<< output <<endl;
	t3 = clock();

	float diff1 ((float)t2 - (float)t1);
	float diff1_sec = diff1/CLOCKS_PER_SEC;

	float diff2 ((float)t3 - (float)t2);
	float diff2_sec = diff1/CLOCKS_PER_SEC;

	cout << "diffs: " << diff1_sec <<"  " << diff2_sec << endl;
	*/

	BWT_Query bq;
	BWT_Builder bd;

t1 = clock();
	bd.build(inputSeq, &bq, debug);
	bq.make_count_table(bd.appended, bd.suffix_indices, debug);
t2 = clock();

float diff1 ((float)t2 - (float)t1);
float diff1_sec = diff1/CLOCKS_PER_SEC;

	//size_t = estimate_data_size();

	if (debug){
		cout << "Example Occ calls"<< endl;
		cout << "------------------------"<< endl;
		cout << "Occ(a,1,2): " << bq.Occ('a',2)<< endl;
		cout << "Occ(b,1,3): " << bq.Occ('b',3)<< endl;
		cout << "Occ(r,1,4): " << bq.Occ('r',4)<< endl;
		cout << "Occ(r,1,1): " << bq.Occ('r',1)<< endl;
		cout << "Occ(d,1,5): " << bq.Occ('d',5)<< endl;
	}

//CONFIG
uint64_t loops = 1;

	int pattern_count = 0;
t3 = clock();	
	cout <<"Pattern to count:"<< pattern << endl;
	for(int count = 1; count <= loops; count++){
		pattern_count = bq.count(pattern,debug);
		//cout << "Found " << pattern_count << " occurances" << endl;
	}
t4 = clock();

float diff2 ((float)t4 - (float)t3); 
float diff2_sec = diff2/CLOCKS_PER_SEC;

cout << loops << " times count " << diff2_sec << endl;
cout << (diff2_sec/loops) << endl;


vector <uint64_t> hits;
t5 = clock();	
        for(int count = 1; count <= loops; count++){
		hits = bq.locate(pattern, bd.suffix_indices, debug);
	}
t6 = clock();

	if (debug){
		for(vector<uint64_t>::iterator it = hits.begin();  it != hits.end(); it++)
			cout<< "hit: " << *it << endl;
	}

float diff3 ((float)t6 - (float)t5);
float diff3_sec = diff3/CLOCKS_PER_SEC;

cout << loops << " times count " << diff3_sec << endl;
cout << (diff3_sec/loops) << endl;

t7 = clock();
        for(int count = 1; count <= loops; count++){
		hits = bq.locate2(pattern, bd.suffix_indices, debug);
	}
t8 = clock();

	if(debug){  
	      for(vector<uint64_t>::iterator it = hits.begin();  it != hits.end(); it++)
                cout<< "hit: " << *it << endl;
	}

float diff4 ((float)t8 - (float)t7);
float diff4_sec = diff4/CLOCKS_PER_SEC;

cout << loops << " times count " << diff4_sec << endl;
cout << (diff4_sec/loops) << endl;

cout << "Build time: " << diff1_sec << endl;
cout << "Query time Count: " << diff2_sec/loops << endl;
cout << "Query time locate: " << diff3_sec/loops  << endl;
cout << "Query time locate2: " << diff4_sec/loops << endl;

}
