Para una explicación más en detalle, consultar la sección 5 del pdf.

Estructura de carpetas:
	· bin: Contiene los archivos ejecutables que se crean tras la compilación.
	· data: Contiene los conjuntos de datos.
	· src: Contiene el código fuente.
	· plots: Contiene los archivos necesarios para la representación en gráficas.
	· tables: Contiene los resultados de ejecutar los distintos algoritmos con un formato ''cómodo'' de importar en LaTex.

Los archivos ejecutables tienen el mismo nombre que sus archivos de código fuente correspondientes, excepto por la extensión.
Estos son:
	· charges: Metaheurística basada en cargas.
	· chargesMemetic: Variante de charges con hibridación con la búsqueda local.
	· chargesBl: Variante de charges que explora el mejor entorno.
	· chargesEuclidean: Variante de charges que utiliza la distancia euclídea en lugar de la distancia en fitness.
	· chargesMaxIter: Variante de charges que se detiene tras un número de iteraciones sin encontrar mejora.
	· chargesReset: Variante de charges que reinicia todas las soluciones menos la mejor a soluciones aleatorias tras un número de iteraciones sin encontrar mejora.

Todos los archivos utilizan las siguientes constantes (que se pueden modificar en las primeras líneas de código):
	· SEED: Semilla usada para la aleatoriedad, 0 por defecto.
	· FOLDS: Número de particiones para el algoritmo N-fold corss validation, 5 por defecto (4 de train, 1 de test).
	· REDUCCTION: Umbral para la tsa de reducción. Se consideran como 0 los pesos por debajo de dicho valor (0.1 por defecto).
	· CHROMOSOMES: Número de cromosomas utilizada por los algoritmos.

Todas las funciones utilizadas contienen una breve descripción previa tanto de su funcionamiento, como de los parámetros de entrada y salida.

Para compilar, sólo es necesario ejecutar make. Esta compilación se realiza utilizando optimización -O2.
Para eliminar los ejecutables, make clean.

Para ejecutar un archivo, hay que ejecutarlo seguido del fichero de datos a utilizar. Por ejemplo, si queremos aplicar el algoritmo charges a los datos de "ionosphere.arff", sería:
	./bin/charges ./data/ionosphere.arff