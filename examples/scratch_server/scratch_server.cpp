/**
 * This example is presently used as a scratch space. It may or may not be broken
 * at any given time.
 */

#include <iostream>

#include <websocketpp/config/debug_asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <websocketpp/extensions/permessage_deflate/enabled.hpp>

struct deflate_config : public websocketpp::config::debug_core {
    using type = deflate_config;
    using base = debug_core;
    
    using concurrency_type = base::concurrency_type;
    
    using request_type = base::request_type;
    using response_type = base::response_type;

    using message_type = base::message_type;
    using con_msg_manager_type = base::con_msg_manager_type;
    using endpoint_msg_manager_type = base::endpoint_msg_manager_type;
    
    using alog_type = base::alog_type;
    using elog_type = base::elog_type;
    
    using rng_type = base::rng_type;
    
    struct transport_config : public base::transport_config {
        using concurrency_type = type::concurrency_type;
        using alog_type = type::alog_type;
        using elog_type = type::elog_type;
        using request_type = type::request_type;
        using response_type = type::response_type;
        typedef websocketpp::transport::asio::basic_socket::endpoint 
            socket_type;
    };

    typedef websocketpp::transport::asio::endpoint<transport_config> 
        transport_type;
        
    /// permessage_compress extension
    struct permessage_deflate_config {};

    typedef websocketpp::extensions::permessage_deflate::enabled
        <permessage_deflate_config> permessage_deflate_type;
};

using server = websocketpp::server<deflate_config>;

using message_ptr = server::message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    /*std::cout << "on_message called with hdl: " << hdl.lock().get() 
              << " and message (" << msg->get_payload().size() << "): " << msg->get_payload()
              << std::endl;
    */
    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        std::cout << "Echo failed because: " 
                  << "(" << e.what() << ")" << std::endl;
    }
}

int main(int argc, char * argv[]) {
	// Create a server endpoint
    server echo_server;
	
	try {
        // Set logging settings        
        if (argc > 1 && std::string(argv[1]) == "-d") {
            echo_server.set_access_channels(websocketpp::log::alevel::all);
            echo_server.set_error_channels(websocketpp::log::elevel::all);
        } else {
            echo_server.set_access_channels(websocketpp::log::alevel::none);
            echo_server.set_error_channels(websocketpp::log::elevel::none);
        }
        
        // Initialize ASIO
        echo_server.init_asio();
        
        // Register our message handler
        using websocketpp::lib::placeholders::_1;
        using websocketpp::lib::placeholders::_2;
        echo_server.set_message_handler(bind(&on_message,&echo_server,_1,_2));
        
        // Listen on port 9002
        echo_server.listen(9002);
        
        // Start the server accept loop
        echo_server.start_accept();
	    
	    // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
