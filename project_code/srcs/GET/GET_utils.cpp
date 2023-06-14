#include "GET.hpp"

void GET::fill_request_line()
{
    if ((request["GET"][0].length() + request["GET"][1].length()) > HEADER_MAX_LENGTH
            && fill_status_code("414", "URI Too Long"))
        return ;
    if (!(request["GET"][1] == "HTTP/1.1")
        && (fill_status_code("505", "version not supported")))
        return ;
    if ((request["GET"].size() > 3) 
        && (fill_status_code("400", "get vec has more than 3 items bad request")))
        return ;
    else
        response["HTTP version"].push_back(request["GET"][1]);
    response["method"].push_back("GET");
    fill_path();
}

void    GET::fill_path()
{
    //decide on absouloute or other option path 
    //400 if wrong
    if (request["GET"][0].find("?") == std::string::npos)
    {
        response["Path"].push_back("absolute");
        response["Path"].push_back(request["GET"][0]);
    }
    else
    {
        if ((request["GET"][0][0] !=  '/' || request["GET"][0].find("&&") != std::string::npos)
            && (fill_status_code("400", "bad origin path format")))
                return ;
        std::string base_path = request["GET"][0].substr(0, request["GET"][0].find("?"));
        response["Path"].push_back("origin");
        response["Path"].push_back(base_path);
        std::vector<std::string> tmp_origin = split(request["GET"][0], "?");
        tmp_origin.erase(tmp_origin.begin());
        tmp_origin = split(*(tmp_origin.begin()), "&");
        for (std::vector<std::string>::iterator it = tmp_origin.begin(); it != tmp_origin.end(); ++it)
        {
            response["Path"].push_back(*it);
        }
    }
}

void    GET::visualize_response()
{
    std::cout << "\nVisualizing reponse API\n" << std::endl;
    std::cout << "{" << std::endl;
    for (response_packet::iterator it = response.begin(); it != response.end(); it++)
    {
        std::cout << "  \"" << it->first << "\": [";

         for (std::vector<std::string>::iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            std::cout << "\"" << *vit << "\", ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
}

int GET::fill_status_code(std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}