#include "HttpRequest.h"
#include "webstring.h"
#include <regex>

namespace nekohttpd
{
std::string HttpRequest::getMethod()
{
    return method_;
}

std::vector<std::string> HttpRequest::getCookies()
{
    std::vector<std::string> cookies;
    for (auto iter = headers_.find("cookie"); iter != headers_.end(); ++iter)
    {
        cookies.push_back(iter->second);
    }
    return cookies;
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
        return iter->first;
    }
    return "";
}

std::vector<std::string> HttpRequest::getParameterValues(std::string name)
{
    std::vector<std::string> values;
    auto range = parameters_.equal_range(name);

    for (auto iter = range.first; iter != range.second; ++iter)
    {
        values.push_back(iter->second);
    }
    return values;
}

std::vector<std::string> HttpRequest::getParameterNames()
{
    std::vector<std::string> names;
    for (auto iter = parameters_.begin(); iter != parameters_.end(); ++iter)
    {
        names.push_back(iter->first);
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

std::string HttpRequest::getBody()
{
    return body_;
}

HttpRequest::ProfilerStatus HttpRequest::httpProfiler(const std::string& buffer)
{
	using namespace std;
	regex requestLineFmt("^([A-Z]+?) (\\S+?) HTTP/(.+?)\r\n");
	regex requestHeaderFmt("(.+?):(.*?)\r\n");

	smatch requestLine;
	smatch requestHeaders;

	//查找报文头(header)与报文体(body)的分割
	size_t requestHeaderEnd = buffer.find("\r\n\r\n");
	if (requestHeaderEnd != string::npos)
	{
		//判断该数据是否完整匹配一个报文请求行(request line)的开始
		if (!regex_search(buffer,  requestLine, requestLineFmt))
		{
			return kbadRequest;
		}

		sregex_iterator itend;
		for (sregex_iterator it(buffer.cbegin(), buffer.cbegin() + requestHeaderEnd + 2, requestHeaderFmt); it != itend; ++it)
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
			method_ = requestLine[1].str();
			size_t paramStart = requestLine[2].str().find("?");
			if (paramStart != std::string::npos)
			{
				requestURI_ = requestLine[2].str().substr(0, paramStart);
                queryString_ = requestLine[2].str().substr(paramStart + 1);
                parameters_ = webstring::ParseKeyValue(queryString_);
            }
			else
			{
				requestURI_ = requestLine[2].str().substr(0, paramStart);
			}

            requestURL_ = requestLine[2].str();
            version_ = requestLine[3].str();

            body_ = buffer.substr(requestHeaderEnd + 4, requestHeaderEnd + 4 + contentLength);
		}
        return kcomplete;
    }
	    return kbadRequest;
}


}