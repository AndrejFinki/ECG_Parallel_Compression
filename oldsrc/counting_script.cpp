#include <bits/stdc++.h>
using namespace std;
#define INF (1<<30)
#define LL long long
// #define endl '\n'

/* How many bits should each number be when we're not doing compression. Since ECG values can go up to 1023 and are only positive, 10 bits are needed */
const int BITS_USED_UNCOMPRESSED = 10;

/* What is the assumed starting value for the first ECG value */
const int STARTING_VALUE = 511;

/* Debug flag for debug information */
const bool DEBUG = false;

/* Name of the temporary file that lists all of the datafiles to run the compression on */
const string LS_FILE = "test_cases.tmp";

/* The relative path for the Data directory */
const string DATA_DIR = "../Data/EKG_360";

/* The range of bits to test */
const int LEFT_RANGE = 2, RIGHT_RANGE = 7;

/* What data to output to result */
const bool OUTPUT_VALUES = true;
const bool OUTPUT_TIME = false;

/* Helper functions. */
void program_exit(string);
void open_file( fstream &, const string );
string to_binary_string(int, const int);

/* Driver for a specific test file */
void driver(string);

/* Read the data and compress it, returning the amount of bits it takes */
int count_difference(string);

int total_count = 0;

int main(){ ios::sync_with_stdio(false);

    /* Extract test cases filepaths */
    string cmd = "ls " + DATA_DIR + " > " + LS_FILE;
    system( cmd.c_str() );

    fstream file;
    open_file( file, LS_FILE );

	cout << "Counting for " << DATA_DIR << endl;

    string test;
    while( getline( file, test ) ) driver(test);

	cout << "Sum," << total_count << endl;

}

void driver( string test_path ){

	int count = count_difference( test_path );
	cout << test_path << "," << count << endl;
	total_count += count;

}

int count_difference( string test_path ){

    fstream test_in;
    test_in.open( DATA_DIR + "/" + test_path );
    if( !test_in ) program_exit( "Could not access test " + test_path );

	int rv = 0;	
	int last_val, current_val; test_in >> last_val;
	while( test_in >> current_val ){
		if( abs( current_val - last_val ) <= 1 ) rv++;
		last_val = current_val;
	}

    test_in.close();

	return rv;

}

string to_binary_string( int x, const int cmp ){
    string rv = "";
    for( int i = 0 ; i < cmp ; i++ ){
        rv += (x%2) + '0';
        x /= 2;
    } reverse( rv.begin(), rv.end() );
    return rv;
}

void program_exit( string msg = "" ){
    if( msg != "" ) cerr << msg << endl;
    exit(0);
}

void open_file( fstream &file, const string file_path ){
    file.open( file_path );
    if( !file ) program_exit( "Could not open file at path: " + file_path );
}