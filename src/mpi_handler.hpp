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

};

void MPI_Handler::run( int argc, char ** argv )
{
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    switch( rank )
    {
        case 0: {
            DataHandler *state = new DataHandler( argc, argv );
            state->read_data();
            const vector <int> * data = state->get_ecg_data();
            const vector <int> compressed_data = ECGProcess::main_process( data, size );
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

#endif