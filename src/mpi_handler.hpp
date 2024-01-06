#ifndef MPI_HANDLER_H
#define MPI_HANDLER_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "ecg_process.hpp"
#include "data_handler.hpp"

class MPI_Handler {

public:

    MPI_Handler() = default;

    static void run( const string & );
    static void mpi_init();
    static void mpi_finalize();
    static int get_rank();
    static int get_size();

};

void MPI_Handler::run(
    const string &file_name
) {
    switch( MPI_Handler::get_rank() )
    {
        case 0: {
            Data_Handler *input = new Data_Handler( "../ECG_Parallel_Compression/data/" + file_name );
            const vector <int> * data = input->read();

            Data_Handler *output = new Data_Handler( "../ECG_Parallel_Compression/output/" + file_name );
            const vector <int> compressed_data = ECG_Process::main_process( data, MPI_Handler::get_size() );
            output->write( &compressed_data );

            delete input;
            delete output;
            break;
        }

        default: {
            ECG_Process::secondary_process();
        }
    }
}

void MPI_Handler::mpi_init()
{
    int argc = 1;
    char *argv = "/workspaces/ECG_Parallel_Compression/src/driver";
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

#endif