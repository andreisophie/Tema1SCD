/* Request Authorization structs */

struct request_authorization_arg {
    string userID<>;
};

enum request_authorization_status {
    SUCCESS,
    USER_NOT_FOUND
};

struct request_authorization_ret {
    enum request_authorization_status status;
    string auth_token<>;
};

/* Request Access Token structs */

struct request_access_token_arg {
    string userID<>;
    string auth_token<>;
};

enum request_access_token_status {
    SUCCESS,
    REQUEST_DENIED
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
    PERMISSION_GRANTED,
    PERMISSION_DENIED,
    TOKEN_EXPIRED,
    RESOURCE_NOT_FOUND,
    OPERATION_NOT_PERMITTED
};

struct validate_delegated_action_ret {
    enum validate_delegated_action_status status;
};

/* Approve Request Token structs */

struct approve_request_token_arg {
    string auth_token<>;
};

enum approve_request_token_status {
    /* FIXME: Maybe this needs changes? */
    SUCCESS,
    REQUEST_DENIED
};

struct approve_request_token_ret {
    /* FIXME: Maybe this needs to be changed? */
    enum approve_request_token_status status;
};

program TEMA1_PROG {
    version TEMA1_VERS {
        /* Request Authorization */
        request_authorization_ret request_authorization(request_authorization_arg) = 1;

        /* Request Access Token */
        request_access_token_ret request_access_token(request_access_token_arg) = 2;

        /* Validate Delegated Action */
        validate_delegated_action_ret validate_delegated_action(validate_delegated_action_arg) = 3;

        /* Approve Request Token */
        approve_request_token_ret approve_request_token(approve_request_token_arg) = 4;
    } = 1;
} = 1;