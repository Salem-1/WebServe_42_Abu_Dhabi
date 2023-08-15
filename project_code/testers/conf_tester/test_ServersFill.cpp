#include "../../includes/libs.hpp"
#include "../../includes/webserve.hpp"
#include "tester.hpp"

tokenized_conf    dummy_conf_tokens()
{
    tokenized_conf tokenized_server;

    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    locations.push_back("location / {\nlimit_except GET {\n    deny all;\n}\ntry $uri $uri/ youpi.bad_extension;");
    locations.push_back("location /put_test/ {\nlimit_except PUT {\n    deny all;\n}\n alias /intra/YoupiBanane/PUT/;");
    locations.push_back("location .bla {\n        include fastcgi_params;\nfastcgi_pass unix:/path/to/your/cgi_test_socket.sock;\n fastcgi_param SCRIPT_FILENAME /path/to/your/cgi_test_executable;");
    locations.push_back("location /post_body {\nlimit_except POST {\n    deny all;\n}\n client_max_body_size 100m;");
    locations.push_back("location /directory/\nalias /intra/YoupiBanane/;\nindex youpi.bad_extension;\ntry_files $uri $uri/ /youpi.bad_extension;\nautoindex on;\ntry 404 /erros/error1.html;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    return (tokenized_server);
}

void    test_listenConf(ServerFill &fill)
{
    conf    servers = fill.servers.servers;
    if (servers.empty())
        std::cout << BOLDRED << "Empty server configuration test failed 😱" << RESET << std::endl;
    else
        std::cout << BOLDRED << "Empty server configuration test failed 😱" << RESET << std::endl;
    
    fill._conf_tokens[0].first = "listen sdffgchvjbk;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    negative_essential_try_catch(fill, "string port ");
    fill._conf_tokens[0].first = "listen 2345678;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    negative_essential_try_catch(fill, "large port number ");
    fill._conf_tokens[0].first = "listen 2345 3333 ;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    positive_essential_try_catch(fill, "large port number ");
    if (!inMap(servers[0], "listen"))
        std::cout << BOLDRED << "listen found in server test failed 😱" << RESET << std::endl;
    else
        std::cout << BOLDGREEN << "listen found in server test passed 😀" << RESET <<std::endl;
}

void visualize_tokens(tokenized_conf &tokens)
{
    for (tokenized_conf::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::cout << BOLDYELLOW << "essentials: " << RESET << std::endl;
        std::cout << BOLDMAGENTA << it->first << RESET << std::endl;
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << BOLDYELLOW << "Location " << i << RESET << std::endl;
            std::cout << it->second[i] << RESET << std::endl;
        }
    }
}

void    test_emptyEssentials(ServerFill &fill)
{
    fill._conf_tokens[0].first = "";

    try
    {
        fill.parseEssentials();
        std::cout << BOLDRED << "Test empty Essentials failed 😱" << RESET << std::endl;

    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
        std::cout << BOLDGREEN << "empty essentials test passed 😀" << RESET <<std::endl;
    }
    fill._conf_tokens[0].first = "listen 3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
 
    positive_essential_try_catch(fill, "non empty");    
}

void    test_mixSpacesEssentials(ServerFill &fill)
{
    fill._conf_tokens[0].first = "lis\nten             3490;\nserver\n_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    negative_essential_try_catch(fill, "bad new live");
    fill._conf_tokens[0].first = "listen\t3490;\nserver_name 127.0.0.1\tlocal_host;\nroot\tintra/YoupiBanane;\nindex\tyoupi.bad_extension;\nclient_max_body_size default;";
    positive_essential_try_catch(fill, "horizontal tab");
    fill._conf_tokens[0].first = "listen\v3490;\nserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    positive_essential_try_catch(fill, "vertical tab");
    fill._conf_tokens[0].first = "listen\t\v\t\t3490;\t\t\t\t\vserver_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    positive_essential_try_catch(fill, "mix tab and spaces tab");
    fill._conf_tokens[0].first = "listen 3490;server_name 127.0.0.1 local_host;root intra/YoupiBanane;\nindex youpi.bad_extension; client_max_body_size default;";
    positive_essential_try_catch(fill, "mix tab and spaces tab");
}

void   test_essentialOccurance(ServerFill &fill)
{
    fill._conf_tokens[0].first = "server_name 127.0.0.1 local_host;\nroot intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    negative_essential_try_catch(fill, "missed listen");
    fill._conf_tokens[0].first = "listen     3490;root intra/YoupiBanane;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    negative_essential_try_catch(fill, "missed servername");
    fill._conf_tokens[0].first = "server_name 127.0.0.1 local_host;\nindex youpi.bad_extension;\nclient_max_body_size default;";
    negative_essential_try_catch(fill, "missed root");
}

void    test_lenEssentials(ServerFill &fill)
{
    fill._conf_tokens[0].first = "listen ";
    negative_essential_try_catch(fill, "Small string");
    fill._conf_tokens[0].first = "listen 3490\nserver name;";
    negative_essential_try_catch(fill, "Small string");
    fill._conf_tokens[0].first = "listen 3490\nserver_name localhost;root /";
    negative_essential_try_catch(fill, "missed semi colon");
    fill._conf_tokens[0].first = "listenn 3490;\nserver_name localhost;root /";
    negative_essential_try_catch(fill, "bad listen word");
    fill._conf_tokens[0].first = "listen 3490;\nserve_name localhost;root /";
    negative_essential_try_catch(fill, "bad server word");
    fill._conf_tokens[0].first = "listen 3490;\nserve_name localhost;root /";
    negative_essential_try_catch(fill, "bad server root");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name localhost;root /; indexx shsj.html";
    negative_essential_try_catch(fill, "bad index root");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name localhost;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad index root");
    fill._conf_tokens[0].first = "listen ;\nserver_name localhost;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name ;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_name localhost;root / asdsad saDSA; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_namec another localhost;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    fill._conf_tokens[0].first = "listen 3490;\nserver_namec another localhost;listen 300;root /; index shsj.html; client_body_size 1000";
    negative_essential_try_catch(fill, "bad argument num");
    // positive_essential_try_catch(fill, "bad word string");
}

void    negative_essential_try_catch(ServerFill &fill, std::string TestCase)
{
    try
    {
        fill.parseEssentials();
        std::cout << BOLDRED << TestCase << " failed 😱" << RESET << std::endl;

    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
        std::cout << BOLDGREEN << TestCase << " test passed 😀" << RESET <<std::endl;
    }
}

void    positive_essential_try_catch(ServerFill &fill, std::string TestCase)
{
    try
    {
        fill.parseEssentials();
        std::cout << BOLDGREEN << TestCase << " test passed 😀" << RESET <<std::endl;

    }
    catch(const std::exception& e)
    {
        std::cout << BOLDRED << TestCase << " failed 😱" << RESET << std::endl;
        std::cerr << e.what() << '\n';
    }
}