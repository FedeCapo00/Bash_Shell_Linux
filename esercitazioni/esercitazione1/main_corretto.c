#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define ARGOEMNTI 3
#define MAXLEAF 10

FILE * ptr;
int v_leaf[MAXLEAF],index_leaf=0;

void write_pid(){
    fprintf(ptr,"%d\n",getpid());
    fflush(ptr);
}

bool file_doesnt_exists(const char * path){
    FILE *tmp=fopen(path,"r");
    bool trovato=true;
    if(tmp!=NULL){
        trovato=false;
    }
    return trovato;
}

void leaf_handler(int signo,siginfo_t* info,void *empty){
    if(signo==SIGUSR1){
        kill(info->si_value.sival_int,SIGUSR2);
        kill(getpid(),SIGTERM); // Il processo uccide se stesso
    }
}

void generate_leaf(int n_leafs){
    int i;
    for(i=0;i<n_leafs;++i){
        int leaf=fork();
        if(leaf==0){
            struct sigaction sl;
            sl.sa_flags=SA_SIGINFO;
            sigemptyset(&sl.sa_mask);
            sl.sa_handler=leaf_handler;
            sigaddset(&sl.sa_mask,SIGCHLD);
            sigaddset(&sl.sa_mask,SIGCONT);
            sigaction(SIGUSR1,&sl,NULL);
            write_pid();
            while (1){
                pause();
            }
        }
        else{
            v_leaf[index_leaf]=leaf;
            index_leaf++;
        }
    }
}

void manager_handler(int signo,siginfo_t* info,void *empty){
    if(signo==SIGUSR1){
        printf("Il manager ha ricevuto un SIGUSR1\n");
        if(index_leaf>0){
            index_leaf--;
            union sigval x_pid;
            x_pid.sival_int=info->si_pid;
            sigqueue(v_leaf[index_leaf],SIGUSR1,x_pid);
        }
        else{
            kill(getpid(),SIGKILL);
        }
    }
    if(signo==SIGTERM){
        index_leaf--;
        for(;index_leaf>=0;--index_leaf){
            kill(v_leaf[index_leaf],SIGKILL);
        }
        kill(getpid(),SIGKILL);
    }
}

int main(int argc, char ** argv){
    int n_argomenti=argc;
    if(n_argomenti!=ARGOEMNTI){
        exit(3);
    }
    int n_figli=atoi(argv[2]);
    if(n_figli<1 || n_figli>10){
        exit(4);
    }
    if(!file_doesnt_exists(argv[1])){
        exit(5);
    }
    ptr=fopen(argv[1],"w");
    write_pid();
    int manager=fork();
    if(manager==0){
        struct sigaction sa;
        sa.sa_flags=SA_SIGINFO;
        sa.sa_handler=manager_handler;
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask,SIGALRM);
        sigaction(SIGUSR1,&sa,NULL);
        sigaction(SIGTERM,&sa,NULL);
        write_pid();
        generate_leaf(n_figli);
        while (1)pause();
    }
    else{
        while(wait(NULL)>0);
    }
    return 0;
}