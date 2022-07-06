#include <iostream> /* Salida de datos por pantalla */
#include <fstream>  /* Lectura de archivos */
#include <sstream>  /* Lectura de archivos */
#include <string>   /* Lectura de archivos */
#include <vector>   /* Almacenamiendo de datos en vectores */
#include <math.h>   /* Cálculo de raices */
#include <stdlib.h> /* Generación aleatoria */
#include <chrono>   /* Cálculo del tiempo de ejecucción */
#include <random>   /* Generación de números aleatorios */
#include <algorithm>/* Para random_shuffle */
#include <regex>    /* Modifica el nombre del archivo de salida para gráficas */
#include <iomanip>  /* Salida en formato de tabla */

#include "funciones.cpp"

using namespace std;


int SEED = 0; /* Semilla usada para la generación aleatoria */
int FOLDS = 5; /* Número de particiones para k-fold cross validation */
float REDUCCTION = 0.1f; /* Cota para la tasa de reducción */

int EVALUATIONS = 15000;
int CHROMOSOMES = 30;

bool OUTPUT = true; /* Salida extendida por pantalla */
bool LATEX = false; /* Salida con un formato mas fácil de usar en tablas de latex */

bool EXPORT = false; /* Salida a un fichero para la representación en tablas */




/*
 * chargesEuclidean: Variante de la metaheurística basada en cargas eléctricas, que utiliza la distancia euclídea en
 * lugar de la distancia en fitness.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @chromosomes: int, número de cromosomas o soluciones iniciales.
 * @evaluations: int, número de evaluaciones por cada iteración.
 * @reduction: float, cota para la tasa de reducción.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> chargesEuclidean(vector<vector<float> > x, vector<string> y, int chromosomes, int evaluations, float reduction) {
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	vector<vector<float> > solutions;
	for (int i=0; i<chromosomes; i++) {
		solutions.push_back(initialize(x[0].size(), reduction, generator, uniform_distribution));
	}

	vector<float> fitness;
	int eval = 0;
	for (int i=0; i<chromosomes; i++) {
		fitness.push_back(agregation(x, y, solutions[i], reduction));
		eval++;
	}

	while (eval < evaluations) {
		int bestSolution = 0;
		int worstSolution = 0;
		float bestFitness = fitness[0];
		float worstFitness = fitness[0];

		for (int i=1; i<chromosomes; i++) {
			if (fitness[i] > bestFitness) {
				bestFitness = fitness[i];
				bestSolution = i;
			} else if (fitness[i] < worstFitness) {
				worstFitness = fitness[i];
				worstSolution = i;
			}
		}

		for (int i=0; i<chromosomes; i++) {
			if (i != bestSolution) {
				float pullForce = bestFitness * fitness[i] * (1 - pow(euclidean(solutions[i], solutions[bestSolution]), 2));
				float pushForce = worstSolution * fitness[i] * (1 - pow(euclidean(solutions[i], solutions[worstSolution]), 2));

				for (int j=0; j<x[0].size(); j++) {
					solutions[i][j] += (solutions[bestSolution][j] - solutions[i][j]) * pullForce;
					if (i != worstSolution) {
						solutions[i][j] -= (solutions[worstSolution][j] - solutions[i][j]) * pushForce;
					}

					if (solutions[i][j] < reduction) {
						solutions[i][j] = 0.0f;
					} else if (solutions[i][j] > 1.0f) {
						solutions[i][j] = 1.0f;
					}
				}

				fitness[i] = agregation(x, y, solutions[i], reduction);
				eval++;
			}
		}

		int random = rand() % chromosomes;
		while (random == bestSolution) {
			random = rand() % chromosomes;
		}
		solutions[random] = initialize(x[0].size(), reduction, generator, uniform_distribution);
		fitness[random] = agregation(x, y, solutions[random], reduction);
		eval++;
	}

	float bestFitness = fitness[0];
	int bestSolution = 0;
	for (int i=1; i<chromosomes; i++) {
		if (fitness[i] > bestFitness) {
			bestFitness = fitness[i];
			bestSolution = i;
		}
	}

	return solutions[bestSolution];
}

/*
 * exportChargesEuclidean: Variante de la metaheurística basada en cargas eléctricas, que utiliza la distancia euclídea en
 * lugar de la distancia en fitness, exportando los resultados a un fichero txt para ser representados.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @chromosomes: int, número de cromosomas o soluciones iniciales.
 * @evaluations: int, número de evaluaciones por cada iteración.
 * @reduction: float, cota para la tasa de reducción.
 * @fold: int, número de partición, utilizado para el nombre del fichero de salida.
 * @file: string, utilizado para el nombre del archivo de salida.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> exportChargesEuclidean(vector<vector<float> > x, vector<string> y, int chromosomes, int evaluations, float reduction, int fold, string file) {
	// Abrimos o creamos (si no existe) el archivo para guardar los datos
	string filename = "graph/" + file + to_string(fold+1) + "-chargesEuclidean.txt";
	regex target(".arff");
	filename = regex_replace(filename, target, "");
	ofstream myfile;
	myfile.open(filename, fstream::in | fstream::out | fstream::trunc);

	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	vector<vector<float> > solutions;
	for (int i=0; i<chromosomes; i++) {
		solutions.push_back(initialize(x[0].size(), reduction, generator, uniform_distribution));
	}

	vector<float> fitness;
	int eval = 0;
	for (int i=0; i<chromosomes; i++) {
		fitness.push_back(agregation(x, y, solutions[i], reduction));
		eval++;
	}

	while (eval < evaluations) {
		int bestSolution = 0;
		int worstSolution = 0;
		float bestFitness = fitness[0];
		float worstFitness = fitness[0];

		for (int i=1; i<chromosomes; i++) {
			if (fitness[i] > bestFitness) {
				bestFitness = fitness[i];
				bestSolution = i;
			} else if (fitness[i] < worstFitness) {
				worstFitness = fitness[i];
				worstSolution = i;
			}
		}

		myfile << eval << "," << bestFitness << endl;

		for (int i=0; i<chromosomes; i++) {
			if (i != bestSolution) {
				float pullForce = bestFitness * fitness[i] * (1 - pow(euclidean(solutions[i], solutions[bestSolution]), 2));
				float pushForce = worstSolution * fitness[i] * (1 - pow(euclidean(solutions[i], solutions[worstSolution]), 2));

				for (int j=0; j<x[0].size(); j++) {
					solutions[i][j] += (solutions[bestSolution][j] - solutions[i][j]) * pullForce;
					if (i != worstSolution) {
						solutions[i][j] -= (solutions[worstSolution][j] - solutions[i][j]) * pushForce;
					}

					if (solutions[i][j] < reduction) {
						solutions[i][j] = 0.0f;
					} else if (solutions[i][j] > 1.0f) {
						solutions[i][j] = 1.0f;
					}
				}

				fitness[i] = agregation(x, y, solutions[i], reduction);
				eval++;
			}
		}

		int random = rand() % chromosomes;
		while (random == bestSolution) {
			random = rand() % chromosomes;
		}
		solutions[random] = initialize(x[0].size(), reduction, generator, uniform_distribution);
		fitness[random] = agregation(x, y, solutions[random], reduction);
		eval++;
	}

	float bestFitness = fitness[0];
	int bestSolution = 0;
	for (int i=1; i<chromosomes; i++) {
		if (fitness[i] > bestFitness) {
			bestFitness = fitness[i];
			bestSolution = i;
		}
	}

	myfile.close();

	return solutions[bestSolution];
}



int main(int argc, char** argv) {
	// Lectura de datos.
	vector<vector<string> > data;
	if (argc > 1){
		data = readfile(argv[1]);
	} else {
		cout << "Introduce el nombre del archivo.\n";
		return 1;
	}
	if (OUTPUT) {
		cout << "Leída una matriz de tamaño (" << data.size() << "," << data[0].size() << ").\n\n";
	}

	// Permutamos los elementos para tomarlos en orden aleatorio dependiendo de la semilla
	srand(SEED);
	random_shuffle(data.begin(), data.end(), randomSeed);

	// Separamos en datos y etiquetas.
	vector<vector<float> > x = getData(data);
	vector<string> y = getLabel(data);

	// Normalizamos los datos
	x = normalizeData(x);

	float classification;
	float reduction;
	vector<float> classificationVector;
	vector<float> reductionVector;
	vector<float> agregationVector;
	vector<float> timeVector;

	for (int k=0; k<FOLDS; k++) {
		if (OUTPUT) {
			cout << "\tIteración " << k+1 << ":\n";
		}

		// Separamos los datos en train y test
		vector<vector<float> > x_train, x_test;
		vector<string> y_train, y_test;

		train_test_split(x, y, x_train, x_test, y_train, y_test, k, FOLDS);
		if (OUTPUT) {
			cout << x_train.size() << " elementos de entrenamiento (" << (float) x_train.size()/x.size()*100 << "%), " 
				 << x_test.size() << " elementos de test (" << (float) x_test.size()/x.size()*100 << "%).\n";
		}

		chrono::time_point<chrono::system_clock> start;
		chrono::time_point<chrono::system_clock> end;

		vector<float> w;

		start = chrono::system_clock::now();
		if (EXPORT) {
			w = exportChargesEuclidean(x, y, CHROMOSOMES, EVALUATIONS, REDUCCTION, k, argv[1]);
		} else {
			w = chargesEuclidean(x, y, CHROMOSOMES, EVALUATIONS, REDUCCTION);
		}
		end = chrono::system_clock::now();

		float elapsed_seconds = chrono::duration<float>(end - start).count();

		if (OUTPUT) {
			cout << "Solución: W = (";
			printArray(w);
			cout << ").\n" << endl;
		}

		vector<string> y_pred = predictLabel(x_test, x_train, y_train, w, REDUCCTION);

		agregationVector.push_back(agregation(y_test, y_pred, w, REDUCCTION, classification, reduction) * 100);
		classificationVector.push_back(classification * 100);
		reductionVector.push_back(reduction * 100);
		timeVector.push_back(elapsed_seconds);
	}
	// Salida en formato de tabla
	if (LATEX) {
    	latexExport(classificationVector, reductionVector, agregationVector, timeVector);
    } else {
		printTable(classificationVector, reductionVector, agregationVector, timeVector);
    }

	return 0;
}