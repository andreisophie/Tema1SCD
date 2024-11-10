/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "tema1.h"
#include <stdio.h> 
#include <time.h> 
#include <rpc/rpc.h>
#include <string.h>

#define REQUEST "REQUEST"
#define INSERT	"INSERT"
#define MODIFY	"MODIFY"
#define READ	"READ"
#define EXECUTE	"EXECUTE"
#define DELETE	"DELETE"

CLIENT *clnt;
FILE *in_file;

void initialize_client(char *host, char *op_file) {
#ifndef	DEBUG
	clnt = clnt_create(host, TEMA1_PROG, TEMA1_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror("Eroare la crearea clientului");
		exit(1);
	}
#endif	/* DEBUG */

	in_file = fopen(op_file, "r");
	if (in_file == NULL) {
		printf("Eroare la deschiderea fisierului client.in\n");
		exit(1);
	}
}

void shutdown_client() {
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */

	fclose(in_file);
}

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

void run_client() {
	char *line = NULL;
	size_t len = 0;
	char *userID;
	char *action;
	char *resource;
	while (getline(&line, &len, in_file) != -1) {
		userID = strtok(line, ",");
		action = strtok(NULL, ",");
		resource = strtok(NULL, "\n");

		if (!strcmp(action, REQUEST)) {
			request_authorization_arg arg;
			arg.userID = userID;
			request_authorization_ret *result = request_authorization_1(&arg, clnt);
			if (result == NULL) {
				clnt_perror(clnt, "Eroare la apelul RPC request_authorization_1");
			}
			if (result->status == REQUEST_AUTHORIZATION_SUCCESS) {
				printf("%s\n", result->auth_token);
			} else {
				printf("USER_NOT_FOUND\n");
			}
		}
	}
	
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

	initialize_client(host, op_file);

	run_client();

	shutdown_client();

	return 0;
}
