#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <numeric>

#include "Utilities.hpp"
#include "graph.hpp"

using namespace std;

class Model : Graph {
private:
    double s;
    std::vector<int> gene_vector;
    std::vector<double> fitness_vector;

public:
    int N;
    int counts = 0;
    double times = 0;
    string input_file_name;
    Model(const string &);
    void BDProcess(int *);
    void Simulation();
    void Simulation(double, int);
};

Model::Model(const string & INPUT_NAME) : Graph(INPUT_NAME) {
    this->s = 0;
    this->N = this->popsize;

    std::vector<int> zeros(N, 0);
    this->gene_vector = zeros;
    std::vector<double> ones(N, 1);
    this->fitness_vector = ones;

    this->input_file_name = INPUT_NAME;
    string::size_type idx;
    idx = this->input_file_name.find("../");
    if (idx != string::npos)
        this->input_file_name.erase(0,3);
}

void Model::BDProcess(int *populations) {
    // Calculate Fitness
    // Birth
    std::vector<double> fitness_accu = accumulate_sum(fitness_vector);
    int birth_pos = random_choice_single(fitness_accu);

    // Death
    int death_index = uniform_int_sample(0, degrees[birth_pos] - 1);
    int death_pos = edgelist[birth_pos][death_index];

    // Update
    if (gene_vector[death_pos] == gene_vector[birth_pos]) {
        return;
    }
    populations[gene_vector[death_pos]] --;
    populations[gene_vector[birth_pos]] ++;

    gene_vector[death_pos] = gene_vector[birth_pos];
    fitness_vector[death_pos] = 1 + gene_vector[birth_pos] * s;
}

void Model::Simulation() {
    int populations[] = {N, 0};

    int start_pos = uniform_int_sample(0, N - 1);
    gene_vector[start_pos] = 1;
    fitness_vector[start_pos] = 1 + s;

    populations[0] --;
    populations[1] ++;

    int t = 0;
    while (populations[0] != 0 && populations[0] != N) {
        // cout << "  " << t << endl;
        t++;
        BDProcess(populations);
    }

    if (populations[0] == 0) {
        // times = (times * counts + t) / ++counts;
        times = times / (counts + 1) * counts + (double) t / (counts + 1);
        ++counts;
    }
}

void Model::Simulation(double s_input, int runs) {
    counts = 0;
    times = 0;
    s = s_input;

    for (int i_run = 0; i_run < runs; i_run++) {
        // if (i_run % 100 == 0)
        //     cout << "Run: " << i_run << endl;
        std::vector<int> zeros(N, 0);
        gene_vector = zeros;
        std::vector<double> ones(N, 1);
        fitness_vector = ones;
        Simulation();
    }
}

int main(int argc, char **argv) {
    ofstream file;
    const string INPUT_NAME = argv[1];
    string output_name = argv[2];
    int runs = atoi(argv[3]);

    // Parameters
    std::vector<double> s_list;
    for(int i = 4; i < argc; i++)
        s_list.push_back(atof(argv[i]));

    Model trial(INPUT_NAME);
    int N = trial.N;
    for (double i_s : s_list) {
        // Run
        trial.Simulation(i_s, runs);
        double counts = trial.counts;
        double ave_gen = trial.times;
        file.open(output_name);
        file << N << "\t";
        file << i_s << "\t";
        file << runs << "\t";
        file << counts << "\t";
        file << ave_gen << endl;
        file.close();
    }
    return 0;
}

// g++ -static -std=c++17 sim.cpp -o sim