#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include "../inc/VariablesFrogger.h"
#include "../inc/Inicializacion.h"
#include "../inc/MovimientoAmbiente.h"

void MoverCosas( Objeto **p_Ini ){
	Objeto *Act, *Aux;
	int i;
		
	Act = *(p_Ini);
	while(Act != NULL){
		if( strstr( Act->DirMov, "Izq" ) != NULL ){
			if( Act->Pos_x <= - Act->Ancho ){
				if( Act->Numero == 1 ) {
					Aux = Act;
					i = 1;
					while( i != CANT_OBJ_POR_LINEA ){
						Aux = Aux->Sig;
						i++;
					}
					Act->Pos_x = Aux->Pos_x + Act->Dif_x;						
				}
				else Act->Pos_x = Act->Ant->Pos_x + Act->Dif_x;							//Para ciclo continuo
			}
			else Act->Pos_x -= Act->Velocidad;
		}
		
		else if( strstr( Act->DirMov, "Der" ) != NULL ){
			if( Act->Pos_x >= SCREEN_W ){
				if( Act->Numero == 1 ) {
					Aux = Act;
					i = 1;
					while( i != CANT_OBJ_POR_LINEA ){
						Aux = Aux->Sig;
						i++;
					}
					Act->Pos_x = Aux->Pos_x - Act->Dif_x;						
				}
				else Act->Pos_x = Act->Ant->Pos_x - Act->Dif_x;							//Para ciclo continuo
			}
			else Act->Pos_x += Act->Velocidad;
		}
		
		Act = Act->Sig;
	}	
	RedibujarDesde( p_Ini );
}
