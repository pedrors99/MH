#include <iostream> /* Salida de datos por pantalla */
#include <fstream>  /* Lectura de archivos */
#include <sstream>  /* Lectura de archivos */
#include <string>   /* Lectura de archivos */
#include <vector>   /* Almacenamiendo de datos en vectores */
#include <math.h>   /* Cálculo de raices */
#include <stdlib.h> /* Generación aleatoria */
#include <chrono>   /* Cálculo del tiempo de ejecucción */

using namespace std;

int SEED = 0; /* Semilla usada para la generación aleatoria */

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
 * k_nn: Algoritmo 1-Nearest Neighbours, que predice una etiqueta asignando la misma que
 * a su vecino más cercano (leave one out).
 *
 * @x: vector<vector<float> >, matriz de datos.
 * @y: vector<string>, vector de etiquetas.
 *
 * return: vector<string>, vector con la predicción de etiquetas.
 */
vector<string> k_nn(vector<vector<float> > x, vector<string> y) {
	vector<int> neighbour;
	vector<float> min_dist;
	for (int i=0; i<y.size(); i++) {
		neighbour.push_back(0);
		min_dist.push_back(y.size());
	}

	for (int i=0; i<y.size(); i++) {
		for (int j=i+1; j<y.size(); j++) {
			float dist = euclidean(x[i], x[j]);
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

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	vector<string> y_pred = k_nn(x, y);
	chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;
  

	// Calculamos la tasa de clasificación
	float classification = class_rate(y, y_pred);

	cout << "Tasa de clasificación: " << classification * 100 << "%.\n";
	cout << "Tasa de reducción: " << 0.0f << "%.\n";
	cout << "Agregación: " << classification * 50 << ".\n";
	cout << "Tiempo de ejecución: " << elapsed_seconds.count() << " segundos.\n";

	return 0;
}