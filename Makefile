server : ./socket/socket.c ./utils/utils.h ./server/server.c ./http/http.c ./uri/uri.c
	gcc -g -o bin/server -Wall ./utils/utils.c  ./socket/socket.c  ./http/http.c ./server/server.c ./uri/uri.c

clean : 
	rm -rf ./bin/*