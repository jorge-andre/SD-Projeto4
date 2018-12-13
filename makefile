# Grupo 19:
# Andre Nicolau - 47880
# Jorge Andre - 49496
# Pedro Luis - 44826

OBJ_dir = object
OBJETOS_data = data.o test_data.o
OBJETOS_entry = data.o entry.o test_entry.o
OBJETOS_list = data.o entry.o list.o test_list.o
OBJETOS_table =data.o entry.o list.o table.o test_table.o
OBJETOS_t_cliente = data.o entry.o client_stub.o network_client.o message.o table-client.o read_write.o base64.o
OBJETOS_t_server = table-server.o data.o entry.o network_server.o read_write.o table_skel.o table.o list.o base64.o message.o
OBJETOS_message = message.o base64.o data.o entry.o test_message.o

data.o = data.h
entry.o = data.h ent-y.h
list.o = data.h entry.h list.h list-private.h
table.o = data.h entry.h list.h table.h table-private.h

#Objetos teste
test_data.o = data.h
test_entry.o = data.h entry.h
test_list.o = data.h entry.h list.h
test_table.o = data.h entry.h table.h
test_message.o = message.h base64.h inet.h

#Objetos cliente/servidor
message.o = message.h base64.h inet.h
client_stub.o = data.h entry.h client_stub.h client_stub-private.h message.h
network_client.o = network_client.h client_stub.h client_stub-private.h inet.h read_write.h message.h
table-client.o = data.h entry.h client_stub.h network_client.h table-client.h
read_write.o = read_write.h
base64.o = base64.h


network_server.o = network_server.h message.h table_skel.h read_write.h inet.h
read_write.o = inet.h read_write.h
table_skel.o = message.h table_skel.h table.h
table-server.o = network_server.h table_skel.h message.h read_write.h inter.h


CC = gcc
CFLAGS = -g -Wall -I ./include

all: table_client table_server client-lib.o

table_server: $(OBJETOS_t_server)
	$(CC) $(addprefix $(OBJ_dir)/, $(OBJETOS_t_server)) -o binary/server

table_client: $(OBJETOS_t_cliente)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJETOS_t_cliente)) -o binary/client

client-lib.o: $(addprefix object/, message.o client_stub.o network_client.o data.o entry.o)
	ld -r $(addprefix $(OBJ_dir)/, message.o client_stub.o network_client.o data.o entry.o) -o object/client-lib.o

test_data: $(OBJETOS_data)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJETOS_data)) -o binary/test_data

test_entry: $(OBJETOS_entry)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJETOS_entry)) -o binary/test_entry

test_list: $(OBJETOS_list)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJETOS_list)) -o binary/test_list

test_table: $(OBJETOS_table)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJETOS_table)) -o binary/test_table

test_message: $(OBJETOS_message)
	$(CC) $(addprefix $(OBJ_dir)/,$(OBJETOS_message)) -o binary/test_message -g

%.o: source/%.c $($@)
	$(CC) $(CFLAGS) -o $(OBJ_dir)/$@ -c $<
