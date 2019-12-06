#ifndef _NET_FORWARD_H
#define _NET_FORWARD_H

#include <memory>

template <class T> class Connection;
class Hive;
template <class T> class Listener;

using ConnectionPtr = std::shared<Connection<T>>;
using HivePtr = std::shared<Hive>;
using ListenerPtr = std::shared<Listener<T>>;

#endif // _NET_FORWARD_H
