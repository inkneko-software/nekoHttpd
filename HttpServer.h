#ifndef NEKOHTTPSERVER_NEKOHTTPSERVER_H
#define NEKOHTTPSERVER_NEKOHTTPSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include "utils/HttpRequest.h"
#include "utils/HttpResponse.h"

namespace nekohttpd
{

using namespace muduo;
using namespace muduo::net;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class HttpServer
{
public:
	HttpServer(EventLoop* loop, const InetAddress& listenAddr) 
	: loop_(loop),
	  server_(loop, listenAddr, "nekoHttpd") 
	{
		server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1));
		server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2,_3));
		server_.setThreadNum(4);
	}

	/**
	 * @brief 
	 * add a request mapping. If the path exists, calling of this method will override the previous one.
	 * @param path 
	 * 				the request path
	 * @param callback 
	 * 				the request callback
	 * @param method 
	 * 				the coresponsed method
	 */
	void requestMapping(std::string path, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> callback, const std::string& method="all");
	
	/**
	 * @brief 
	 * add a get method request mapping. If the path exists, calling of this method will override the previous one.
	 * @param path 
	 * 				the request path
	 * @param callback 
	 * 				the request callback
	 */
	void getMapping(std::string path, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> callback);
	/**
	 * @brief 
	 * add a post method request mapping. If the path exists, calling of this method will override the previous one.
	 * @param path 
	 * 				the request path
	 * @param callback 
	 * 				the request callback
	 */
	void postMapping(std::string path, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> callback);

	/**
	 * @brief 
	 * Start the server. The function will block the current thread until the server terminated.
	 */
	void start();

private:
	void onConnection(const TcpConnectionPtr &conn);
	void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp);
	std::shared_ptr<HttpResponse> HttpHandler(std::shared_ptr<HttpRequest> request);
private:
	EventLoop *loop_;
	TcpServer server_;
	std::map<std::string, std::pair<std::string, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)>>> requestMapping_;
};

} //namespace nekohttpd


#endif //NEKOHTTPSERVER_NEKOHTTPSERVER_H