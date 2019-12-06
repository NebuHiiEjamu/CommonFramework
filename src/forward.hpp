#ifndef _NET_FORWARD_H
#define _NET_FORWARD_H

#include <memory>

class Connection;
class Hive;
class Listener;

using HivePtr = std::shared<Hive>;
using ListenerPtr = std::shared<Listener>;
using ConnectionPtr = std::shared<Connection>;

#endif // _NET_FORWARD_H
