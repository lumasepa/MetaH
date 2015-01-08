//-----------------------------------------------------------------------------
/** TS.cpp
 *
 * SV - 12/01/10
 * JH - 03/05/10
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
//Representation and initializer
#include <eoInt.h>
#include <eoInit.h>
#include <eoScalarFitness.h>

// fitness function
#include "SPLP_locEval.h"
#include <eval/queenEval.h>
#include <eval/moFullEvalByModif.h>
#include <eval/moFullEvalByCopy.h>

//Neighbors and Neighborhoods
#include <problems/permutation/moShiftNeighbor.h>
#include <neighborhood/moOrderNeighborhood.h>

//Algorithm and its components
#include <algo/moTS.h>

//mo eval
#include <eval/moFullEvalByModif.h>
#include <eval/moFullEvalByCopy.h>


// REPRESENTATION
//-----------------------------------------------------------------------------
//typedef eoInt<eoMinimizingFitness> Indi; //Permutation (Queen's problem representation)
typedef eoBit<eoMinimizingFitness> Indi;
typedef moShiftNeighbor<Indi> shiftNeighbor; //shift Neighbor
typedef moOrderNeighborhood<shiftNeighbor> orderShiftNeighborhood; //order shift Neighborhood (Indexed)

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

    // seed
    eoValueParam<uint32_t> seedParam(time(0), "seed", "Random number seed", 'S');
    parser.processParam( seedParam );
    unsigned seed = seedParam.value();

    eoValueParam<string> problem_file_param("", "problem_file", "Problem File", 'F', true);
    parser.processParam( problem_file_param );
    string problem_file = problem_file_param.value();

    // description of genotype
    eoValueParam<unsigned int> vecSizeParam(8, "vecSize", "Genotype size", 'V');
    parser.processParam( vecSizeParam, "Representation" );
    unsigned vecSize = vecSizeParam.value();

    // size tabu list
    eoValueParam<unsigned int> sizeTabuListParam(7, "sizeTabuList", "size of the tabu list", 'T');
    parser.processParam( sizeTabuListParam, "Search Parameters" );
    unsigned sizeTabuList = sizeTabuListParam.value();

    // time Limit
    eoValueParam<unsigned int> timeLimitParam(1, "timeLimit", "time limits", 'T');
    parser.processParam( timeLimitParam, "Search Parameters" );
    unsigned timeLimit = timeLimitParam.value();

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
    // you'll aways get the same result, NOT a random run
    rng.reseed(seed);


    eoUniformGenerator<bool> uGen;
    eoInitFixedLength<Indi> random(problemDescription.warehouses_number, uGen);

    /* =========================================================
     *
     * Full evaluation fitness function
     *
     * ========================================================= */

    oneMaxEval<Indi> fullEval;


    /* =========================================================
     *
     * Initializer of a solution
     *
     * ========================================================= */

    //eoInitPermutation<Indi> init(problemDescription.warehouses_number);


    /* =========================================================
     *
     * Declare and init solutions
     *
     * ========================================================= */

    Indi sol1;
    Indi sol2;
    Indi sol3;

    //random initialization
    random(sol1);
    random(sol2);
    random(sol3);

    //evaluation
    fullEval(sol1);
    fullEval(sol2);
    fullEval(sol3);

    /* =========================================================
     *
     * evaluation of a neighbor solution
     *
     * ========================================================= */

    moFullEvalByCopy<shiftNeighbor> shiftEval(fullEval);

    /* =========================================================
     *
     * the neighborhood of a solution
     *
     * ========================================================= */

    orderShiftNeighborhood orderShiftNH((problemDescription.warehouses_number-1) * (problemDescription.warehouses_number-1));

    /* =========================================================
     *
     * Comparator of neighbors and solutions
     *
     * ========================================================= */

    moSolNeighborComparator<shiftNeighbor> solComparator;
    moNeighborComparator<shiftNeighbor> comparator;

    /* =========================================================
     *
     * tabu list
     *
     * ========================================================= */

    moNeighborVectorTabuList<shiftNeighbor> tl(sizeTabuList,0);

    /* =========================================================
     *
     * Memories
     *
     * ========================================================= */

    moDummyIntensification<shiftNeighbor> inten;
    moDummyDiversification<shiftNeighbor> div;
    moBestImprAspiration<shiftNeighbor> asp;

    /* =========================================================
     *
     * continuator
     *
     * ========================================================= */

    moTimeContinuator<shiftNeighbor> continuator(timeLimit);


    /* =========================================================
     *
     * the local search algorithms
     *
     * ========================================================= */

    //Basic Constructor
    moTS<shiftNeighbor> localSearch1(orderShiftNH, fullEval, shiftEval, 2, 7);

    //Simple Constructor
    moTS<shiftNeighbor> localSearch2(orderShiftNH, fullEval, shiftEval, 3, tl);

    //General Constructor
    moTS<shiftNeighbor> localSearch3(orderShiftNH, fullEval, shiftEval, comparator, solComparator, continuator, tl, inten, div, asp);

    /* =========================================================
     *
     * execute the local search from random solution
     *
     * ========================================================= */





    //Can be eval here, else it will be done at the beginning of the localSearch
    //fullEval(solution);


    //Run the three Tabu Search and print initial and final solutions
    std::cout << "Tabu Search 1:" << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << "initial: " << sol1 << std::endl ;
    localSearch1(sol1);
    std::cout << "final:   " << sol1 << std::endl << std::endl;

    std::cout << "Tabu Search 2:" << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << "initial: " << sol2 << std::endl ;
    localSearch2(sol2);
    std::cout << "final:   " << sol2 << std::endl << std::endl;

    std::cout << "Tabu Search 3:" << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << "initial: " << sol3 << std::endl ;
    localSearch3(sol3);
    std::cout << "final:   " << sol3 << std::endl ;

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
