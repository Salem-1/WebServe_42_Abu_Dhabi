#ifndef TESTER_HPP
# define TESTER_HPP
#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"

tokenized_conf    dummy_conf_tokens();
void visualize_tokens(tokenized_conf &tokens);
void    test_emptyEssentials(ServerFill &fill);
void    negative_essential_try_catch(
        ServerFill &fill, std::string TestCase);
void    test_lenEssentials(ServerFill &fill);
void    positive_essential_try_catch(ServerFill &fill, std::string TestCase);
void    test_mixSpacesEssentials(ServerFill &fill);
void   test_essentialOccurance(ServerFill &fill);
void    test_listenConf(ServerFill &fill);
#endif