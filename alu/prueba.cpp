#include "header.h"
using namespace std;

bool estado = false;                 //siendo true viva, false muerta
int muertos = 0;
int vivos = 0;
vector<bool> vivosMuertos = [];

void(vector<bool> vivosMuertos){
    for (int i= 0, i < vivosMuertos.size(), i++){
        if (vivosMuertos[i] = false){
            muertos++;
        }
        if (vivosMuertos[i] = true){
            vivos++;
        }
    }
    if (estado ==false) || (vivos == 3){
        estado = true;
    }

    if (estado ==true) || (vivos == 3) || (vivos == 2){
        estado = true;
    }
    else{
        estado = false;
    }
    cout<<estado<<endl;
}   