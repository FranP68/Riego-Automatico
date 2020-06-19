// porcentaje Humedad= (-0,00104 SalidaADC + 1,03688 ) *100
#include "sapi.h"

int sensadoHumedad();

int sensadoHumedad() {

	adcConfig(ADC_ENABLE); /* ADC */
	int valorSensor = 0;
	/* Variable para almacenar el valor leido del ADC CH1 */
	int muestra = 0;

	/* Variables de delays no bloqueantes */
	delay_t delay1;

	/* Inicializar Retardo no bloqueante con tiempo en ms */
	delayConfig(&delay1, 500);
	/* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
	//if (delayRead(&delay1)) {

		/* Leo la Entrada Analogica AI0 - ADC0 CH1 */
		muestra = adcRead(CH1);

	//}

	if (muestra < 420) {
		valorSensor = 100;
	} else {
		if (muestra > 1023) {
			valorSensor = 0;
		} else {
			valorSensor = (-0.1668 * muestra) + 169.682;
		}
	}
	adcConfig(ADC_DISABLE);
	return valorSensor;
}

