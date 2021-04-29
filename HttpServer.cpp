#include "HttpServer.h"
#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>

#include "utils/webstring.h"

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Atomic.h>

#define SERVER_SIGNATURE "NekoHttpServer/v1.1"
namespace nekohttpd
{

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    const InetAddress &remote = conn->peerAddress();
    std::shared_ptr<HttpRequest> request(new HttpRequest(remote.toIp(), remote.port()));
    conn->setContext(request);
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
    std::shared_ptr<HttpRequest> request = boost::any_cast<std::shared_ptr<HttpRequest>>(conn->getContext());
    switch(request->httpProfiler(buf->peek()))
    {
        case HttpRequest::kcomplete:
            conn->send(HttpHandler(request)->toString());
            conn->forceClose();
            break;
        default:
            break;
    }
}

void HttpServer::requestMapping(std::string path, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> callback, const std::string& method)
{
    requestMapping_[path] = {method, callback};
}
void HttpServer::getMapping(std::string path, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> callback)
{
    requestMapping_[path] = {"get", callback};
}
void HttpServer::postMapping(std::string path, std::function<void(std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>)> callback)
{
    requestMapping_[path] = {"post", callback};
}

std::shared_ptr<HttpResponse> HttpServer::HttpHandler(std::shared_ptr<HttpRequest> request)
{
    LOG_INFO << "peer: " << request->getRemoteIPPort() << " " << request->getMethod() <<" requesting: " << request->getRequestURI();
    std::shared_ptr<HttpResponse> response(new HttpResponse);
    response->setServerSignature(SERVER_SIGNATURE);

    auto mapping = requestMapping_.find(request->getRequestURI());
    if (mapping != requestMapping_.end())
    {
        mapping->second.second(request, response);
        return response;
    }
    response->setStatus(response->k404NotFound);
    response->setContent("Page not found. <br> requesting: " + request->getRequestURL());
    response->setContentType(ContentType::HTML_UTF8);
    return response;
}

void HttpServer::start()
{
    server_.start();
    loop_->loop();
}

}
