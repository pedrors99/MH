Para una explicación más en detalle, consultar la sección 5 del pdf.

Estructura de carpetas:
	· bin: Contiene los archivos ejecutables que se crean trás la compilación.
	· data: Contiene los conjuntos de datos.
	· src: Contiene el código fuente.
	· graph: Contiene los datos necesarios para representar gráficas del algoritmo localSearch.

Los archivos ejecutables tienen el mismo nombre que sus archivos de código fuente correspondientes, excepto por la extensión.
Estos son:
	· knn: Algoritmo 1-nn (1 Nearest Neighbour).
	· greedy: Algoritmo greedy o relief.
	· localSearch: Algoritmo de búsqueda local.

Todos los archivos utilizan las siguientes constantes (que se pueden modificar en las primeras líneas de código):
	· SEED: Semilla usada para la aleatoriedad, 0 por defecto.
Además, los algoritmos greedy y localSearch tiene las constantes:
	· FOLDS: Número de particiones para el algoritmo N-fold corss validation, 5 por defecto (4 de train, 1 de test).
	· REDUCCTION: Umbral para la tsa de reducción. Se consideran como 0 los pesos por debajo de dicho valor (0.1 por defecto).
El algoritmo localSearch tiene otra constante:
	· EXPORT: Determina si almacena los datos en un fichero para su posterior representación.

Todas las funciones utilizadas contienen una breve descripción previa tanto de su funcionamiento, como de los parámetros de entrada y salida.

Para compilar, sólo es necesario ejecutar make. Esta compilación se realiza utilizando optimización -O2.
Para eliminar los ejecutables, make clean.
Para eliminar los ejecutables y los ficheros asociados a las gráficas, make fullclean.

Para ejecutar un archivo, hay que ejecutarlo seguido del fichero de datos a utilizar. Por ejemplo, si queremos aplicar el algoritmo localSearch a los datos de "ionosphere.arff", sería:
	./bin/localSearch ./data/ionosphere.arff