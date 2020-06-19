#include "MEF.h"
#include "sapi.h"
extern volatile int TMEF;
extern volatile int ticksMef;
unsigned char flagMef = 0;

extern unsigned char entradaKey;
extern unsigned char entradaPresiono;
extern unsigned char entradaNumero;
extern int frecTotal;
volatile int ticksP = 0;
unsigned char flagP = 0;

volatile int flagFrecOk=0;


extern int flagBomba;
extern int flagFrecuencia;
extern volatile int ticksBomba;
extern int ticksSeg;
extern int ticksMin;
extern int ticksHora;
extern int ticksFrec;

extern int litrosMaceta;



int ticksReloj = 0;
extern int flagReloj;


void interrupt_handler(void *ptr) {
	ticksMef++;
	ticksP++;
	if (ticksMef == (TMEF)) {
		flagMef = 1;
		ticksMef = 0;
	} else {
		if (TMEF == -1) {
			ticksMef = 0;
		}

	}
	if (ticksP == 20) {

		ticksP = 0;
		flagP = 1;

	}
	if (flagBomba == 1) {
		ticksBomba++;
	}
	if (ticksBomba == litrosMaceta * 20 * 11) {
		flagBomba = 0;
		ticksBomba = 0;
	}

//	--------- funcion reloj ---------------------
	if (flagFrecuencia == 1) {
		ticksFrec++;
		ticksReloj++;
	}

	if (ticksFrec == frecTotal) {
		flagFrecOk=1;
	}

	if(ticksReloj==20){
		ticksReloj=0;
		flagReloj=1;
	}



	/*if (ticksFrec == 20) {
		ticksSeg++;
		if (ticksSeg == 60) {
			ticksSeg = 0;
			ticksMin++;
		}
		if (ticksMin == 60) {
			ticksMin = 0;
			ticksHora++;
		}
		if (ticksHora == 24) {
			ticksHora = 0;
		}
	}
*/
// -----------------------------------------------

}
/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {

	/* ------------- INICIALIZACIONES ------------- */

	inicializarMEF();
	tickConfig(50); // tickRateHz =   20 =>   20 ticks per second => 50 ms tick
	tickCallbackSet(interrupt_handler, (void*) LEDR);
	while (1) {
		flagMef = 1;
		if (flagMef) {
			leerTeclado();
			actualizarMEF();

			if (entradaPresiono)
				entradaPresiono = 0;
			flagMef = 0;
		}
		/*if (flagP) {
		 flagP=0;
		 lcdSendStringRaw("h ");
		 //delay(2000);
		 //lcdClear();
		 //lcdGoToXY(0, 0);
		 }*/
	}
	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
	 por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
