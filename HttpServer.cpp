#include "HttpServer.h"
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>

#include "webstring.h"


#define WATERLINE_WRITE_BUFFER 4096
#define SERVER_SIGNATURE "NekoHttpServer/v1.1"


std::map<std::string, std::string> RetrieveFromKeyValueFmt(std::string path)
{
	std::fstream file(path, std::fstream::in);
	if (file.good() == false)
	{
		throw std::runtime_error(std::string("Failed to open file: ") + path);
	}
	file.seekg(0, file.end);
	size_t fileLength = file.tellg();
	file.seekg(0, file.beg);

	if (fileLength <= 0)
	{
		return {};
	}
	std::unique_ptr<char[]> buffer(new char[fileLength]());
	file.read(buffer.get(), fileLength);
	file.close();

	std::string fileContent(buffer.get());
	std::regex configFormat("(.+?)=(.+?)\n");
	std::regex commentFormat("([^#]*?)#(.*?)");
	std::sregex_iterator rend;
	std::map<std::string, std::string> settingMap;

	for (auto it = std::sregex_iterator(fileContent.begin(), fileContent.end(), configFormat); it != rend; ++it)
	{
		std::smatch result;
		std::string buffer((*it)[2].str());
		if (std::regex_match(buffer, result, commentFormat))
		{
			settingMap.insert({ webstring::strip((*it)[1].str()), webstring::strip(result[1].str()) });
		}
		else
		{
			settingMap.insert({ webstring::strip((*it)[1].str()), webstring::strip((*it)[2].str()) });
		}
	}

	return settingMap;

}

NekoHttpServer::~NekoHttpServer()
{
	if (serverProperty.verbose >= VerboseLevel::less)
	{
		std::cout << "Closing server...." << std::endl;
	}
	
	for (auto client : connectedClients)
	{
		close(client.first);
	}
	close(serverProperty.listenfd);
}

std::map<std::string, std::string> NekoHttpServer::Init(std::string confPath) noexcept(false)
{
	using namespace std;
	map<string, string> setting = RetrieveFromKeyValueFmt(confPath);
	if (setting.count("port") && setting.count("address"))
	{
		try
		{
			serverProperty.bind = { stoi(setting["port"]), webstring::strip(setting["address"], "\"\'") };
		}
		catch(...)
		{
			throw runtime_error("Invaild port or address, examine your configuration file.");
		}
	}

	if (setting.count("unixPath") != 0)
	{
		serverProperty.unixBind = webstring::strip(setting["unixPath"], "\"\'");
	}
	else
	{
		serverProperty.unixBind = "/var/run/NekoHttpServer/";
	}

	if (setting.count("maxClients") != 0)
	{
		try
		{
			serverProperty.maxClients = stoi(setting["maxClients"]);
			if (serverProperty.maxClients <= 1)
			{
				throw std::runtime_error("?");
			}
		}
		catch (...)
		{
			throw runtime_error(std::string("Invaild maxClients: ") + setting["maxClients"]);
		}
	}
	else
	{
		serverProperty.maxClients = 1024;
	}
	
	if (setting.count("documentRoot") != 0)
	{
		serverProperty.documentRoot = webstring::strip(setting["documentRoot"], " \"\'");
	}
	else
	{
		serverProperty.documentRoot = "/var/www/html";
	}

	//对于路径末尾是否带斜线的处理。documentRoot末尾应缀上斜线
	if (serverProperty.documentRoot[serverProperty.documentRoot.length() - 1] != '/')
	{
		serverProperty.documentRoot += '/';
	}

	if (setting.count("verboseLevel") != 0)
	{
		try
		{
			switch (stoi(setting["verboseLevel"]))
			{
			case 3:
				serverProperty.verbose = VerboseLevel::full;
				break;
			case 2:
				serverProperty.verbose = VerboseLevel::essential;
				break;
			case 1:
				serverProperty.verbose = VerboseLevel::less;
				break;
			default:
				serverProperty.verbose = VerboseLevel::silence;
				break;
			}
		}
		catch (...)
		{
			throw runtime_error("Invaild verbose level.");
		}
	}
	else
	{
		serverProperty.verbose = VerboseLevel::silence;
	}
	
	if (setting.count("KeepAliveTimeout") != 0)
	{
		try
		{
			serverProperty.timeout = stoi(setting["KeepAliveTimeout"]);
		}
		catch (...)
		{
			throw runtime_error("Invaild timeout value.");
		}
	}
	else
	{
		serverProperty.timeout = 5;
	}

	if (setting.count("KeepAliveMaxRequestNum") != 0)
	{
		try
		{
			serverProperty.maxRequestsNum = stoi(setting["KeepAliveMaxRequestNum"]);
		}
		catch (...)
		{
			throw runtime_error("Invaild timeout value.");
		}
	}
	else
	{
		serverProperty.maxRequestsNum = 60;
	}

	return setting;
}

