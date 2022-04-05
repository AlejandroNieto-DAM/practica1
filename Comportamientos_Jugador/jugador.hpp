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
      
      brujula = 0;
      ultimaAccion = actIDLE;
      bienSituado = false;
      choque = false;
      bikiniOn = false;
      recarga = false;
      zapatillasOn = false;
    
      positionToGo.first = -1;
      positionToGo.second = -1;
      accionesMuro = 0;
      giro = 0;
      filp = colp = 99;

    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    void rellenarVisionCompleta(Sensores sensores);
    void findPosition(int i);
    pair<int, int> getPos(int i);
    int interact(Action accion, int valor);

    void actualizarBrujulaPosicion();
    void actualizarPosicion();
  private:
  
  // Declarar aquí las variables de estado
  int fil, col, brujula, filp, colp;
  Action ultimaAccion;
  bool bienSituado, bikiniOn, zapatillasOn, recarga, choque;
  vector<Action> acciones;
  map<char, int> valoresCasillas;
  int accionesMuro;
  int giro;
  pair<int, int> positionToGo;

};

#endif
