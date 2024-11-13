/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "tema1.h"
#include <stdio.h> 
#include <time.h> 
#include <rpc/rpc.h>
#include "token.h"

typedef struct {
	char *userID;
	char *auth_token;
	char *access_token;
	int availability;
	char *renew_token;
	char **resource_permissions;
} user_info_t;

#define USERID_SIZE 16
int users_count = 0;
user_info_t *user_infos;
#define STING_SIZE 64
int resource_count = 0;
char **resources;
int token_validity;
FILE *approval_file_ptr;

void initialize_server(char *userid_file, char *resource_file, char *approval_file, int arg_token_validity) {
	// Read client IDs
	FILE *userid_file_ptr = fopen(userid_file, "r");
	if (userid_file_ptr == NULL) {
		printf("Eroare la deschiderea fisierului %s\n", userid_file);
		exit(1);
	}
	fscanf(userid_file_ptr, "%d", &users_count);
	user_infos = (user_info_t *)malloc(users_count * sizeof(user_info_t));
	for (int i = 0; i < users_count; i++) {
		user_infos[i].userID = (char *)malloc(USERID_SIZE * sizeof(char));
		fscanf(userid_file_ptr, "%s", user_infos[i].userID);
	}
	fclose(userid_file_ptr);

	// Read resources
	FILE *resource_file_ptr = fopen(resource_file, "r");
	if (resource_file_ptr == NULL) {
		printf("Eroare la deschiderea fisierului %s\n", resource_file);
		exit(1);
	}

	fscanf(resource_file_ptr, "%d", &resource_count);
	resources = (char **)malloc(resource_count * sizeof(char *));
	for (int i = 0; i < resource_count; i++) {
		resources[i] = (char *)malloc(STING_SIZE * sizeof(char));
		fscanf(resource_file_ptr, "%s", resources[i]);
	}
	fclose(resource_file_ptr);

	// Open approvals file
	approval_file_ptr = fopen(approval_file, "r");
	if (approval_file_ptr == NULL) {
		printf("Eroare la deschiderea fisierului %s\n", approval_file);
		exit(1);
	}

	// Set token validity
	token_validity = arg_token_validity;
	printf("Server initializat cu succes\n\n");
}

request_authorization_ret *
request_authorization_1_svc(request_authorization_arg *argp, struct svc_req *rqstp)
{
	static request_authorization_ret  result;

	char *userID = argp->userID;

	// Print message
	printf("BEGIN %s AUTHZ\n", userID);

	// Check if user exists
	for (int i = 0; i < users_count; i++) {
		if (strcmp(user_infos[i].userID, userID) == 0) {
			result.status = REQUEST_AUTHORIZATION_SUCCESS;
			char *token = generate_access_token(userID);
			user_infos[i].auth_token = token;
			result.auth_token = token;
			printf("\tRequestToken = %s\n", token);
			return &result;
		}
	}
	// User not found
	result.status = REQUEST_AUTHORIZATION_USER_NOT_FOUND;
	return &result;
}

// Trim training newline character in-place
void trim_string(char *str) {
	int len = strlen(str);

	// Remove trailing newline
	if (str[len - 1] == '\n') {
		str[len - 1] = '\0';
	}
}

approve_request_token_ret *
approve_request_token_1_svc(approve_request_token_arg *argp, struct svc_req *rqstp)
{
	static approve_request_token_ret  result;

	char *auth_token = argp->auth_token;

	// search for token in db
	for (int i = 0; i < users_count; i++) {
		// Some users might not have a token assigned
		if (user_infos[i].auth_token == NULL) {
			continue;
		}
		if (!strcmp(user_infos[i].auth_token, auth_token)) {
			// Read each permission from file
			char *line = NULL;
			size_t len = 0;
			getline(&line, &len, approval_file_ptr);
			// If line is "*,-" then user has no permissions
			if (strstr(line, "*,-")) {
				result.status = APPROVE_REQUEST_TOKEN_SUCCESS;
				return &result;
			}
			// Allocate memory for permissions array
			user_infos[i].resource_permissions = (char **)calloc(resource_count, sizeof(char *));
			char *resource = strtok(line, ",");
			char *permissions = strtok(NULL, ",");
			// trim newline
			trim_string(permissions);
			do {
				// Check if resource exists
				for (int j = 0; j < resource_count; j++) {
					if (!strcmp(resources[j], resource)) {
						// Add resource to user
						user_infos[i].resource_permissions[j]= permissions;
						break;
					}
				}
				resource = strtok(NULL, ",");
				if (resource == NULL) {
					break;
				}
				permissions = strtok(NULL, ",");
				trim_string(permissions);
			} while (resource != NULL && permissions != NULL);

			// Answer request
			result.status = APPROVE_REQUEST_TOKEN_SUCCESS;
			return &result;
		}
	}
	// Token not found
	result.status = APPROVE_REQUEST_TOKEN_ERROR;
	return &result;
}

request_access_token_ret *
request_access_token_1_svc(request_access_token_arg *argp, struct svc_req *rqstp)
{
	static request_access_token_ret  result;

	char *userID = argp->userID;
	char *auth_token = argp->auth_token;

	// Search user in db
	for (int i = 0; i < users_count; i++) {
		if (!strcmp(userID, user_infos[i].userID)) {
			// Check if token is valid
			if (user_infos[i].auth_token == NULL) {
				result.status = REQUEST_ACESS_TOKEN_REQUEST_DENIED;
				return &result;
			}
			if (!strcmp(auth_token, user_infos[i].auth_token)) {
				// Check if user has signed the token
				if (user_infos[i].resource_permissions == NULL) {
					result.status = REQUEST_ACESS_TOKEN_REQUEST_DENIED;
					return &result;
				}
				// Generate access token
				char *access_token = generate_access_token(auth_token);
				result.access_token = access_token;
				// TODO: These don't always need to be generated
				// result.renew_token = generate_access_token(access_token);
				// result.availability = token_validity;
				result.renew_token = (char *)malloc(TOKEN_LEN * sizeof(char));
				result.status = REQUEST_ACESS_TOKEN_SUCCESS;
				user_infos[i].access_token = access_token;
				printf("\tAccessToken = %s\n", access_token);
				return &result;
			} else {
				// Token not valid
				result.status = REQUEST_ACESS_TOKEN_REQUEST_DENIED;
				return &result;
			}
		}
	}

	return &result;
}

validate_delegated_action_ret *
validate_delegated_action_1_svc(validate_delegated_action_arg *argp, struct svc_req *rqstp)
{
	static validate_delegated_action_ret  result;

	char *resource = argp->resource;
	char *access_token = argp->access_token;
	char* op_type = argp->op_type;

	// Check if request has access token
	if (access_token == NULL) {
		printf("DENY (%s,%s,,0)\n", op_type, resource);
		result.status = VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED;
		return &result;
	}

	// Search for user
	for (int i = 0; i < users_count; i++) {
		if (user_infos[i].access_token == NULL) {
			continue;
		}
		if (!strcmp(user_infos[i].access_token, access_token)) {
			// Check if user has permission
			// Note: If user has an access token, then his auth token must have been already signed
			for (int j = 0; j < resource_count; j++) {
				if (!strcmp(resources[j], resource)) {
					if (user_infos[i].resource_permissions[j] == NULL) {
						printf("DENY (%s,%s,%s,%d)\n", op_type, resource, user_infos[i].access_token, user_infos[i].availability);
						result.status = VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED;
						return &result;
					}
					// Check if user has permission
					char op_type_chr;
					if (op_type[0] == 'E') {
						op_type_chr = 'X';
					} else {
						op_type_chr = op_type[0];
					}
					if (strchr(user_infos[i].resource_permissions[j], op_type_chr)) {
						printf("PERMIT (%s,%s,%s,%d)\n", op_type, resource, user_infos[i].access_token, user_infos[i].availability);
						result.status = VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED;
						return &result;
					} else {
						printf("DENY (%s,%s,%s,%d)\n", op_type, resource, user_infos[i].access_token, user_infos[i].availability);
						result.status = VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED;
						return &result;
					}
				}
			}
			result.status = VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND;
			return &result;
		}
	}

	printf("DENY (%s,%s,,0)\n", op_type, resource);
	result.status = VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED;
	return &result;
}
