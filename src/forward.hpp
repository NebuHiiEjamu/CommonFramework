#ifndef _FORWARD_H
#define _FORWARD_H

#include <memory>

class Acceptor;
template <class T> class Connection;
class Hive;
class Logger;

using AcceptorPtr = std::shared<Listener>;
template <class T> using ConnectionPtr = std::shared<Connection<T>>;
template <class T> using ConnectionRef = std::weak_ptr<Connection<T>>;
using HivePtr = std::shared<Hive>;
using HiveRef = std::weak_ptr<Hive>;
using LoggerPtr = std::shared<Logger>;
using LoggerRef = std::weak_ptr<Logger>;

#endif // _FORWARD_H
