#ifndef _oneMaxEval_h
#define _oneMaxEval_h

#include <eoEvalFunc.h>
#include <SPLP_ProblemDescription.h>
/**
 * Full evaluation Function for OneMax problem
 */
#include <climits>
#include <cfloat>

template< class EOT > class oneMaxEval : public eoEvalFunc<EOT>
{
public:

	/**
	 * Count the number of 1 in a bitString
	 * @param _sol the solution to evaluate
	 */
    void operator() (EOT& _sol) {

// Nota, la función objetivo devuelve, el coste de poner los warehouses
// y a eso se le suma el minimo de las distancias entre los clientes y los warehouses

        // Tiene que haber por lo menos un warehouse en la solucion
        bool is_valid_solution = false;
        for (unsigned int i = 0; i < _sol.size(); i++)
        {
            if(_sol[i] == 1) {
                is_valid_solution = true;
                break;
            }
        }
        // si hay al menos un warehouse en la solucion
        if (is_valid_solution){

            float total_cost = 0;

            // calculamos el coste de cada cliente al warehouse con menos coste
            for (int i = 0; i < problemDescription.clients_number; ++i){
                float minDistance = FLT_MAX;
                for (int j = 0; j < problemDescription.warehouses_number; ++j) {
                    // si es uno de los warehouses seleccionados en la solucion a evaluar
                    if(_sol[j] == 1){
                        if (problemDescription.client_to_wharehouse_distance[i][j] < minDistance){
                            minDistance = problemDescription.client_to_wharehouse_distance[i][j];
                        }
                    }
                }
                // Añadimos el coste minimo del cliente al warehouse
                total_cost += minDistance;
            }
            _sol.fitness(total_cost);
            cout << "sol = " << _sol << endl;

        } // Si la solucion no es valida
        else {
            _sol.fitness(INT_MAX);
        }
    }
};

#endif
