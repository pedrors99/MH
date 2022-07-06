#include <iostream> /* Salida de datos por pantalla */
#include <fstream>  /* Lectura de archivos */
#include <sstream>  /* Lectura de archivos */
#include <string>   /* Lectura de archivos */
#include <vector>   /* Almacenamiendo de datos en vectores */
#include <math.h>   /* Cálculo de raices */
#include <stdlib.h> /* Generación aleatoria */
#include <chrono>   /* Cálculo del tiempo de ejecucción */
#include <random>   /* Generación de números aleatorios */
#include <regex>    /* Modifica el nombre del archivo de salida para gráficas */

using namespace std;

/*
 * randomSeed: Devuelve un entero aleatorio utilizando rand.
 * Función necesaria para usar semillas en random_shuffle al permutar el orden de los datos de entrada.
 *
 * @i: int, semilla.
 *
 * return: número aleatorio.
 */
int randomSeed (int i) { return std::rand()%i;}

int SEED = 0; /* Semilla usada para la generación aleatoria */
int FOLDS = 5; /* Número de particiones para k-fold cross validation */
float REDUCCTION = 0.1f; /* Cota para la tasa de reducción */
bool EXPORT = false; /* Almacena la mejora de la agregación del algoritmo bl en un archivo txt */

/*
 * readfile: Lee el contenido de un archivo, elimina las líneas vacías o que comienzan 
 * por @, y almacena los datos en una matriz de strings usando vector.
 *
 * @fname: string, nombre del archivo a leer.
 *
 * return: vector<vector<string> >, matriz con el contenido del archivo leído.
 */
vector<vector<string> > readfile(string fname) {
	vector<vector<string> > content;
	vector<string> row;
	string line, word;
 
	fstream file(fname, ios::in);
	if(file.is_open()) {
		while(getline(file, line)) {
			row.clear();
 
 			if (line.size() > 1) {
				stringstream str(line);
	 
				while(getline(str, word, ',')) {
					// Eliminamos el carácter de salto de línea
					if ((word[word.size()-1] == '\r') || (word[word.size()-1] == '\n') ) {
						word.pop_back();
					}
					row.push_back(word);
				}
				content.push_back(row);
			}
		}

		// Eliminamos líneas vacías o las que empiezan por @
		for (int i=content.size()-1; i>=0; i--) {
			if (content[i][0][0] == '@' || content[i][0][0] == ' ')
				content.erase(content.begin()+i);
		}
	}
	else
		cout << "No puedo abrir el archivo\n";

	return content;
}

/*
 * printData: Imprime el contenido de una matriz por pantalla.
 *
 * @data: <vector<vector<T> >, matriz a imprimir por pantalla.
 * @size: int, número de filas a imprimir, por defecto se imprime la matriz entera.
 */
template <typename T>
void printData(vector<vector<T> > data, int size=0) {
	if (size == 0) {
		size = data.size();
	}
	for (int i=0; i<data.size() && i<size; i++) {
		cout << "[" << i << "] ";
		for (int j=0; j<data[i].size(); j++) {
			cout << data[i][j] << " ";
		}
		cout << "\n";
	}
}

/*
 * printData: Imprime el contenido de una matriz por pantalla.
 *
 * @x: <vector<vector<float> >, matriz a imprimir por pantalla.
 * @y: <vector<string>, vector de etiquetas.
 * @size: int, número de filas a imprimir, por defecto se imprime la matriz entera.
 */
void printData(vector<vector<float> > x, vector<string> y, int size=0) {
	if (size == 0) {
		size = x.size();
	}
	for (int i=0; i<x.size() && i<size; i++) {
		cout << "[" << i << "] ";
		for (int j=0; j<x[i].size(); j++) {
			cout << x[i][j] << " ";
		}
		cout << y[i] << "\n";
	}
}

/*
 * getData: Elimina la útlima columna (etiquetas) de una matriz, y transforma el 
 * tipo de los datos a float.
 *
 * NOTA: Usamos float porque los datos de las bases de datos que vamos a usar no tiene
 * demasiados decimales. En caso de que necesitasemos más precisión, podríamos usar double.
 *
 * @data: <vector<vector<string> >, matriz original.
 *
 * return: <vector<vector<float> >, matriz sin etiquetas.
 */
vector<vector<float> > getData(vector<vector<string> > data) {
	vector<vector<float> > x;

	for (int i=0; i<data.size(); i++) {
		vector<float> row;
		for (int j=0; j<data[i].size()-1; j++) {
			row.push_back(stof(data[i][j]));
		}
		x.push_back(row);
	}

	return x;
}

/*
 * getData: Devuelve la útlima columna (etiquetas) de una matriz.
 *
 * @data: <vector<vector<string> >, matriz original.
 *
 * return: <vector<string>, matriz de etiquetas.
 */
vector<string> getLabel(vector<vector<string> > data) {
	vector<string> y;
	int size = data[0].size();

	for (int i=0; i<data.size(); i++) {
		y.push_back(data[i][size-1]);
	}

	return y;
}

/*
 * normalizeData: Normaliza los tados de una matriz, transformando
 * los valores al intervalo [0,1].
 *
 * @data: <vector<vector<float> >, matriz original.
 *
 * return: <vector<vector<float> >, matriz normalizada.
 */
vector<vector<float> > normalizeData(vector<vector<float> > x) {
	for (int j=0; j<x[0].size(); j++) {
		float max = x[0][j];
		float min = x[0][j];
		for (int i=0; i<x.size(); i++) {
			if (x[i][j] > max)
				max = x[i][j];
			if (x[i][j] < min)
				min = x[i][j];
		}

		for (int i=0; i<x.size(); i++) {
			if (max != min)
				x[i][j] = (x[i][j] - min) / (max - min);
		}
	}

	return x;
}

/*
 * train_test_split: Divide una matriz de datos y etiquetas en partes casi iguales (si la division es 
 * exacta, todas tendrán el mismo tamaño, si no lo es, habrá algunas que tendrán un elemento extra).
 *
 * @x: vector<vector<float> >, matriz de datos original.
 * @y: vector<string>, vector de etiquetas original.
 * @x_train: vector<vector<float> >, matriz de datos de entrenamiento (se modificará).
 * @x_test: vector<vector<float> >, matriz de datos de test (se modificará).
 * @y_train: vector<string>, vector de etiquetas de entrenamiento (se modificará).
 * @y_test: vector<string>, vector de etiquetas de test (se modificará).
 */
void train_test_split(vector<vector<float> > x, vector<string>& y, vector<vector<float> >& x_train, vector<vector<float> >& x_test,
	vector<string>& y_train, vector<string>& y_test, int part, int parts) {
	int length = x.size() / parts;
	int remain = x.size() % parts;

	int begin = 0;
	int end = 0;

	if (part < remain) {
		begin = part * (length + 1);
		end = begin + (length + 1);
	} else if (part == remain) {
		begin = part * (length + 1);
		end = begin + length;
	} else {
		begin = part * length + remain;
		end = begin + length;
	}

	for (int i=0; i<x.size(); i++) {
		if (i < begin || i >= end) {
			x_train.push_back(x[i]);
			y_train.push_back(y[i]);
		} else {
			x_test.push_back(x[i]);
			y_test.push_back(y[i]);
		}
	}
}

/*
 * euclidean: Calcula la distancia euclídea entre dos puntos.
 *
 * @x0: vector<float>, primer punto.
 * @x1: vector<float>, segundo punto.
 *
 * return: float, distancia entre los dos puntos, o 0 si los puntos tienen
 * dimensiones distintas.
 */
float euclidean(vector<float> x0, vector<float> x1) {
	if (x0.size() != x1.size()) {
		cout << "Los vectores tienen tamaño distinto.\n";
		return 0.0f;
	} else {
		float dist = 0.0f;
		for (int i=0; i<x0.size(); i++) {
			dist += (x0[i] - x1[i]) * (x0[i] - x1[i]);
		}
		return (float) sqrt(dist);
	}
}

/*
 * euclidean: Calcula la distancia euclídea entre dos puntos, modifica por un vector de pesos.
 *
 * @x0: vector<float>, primer punto.
 * @x1: vector<float>, segundo punto.
 * w: vector<float>, vector de pesos.
 *
 * return: float, distancia entre los dos puntos, o 0 si los puntos tienen
 * dimensiones distintas.
 */
float euclidean(vector<float> x0, vector<float> x1, vector<float> w) {
	if (x0.size() != x1.size()) {
		cout << "Los vectores tienen tamaño distinto.\n";
		return 0.0f;
	} else {
		float dist = 0.0f;
		for (int i=0; i<x0.size(); i++) {
			dist += w[i] * (x0[i] - x1[i]) * (x0[i] - x1[i]);
		}
		return (float) sqrt(dist);
	}
}

/*
 * class_rate: Calcula la tasa de clasificación, es decir, el porcentaje de elementos
 * clasificados correctamente.
 *
 * @y: vector<string>, vector de etiquetas reales.
 * @y_pred: vector<string>, vector con la predicción de etiquetas.
 *
 * return: float, tasa de clasificación entre 0 y 1.
 */
float class_rate(vector<string> y, vector<string> y_pred) {
	if (y.size() != y_pred.size()) {
		cout << "Error al calcular la tasa de clasificación: El tamaño de los vectores no coincide.\n";
		return 0.0f;
	} else {
		int sucess = 0;
		for (int i=0; i<y.size(); i++) {
			if (y_pred[i] == y[i]) {
				sucess++;
			}
		}
		return (float) sucess/y.size();
	}
}

/*
 * reducc_rate: Calcula la tasa de reducción, es decir, el porcentaje de características que se
 * ignorán debido a su poco impacto en la clasificación.
 *
 * @w: vector<float>, vector de pesos de las características.
 *
 * return: float, tasa de reducción entre 0 y 1.
 */
float red_rate(vector<float> w, float min) {
	int reducc = 0;
	for (int i=0; i<w.size(); i++) {
		if (w[i] < min) {
			reducc++;
		}
	}

	return (float) reducc/w.size();
}

/*
 * permutation: Calcula una permutación para un vector.
 *
 * @size: int, tamaño del vector.
 *
 * return: vector<int>, vector con la nueva reordenación.
 */
vector<int> permutation(int size) {
	vector<int> perm;
	for (int i=0; i<size; i++) {
		perm.push_back(i);
	}
	random_shuffle(perm.begin(), perm.end());

	return perm;
}

/*
 * loo_prediction: Cálcula las etiquetas utilizando el algoritmo leave-one-out y un vector de pesos
 * que modifica la distancia euclídea.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas reales.
 * @w: vector<float>, vector de pesos.
 *
 * return: vector<string>, vector con la predicción de etiquetas.
 */
vector<string> loo_prediction(vector<vector<float> > x, vector<string> y, vector<float> w) {
	vector<int> neighbour;
	vector<float> min_dist;
	for (int i=0; i<y.size(); i++) {
		neighbour.push_back(0);
		min_dist.push_back(y.size());
	}

	for (int i=0; i<y.size(); i++) {
		for (int j=i+1; j<y.size(); j++) {
			float dist = euclidean(x[i], x[j], w);
			if (dist < min_dist[i]) {
				neighbour[i] = j;
				min_dist[i] = dist;
			}
			if (dist < min_dist[j]) {
				neighbour[j] = i;
				min_dist[j] = dist;
			}
		}
	}

	vector<string> y_pred;
	for (int i=0; i<y.size(); i++) {
		y_pred.push_back(y[neighbour[i]]);
	}

	return y_pred;
}

/*
 * localSearch: Algoritmo para calcular el vector de pesos.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas.
 *
 * return: vector<float>, vector de pesos.
 */
vector<float> localSearch(vector<vector<float> > x, vector<string> y) {
	vector<float> w;
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	// Inicializamos el vector de pesos con la distribución uniforme
	for (int i=0; i<x[0].size(); i++) {
		float rand = uniform_distribution(generator);
		if (rand < REDUCCTION) {
			w.push_back(0.0f);
		} else {
			w.push_back(rand);
		}
	}
	float agregation = class_rate(loo_prediction(x, y, w), y) + red_rate(w, REDUCCTION);

	// Comenzamos el algoritmo
	int iteration = 0;
	int iteration_mod = 0;

	while (iteration < 15000 && iteration_mod < 20) {
		bool modified = false;
		vector<int> perm = permutation(w.size());

		// Obtenemos un vecino
		for (int i=0; i<w.size() && !modified && iteration < 15000; i++) {
			vector<float> neighbour(w);
			neighbour[perm[i]] += normal_distribution(generator);

			if (neighbour[perm[i]] > 1.0f) {
				neighbour[perm[i]] = 1.0f;
			} else if (neighbour[perm[i]] < REDUCCTION) {
				neighbour[perm[i]] = 0.0f;
			}

			// Si el vecino es mejor que el actual, lo cambiamos
			float neighbour_agregation = class_rate(loo_prediction(x, y, neighbour), y) + red_rate(neighbour, REDUCCTION);
			if (neighbour_agregation > agregation) {
				modified = true;
				w = neighbour;
				agregation = neighbour_agregation;
			}

			iteration++;
		}

		if (modified) {
			iteration_mod = 0;
		} else {
			iteration_mod++;
		}
	}

	return w;
}

/*
 * export_localSeach: Algoritmo para calcular el vector de pesos.
 * Guarda la mejora de resultados y la iteración como coordenadas en un archivo txt para su representación.
 *
 * NOTA: Al escribir en un archivo, aumenta el tiempo de ejecución.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas.
 *
 * return: vector<float>, vector de pesos.
 */
vector<float> export_localSearch(vector<vector<float> > x, vector<string> y, string file, int fold) {
	// Abrimos o creamos (si no existe) el archivo para guardar los datos
	string filename = "graph/data/" + file + to_string(fold+1) + ".txt";
	filename.erase(6,5);
	regex target(".arff");
	filename = regex_replace(filename, target, "");
	ofstream myfile;
	myfile.open(filename, fstream::in | fstream::out | fstream::trunc);

	vector<float> w;
	default_random_engine generator(SEED);
	uniform_real_distribution<float> uniform_distribution(0.0, 1.0);
	normal_distribution<float> normal_distribution(0.0, 0.3);

	// Inicializamos el vector de pesos con la distribución uniforme
	for (int i=0; i<x[0].size(); i++) {
		float rand = uniform_distribution(generator);
		if (rand < REDUCCTION) {
			w.push_back(0.0f);
		} else {
			w.push_back(rand);
		}
	}
	float clas = class_rate(loo_prediction(x, y, w), y);
	float red = red_rate(w, REDUCCTION);
	float agregation = clas + red;

	myfile << "0," << to_string(clas) << "," << to_string(red) << "\n";

	// Comenzamos el algoritmo
	int iteration = 0;
	int iteration_mod = 0;

	while (iteration < 15000 && iteration_mod < 20) {
		bool modified = false;
		vector<int> perm = permutation(w.size());

		// Obtenemos un vecino
		for (int i=0; i<w.size() && !modified && iteration < 15000; i++) {
			vector<float> neighbour(w);
			neighbour[perm[i]] += normal_distribution(generator);

			if (neighbour[perm[i]] > 1.0f) {
				neighbour[perm[i]] = 1.0f;
			} else if (neighbour[perm[i]] < REDUCCTION) {
				neighbour[perm[i]] = 0.0f;
			}

			// Si el vecino es mejor que el actual, lo cambiamos
			clas = class_rate(loo_prediction(x, y, neighbour), y);
			red = red_rate(neighbour, REDUCCTION);
			float neighbour_agregation = clas + red;
			if (neighbour_agregation > agregation) {
				modified = true;
				w = neighbour;
				agregation = neighbour_agregation;
				myfile << to_string(iteration) << "," << to_string(clas) << "," << to_string(red) << "\n";

			}

			iteration++;
		}

		if (modified) {
			iteration_mod = 0;
		} else {
			iteration_mod++;
		}
	}

	myfile.close();

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
	cout << "Leída una matriz de tamaño (" << data.size() << "," << data[0].size() << ").\n";

	// Permutamos los elementos para tomarlos en orden aleatorio dependiendo de la semilla
	srand(SEED);
	random_shuffle(data.begin(), data.end(), randomSeed);

	// Separamos en datos y etiquetas.
	vector<vector<float> > x = getData(data);
	vector<string> y = getLabel(data);

	// Normalizamos los datos
	x = normalizeData(x);

	// K-fold cross validation
	for (int k=0; k<FOLDS; k++) {
		cout << "\n\tIteración " << k+1 << ":\n";

		// Separamos los datos en train y test
		vector<vector<float> > x_train, x_test;
		vector<string> y_train, y_test;

		train_test_split(x, y, x_train, x_test, y_train, y_test, k, FOLDS);
		cout << x_train.size() << " elementos de entrenamiento (" << (float) x_train.size()/x.size()*100 << "%), " 
			 << x_test.size() << " elementos de test (" << (float) x_test.size()/x.size()*100 << "%).\n";

		// Aplicamos el algoritmo localSearch
		vector<float> w;

		chrono::time_point<chrono::system_clock> start;
		chrono::time_point<chrono::system_clock> end;
		if (EXPORT) { // Guardando datos para representar en gráficas
			start = chrono::system_clock::now();
			w = export_localSearch(x_train, y_train, argv[1], k);
			end = chrono::system_clock::now();
		} else { // Sin guardar datos
			start = chrono::system_clock::now();
			w = localSearch(x_train, y_train);
			end = chrono::system_clock::now();
		}

		chrono::duration<double> elapsed_seconds = end - start;
  
		// Calculamos las predicciones en test
		vector<string> y_pred;
		for (int i=0; i<y_test.size(); i++) {
			float min_dist = y_test[0].size();
			int neighbour = 0;

			for (int j=0; j<x_train.size(); j++) {
				if (euclidean(x_test[i], x_train[j], w) < min_dist) {
					min_dist = euclidean(x_test[i], x_train[j], w);
					neighbour = j;
				}
			}
			y_pred.push_back(y_train[neighbour]);
		}

		// Calculamos la tasa de clasificación y de reducción
		float classification = class_rate(y_test, y_pred);
		float reduction = red_rate(w, REDUCCTION);

		cout << "Tasa de clasificación: " << classification * 100 << "%.\n";
		cout << "Tasa de reducción: " << reduction * 100 << "%.\n";
		cout << "Agregación: " << (classification + reduction) / 2 * 100 << ".\n";
		cout << "Tiempo de ejecución: " << elapsed_seconds.count() << " segundos.\n";
	}
	return 0;
}