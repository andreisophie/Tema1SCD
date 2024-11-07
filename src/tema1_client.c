/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "tema1.h"
#include <stdio.h> 
#include <time.h> 
#include <rpc/rpc.h> 


void
tema1_prog_1(char *host)
{
	CLIENT *clnt;
	request_authorization_ret  *result_1;
	request_authorization_arg  request_authorization_1_arg;
	request_access_token_ret  *result_2;
	request_access_token_arg  request_access_token_1_arg;
	validate_delegated_action_ret  *result_3;
	validate_delegated_action_arg  validate_delegated_action_1_arg;
	approve_request_token_ret  *result_4;
	approve_request_token_arg  approve_request_token_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, TEMA1_PROG, TEMA1_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = request_authorization_1(&request_authorization_1_arg, clnt);
	if (result_1 == (request_authorization_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = request_access_token_1(&request_access_token_1_arg, clnt);
	if (result_2 == (request_access_token_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = validate_delegated_action_1(&validate_delegated_action_1_arg, clnt);
	if (result_3 == (validate_delegated_action_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = approve_request_token_1(&approve_request_token_1_arg, clnt);
	if (result_4 == (approve_request_token_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	char *op_file;

	if (argc < 3) {
		printf ("Utilizare: %s <adresa_server> <fisier_operatii> \n", argv[0]);
		exit (1);
	}
	host = argv[1];
	op_file = argv[2];
	tema1_prog_1 (host);

	return 0;
}
