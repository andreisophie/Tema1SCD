/* Request Authorization structs */

/* The only data required as argument is the user's id */
struct request_authorization_arg {
    string userid<>;
};

/* Possible return statuses of the function, only possible error is user not found */
enum request_authorization_status {
    REQUEST_AUTHORIZATION_SUCCESS,
    REQUEST_AUTHORIZATION_USER_NOT_FOUND
};

/* Every procedure returns a status code and requested data */
struct request_authorization_ret {
    enum request_authorization_status status;
    string auth_token<>;
};

/* Approve Request Token structs */

/* The only data required as argument is the user's auth token */
struct approve_request_token_arg {
    string auth_token<>;
};

/* The protocol doesn't state an error when an invalid token is sent, */
/* so just generically separate possible result into success and error */
enum approve_request_token_status {
    APPROVE_REQUEST_TOKEN_SUCCESS,
    APPROVE_REQUEST_TOKEN_ERROR
};

/* This procedure call doesn't return anything besides a status, */
/* all the fun stuff happens on the server side */
struct approve_request_token_ret {
    enum approve_request_token_status status;
};

/* Request Access Token structs */

/* Requested data as stated by the protocol, plus the field automatically_refresh_token */
/* Because the server needs to know whether user wants a renew token too */
struct request_access_token_arg {
    string userid<>;
    string auth_token<>;
    int automatically_refresh_token;
};

/* Possible result states, as stated by the protocol */
enum request_access_token_status {
    REQUEST_ACESS_TOKEN_SUCCESS,
    REQUEST_ACESS_TOKEN_REQUEST_DENIED
};

/* Return values as stated by the protocol, plus the status */
struct request_access_token_ret {
    enum request_access_token_status status;
    string access_token<>;
    string renew_token<>;
    unsigned int availability;
};

/* Refresh Access Token structs */

/* This procedure will have the same arguments as Request Access Token, */
/* except I know for sure that user wants automatically_refresh_token, so no longer need to send it */
struct refresh_access_token_arg {
    string userid<>;
    string renew_token<>;
};

/* Generically separate possible results into success and error */
enum refresh_access_token_status {
    REFRESH_ACESS_TOKEN_SUCCESS,
    REFRESH_ACESS_TOKEN_ERROR
};

/* Same return data as Request Access Token */
struct refresh_access_token_ret {
    enum refresh_access_token_status status;
    string access_token<>;
    string renew_token<>;
    unsigned int availability;
};

/* Validate Delegated Action structs */

/* Requested data, as stated by the protocol */
struct validate_delegated_action_arg {
    string op_type<>;
    string resource<>;
    string access_token<>;
};

/* Possible statuses of the procedure, as stated by the protocol */
enum validate_delegated_action_status {
    VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED,
    VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED,
    VALIDATE_DELEGATED_ACTION_TOKEN_EXPIRED,
    VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND,
    VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED
};

/* The only data returned by the procedure is the status, the rest happens server-side */
struct validate_delegated_action_ret {
    enum validate_delegated_action_status status;
};

program TEMA1_PROG {
    version TEMA1_VERS {
        /* Request Authorization */
        request_authorization_ret request_authorization(request_authorization_arg) = 1;

        /* Approve Request Token */
        approve_request_token_ret approve_request_token(approve_request_token_arg) = 2;

        /* Request Access Token */
        request_access_token_ret request_access_token(request_access_token_arg) = 3;

        /* Refresh Access Token */
        refresh_access_token_ret refresh_access_token(refresh_access_token_arg) = 4;

        /* Validate Delegated Action */
        validate_delegated_action_ret validate_delegated_action(validate_delegated_action_arg) = 5;
    } = 1;
} = 1;