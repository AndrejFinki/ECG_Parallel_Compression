#ifndef ECG_PROCESS_H
#define ECG_PROCESS_H

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#include "program_state.hpp"
#include "compression.hpp"

class ECGProcess {

public:

    ECGProcess() = default;

    static void run( int, char ** );
    static void mpi_init();
    static void mpi_finalize();

};

void ECGProcess::run( int argc, char ** argv )
{
    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    switch( rank )
    {
        case 0: {

            ProgramState *state = new ProgramState( argc, argv );
            state->read_file();
            const vector <int> *data = state->get_ecg_data();

            int data_per_process = data->size() / size;
            MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

            vector <int> pdata( data_per_process );
            MPI_Scatter( data->data(), data_per_process, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

            Compression::inplace_compress( pdata );

            vector <int> sq_data( data_per_process * size );
            MPI_Gather( pdata.data(), data_per_process, MPI_INT, sq_data.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

            int last_element = sq_data.size()-1;
            cout << sq_data[ last_element ] << ' ' << sqrt( sq_data[ last_element ] ) << endl;

            delete state;

            break;

        }

        default: {

            int data_per_process;

            MPI_Bcast( &data_per_process, 1, MPI_INT, 0, MPI_COMM_WORLD );

            vector <int> pdata( data_per_process );

            MPI_Scatter( NULL, 0, MPI_INT, pdata.data(), data_per_process, MPI_INT, 0, MPI_COMM_WORLD );

            Compression::inplace_compress( pdata );

            MPI_Gather( pdata.data(), data_per_process, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD );

        }
    }

}

void ECGProcess::mpi_init()
{
    int argc = 1;
    char *argv = "/workspaces/IV_PIDP/Proekt/src/driver";
    char **argv1 = &argv;
    MPI_Init( &argc, &argv1 );
}

void ECGProcess::mpi_finalize()
{
    MPI_Finalize();
}

#endif