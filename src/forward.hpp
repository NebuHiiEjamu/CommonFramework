#ifndef _FORWARD_H
#define _FORWARD_H

#include <memory>

class Acceptor;
template <class T> class Connection;
class Hive;
class Logger;

using AcceptorPtr = std::shared_ptr<Acceptor>;
template <class T> using ConnectionPtr = std::shared_ptr<Connection<T>>;
template <class T> using ConnectionRef = std::weak_ptr<Connection<T>>;
using HivePtr = std::shared_ptr<Hive>;
using HiveRef = std::weak_ptr<Hive>;
using LoggerPtr = std::shared_ptr<Logger>;
using LoggerRef = std::weak_ptr<Logger>;

#endif // _FORWARD_H
