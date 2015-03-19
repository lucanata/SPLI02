#include "my.h"

char fg[]={0,1,2,3,4,5,6};
char bg[]={8,8,8,8,8,8,8};

void colore(int col){
  col%=sizeof(fg);
  myprintf("\033[0;%d;%dm",30+fg[col],40+bg[col]);	
}

void o_colore(int col){
  col%=sizeof(fg);
  printf("\033[0;%d;%dm",30+fg[col],40+bg[col]);	
}


void myprintf(const char *fmt, ...){
  va_list ap;
  va_start(ap,fmt);
  vsprintf(outbuf+olen,fmt,ap);
  va_end(ap);
  olen+=strlen(outbuf+olen);
}

void print_liv2(const u_char *p){
  int i;
  for(i=0;;i++){
    myprintf("%x",*p++);
    if(i==5)break;
    myprintf(":");
  }
}

void print_ipv4(const u_char *p){
  const u_char *mp;
  int i;
  for(mp=p,i=0;;i++){
    myprintf("%d",*mp++);
    if(i==3)break;
    myprintf(".");
  }
}

void print_ipv6(const u_char *p){
  const u_char *mp;
  int i;
  for(mp=p,i=0;;i++){
    myprintf("%x",ntohs(*(u_int *)mp));
    if(i==7)break;
    mp+=2;
    myprintf(":");
  }
}

// Controllo header HTTP della request
int control_header_req(char *buf){
    if (strcmp(buf,"OPTIONS")==0)
        return 1;
    if (strcmp(buf,"GET")==0)
        return 1;
    if (strcmp(buf,"HEAD")==0)
        return 1;
    if (strcmp(buf,"POST")==0)
        return 1;
    if (strcmp(buf,"PUT")==0)
        return 1;
    if (strcmp(buf,"DELETE")==0)
        return 1;
    if (strcmp(buf,"TRACE")==0)
        return 1;
    if (strcmp(buf,"CONNECT")==0)
        return 1;
    else return 0;
}

// Controllo header HTTP della response
int control_header_res(char *buf){
    //remove_spaces(buf);
    if (strcmp(buf, "HTTP/1.0")==0)
        return 1;
    if (strcmp(buf, "HTTP/1.1")==0)
        return 1;
    else return 0;
}

// Controllo contenuto campi Header passando il nome del campo
int analyze(char *buf){
    if (strcasecmp(buf, "Accept-Charset")==0)
        return 1;
    if (strcasecmp(buf, "Accept-Encoding")==0)
        return 1;
    if (strcasecmp(buf, "Accept-Language")==0)
        return 1;
    if (strcasecmp(buf, "From")==0)
        return 1;
    if (strcasecmp(buf, "Host")==0)
        return 1;
    if (strcasecmp(buf, "If-Modified-Since")==0)
        return 1;
    if (strcasecmp(buf, "If-Unmodified-Since")==0)
        return 1;
    if (strcasecmp(buf, "User-Agent")==0)
        return 1;
    if (strcasecmp(buf, "Content-Type")==0)
        return 1;
    if (strcasecmp(buf, "Content-Length")==0)
        return 1;
    if (strcasecmp(buf, "Last-Modified")==0)
        return 1;
    if (strcasecmp(buf, "Expires")==0)
        return 1;
    if (strcasecmp(buf, "Age")==0)
        return 1;
    if (strcasecmp(buf, "Location")==0)
        return 1;
    if (strcasecmp(buf, "Retry-After")==0)
        return 1;
    if (strcasecmp(buf, "Server")==0)
        return 1;
    if (strcasecmp(buf, "Connection")==0)
        return 1;
    if (strcasecmp(buf, "Date")==0)
        return 1;
    else
        return 0;
    // Ritorno 1 se matcha 0 se no
}

// Tolgo gli spazi da source fino allo 0
void remove_spaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}