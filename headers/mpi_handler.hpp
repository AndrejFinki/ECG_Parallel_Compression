#ifndef MPI_HANDLER_H
#define MPI_HANDLER_H

#include <bits/stdc++.h>
using namespace std;

#include <mpi.h>

#include "ecg_process_mpi.hpp"

class MPI_Handler {

public:

    MPI_Handler() = default;

    static void run( const vector <int> *, vector <int> * );
    static void mpi_init();
    static void mpi_finalize();
    static int get_rank();
    static int get_size();
    static void sync();

};

void MPI_Handler::run(
    const vector <int> * input_data,
    vector <int> * output_data
) {
    ECG_Process *process = new ECG_Process_MPI( MPI_Handler::get_rank(), MPI_Handler::get_size() );

    switch( MPI_Handler::get_rank() )
    {
        case 0: {
            process->main_process( input_data, output_data );
            break;
        }

        default: {
            process->secondary_process();
        }
    }

    delete process;
}

void MPI_Handler::mpi_init()
{
    int argc = 1;
    char *argv;
    char **argv1 = &argv;
    MPI_Init( &argc, &argv1 );
}

void MPI_Handler::mpi_finalize()
{
    MPI_Finalize();
}

int MPI_Handler::get_rank()
{
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    return rank;
}

int MPI_Handler::get_size()
{
    int size;
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    return size;
}

void MPI_Handler::sync()
{
    MPI_Barrier( MPI_COMM_WORLD );
}

#endif