#Carpetas make -f
DOC = ./doc/
LIB = ./lib/
MAN = ./man/
OBJ = ./obj/
SRCLIB = ./srclib/
SRC = ./src/
CA = ./certs/
ECHO =./echo/
CLI_SER=./cliente_servidor/
#Creacion directorios
DIRS = obj lib certs echo cliente_servidor

GROUP = G-2311-03-P3
GROUP1 = G-2311-03-P1
#Compilador
CC = gcc -pthread
#Librerias
LDFLAGS = -lm
CCLIBS = -lircinterface -lircredes -lirctad -lssl
CCLAGS =  -rdynamic
FLAGS = `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0`
FLAGSE = `pkg-config --libs gtk+-3.0`
FLAGSC = `pkg-config --cflags gtk+-3.0`
#Ficheros
EXEC = $(CLI_SER)cliente_IRC $(CLI_SER)servidor_IRC $(ECHO)cliente_echo $(ECHO)servidor_echo
SOURCE_FILES =

all: dirs certificados libreria $(EXEC)

dirs:
	@mkdir -p $(DIRS)

libreria: $(LIB)lib.a

$(LIB)lib.a: $(OBJ)lib.o
	@ar -rv $@ $^ -lssl
	@echo "Libreria lib.a generada"

$(CLI_SER)servidor_IRC: $(OBJ)server.o $(OBJ)commands.o $(OBJ)canal.o $(OBJ)user.o $(OBJ)utilidadesTAD.o $(LIB)lib.a
	@$(CC) $(CCFLAGS) $^ -o  $@ $(CCLIBS)
	@echo "Ejecutable servidor_IRC creado"

$(OBJ)server.o: $(SRC)$(GROUP)server.c
	@$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)commands.o: $(SRC)$(GROUP)commands.c
	@$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)canal.o: $(SRC)$(GROUP)canal.c
	@$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)user.o: $(SRC)$(GROUP)user.c
	@$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)utilidadesTAD.o: $(SRC)$(GROUP)utilidadesTAD.c
	@$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)lib.o: $(SRCLIB)lib.c
	@$(CC) $(CCFLAGS) -o $@ -c $^

$(CLI_SER)cliente_IRC: $(OBJ)xchat2.o
		@$(CC) $^ -o $@ $(CCLIBS) $(FLAGSE) $(CCLAGS)

$(OBJ)xchat2.o: $(SRC)$(GROUP)xchat2.c
		@$(CC) $(CCLAGS) $(FLAGSC) -c -o $@ $^
		@echo "Ejecutable cliente_IRC creado"

$(ECHO)cliente_echo: $(SRC)clienteEcho.c $(LIB)lib.a
	@$(CC) $(CCLAGS) -o $@ $^ $(CCLIBS)
	@echo "Ejecutable cliente_Echo creado"

$(ECHO)servidor_echo: $(SRC)servidorEcho.c $(LIB)lib.a
	@$(CC) $(CCLAGS) -o $@ $^ $(CCLIBS)
	@echo "Ejecutable servidor_Echo creado"

clean:
		@rm -f $(CA)*.* *.o *.gch lib/lib.a obj/*.o includes/*.gch  $(GROUP).tar.gz $(EXEC)

dox:
		doxygen Doxyfile

certificados:
	@openssl genrsa -out $(CA)rootkey.pem 2048
	@openssl req -new -x509 -key $(CA)rootkey.pem -subj "/C=ES/ST=MADRID/L=ALCOBENDAS/O=EPS/CN=Redes2 CA/OU=ICR" -out $(CA)rootcert.pem
	@cat $(CA)rootcert.pem $(CA)rootkey.pem > $(CA)ca.pem
	@openssl genrsa -out $(CA)clientkey.pem 2048
	@openssl req -new -key $(CA)clientkey.pem -subj "/C=ES/ST=MADRID/L=ALCOBENDAS/O=EPS/CN=$(GROUP1)-client/OU=ICR" -out $(CA)clientx.pem
	@openssl x509 -req -in $(CA)/clientx.pem -out $(CA)/clientcert.pem -CA $(CA)/rootcert.pem -CAkey $(CA)/rootkey.pem -CAcreateserial
	@cat $(CA)clientcert.pem $(CA)clientkey.pem > $(CA)cliente.pem
	@openssl genrsa -out $(CA)serverkey.pem 2048
	@openssl req -new -key $(CA)serverkey.pem -subj "/C=ES/ST=MADRID/L=ALCOBENDAS/O=EPS/CN=$(GROUP1)-server/OU=ICR" -out $(CA)serverx.pem
	@openssl x509 -req -in $(CA)serverx.pem -out $(CA)servercert.pem -CA $(CA)rootcert.pem -CAkey $(CA)rootkey.pem -CAcreateserial
	@cat $(CA)servercert.pem $(CA)serverkey.pem > $(CA)servidor.pem

tar:
		@ echo "Comprimiendo el archivo"
		tar --exclude .git -czvf $(GROUP).tar.gz ../$(GROUP) --exclude $(GROUP).tar.gz
		@ echo "Archivo comprimido ..."
