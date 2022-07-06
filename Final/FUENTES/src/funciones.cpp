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
 * average: Calcula la media de los elementos de un vector.
 *
 * @array: vector<float>, array del cual calculamos la media.
 *
 * return: float, media de los elementos.
 */
float average(vector<float> array) {
	float sum = 0;
	for (int i=0; i<array.size(); i++) {
		sum += array[i];
	}
	return sum / array.size();
}

/* ----- FUNCIONES DE ENTRADA Y SALIDA ----- */

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
 * printElement: Imprime un elemento en formato de tabla.
 *
 * @t: template, valor a imprimir por pantalla.
 * @width: int, tamaño de la columna
 */
template<typename T>
void printElement(T t, const int& width) {
	char separator = ' ';
    cout << left << setw(width) << setfill(separator) << t;
}

/*
 * printData: Imprime el contenido de un array por pantalla.
 *
 * @data: vector<vector<T> >, array a imprimir por pantalla.
 * @size: int, número de elementos a imprimir, por defecto se imprime el array entero.
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
 * @x: vector<vector<float> >, matriz a imprimir por pantalla.
 * @y: vector<string>, vector de etiquetas.
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
 * printData: Imprime el contenido de una matriz por pantalla.
 *
 * @array: vector<T>, matriz a imprimir por pantalla.
 * @y: <vector<string>, vector de etiquetas.
 * @size: int, número de filas a imprimir, por defecto se imprime la matriz entera.
 */
template <typename T>
void printArray(vector<T> array, int size=0) {
	if (size == 0) {
		size = array.size();
	}
	for (int i=0; i<size; i++) {
		cout << array[i];
		if (i < size - 1) {
			cout << ", ";
		}
	}
}

/*
 * printTable: Imprime los resultados de los algoritmos en forma de tabla.
 *
 * @clas: vector<float>, vector con las tasas de clasificación.
 * @red: vector<float>, vector con las tasas de reducción.
 * @agre: vector<float>, vector con las agregaciones.
 * @time: vector<float>, vector con los tiempos de ejecución.
 */
void printTable(vector<float> clas, vector<float> red, vector<float> agre, vector<float> time) {
	cout << fixed;
	cout << setprecision(2);
	printElement(" ", 14);
	printElement("%_class", 10);
	printElement("%_red", 10);
	printElement("Agre.", 10);
	printElement("T", 10);
	cout << endl;

    for (int i=0; i<clas.size(); i++) {
    	printElement("Partición " + to_string(i+1), 15);
    	printElement(clas[i], 10);
    	printElement(red[i], 10);
    	printElement(agre[i], 10);
    	printElement(time[i], 10);
    	cout << endl;
    }

    printElement("Media", 14);
    printElement(average(clas), 10);
    printElement(average(red), 10);
    printElement(average(agre), 10);
    printElement(average(time), 10);
    cout << endl;
}

/*
 * latexExport: Imprime los resultados con un formato más cómodo de importar en una tabla de latex.
 *
 * @clas: vector<float>, vector con las tasas de clasificación.
 * @red: vector<float>, vector con las tasas de reducción.
 * @agre: vector<float>, vector con las agregaciones.
 * @time: vector<float>, vector con los tiempos de ejecución.
 */
void latexExport(vector<float> clas, vector<float> red, vector<float> agre, vector<float> time) {
	cout << fixed;
	cout << setprecision(2);
	for (int i=0; i<clas.size(); i++) {
		cout << " & " << clas[i] << " & " << red[i] << " & " << agre[i] << " & " << time[i] << endl;
	}
	cout << " & " << average(clas) << " & " << average(red) << " & " << average(agre) << " & " << average(time) << endl;
}

/* ----- FUNCIONES DE PROCESADO DE DATOS ----- */

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

/* ----- FUNCIONES DE DISTANCIA ----- */

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
float euclidean(vector<float> x0, vector<float> x1, vector<float> w, float reduction) {
	if (x0.size() != x1.size()) {
		cout << "Los vectores tienen tamaño distinto.\n";
		return 0.0f;
	} else {
		float dist = 0.0f;
		for (int i=0; i<x0.size(); i++) {
			if (w[i] > reduction) {
				dist += w[i] * (x0[i] - x1[i]) * (x0[i] - x1[i]);
			}
		}
		return (float) sqrt(dist);
	}
}

/* ----- FUNCIONES DE EVALUACIÓN ----- */

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
 * predictLabel: predice las etiquetas de una matriz de características a partir de la más cercana de otra
 * matriz, utilizando la distancia euclída modificada por un vector de pesos.
 *
 * x1: vector<vector<float> >, matriz de características cuyas etiquetas queremos predecir.
 * x2: vector<vector<float> >, matriz de características que utilizamos para predecir las etiquetas de x1.
 * y: vector<string>, vector de etiquetas de la matriz x2.
 * w: vector<float>, vector de pesos.
 * reduction: float, valor por debajo del cual los pesos se consideran 0.
 *
 * return: vector<string>, vector de etiquetas predichas.
 */
vector<string> predictLabel(vector<vector<float> > x1, vector<vector<float> > x2, vector<string> y, vector<float> w, float reduction) {
	vector<string> y_pred;

	for (int i=0; i<x1.size(); i++) {
		float minDist = euclidean(x1[i], x2[0], w, reduction);
		int neighbour = 0;

		for (int j=1; j<x2.size(); j++) {
			float dist = euclidean(x1[i], x2[j], w, reduction);
			if (dist < minDist) {
				minDist = dist;
				neighbour = j;
			}
		}

		y_pred.push_back(y[neighbour]);
	}
	return y_pred;
}

/*
 * predictLabelLoo: predice las etiquetas de una matriz de características utilizando el vecino más cercano
 * y leave-one-out.
 *
 * x: vector<vector<float> >, matriz de características cuyas etiquetas queremos predecir.
 * y: vector<string>, vector de etiquetas reales de la matriz.
 * w: vector<float>, vector de pesos.
 * reduction: float, valor por debajo del cual los pesos se consideran 0.
 *
 * return: vector<string>, vector de etiquetas predichas.
 */
vector<string> predictLabelLoo(vector<vector<float> > x, vector<string> y, vector<float> w, float reduction) {
	vector<int> neighbour;
	vector<float> min_dist;
	for (int i=0; i<y.size(); i++) {
		neighbour.push_back(0);
		min_dist.push_back(y.size());
	}

	for (int i=0; i<y.size(); i++) {
		for (int j=i+1; j<y.size(); j++) {
			float dist = euclidean(x[i], x[j], w, reduction);
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
 * agregation: Calcula la agregación, media de la tasa de reducción y de clasificación.
 *
 * @y: vector<string>, vector con las etiquetas reales.
 * @y_pred: vector<string>, vector con las etiquetas predecidas.
 * @w: vector<float>, vector de pesos de las características.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @clas: float, tasa de clasificación, a modificar.
 * @red: float, tasa de reducción, a modificar.
 * @output: bool, determina si imprime o no por pantalla.
 *
 * return: float, agregación entre 0 y 1.
 */
float agregation(vector<string> y, vector<string> y_pred, vector<float> w, float reduction, float &clas, float &red, bool output=false) {
	clas = class_rate(y, y_pred);
	red = red_rate(w, reduction);

	if (red == 1.0f) {
		clas = 0.0f;
	}

	if (output) {
		cout << "Tasa de clasificación: " << clas * 100 << "%.\n";
		cout << "Tasa de reducción: " << red * 100 << "%.\n";
		cout << "Agregación: " << clas * 50 + red * 50 << ".\n";
	}

	return (clas + red) / 2;
}

/*
 * agregation: Calcula la agregación, media de la tasa de reducción y de clasificación.
 *
 * @y: vector<vector<float> >, matriz con las características.
 * @y: vector<string>, vector con las etiquetas.
 * @w: vector<float>, vector de pesos de las características.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @output: bool, determina si imprime o no por pantalla.
 *
 * return: float, agregación entre 0 y 1.
 */
float agregation(vector<vector<float> > x, vector<string> y, vector<float> w, float reduction, bool output=false) {
	vector<string> y_pred = predictLabelLoo(x, y, w, reduction);
	float clas, red;
	return agregation(y, y_pred, w, reduction, clas, red, output);
}

/*
 * localSearch: Algoritmo de búsqueda local.
 *
 * @y: vector<vector<float> >, matriz con las características.
 * @y: vector<string>, vector con las etiquetas.
 * @w: vector<float>, vector de pesos de las características.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @eval: int, número de evaluaciones realizadas, se modifica.
 * @evaluations: int, número máximo de evaluaciones a realizar.
 * @maxNoChange: int, número máximo de iteraciones sin realizar ninguna modificación al vector de pesos.
 * @dim: int, dimensión de cada vector.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 *
 * return: vector<float>, vector de pesos.
 */
vector<float> localSearch(vector<vector<float> > x, vector<string> y, vector<float> w, float reduction, int &eval, int evaluations, int maxNoChange, default_random_engine generator, normal_distribution<float> distribution) {
	float fitness = agregation(x, y, w, reduction);
	int noChange = 0;

	while (eval < evaluations && noChange < maxNoChange) {
		vector<int> perm = permutation(x[0].size());
		bool loop = true;

		for (int i=0; i<perm.size() && eval<evaluations && loop; i++) {
			vector<float> neighbour(w);

			neighbour[perm[i]] += distribution(generator);
			if (neighbour[perm[i]] < reduction) {
				neighbour[perm[i]] = 0.0f;
			} else if (neighbour[perm[i]] > 1.0f) {
				neighbour[perm[i]] = 1.0f;
			}

			float agre = agregation(x, y, neighbour, reduction);
			eval++;
			if (agre > fitness) {
				fitness = agre;
				w = neighbour;
				loop = false;
				noChange = 0;
			}
		}
		noChange++;
	}
	return w;
}

/*
 * exportLocalSearch: Algoritmo de búsqueda local que exporta los resultados a un archivo.
 *
 * @y: vector<vector<float> >, matriz con las características.
 * @y: vector<string>, vector con las etiquetas.
 * @w: vector<float>, vector de pesos de las características.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @eval: int, número de evaluaciones realizadas, se modifica.
 * @evaluations: int, número máximo de evaluaciones a realizar.
 * @maxNoChange: int, número máximo de iteraciones sin realizar ninguna modificación al vector de pesos.
 * @dim: int, dimensión de cada vector.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 * @myfile: ofstream, archivo al cuál exportar los resultados.
 *
 * return: vector<float>, vector de pesos.
 */
vector<float> exportLocalSearch(vector<vector<float> > x, vector<string> y, vector<float> w, float reduction, int &eval, int evaluations, int maxNoChange, default_random_engine generator, normal_distribution<float> distribution, ofstream& myfile) {
	float fitness = agregation(x, y, w, reduction);
	int noChange = 0;

	myfile << eval << "," << fitness << endl;

	while (eval < evaluations && noChange < maxNoChange) {
		vector<int> perm = permutation(x[0].size());
		bool loop = true;

		for (int i=0; i<perm.size() && eval<evaluations && loop; i++) {
			vector<float> neighbour(w);

			neighbour[perm[i]] += distribution(generator);
			if (neighbour[perm[i]] < reduction) {
				neighbour[perm[i]] = 0.0f;
			} else if (neighbour[perm[i]] > 1.0f) {
				neighbour[perm[i]] = 1.0f;
			}

			float agre = agregation(x, y, neighbour, reduction);
			eval++;
			if (agre > fitness) {
				fitness = agre;
				w = neighbour;
				loop = false;
				noChange = 0;

				myfile << eval << "," << fitness << endl;
			}
		}
		noChange++;
	}
	return w;
}

/* ----- FUNCIONES DE ALGORTIMOS GENÉTICOS ----- */

/*
 * initialize: Inicializa una vector con valores de una distribución uniforme en [0,1].
 *
 * @size: int, dimensión del vector.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: uniform_real_distribution, para la generación de la distribución uniforme.
 *
 * return: vector<float>, vector inicializado.
 */
vector<float> initialize(int size, float reduction, default_random_engine &generator, uniform_real_distribution<float> &distribution) {
	vector<float> w;

	for (int i=0; i<size; i++) {
		float gene = distribution(generator);
		if (gene < reduction) {
			gene = 0.0f;
		}
		w.push_back(gene);
	}

	return w;
}

/*
 * selection: Algoritmo de selección.
 *
 * @solutions: vector<vector<float> >, matriz de soluciones (padres).
 * @fitness: vector<float>, vector con la agregación de cada solución.
 * @size: int, número de soluciones a devolver que actuarán como padres. Serán el total de soluciones
 * para el generacional, y dos para el estacionario.
 *
 * return: vector<vector<float> >, matriz inicializada.
 */
vector<vector<float> > selection(vector<vector<float> > solutions, vector<float> fitness, int size) {
	vector<vector<float> > parents;
	for (int i=0; i<size; i++) {
		int random1 = rand() % solutions.size();
		int random2 = rand() % solutions.size();

		if (fitness[random1] > fitness[random2]) {
			parents.push_back(solutions[random1]);
		} else {
			parents.push_back(solutions[random2]);
		}
	}
	return parents;
}

/*
 * crossBlx: Algoritmo de cruce utilizando Blx.
 *
 * @parents: vector<vector<float> >, matriz de soluciones (padres).
 * @crossChance: float, probabilidad de cruce.
 * @alpha: float, utilizado por Blx para tederminar el tamaño del intervalo al que pretenecen las soluciones generadas.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: uniform_real_distribution, para la generación de la distribución uniforme.
 *
 * return: vector<vector<float> >, matriz de soluciones tras el cruce.
 */
vector<vector<float> > crossBlx(vector<vector<float> > solutions, float crossChance, float alpha, float reduction, default_random_engine generator, uniform_real_distribution<float> distribution) {
	int crosses = crossChance * solutions.size() / 2;
	vector<vector<float> > newSolutions;

	for (int i=0; i<crosses; i++) {
		int parent1 = 2 * i;
		int parent2 = (2 * i + 1) % solutions.size();

		for (int j=0; j<2; j++) {
			vector<float> w;
			for (int k=0; k<solutions[parent1].size(); k++) {
				float min;
				float max;
				if (solutions[parent1][k] < solutions[parent2][k]) {
					min = solutions[parent1][k];
					max = solutions[parent2][k];
				} else {
					min = solutions[parent2][k];
					max = solutions[parent1][k];
				}
				min -= alpha * (max - min);
				max += alpha * (max - min);
				float gene = min + distribution(generator) * (max - min);
				if (gene < reduction) {
					gene = 0.0f;
				} else if (gene > 1.0f) {
					gene = 1.0f;
				}
				w.push_back(gene);
			}
			newSolutions.push_back(w);
		}
	}

	for (int i=2*crosses; i<solutions.size(); i++) {
		newSolutions.push_back(solutions[i]);
	}

	return newSolutions;
}

/*
 * crossBlx: Algoritmo de cruce utilizando cruce aritmético.
 *
 * @parents: vector<vector<float> >, matriz de soluciones (padres).
 * @crossChance: float, probabilidad de cruce.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: uniform_real_distribution, para la generación de la distribución uniforme.
 *
 * return: vector<vector<float> >, matriz de soluciones tras el cruce.
 */
vector<vector<float> > crossCa(vector<vector<float> > parents, float crossChance, float reduction, default_random_engine generator, uniform_real_distribution<float> distribution) {
	int crosses = crossChance * parents.size() / 2;
	vector<vector<float> > newSolutions;

	for (int i=0; i<crosses; i++) {
		int parent1 = 2 * i;
		int parent2 = (2 * i + 1) % parents.size();
		float alpha = distribution(generator);

		for (int j=0; j<2; j++) {
			vector<float> w;

			if (j == 1) {
				alpha = 1 - alpha;
			}

			for (int k=0; k<parents[parent1].size(); k++) {
				float gene = alpha * parents[parent1][k] + (1 - alpha) * parents[parent2][k];
				if (gene < reduction) {
					gene = 0.0f;
				}
				w.push_back(gene);
			}
			newSolutions.push_back(w);
		}
	}

	for (int i=2*crosses; i<parents.size(); i++) {
		newSolutions.push_back(parents[i]);
	}

	return newSolutions;
}

/*
 * mutations: Algoritmo de mutación.
 *
 * @parents: vector<vector<float> >, matriz de soluciones (hijos).
 * @mutationChance: float, probabilidad de mutación.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 *
 * return: vector<vector<float> >, matriz de soluciones tras la mutación.
 */
vector<vector<float> > mutations(vector<vector<float> > parents, float mutationChance, float reduction, default_random_engine generator, normal_distribution<float> distribution) {
	int mutations = mutationChance * parents.size() * parents[0].size();
	vector<int> perm = permutation(parents.size() * parents[0].size());
	for (int i=0; i<mutations; i++) {
		int chromosome = perm[i] / parents[0].size();
		int gene = perm[i] % parents[0].size();

		parents[chromosome][gene] += distribution(generator);
		if (parents[chromosome][gene] < reduction) {
			parents[chromosome][gene] = 0.0f;
		} else if (parents[chromosome][gene] > 1.0f) {
			parents[chromosome][gene] = 1.0f;
		}
	}

	return parents;
}

/*
 * replaceGenerational: Algoritmo de reemplazo de soluciones en el algoritmo genético generacional.
 *
 * @solutions: vector<vector<float> >, matriz de soluciones de la generación anterior.
 * @newSolutions: vector<vector<float> >, matriz con las soluciones generadas en esta generación.
 * @fitness: vector<float>, vector de las agregaciones de las soluciones.
 * @x: vector<vector<float> >, matriz con los valores.
 * @y: vector<string>, vector con las etiquetas.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @eval: int, número de evaluaciones realizadas.
 *
 * return: vector<vector<float> >, matriz de soluciones tras la mutación.
 */
vector<vector<float> > replaceGenerational(vector<vector<float> > solutions, vector<vector<float> > newSolutions, vector<float> &fitness, vector<vector<float> > x, vector<string> y, float reduction, int &eval) {
	// Elitista
	vector<float> elitist = solutions[0];
	float elitistFitness = fitness[0];
	for (int i=1; i<solutions.size(); i++) {
		if (fitness[i] > elitistFitness) {
			elitistFitness = fitness[i];
			elitist = solutions[i];
		}
	}

	// Reemplazo
	solutions = newSolutions;
	for (int i=0; i<solutions.size(); i++) {
		fitness[i] = agregation(x, y, solutions[i], reduction);
		eval++;
	}

	// Elitista
	int minPos = 0;
	float minFitness = fitness[0];
	for (int i=1; i<solutions.size(); i++) {
		if (fitness[i] < minFitness) {
			minFitness = fitness[i];
			minPos = i;
		}
	}

	if (elitistFitness > fitness[minPos]) {
		solutions[minPos] = elitist;
		fitness[minPos] = elitistFitness;
	}

	return solutions;
}

/*
 * replaceStationary: Algoritmo de reemplazo de soluciones en el algoritmo genético estacionario.
 *
 * @solutions: vector<vector<float> >, matriz de soluciones de la generación anterior.
 * @newSolutions: vector<vector<float> >, matriz con las dos soluciones generadas en esta generación.
 * @fitness: vector<float>, vector de las agregaciones de las soluciones.
 * @x: vector<vector<float> >, matriz con los valores.
 * @y: vector<string>, vector con las etiquetas.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @eval: int, número de evaluaciones realizadas.
 *
 * return: vector<vector<float> >, matriz de soluciones tras la mutación.
 */
vector<vector<float> > replaceStationary(vector<vector<float> > solutions, vector<vector<float> > newSolutions, vector<float> &fitness, vector<vector<float> > x, vector<string> y, float reduction, int &eval) {
	// Búscamos los dos elementos con menor fitness
	int minPos1 = 0;
	int minPos2 = 1;
	if (fitness[minPos2] < fitness[minPos1]) { // fitness(minPos1) < fitness(minPos2)
		minPos1 = 1;
		minPos2 = 0;
	}
	for (int i=2; i<solutions.size(); i++) {
		if (fitness[i] < fitness[minPos1]) {
			minPos2 = minPos1;
			minPos1 = i;
		} else if (fitness[i] < fitness[minPos2]) {
			minPos2 = i;
		}
	}

	// Calculamos el fitness de los hijos
	float agregation1 = agregation(x, y, newSolutions[0], reduction);
	float agregation2 = agregation(x, y, newSolutions[1], reduction);
	eval += 2;

	// Sustituimos sin son mayores
	if (agregation1 > agregation2) {
		if (agregation1 > fitness[minPos2]) {
			solutions[minPos2] = newSolutions[0];
			fitness[minPos2] = agregation1;
			if (agregation2 > fitness[minPos1]) {
				solutions[minPos1] = newSolutions[1];
				fitness[minPos1] = agregation2;
			}
		} else if (agregation1 > fitness[minPos1]) {
			solutions[minPos1] = newSolutions[0];
			fitness[minPos1] = agregation1;
		}
	} else {
		if (agregation2 > fitness[minPos2]) {
			solutions[minPos2] = newSolutions[1];
			fitness[minPos2] = agregation2;
			if (agregation1 > fitness[minPos1]) {
				solutions[minPos1] = newSolutions[0];
				fitness[minPos1] = agregation1;
			}
		} else if (agregation2 > fitness[minPos1]) {
			solutions[minPos1] = newSolutions[1];
			fitness[minPos1] = agregation2;
		}
	}

	return solutions;
}

/* ----- FUNCIONES DE ALGORITMOS MEMÉTICOS ----- */

/*
 * memeticLocalSearch: Algoritmo de búsqueda local para los algoritmos genéticos.
 *
 * @x: vector<vector<float> >, matriz con los valores.
 * @y: vector<string>, vector con las etiquetas.
 * @w: vector<float>, vector solución.
 * @iterations: int, número máximo de evaluaciones de la búsqueda local.
 * @fitness: float, agregación de la solución.
 * @eval: int, número de evaluaciones realizadas.
 * @maxEval: int, número máximo de evaluaciones.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 *
 * return: vector<float>, vector de pesos de la solución tras la búsqueda local.
 */
vector<float> memeticLocalSearch(vector<vector<float> > x, vector<string> y, vector<float> w, int iterations, float &fitness, int &eval, int maxEval, float reduction, default_random_engine generator, normal_distribution<float> distribution) {
	int iter = 0;
	while (iter < iterations && eval < maxEval) {
		vector<int> perm = permutation(x[0].size());
		bool loop = true;

		for (int i=0; i<perm.size() && iter<iterations && eval<maxEval && loop; i++) {
			vector<float> neighbour(w);

			neighbour[perm[i]] += distribution(generator);
			if (neighbour[perm[i]] < reduction) {
				neighbour[perm[i]] = 0.0f;
			} else if (neighbour[perm[i]] > 1.0f) {
				neighbour[perm[i]] = 1.0f;
			}

			float agre = agregation(x, y, neighbour, reduction);
			eval++;
			if (agre > fitness) {
				w = neighbour;
				fitness = agre;
				loop = false;
			}

			iter++;
		}
	}

	return w;
}

/*
 * exportMemeticLocalSearch: Algoritmo de búsqueda local para los algoritmos genéticos.
 *
 * @x: vector<vector<float> >, matriz con los valores.
 * @y: vector<string>, vector con las etiquetas.
 * @w: vector<float>, vector solución.
 * @iterations: int, número máximo de evaluaciones de la búsqueda local.
 * @fitness: float, agregación de la solución.
 * @eval: int, número de evaluaciones realizadas.
 * @maxEval: int, número máximo de evaluaciones.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 * @myfile: ofstream, archivo al cuál exportar los resultados.
 *
 * return: vector<float>, vector de pesos de la solución tras la búsqueda local.
 */
vector<float> exportMemeticLocalSearch(vector<vector<float> > x, vector<string> y, vector<float> w, int iterations, float &fitness, int &eval, int maxEval, float reduction, default_random_engine generator, normal_distribution<float> distribution, ofstream& myfile) {
	int iter = 0;
	while (iter < iterations && eval < maxEval) {
		vector<int> perm = permutation(x[0].size());
		bool loop = true;

		for (int i=0; i<perm.size() && iter<iterations && eval<maxEval && loop; i++) {
			vector<float> neighbour(w);

			neighbour[perm[i]] += distribution(generator);
			if (neighbour[perm[i]] < reduction) {
				neighbour[perm[i]] = 0.0f;
			} else if (neighbour[perm[i]] > 1.0f) {
				neighbour[perm[i]] = 1.0f;
			}

			float agre = agregation(x, y, neighbour, reduction);
			eval++;
			if (agre > fitness) {
				w = neighbour;
				fitness = agre;
				loop = false;

				myfile << eval << "," << fitness << endl;
			}

			iter++;
		}
	}

	return w;
}

/*
 * sortInd: devuelve la posición de los elementos de mayor valor de un vector.
 *
 * @fitness, vector<float>, vector de agregación.
 * @size: int, número de elementos a devolver.
 *
 * return: vector<int>, vector con la posición de los elementos de mayor agregación.
 */
vector<int> sortInd(vector<float> fitness, int size) {
	vector<int> ind;
	for (int i=0; i<size; i++) {
		int bestPos = 0;
		float bestFitness = 0;

		for (int j=0; j<fitness.size(); j++) {
			if (fitness[j] > bestFitness) {
				bool contained = false;
				for (int k=0; k<ind.size() && !contained; k++) {
					if (ind[k] == j) {
						contained = true;
					}
				}

				if (!contained) {
					bestFitness = fitness[j];
					bestPos = j;
				}
			}
		}
		ind.push_back(bestPos);
	}

	return ind;
}

/* ----- FUNCIONES PARA ALGORITTMOS POR TRAYECTORIAS ----- */

/*
 * mutation: Algoritmo de mutación.
 *
 * @w: vector<float>, vector de pesos solución.
 * @mutationChance: float, probabilidad de mutación.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 *
 * return: vector<float>, vector con la solución tras la mutación.
 */
vector<float> mutation(vector<float> w, float mutationChance, float reduction, default_random_engine generator, normal_distribution<float> distribution) {
	int mutations = w.size() * mutationChance;

	vector<int> perm = permutation(w.size());
	for (int i=0; i<mutations; i++) {
		w[perm[i]] += distribution(generator);

		if (w[perm[i]] < reduction) {
			w[perm[i]] = 0.0f;
		} else if (w[perm[i]] > 1.0f) {
			w[perm[i]] = 1.0f;
		}
	}

	return w;
}

/*
 * mutation: Algoritmo de mutación que modifica un único componente del vector.
 *
 * @w: vector<float>, vector de pesos solución.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @generator: default_random_engine, generador aleatorio.
 * @distribution: normal_distribution, para la generación de la distribución normal.
 *
 * return: vector<float>, vector con la solución tras la mutación.
 */
vector<float> mutation(vector<float> w, float reduction, default_random_engine generator, normal_distribution<float> distribution) {
	int pos = permutation(w.size())[0];
	
	w[pos] += distribution(generator);

	if (w[pos] < reduction) {
		w[pos] = 0.0f;
	} else if (w[pos] > 1.0f) {
		w[pos] = 1.0f;
	}

	return w;
}

/*
 * es: Algoritmo de enfriamiento simulado.
 *
 * @w: vector<float>, vector de pesos solución.
 * @x: vector<vector<float> >, matriz con los valores.
 * @y: vector<string>, vector con las etiquetas.
 * @reduction: float, valor debajo del cual los pesos se consideran como 0.
 * @mu: float, variable utilizada para calcular la temperatura inicial.
 * @phi: float, variable utilizada para calcular la temperatura inicial.
 * @generator: default_random_engine, generador aleatorio.
 * @normal_distribution: normal_distribution, para la generación de la distribución normal.
 * @uniform_distribution: uniform_distribution, para la generación de la distribución uniforme.
 *
 * return: vector<float>, vector de pesos solución.
 */
vector<float> es(vector<float> w, vector<vector<float> > x, vector<string> y, float reduction, float mu, float phi, int &eval, int evaluations, default_random_engine generator, normal_distribution<float> normal_distribution, uniform_real_distribution<float> uniform_distribution) {
	float fitness = agregation(x, y, w, reduction);
	eval++;

	int exitos = 1;
	int maxNeighbours = 10 * x[0].size();
	int maxExitos = 0.1 * maxNeighbours;
	float m = evaluations / maxNeighbours;

	float bestFitness = fitness;
	vector<float> bestW = w;

	float t = -mu * fitness / log(phi);
	float tFinal = 0.001f;

	// La temperatura final tiene que ser menor que la inicial.
	while (t < tFinal) {
		tFinal /= 10;
	}

	float beta = (t - tFinal) / (m * t * tFinal);

	while (exitos > 0 && eval < evaluations) {
		exitos = 0;
		int neighbours = 0;

		while (neighbours < maxNeighbours && exitos < maxExitos && eval < evaluations) {
			vector<float> newW = mutation(w, reduction, generator, normal_distribution);
			neighbours++;

			float agre = agregation(x, y, newW, reduction);
			eval++;

			float delta = agre - fitness;
			float x = uniform_distribution(generator);

			if (delta > 0 || x <= exp(delta/t)) {
				w = newW;
				fitness = agre;
				exitos++;

				if (fitness > bestFitness) {
					bestFitness = fitness;
					bestW = w;
				}
			}
		}
		t = t / (1 + beta * t);
	}
	return bestW;
}

