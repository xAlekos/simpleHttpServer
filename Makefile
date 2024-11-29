server : ./socket/socket.c ./utils/utils.h ./utils/utils.c ./server/server.c ./http/http.c ./uri/uri.c ./mime_type/mime_type.c ./threadpool/threadpool.c
	gcc -g -o bin/server -Wall ./utils/utils.c  ./socket/socket.c  ./http/http.c ./server/server.c ./uri/uri.c ./mime_type/mime_type.c ./threadpool/threadpool.c

clean : 
	rm -rf ./bin/*