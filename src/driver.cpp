#include <bits/stdc++.h>
using namespace std;

#include "program_state.hpp"
#include "ecg_process.hpp"

int main( int argc, char ** argv )
{
    ECGProcess::mpi_init();

    ECGProcess::run( argc, argv );

    ECGProcess::mpi_finalize();
}