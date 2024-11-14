/* Request Authorization structs */

struct request_authorization_arg {
    string userid<>;
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
    APPROVE_REQUEST_TOKEN_SUCCESS,
    APPROVE_REQUEST_TOKEN_ERROR
};

struct approve_request_token_ret {
    enum approve_request_token_status status;
};

/* Request Access Token structs */

struct request_access_token_arg {
    string userid<>;
    string auth_token<>;
    int automatically_refresh_token;
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

/* Refresh Access Token structs */

struct refresh_access_token_arg {
    string userid<>;
    string renew_token<>;
};

enum refresh_access_token_status {
    REFRESH_ACESS_TOKEN_SUCCESS,
    REFRESH_ACESS_TOKEN_ERROR
};

struct refresh_access_token_ret {
    enum refresh_access_token_status status;
    string access_token<>;
    string renew_token<>;
    unsigned int availability;
};

/* Validate Delegated Action structs */

struct validate_delegated_action_arg {
    string op_type<>;
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

        /* Refresh Access Token */
        refresh_access_token_ret refresh_access_token(refresh_access_token_arg) = 4;

        /* Validate Delegated Action */
        validate_delegated_action_ret validate_delegated_action(validate_delegated_action_arg) = 5;
    } = 1;
} = 1;