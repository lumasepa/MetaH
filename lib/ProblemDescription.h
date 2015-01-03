#ifndef ProblemDescription_h
#define ProblemDescription_h

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class ProblemDescription{

public:
    static ProblemDescription& getInstance()
    {
        static ProblemDescription instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

private:
    ProblemDescription() {};
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    ProblemDescription(ProblemDescription const&); // Don't Implement
    void operator=(ProblemDescription const&); // Don't implement

public:
    unsigned int warehouses_number;
    unsigned int clients_number;

    vector<float> warehouse_costs;
    vector< vector<float> > client_to_wharehouse_distance;

    void set_problem_from_file(string problem_file){
        ifstream file(problem_file.c_str(), std::ifstream::in);
        if (file.fail())
        {
            cout << "Error opening file : " << problem_file << endl;
            exit(-10);
        }

        file >> warehouses_number;
        file >> clients_number;

        float cost = 0;
        int ignore = 0;

        for (int i = 0; i < warehouses_number; ++i){
            file >> ignore;
            file >> cost;
            warehouse_costs.push_back(cost);
        }

        float distance_cost;

        for (int i = 0; i < clients_number; ++i){
            file >> ignore;
            client_to_wharehouse_distance.push_back(vector<float>());
            for (int j = 0; j < warehouses_number; ++j) {
                file >> distance_cost;
                client_to_wharehouse_distance[i].push_back(distance_cost);
            }
        }
    }

    float operator()(int client, int warehouse){
        return client_to_wharehouse_distance[client][warehouse];
    }

    unsigned int operator()(int warehouse){
        return warehouse_costs[warehouse];
    }

    void print (ostream& out)
    {
        out << "warehouses_number " << this->warehouses_number <<endl;
        out << "clients_number " << this->clients_number <<endl;
        out << "warehouse_costs : " << endl;
        for(int i = 0; i < this->warehouse_costs.size(); ++i)
            out << i << " " << warehouse_costs[i] << endl;

        out << "client_to_wharehouse_distance : " << endl;
        for(int i = 0; i < this->client_to_wharehouse_distance.size(); ++i){
            out << i << " ";
            for (int j = 0; j < this->client_to_wharehouse_distance[i].size(); ++j){
                out << client_to_wharehouse_distance[i][j] << " ";
            }
            out << endl;
        }
    }

};

#define problemDescription ProblemDescription::getInstance()

/*class WarehouseSolution {
    eoBit<unsigned int> warehouses;
    ProblemDescription * problem_description;
};*/

#endif