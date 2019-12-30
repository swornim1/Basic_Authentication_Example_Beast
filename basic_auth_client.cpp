/*******************************************************************************
 * Author: Swornim Baral
 * 
 * Description: This is a client for testing the basic authentication mechanism 
 * 
 * References
 * 1) Vinnie Falco  HTTP_CLIENT_SYNC.CPP [Source code], https://www.boost.org/doc/libs/1_68_0/libs/beast/example/http/client/sync/http_client_sync.cpp
 * 2) modoran (2011) base64.h [Source code], http://www.cplusplus.com/forum/beginner/51572/
 * 3) modoran (2011) base64.cpp [Source code], http://www.cplusplus.com/forum/beginner/51572/
 **********************************************************************************/

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "base64.cpp"

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>


int main()
{
    
    auto const server_ip = "52.15.155.121";
    auto const auth_enabled_target =  "/basic_auth_test/index.html";
    auto const auth_disabled_target =  "/index.html";
    auto const port = "80";
    
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    tcp::socket socket{ioc};

    // Look up the domain name
    auto const results = resolver.resolve(server_ip, port);

    // Make the connection on the IP address
    net::connect(socket, results.begin(), results.end());
   
    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, auth_enabled_target, 11};
    req.set(http::field::host, server_ip);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);

    // This buffer is used for reading and must be persisted
    boost::beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(socket, buffer, res);
    
    //Retrieves the authentication type used in the response from server
    auto auth_type = res.operator[]("WWW-Authenticate");
    
    //If the target has no authentication setup the auth_type will be empty
    if(auth_type.empty())
    {
        //The client print the response if the server has no authentication setup
        std::cout << res << std::endl;
    }else
    {
        //the username and the password are set to test:test on the server
        std::string auth_cred = "test:test";
        std::string encoded_auth_cred = "Basic " + base64_encode((const unsigned char *)auth_cred.c_str(),auth_cred.length());
        
        req.set(http::field::authorization, encoded_auth_cred);
        
        http::write(socket, req);

        // This buffer is used for reading and must be persisted
        boost::beast::flat_buffer buffer_after_auth;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res_after_auth;

        http::read(socket, buffer_after_auth, res_after_auth);
        std::cout << res_after_auth << std::endl;
    }

    // Gracefully close the socket
    boost::system::error_code ec;
    socket.shutdown(tcp::socket::shutdown_both, ec);
    return 0;
}