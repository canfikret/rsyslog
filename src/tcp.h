/* The mapping for relp over TCP.
 *
 * Copyright 2008-2013 by Rainer Gerhards and Adiscon GmbH.
 *
 * This file is part of librelp.
 *
 * Librelp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Librelp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with librelp.  If not, see <http://www.gnu.org/licenses/>.
 *
 * A copy of the GPL can be found in the file "COPYING" in this distribution.
 *
 * If the terms of the GPL are unsuitable for your needs, you may obtain
 * a commercial license from Adiscon. Contact sales@adiscon.com for further
 * details.
 *
 * ALL CONTRIBUTORS PLEASE NOTE that by sending contributions, you assign
 * your copyright to Adiscon GmbH, Germany. This is necessary to permit the
 * dual-licensing set forth here. Our apologies for this inconvenience, but
 * we sincerely believe that the dual-licensing model helps us provide great
 * free software while at the same time obtaining some funding for further
 * development.
 */
#ifndef RELPTCP_H_INCLUDED
#define	RELPTCP_H_INCLUDED

#include <gnutls/gnutls.h>
#include "relp.h"

typedef enum { relpTCP_RETRY_none = 0,
       relpTCP_RETRY_handshake = 1,
       relpTCP_RETRY_recv = 2, 
       relpTCP_RETRY_send = 3 } relpTcpRtryState_t;

/* the RELPTCP object 
 * rgerhards, 2008-03-16
 */
typedef struct relpTcp_s {
	BEGIN_RELP_OBJ;
	relpEngine_t *pEngine;
	unsigned char *pRemHostIP; /**< IP address of remote peer (currently used in server mode, only) */
	unsigned char *pRemHostName; /**< host name of remote peer (currently used in server mode, only) */
	int sock;	/**< the socket we use for regular, single-socket, operations */
	int *socks;	/**< the socket(s) we use for listeners, element 0 has nbr of socks */
	int iSessMax;	/**< maximum number of sessions permitted */
	/* variables for TLS support */
	int bEnableTLS;
	int bTLSActive;	/**< is TLS actually active (properly activated) on this session? */
	int bEnableTLSZip;
	int dhBits;	/**< number of bits for Diffie-Hellman key */
	gnutls_anon_client_credentials_t anoncred;	/**< client anon credentials */
	gnutls_anon_server_credentials_t anoncredSrv;	/**< server anon credentials */
	gnutls_session_t session;
	gnutls_dh_params_t dh_params; /**< server DH parameters for anon mode */
	relpTcpRtryState_t rtryOp;
} relpTcp_t;


/* macros for quick member access */
#define relpTcpGetNumSocks(pThis)    ((pThis)->socks[0])
#define relpTcpGetLstnSock(pThis, i) ((pThis)->socks[i])
#define relpTcpGetSock(pThis)        ((pThis)->sock)

/* inlines (only for library-internal use!) */
static inline relpTcpRtryState_t
relpTcpRtryOp(relpTcp_t *pThis)
{
	return pThis->rtryOp;
}

/* prototypes */
relpRetVal relpTcpConstruct(relpTcp_t **ppThis, relpEngine_t *pEngine);
relpRetVal relpTcpDestruct(relpTcp_t **ppThis);
relpRetVal relpTcpAbortDestruct(relpTcp_t **ppThis);
relpRetVal relpTcpLstnInit(relpTcp_t *pThis, unsigned char *pLstnPort, int ai_family);
relpRetVal relpTcpAcceptConnReq(relpTcp_t **ppThis, int sock, relpSrv_t *pSrv);
relpRetVal relpTcpRcv(relpTcp_t *pThis, relpOctet_t *pRcvBuf, ssize_t *pLenBuf);
relpRetVal relpTcpSend(relpTcp_t *pThis, relpOctet_t *pBuf, ssize_t *pLenBuf);
relpRetVal relpTcpConnect(relpTcp_t *pThis, int family, unsigned char *port, unsigned char *host, unsigned char *clientIP);
relpRetVal relpTcpEnableTLS(relpTcp_t *pThis);
relpRetVal relpTcpEnableTLSZip(relpTcp_t *pThis);
relpRetVal relpTcpSetDHBits(relpTcp_t *pThis, int bits);
int relpTcpGetRtryDirection(relpTcp_t *pThis);
void relpTcpRtryHandshake(relpTcp_t *pThis);

#endif /* #ifndef RELPTCP_H_INCLUDED */
