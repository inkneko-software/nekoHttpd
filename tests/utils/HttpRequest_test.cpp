#include "../../utils/HttpRequest.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace nekohttpd;
int main()
{
    string buffer = "msg=hidden&username=inkneko&access=test";
    string packet = "GET /hello HTTP/1.1\r\n";
    packet += "Host: localhost\r\n";
    packet += "Content-Length: " + to_string(buffer.size()) + "\r\n\r\n";
    packet += buffer;

    HttpRequest request("127.0.0.1", 8888);
    cout << "status: " << request.httpProfiler(packet) << endl;
    cout << "method: " << request.getMethod() << endl;
    cout << "request path: " << request.getRequestURL() << endl;
    cout << "content length: " << request.getContentLength() << endl;
    cout << "content: " << request.getBody() << endl;

    cout << "second-->" << endl;

    string content = "POST /?msg=nihao&next=good HTTP/1.1\r\n"
                     "Host: www.baidu.com\r\n"
                     "Connection: keep-alive\r\n"
                     "Cache-Control: max-age=0\r\n"
                     "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"90\", \"Google Chrome\";v=\"90\"\r\n"
                     "sec-ch-ua-mobile: ?0\r\n"
                     "DNT: 1\r\n"
                     "Upgrade-Insecure-Requests: 1\r\n"
                     "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36\r\n"
                     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
                     "Sec-Fetch-Site: none\r\n"
                     "Sec-Fetch-Mode: navigate\r\n"
                     "Sec-Fetch-User: ?1\r\n"
                     "Sec-Fetch-Dest: document\r\n"
                     "Accept-Encoding: gzip, deflate, br\r\n"
                     "Accept-Language: zh-CN,zh;q=0.9\r\n"
                     "Content-Type: application/x-www-form-urlencoded\r\n"
                     "Cookie: cookie1=dick; session_id=test_session_id ;_uid=12345\r\n";
    content += "Content-Length: " + to_string(buffer.size()) + "\r\n\r\n";
    content += buffer;
    HttpRequest re2("127.0.0.1", 8888);
    cout << "status: " << re2.httpProfiler(content) << endl;
    cout << "method: " << re2.getMethod() << endl;
    cout << "request path: " << re2.getRequestURL() << endl;
    cout << "content length: " << re2.getContentLength() << endl;
    for(auto& name : re2.getHeaderNames())
    {
        for(auto& value : re2.getHeaders(name))
        {
            cout << "[" << name << "] " << value << endl;
        }
    }
    cout << "content: " << re2.getBody() << endl;
    for(auto& name : re2.getParameterNames())
    {
        for(auto& value: re2.getParameterValues(name))
        {
            cout << "<" << name << "> " << value << endl;
        }
    }

    cout << "cookie test:" << endl;
    for(auto&cookie : re2.getCookies())
    {
        cout << '[' << cookie.first << ']' << "->" << cookie.second << endl;
    }

    cout << "post parameter test: " << endl;
    cout << "[username]" << re2.getParameter("username") << endl;
    cout << "[access]" << re2.getParameter("access") << endl;
    cout << "[msg]" << re2.getParameter("msg") << endl;

    return 0;
}

