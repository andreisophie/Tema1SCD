/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TEMA1_H_RPCGEN
#define _TEMA1_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct request_authorization_arg {
	char *userID;
};
typedef struct request_authorization_arg request_authorization_arg;

enum request_authorization_status {
	REQUEST_AUTHORIZATION_SUCCESS = 0,
	REQUEST_AUTHORIZATION_USER_NOT_FOUND = 1,
};
typedef enum request_authorization_status request_authorization_status;

struct request_authorization_ret {
	enum request_authorization_status status;
	char *auth_token;
};
typedef struct request_authorization_ret request_authorization_ret;

struct approve_request_token_arg {
	char *auth_token;
};
typedef struct approve_request_token_arg approve_request_token_arg;

enum approve_request_token_status {
	APPROVE_REQUEST_TOKEN_SUCCESS = 0,
	APPROVE_REQUEST_TOKEN_ERROR = 1,
};
typedef enum approve_request_token_status approve_request_token_status;

struct approve_request_token_ret {
	enum approve_request_token_status status;
};
typedef struct approve_request_token_ret approve_request_token_ret;

struct request_access_token_arg {
	char *userID;
	char *auth_token;
};
typedef struct request_access_token_arg request_access_token_arg;

enum request_access_token_status {
	REQUEST_ACESS_TOKEN_SUCCESS = 0,
	REQUEST_ACESS_TOKEN_REQUEST_DENIED = 1,
};
typedef enum request_access_token_status request_access_token_status;

struct request_access_token_ret {
	enum request_access_token_status status;
	char *access_token;
	char *renew_token;
	u_int availability;
};
typedef struct request_access_token_ret request_access_token_ret;

struct validate_delegated_action_arg {
	char *op_type;
	char *resource;
	char *access_token;
};
typedef struct validate_delegated_action_arg validate_delegated_action_arg;

enum validate_delegated_action_status {
	VALIDATE_DELEGATED_ACTION_PERMISSION_GRANTED = 0,
	VALIDATE_DELEGATED_ACTION_PERMISSION_DENIED = 1,
	VALIDATE_DELEGATED_ACTION_TOKEN_EXPIRED = 2,
	VALIDATE_DELEGATED_ACTION_RESOURCE_NOT_FOUND = 3,
	VALIDATE_DELEGATED_ACTION_OPERATION_NOT_PERMITTED = 4,
};
typedef enum validate_delegated_action_status validate_delegated_action_status;

struct validate_delegated_action_ret {
	enum validate_delegated_action_status status;
};
typedef struct validate_delegated_action_ret validate_delegated_action_ret;

#define TEMA1_PROG 1
#define TEMA1_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define request_authorization 1
extern  request_authorization_ret * request_authorization_1(request_authorization_arg *, CLIENT *);
extern  request_authorization_ret * request_authorization_1_svc(request_authorization_arg *, struct svc_req *);
#define approve_request_token 2
extern  approve_request_token_ret * approve_request_token_1(approve_request_token_arg *, CLIENT *);
extern  approve_request_token_ret * approve_request_token_1_svc(approve_request_token_arg *, struct svc_req *);
#define request_access_token 3
extern  request_access_token_ret * request_access_token_1(request_access_token_arg *, CLIENT *);
extern  request_access_token_ret * request_access_token_1_svc(request_access_token_arg *, struct svc_req *);
#define validate_delegated_action 4
extern  validate_delegated_action_ret * validate_delegated_action_1(validate_delegated_action_arg *, CLIENT *);
extern  validate_delegated_action_ret * validate_delegated_action_1_svc(validate_delegated_action_arg *, struct svc_req *);
extern int tema1_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define request_authorization 1
extern  request_authorization_ret * request_authorization_1();
extern  request_authorization_ret * request_authorization_1_svc();
#define approve_request_token 2
extern  approve_request_token_ret * approve_request_token_1();
extern  approve_request_token_ret * approve_request_token_1_svc();
#define request_access_token 3
extern  request_access_token_ret * request_access_token_1();
extern  request_access_token_ret * request_access_token_1_svc();
#define validate_delegated_action 4
extern  validate_delegated_action_ret * validate_delegated_action_1();
extern  validate_delegated_action_ret * validate_delegated_action_1_svc();
extern int tema1_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_request_authorization_arg (XDR *, request_authorization_arg*);
extern  bool_t xdr_request_authorization_status (XDR *, request_authorization_status*);
extern  bool_t xdr_request_authorization_ret (XDR *, request_authorization_ret*);
extern  bool_t xdr_approve_request_token_arg (XDR *, approve_request_token_arg*);
extern  bool_t xdr_approve_request_token_status (XDR *, approve_request_token_status*);
extern  bool_t xdr_approve_request_token_ret (XDR *, approve_request_token_ret*);
extern  bool_t xdr_request_access_token_arg (XDR *, request_access_token_arg*);
extern  bool_t xdr_request_access_token_status (XDR *, request_access_token_status*);
extern  bool_t xdr_request_access_token_ret (XDR *, request_access_token_ret*);
extern  bool_t xdr_validate_delegated_action_arg (XDR *, validate_delegated_action_arg*);
extern  bool_t xdr_validate_delegated_action_status (XDR *, validate_delegated_action_status*);
extern  bool_t xdr_validate_delegated_action_ret (XDR *, validate_delegated_action_ret*);

#else /* K&R C */
extern bool_t xdr_request_authorization_arg ();
extern bool_t xdr_request_authorization_status ();
extern bool_t xdr_request_authorization_ret ();
extern bool_t xdr_approve_request_token_arg ();
extern bool_t xdr_approve_request_token_status ();
extern bool_t xdr_approve_request_token_ret ();
extern bool_t xdr_request_access_token_arg ();
extern bool_t xdr_request_access_token_status ();
extern bool_t xdr_request_access_token_ret ();
extern bool_t xdr_validate_delegated_action_arg ();
extern bool_t xdr_validate_delegated_action_status ();
extern bool_t xdr_validate_delegated_action_ret ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TEMA1_H_RPCGEN */
