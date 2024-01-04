#ifndef ECG_PROCESS_H
#define ECG_PROCESS_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "compression.hpp"

class ECGProcess {

public:

    ECGProcess() = default;

    static vector <int> main_process( const vector <int> *, const int & );
    static void secondary_process();

};

vector <int> ECGProcess::main_process( const vector <int> * data, const int &size )
{
    int data_per_process = data->size() / size;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( data->data(), data_per_process, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );

    vector <int> sq_data( data_per_process * size );
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, sq_data.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    return sq_data;
}

void ECGProcess::secondary_process()
{
    int data_per_process;
    MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

    vector <int> pdata( data_per_process );
    MPI_Scatter( NULL, 0, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

    Compression::inplace_compress( pdata );
    
    MPI_Gather( pdata.data(), data_per_process, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD );
}

#endif