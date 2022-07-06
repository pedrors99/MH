Para una explicación más en detalle, consultar la sección 5 del pdf.

Estructura de carpetas:
	· bin: Contiene los archivos ejecutables que se crean tras la compilación.
	· data: Contiene los conjuntos de datos.
	· src: Contiene el código fuente.
	· table: Contiene los resultados (en un formato fácil de importar en látex) de las ejecuciones de todos los algoritmos.
	· graph: Contiene los archivos usados para la representación en gráficas de los resultados.

Los archivos ejecutables tienen el mismo nombre que sus archivos de código fuente correspondientes, excepto por la extensión.
Estos son:
	· aggBlx: Algoritmo genético generacional con cruce Blx.
	· aggCa:  Algoritmo genético generacional con cruce aritmético.
	· ageBlx: Algoritmo genético estacionario con cruce Blx.
	· ageCa:  Algoritmo genético estacionario con cruce aritmético.
	· am:     Algoritmo memético.

Todos los archivos utilizan las siguientes constantes (que se pueden modificar en las primeras líneas de código):
	· SEED: Semilla usada para la aleatoriedad, 0 por defecto.
	· FOLDS: Número de particiones para el algoritmo N-fold corss validation, 5 por defecto (4 de train, 1 de test).
	· REDUCCTION: Umbral para la tsa de reducción. Se consideran como 0 los pesos por debajo de dicho valor (0.1 por defecto).
	· CHROMOSOMES: Número de cromosomas utilizada por los algoritmos genéticos. Por defecto, 30 en los genéticos y 10 en los meméticos.
El resto de constantes son exclusivas a los distintos algoritmos, y están detalladas en el pdf.

Todas las funciones utilizadas contienen una breve descripción previa tanto de su funcionamiento, como de los parámetros de entrada y salida.

Para compilar, sólo es necesario ejecutar make. Esta compilación se realiza utilizando optimización -O2.
Para eliminar los ejecutables, make clean.

Para ejecutar un archivo, hay que ejecutarlo seguido del fichero de datos a utilizar. Por ejemplo, si queremos aplicar el algoritmo aggBlx a los datos de "ionosphere.arff", sería:
	./bin/aggBlx ./data/ionosphere.arff