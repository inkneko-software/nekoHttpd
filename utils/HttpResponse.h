#ifndef NEKOHTTPD_UTILS_HTTPRESPONSE_H
#include <string>
#include <map>
#include <vector>

namespace nekohttpd
{

class ContentType{
public:
	static const std::string HTML_UTF8;
	static const std::string JSON;
}; 


class HttpResponse
{
public:
	HttpResponse() = default;
	enum HttpResponseStatus
	{
		k200OK = 200,
		k201Created = 201,
		k202Accepted = 202,
		k301MovedPermanently = 301,
		k302Found = 302,
		k304NotModified = 304,
		k400BadRequest = 400,
		k401Unauthorized = 401,
		k403Forbidden = 403,
		k404NotFound = 404,
		k405MethodNotAllowed = 405,
		k500InternalServerError = 500,
		k501NotImplemented = 501,
		k502BadGateway = 502,
		k503ServiceUnavailable = 503
	};

	/**
	 * @brief
	 * Set the response status
	 * @param status the status
	 */
	void setStatus(HttpResponseStatus status);

    /**
     * @brief
     * Get the current HTTP status code.
     * @return HttpResponseStatus the status code.
     */
    HttpResponseStatus getStatus();

	/**
	 * @brief 
	 * Add a header with given name and value.
	 * This method allows to add multiple values to the same header name.(by duplicated it, eg. set-cookie: a \r\n set-cookie: b)
	 * @param name 
	 * 				the name of the header.
	 * @param value 
	 * 				the value of the header.
	 */
	void addHeader(std::string name, std::string value);

	/**
	 * @brief
	 * Set the header with given name and value.
	 * If the header with that name exists, the value will be overwritten. 
	 * @param name 
	 * 				the name of the header.
	 * @param value 
	 * 				the value of the header.
	 */
	void setHeader(std::string name, std::string value);

	/**
	 * @brief 
	 * Add cookie value to this response.
	 * The cookie can be added for multiple times.
	 * @param value 
	 * 				the value of the cookie.
	 */
	void addCookie(std::string value);

	/**
	 * @brief
	 * set cookie
	 * @param key
	 * 				the cookie name
	 * @param value
	 * 				the cookie value
	 * @param expires_seconds 
	 * 				the expired time, in million seconds
	 * @param domain 
	 * 				the applied domain
	 * @param path 
	 * 				the applied path
	 */
	void setCookie(std::string key, std::string value, time_t expiresMilliseconds=-1, std::string domain="", std::string path="");

	/**
	 * @brief 
	 * Detect wheather the header exists.
	 * @param name 
	 * 				the name of the header.
	 * @return true
	 * 				the header exists.
	 * @return false 
	 * 				the header does not exists.
	 */
	bool containsHeader(std::string name);

    /**
     * @brief 
     * Send the http error to the client.
     * This will clear the body, replace it with a html like error page and the provided msg. The added headers WILL NOT be modified.
     * @param sc 
     *              the status code.
     * @param msg
     *              the customed message.
     */
    void sendError(HttpResponseStatus sc, std::string msg);

    /**
     * @brief
     * Get the first header presented in the response.
     * @param name 
     *              the header name.
     * @return std::string the header value. 
     *              If the header does not exist, empty string will return. Thus use containsHeader if necessary.
     */
    std::string getHeader(std::string name);

	/**
	 * @brief 
	 * set response content-type
	 * @param contentType
	 * 				the content type
	 */
	void setContentType(const std::string &contentType);


	/**
	 * @brief 
	 * set header field [server]
	 * @param name 
	 * 				the server name
	 */
	void setServerSignature(const std::string& name);
	/**
	 * @brief 
	 * set message.
	 * @param content 
	 * 				the content to be set as message.
	 */
	void setContent(std::string content);

	/**
	 * @brief 
	 * append the content to the end of the message.
	 * @param content 
	 * 				the content to be appended.
	 */
	void appendContent(std::string content);

	/**
	 * @brief 
	 * Generate HTTP packet
	 * @return std::string 
	 */
	std::string toString();

private:
    HttpResponseStatus status_ = k200OK;
    std::multimap<std::string, std::string> headers_;
	std::string body_;
	std::string version_ = "HTTP/1.1";

private:
	std::string translateStatusCode(HttpResponseStatus status);
};

} // namespace nekohttpd

#endif //NEKOHTTPD_UTILS_HTTPRESPONSE_H