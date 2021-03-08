#include <unistd.h> /* pentru open(), exit() */
#include <fcntl.h> /* O_RDWR */
#include <errno.h> /* perror() */
#include <stdlib.h> 
#include <stdio.h>
void fatal(char * mesaj_eroare)
{
    perror(mesaj_eroare);
    exit(-1);
}
     
int main(int argc,char *argv[])
{
    int miner_sursa, miner_destinatie;
    int copiat;
    char buf[1024];
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
     
    while ((copiat = read(miner_sursa, buf, sizeof(buf)))) {
        if (copiat < 0)
            fatal("Eroare la citire");
        copiat = write(miner_destinatie, buf, copiat);
        if (copiat < 0)
            fatal("Eroare la scriere");
    }
    
    close(miner_sursa);
    close(miner_destinatie);
    return 0;
}


