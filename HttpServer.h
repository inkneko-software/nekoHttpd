#ifndef NEKOHTTPSERVER_NEKOHTTPSERVER_H

#include <string>
#include <map>
#include <set>
#include <memory>
#include <fstream>
#include <functional>
#include <ctime>

#include "HTTPPacket.h"
namespace nekohttpd
{

class HttpServer
{
public:
	using HttpCallback = std::function<HTTPPacket::HTTPResponsePacket(HTTPPacket::HTTPRequestPacket, HTTPPacket::HTTPResponsePacket)>;

	std::map<std::string, std::string> Init(std::string confPath = "setting.conf") noexcept(false);

	~HttpServer();

private:
	std::map<std::string, HttpCallback> routeTable;

};

} //namespace nekohttpd


#endif //NEKOHTTPSERVER_NEKOHTTPSERVER_H