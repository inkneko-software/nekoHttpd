#ifndef NEKOHTTPD_UTILS_HTTPREQUEST_H
#include <string>
#include <map>
#include <vector>

namespace nekohttpd
{
class HttpRequest
{
public:
    HttpRequest(const std::string& clientAddr, const uint16_t& port)
    : clientAddr_(clientAddr), port_(port)
    {}
    /**
     * @brief
     * get request method.
     * @return std::string 
     */
    std::string getMethod();

    /**
     * @brief 
     * get all cookies.
     * @return std::map<std::string, std::string> all cookies
     */
    std::map<std::string, std::string> getCookies();

    /**
     * @brief 
     * get specified cookie
     * @param name the name of the cookie
     * @return std::string cookie value
     */
    std::string getCookie(std::string name);

    /**
     * @brief
     * get header. returns the first header.
     * @param name 
     * @return std::string 
     */
    std::string getHeader(std::string name, bool caseSensitive=false);

    /**
     * @brief 
     * get headers.
     * @param name 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getHeaders(std::string , bool caseSensitive=false);

    /**
     * @brief 
     * get header names
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getHeaderNames();

    /**
     * @brief 
     * get request URI
     * @return std::string 
     */
    std::string getRequestURI();

    /**
     * @brief
     * get request url
     * @return std::string 
     */
    std::string getRequestURL();

    /**
     * @brief
     * get query string
     * @return std::string 
     */
    std::string getQueryString();

    /**
     * @brief
     * get parameter occured in querystring and post body
     * @param name 
     * @return std::string 
     */
    std::string getParameter(std::string name);

    /**
     * @brief 
     * get parameter values
     * @param name 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getParameterValues(std::string name);

    /**
     * @brief
     * get parameter names
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getParameterNames();

    /**
     * @brief
     * get remote host info.
     * @return std::pair<std::string, uint16_t> 
     *              ipaddr, port
     */
    std::pair<std::string, uint16_t> getRemoteHost();

    /**
     * @brief
     * get remote host ip:port in str
     * @return std::string ip:port
     */
    std::string getRemoteIPPort();

    //session module
    //HttpSession getSession(bool create = false);
    //std::string getSessionId();

    std::istream getInputStream() = delete;

    int getContentLength();
    std::string getBody();

    enum ProfilerStatus
    {
        khead = 1,
        kbadRequest = 2,
        kcontentPadding = 3,
        kcomplete = 4
    };

    ProfilerStatus httpProfiler(const std::string &buffer);
private:
    std::string clientAddr_;
    uint16_t port_;

    ProfilerStatus status_;
    std::string method_;
    std::string version_;
    std::string requestURI_;
    std::string requestURL_;
    std::string queryString_;
    std::multimap<std::string, std::string> headers_;
    std::string body_;
    std::multimap<std::string, std::string> parameters_;
    std::map<std::string, std::string> cookies_;
};

} // namespace nekohttpd

#endif // NEKOHTTPD_UTILS_HTTPREQUEST_H