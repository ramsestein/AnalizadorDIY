# AnalizadorDIY

<b>Marco teórico</b>

Este proyecto se basa en otro proyecto anterior que se encuentra en Instructables, llamado “Espectrofotómetro DIY” en el que se construía el aparato y se conseguían obtener valores de absorbancia de distintas sustancias.

El proyecto consiste, en primera instancia, en la modificación de este diseño inicial convirtiéndolo en algo totalmente automatizado y simplificando su uso por parte del usuario final. Finalmente se intenta escalar el sistema del aparato hasta su versión más simple funcional, con lo que abaratar las técnicas de fabricación del mismo.

El primer paso del proyecto ha consistido en detectar que variables pueden ser de interés a la hora de detectarlas y cuales pueden tener o no significancia clínica.

El Ph y la cantidad de cloro de la muestra se mostraron como dos variables útiles para ser captadas. El cloro se presentaba especialmente útil al medirse su diferencial después de pasar por el liquido una corriente DC de al menos 12V. Con este diseño se intenta lisar las células que estén en la disolución, diluyendo el contenido de estas en el resto de la disolución.

En las pruebas preliminares, en las que se añadió a la muestra de orina pequeñas muestras de sangre (dada la imposibilidad práctica de conseguir muestras de orina con infección). Pareciera que esta variable podía parecer una variable útil de medir.


<b>Espectrofotometro</b>

Modelos
Espectrofotómetro v1
(versión actual)

Este diseño elimina la interacción humana hacia el sistema, reduciéndose a una simple activación en el ordenador del sistema

Se elimina también la necesidad de un driver especifico y una alimentación supletoria al USB en el sistema utilizando un motor paso a paso alimentado a 5V reciclado de una impresora.

Se mantiene el sistema óptico, que ahora es controlado por el Arduino, que además lee los datos, los procesa y los envía a través del USB, que también sirve de alimentación

<b>Clorímetro</b>

Dada la inexistencia de un sensor de cloro compatible con Arduino, se desarrolla un sistema de detección de cloro.

Un clorímetro tiene una electrónica bastante sencilla. Su funcionamiento se basa en una celda redox, que permite mediante la medida de conductividad que existe en la disolución, medir la cantidad de cloro que en ella se encuentra. Esta simpleza aparente oculta otros retos a los que uno debe enfrentarse, como la durabilidad lo más larga posible del producto, la conectividad a sistemas informáticos (para procesar esta señal junto al resto permitiendo el diagnostico final) o la fiabilidad del propio aparato

La durabilidad de la celda fue un problema más difícil de solucionar. Por la propia naturaleza de la medición, existe una oxidación de los electrodos (los cuales eran, simplemente, dos clavos reciclados del taller), lo que daba dos problemas, el primero era que los electrodos no eran infinitos, necesitaba un recambio que estimaba cada 2 días de uso intensivo; esto no es algo tan grave, pues el material es barato y solo se necesitaba cambiarlos por unos nuevos; el segundo problema era más difícil, la oxidación de los electrodos modificaba el metal en ellos, lo que cambiaba la señal que se recibía a cada paso de uso. Se estudió durante bastante tiempo cuales eran los cambios de la señal con respecto al tiempo y a la magnitud de su uso y se vio que el cambio de la señal no era homogéneo, cuando el electrodo comenzaba su oxidación se cubre con una capa fina de oxido de hierro, lo que protegía en núcleo del electrodo y alargaba su vida y estabilidad en este estado. La respuesta fue calibrar el aparato directamente en este estado de ligera oxidación, esto estabilizó la señal que se recibía y alargó la vida de los electrodos, pues ahora podían utilizarse en un estado más estable, pasando de los 2 días iniciales, a 2 semanas con el mismo uso intensivo.

La fiabilidad del aparato fue contrastada durante todo el proceso con un convencional comercial, tomando los valores de éste como referencia. Uno de los puntos a tener en cuenta es la robustez de la señal de nuestra celda. Los voltajes utilizados por el aparato convencional y con el propio eran bastante diferentes, llegando a triplicarse el 1,2 voltios que utilizaba el comercial. Esto no fue azaroso, muchos voltajes se probaron y se descubrió que este era el voltaje más aceptable y estable. Se realizaron muchas pruebas, como la adición de otras soluciones que pudieran causar ruido en la muestra o variar el resultado por interferencia en la celda. La versión comercial sufrió el ruido y los cambios, el nuestro no.

Se utilizaron los valores de referencia del clorímetro comercial para escalar el nuestro, pero quedó claro desde el principio que esto reducía el potencial del que habíamos construido. La digitalización de los datos y la escala permitía al nuestro llegar a valores más altos teóricos que el comercial, pero no podíamos asegurar en este caso una fiabilidad, pues se basaría en una extrapolación matemática que bien podía no ser cierta para ese rango de valores. Por ello nuestro clorímetro llega solo a valores de 4,5 medidas de cloro, cuando muy presumiblemente llegue más allá.

El clorímetro ha sido un proyecto muy interesante, tanto en la práctica como académicamente, pues ha dado grandes nociones practicas tanto del comportamiento de aparatos de medida similares como del comportamiento de las señales en una disolución. Aún así, este no es más que un primer paso, actualmente ya estamos trabajando en replicar y adaptar en la misma placa del clorímetro un medidor de ppm y de pH, añadiendo nuevos sensores a nuestro prototipo, lo que permitirá acercarnos cada vez más a nuestra meta, el detector libre y de bajo coste de infecciones de orina (y presumiblemente, otros líquidos corporales)

PH
Se planea utilizar un sensor de Ph prefabricado para Arduino
