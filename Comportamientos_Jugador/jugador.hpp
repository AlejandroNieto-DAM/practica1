#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <queue>
#include <cmath>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

class ComportamientoJugador : public Comportamiento {

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = 99;
      filM = colM = 0;
      brujula = 0;
      ultimaAccion = actIDLE;
      bienSituado = false;
      bikiniOn = false;
      recarga = false;
      zapatillasOn = false;
      girar_izquierda = girar_derecha = false;
      contadorTriangulos = 0;
      canPuntuaciones = false;
      inicializarValoresCasillas();
      util = 0;
      positionToGo.first = -1;
      positionToGo.second = -1;
      accionesMuro = 0;
      giro = 0;

    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    void inicializarValoresCasillas();
    void rellenarVisionCompleta(Sensores sensores);
    void rellenarMemoria(Sensores sensores);
    void findPosition(int i);
    pair<int, int> getPos(int i);
    void calculoTriangulo(Sensores sensores);
    int interact(Action accion, int valor);

    void actualizarBrujulaPosicion();

  private:
  
  // Declarar aquí las variables de estado
  int fil, col, brujula;
  Action ultimaAccion;
  bool bienSituado, bikiniOn, zapatillasOn, recarga;
  bool girar_derecha, girar_izquierda, canPuntuaciones;
  vector<Action> acciones;
  map<char, int> valoresCasillas;
  vector<int> puntuaciones;
  int contadorTriangulos;
  int accionesMuro;
  vector<vector<pair<char, bool>>> memory;
  vector<vector<bool>> casillasUsadas;
  int filM, colM;
  int util;
  int giro;
  pair<int, int> positionToGo;

};

#endif
