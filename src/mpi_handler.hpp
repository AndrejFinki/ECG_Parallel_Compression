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

    static void run( int, char ** );
    static void mpi_init();
    static void mpi_finalize();
    static int get_rank();
    static int get_size();

};

void MPI_Handler::run(
    int argc,
    char ** argv
) {
    switch( MPI_Handler::get_rank() )
    {
        case 0: {
            DataHandler *state = new DataHandler( argc, argv );
            state->read_data();
            const vector <int> * data = state->get_ecg_data();
            const vector <int> compressed_data = ECGProcess::main_process( data, MPI_Handler::get_size() );
            state->write_output( compressed_data );
            delete state;
            break;
        }

        default: {
            ECGProcess::secondary_process();
        }
    }
}

void MPI_Handler::mpi_init()
{
    int argc = 1;
    char *argv = "/workspaces/IV_PIDP/Proekt/src/driver";
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