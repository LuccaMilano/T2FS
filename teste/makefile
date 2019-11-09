#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
# 

CC=gcc
LIB_DIR=../lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: testidentify testreadmbr testformat testcreate testopen testopenclose
	
testidentify: testidentify.c $(LIB_DIR)/libt2fs.a
	$(CC) -o testidentify testidentify.c -L$(LIB_DIR) -lt2fs -Wall

testreadmbr: testreadmbr.c $(LIB_DIR)/libt2fs.a
	$(CC) -o testreadmbr testreadmbr.c -L$(LIB_DIR) -lt2fs -Wall

testformat: testformat.c $(LIB_DIR)/libt2fs.a
	$(CC) -o testformat testformat.c -L$(LIB_DIR) -lt2fs -Wall

testcreate: testcreate.c $(LIB_DIR)/libt2fs.a
	$(CC) -o testcreate testcreate.c -L$(LIB_DIR) -lt2fs -Wall

testopen: testopen.c $(LIB_DIR)/libt2fs.a
	$(CC) -o testopen testopen.c -L$(LIB_DIR) -lt2fs -Wall

testopenclose: testopenclose.c $(LIB_DIR)/libt2fs.a
	$(CC) -o testopenclose testopenclose.c -L$(LIB_DIR) -lt2fs -Wall

clean:
	rm -rf testidentify testreadmbr testformat testcreate testopen testopenclose


