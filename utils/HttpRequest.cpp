#include "HttpRequest.h"
#include "webstring.h"
//#include <regex>
#include <boost/regex.hpp>
#include <iostream>

namespace nekohttpd
{
std::string HttpRequest::getMethod()
{
    return method_;
}

std::map<std::string, std::string> HttpRequest::getCookies()
{
    return cookies_;
}

std::string HttpRequest::getCookie(std::string name)
{
    return cookies_[name];
}

std::string HttpRequest::getHeader(std::string name, bool caseSensitive)
{
    for(auto& header : headers_)
    {
        if (caseSensitive)
        {
            if (header.first == name)
            {
                return header.second;
            }
        }
        else
        {
            if (webstring::tolower(header.first) == webstring::tolower(name))
            {
                return header.second;
            }
        }
    }
    return "";
}

std::vector<std::string> HttpRequest::getHeaders(std::string name, bool caseSensitive)
{
    std::vector<std::string> values;
    for(auto& header : headers_)
    {
        if (caseSensitive)
        {
            if (header.first == name)
            {
                values.push_back(header.second);
            }
        }
        else
        {
            if (webstring::tolower(header.first) == webstring::tolower(name))
            {
                values.push_back(header.second);
            }
        }
    }
    return values;
}

std::vector<std::string> HttpRequest::getHeaderNames()
{
    std::vector<std::string> names;
    for (auto iter = headers_.begin(); iter != headers_.end(); ++iter)
    {
        names.push_back(iter->first);
    }
    return names;
}

std::string HttpRequest::getRequestURI()
{
    return requestURI_;
}

std::string HttpRequest::getRequestURL()
{
    return requestURL_;
}

std::string HttpRequest::getQueryString()
{
    return queryString_;
}

std::string HttpRequest::getParameter(std::string name)
{
    auto iter = parameters_.find(name);
    if (iter != parameters_.end())
    {
        return webstring::urldecode(iter->second);
    }

    return "";
}

std::vector<std::string> HttpRequest::getParameterValues(std::string name)
{
    std::vector<std::string> values;
    auto range = parameters_.equal_range(name);

    for (auto iter = range.first; iter != range.second; ++iter)
    {
        values.push_back(webstring::urldecode(iter->second));
    }
    return values;
}

std::vector<std::string> HttpRequest::getParameterNames()
{
    std::vector<std::string> names;
    for (auto iter = parameters_.begin(); iter != parameters_.end(); ++iter)
    {
        names.push_back(webstring::urldecode(iter->first));
    }
    return names;
}

std::pair<std::string, uint16_t> HttpRequest::getRemoteHost()
{
    return {clientAddr_, port_};
}

std::string HttpRequest::getRemoteIPPort()
{
    return clientAddr_ + ":" + std::to_string(port_);
}

int HttpRequest::getContentLength()
{
    try{
        return std::stoi(getHeader("content-length"));
    }catch(...){
        return 0;
    }
}

std::string HttpRequest::getContentType(){
    return getHeader("content-type");
}

std::string HttpRequest::getBody()
{
    return body_;
}

HttpRequest::ProfilerStatus HttpRequest::httpProfiler(const std::string& buffer)
{

    using namespace boost;
    regex requestLineFmt("^([A-Z]+) (\\S+) HTTP/(.+?)\r\n");
    regex requestHeaderFmt("(.+?):(.*?)\r\n");

    smatch requestLine;
    smatch requestHeaders;

    //查找报文头(header)与报文体(body)的分割
    size_t requestLineEnd = buffer.find("\r\n");
    size_t requestHeaderEnd = buffer.find("\r\n\r\n");
    if (requestHeaderEnd != std::string::npos)
    {
        //判断该数据是否完整匹配一个报文请求行(request line)的开始
        if (!regex_search(buffer,  requestLine, requestLineFmt))
        {
            return kbadRequest;
        }

        sregex_iterator itend;
        for (sregex_iterator it(buffer.cbegin() + requestLineEnd + 2, buffer.cbegin() + requestHeaderEnd + 2, requestHeaderFmt); it != itend; ++it)
        {
            headers_.insert({ webstring::strip(it->operator[](1).str()), webstring::strip(it->operator[](2).str()) });
        }

        //如果没有host头，则为bad request
        if (getHeader("host") == "")
        {
            return kbadRequest;
        }

        //判断是否有body
        size_t contentLength = getContentLength();


        //判断当前剩余的数据长度是否满足content-length的长度
        if (buffer.length() - (requestHeaderEnd + 4) >= contentLength)
        {
            //query string的处理
            method_ = requestLine.str(1);
            size_t paramStart = requestLine.str(2).find("?");
            if (paramStart != std::string::npos)
            {
                requestURI_ = requestLine.str(2).substr(0, paramStart);
                queryString_ = requestLine.str(2).substr(paramStart + 1);
                parameters_ = webstring::ParseKeyValue(queryString_);
            }
            else
            {
                requestURI_ = requestLine.str(2).substr(0, paramStart);
            }

            requestURL_ = requestLine.str(2);
            version_ = requestLine.str(3);

            body_ = buffer.substr(requestHeaderEnd + 4, requestHeaderEnd + 4 + contentLength);
            //如果请求内容的类型为application/x-www-form-urlencoded，则将请求数据添加到parameters_中
            if (getContentType().find("application/x-www-form-urlencoded") != std::string::npos)
            {
                auto postParameter = webstring::ParseKeyValue(body_);
                for(auto& iter : postParameter)
                {
                    parameters_.insert(iter.first, iter.second);
                }
            }


            std::vector<std::string> cookies = getHeaders("cookie");
            for (auto& cookie : cookies)
            {
                std::multimap<std::string, std::string> parsedCookies = webstring::ParseKeyValue(cookie, '=', ';');
                for(auto& parsedCookie : parsedCookies)
                {
                    cookies_[parsedCookie.first] = parsedCookie.second;
                }
            }
        }
        return kcomplete;
    }
        return kbadRequest;
}


}
