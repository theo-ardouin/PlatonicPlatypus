#include "tserver.h"

ts_t* ts_create(int ai_family, uint16_t port)
{
  ts_t* ts;

  if ((ts = malloc(sizeof(*ts))) == NULL)
    return (NULL);
  if ((ts->clients = ll_create()) == NULL)
    return (NULL);
  if ((ts->select = sl_create()) == NULL)
    return (NULL);
  if (!ts_init_socket(&ts->server, ai_family, port))
    return (NULL);
  if (sl_add(ts->select, SL_READ, ts->server.sock) == NULL)
    return (NULL);
  return (ts);
}

ts_t* ts_update(ts_t* server, float seconds)
{
  int update;
  ll_t* tmp;
  sl_sock_t* s;

  if (server == NULL)
    return (NULL);
  if ((update = sl_update(server->select, seconds)) == -1)
    return (NULL);
  if (update == 0)
    return (server);
  tmp = server->select->fds->begin;
  while (tmp)
  {
    s = tmp->elem;
    if (FD_ISSET(s->sock, &server->select->readfs))
    {
      //TODO
    }
    if (FD_ISSET(s->sock, &server->select->writefs))
    {
      //TODO
    }
    tmp = tmp->next;
  }
  //TODO
  return (server);
}

void ts_destroy(ts_t* server)
{
  if (server == NULL)
    return;
  //TODO Remove clients
  free(server);
}

int ts_init_socket(sk_t* sk, int ai_family, uint16_t port)
{
  char strport[8];
  struct addrinfo* tmp;
  struct addrinfo* result;
  struct addrinfo hints = { 0 };

  hints.ai_family = ai_family;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  sprintf(strport, "%hu", port);
  if (getaddrinfo(NULL, strport, &hints, &result) != 0)
    return (0);
  tmp = result;
  while (tmp)
  {
    if ((sk->sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol)) != INVALID_SOCKET)
    {
      if (ts_bind_socket(sk, tmp))
      {
        memcpy(&sk->addr, tmp->ai_addr, tmp->ai_addrlen);
        sk->addr_size = tmp->ai_addrlen;
        break;
      }
      sk_close(sk);
    }
    tmp = tmp->ai_next;
  }
  freeaddrinfo(result);
  return (tmp != NULL);
}

int ts_bind_socket(sk_t* socket, struct addrinfo* addrinfo)
{
  char reuse = 1;

  if (bind(socket->sock, addrinfo->ai_addr, addrinfo->ai_addrlen) == SOCKET_ERROR)
    return (0);
  if (listen(socket->sock, SOMAXCONN) == SOCKET_ERROR)
    return (0);
  setsockopt(socket->sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  return (1);
}