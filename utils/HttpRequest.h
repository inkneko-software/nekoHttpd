#ifndef NEKOHTTPD_UTILS_HTTPREQUEST_H
#include <string>
#include <map>
#include <vector>

namespace nekohttpd
{
class HttpRequest
{
public:
    /**
     * @brief
     * get request method.
     * @return std::string 
     */
    std::string getMethod();

    /**
     * @brief 
     * get cookie.
     * @return std::vector<std::string> 
     */
    std::vector<std::string> getCookies();

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
    ProfilerStatus status_;
    std::string method_;
    std::string version_;
    std::string requestURI_;
    std::string requestURL_;
    std::string queryString;
    std::multimap<std::string, std::string> headers_;
    std::string body_;
    std::multimap<std::string, std::string> parameters_;
};

} // namespace nekohttpd

#endif // NEKOHTTPD_UTILS_HTTPREQUEST_H