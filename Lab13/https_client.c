#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include "stdio.h"
#include "string.h"
#define LEN 8092

int main()
{
    BIO * bio;
    SSL * ssl;
    SSL_CTX * ctx;

    int p;

    char * request = "GET / HTTP/1.0\r\nHost: www.verisign.com\r\n\r\n";
    char r[1024];

    /* initializare librarie */

    SSL_library_init ();
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    /* TO DO initializare context */
    ctx = SSL_CTX_new(SSLv23_client_method());
   

    /* incarca trust store */


    if(! SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL))
    {
        fprintf(stderr, "Error loading trust store\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return 0;
    }


    /* stabileste conexiune */

    bio = BIO_new_ssl_connect(ctx);
   
    /* Seteaza flag SSL_MODE_AUTO_RETRY  */

    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    /* TO DO setup conexiune */
    BIO_set_conn_hostname(bio, "www.verisign.com:443");

    /* verifica conexiunea deschisa si efectueaza handshake */
    if(BIO_do_connect(bio) <= 0)
    {
        perror("Eroare BIO_do_connect.");
        exit(1);
    }

    /* TO DO verifica certificat */
    if(SSL_get_verify_result(ssl) != X509_V_OK)
    {
        perror("Eroare SSL_get_verify_result.");
        exit(1);
    }

    /* Trimite request */

    BIO_write(bio, request, strlen(request));

    /* TO DO Citeste raspuns si pregateste  output*/
    char buf[LEN];
    FILE *out = fopen("test.html", "w");
    int x;
    
    while(1) {
        memset(buf, 0, LEN);
        x = BIO_read(bio, buf, LEN);

        if(x == 0) {
            break;
        } else if(x < 0) {
            if(! BIO_should_retry(bio)) {
                perror("Eroare BIO_read.");
                exit(1);
            }
            continue;
        }
        
        fprintf(out, "%s", buf);
    }
    
    
    fclose(out);
    /* Inchide conexiune si elibereaza context */

    BIO_free_all(bio);
    SSL_CTX_free(ctx);
    return 0;
}
