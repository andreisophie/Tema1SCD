/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "tema1.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
tema1_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		request_authorization_arg request_authorization_1_arg;
		request_access_token_arg request_access_token_1_arg;
		validate_delegated_action_arg validate_delegated_action_1_arg;
		approve_request_token_arg approve_request_token_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case request_authorization:
		_xdr_argument = (xdrproc_t) xdr_request_authorization_arg;
		_xdr_result = (xdrproc_t) xdr_request_authorization_ret;
		local = (char *(*)(char *, struct svc_req *)) request_authorization_1_svc;
		break;

	case request_access_token:
		_xdr_argument = (xdrproc_t) xdr_request_access_token_arg;
		_xdr_result = (xdrproc_t) xdr_request_access_token_ret;
		local = (char *(*)(char *, struct svc_req *)) request_access_token_1_svc;
		break;

	case validate_delegated_action:
		_xdr_argument = (xdrproc_t) xdr_validate_delegated_action_arg;
		_xdr_result = (xdrproc_t) xdr_validate_delegated_action_ret;
		local = (char *(*)(char *, struct svc_req *)) validate_delegated_action_1_svc;
		break;

	case approve_request_token:
		_xdr_argument = (xdrproc_t) xdr_approve_request_token_arg;
		_xdr_result = (xdrproc_t) xdr_approve_request_token_ret;
		local = (char *(*)(char *, struct svc_req *)) approve_request_token_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (TEMA1_PROG, TEMA1_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, TEMA1_PROG, TEMA1_VERS, tema1_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (TEMA1_PROG, TEMA1_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, TEMA1_PROG, TEMA1_VERS, tema1_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (TEMA1_PROG, TEMA1_VERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
