
OBJ1 = client.o
OBJ2 = serveur.o
OPTIONS	= -DDEBUG
# Adaptation a Darwin / MacOS X avec fink
# Du fait de l'absence de libtermcap on se fait pas mal
# cracher dessus mais ca marche...
ifeq ($(shell uname), Darwin)
LFLAGS	+= -L/opt/local/lib
CFLAGS	+= -I /opt/local/include
endif
#Changer si necessaire le chemin d'acces aux librairies

# Adaptation a Linux
ifeq ($(shell uname), Linux)
OPTIONS	+= -ltermcap
endif

# Adaptation a FreeBSD
# Attention : il faut utiliser gmake...
ifeq ($(shell uname),FreeBSD)
OPTIONS	+= -ltermcap
endif

# Adaptation a Solaris

ifeq ($(shell uname),SunOS)
OPTIONS	+= -ltermcap  -lsocket -lnsl
CFLAGS	+= -I..
endif

EXEC = ${OBJ1} client ${OBJ2} serveur
all: ${EXEC}


client.o : client.c
	gcc  $(CFLAGS) -c  client.c

serveur.o :	serveur.c
	gcc  $(CFLAGS) -c  serveur.c

tchat-client : ${OBJ1}
	gcc $(LFLAGS) ${OBJ1} -o client -lcurses   $(OPTIONS)

tchat-server : ${OBJ2}
	gcc $(LFLAGS) ${OBJ2} -o serveur -lcurses   $(OPTIONS)



clean :
	rm -f ${EXEC} core
