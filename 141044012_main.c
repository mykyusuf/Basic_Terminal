#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define TAM 1000

void  parse(char *line, char **argv);

int main(void) {
    
    char  end[TAM];
    char  *argv[7]={"ls","cd","exit","pwd","cat","wc",NULL};
    char  *gdir;
    char  *adress;
    char  *newAdress;
    char buf[TAM];
    pid_t pid;
    int status;
    FILE *fp;
    int count=0;
    
    
    while (1) {
        
        printf("MYK-$: ");
        fgets(end, 1000, stdin);
        
        // Satir sonu olup olmadıgı kontrol edilir
        if (end[strlen(end) - 1] == '\n')
            end[strlen(end) - 1] = '\0';
        
        parse(end, argv);
        
        
        // Exit komutu varsa cikilir
        if (!strcmp(argv[0], "exit")) exit(0);
        
        
        pid = fork();
        
        if (pid == 0) {
            
            
            if (!strcmp(argv[0], "help")){
                
                printf("Kullanilabilir Komutlar: \n 1) cd \n 2) ls \n 3) pwd \n 4) cat \n 5) wc \n &) exit \n");
                
                
            }
            
            else if (!strcmp(argv[0], "cd")){
                
                gdir = getcwd(buf, sizeof(buf));
                adress = strcat(gdir, "/");
                newAdress = strcat(adress, argv[1]);
                
                chdir(newAdress);
                
                continue;
                
            }
            
            else if (!strcmp(argv[0], "cat")){
                
                if(argv[1]!=NULL){
                fp = fopen(argv[1], "r");
                
                fread(buf,TAM, 1, fp);
                    printf("%s\n", buf);
                
                fclose(fp);

                }
                else
                    printf("Eksik Arguman! Txt dosyasi veriniz !\n");
                
            }
            
            else if (!strcmp(argv[0], "we")){
                
                if(argv[1]!=NULL){
                fp = fopen(argv[1],"r");
                
                while( fgets (buf, 60, fp)!=NULL ) {
                    count+=1;
                }
                printf("Line sayisi: %d \n",count);
                fclose(fp);
                }
                else
                    printf("Eksik Arguman! Txt dosyasi veriniz !\n");

                
            }
            else if(!strcmp(argv[0],"ls")){
                
            printf("Listeleniyor: \n");
            
                if (execvp("ls", argv) < 0) {
                
                printf("%s: Gecersiz Komut !", argv[0]);
                exit(1);
            }
            
                
            }
            else
                printf("Gecersiz Komut Girdiniz! \n");

        }else {
            
            waitpid(pid,&status,0);
        }
        
        
        
    }
    return 0;
}

void  parse(char *line, char **argv){
    
    while (*line != '\0') {
        while (*line == ' ' || *line == '\t' || *line == '\n')
            
            *line++ = '\0';
        *argv++ = line;
        
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
            line++;             
    } 
    *argv = NULL;

}
