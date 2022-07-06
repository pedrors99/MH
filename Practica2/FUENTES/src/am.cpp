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

int CHROMOSOMES = 10; /* Número de cromosomas */
int ITERATIONS = 15000; /* Número de iteraciones para el algoritmo */
float ALPHA = 0.3f; /* Variable del algoritmo BLX */
float CROSS = 0.7f; /* Probabilidad de cruce */
float MUTATION = 0.1f; /* Probabilidad de mutar de un gen */

int STEP = 10; /* Número de generaciones trás las cuales aplicamos local search */

bool OUTPUT = true; /* Salida extendida por pantalla */
bool LATEX = false; /* Salida con un formato mas fácil de usar en tablas de latex */

bool EXPORT = false; /* Salida a un fichero para la representación en tablas */



/*
 * am: Algortimo memético generacional con cruce blx.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @chromosomes: int, número de cromosomas.
 * @iterations: int, número de iteraciones.
 * @reduction: float, cota para la tasa de reducción.
 * @alpha: float, variable del algoritmo de cruce blx.
 * @crossChance: float, probabilidad de cruce, entre 0 y 1.
 * @mutationChance: float, probabilidad de mutación, entre 0 y 1.
 * @generationStep: int, número de generationes trás las cuáles aplicamos la búsqueda local.
 * @size: float, porcentaje de la población a la que aplicamos la búsqueda local.
 * @sort: bool, determina si aplicamos búsqueda local a las mejores soluciones o a soluciones aleatorias.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> am(vector<vector<float> > x, vector<string> y, int chromosomes, int iterations, float reduction, float alpha, float crossChance, float mutationChance, int generationStep, float size, bool sort=false) {
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	int eval = 0;
	int generation = 0;
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
		//generation++;

		// Selección
		newSolutions = selection(solutions, fitness, solutions.size());

		// Cruce
		newSolutions = crossCa(newSolutions, crossChance, reduction, generator, uniform_distribution);

		// Mutación
		newSolutions = mutations(newSolutions, mutationChance, reduction, generator, normal_distribution);

		// Algoritmo memético
		if (generation % generationStep == 0) {
			int memeticSize = newSolutions.size() * size;
			vector<int> ind;

			if (sort) {
				ind = sortInd(fitness, memeticSize);
			} else {
				for (int i=0; i<memeticSize; i++) {
					ind.push_back(i);
				}
			}

			for (int i=0; i<memeticSize; i++) {
				newSolutions[ind[i]] = memeticLocalSearch(x, y, newSolutions[ind[i]], 2 * newSolutions[ind[i]].size(), fitness[ind[i]], eval, iterations, reduction, generator, normal_distribution);
			}
		}

		// Reemplazamiento
		solutions = replaceGenerational(solutions, newSolutions, fitness, x, y, reduction, eval);
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
 * exportAm: Algortimo memético generacional con cruce blx exportando resultados.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @chromosomes: int, número de cromosomas.
 * @iterations: int, número de iteraciones.
 * @reduction: float, cota para la tasa de reducción.
 * @alpha: float, variable del algoritmo de cruce blx.
 * @crossChance: float, probabilidad de cruce, entre 0 y 1.
 * @mutationChance: float, probabilidad de mutación, entre 0 y 1.
 * @generationStep: int, número de generationes trás las cuáles aplicamos la búsqueda local.
 * @size: float, porcentaje de la población a la que aplicamos la búsqueda local.
 * @fold: int, número de la partición.
 * @file: string, nombre del archivo.
 * @sort: bool, determina si aplicamos búsqueda local a las mejores soluciones o a soluciones aleatorias.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> exportAm(vector<vector<float> > x, vector<string> y, int chromosomes, int iterations, float reduction, float alpha, float crossChance, float mutationChance, int generationStep, float size, int fold, string file, bool sort=false) {
	// Abrimos o creamos (si no existe) el archivo para guardar los datos
	string filename = "graph/" + file + to_string(fold+1) + "-am";
	if (size < 1.0f) {
		filename += "0";
	}
	filename += to_string((int)(size * 10));
	if (sort) {
		filename += "mej";
	}
	filename += ".txt";
	regex target(".arff");
	filename = regex_replace(filename, target, "");
	ofstream myfile;
	myfile.open(filename, fstream::in | fstream::out | fstream::trunc);
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	int eval = 0;
	int generation = 0;
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
		generation++;

		// Selección
		newSolutions = selection(solutions, fitness, solutions.size());

		// Cruce
		newSolutions = crossCa(newSolutions, crossChance, reduction, generator, uniform_distribution);

		// Mutación
		newSolutions = mutations(newSolutions, mutationChance, reduction, generator, normal_distribution);

		// Algoritmo memético
		if (generation % generationStep == 0) {
			int memeticSize = newSolutions.size() * size;
			vector<int> ind;

			if (sort) {
				ind = sortInd(fitness, memeticSize);
			} else {
				for (int i=0; i<memeticSize; i++) {
					ind.push_back(i);
				}
			}

			for (int i=0; i<memeticSize; i++) {
				newSolutions[ind[i]] = memeticLocalSearch(x, y, newSolutions[ind[i]], 2 * newSolutions[ind[i]].size(), fitness[ind[i]], eval, iterations, reduction, generator, normal_distribution);
			}
		}

		// Reemplazamiento
		solutions = replaceGenerational(solutions, newSolutions, fitness, x, y, reduction, eval);

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
	float size;
	bool sort = false;

	if (argc > 1){
		data = readfile(argv[1]);
	} else {
		cout << "Introduce el nombre del arcahivo.\n";
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

	if (OUTPUT) {
		cout << "Introduce el porcentaje de población al que aplicar búsqueda local (entre 0.0 y 1.0): ";
	}
	cin >> size;

	if (size < 0.0f || size > 1.0f) {
		cout << "Entrada no válida." << endl;
		return 1;
	}

	if (size < 1.0f) {
		if (OUTPUT) {
			cout << "Introduce si toma soluciones aleatorias (0) o las mejores (1): ";
		}
		cin >> sort;
	}
	cout << endl;

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
			w = exportAm(x_train, y_train, CHROMOSOMES, ITERATIONS, REDUCCTION, ALPHA, CROSS, MUTATION, STEP, size, k, argv[1], sort);
		} else {
			w = am(x_train, y_train, CHROMOSOMES, ITERATIONS, REDUCCTION, ALPHA, CROSS, MUTATION, STEP, size, sort);
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