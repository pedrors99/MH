#include <iostream>  /* Salida de datos por pantalla */
#include <fstream>   /* Lectura de archivos */
#include <sstream>  /* Lectura de archivos */
#include <string>    /* Lectura de archivos */
#include <vector>    /* Almacenamiendo de datos en vectores */
#include <math.h>    /* Cálculo de raices */
#include <stdlib.h>  /* Generación aleatoria */
#include <chrono>    /* Cálculo del tiempo de ejecucción */

using namespace std;

int SEED = 0; /* Semilla usada para la generación aleatoria */
int FOLDS = 5; /* Número de particiones para k-fold cross validation */
float REDUCCTION = 0.1f; /* Cota para la tasa de reducción */

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
 
	fstream file (fname, ios::in);
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
 * relief: Algoritmo para calcular el vector de pesos.
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas.
 *
 * return: vector<float>, vector de pesos.
 */
vector<float> relief(vector<vector<float> > x, vector<string> y) {
	vector<float> w;
	for (int i=0; i<x[0].size(); i++) {
		w.push_back(0.0f);
	}

	for (int i=0; i<x.size(); i++) {
		int ind_enemy = i;
		int ind_friend = i;

		float dist_enemy = x[0].size();
		float dist_friend = x[0].size();

		// Buscamos el amigo y el enemigo más cercano
		for (int j=0; j<x.size(); j++) {
			if (i != j) {
				if (y[i] == y[j] && euclidean(x[i], x[j]) < dist_friend) {
					ind_friend = j;
					dist_friend = euclidean(x[i], x[j]);
				} else if (y[i] != y[j] && euclidean(x[i], x[j]) < dist_enemy) {
					ind_enemy = j;
					dist_enemy = euclidean(x[i], x[j]);
				}
			}
		}

		// Actualizamos el vector de pesos
		for (int j=0; j<x[i].size(); j++) {
			w[j] += abs(x[i][j] - x[ind_enemy][j]) - abs(x[i][j] - x[ind_friend][j]);
		}
	}

	float max_w = w[0];
	for (int i=0; i<w.size(); i++) {
		if (w[i] > max_w) {
			max_w = w[i];
		}
	}

	for (int i=0; i<w.size(); i++) {
		if (w[i] < REDUCCTION) {
			w[i] = 0;
		} else {
			w[i] = w[i] / max_w;
		}
	}

	return w;
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
float red_rate(vector<float> w, float reduction) {
	int reducc = 0;
		for (int i=0; i<w.size(); i++) {
			if (w[i] < reduction) {
				reducc++;
			}
		}

	return (float) reducc/w.size();
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

		// Aplicamos el algoritmo RELIEF
		vector<float> w;

		chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
		w = relief(x_train, y_train);
		chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();

		chrono::duration<double> elapsed_seconds = end - start;
  

		// Calculamos las predicciones
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