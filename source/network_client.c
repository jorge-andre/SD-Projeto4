#include <stdio.h>

#include "client_stub.h"
#include "client_stub-private.h"
#include "inet.h"
#include "read_write.h"

#include "network_client.h"

int sockfd;
struct sockaddr_in server;
int nbytes,count;

int network_connect(struct rtable_t *rtable){

  if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    printf("Falha a criar o socket\n");
    return -1;
  }
  
  server.sin_family = AF_INET;
  server.sin_port = htons(rtable -> port);
  if (inet_pton(AF_INET,rtable->ip,&server.sin_addr) < 1) {
    printf("socket fechado\n");
    close(sockfd);
    return -1;
  }
  
  if (connect(sockfd,(struct sockaddr *) &server, sizeof(server)) < 0) {
    printf("socket fechado\n");
    close(sockfd);
    return -1;
  }

  rtable -> sockfd = sockfd;

  return 0;

}

struct message_t *network_send_receive(struct rtable_t * rtable,struct message_t *msg){

  char *buffer;
  int size_b;

  
  
  if ((size_b = message_to_buffer(msg,&buffer)) < 0) {
    printf("socket fechado1\n");
    close(rtable->sockfd);
    return NULL;
  }
    
  if ((nbytes = write_all(rtable->sockfd,buffer,size_b)) < size_b) {
    printf("socket fechado1\n");
    close(rtable->sockfd);
    return NULL;
  }

  
  int size = OPCODE_SIZE + C_TYPE_SIZE;
  char *buf = malloc(size);

  int nbytes;
  short c_type_net;
  short c_type;

  if ((nbytes=read_all(rtable -> sockfd,buf,OPCODE_SIZE))<OPCODE_SIZE) {
    free(buf);
    close(rtable->sockfd);
    return NULL;
  }

    

  if ((nbytes=read_all(rtable -> sockfd,(buf+OPCODE_SIZE),C_TYPE_SIZE))<C_TYPE_SIZE) {
    free(buf);
    close(rtable->sockfd);
    return NULL;
  }

  memcpy(&c_type_net,buf+OPCODE_SIZE,C_TYPE_SIZE);
  c_type = ntohs(c_type_net);

  switch (c_type) {
    /*case CT_KEY :{
    
      short s_k,s_k_net;

      if ((nbytes=read_all(rtable -> sockfd,&s_k_net,2))<2) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }
    
      s_k = ntohs(s_k_net);

      buf = realloc(buf,size + 2 );
      memcpy(buf+size,&s_k_net,2);
      size +=2;

      buf = realloc(buf,size + 2 + s_k );

      if ((nbytes=read_all(rtable -> sockfd,buf + size,s_k))<s_k) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }

      size += s_k;


      break;
    }*/case CT_VALUE:{

      

      int s_d,s_d_net;

      if ((nbytes=read_all(rtable -> sockfd,&s_d_net,4))<4) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }

      s_d = ntohl(s_d_net);
      
      buf = realloc(buf,size+4);
      memcpy(buf+size,&s_d_net,4);
      size += 4;

      buf = realloc(buf,size+s_d);

      if ((nbytes=read_all(rtable -> sockfd,buf+size,s_d))<s_d) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }

      size += s_d;


      break;
    }/*case CT_ENTRY:{

        int s_d,s_d_net;
        short s_k,s_k_net;

      if ((nbytes=read_all(rtable -> sockfd,&s_k_net,2))<2) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }
    
      s_k = ntohs(s_k_net);

      buf = realloc(buf,size + 2 );
      memcpy(buf+size,&s_k_net,2);
      size +=2;

      buf = realloc(buf,size + 2 + s_k );

      if ((nbytes=read_all(rtable -> sockfd,buf + size,s_k))<s_k) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }

      size += s_k;

      if ((nbytes=read_all(rtable -> sockfd,&s_d_net,4))<4) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }

      s_d = ntohl(s_d_net);
      
      buf = realloc(buf,size+4);
      memcpy(buf+size,&s_d_net,4);
      size += 4;

      buf = realloc(buf,size+s_d);

      if ((nbytes=read_all(rtable -> sockfd,buf+size,s_d))<s_d) {
        close(rtable -> sockfd);
        free(buf);
        return NULL;
      }

      size += s_d;
      

      break;
    }*/case CT_KEYS:{

      
      int i = 0,num_k_net,num_k;
      short k_s,k_s_net;

      buf = realloc(buf,size+4);

      if ((nbytes=read_all(rtable -> sockfd,&num_k_net,4))<4) {
        close(rtable->sockfd);
        free(buf);
        return NULL;
      }

      num_k = ntohl(num_k_net);

      memcpy(buf+size,&num_k_net,4);
      size+=4;


      while(i<num_k){
       
        if ((nbytes=read_all(rtable -> sockfd,&k_s_net,2))<2) {
          close(rtable->sockfd);
          free(buf);
          return NULL;
        }
        
        buf = realloc(buf,size+2);
        k_s = ntohs(k_s_net);
        memcpy(buf+size,&k_s_net,2);
        size+=2;

        buf = realloc(buf,size+k_s);
        
        if ((nbytes=read_all(rtable -> sockfd,buf+size,k_s))<k_s) {
          close(rtable->sockfd);
          free(buf);
          return NULL;
        }
        size+=k_s;
        i++;
      }

      break;
    }case CT_RESULT:{

      
      buf = realloc(buf,size+4);

      if ((nbytes=read_all(rtable -> sockfd,buf+size,4))<4) {
        close(rtable->sockfd);
        free(buf);
        return NULL;
      }

      size+=4;

      
      break;
    }
  }


  return buffer_to_message(buf,size);

}

int network_close(struct rtable_t * rtable){

  close(rtable -> sockfd); 
  return 0;

}
