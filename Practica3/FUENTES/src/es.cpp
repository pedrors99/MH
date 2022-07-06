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

int EVALUATIONS = 15000; /* Número de evaluaciones para cada búsqueda local */

float MU = 0.3f; /* Variable utilizada para calcular la temperatura inicial */
float PHI = 0.3f; /* Variable utilizada para calcular la temperatura inicial */

bool OUTPUT = true; /* Salida extendida por pantalla */
bool LATEX = false; /* Salida con un formato mas fácil de usar en tablas de latex */




/*
 * es: Algortimo de enfriamiento simulado.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @evaluations: int, número de evaluaciones por cada iteración.
 * @reduction: float, cota para la tasa de reducción.
 * @mu: float, variable utilizada para calcular la temperatura inicial.
 * @phi: float, variable utilizada para calcular la temperatura inicial.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> es(vector<vector<float> > x, vector<string> y, int evaluations, float reduction, float mu, float phi) {
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	vector<float> w = initialize(x[0].size(), reduction, generator, uniform_distribution);
	int eval = 0;

	w = es(w, x, y, reduction, mu, phi, eval, evaluations, generator, normal_distribution, uniform_distribution);

	return w;
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
		w = es(x, y, EVALUATIONS, REDUCCTION, MU, PHI);
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