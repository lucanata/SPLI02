#include "my.h"

void liv4(u_int type,u_int len,const u_char *p){
  int ihl,flag,i;
  /* La porta destinatario dsap e la porta sorgente ssap saranno parametri
   * da passare al livello 7 per poter determinare il protocollo utilizzato
   * (noi visto che analizziamo http una delle 2 porte dovrà avere valore 80)
   */
  u_int dsap,ssap;
  u_long seq_num,ack_num;
  struct filt_tcp *aux_tcp;
  struct filt_udp *aux_udp;
  u_int urg;
  const u_char *mp;
  u_char ff;
  /* Nel parametro proto è contenuto il numero che identifica il
   * protocollo di liv. 4 vedi Assigned IP numbers
   * su ftp://ftp.ripe.net/rfc/rfc790.txt pag. 5
   * oppure su  http://en.wikipedia.org/wiki/List_of_IP_protocol_numbers
   */
  // TCP
  switch(type){
  case 6:
    if(!r_tcp)return;
    // source port 2 bytes
    ssap=ntohs(*(u_int *)p);
    // destination port 2 bytes
    dsap=ntohs(*(u_int *)(p+2));
    flag=0;
    for(aux_tcp=filt_tcp;aux_tcp!=NULL;aux_tcp=aux_tcp->next){
      if(aux_tcp->ssap!=ssap && aux_tcp->ssap!=0){
	flag=1;
	continue;
      }
      if(aux_tcp->dsap!=dsap && aux_tcp->dsap!=0){
	flag=1;
	continue;
      }
      flag=0;
      break;
    }
    seq_num=ntohl(*(u_long *)(p+4));
    ack_num=ntohl(*(u_long *)(p+8));
    urg=ntohl(*(u_int *)(p+18));
    ihl=((*(p+12))&0xf0)/4; /* perché /4 e non * 4? perché i bit sono nella
     parte più significativa del byte altrimenti dovrei fare
    ihl = (((*(p+12))&0xf0)/8 )*4;
    */
    ff=*(p+13);
    if(p_tcp){
      colore(4);
      myprintf("TCP  |");
      myprintf("%d -> %d",ssap,dsap);
      myprintf(" Seq:%lu",seq_num);
      if(ff&0x20)myprintf(" URG:%d",urg);
      if(ff&0x10)myprintf(" ACK:%lu",ack_num);
      if(ff&0x08)myprintf(" PSH");
      if(ff&0x04)myprintf(" RST");
      if(ff&0x02&&ack_num==0)myprintf(" REQ");
      if(ff&0x02&&ack_num==1)myprintf(" ACP");
      if(ff&0x01)myprintf(" FIN");
      myprintf("\n");
    }
    if(flag){
      filt_kill=1;
      return;
    }
    liv7(len-ihl,p+ihl, ssap, dsap);
    return;
  // UDP  
  case 17:
    if(!r_udp)return;  
    ssap=ntohs(*(u_int *)p);
    dsap=ntohs(*(u_int *)(p+2));
    flag=0;
    for(aux_udp=filt_udp;aux_udp!=NULL;aux_udp=aux_udp->next){
      if(aux_udp->ssap!=ssap&&aux_udp->ssap!=0){
	flag=1;
	continue;
      }
      if(aux_udp->dsap!=dsap&&aux_udp->dsap!=0){
	flag=1;
	continue;
      }
      flag=0;
      break;
    }
    if(p_udp){
      colore(4);
      myprintf("UDP  |");
      myprintf("%d -> %d",ssap,dsap);
      myprintf("\n");
    }
    if(flag){
      filt_kill=1;
      return;
    }
    liv7(len-8,p+8, ssap, dsap);
    return;
// IGMP
  case 2:
    if(!p_igmp)return;  
    colore(4);
    myprintf("IGMP |");
    switch((*p)){
    case 0x11:
      myprintf("Query ");
      print_ipv4(p+4);
      break;
    case 0x12:
      myprintf("Report ");
      print_ipv4(p+4);
      break;
    case 0x16:
      myprintf("Nreport ");
      print_ipv4(p+4);
      break;
    case 0x17: 
      myprintf("Leave ");
      print_ipv4(p+4);
      break;
    case 0x13:
      myprintf("DVMRP ** ");
      break;
    case 0x14:
      myprintf("PIM ** ");
      break;
    case 0x1e:
      myprintf("MRESP ** ");
      break;
    case 0x1f:
      myprintf("MTRACE ** ");
      break;
    default:
      unknown=1;
      return;
    }    
   
    myprintf("\n");
    decoded=1;
    return;
// ICMP
  case 1:
    if(!p_icmp)return;  
    colore(4);
    myprintf("ICMP |");
    switch((*p)&0x0f){
    case 0:
      myprintf("Echo Reply");
      break;
    case 8:
      myprintf("Echo Request");
      break;
    case 13:
      myprintf("Timestamp Request");
      break;
    case 14:
      myprintf("Timestamp Reply");
      break;

    default:
//      unknown=1;
      return;
    }   

    myprintf("\n");
    decoded=1;
    return;

  default:
    unknown=1;
    return;
  }
}
