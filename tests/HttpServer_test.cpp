#include "../HttpServer.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace nekohttpd;
using namespace muduo;
using namespace muduo::net;

int main()
{
    EventLoop loop;
    InetAddress listenAddr(8080);
    HttpServer server(&loop, listenAddr);

    server.getMapping("/helloworld", [](std::shared_ptr<HttpRequest> req, std::shared_ptr<HttpResponse> resp) {
        resp->setContent("<h1>Hello from NekoHttpd!</h1>");
        resp->appendContent("来自墨猫超文本承载器");
        resp->setContentType(ContentType::HTML_UTF8);
    });

    server.start();

    return 0;
}