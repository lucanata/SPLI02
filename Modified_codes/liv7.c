#include "my.h"
#include<string.h>
#include<ctype.h>

void liv7(u_int len,const u_char *p,u_int sport,u_int dport){
  int i;
  int flag_telnet_res=0, flag_http_res=0;
  int flag_telnet_req=0, flag_http_req=0;
  int p_http_body=0;

  int status_line=1;
  u_char buffer[(int) len];
  char aux[(int) len];
  memset(buffer, 0, sizeof buffer);
  memset(buffer, 0, sizeof aux);
  char *tokenized;
  int count_space = 0;
  int count_n=0;
  int k=0;

  if((int)len<=0)return;

  if(sport==23||dport==23){
    if(!r_telnet) return;
    if(p_telnet){
      colore(8);
      myprintf("Telnet   |");    
  
      if(sport==23) { 
        myprintf("###Response###\n"); 
        flag_telnet_res=1;
      }
      else {
        myprintf("###Request###\n");
        flag_telnet_req=1;
      }
    }
    myprintf("\n");
    fflush(mem);
    decoded=1;
  }
  else{
    if(sport==80||dport==80){
      if(!r_http) return;
      if(p_http){
        colore(5);
        myprintf("Http     |");
        if(sport==80) {
          myprintf("###Response###\n");
          flag_http_res=1;
        }
        else{
          myprintf("###Request###\n");
          flag_http_req=1;
        }
        for (i=1; i<= len; i++){
          if ((* (char *)p)=='\r' || (* (char *)p)=='\t'){      // Scorro caratteri di formattazione
            p++;
            continue;
          }
          if ((* (char *) p )=='\n'){                           // Conto gli a capo (se sono 2 ho finito l'header)
            ++count_n;
            buffer[k]='\0';
            strcpy(aux, (char *) buffer);
            if (count_n==2){
              break;
            }
            if (status_line){                               // Se sono nella prima riga
              status_line=0;
              tokenized=strtok((char *)buffer, " ");
              if (flag_http_req && tokenized!=NULL){
                if((strcmp(tokenized,"OPTIONS")==0)||(strcmp(tokenized,"GET")==0)||(strcmp(tokenized,"HEAD")==0)||(strcmp(tokenized,"POST")==0)||(strcmp(tokenized,"PUT")==0)||(strcmp(tokenized,"DELETE")==0)||(strcmp(tokenized,"TRACE")==0)||(strcmp(tokenized,"CONNECT")==0)){
                  myprintf("         |Status-Line: ");
                  myprintf("%s\n", aux);
                } 
              }
              else {
                if (flag_http_res && tokenized!=NULL){
                  if((strcmp(tokenized,"HTTP/1.0")==0)||(strcmp(tokenized,"HTTP/1.1")==0)){
                    myprintf("         |Status-Line: ");
                    myprintf("%s\n", aux);
                  } 
                  else{                                       // Se non è un header
                    myprintf("         |####BODY####\n");
                  break;
                  }
                }
              }
            }
            else {
              tokenized = strtok((char *)buffer, ":");
              if (strcasecmp(tokenized,"Accept-Charset")==0||strcasecmp(tokenized, "Accept-Encoding")==0||strcasecmp(tokenized, "Accept-Language")==0||strcasecmp(tokenized,"Cookie")==0||strcasecmp(tokenized, "From")==0||strcasecmp(tokenized, "Host")==0||strcasecmp(tokenized, "If-Modified-Since")==0||strcasecmp(tokenized, "If-Unmodified-Since")==0||strcasecmp(tokenized, "User-Agent")==0||strcasecmp(tokenized, "Content-Type")==0||strcasecmp(tokenized, "Content-Length")==0||strcasecmp(tokenized,"Referer")==0||strcasecmp(tokenized, "Last-Modified")==0||strcasecmp(tokenized, "Expires")==0||strcasecmp(tokenized, "Age")==0||strcasecmp(tokenized, "Location")==0||strcasecmp(tokenized, "Retry-After")==0||strcasecmp(tokenized, "Server")==0||strcasecmp(tokenized, "Connection")==0||strcasecmp(tokenized, "Date")==0){
                myprintf("         |%s: ", tokenized);
                tokenized = (char *)buffer + strlen(tokenized) + 2;
                myprintf("%s\n", tokenized);
              }
            }
            k=0;
          }
          else{
            if (isascii(*p))
              buffer[k] = (*p);
            else 
              buffer[k]=' ';
            count_n = 0;
            k++;
          }
          p++;
        }
        myprintf("\n");
        decoded=1;
        return;
      }
    }
  }
  
}
