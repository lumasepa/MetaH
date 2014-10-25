#include <stdexcept>  
#include <iostream>   
#include <sstream>    
#include <fstream>
#include <string.h>

#include <eo> 

using namespace std;

#include <ga/eoBit.h>                         
#include <problems/bitString/moBitNeighbor.h> 
#include <problems/eval/oneMaxFullEval.h>
#include <problems/eval/moOneMaxIncrEval.h>
#include <eval/moFullEvalByModif.h>

#include <neighborhood/moOrderNeighborhood.h> 
#include <algo/moSimpleHC.h>

typedef eoBit<unsigned int> Indi;
typedef moBitNeighbor<unsigned int> Neighbor; 


void main_function(int argc, char **argv) {
    eoParser parser(argc, argv);

    eoValueParam<uint32_t> seedParam(time(0), "seed", "Random number seed", 'S');
    parser.processParam( seedParam );
    unsigned seed = seedParam.value();

    // length of the bit string
    eoValueParam<unsigned int> vecSizeParam(20, "vecSize", "Genotype size", 'V');
    parser.processParam( vecSizeParam, "Representation" );
    unsigned vecSize = vecSizeParam.value();

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

    rng.reseed(seed);
    // Hay que construir las soluciones iniciales metiendo los datos desde arrays

    oneMaxEval<Indi> locEval;    // Hay que programar locEval y locNeighbor Eval
    moOneMaxIncrEval<Neighbor> locNeighborEval;

    moOrderNeighborhood<Neighbor> neighborhood(vecSize);
    moSimpleHC<Neighbor> hc(neighborhood, locEval, locNeighborEval); 

    Indi solution;
    random(solution);
    locEval(solution);

    std::cout << "initial: " << solution << std::endl ;

    // Apply the local search on the solution !
    hc(solution);

    // Output: the final solution
    std::cout << "final:   " << solution << std::endl ;

}


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
