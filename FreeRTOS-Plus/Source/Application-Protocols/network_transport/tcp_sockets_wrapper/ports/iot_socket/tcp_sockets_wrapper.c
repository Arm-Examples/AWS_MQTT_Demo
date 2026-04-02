/* -----------------------------------------------------------------------------
 * Copyright (c) 2024-2026 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * -----------------------------------------------------------------------------
 */

#include "iot_socket.h"
#include "tcp_sockets_wrapper.h"

// Map IoT return codes to socket wrapper errors
static int32_t rc_to_errno (int32_t rc) {
  switch (rc) {
    case 0:                         return TCP_SOCKETS_ERRNO_NONE;
    case IOT_SOCKET_ERROR:          return TCP_SOCKETS_ERRNO_ERROR;
    case IOT_SOCKET_ESOCK:          return TCP_SOCKETS_ERRNO_EINVAL;
    case IOT_SOCKET_EINVAL:         return TCP_SOCKETS_ERRNO_EINVAL;
    case IOT_SOCKET_ENOTSUP:        return TCP_SOCKETS_ERRNO_ERROR;
    case IOT_SOCKET_ENOMEM:         return TCP_SOCKETS_ERRNO_ENOMEM;
    case IOT_SOCKET_EAGAIN:         return TCP_SOCKETS_ERRNO_EWOULDBLOCK;
    case IOT_SOCKET_EINPROGRESS:    return TCP_SOCKETS_ERRNO_EWOULDBLOCK;
    case IOT_SOCKET_ETIMEDOUT:      return TCP_SOCKETS_ERRNO_ERROR;
    case IOT_SOCKET_EISCONN:        return TCP_SOCKETS_ERRNO_EISCONN;
    case IOT_SOCKET_ENOTCONN:       return TCP_SOCKETS_ERRNO_ENOTCONN;
    case IOT_SOCKET_ECONNREFUSED:   return TCP_SOCKETS_ERRNO_ENOTCONN;
    case IOT_SOCKET_ECONNRESET:     return TCP_SOCKETS_ERRNO_ECLOSED;
    case IOT_SOCKET_ECONNABORTED:   return TCP_SOCKETS_ERRNO_ECLOSED;
    case IOT_SOCKET_EALREADY:       return TCP_SOCKETS_ERRNO_EWOULDBLOCK;
    case IOT_SOCKET_EADDRINUSE:     return TCP_SOCKETS_ERRNO_EINVAL;
    case IOT_SOCKET_EHOSTNOTFOUND:  return TCP_SOCKETS_ERRNO_ERROR;
    default:                        return TCP_SOCKETS_ERRNO_ERROR;
  }
}

/**
 * @brief Establish a connection to server.
 *
 * @param[out] pTcpSocket The output parameter to return the created socket descriptor.
 * @param[in] pHostName Server hostname to connect to.
 * @param[in] pServerInfo Server port to connect to.
 * @param[in] receiveTimeoutMs Timeout (in milliseconds) for transport receive.
 * @param[in] sendTimeoutMs Timeout (in milliseconds) for transport send.
 *
 * @note A timeout of 0 means infinite timeout.
 *
 * @return Non-zero value on error, 0 on success.
 */
BaseType_t TCP_Sockets_Connect (Socket_t *pTcpSocket, const char *pHostName,
                                uint16_t port, uint32_t receiveTimeoutMs, uint32_t sendTimeoutMs) {
  int32_t sock, rc;
  uint32_t ip, ip_len;

  /* Create a new TCP socket. */
  rc = iotSocketCreate (IOT_SOCKET_AF_INET, IOT_SOCKET_SOCK_STREAM, IOT_SOCKET_IPPROTO_TCP);
  if (rc < 0) {
    return (rc_to_errno (rc));
  }
  sock = rc;

  /* Resolve host address */
  ip_len = sizeof(ip);
  rc = iotSocketGetHostByName (pHostName, IOT_SOCKET_AF_INET, (uint8_t *)&ip, &ip_len);
  if (rc < 0) {
    iotSocketClose (sock);
    return (rc_to_errno (rc));
  }

  /* Establish connection */
  rc = iotSocketConnect (sock, (uint8_t *)&ip, 4U, port);
  if (rc < 0) {
    iotSocketClose (sock);
    return (rc_to_errno (rc));
  }

  /* Set socket receive timeout */
  rc = iotSocketSetOpt (sock, IOT_SOCKET_SO_RCVTIMEO, (const void *)&receiveTimeoutMs, 4);
  if (rc < 0) {
    iotSocketClose (sock);
    return (rc_to_errno (rc));
  }

  /* Set socket send timeout */
  rc = iotSocketSetOpt (sock, IOT_SOCKET_SO_SNDTIMEO, (const void *)&sendTimeoutMs, 4);
  if (rc < 0) {
    iotSocketClose (sock);
    return (rc_to_errno (rc));
  }
  *(int32_t *)pTcpSocket = (sock + 1);
  return (rc);
}

/**
 * @brief End connection to server.
 *
 * @param[in] tcpSocket The socket descriptor.
 */
void TCP_Sockets_Disconnect (Socket_t tcpSocket) {
  int32_t sock = (int32_t)tcpSocket - 1;

  iotSocketClose (sock);
}

/**
 * @brief Transmit data to the remote socket.
 *
 * The socket must have already been created using a call to TCP_Sockets_Connect().
 *
 * @param[in] xSocket The handle of the sending socket.
 * @param[in] pvBuffer The buffer containing the data to be sent.
 * @param[in] xDataLength The length of the data to be sent.
 *
 * @return
 * * On success, the number of bytes actually sent is returned.
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t TCP_Sockets_Send (Socket_t xSocket, const void *pvBuffer, size_t xBufferLength) {
  int32_t rc, sock = (int32_t)xSocket - 1;

  rc = iotSocketSend (sock, pvBuffer, xBufferLength);
  if (rc < 0) {
    return (rc_to_errno (rc));
  }

  return (rc);
}

/**
 * @brief Receive data from a TCP socket.
 *
 * The socket must have already been created using a call to TCP_Sockets_Connect().
 *
 * @param[in] xSocket The handle of the socket from which data is being received.
 * @param[out] pvBuffer The buffer into which the received data will be placed.
 * @param[in] xBufferLength The maximum number of bytes which can be received.
 * pvBuffer must be at least xBufferLength bytes long.
 *
 * @return
 * * If the receive was successful then the number of bytes received (placed in the
 *   buffer pointed to by pvBuffer) is returned.
 * * If a timeout occurred before data could be received then 0 is returned (timeout
 *   is set using @ref SOCKETS_SO_RCVTIMEO).
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t TCP_Sockets_Recv (Socket_t xSocket, void *pvBuffer, size_t xBufferLength) {
  int32_t rc, sock = (int32_t)xSocket - 1;

  rc = iotSocketRecv (sock, pvBuffer, xBufferLength);
  if (rc < 0) {
    if (rc == IOT_SOCKET_EAGAIN) {
      return (0);
    }
    return (rc_to_errno (rc));
  }

  return (rc);
}
