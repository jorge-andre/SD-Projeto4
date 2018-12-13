#include <stdio.h>

#include "message.h"
#include "base64.h"
#include "inet.h"
#include "message-private.h"
#include "data.h"

int message_to_buffer(struct message_t *msg, char **msg_buf){



if (msg==NULL || msg_buf == NULL ) {
    return -1;
}



int offset = 0;
int size = OPCODE_SIZE+C_TYPE_SIZE;
*msg_buf = malloc(size);
short a;
int b;

a = htons(msg->opcode);
memcpy(*msg_buf,&a,OPCODE_SIZE);
offset += OPCODE_SIZE;

a = htons(msg->c_type);
memcpy(*msg_buf+offset,&a,C_TYPE_SIZE);
offset += C_TYPE_SIZE;

if(msg -> c_type == CT_KEY){
                

                *msg_buf = realloc(*msg_buf,size+2);
                size += 2;

                a = htons((short)strlen(msg->content.key));
                memcpy(*msg_buf+offset,&a,2);
                offset += 2;

                *msg_buf = realloc(*msg_buf,size+strlen(msg->content.key));
                size += strlen(msg->content.key);

                memcpy(*msg_buf+offset,msg->content.key,strlen(msg->content.key));
                offset += strlen(msg->content.key);

}else if (msg -> c_type == CT_VALUE) {
                
                char *value64;
                int b_net;
                
                b = base64_encode_alloc((char *)msg->content.value->data, msg->content.value->datasize, &value64);   
                
                *msg_buf = realloc(*msg_buf,size+4+b);
                b_net = htonl(b);
                memcpy(*msg_buf+offset,&b_net,4);
                offset += 4;
                memcpy(*msg_buf+offset,value64,b);

                size = size + 4 + b;

}else if (msg -> c_type == CT_ENTRY) {

                char *value64_e;
                short ks_net;
                int ds_net;
                int ds;

                *msg_buf = realloc(*msg_buf,size+2);
                ks_net = htons(strlen(msg->content.entry ->key));
                memcpy(*msg_buf+offset,&ks_net,2);
                offset+=2;

                *msg_buf = realloc(*msg_buf,size+2+strlen(msg->content.entry->key));
                memcpy(*msg_buf+offset,msg->content.entry->key,strlen(msg->content.entry->key));
                offset += strlen(msg->content.entry->key);

                ds = base64_encode_alloc ((char*)msg->content.entry->value->data,msg->content.entry->value->datasize , &value64_e);
                ds_net = htonl(ds);
                
                *msg_buf = realloc(*msg_buf,size+2+strlen(msg->content.entry->key)+4);                
                memcpy(*msg_buf+offset,&ds_net,4);
                offset+=4;

                *msg_buf = realloc(*msg_buf,size+2+strlen(msg->content.entry->key)+4+ds);
                memcpy(*msg_buf+offset,value64_e,ds);

                size += 2 + strlen(msg->content.entry->key) + 4 + ds;



}else if (msg -> c_type == CT_KEYS) {
   
                int num_k = 0;
                int num_k_net;
                int i = 0;
                short ks,ks_net;

                while(msg->content.keys[num_k]!=NULL){
                    num_k++;
                }
                *msg_buf = realloc(*msg_buf,size+4);
                num_k_net = htonl(num_k);
                memcpy(*msg_buf+offset,&num_k_net,4);
                offset+=4;
                size+=4;

                while(msg->content.keys[i]!=NULL){
                    
                    *msg_buf = realloc(*msg_buf,size+2);
                    ks = strlen(msg->content.keys[i]);
                    ks_net = htons(ks);
                    memcpy(*msg_buf+offset,&ks_net,2);
                    offset +=2;
                    size+=2;

                    *msg_buf = realloc(*msg_buf,size+ks);
                    memcpy(*msg_buf+offset,msg->content.keys[i],ks);
                    offset+=ks;
                    size+=ks;

                    i++;

                }
                
}else if (msg -> c_type == CT_RESULT) {
    
    *msg_buf = realloc(*msg_buf,size+4);
    size += 4;
    b=htonl(msg->content.result);
    memcpy(*msg_buf+offset,&b,4);

}

return size;

}


















struct message_t *buffer_to_message(char *msg_buf, int msg_size){




 struct message_t *msg = malloc(sizeof(struct message_t));
 int offset = 0;
 short a;
 int b;
 


    memcpy(&a,msg_buf,2);
    msg -> opcode = ntohs(a);
    offset +=2;

    memcpy(&a,msg_buf + offset,2);
    msg -> c_type = ntohs(a);
    offset +=2;

    
    
    

 if(msg -> c_type == CT_KEY){
    
                memcpy(&a,msg_buf + offset,2);
                offset +=2;
                msg -> content.key = malloc(ntohs(a));
            
                memcpy(msg -> content.key,msg_buf + offset,ntohs(a));    

}else if (msg -> c_type == CT_VALUE) {
    
                int size_h;
                
                memcpy(&b,msg_buf+offset,4);
                offset += 4;
                size_h = ntohl(b);
                msg ->content.value = data_create(size_h);
                free(msg ->content.value->data);
                base64_decode_alloc(msg_buf+offset, msg -> content.value -> datasize, (char **)&(msg -> content.value -> data),NULL);


}else if (msg -> c_type == CT_ENTRY) {
                
                short ks;
                int datas;
                char *value64_d;
                char *k;
                

                memcpy(&ks,msg_buf+offset,2);
                ks = ntohs(ks);
                offset += 2;

                k = malloc(ks);
                memcpy(k,msg_buf+offset,ks);
                offset+=ks;

                memcpy(&datas,msg_buf+offset,4);
                datas = ntohl(datas);
                offset+=4;

                base64_decode_alloc(msg_buf+offset,datas,&value64_d,NULL);

                msg->content.entry = entry_create(k,data_create2(datas,value64_d));
                
                


}else if (msg -> c_type == CT_KEYS) {

                int n_ks;
                int i = 0;
                short k_s;

                memcpy(&n_ks,msg_buf+offset,4);
                offset+=4;
                n_ks = ntohl(n_ks);

                msg->content.keys = malloc((n_ks+1)*sizeof(char*));

                
                for(i = 0; i < n_ks; i++)
                {
                    memcpy(&k_s,msg_buf+offset,2);
                    offset += 2;
                    k_s = ntohs(k_s);

                    msg->content.keys[i] = malloc(k_s+1);

                    memcpy(msg->content.keys[i],msg_buf+offset,k_s);
                    msg->content.keys[i][k_s] = '\0';
                    offset += k_s;
                }
                
                msg->content.keys[n_ks] = NULL;

}else if (msg -> c_type == CT_RESULT) {
    
                memcpy(&b,msg_buf + offset,4);
                msg -> content.result = ntohl(b);

}else if(msg -> c_type != CT_NONE){
                free_message(msg);
                return NULL;
}




  
return msg;
    
}

void free_message(struct message_t *msg){

    
    if (msg -> c_type == CT_KEY) {
        free(msg->content.key);
    }else if(msg -> c_type == CT_VALUE) {
        data_destroy(msg->content.value);
    }else if (msg -> c_type == CT_ENTRY){
        entry_destroy(msg->content.entry);
    }else if(msg -> c_type == CT_KEYS){
        int i = 0;
        while(msg->content.keys[i] != NULL){
            free(msg->content.keys[i]);
            i++;
        }
        free(msg->content.keys);
    }
    free(msg);

}
