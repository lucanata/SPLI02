#include "my.h"
#include<string.h>
#include<ctype.h>

#define N 2000

void liv7(u_int len,const u_char *p, u_int ssap, u_int dsap){
  int i, k=0;
  // variabili provvisorie
  //int r_http=1;
  //int p_http=1;
  int p_http_body=0;
  u_char buffer[(int) len];
  char aux[(int) len];
  //u_char buffer[N];
  //char aux[N];
  char *buf2;
  memset(buffer, 0, sizeof buffer);
  memset(buffer, 0, sizeof aux);
  // contatore del numero di spazi
  int count_space = 0;
  // contatore di new line
  int count_n = 0;
  // flag che indica che la parola è completata
  int flag_word=0;
  // flag che indica se siamo nella prima riga
  int first_line=1;
  // flag che indica una http request
  int flag_http_req=0;
  // flag che indica una http response
  int flag_http_res=0;
 /*
  * Per quanto riguarda il livello 7 dal livello 4 dobbiamo passare come 
  * parametro la porta destinazione e quella sorgente. nel caso una delle due
  * sia uguale ad 80 vuol dire che ci troviamo nel caso del protocollo http
  * da lì bisogna analizzare il pacchetto e stampare le informazioni.
  */
  if((int)len<=0)return;
  colore(5);
  
  if (ssap==80 || dsap==80){
      if(!r_http) return;
      if (ssap==80) printf("HTTP Response|\n"), flag_http_res=1;
      else printf("HTTP Request| \n"), flag_http_req=1;
      if (p_http){
          
          for (i=1; i<= len; i++){
              /*
              if ((* (char *)p)==' '){
                  ++count_space;
                  if (count_space==1)
                      flag_word = 1;
                  //da completare
                   
              }
               */
              if ((* (char *)p)=='\r' || (* (char *)p)=='\t'){
                  p++;
                  continue;
              }
              if ((* (char *) p )=='\n'){
                  ++count_n;
                  buffer[k]='\0';
                  strcpy(aux, (char *) buffer);
                  if (count_n==2){
                      // Fine Header
                      break;
                  }
                  if (first_line){
                      first_line=0;
                      buf2=strtok((char *)buffer, " ");
                      if (flag_http_req && buf2!=NULL && control_header_req(buf2)){
                          printf("Status-Line: ");
                          printf("%s\n", aux);
                          /*
                          printf("%s ", buf2);
                          buf2 = strtok(NULL, " ");
                          printf("%s ", buf2);
                          buf2 = strtok(NULL, " ");
                          printf("%s\n", buf2);
                          */
                      }
                      
                      else {if (flag_http_res && buf2!=NULL && control_header_res(buf2)){
                          printf("Status-Line: ");
                          printf("%s\n", aux);
                          //printf("%s ", buf2);
                          //buf2 = strtok(NULL, " ");
                          //printf("%s ", buf2);
                          //buf2 = strtok(NULL, " ");
                          //printf("%s\n", buf2);
                          
                      }
                      else{ //non è un header ma la continuazione del body precedente
                          printf("Chunk of the body...\n");
                          break;
                      }
                      }
                  }
                  else {
                      //da completare
                      buf2 = strtok((char *)buffer, ":");
                      if (analyze(buf2)){
                          printf("%s: ", buf2);
                          buf2 = (char *)buffer + strlen(buf2) + 2;
                          //buf2 = strtok(NULL, ":");
                          printf("%s\n", buf2);
                      }
                      else
                          //continue;
                          ;   
                  }
                  k=0;
                  //memset(buffer, 0, sizeof buffer);
                  //memset(buffer, 0, sizeof aux);
              }
              else{
                  if(isascii(*p))buffer[k] = (*p);
                  else buffer[k]=' ';
                  count_n = 0;
                  k++;
              }
              p++;
          }
          myprintf("\n");
          decoded=1;
          return;
          /*
          for (i=0; i<3; i++) {
          sscanf((char *)p, "%s", buffer);
          myprintf("%s", buffer);
          }
          myprintf("\n");
          if (strcmp(buffer, "Host")){}
          if (strcmp(buffer, ))
          printf("%s\n", buffer);
           */
      }
  }
  /*else {
    myprintf("APPL |");
    for(i=1;i<=len;i++){
      //if(isprint(*p))myprintf("%c",*p);
      //else myprintf(".");
      if(isascii(*p))myprintf("%c",*p);
      else myprintf(".");
      if(isascii(*p))fprintf(mem,"%c",*p);
      else fprintf(mem,".");
      p++;
      if((i%70)==0)myprintf("\n     |");
    }
  }*/
  //myprintf("\n");
//  fflush(mem);
  //decoded=1;
}
