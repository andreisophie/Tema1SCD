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
#define INSERT "INSERT"
#define MODIFY "MODIFY"
#define READ "READ"
#define EXECUTE "EXECUTE"
#define DELETE "DELETE"

CLIENT *clnt;
FILE *in_file;

void initialize_client(char *host, char *op_file)
{
#ifndef DEBUG
	clnt = clnt_create(host, TEMA1_PROG, TEMA1_VERS, "udp");
	if (clnt == NULL)
	{
		clnt_pcreateerror("Eroare la crearea clientului");
		exit(1);
	}
#endif /* DEBUG */

	in_file = fopen(op_file, "r");
	if (in_file == NULL)
	{
		printf("Eroare la deschiderea fisierului client.in\n");
		exit(1);
	}
}

void shutdown_client()
{
#ifndef DEBUG
	clnt_destroy(clnt);
#endif /* DEBUG */

	fclose(in_file);
}

typedef struct
{
	char *userID;
	char *auth_token;
	char *access_token;
	int automatically_refresh_token;
} user_tokens_t;

user_tokens_t *find_user_by_ID(user_tokens_t *user_tokens_array, int registered_users_count, char *userID)
{
	for (int i = 0; i < registered_users_count; i++)
	{
		if (!strcmp(user_tokens_array[i].userID, userID))
		{
			return &user_tokens_array[i];
		}
	}
	return NULL;
}

void call_request_access_token(user_tokens_t *current_user)
{
	request_access_token_arg arg;
	arg.userID = current_user->userID;
	arg.auth_token = current_user->auth_token;
	arg.automatically_refresh_token = current_user->automatically_refresh_token;
	request_access_token_ret *result = request_access_token_1(&arg, clnt);
	// treat errors
	if (result == NULL)
	{
		clnt_perror(clnt, "Eroare la apelul RPC request_access_token_1");
		return;
	}
	switch (result->status)
	{
	case REQUEST_ACESS_TOKEN_REQUEST_DENIED:
		printf("REQUEST_DENIED\n");
		return;
	case REQUEST_ACESS_TOKEN_SUCCESS:
		current_user->access_token = result->access_token;
		if (current_user->automatically_refresh_token)
		{
			printf("%s -> %s, %s\n", current_user->auth_token, current_user->access_token, result->renew_token);
			current_user->auth_token = result->renew_token;
		}
		else
		{
			printf("%s -> %s\n", current_user->auth_token, current_user->access_token);
		}
		break;
	default:
		printf("A avut loc o eroare necunoscuta: Request Access Token\n");
		return;
	}
}

void call_approve_request_token(user_tokens_t *current_user)
{
	// next step is to sign the token
	approve_request_token_arg arg;
	arg.auth_token = current_user->auth_token;
	approve_request_token_ret *result = approve_request_token_1(&arg, clnt);
	// treat errors
	if (result == NULL)
	{
		clnt_perror(clnt, "Eroare la apelul RPC approve_request_token_1");
		return;
	}
	if (result->status == APPROVE_REQUEST_TOKEN_SUCCESS)
	{
		call_request_access_token(current_user);
	}
	else
	{
		printf("A avut loc o eroare necunoscuta: Approve Request Token\n");
	}
}

void call_request_authorization(user_tokens_t *current_user)
{
	request_authorization_arg arg;
	arg.userID = current_user->userID;
	request_authorization_ret *result = request_authorization_1(&arg, clnt);
	// treat errors
	if (result == NULL)
	{
		clnt_perror(clnt, "Eroare la apelul RPC request_authorization_1");
		return;
	}
	switch (result->status)
	{
	case REQUEST_AUTHORIZATION_SUCCESS:
		current_user->auth_token = result->auth_token;
		call_approve_request_token(current_user);
		break;
	case REQUEST_AUTHORIZATION_USER_NOT_FOUND:
		printf("USER_NOT_FOUND\n");
		break;
	default:
		printf("A avut loc o eroare necunoscuta: Request Authorization\n");
		break;
	}
}

void run_request_action(user_tokens_t *current_user, int automatically_refresh_token)
{
	current_user->automatically_refresh_token = automatically_refresh_token;
	call_request_authorization(current_user);
}

void run_resource_action(user_tokens_t *current_user, char *action, char *resource)
{
	validate_delegated_action_arg arg;
	arg.op_type = action;
	arg.resource = resource;
	if (current_user->access_token == NULL)
	{
		arg.access_token = calloc(1, 1);
	}
	else
	{
		arg.access_token = current_user->access_token;
	}
	validate_delegated_action_ret *result = validate_delegated_action_1(&arg, clnt);
	// treat errors
	if (result == NULL)
	{
		clnt_perror(clnt, "Eroare la apelul RPC validate_delegated_action_1");
		return;
	}
	switch (result->status)
	{
	case VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED:
		printf("PERMISSION_GRANTED\n");
		break;
	case VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED:
		printf("PERMISSION_DENIED\n");
		break;
	case VALIDATE_DELEGATED_ACTION_TOKEN_EXPIRED:
		printf("TOKEN_EXPIRED\n");
		break;
	case VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND:
		printf("RESOURCE_NOT_FOUND\n");
		break;
	case VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED:
		printf("OPERATION_NOT_PERMITTED\n");
		break;
	default:
		printf("A avut loc o eroare necunoscuta: Validate Delegated Action\n");
		break;
	}
}

void run_action(user_tokens_t *current_user, char *userID, char *action, char *resource)
{
	if (!strcmp(action, REQUEST))
	{
		int automatically_refresh_token = atoi(resource);
		run_request_action(current_user, automatically_refresh_token);
	}
	else
	{
		// if action is not REQUEST, then it is one of RIMDX
		run_resource_action(current_user, action, resource);
	}
}

void run_client()
{
	char *line = NULL;
	size_t len = 0;
	char *userID;
	char *action;
	char *resource;
	int registered_users_count = 0;
	user_tokens_t *user_tokens_array = NULL;
	while (getline(&line, &len, in_file) != -1)
	{
		userID = strtok(line, ",");
		action = strtok(NULL, ",");
		resource = strtok(NULL, "\n");
		user_tokens_t *current_user = find_user_by_ID(user_tokens_array, registered_users_count, userID);
		if (current_user == NULL)
		{
			// register user in client-side database
			user_tokens_array = (user_tokens_t *)realloc(user_tokens_array, (registered_users_count + 1) * sizeof(user_tokens_t));
			registered_users_count++;
			current_user = &user_tokens_array[registered_users_count - 1];
			current_user->userID = userID;
		}
		run_action(current_user, userID, action, resource);
	}
}

int main(int argc, char *argv[])
{
	char *host;
	char *op_file;

	if (argc < 3)
	{
		printf("Utilizare: %s <adresa_server> <fisier_operatii> \n", argv[0]);
		exit(1);
	}
	host = argv[1];
	op_file = argv[2];

	initialize_client(host, op_file);

	run_client();

	shutdown_client();

	return 0;
}
