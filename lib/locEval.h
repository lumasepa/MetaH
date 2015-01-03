#ifndef _oneMaxEval_h
#define _oneMaxEval_h

#include <eoEvalFunc.h>
#include <ProblemDescription.h>
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
// en este caso, 100*el nº de 1 en el vector solución y a eso se le suma
// el minimo de las distancias entre los WH y todos los clientes

        // Tiene que haber por lo menos un warehouse en la solucion
        bool is_valid_solution = false;
        for (unsigned int i = 0; i < _sol.size(); i++)
        {
            if(_sol[i] == 1) {
                is_valid_solution = true;
                break;
            }
        }

        if (is_valid_solution){
            float total_cost = 0; // Objective Fitness
            unsigned int warehose_total_cost = 0;
            for (unsigned int i = 0; i < _sol.size(); i++) {   // For each warehouse
                // si seleccionamos ese warehouse en la solucion a evaluar
                if (_sol[i] == 1) {
                    warehose_total_cost += problemDescription.warehouse_costs[i];
                }
            }

            for (int i = 0; i < problemDescription.clients_number; ++i){
                float minDistance = FLT_MAX;
                for (int j = 0; j < problemDescription.warehouses_number; ++j) {
                    if(_sol[j] == 1){
                        if (problemDescription.client_to_wharehouse_distance[i][j] < minDistance){
                            minDistance = problemDescription.client_to_wharehouse_distance[i][j];
                        }
                    }
                }
                total_cost += minDistance;
            }

            //cout << "antes = " << _sol.fitness() << "\n";
            total_cost += warehose_total_cost;
            _sol.fitness(total_cost);
            cout << "sol = " << _sol << endl;
        }else{
            _sol.fitness(INT_MAX);
        }
    }
};

#endif
