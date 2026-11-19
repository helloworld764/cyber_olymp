#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static void log_event(const char*s){if(s){fputs(s,stdout);putchar('\n');}}
static void greet(void){char n[64];fputs("name> ",stdout);if(fgets(n,sizeof n,stdin)){n[strcspn(n,"\n")]=0;printf("Hello, %s!\n",n);}}
static void note(const char*s){char b[64];if(!s)return;if((size_t)snprintf(b,sizeof b,"%s",s)>=sizeof b){fputs("too long\n",stderr);return;}log_event(b);}
typedef struct{char v[32];}Session;
static Session*open_session(void){Session*s=calloc(1,sizeof*s);if(s)snprintf(s->v,sizeof s->v,"SESSION=OK");return s;}
static void close_session(Session**p){if(p&&*p){free(*p);*p=0;}}
static int admin(const char*s){const char*t=getenv("ADMIN_TOKEN");return t&&s&&!strcmp(s,t);}
static int report(const char*n){char p[256];if(!n!*nstrlen(n)>200strstr(n,"..")strchr(n,'/')||strchr(n,'\\'))return-1;snprintf(p,sizeof p,"/var/log/%s",n);pid_t x=fork();if(x<0)return-1;if(!x){execl("/bin/cat","cat",p,(char*)0);_exit(127);}int st;waitpid(x,&st,0);return WIFEXITED(st)?WEXITSTATUS(st):-1;}
static int cfg(int i){static const int a[]={1,2,3,4,5,6,7,8};return i>=0&&i<8?a[i]:-1;}
static int tmp(char p[]){strcpy(p,"/tmp/app-XXXXXX");int f=mkstemp(p);if(f<0)return-1;close(f);return 0;}
static unsigned long sum(const unsigned char*s){unsigned long r=0;for(size_t i=0;s&&s[i];i++)r+=s[i];return r;}
int main(int c,char**v){if(c<2)return 1;if(!strcmp(v[1],"greet"))greet();else if(!strcmp(v[1],"note")&&c>2)note(v[2]);else if(!strcmp(v[1],"admin")&&c>2)puts(admin(v[2])?"GRANTED":"DENIED");else if(!strcmp(v[1],"report")&&c>2)report(v[2]);else if(!strcmp(v[1],"cfg")&&c>2)printf("%d\n",cfg(atoi(v[2])));else if(!strcmp(v[1],"cksum")&&c>2)printf("%lu\n",sum((unsigned char*)v[2]));Session*s=open_session();close_session(&s);}
