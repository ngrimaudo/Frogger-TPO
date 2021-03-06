#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include "../inc/VariablesFrogger.h"
#include "../inc/Inicializacion.h"

void RedibujarDesde( Objeto **p_DesdeAca ){
	Objeto *Act;
	
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	
	Act = *(p_DesdeAca);
	while(Act != NULL){
		al_draw_bitmap( Act->Imagen, Act->Pos_x, Act->Pos_y, 0 ); //dibujamos todo con 0 flags
		Act = Act->Sig;
	}
	al_flip_display(); //mostramos
}

int InicializarALLEGRO( ALLEGRO_DISPLAY **p_display, ALLEGRO_EVENT_QUEUE **p_event_queue, ALLEGRO_TIMER **p_resting_timer, ALLEGRO_TIMER **p_frames_timer, ALLEGRO_BITMAP ***p_VecFrog, bool **p_key ){
	
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -5;
	}
	
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -5;
	}
	
	*(p_resting_timer) = al_create_timer(FROG_RESTING_TIME);
	if( ! (*p_resting_timer) ) {
		fprintf(stderr, "failed to create timer!\n");
		return -5;
	}
	
	*(p_frames_timer) = al_create_timer(FRAME_RATE);
	if( ! (*p_frames_timer) ) {
		fprintf(stderr, "failed to create timer!\n");
		return -5;
	}
	
	if(!al_init_image_addon()) {
		al_show_native_message_box( (*p_display), "Error", "Error", "Failed to initialize image addon!", 
					NULL, ALLEGRO_MESSAGEBOX_ERROR );
		return -5;
	}
	
	*(p_display) = al_create_display(SCREEN_W, SCREEN_H);
	if( ! (*p_display) ) {
		fprintf(stderr, "failed to create display!\n");
		return -5;
	}
		
	*(p_event_queue) = al_create_event_queue(); 		//creamos cola de eventos
	if( ! (*p_event_queue) ) {
		fprintf(stderr, "failed to create event_queue!\n");
		return -5;
	}
	
	*(p_VecFrog) = ( ALLEGRO_BITMAP ** ) malloc( 8 * sizeof(ALLEGRO_BITMAP *) );
		if( (*p_VecFrog) == NULL ){
			fprintf(stderr, "No hay memoria para VecFrog!\n");
			return -5;
		}
	CargarSprites( p_VecFrog, "Imagenes/" );
	
	*(p_key) = ( bool * ) malloc( 4 * sizeof(bool) );
		if( (*p_key) == NULL ){
			fprintf(stderr, "No hay memoria para Key!\n");
			return -5;
		}
	CargarKey( p_key );
	
	/*aaa*/
	fprintf(stderr, "\n KEY: %d %d %d %d ", (*p_key)[0], (*p_key)[1], (*p_key)[2], (*p_key)[3]);
	/*aaa*/	
	
	al_register_event_source( (*p_event_queue), al_get_display_event_source(*p_display) ); //conectamos eventos de la pantalla 
	al_register_event_source( (*p_event_queue), al_get_keyboard_event_source() );
	al_register_event_source( (*p_event_queue), al_get_timer_event_source(*p_resting_timer) );
	al_register_event_source( (*p_event_queue), al_get_timer_event_source(*p_frames_timer) );
	
	return 0;		
}

void CargarSprites( ALLEGRO_BITMAP ***p_VecFrog, const char *CarpetaContenedora ){
	char RutaFinal[100];
	ALLEGRO_BITMAP **VecFrog = *(p_VecFrog);
	RutaFinal[0] = '\0';
	
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaArriba.png");
	VecFrog[0] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaSaltoArriba.png");
	VecFrog[1] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaAbajo.png");
	VecFrog[2] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaSaltoAbajo.png");
	VecFrog[3] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaIzquierda.png");
	VecFrog[4] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaSaltoIzquierda.png");
	VecFrog[5] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaDerecha.png");
	VecFrog[6] = al_load_bitmap(RutaFinal);
	strcpy(RutaFinal, CarpetaContenedora);
	strcat(RutaFinal, "/RanaSaltoDerecha.png");
	VecFrog[7] = al_load_bitmap(RutaFinal);
}	

void CargarKey( bool **p_key ){
	bool *key = *(p_key);
	
	key[0] = false;
	key[1] = false;
	key[2] = false;
	key[3] = false;	
}

int EnlistarObjetos( Objeto **p_Ini,  Objeto **p_ObjetoFrog ){
	FILE *fp;
	Objeto *New;
	char *Nombre, *Numero, *RutaImagen, *DirMov, *Velocidad, *Alto, *Ancho, *Dif_x;
	
	fp = fopen( "Archivos/ObjetosCreados.txt", "r+" );
		if( fp == NULL ) return -5;
	
	Nombre = (char *) malloc( 10 * sizeof(char) );
		if( Nombre == NULL ) return -5;
	Numero = (char *) malloc( 3 * sizeof(char) );
		if( Numero == NULL ) return -5;
	RutaImagen = (char *) malloc( 75 * sizeof(char) );
		if( RutaImagen == NULL ) return -5;
	DirMov = (char *) malloc( 4 * sizeof(char) );
		if( DirMov == NULL ) return -5;
	Velocidad = (char *) malloc( 4 * sizeof(char) );
		if( Velocidad == NULL ) return -5;
	Alto = (char *) malloc( 4 * sizeof(char) );
		if( Alto == NULL ) return -5;
	Ancho = (char *) malloc( 4 * sizeof(char) );
		if( Ancho == NULL ) return -5;
	Dif_x = (char *) malloc( 25 * sizeof(char) );
		if( Dif_x == NULL ) return -5;
		
	New = *p_Ini;
	fscanf( fp, "%[^\t]", Nombre );
		
	while( strstr(Nombre, "FIN") == NULL ){
		fscanf( fp, "\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n", Numero, RutaImagen, DirMov, Velocidad, Alto, Ancho, Dif_x );		
		
		// Lista Vacía
		if( *(p_Ini) == NULL ){
			*(p_Ini) = (Objeto *) malloc( sizeof(Objeto)  );
				if( (*p_Ini) == NULL ) return -5;			
			New = *(p_Ini);
			New->Ant = NULL;
		}
		// Lista NO Vacía
		else {
			New->Sig = (Objeto *) malloc( sizeof(Objeto) );
				if( New->Sig == NULL ) return -5;
			New->Sig->Ant = New;
			New = New->Sig;
		}
		
		if( CargarDatosEnObjeto( New, Nombre, Numero, RutaImagen, DirMov, Velocidad, Alto, Ancho, Dif_x ) == -5 ) return -5;		
		if( strstr(Nombre, "Frog") != NULL && atoi(Numero) == 1 ) *(p_ObjetoFrog) = New;
		
		fscanf( fp, "%[^\t]\t", Nombre );
	}
	New->Sig = NULL;
	
	TerminarDeAcomodarLista( p_Ini );

	RedibujarDesde( p_Ini );
	fclose( fp );
	free( Nombre );
	free( Numero );
	free( RutaImagen );
	free( DirMov );
	free( Velocidad );
	free( Alto );
	free( Ancho );
	free( Dif_x );
	
	return 0;
}

int CargarDatosEnObjeto( Objeto *New, char *Nombre, char *Numero, char *RutaImagen, char *DirMov, char *Velocidad, char *Alto, char *Ancho, char *Dif_x ){
	
	New->Nombre = (char *) malloc( 10 * sizeof(char) );
		if( New->Nombre == NULL ) return -5;
	New->DirImagen = (char *) malloc( 75 * sizeof(char) );
		if( New->DirImagen == NULL ) return -5;
	New->DirMov = (char *) malloc( 4 * sizeof(char) );
		if( New->DirMov == NULL ) return -5;
	
	strcpy( New->Nombre, Nombre );
	New->Numero = atoi( Numero );
	strcpy( New->DirImagen, RutaImagen );
	strcpy( New->DirMov, DirMov );
	New->Imagen = al_load_bitmap( New->DirImagen );
		if( New->Imagen == NULL ){
			fprintf(stderr, "\n\n\tError al cargar la imagen %s, de %s.", RutaImagen, Nombre);
			return -5;
		}

	if( strstr(New->Nombre, "Frog") != NULL && atoi(Numero) == 1 ){
		New->Velocidad = 0;
		New->Alto = FROG_SIZE;
		New->Ancho = FROG_SIZE;
		New->Dif_x = 0;	
		New->Pos_x = X_INI_FROG;
		New->Pos_y = Y_INI_FROG;
	}
	else if( strstr(New->Nombre, "Back") != NULL ){
		New->Velocidad = 0;
		New->Alto = 0;
		New->Ancho = 0;
		New->Dif_x = 0;
		New->Pos_x = 0;
		New->Pos_y = 0;
	}
	else{
		New->Velocidad = atof( Velocidad );
		New->Alto = atof( Alto );
		New->Ancho = atof( Ancho );
		New->Dif_x = atof( Dif_x );	
		if( strstr( New->Nombre, "Tronco2" ) != NULL ) New->Pos_y = UPPER_OFFSET + BOUNCE_Y + 8;
		if( strstr( New->Nombre, "Tortuga2" ) != NULL ) New->Pos_y = UPPER_OFFSET + 2 * BOUNCE_Y + 8;
		if( strstr( New->Nombre, "Tronco3" ) != NULL ) New->Pos_y = UPPER_OFFSET + 3 * BOUNCE_Y + 8;
		if( strstr( New->Nombre, "Tronco1" ) != NULL ) New->Pos_y = UPPER_OFFSET + 4 * BOUNCE_Y + 8;
		if( strstr( New->Nombre,"Tortuga3" ) != NULL ) New->Pos_y = UPPER_OFFSET + 5 * BOUNCE_Y + 8;
		if( strstr( New->Nombre,"Auto1" ) != NULL ) New->Pos_y = UPPER_OFFSET + 7 * BOUNCE_Y + 8;
		if( strstr( New->Nombre,"Auto2" ) != NULL ) New->Pos_y = UPPER_OFFSET + 8 * BOUNCE_Y + 8;
		if( strstr( New->Nombre,"Auto3" ) != NULL ) New->Pos_y = UPPER_OFFSET + 9 * BOUNCE_Y + 8;
		if( strstr( New->Nombre,"Auto4" ) != NULL ) New->Pos_y = UPPER_OFFSET + 10 * BOUNCE_Y + 8;
		if( strstr( New->Nombre,"Auto5" ) != NULL ) New->Pos_y = UPPER_OFFSET + 11 * BOUNCE_Y + 8;
	}
	return 0;
}

void TerminarDeAcomodarLista( Objeto **p_Ini ){
	Objeto *Act;
	
	Act = *p_Ini;
	
	while( Act != NULL ){
		if( strstr( Act->DirMov, "Izq" ) != NULL ){
			if( Act->Numero == 1 ) Act->Pos_x = SCREEN_W;
			else Act->Pos_x = Act->Ant->Pos_x + Act->Dif_x;
		}
		if( strstr( Act->DirMov, "Der" ) != NULL ){
			if( Act->Numero == 1 ) Act->Pos_x = 0 - Act->Ancho;
			else Act->Pos_x = Act->Ant->Pos_x - Act->Dif_x;
		}
		Act = Act->Sig;
	}
}

void FinalizarALLEGRO( ALLEGRO_DISPLAY **p_display, ALLEGRO_EVENT_QUEUE **p_event_queue, ALLEGRO_TIMER **p_resting_timer, ALLEGRO_TIMER **p_frames_timer, ALLEGRO_BITMAP ***p_VecFrog, bool **p_key ){
	
	al_destroy_timer( *(p_resting_timer) );
	al_destroy_timer( *(p_frames_timer) );
	al_destroy_display( *(p_display) );
	al_destroy_event_queue( *(p_event_queue) );
	free( *(p_VecFrog) );
	free( *(p_key) );	
}

void LiberarMemoria( Objeto **p_Ini ){
	Objeto *Act, *Ant;
	
	Act = *(p_Ini);
	while(Act != NULL){
		Ant = Act;
		Act = Act->Sig;
		free(Ant->Nombre);
		free(Ant->DirImagen);
		free(Ant);
	}
}

void InicializarTimers( ALLEGRO_TIMER **resting_timer, ALLEGRO_TIMER **frames_timer ){
	al_start_timer(*resting_timer);		
	al_start_timer(*frames_timer);	
}

