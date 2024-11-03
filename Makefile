server : ./socket/socket.c ./utils/utils.h ./server/server.c ./http/http.c
	gcc -o bin/server -Wall ./socket/socket.c ./server/server.c ./utils/utils.h ./http/http.c

clean : 
	rm -rf ./bin/*