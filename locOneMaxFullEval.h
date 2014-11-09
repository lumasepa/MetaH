#include <eoEvalFunc.h>

template< class EOT >
class oneMaxFullEval : public eoEvalFunc<EOT>
{
public:

    /**
     * Count the number of 1 in a bitString
     * @param _sol the solution to evaluate
     */
    void operator() (EOT& _solution) {
        unsigned int sum = 0;
        for (unsigned int i = 0; i < _solution.size(); i++)
            sum += _solution[i];
        _solution.fitness(sum);
    }

}; 
