#ifndef _oneMaxEval_h
#define _oneMaxEval_h

#include <eoEvalFunc.h>
#include <ProblemDescription.h>
/**
 * Full evaluation Function for OneMax problem
 */

template< class EOT > class oneMaxEval : public eoEvalFunc<EOT>
{
public:

	/**
	 * Count the number of 1 in a bitString
	 * @param _sol the solution to evaluate
	 */
    void operator() (EOT& _sol) {


        int locCost = 100; // In this case is 100 for all warehouses, we use this for simplicity

// Nota, la función objetivo devuelve, el coste de poner los warehouses
// en este caso, 100*el nº de 1 en el vector solución y a eso se le suma
// el minimo de las distancias entre los WH y todos los clientes
       /* unsigned int sum = 0;
        for (unsigned int i = 0; i < _sol.size(); i++)
            sum += _sol[i];
*/
        unsigned int sum = 0; // Objective Fitness
        unsigned int posCost = 0; // Warehouse cost 100*warehouses in this case
        for (unsigned int i = 0; i < _sol.size(); i++) {   // For each warehouse
            posCost += _sol[i] * locCost;
            unsigned int minDistance = 0; // The minium distance between a client and one possible WH
            for (unsigned int j = 0; j < problemDescription.clients_number; j++) {   // For each client
                if (_sol[i] == 1) {
                    if (minDistance == 0)
                        minDistance = problemDescription.client_to_wharehouse_distance[i][j];
                    if (problemDescription.client_to_wharehouse_distance[i][j] <= minDistance)
                        minDistance = problemDescription.client_to_wharehouse_distance[i][j];
                }
            }
            sum += minDistance;
        }
        //cout << "antes = " << _sol.fitness() << "\n";
        cout << "despues = " << sum << "\n";
        sum += posCost;
        _sol.fitness(sum);
    }
};

#endif
