#include<stdio.h>
#include<gtk/gtk.h>
#include<pthread.h>
#include<stdlib.h>
GtkWidget *box[16];
int M[4][4];
int suma[10]={0,0,0,0,0,0,0,0,0,0};

void close_clicked(GtkWidget *widget, gpointer window){
	int i = 0;
	gtk_widget_destroy(GTK_WIDGET(window));
	for(i = 0; i < 10; i++) suma[i] = 0;
}	
void sucess(int ans){

	GtkWidget *subWindow,*label1,*playAgain,*tryAgain,*label2,*des;
	subWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(subWindow), 310, 100);
	des = gtk_layout_new(NULL,NULL);
	playAgain=gtk_button_new_with_label("¡Play Again!");
	tryAgain=gtk_button_new_with_label("¡Try Again!, I'll NOT give up");
	label1 = gtk_label_new("¡Congratulations, you are the best one!");
	label2 = gtk_label_new("¡Sorry that's not a correct solve!");
	
	if(ans == 1){
		gtk_window_set_title(GTK_WINDOW(subWindow),"Congratulations");
		gtk_layout_put(GTK_LAYOUT(des),label1,50,20);
		gtk_layout_put(GTK_LAYOUT(des),playAgain, 120,45);
	}
	else{
		gtk_window_set_title(GTK_WINDOW(subWindow),"You miss");
		gtk_layout_put(GTK_LAYOUT(des),label2,75,15);
		gtk_layout_put(GTK_LAYOUT(des),tryAgain, 70,45);
	}
	
	
	gtk_container_add(GTK_CONTAINER(subWindow),des);
	g_signal_connect(playAgain,"clicked",G_CALLBACK(close_clicked),G_OBJECT(subWindow));
	g_signal_connect(tryAgain,"clicked",G_CALLBACK(close_clicked),G_OBJECT(subWindow));
	
	gtk_widget_show_all(subWindow);
}

void *columns(){
	int i,j, index=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			suma[index] = suma[index] + M[j][i];
			
		}
		
		index++;
	}
}


void *rows(){
	int i,j, index=4;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			suma[index] = suma[index] + M[i][j];
			
		}
		
		index++;
	}
}

void *diagonals(){
	int i,j;
	suma[8] = M[0][0] + M[1][1] + M[2][2] + M[3][3];
	suma[9] = M[0][3] + M[1][2] + M[2][1] + M[3][0];
}

int check_solution(){
	int i, cont = 0;
	pthread_t h1;
	pthread_t h2;
	pthread_t h3;
	pthread_create(&h1, NULL , columns , NULL);
	pthread_create(&h2, NULL , rows , NULL);
	pthread_create(&h3, NULL , diagonals, NULL);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	pthread_join(h3, NULL);
	
	for(i = 0;i < 9; i++){
		if(suma[i] == suma[i+1]) cont++;
	}
	printf("cont: %d\n",cont);
	if(cont == 9 ) return 1;
	else return 0;
}

void check_clicked(){
	//int M[4][4];
	int index=0,i,j;
	int a[16],b[1];
	//for(i=0;i<16;i++) a[i] = atoi(gtk_entry_get_text(GTK_ENTRY(box[i])));


	for(i = 0; i<4; i++){
		for(j = 0; j<4; j++){
			M[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(box[index])));;
			index++;
		}
		//index++;
	}
		for(i = 0; i<4; i++){
			for(j = 0; j<4; j++){
				printf("%d ",M[i][j]);
			}
			printf("\n");
		}

	int ans = check_solution(M);
	sucess(ans);
	for(i = 0; i<4; i++){
		printf("Columna %d: %d\n",i+1,suma[i]);
	}
	for(i = 4; i<8; i++){
		printf("Fila %d: %d\n",i+1,suma[i]);
	}
	printf("Diagonal 1:%d\n", suma[8]);
	printf("Diagonal 2:%d\n", suma[9]);


}



int main(int argc, char *argv[]){
    GtkWidget  *window, *close, *check, *rules[4],*design;
    int i,j, index=0;
    int row = 100, col = 120, aux = 10;
    gtk_init(&argc,&argv);
    //Creando una nueva ventana
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //Estableciendo propiedades a la ventana

    gtk_window_set_default_size(GTK_WINDOW(window), 550, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Magic Square Game - Sams");
    
    rules[0] = gtk_label_new("This is the MAGIC SQUARE GAME, simple rules:");
    rules[1] = gtk_label_new("-You have to insert in each field max. 2 digits.");
    rules[2] = gtk_label_new("-The correct solve in this game is: the sum of the numbers in every row and column");
    rules[3] = gtk_label_new("    ·The result of adding all the numbers in every row, column and diagonal must be the same.");
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    design = gtk_layout_new(NULL, NULL); 
    check = gtk_button_new_with_label("Check Solution");
    close = gtk_button_new_with_label("Close App");
    for(i = 0; i<4; i++){
    	gtk_layout_put(GTK_LAYOUT(design),rules[i],10,aux);
    	aux+=20;
    }	
    for(i=0;i<4;i++){
    	for(j=0;j<4;j++){
	    	box[index] = gtk_entry_new();
    		gtk_entry_set_max_length(GTK_ENTRY(box[index]),2);
    		gtk_entry_set_width_chars(GTK_ENTRY(box[index]),7);
    		gtk_layout_put(GTK_LAYOUT(design),box[index],row,col);
    		
    		row += 90;
    		index++;
    	}
    	col+=50;
    	row=100;
    }
    gtk_layout_put(GTK_LAYOUT(design),check,120, 330);
    gtk_layout_put(GTK_LAYOUT(design), close, 330, 330);
    gtk_container_add(GTK_CONTAINER(window), design);
    gtk_widget_show_all(window);
    g_signal_connect(close,"clicked",G_CALLBACK(close_clicked),G_OBJECT(window));
	g_signal_connect(check,"clicked",G_CALLBACK(check_clicked),G_OBJECT(window));
	
	
	
    gtk_main();
    return 0;
}
