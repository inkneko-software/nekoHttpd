#include "../../utils/HttpResponse.h"
#include <iostream>
using namespace nekohttpd;
using namespace std;
int main()
{
    HttpResponse response;
    response.setStatus(HttpResponse::k404NotFound);
    response.setContent("hello server!");
    response.setCookie("_session_id", "aes256ae2802b02df013f7d1f272ea1247e4a27d4747f8", 30 * 24 * 60 * 60l, "inkneko.com", "/");
    cout << response.toString() << endl;
    return 0;
}