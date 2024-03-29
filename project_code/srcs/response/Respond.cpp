

#include "Respond.hpp"

Respond::Respond()
{};

Respond::Respond(int client_socket) :client_socket(client_socket),  sending(false), response_bytes_sent(0)
{
}

Respond::~Respond()
{
}

void    Respond::respond(packet_map &request, t_request &full_request,  conf &servers, std::string port)
{
    
    //here should extract the port and hostname to give to the corresponding method
    stringmap server_info = getServerInfo(request, servers, port);
    fillResponse(request, full_request, server_info);
    sending = true;
    
    std::cout << "We have ready response" << std::endl;
}

void    Respond::flushResponse()
{
    response.clear();
    response_string = "";
    response_bytes_sent = 0;
    sending = false;

}

void    Respond::fillResponse(packet_map &request, t_request &full_request, stringmap &server_info)
{
    if ((response.find("Content-Length:") != response.end() && response.find("Transfer-Encoding:") != response.end())
        || checkPoisonedURL(request))
    {
        fillStatuCode("400", "request method not supported");
        return ;
    }
    response["Status-code"].push_back("200");
	std::string cgi_path = isCGI(server_info, request);    
	std::set<std::string> supported_methods;

	if (request.find("GET") != request.end()
        && isSupportedMethod("GET", supported_methods))
    {
        fillSupportedMethods(supported_methods, server_info, "GET" ,  request);
        if(!(isSupportedMethod("GET", supported_methods)))
        {
            response_string = err.code(server_info, "405");
            return ;
        }   
		if (bodyTooBig(response, server_info, full_request.body) == true)
			response_string = err.code(server_info, "413");
	    else if (cgi_path != "")
			response_string = getExecute(request, full_request, server_info, cgi_path);
		else
        	response_string = normalGETResponse(request, server_info);
    }
    else if (request.find("POST") != request.end())
    {
        fillSupportedMethods(supported_methods, server_info, "POST" ,  request);
        if(!(isSupportedMethod("POST", supported_methods)))
                 {
            response_string = err.code(server_info, "405");
            return ;
        } 
		if (bodyTooBig(response, server_info, full_request.body) == true)
			response_string = err.code(server_info, "413");
        else if (cgi_path != "")
			response_string = postExecute(request, full_request, server_info, cgi_path);
		else
		{
			Post apost(request, full_request, server_info, response);
			apost.handlePost();
			response_string = apost.get_response();
		}
    }
    else if (request.find("PUT") != request.end())
    {
        fillSupportedMethods(supported_methods, server_info, "PUT" ,  request);
        if(!(isSupportedMethod("PUT", supported_methods)))
                 {
            response_string = err.code(server_info, "405");
            return ;
        } 
        PUT put(request, full_request, server_info, response);
		put.handlePUT();
		response_string = put._response;
    }
    else if (request.find("DELETE") != request.end())
    {
        fillSupportedMethods(supported_methods, server_info, "DELETE" ,  request);
        if(!(isSupportedMethod("DELETE", supported_methods)))
                 {
            response_string = err.code(server_info, "405");
            return ;
        } 
            response_string = err.code(server_info, "405");
        DELETE DELETE_response;
        response_string = DELETE_response.deleteResponseFiller(request, response, server_info);
    }
    else
	{
		response_string = err.code(server_info, "405");
	}
};

std::string     Respond::normalGETResponse(packet_map &request, stringmap &server_info)
{
    GET GET_fill(request, response);
    GET_fill.prepareGetResponse(server_info);
    return (GET_response(GET_fill.response).fillGetResponse(server_info));
}

int Respond::checkPoisonedURL(packet_map &request)
{
    if (request.find("GET") != request.end())
    {
		if (request["GET"].size() != 2)
			return (0);
        if (request["GET"][0].find(" ") != std::string::npos)
            return (1);
    }
    if (request.find("POST") != request.end())
    {
        if (request["POST"][0].find(" ") != std::string::npos)
             return (1);
    }
    if (request.find("DELETE") != request.end())
    {
        if (request["DELETE"][0].find(" ") != std::string::npos)
          return (1);
    }
    return (0);
}


void    Respond::sendAll(connection_state &state)
{
    size_t  packet_len = response_string.length(); 
    const char *a = response_string.c_str();
    int send_ret = 0;

    if (packet_len - response_bytes_sent > SEND_BUFFER_SIZE)
        send_ret = send(client_socket, &a[response_bytes_sent], SEND_BUFFER_SIZE, 0);
    else
        send_ret = send(client_socket, &a[response_bytes_sent], packet_len - response_bytes_sent, 0);  
    std::cout << send_ret << " bytes sent , packet len = " << packet_len << "\n";
    response_bytes_sent += send_ret;
    if (send_ret <= 0 || response_bytes_sent > packet_len)
    {
    	vis_str(response_string, "inside send all\n");
        state = KILL_CONNECTION;
        flushResponse();
		response_string.clear();
    }
}

int Respond::fillStatuCode(std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}

stringmap  Respond::getServerInfo(packet_map &request,conf &servers, std::string port)
{

    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers[i]["Port"] == port)
            nominated_servers.push_back(i);
    }
    for (unsigned long i = 0; i < nominated_servers.size(); i++)
    {
        server_names =  split(servers[nominated_servers[i]]["server_name"], " ");
        for (unsigned long j = 0; j < server_names.size(); j++)
        {   
            if (request.find("Host:") == request.end() || request["Host:"].size() != 1)  
            {
                fillStatuCode("400", "No host");   
                return (servers[nominated_servers[0]]);
            } 
            if (server_names[j] == fillRequestedHostName(request, port))
            {
                return (servers[nominated_servers[i]]);
            }
        }
    }
    return (servers[nominated_servers[0]]);
}

std::string     Respond::fillRequestedHostName(packet_map &request, std::string &port)
{
    std::string requested_host;
    if (request["Host:"][0].find(":") == std::string::npos || request["Host:"][0].find(":") == request["Host:"][0].length())
        requested_host  = request["Host:"][0];
    else
        requested_host  = request["Host:"][0].substr(0, request["Host:"][0].find(":"));
    std::pair<std::string, std::string> host_port(requested_host, port); 
    return (requested_host);
}

bool    Respond::isSupportedMethod(std::string method, std::set<std::string> &supported_methods)
{
    if (supported_methods.empty() == true || supported_methods.find(method) != supported_methods.end())
		return (true);
	return (false);
}

void   Respond::fillSupportedMethods(
                    std::set<std::string> &supported_methods, stringmap &server_info
                    , std::string method, packet_map& request)
{
    supported_methods.clear();
    fillPath(request, response, method);
    if (response["Status-code"][0] != "200")
        return ;
    std::string path = response["Path"][1];

    
    std::string dir;
    if (path == "/")
        dir = path;
    else if (std::count(path.begin(), path.end(), '/') > 1)
        dir = path.substr(0, path.find("/", path.find("/") + 1));
    else if (path.find(".") != std::string::npos)
        dir = "/";
    else if (path.find("?") == std::string::npos)
        dir = path;
    else if (path.find("?") != 0)
        dir = path.substr(0, path.find("?") - 1);
	else
		dir = path;
    response["dir"].clear();  
    response["dir"].push_back(dir);
    server_info["constructed path dir"] = dir;
    std::string allowed_methods = dir + " methods";
    if (server_info.find(allowed_methods) != server_info.end())
    {
        if (server_info[allowed_methods].empty())
            supported_methods.insert("nothing");
        else
            supported_methods = split_to_set(server_info[allowed_methods], " ");
        return ;
    }
    supported_methods.insert("GET");
    supported_methods.insert("POST");
    supported_methods.insert("DELETE");
    supported_methods.insert("PUT");
}
