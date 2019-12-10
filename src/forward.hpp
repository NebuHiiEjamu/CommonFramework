#ifndef _FORWARD_H
#define _FORWARD_H

#include <memory>

template <class T> class Connection;
class Hive;
template <class T> class Listener;
class Logger;

template <class T> using ConnectionPtr = std::shared<Connection<T>>;
template <class T> using ConnectionRef = std::weak_ptr<Connection<T>>;
using HivePtr = std::shared<Hive>;
using HiveRef = std::weak_ptr<Hive>;
template <class T> using ListenerPtr = std::shared<Listener<T>>;
using LoggerPtr = std::shared<Logger>;
using LoggerRef = std::weak_ptr<Logger>;

#endif // _FORWARD_H
