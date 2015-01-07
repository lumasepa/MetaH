//-----------------------------------------------------------------------------
/** testSimulatedAnnealing.cpp
 *
 * SV - 29/03/10
 * JH - 20/04/10
 */
//-----------------------------------------------------------------------------

// standard includes
#define HAVE_SSTREAM

#include <stdexcept>  // runtime_error 
#include <iostream>   // cout
#include <sstream>  // ostrstream, istrstream
#include <fstream>
#include <string.h>

// the general include for eo
#include <eo>
#include <ga.h>

using namespace std;

//-----------------------------------------------------------------------------
// representation of solutions, and neighbors
#include <ga/eoBit.h>                         // bit string : see also EO tutorial lesson 1: FirstBitGA.cpp
#include <problems/bitString/moBitNeighbor.h> // neighbor of bit string


// fitness function
#include <eval/queenEval.h>
#include <eval/moFullEvalByModif.h>
#include <eval/moFullEvalByCopy.h>

#include <neighborhood/moRndWithReplNeighborhood.h>

//Algorithm and its components
#include <coolingSchedule/moCoolingSchedule.h>
#include <algo/moSA.h>

//continuators
#include <continuator/moCheckpoint.h>
#include <continuator/moFitnessStat.h>
#include <continuator/moCounterMonitorSaver.h>

#include <problems/permutation/moShiftNeighbor.h>

// Singleton with the problem description
#include <SPLP_ProblemDescription.h>

#include "SPLP_locEval.h"

#include <neighborhood/moOrderNeighborhood.h> // visit all neighbors in increasing order of bit index


// Declaration of types
//-----------------------------------------------------------------------------
// Indi is the typedef of the solution type like in paradisEO-eo
typedef eoBit<eoMinimizingFitness> Indi;                      // bit string with unsigned fitness type
// Neighbor is the typedef of the neighbor type,
// Neighbor = How to compute the neighbor from the solution + information on it (i.e. fitness)
// all classes from paradisEO-mo use this template type
typedef moBitNeighbor<eoMinimizingFitness> Neighbor ;         // bit string neighbor with unsigned fitness type
typedef moShiftNeighbor<Indi> shiftNeighbor;
typedef moRndWithReplNeighborhood<shiftNeighbor> rndShiftNeighborhood; //rnd shift Neighborhood (Indexed)

void main_function(int argc, char **argv)
{
    /* =========================================================
    *
    * Parameters
    *
    * ========================================================= */

    // First define a parser from the command-line arguments
    eoParser parser(argc, argv);

    // For each parameter, define Parameter, read it through the parser,
    // and assign the value to the variable

    eoValueParam<uint32_t> seedParam(time(0), "seed", "Random number seed", 'S');
    parser.processParam( seedParam );
    unsigned seed = seedParam.value();

    // description of genotype
    eoValueParam<unsigned int> vecSizeParam(8, "vecSize", "Genotype size", 'V');
    parser.processParam( vecSizeParam, "Representation" );
    unsigned vecSize = vecSizeParam.value();

    // File with the problem
    eoValueParam<string> problem_file_param("", "problem_file", "Problem File", 'F', true);
    parser.processParam( problem_file_param );
    string problem_file = problem_file_param.value();

    // the name of the "status" file where all actual parameter values will be saved
    string str_status = parser.ProgramName() + ".status"; // default value
    eoValueParam<string> statusParam(str_status.c_str(), "status", "Status file");
    parser.processParam( statusParam, "Persistence" );

    // do the following AFTER ALL PARAMETERS HAVE BEEN PROCESSED
    // i.e. in case you need parameters somewhere else, postpone these
    if (parser.userNeedsHelp()) {
        parser.printHelp(cout);
        exit(1);
    }
    if (statusParam.value() != "") {
        ofstream os(statusParam.value().c_str());
        os << parser;// and you can use that file as parameter file
    }

    cout << "problem_file : " << problem_file << endl;
    problem_file = "cap71.txt";
    problemDescription.set_problem_from_file(problem_file);

    /* =========================================================
     *
     * Random seed
     *
     * ========================================================= */

    //reproducible random seed: if you don't change SEED above,
    // you'll always get the same result, NOT a random run
    rng.reseed(seed);


    // a Indi random initializer: each bit is random
    // more information: see EO tutorial lesson 1 (FirstBitGA.cpp)
    eoUniformGenerator<bool> uGen;
    eoInitFixedLength<Indi> random(problemDescription.warehouses_number, uGen);

    /* =========================================================
     *
     * Eval fitness function (full evaluation)
     *
     * ========================================================= */

    // the fitness function is just the number of 1 in the bit string
    oneMaxEval<Indi> fullEval;

    // Use it if there is no incremental evaluation: a neighbor is evaluated by the full evaluation of a solution
    moFullEvalByModif<Neighbor> neighborEval(fullEval);

    /* =========================================================
     *
     * the neighborhood of a solution
     *
     * ========================================================= */

    //moOrderNeighborhood<Neighbor> neighborhood(problemDescription.warehouses_number);
    rndShiftNeighborhood neighborhood((problemDescription.warehouses_number-1) * (problemDescription.warehouses_number-1));
    /* =========================================================
     *
     * the local search algorithm
     *
     * ========================================================= */

    moSA<Neighbor> localSearch1(neighborhood, fullEval, neighborEval);

    /* =========================================================
     *
     * execute the local search from random solution
     *
     * ========================================================= */

    Indi solution1, solution2;

    random(solution1);

    fullEval(solution1);

    std::cout << "#########################################" << std::endl;
    std::cout << "initial solution1: " << solution1 << std::endl ;

    localSearch1(solution1);

    std::cout << "final solution1: " << solution1 << std::endl ;
    std::cout << "#########################################" << std::endl;


    /* =========================================================
     *
     * the cooling schedule of the process
     *
     * ========================================================= */

    // initial temp, factor of decrease, number of steps without decrease, final temp.
//    moSimpleCoolingSchedule<Indi> coolingSchedule(1, 0.9, 100, 0.01);

    /* =========================================================
     *
     * Comparator of neighbors
     *
     * ========================================================= */

//    moSolNeighborComparator<shiftNeighbor> solComparator;

    /* =========================================================
     *
     * Example of Checkpointing
     *
     * ========================================================= */
/*
    moTrueContinuator<shiftNeighbor> continuator;//always continue
    moCheckpoint<shiftNeighbor> checkpoint(continuator);
    moFitnessStat<Indi> fitStat;
    checkpoint.add(fitStat);
    eoFileMonitor monitor("fitness.out", "");
    moCounterMonitorSaver countMon(100, monitor);
    checkpoint.add(countMon);
    monitor.add(fitStat);

//  moSA<shiftNeighbor> localSearch2(neighborhood, fullEval, neighborEval, coolingSchedule, solComparator, checkpoint);

    random(solution2);

    fullEval(solution2);

    std::cout << "#########################################" << std::endl;
    std::cout << "initial solution2: " << solution2 << std::endl ;

//    localSearch2(solution2);

    std::cout << "final solution2: " << solution2 << std::endl ;
    std::cout << "#########################################" << std::endl; */
}

// A main that catches the exceptions

int main(int argc, char **argv)
{
    try {
        main_function(argc, argv);
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << '\n';
    }
    return 1;
}
