#ifndef _NET_FORWARD_H
#define _NET_FORWARD_H

#include <memory>

template <class T> class Connection;
class Hive;
template <class T> class Listener;

template <class T> using ConnectionPtr = std::shared<Connection<T>>;
using HivePtr = std::shared<Hive>;
template <class T> using ListenerPtr = std::shared<Listener<T>>;

#endif // _NET_FORWARD_H
