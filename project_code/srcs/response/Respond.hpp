/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:50 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/24 16:46:46 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET_response.hpp"
# include "../GET/GET.hpp"
# include "../DELETE/DELETE.hpp"

class Respond
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_pack;

public:
    Respond();
    Respond(int client_socket);
    ~Respond();
    void    respond(packet_map &request,  std::map<std::string, std::string> &server_info);
    int     client_socket;
    void    fill_response(packet_map &request,  std::map<std::string, std::string> &server_info);
    void    visualize_response();
    int     fill_status_code(std::string status_code, std::string message);
    void    send_all();
    void    flush_response();
    response_pack   response;
    std::string     response_packet;
private:
    int     check_poisoned_url(packet_map &request);
    /* data */

};

#endif