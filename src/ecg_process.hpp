#ifndef ECG_PROCESS_H
#define ECG_PROCESS_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "compression.hpp"

class ECG_Process {
public:
    virtual vector <int> main_process( const vector <int> *, const int & ) = 0;
    virtual void secondary_process() = 0;
};

class ECG_Process_Method_1 : public ECG_Process {
public:
    vector <int> main_process( const vector <int> *, const int & );
    void secondary_process();
};

vector <int> ECG_Process_Method_1::main_process(
    const vector <int> * data,
    const int &size
) {
    int data_per_process = data->size() / size;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( data->data(), data_per_process, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );

    vector <int> sq_data( data_per_process * size );
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, sq_data.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    if( data_per_process * size != data->size() ) {
        vector <int> extra_data;
        for( int i = data_per_process * size ; i < data->size() ; i++ ) {
            extra_data.push_back( data->at(i) );
        }
        Compression::inplace_compress( extra_data );
        for( int &i : extra_data ) {
            sq_data.push_back( i );
        }
    }

    return sq_data;
}

void ECG_Process_Method_1::secondary_process()
{
    int data_per_process;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( NULL, 0, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );
    
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD );
}

#endif