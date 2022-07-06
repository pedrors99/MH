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

int CHROMOSOMES = 30; /* Número de cromosomas */
int ITERATIONS = 15000; /* Número de iteraciones para el algoritmo */
float ALPHA = 0.3f; /* Variable del algoritmo BLX */
float CROSS = 0.7f; /* Probabilidad de cruce */
float MUTATION = 0.1f; /* Probabilidad de mutar de un gen */

bool OUTPUT = true; /* Salida extendida por pantalla */
bool LATEX = false; /* Salida con un formato mas fácil de usar en tablas de latex */

bool EXPORT = false; /* Salida a un fichero para la representación en tablas */



/*
 * ageBlx: Algortimo genético estacionario con cruce blx.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @chromosomes: int, número de cromosomas.
 * @iterations: int, número de iteraciones.
 * @reduction: float, cota para la tasa de reducción.
 * @alpha: float, variable del algoritmo de cruce blx.
 * @mutation_chance: float, probabilidad de mutación, entre 0 y 1.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> ageBlx(vector<vector<float> > x, vector<string> y, int chromosomes, int iterations, float reduction, float alpha, float mutationChance) {
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	int eval = 0;
	vector<vector<float> > solutions;
	vector<float> fitness;

	// Inicializamos las soluciones con la distribución normal
	for (int i=0; i<chromosomes; i++) {
		solutions.push_back(initialize(x[0].size(), reduction, generator, uniform_distribution));
		fitness.push_back(agregation(x, y, solutions[i], reduction));
		eval++;
	}

	vector<vector<float> > newSolutions;

	while (eval < iterations) {
		// Selección
		newSolutions = selection(solutions, fitness, 2);

		// Cruce
		newSolutions = crossBlx(newSolutions, 1.0f, alpha, reduction, generator, uniform_distribution);

		// Mutación
		newSolutions = mutations(newSolutions, mutationChance, reduction, generator, normal_distribution);

		// Reemplazamiento
		solutions = replaceStationary(solutions, newSolutions, fitness, x, y, reduction, eval);
	}

	// Solución
	float maxAgregation = fitness[0];
	int maxPos = 0;
	for (int i=1; i<solutions.size(); i++) {
		if (fitness[i] > maxAgregation) {
			maxAgregation = fitness[i];
			maxPos = i;
		}
	}

	return solutions[maxPos];
}

/*
 * exportAgeBlx: Algortimo genético estacionario con cruce Blx exportando resultados.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @chromosomes: int, número de cromosomas.
 * @iterations: int, número de iteraciones.
 * @reduction: float, cota para la tasa de reducción.
 * @alpha: float, variable del algoritmo de cruce blx.
 * @mutation_chance: float, probabilidad de mutación, entre 0 y 1.
 * @fold: int, número de la partición.
 * @file: string, nombre del archivo.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> exportAgeBlx(vector<vector<float> > x, vector<string> y, int chromosomes, int iterations, float reduction, float alpha, float mutationChance, int fold, string file) {
	// Abrimos o creamos (si no existe) el archivo para guardar los datos
	string filename = "graph/" + file + to_string(fold+1) + "-ageBlx.txt";
	regex target(".arff");
	filename = regex_replace(filename, target, "");
	ofstream myfile;
	myfile.open(filename, fstream::in | fstream::out | fstream::trunc);

	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	int eval = 0;
	vector<vector<float> > solutions;
	vector<float> fitness;

	// Inicializamos las soluciones con la distribución normal
	for (int i=0; i<chromosomes; i++) {
		solutions.push_back(initialize(x[0].size(), reduction, generator, uniform_distribution));
		fitness.push_back(agregation(x, y, solutions[i], reduction));
		eval++;
	}

	vector<vector<float> > newSolutions;

	float maxAgregation = fitness[0];
	int maxPos = 0;
	for (int i=1; i<solutions.size(); i++) {
		if (fitness[i] > maxAgregation) {
			maxAgregation = fitness[i];
			maxPos = i;
		}
	}
	myfile << eval << "," << fitness[maxPos] << endl;

	while (eval < iterations) {
		// Selección
		newSolutions = selection(solutions, fitness, 2);

		// Cruce
		newSolutions = crossBlx(newSolutions, 1.0f, alpha, reduction, generator, uniform_distribution);

		// Mutación
		newSolutions = mutations(newSolutions, mutationChance, reduction, generator, normal_distribution);

		// Reemplazamiento
		solutions = replaceStationary(solutions, newSolutions, fitness, x, y, reduction, eval);

		// Solución
		maxAgregation = fitness[0];
		maxPos = 0;
		for (int i=1; i<solutions.size(); i++) {
			if (fitness[i] > maxAgregation) {
				maxAgregation = fitness[i];
				maxPos = i;
			}
		}

		myfile << eval << "," << fitness[maxPos] << endl;
	}

	myfile.close();

	return solutions[maxPos];
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
			w = exportAgeBlx(x_train, y_train, CHROMOSOMES, ITERATIONS, REDUCCTION, ALPHA, MUTATION, k, argv[1]);
		} else {
			w = ageBlx(x_train, y_train, CHROMOSOMES, ITERATIONS, REDUCCTION, ALPHA, MUTATION);
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