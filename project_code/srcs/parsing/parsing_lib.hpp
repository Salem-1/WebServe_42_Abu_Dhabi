

#ifndef PARSING_LIB
# define PARSING_LIB
# include <iostream>
# include <sstream>
# include <vector>
# include <map>


typedef std::map<std::string, std::vector<std::string> >    packet_map;
typedef std::map<std::string, std::vector<std::string> >    response_packet;
typedef std::vector<std::map<std::string, std::string> >    conf; 
std::vector<std::string> split(std::string str, std::string delimeter);
int                     fill_status_code(
                            response_packet &response, std::string status_code, std::string message);
void                    fill_path(
                            packet_map &request,response_packet &response, std::string method);
#endif