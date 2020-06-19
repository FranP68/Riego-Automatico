#include "sapi.h"
#include "chip.h"
#include "teclado.h"
#include "sensor.h"

#define TECLADO_SCAN 40

volatile int TMEF;
volatile int ticksMef = 0;
int flagBomba = 0;
volatile int ticksBomba = 0;
int ticksSeg = 0;
int ticksMin = 0;
int ticksHora = 0;
volatile int pasar = 0;
volatile int frecCargada = 0;
int ticksFrec = 0;
extern volatile int flagFrecOk;
int flagReloj = 0;



int estadoHum = 0;
int estadoFrec = 0;
extern int ticksReloj;

int flagFrecuencia = 0;
int frecHora;
int frecMin;
int frecSeg;
int frecHoraFijo;
int frecMinFijo;
int frecSegFijo;

int frecTotal;
int check = 1;
int cargaTotal = 0;

char estado = 'p'; //Variable estado: p:preparacion, i:Inicio; t:riego por tiempo; h: riego por humedad;
//m: muestra de datos; s: sin agua; r: regando
int8_t tecla = -1;
int8_t tecla2 = -1;
char fvector[] = { '12' };
delay_t escaneoTeclado;
void inicializarMEF();
void actualizarMEF();
char c = 0;
extern int litrosMaceta = 0;
int litrosTanque = 0;
int humedadMin = 0;
int humedad = 0;
char hum[2];
char carga[2];
//------------------
extern unsigned char entradaKey;
extern unsigned char entradaPresiono;
extern unsigned char entradaNumero;
int j = 0;
int z = 0;
int y = 0;
int q = 0;
char num[2];
char num2[2];
char num3[2];
char vecHora[2];
char vecMin[2];
char vecSeg[2];

int litros;

void imprimirHumedad(int h, char v[2]) {
	if (h == 100) {
		lcdSendStringRaw("Humedad: 100%");
	} else {
		if (h < 10) {
			v[0] = ' ';
			v[1] = (humedad % 10) + 48;
			lcdSendStringRaw("Humedad:");
			lcdSendStringRaw(v);
			lcdSendStringRaw("%");
		} else {
			v[0] = (h / 10) + 48;
			v[1] = (h % 10) + 48;
			lcdSendStringRaw("Humedad:");
			lcdSendStringRaw(v);
			lcdSendStringRaw("%");
		}
	}
}

//------------------actualizar maquina de estado--------------------------------------------------------------------

void actualizarMEF() {

	gpioMap_t pin = ENET_TXEN;
	gpioInit(pin, GPIO_OUTPUT);

	switch (estado) {
	case 'i':
		lcdGoToXY(0, 0);
		lcdSendStringRaw("A: por tiempo");

		lcdGoToXY(0, 1);
		lcdSendStringRaw("B: por humedad");

		if (entradaPresiono) {
			if (entradaKey == 'A') {
				estado = 't';
				lcdClear();
			}
		}
		if (entradaPresiono) {
			if (entradaKey == 'B') {
				estado = 'h';
				lcdClear();
			}
		}
		if (entradaKey == '#') {
			estado = 'l';
			lcdClear();
			entradaKey = 'o';
			num[0] = '\0';
			num[1] = '\0';
			j = 0;
			cargaTotal = 0;
			litrosTanque = 0;
			litrosMaceta = 0;
			delay(500);
		}
		if (entradaKey == 'D') {
			estado = 'p';
			lcdClear();
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Bienvenidos");
			delay(400);
			lcdClear();
			lcdGoToXY(0, 0);
			entradaKey = 'o';
			num[0] = '\0';
			num[1] = '\0';
			num2[0] = '\0';
			num2[1] = '\0';
			num3[0] = '\0';
			num3[1] = '\0';
			ticksSeg = 0;
			ticksMin = 0;
			ticksHora = 0;
			pasar = 0;
			frecCargada = 0;
			cargaTotal = 0;
			litrosMaceta = 0;
			litrosTanque = 0;
			humedadMin = 0;
			humedad = 0;
			j = 0;
			z = 0;
			y = 0;
			q = 0;

		}

		break;
		//-----------------estado elegir humedad---------------------------------------------------------------------

	case 'h':

		lcdGoToXY(0, 0);
		lcdSendStringRaw("Elija el % de");
		lcdGoToXY(0, 1);
		lcdSendStringRaw("humedad: ");

		if (entradaPresiono) {
			if (entradaKey == '#') {
				estado = 'i';
				lcdClear();
				entradaKey = 'o';
				num[0] = '\0';
				num[1] = '\0';
				j = 0;
				humedadMin = 0;
				litrosMaceta = 0;
			}
			if (entradaKey == 'D') {
				estado = 'p';
				lcdClear();
				lcdGoToXY(0, 0);
				lcdSendStringRaw("Bienvenidos");
				delay(400);
				lcdClear();
				lcdGoToXY(0, 0);
				entradaKey = 'o';
				num[0] = '\0';
				num[1] = '\0';
				num2[0] = '\0';
				num2[1] = '\0';
				num3[0] = '\0';
				num3[1] = '\0';
				ticksSeg = 0;
				ticksMin = 0;
				ticksHora = 0;
				pasar = 0;
				frecCargada = 0;
				cargaTotal = 0;
				litrosMaceta = 0;
				litrosTanque = 0;
				humedadMin = 0;
				humedad = 0;
				j = 0;
				z = 0;
				y = 0;
				q = 0;

			}

			if (entradaNumero) {
				num[j] = entradaKey;
				j++;
				lcdSendStringRaw(num);

			} else {
				if (entradaKey == '*' && j >= 1 && j < 3) {

					if (j == 1) {
						humedadMin = (num[0] - 48);

					} else {
						humedadMin = (num[0] - 48) * 10 + (num[1] - 48);
					}
					num[0] = '\0';
					num[1] = '\0';
					j = 0;
					estadoHum = 1;
					estado = 'k'; // pasa al estado para chequear humedad
					lcdClear();
				} else {
					if (j >= 3) {
						for (int x = 0; x <= j; x++) {
							num[x] = '\0';
						}
						j = 0;
						lcdClear();
						lcdSendStringRaw("Ingrese nuevamente");
						delay(1000);
						lcdClear();
					}
				}
			}
			delay(400);
		}
		break;
		//-------------------estado humedad minima-------------------------------------------------------------------

	case 'k':
		lcdGoToXY(0, 0);
		humedad = sensadoHumedad();
		imprimirHumedad(humedad, hum);
		if (humedad < humedadMin) {
			estado = 'r';
			flagBomba = 1;

		}
		delay(500);
		break;
		//------------------------estado elegir frecuencia--------------------------------------------------------------

	case 't':

		lcdGoToXY(0, 0);
		lcdSendStringRaw("Elija hs:min:seg");

		if (entradaKey == '#') {
			estado = 't';
			entradaKey = 'o';
			num[0] = '\0';
			num[1] = '\0';
			num2[0] = '\0';
			num2[1] = '\0';
			num3[0] = '\0';
			num3[1] = '\0';
			estado = 't';
			pasar = 0;
			j = 0;
			frecHora = 0;
			frecMin = 0;
			frecSeg = 0;
			z = 0;
			y = 0;
			frecTotal = 0;
			lcdClear();

		}
		if (entradaKey == 'D') {
			estado = 'p';
			lcdClear();
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Bienvenidos");
			delay(400);
			lcdClear();
			lcdGoToXY(0, 0);
			entradaKey = 'o';
			num[0] = '\0';
			num[1] = '\0';
			num2[0] = '\0';
			num2[1] = '\0';
			num3[0] = '\0';
			num3[1] = '\0';
			ticksSeg = 0;
			ticksMin = 0;
			ticksHora = 0;
			pasar = 0;
			frecCargada = 0;
			cargaTotal = 0;
			litrosMaceta = 0;
			litrosTanque = 0;
			humedadMin = 0;
			humedad = 0;
			j = 0;
			z = 0;
			y = 0;
			q = 0;
			pasar = 4;

		}

		if (entradaPresiono && pasar == 0) {
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Elija hs:min:seg");
			lcdGoToXY(0, 1);
			lcdSendStringRaw("hs:     ");
			lcdGoToXY(3, 1);
			if (entradaNumero) {
				num[j] = entradaKey;
				j++;
				lcdSendStringRaw(num);

			} else {
				delay(400);
				if (entradaKey == '*' && j >= 1 && j < 3) {
					pasar = 1;
					entradaKey = 'o';
					if (j == 1) {

						frecHora = (num[0] - 48);
						frecHoraFijo = frecHora;
					} else {

						frecHora = (num[0] - 48) * 10 + (num[1] - 48);
						frecHoraFijo = frecHora;
					}
					//num[0] = '\0';
					//num[1] = '\0';
					j = 0;

					//estado = 'f'; // pasa al estado para chequear frecuencia
					//lcdClear();
				} else {
					if (j >= 3) {
						for (int x = 0; x <= j; x++) {
							num[x] = '\0';
						}
						j = 0;
						lcdClear();
						lcdSendStringRaw("Ingrese nuevamente");
						delay(1000);
						lcdClear();
					}
				}
			}

			delay(400);
		}
		if (entradaPresiono && pasar == 1) {
			lcdClear();
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Elija hs:min:seg");
			lcdGoToXY(0, 1);
			lcdSendStringRaw("hs:");
			lcdGoToXY(3, 1);
			lcdSendStringRaw(num);
			lcdSendStringRaw("min:    ");
			lcdGoToXY(9, 1);
			if (entradaNumero) {
				num2[z] = entradaKey;
				z++;
				lcdSendStringRaw(num2);

			} else {
				delay(400);
				if (entradaKey == '*' && z >= 1 && z < 3) {

					if (z == 1) {

						frecMin = (num2[0] - 48);
						frecMinFijo = frecMin;

					} else {

						frecMin = (num2[0] - 48) * 10 + (num2[1] - 48);
						frecMinFijo = frecMin;
					}
					if (frecMin > 60) {
						lcdClear();
						lcdGoToXY(0, 0);
						lcdSendStringRaw("numero no valido");
						delay(500);
						num[0] = '\0';
						num[1] = '\0';
						num2[0] = '\0';
						num2[1] = '\0';
						num3[0] = '\0';
						num3[1] = '\0';
						estado = 't';
						pasar = 0;
					} else {
						pasar = 2;
						entradaKey = 'o';
					}
					//num2[0] = '\0';
					//num2[1] = '\0';
					z = 0;

					//estado = 'f'; // pasa al estado para chequear frecuencia
					//lcdClear();
				} else {
					if (z >= 3) {
						for (int x = 0; x <= z; x++) {
							num2[x] = '\0';
						}
						z = 0;
						lcdClear();
						lcdSendStringRaw("Ingrese nuevamente");
						delay(1000);
						lcdClear();
					}
				}
			}

			delay(400);
		}
		if (entradaPresiono && pasar == 2) {
			lcdClear();
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Elija hs:min:seg");
			lcdGoToXY(0, 1);
			lcdSendStringRaw("hs:");
			lcdGoToXY(3, 1);
			lcdSendStringRaw(num);
			lcdSendStringRaw("min:");
			lcdGoToXY(8, 1);
			lcdSendStringRaw(num2);
			lcdGoToXY(10, 1);
			lcdSendStringRaw("seg:");
			lcdGoToXY(14, 1);
			if (entradaNumero) {
				num3[y] = entradaKey;
				y++;
				lcdSendStringRaw(num3);

			} else {
				delay(400);
				if (entradaKey == '*' && y >= 1 && y < 3) {

					if (y == 1) {

						frecSeg = (num3[0] - 48);
						frecSegFijo = frecSeg;

					} else {

						frecSeg = (num3[0] - 48) * 10 + (num3[1] - 48);
						frecSegFijo = frecSeg;
					}
					if (frecSeg > 60) {
						lcdClear();
						lcdGoToXY(0, 0);
						lcdSendStringRaw("numero no valido");
						delay(500);
						num[0] = '\0';
						num[1] = '\0';
						num2[0] = '\0';
						num2[1] = '\0';
						num3[0] = '\0';
						num3[1] = '\0';
						estado = 't';
						pasar = 0;
					} else {
						frecCargada = 1;
						estadoFrec = 1;
					}
					//num3[0] = '\0';
					//num3[1] = '\0';
					y = 0;

					//estado = 'f'; // pasa al estado para chequear frecuencia
					//lcdClear();
				} else {
					if (y >= 3) {
						for (int x = 0; x <= y; x++) {
							num3[x] = '\0';
						}
						y = 0;
						lcdClear();
						lcdSendStringRaw("Ingrese nv");
						delay(1000);
						lcdClear();
					}
				}
			}

			delay(400);
		}

		if (frecCargada == 1) {
			frecCargada = 0;
			flagFrecOk = 0;
			frecTotal = (frecSeg * 20) + (frecMin * 60 * 20)
					+ (frecHora * 60 * 60 * 20);
			flagFrecuencia = 1;
			flagReloj = 0;
			estado = 'c'; //pasa a chekear el tiempo
		}

		break;
		//------------------------estado fijar frecuencia--------------------------------------------------------------

	case 'c':
		lcdGoToXY(0, 0);
		lcdClear();
		humedad = sensadoHumedad();
		imprimirHumedad(humedad, hum);

		if (flagFrecOk == 1) {
			flagBomba = 1;
			estado = 'r';
		}

		if (flagReloj) {
			flagReloj = 0;
			if (frecHora > 0) {
				if (frecMin > 0) {
					if (frecSeg > 0) {
						frecSeg--;
					} else {
						frecSeg = 59;
						frecMin = 59;
					}
				} else {
					if (frecSeg > 0) {
						frecSeg--;
					} else {
						frecHora--;
						frecMin = 59;
						frecSeg = 59;
					}
				}
			} else {
				if (frecMin > 0) {
					if (frecSeg > 0) {
						frecSeg--;
					} else {
						frecMin--;
						frecSeg = 59;
					}
				} else {
					if (frecSeg > 0) {
						frecSeg--;
					}
				}
			}
		}
		lcdGoToXY(0, 1);
		vecHora[0] = (frecHora / 10) + 48;
		vecHora[1] = (frecHora % 10) + 48;
		vecMin[0] = (frecMin / 10) + 48;
		vecMin[1] = (frecMin % 10) + 48;
		vecSeg[0] = (frecSeg / 10) + 48;
		vecSeg[1] = (frecSeg % 10) + 48;
		lcdSendStringRaw(vecHora);
		lcdSendStringRaw(":");
		lcdSendStringRaw(vecMin);
		lcdSendStringRaw(":");
		lcdSendStringRaw(vecSeg);

		delay(500);
		break;

		//----------------------estado elegir litros maceta 1 --------------------------------------------------------------
	case 'p':

		lcdGoToXY(0, 0);
		lcdSendStringRaw("Ingrese litros");
		lcdGoToXY(0, 1);
		lcdSendStringRaw("de la maceta:");
		if (entradaPresiono) {
			if (entradaKey == '#') {
				estado = 'p';
				lcdClear();
				entradaKey = 'o';
				num[0] = '\0';
				num[1] = '\0';
				j = 0;
				cargaTotal = 0;
				litrosMaceta = 0;
			}
			if (entradaKey == 'D') {
				estado = 'p';
				lcdClear();
				lcdGoToXY(0, 0);
				lcdSendStringRaw("Bienvenidos");
				delay(400);
				lcdClear();
				lcdGoToXY(0, 0);
				entradaKey = 'o';
				num[0] = '\0';
				num[1] = '\0';
				num2[0] = '\0';
				num2[1] = '\0';
				num3[0] = '\0';
				num3[1] = '\0';
				ticksSeg = 0;
				ticksMin = 0;
				ticksHora = 0;
				pasar = 0;
				frecCargada = 0;
				cargaTotal = 0;
				litrosMaceta = 0;
				litrosTanque = 0;
				humedadMin = 0;
				humedad = 0;
				j = 0;
				z = 0;
				y = 0;
				q = 0;

			}

			if (entradaNumero) {
				num[j] = entradaKey;
				j++;
				lcdSendStringRaw(num);

			} else if (entradaKey == '*' && j >= 1 && j < 3) {

				if (j == 1) {
					litrosMaceta = (num[0] - 48);

				} else {
					litrosMaceta = (num[0] - 48) * 10 + (num[1] - 48);
				}
				num[0] = '\0';
				num[1] = '\0';
				j = 0;
				estado = 'l';
				lcdClear();
			} else {
				if (j >= 3) {
					for (int x = 0; x <= j; x++) {
						num[x] = '\0';
					}
					j = 0;
					lcdClear();
					lcdSendStringRaw("Ingrese nuevamente");
					delay(1000);
					lcdClear();
				}
			}
			delay(400);
		}

		break;

		//------------------------estado elegir litros deposito 2 ----------------------------------------------------------

	case 'l':

		lcdGoToXY(0, 0);
		lcdSendStringRaw("Ingrese litros");
		lcdGoToXY(0, 1);
		lcdSendStringRaw("del deposito:");
		delay(100);
		if (entradaKey == '#') {
			estado = 'p';
			lcdClear();
			entradaKey = 'o';
			num[0] = '\0';
			num[1] = '\0';
			j = 0;
			cargaTotal = 0;
			litrosTanque = 0;
			litrosMaceta = 0;
		}
		if (entradaKey == 'D') {
			estado = 'p';
			lcdClear();
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Bienvenidos");
			delay(400);
			lcdClear();
			lcdGoToXY(0, 0);
			entradaKey = 'o';
			num[0] = '\0';
			num[1] = '\0';
			num2[0] = '\0';
			num2[1] = '\0';
			num3[0] = '\0';
			num3[1] = '\0';
			ticksSeg = 0;
			ticksMin = 0;
			ticksHora = 0;
			pasar = 0;
			frecCargada = 0;
			cargaTotal = 0;
			litrosMaceta = 0;
			litrosTanque = 0;
			humedadMin = 0;
			humedad = 0;
			j = 0;
			z = 0;
			y = 0;
			q = 0;

		}

		if (entradaPresiono) {
			if (entradaNumero) {
				num[j] = entradaKey;
				j++;
				lcdSendStringRaw(num);

			} else {

				if (entradaKey == '*' && j >= 1 && j < 3) {

					if (j == 1) {
						litrosTanque = (num[0] - 48);
						cargaTotal = litrosTanque * 3;

					} else {
						litrosTanque = (num[0] - 48) * 10 + (num[1] - 48);
						cargaTotal = litrosTanque * 3;
					}
					num[0] = '\0';
					num[1] = '\0';
					j = 0;
					estado = 'i'; // pasa al estado de seleccion de modo
					lcdClear();
					lcdSendStringRaw("Seleccion de");
					lcdGoToXY(0, 1);
					lcdSendStringRaw(" modo de riego");
					delay(1200);
					lcdClear();
				} else {
					if (j >= 3) {
						for (int x = 0; x <= j; x++) {
							num[x] = '\0';
						}
						j = 0;
						lcdClear();
						lcdSendStringRaw("Ingrese nuevamente");
						delay(1000);
						lcdClear();
					}
				}
			}
			delay(400);
		}

		break;

		//-----------------------------estado sin agua---------------------------------------------------------

	case 's':

		lcdGoToXY(0, 0);
		lcdSendStringRaw("Sin agua!");
		lcdGoToXY(0, 1);
		lcdSendStringRaw("Presione C");
		delay(200);
		if (entradaPresiono) {
			if (entradaKey == 'C') {
				estado = 'w';
			}
			lcdClear();
		}
		delay(200);
		break;

		//--------------------------estado regando-------------------------------------------------------------------------

	case 'w':

		lcdGoToXY(0, 0);
		lcdSendStringRaw("Ingrese litros");
		lcdGoToXY(0, 1);
		lcdSendStringRaw("del deposito:");
		delay(400);
		if (entradaPresiono) {
			if (entradaNumero) {
				num[j] = entradaKey;
				j++;
				lcdSendStringRaw(num);

			} else {
				if (entradaKey == '*' && j >= 0 && j < 3) {

					if (j == 1) {
						litrosTanque = (num[0] - 48);
						cargaTotal = litrosTanque * 3;
					} else {
						litrosTanque = (num[0] - 48) * 10 + (num[1] - 48);
						cargaTotal = litrosTanque * 3;
					}
					num[0] = '\0';
					num[1] = '\0';
					j = 0;
					estado = 'r'; //vuelve al estado riego con agua
					lcdClear();
				} else {
					if (j >= 3) {
						for (int x = 0; x <= j; x++) {
							num[x] = '\0';
						}
						j = 0;
						lcdClear();
						lcdSendStringRaw("Ingrese nuevamente");
						delay(1000);
						lcdClear();
					}
				}

			}
			delay(400);
		}

		break;

		case 'r':

		lcdClear();
		lcdGoToXY(0, 0);
		humedad = sensadoHumedad();
		imprimirHumedad(humedad, hum);
		lcdGoToXY(0, 1);
		if (carga < 10) {
			carga[0] = ' ';
			carga[1] = (cargaTotal % 10) + 48;
			lcdSendStringRaw("carga:");
			lcdSendStringRaw(carga);
		} else {
			carga[0] = (cargaTotal / 10) + 48;
			carga[1] = (cargaTotal % 10) + 48;
			lcdSendStringRaw("carga:");
			lcdSendStringRaw(carga);
		}
		if (check) {
			if (litrosMaceta <= cargaTotal) {
				cargaTotal = cargaTotal - litrosMaceta;
				//lcdSendStringRaw("Regando");
				check = 0;

			} else {
				estado = 's';
				j = 0;
				lcdClear();
			}
		}
		if(flagBomba == 1){
			lcdSendStringRaw(" Regando");

		}
		if (flagBomba == 0) {
			lcdClear();
			lcdGoToXY(0, 0);
			lcdSendStringRaw("Riego completado");
			delay(800);

			if (estadoFrec == 1) {
				frecHora = frecHoraFijo;
				frecMin = frecMinFijo;
				frecSeg = frecSegFijo;
				frecCargada = 1;
				flagFrecOk = 0;
				flagReloj = 0;
				ticksFrec = 0;
				ticksReloj = 0;

				estado = 't';
				check = 1;

			}

			if(estadoHum == 1){

				estado = 'k';
				check=1;
			}

		}
		delay(300);
		//gpioWrite(pin, 1);
		//lcdData(0);
		//while (gpioRead(pin)) {
		//	lcdGoToXY(0, 0);
		//	lcdSendStringRaw("Regando");
		//lcdData(0);
		//	delay(1000);
		//}
		//delay(1000);
		lcdClear();
		break;

	default:

		break;

	} //fin swicth

} //fin actualizarMEF

void inicializarMEF() {

// Inicializar y configurar la plataforma
	boardConfig();

// Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x2 pixeles
	lcdInit(16, 2, 5, 8);

	TMEF = 1;
	ticksMef = 0;
	lcdSendString("Bienvenidos");

	delay(1000);
	lcdClear();
	lcdGoToXY(0, 0);

	initKeypad();

	delayInit(&escaneoTeclado, TECLADO_SCAN);
}

void mostrarHumedad() {
	adcInit(ENABLE);

}
