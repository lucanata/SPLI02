#include "my.h"

void liv4(u_int type,u_int len,const u_char *p){
  int ihl,flag,i;
  u_int dsap,ssap;
  u_long seq_num,ack_num;
  struct filt_tcp *aux_tcp;
  struct filt_udp *aux_udp;
  u_int urg;
  const u_char *mp;
  u_char ff; 
  
  switch(type){
  case 6:
    if(!r_tcp)return;  
    ssap=ntohs(*(u_int *)p);
    dsap=ntohs(*(u_int *)(p+2));
    flag=0;
    for(aux_tcp=filt_tcp;aux_tcp!=NULL;aux_tcp=aux_tcp->next){
      if(aux_tcp->ssap!=ssap&&aux_tcp->ssap!=0){
	flag=1;
	continue;
      }
      if(aux_tcp->dsap!=dsap&&aux_tcp->dsap!=0){
	flag=1;
	continue;
      }
      flag=0;
      break;
    }
    seq_num=ntohl(*(u_long *)(p+4));
    ack_num=ntohl(*(u_long *)(p+8));
    urg=ntohl(*(u_int *)(p+18));
    ihl=((*(p+12))&0xf0)/4;         
    ff=*(p+13);
    if(p_tcp){
      colore(4);
      myprintf("TCP      |");
      myprintf("%d -> %d\n",ssap,dsap);
      myprintf("         |Seq:%lu\n",seq_num);
      if(ff&0x20)myprintf("         |URG:%d\n",urg);
      if(ff&0x10)myprintf("         |ACK:%lu\n",ack_num);
      if(ff&0x08)myprintf("         |PSH\n");
      if(ff&0x04)myprintf("         |RST\n");
      if(ff&0x02&&ack_num==0)myprintf("         |REQ\n");
      if(ff&0x02&&ack_num==1)myprintf("         |ACP\n");
      if(ff&0x01)myprintf("         |FIN\n");
    }
    if(flag){
      filt_kill=1;
      return;
    }
    liv7(len-ihl,p+ihl,ssap,dsap);      //passo anche le porte per capire che traffico è
    return;
    
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
      myprintf("UDP      |");
      myprintf("%d -> %d\n",ssap,dsap);
    }
    if(flag){
      filt_kill=1;
      return;
    }
    liv7(len-8,p+8,ssap,dsap);
    return;

  case 2:
    if(!p_igmp)return;  
    colore(4);
    myprintf("IGMP     |");
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

  case 1:
    if(!p_icmp)return;
    u_int icmp_type=(*p);
    u_int icmp_code=(*(p+1)); 
    colore(4);
    myprintf("ICMP     |");
    myprintf("Type: ");
    switch((*p)){
    case 0:
      myprintf("Echo Reply\n");
      break;
    case 3:
      myprintf("Destination Unreacheable  \n");
      myprintf("         |Code: ");
      switch(icmp_code){
      case 0:
        myprintf("Net Unreacheable  ");
        break;
      case 1:
        myprintf("Host Unreacheable  ");
        break;
      case 2:
        myprintf("Protocol Unreacheable  ");
        break;
      case 3:
        myprintf("Port Unreacheable  ");
        break;
      case 4:
        myprintf("Fragment required  ");
        break;
      case 5:
        myprintf("Source route failed  ");
        break;
      case 6:
        myprintf("Destination network unknown  ");
        break;
      case 7:
        myprintf("Destination host unknown  ");
        break;
      case 8:
        myprintf("Source host isolated  ");
        break;
      case 9:
        myprintf("Network administratively prohibited  ");
        break;
      case 10:
        myprintf("Host administratively prohibited  ");
        break;
      case 11:
        myprintf("Network unreacheable fot TOS  ");
        break;
      case 12:
        myprintf("Host unreacheable fot TOS  ");
        break;
      case 13:
        myprintf("Communications administratively prohibited  ");
        break;
      }
      break;
    case 4:
      myprintf("Source quench  ");
      break;
    case 5:
      myprintf("Redirect  \n");
      myprintf("         |Code: ");
      switch(icmp_code){
      case 0:
        myprintf("Redirect datagram for the network  ");
        break;
      case 1:
        myprintf("Redirect datagram for the host  ");
        break;
      case 2:
        myprintf("Redirect datagram for the TOS & network  ");
        break;
      case 3:
        myprintf("Redirect datagram for the TOS & host  ");
        break;      
      }
      break;
    case 6:
      myprintf("Alternative Host Address  ");
      break;
    case 8:
      myprintf("Echo Request  ");
      break;
    case 9:
      myprintf("Router advertisement  ");
      break;
    case 10:
      myprintf("Router selection  ");
      break;
    case 11:
      myprintf("Time Exceeded  ");
      myprintf("         |Code: ");
      switch(icmp_code){
      case 0:
        myprintf("TTL exceeded  ");
        break;
      case 1:
        myprintf("Fragment reassembly time exceeded  ");
        break;
      }
      break;
    case 12:
      myprintf("Parameter problem  \n");
      myprintf("         |Code: ");
      switch(icmp_code){
      case 0:
        myprintf("Pointer problem  ");
        break;
      case 1:
        myprintf("Missing a required operand  ");
        break;
      case 2:
        myprintf("Bad length  ");
        break;
      }
    case 13:
      myprintf("Timestamp");
      break;
    case 14:
      myprintf("Timestamp reply  ");
      break;
    case 15:
      myprintf("Information request  ");
      break;
    case 16:
      myprintf("Information reply  ");
      break;
    case 17:
      myprintf("Address mask request  ");
      break;
    case 18:
      myprintf("Address mask reply  ");
      break;
    case 30:
      myprintf("Traceroute  ");
      break;
    default:
      unknown=1;
      return;
    }
    decoded=1;
    return;
  default:
    unknown=1;
    return;
  }
}
