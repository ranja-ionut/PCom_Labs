build: https_client

https_client: https_client.c
	gcc $? -o $@ -lssl -lcrypto

PHONY: clean
clean:
	rm -f https_client
		
