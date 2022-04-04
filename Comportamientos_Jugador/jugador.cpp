#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

void ComportamientoJugador::inicializarValoresCasillas()
{
	valoresCasillas['A'] = 15;
	valoresCasillas['B'] = 5;
	valoresCasillas['T'] = 74;
	valoresCasillas['S'] = 75;
	valoresCasillas['M'] = -10000;
	valoresCasillas['P'] = -1000;

	valoresCasillas['G'] = 100;
	valoresCasillas['K'] = 50;
	valoresCasillas['D'] = 50;
	valoresCasillas['X'] = 100;
}

void ComportamientoJugador::rellenarMemoria(Sensores sensores)
{
}

void ComportamientoJugador::rellenarVisionCompleta(Sensores sensores)
{
	mapaResultado[fil][col] = sensores.terreno[0];

	int contFil = 0;
	int contCol = 0;
	int util = 1;

	switch (brujula)
	{
	case 0:

		contFil = -1;
		contCol = -1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[fil + contFil][col + contCol + j] = sensores.terreno[util];
				util++;
			}

			contCol--;
			contFil--;
		}

		break;

	case 1:

		contFil = -1;
		contCol = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[fil + contFil + j][col + contCol] = sensores.terreno[util];
				util++;
			}

			contCol++;
			contFil--;
		}
		break;
	case 2:

		contFil = 1;
		contCol = 1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[fil + contFil][col + contCol - j] = sensores.terreno[util];
				util++;
			}

			contCol++;
			contFil++;
		}
		break;
	case 3:

		contFil = 1;
		contCol = -1;

		for (int i = 3; i <= 7; i += 2)
		{
			for (int j = 0; j < i; j++)
			{
				mapaResultado[fil + contFil - j][col + contCol] = sensores.terreno[util];
				util++;
			}

			contCol--;
			contFil++;
		}
		break;
	}
}

void ComportamientoJugador::calculoTriangulo(Sensores sensores)
{
	int sumatoriaTriangulo = 0;
	for (int i = 0; i < sensores.terreno.size(); i++)
	{

		sumatoriaTriangulo += valoresCasillas[sensores.terreno[i]];
	}

	cout << "Puntuacion triangulo = " << sumatoriaTriangulo << endl;
	puntuaciones.push_back(sumatoriaTriangulo);
}

pair<int, int> ComportamientoJugador::getPos(int i)
{
	pair<int, int> out;

	int lejos = 0;
	int movLateral = 0;

	if (i >= 1 && i <= 3)
	{
		lejos = 1;
	}
	else if (i >= 4 && i <= 8)
	{
		lejos = 2;
	}
	else
	{
		lejos = 3;
	}

	switch (i)
	{
	case 1:
	case 5:
	case 11:
		movLateral = 1;
		break;
	case 4:
	case 10:
		movLateral = 2;
		break;
	case 9:
		movLateral = 3;
		break;
	case 3:
	case 7:
	case 13:
		movLateral = -1;
		break;
	case 8:
	case 14:
		movLateral = -2;
		break;
	case 15:
		movLateral = -3;
		break;
	}

	out.first = lejos;
	out.second = movLateral;

	return out;
}

void ComportamientoJugador::findPosition(int i)
{

	pair<int, int> mov = getPos(i);
	int movLateral = mov.second;
	int lejos = mov.first;

	if (movLateral > 0)
	{

		for (int k = 0; k < lejos; k++)
		{
			acciones.push_back(actFORWARD);
		}

		acciones.push_back(actTURN_L);

		for (int k = 0; k < movLateral; k++)
		{
			acciones.push_back(actFORWARD);
		}
	}
	else if (movLateral < 0)
	{

		for (int k = 0; k < lejos; k++)
		{
			acciones.push_back(actFORWARD);
		}

		acciones.push_back(actTURN_R);

		for (int k = 0; k < abs(movLateral); k++)
		{
			acciones.push_back(actFORWARD);
		}
	}
	else
	{

		for (int k = 0; k < lejos; k++)
		{
			acciones.push_back(actFORWARD);
		}
	}
}

void ComportamientoJugador::actualizarBrujulaPosicion()
{
	switch (ultimaAccion)
	{
	case actFORWARD:
		switch (brujula)
		{
		case 0:
			fil--;
			break;
		case 1:
			col++;
			break;
		case 2:
			fil++;
			break;
		case 3:
			col--;
			break;
		}
		break;
	case actTURN_L:
		brujula = (brujula + 3) % 4;
		break;
	case actTURN_R:
		brujula = (brujula + 1) % 4;
		break;
	}
}

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;

	if (sensores.nivel == 1 || sensores.nivel == 2)
	{
		this->actualizarBrujulaPosicion();
	}

	if(acciones.empty()){

	
		for (int i = 0; i < sensores.terreno.size(); i++)
		{

			if (sensores.terreno[i] == 'G' && !bienSituado && i != 0)
			{
				findPosition(i);
			}
			else if (sensores.terreno[i] == 'X' && i != 0 && sensores.bateria < 2500)
			{

				findPosition(i);
				int timeIdle = 20;
				for (int i = 0; i < timeIdle; i++)
				{
					acciones.push_back(actIDLE);
				}
			}
			else if (sensores.terreno[i] == 'K' && !bikiniOn && i != 0)
			{
				findPosition(i);
			}
			else if (sensores.terreno[i] == 'D' && !zapatillasOn && i != 0)
			{
				findPosition(i);
			}
		}

	}

	if (acciones.empty())
	{
		cout << "Entro a buscar accion" << endl;
		

		if (bienSituado && acciones.empty())
		{

			bool encontrado = false;

			switch (brujula)
			{
			case 0:

				for (int i = 0; i < mapaResultado.size() and encontrado == false; i++)
				{
					for (int j = 0; j < mapaResultado.size() and encontrado == false; j++)
					{
						if (mapaResultado[i][j] == '?')
						{
							encontrado = true;
							positionToGo.first = i;
							positionToGo.second = j;
						}
					}
				}
				
				cout << "Position to go: " << positionToGo.first << " " << positionToGo.second << endl;


				if (positionToGo.first < fil)
				{
					for (int i = 0; i < (fil - positionToGo.first); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.first > fil)
				{

					acciones.push_back(actTURN_R);
					acciones.push_back(actTURN_R);

					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}

				if (positionToGo.second > col and positionToGo.first < fil)
				{

					acciones.push_back(actTURN_R);
					for (int i = 0; i < (positionToGo.second - col); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.second < col and positionToGo.first < fil)
				{

					acciones.push_back(actTURN_L);
					for (int i = 0; i < (col - positionToGo.second); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.second > col and positionToGo.first > fil)
				{

					acciones.push_back(actTURN_L);
					for (int i = 0; i < (positionToGo.second - col); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.second < col and positionToGo.first > fil)
				{
					acciones.push_back(actTURN_R);
					for (int i = 0; i < (col - positionToGo.second); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}

				break;

			case 2:

				for (int i = mapaResultado.size() - 1; i >= 0 and encontrado == false; i--)
				{
					for (int j = mapaResultado.size() - 1; j >= 0 and encontrado == false; j--)
					{
						if (mapaResultado[i][j] == '?')
						{
							encontrado = true;
							positionToGo.first = i;
							positionToGo.second = j;
						}
					}
				}

				cout << "Position to go south: " << positionToGo.first << " " << positionToGo.second << endl;

				if (positionToGo.first > fil)
				{
					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.first < fil){
					
					acciones.push_back(actTURN_R);
					acciones.push_back(actTURN_R);

					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
		

				if (positionToGo.second > col and positionToGo.first > fil)
				{

					acciones.push_back(actTURN_L);
					for (int i = 0; i < (positionToGo.second - col); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.second < col and positionToGo.first > fil)
				{

					acciones.push_back(actTURN_R);
					for (int i = 0; i < (col - positionToGo.second); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.second > col and positionToGo.first < fil){
					
					acciones.push_back(actTURN_R);
					for (int i = 0; i < (positionToGo.second - col); i++)
					{
						acciones.push_back(actFORWARD);
					}
					
				} else if(positionToGo.second < col and positionToGo.first < fil){
					acciones.push_back(actTURN_L);
					for (int i = 0; i < (col - positionToGo.second); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}


				break;

			case 1:

				for (int i = mapaResultado.size() - 1; i >= 0 and encontrado == false; i--)
				{
					for (int j = 0; j < mapaResultado.size() and encontrado == false; j++)
					{
						if (mapaResultado[j][i] == '?')
						{
							encontrado = true;
							positionToGo.first = j;
							positionToGo.second = i;
						}
					}
				}
				cout << "Position to go east: " << positionToGo.first << " " << positionToGo.second << endl;

				if (positionToGo.second > col)
				{
					for (int i = 0; i < (positionToGo.second - col); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.second < col){
					
					acciones.push_back(actTURN_R);
					acciones.push_back(actTURN_R);

					for (int i = 0; i < (col - positionToGo.second); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
			
				if (positionToGo.first > fil && positionToGo.second > col)
				{

					acciones.push_back(actTURN_R);
					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.first < fil && positionToGo.second > col)
				{

					acciones.push_back(actTURN_L);
					for (int i = 0; i < (fil - positionToGo.first); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.first > fil && positionToGo.second < col){
					acciones.push_back(actTURN_L);
					for (int i = 0; i < (fil - positionToGo.first); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.first < fil && positionToGo.second < col){
					acciones.push_back(actTURN_R);
					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}

				break;

			case 3:

				for (int i =  0; i < mapaResultado.size() and encontrado == false; i++)
				{
					for (int j = mapaResultado.size() - 1; j > 0 and encontrado == false; j--)
					{
						if (mapaResultado[j][i] == '?')
						{
							encontrado = true;
							positionToGo.first = j;
							positionToGo.second = i;
						}
					}
				}
				cout << "Position to go west: " << positionToGo.first << " " << positionToGo.second << " " << fil << endl;

				if (positionToGo.second < col)
				{
					for (int i = 0; i < (col - positionToGo.second); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.second > col){
					
					acciones.push_back(actTURN_R);
					acciones.push_back(actTURN_R);

					for (int i = 0; i < (positionToGo.second - col); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
			
				if (positionToGo.first < fil && positionToGo.second < col)
				{

					acciones.push_back(actTURN_R);
					for (int i = 0; i < (fil - positionToGo.first); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}
				else if (positionToGo.first > fil && positionToGo.second < col)
				{
					acciones.push_back(actTURN_L);
					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.first > fil && positionToGo.second > col){
					acciones.push_back(actTURN_R);
					for (int i = 0; i < (fil - positionToGo.first); i++)
					{
						acciones.push_back(actFORWARD);
					}
				} else if(positionToGo.first < fil && positionToGo.second > col){
					acciones.push_back(actTURN_L);
					for (int i = 0; i < (positionToGo.first - fil); i++)
					{
						acciones.push_back(actFORWARD);
					}
				}

				break;
			}

			
		}
	}

	if (sensores.nivel < 2)
	{
		bienSituado = true;
		fil = sensores.posF;
		col = sensores.posC;
		brujula = sensores.sentido;
	}

	// G es la casilla celeste las cuales si se saben donde se situan en el mapa
	if (sensores.terreno[0] == 'G' && !bienSituado)
	{
		fil = sensores.posF;
		col = sensores.posC;
		bienSituado = true;
	}

	if (sensores.terreno[0] == 'K' && !bikiniOn)
	{
		bikiniOn = true;
	}

	if (sensores.terreno[0] == 'D' && !zapatillasOn)
	{
		zapatillasOn = true;
	}

	if (bienSituado)
	{
		rellenarVisionCompleta(sensores);
	}

	cout << "Position to go fuera: " << positionToGo.first << " " << positionToGo.second << endl;
 

	if (positionToGo.first != -1 and positionToGo.second != -1 and mapaResultado[positionToGo.first][positionToGo.second] != '?')
	{
		acciones.clear();
		positionToGo.first = -1;
		positionToGo.second = -1;
	}

	cout << "Sensores terreno 2: " << sensores.terreno[2] << endl;
	cout << "Sensores terreno 1: " << sensores.terreno[1] << endl;
	cout << "Giro " << giro << endl;
	

	/*if	(sensores.terreno[2] != 'S' and sensores.terreno[1] == 'S' and giro <= 0){
		cout << "Hola toy en lo nuevo pa la izq" << endl;
		acciones.erase(acciones.begin());
		acciones.insert(acciones.begin(), actTURN_L);
		acciones.insert(acciones.begin(), actFORWARD);
		acciones.insert(acciones.begin(), actTURN_R);
		acciones.insert(acciones.begin(), actFORWARD);
		acciones.insert(acciones.end(), actFORWARD);
		giro = 4;
	} else if(sensores.terreno[2] != 'S' and sensores.terreno[3] == 'S' and giro <= 0){
		cout << "Hola toy en lo nuevo pa la derecha" << endl;
		acciones.erase(acciones.begin());
		acciones.insert(acciones.begin(), actTURN_R);
		acciones.insert(acciones.begin(), actFORWARD);
		acciones.insert(acciones.begin(), actTURN_L);
		acciones.insert(acciones.begin(), actFORWARD);
		acciones.insert(acciones.begin(), actFORWARD);
		giro = 4;
	} else {
		giro--;
	}*/

	if (sensores.terreno[2] == 'P' and acciones.front() == actFORWARD and !acciones.empty() and (ultimaAccion != actTURN_L || ultimaAccion != actTURN_R))
	{
		while(acciones.front() == actFORWARD and !acciones.empty()){
			//cout << "Entrobucle" << endl;
			acciones.erase(acciones.begin());
			cout << acciones.front() << endl;
		}
		//cout << "Salgobucle" << endl;
	}

	cout << "Accion ? " << acciones.front() << " " << acciones.empty() << endl;

	

	if (sensores.terreno[2] == 'M' )
	{
		cout << "entro" << endl;
		acciones.insert(acciones.begin(), actTURN_R);
		acciones.insert(acciones.begin(), actFORWARD);
		acciones.insert(acciones.begin(), actTURN_L);
	}

	if (acciones.empty() and giro <= 0)
	{

		if(sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M'){

			if (sensores.terreno[2] == 'K' or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'G' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'T' and sensores.superficie[2] == '_')
			{
				acciones.push_back(actFORWARD);
				util--;
			}
			else if ((bikiniOn and sensores.terreno[2] == 'A') or (zapatillasOn and sensores.terreno[2] == 'B'))
			{
				acciones.push_back(actFORWARD);
				util--;
			}
			else if ((!bikiniOn and sensores.terreno[2] == 'A') or (!zapatillasOn and sensores.terreno[2] == 'B'))
			{
				
				util--;
			}
		} else {
			acciones.push_back(actIDLE);
		}
		
	}

	cout << "Accion realizada " << acciones.front() << endl;


	accion = acciones.front();
	acciones.erase(acciones.begin());

	cout << "Acciones restantes " << acciones.size() << endl;
	cout << "Accion siguiente " << acciones.front() << endl;

	ultimaAccion = accion;
	return accion;
}
int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
