#include "HttpResponse.h"
#include <sstream>
namespace nekohttpd
{

const std::string ContentType::JSON = "application/json; charset=utf8";

void HttpResponse::addHeader(std::string name, std::string value)
{
    headers_.insert({name, value});
}

void HttpResponse::setHeader(std::string name, std::string value)
{
    headers_.erase(name);
    headers_.insert({name, value});
}

void HttpResponse::addCookie(std::string value)
{
    addHeader("cookie", value);
}

bool HttpResponse::containsHeader(std::string name)
{
    return headers_.find(name) != headers_.end();
}

void HttpResponse::sendError(HttpResponseStatus sc, std::string msg)
{
    body_ = msg;
    status_ = sc;
}

void HttpResponse::setStatus(HttpResponseStatus status)
{
    status_ = status;
}

HttpResponse::HttpResponseStatus HttpResponse::getStatus()
{
    return status_;
}

std::string HttpResponse::getHeader(std::string name)
{
    auto header = headers_.find(name);
    if (header != headers_.end())
    {
        return header->second;
    }
    return "";
}

void HttpResponse::setContent(std::string content)
{
    body_ = content;
}

void HttpResponse::appendContent(std::string content)
{
    body_ += content;
}

std::string HttpResponse::HttpResponse::toString()
{
    setHeader("content-length", std::to_string(body_.size()));
    setHeader("server", serverSignature_);
    std::stringstream stream;
    stream << std::noskipws;
    stream << version_ << " " << status_ << " " << translateStatusCode(status_) << "\r\n";
    for(auto& header : headers_)
    {
        stream << header.first << ": " << header.second << "\r\n";
    }
    stream << "\r\n" << body_;
    return stream.str();
}

std::string HttpResponse::translateStatusCode(HttpResponse::HttpResponseStatus status)
{
    std::string msg;
    switch (status)
    {
    case HttpResponseStatus::k200OK:
        msg = "OK";
        break;
    case HttpResponseStatus::k201Created:
        msg = "Created";
        break;
    case HttpResponseStatus::k202Accepted:
        msg = "Accepted";
        break;
    case HttpResponseStatus::k301MovedPermanently:
        msg = "Moved Permanently";
        break;
    case HttpResponseStatus::k302Found:
        msg = "Found";
        break;
    case HttpResponseStatus::k304NotModified:
        msg = "Not Modified";
        break;
    case HttpResponseStatus::k400BadRequest:
        msg = "Bad Request";
        break;
    case HttpResponseStatus::k401Unauthorized:
        msg = "Unauthorized";
        break;
    case HttpResponseStatus::k403Forbidden:
        msg = "Forbidden";
        break;
    case HttpResponseStatus::k404NotFound:
        msg = "Not Fouond";
        break;
    case HttpResponseStatus::k405MethodNotAllowed:
        msg = "Method Not Allowed";
        break;
    case HttpResponseStatus::k500InternalServerError:
        msg = "Internal Server Error";
        break;
    case HttpResponseStatus::k501NotImplemented:
        msg = "Not Implemented";
        break;
    case HttpResponseStatus::k502BadGateway:
        msg = "Bad Gateway";
        break;
    case HttpResponseStatus::k503ServiceUnavailable:
        msg = "Service Unavailable";
        break;
    default:
        break;
	}
    return msg;
}

}