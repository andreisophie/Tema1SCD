/* Request Authorization structs */

struct request_authorization_arg {
    string userID<>;
};

enum request_authorization_status {
    REQUEST_AUTHORIZATION_SUCCESS,
    REQUEST_AUTHORIZATION_USER_NOT_FOUND
};

struct request_authorization_ret {
    enum request_authorization_status status;
    string auth_token<>;
};

/* Approve Request Token structs */

struct approve_request_token_arg {
    string auth_token<>;
};

enum approve_request_token_status {
    /* FIXME: Maybe this needs changes? */
    APPROVE_REQUEST_TOKEN_SUCCESS,
    APPROVE_REQUEST_TOKEN_REQUEST_DENIED
};

struct approve_request_token_ret {
    /* FIXME: Maybe this needs to be changed? */
    enum approve_request_token_status status;
};

/* Request Access Token structs */

struct request_access_token_arg {
    string userID<>;
    string auth_token<>;
};

enum request_access_token_status {
    REQUEST_ACESS_TOKEN_SUCCESS,
    REQUEST_ACESS_TOKEN_REQUEST_DENIED
};

struct request_access_token_ret {
    enum request_access_token_status status;
    string access_token<>;
    string renew_token<>;
    unsigned int availability;
};

/* Validate Delegated Action structs */

enum operation_type {
    READ,
    INSERT,
    MODIFY,
    DELETE,
    EXECUTE
};

struct validate_delegated_action_arg {
    enum operation_type op_type;
    string resource<>;
    string access_token<>;
};

enum validate_delegated_action_status {
    VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED,
    VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED,
    VALIDATE_DELEGATED_ACTION_TOKEN_EXPIRED,
    VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND,
    VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED
};

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

        /* Validate Delegated Action */
        validate_delegated_action_ret validate_delegated_action(validate_delegated_action_arg) = 4;

        
    } = 1;
} = 1;