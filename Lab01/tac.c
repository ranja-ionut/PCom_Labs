#include <unistd.h> /* pentru open(), exit() */
#include <fcntl.h> /* O_RDWR */
#include <errno.h> /* perror() */
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
void fatal(char * mesaj_eroare)
{
    perror(mesaj_eroare);
    exit(-1);
}
     
int main(int argc,char *argv[])
{
    int miner_sursa, miner_destinatie;
    int i=0;
    char **tac,**cat;
    tac = malloc(1024*sizeof(char*));
    cat = malloc(1024*sizeof(char*));
    for(int j=0;j<1024;j++){
        tac[j]=malloc(1024);
        cat[j]=malloc(1024);
    }
    if(argc==1){
		printf("Eroare!\n");
		exit(-1);
	}
    miner_sursa = open(argv[1], O_RDONLY);
    miner_destinatie = STDOUT_FILENO;
     
    if (miner_sursa < 0 || miner_destinatie < 0)
        fatal("Nu pot deschide un fisier");
     
    lseek(miner_sursa, 0, SEEK_SET);
    lseek(miner_destinatie, 0, SEEK_SET);
    FILE *f=fopen(argv[1],"r");
    while (!feof(f)) {
        char *line = readline(f);
        strcpy(cat[i++],line);
    }    
    int j=i-1;
    int k=0;
    while(j>=0){
        printf("%s",cat[j]);    
    }
 
    close(miner_sursa);
    close(miner_destinatie);
    return 0;
}


