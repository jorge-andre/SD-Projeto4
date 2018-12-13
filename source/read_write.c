#include <stdio.h>
#include <errno.h>
#include "inet.h"
#include "read_write.h"


int write_all(int sockfd,void *buf,int len){
    int bufsize = len;


    while (len > 0) {
      int res = write(sockfd,buf,len);
      if (res<0) {
        if (errno==EINTR) continue;
        return -1;
      }
      buf +=res;
      len -=res;
      
    }
    return bufsize;
}

int read_all(int sockfd,void *buf,int len){

  int bufsize = len;
  

  while (len > 0) {


    int res = read(sockfd,buf,len);
     
    if (res < 0) {
      if (errno==EINTR) continue;
      return -1;
    }

    buf +=res;
    len -=res;

    
  }

  
  return bufsize;

}
