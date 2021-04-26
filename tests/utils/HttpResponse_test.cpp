#include "../../utils/HttpResponse.h"
#include <iostream>
using namespace nekohttpd;
using namespace std;
int main()
{
    HttpResponse response;
    response.setStatus(HttpResponse::k404NotFound);
    response.setContent("hello server!");
    cout << response.toString() << endl;
    return 0;
}