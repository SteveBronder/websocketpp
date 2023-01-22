/*
 * Copyright (c) 2014, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef WEBSOCKETPP_CONFIG_CORE_HPP
#define WEBSOCKETPP_CONFIG_CORE_HPP

// Non-Policy common stuff
#include <websocketpp/common/cpp11.hpp>
#include <websocketpp/common/platforms.hpp>
#include <websocketpp/common/stdint.hpp>

// Concurrency
#include <websocketpp/concurrency/basic.hpp>

// Transport
#include <websocketpp/transport/iostream/endpoint.hpp>

// HTTP
#include <websocketpp/http/request.hpp>
#include <websocketpp/http/response.hpp>

// Messages
#include <websocketpp/message_buffer/alloc.hpp>
#include <websocketpp/message_buffer/message.hpp>

// Loggers
#include <websocketpp/logger/basic.hpp>
#include <websocketpp/logger/levels.hpp>

// RNG
#include <websocketpp/random/none.hpp>

// User stub base classes
#include <websocketpp/connection_base.hpp>
#include <websocketpp/endpoint_base.hpp>

// Extensions
#include <websocketpp/extensions/permessage_deflate/disabled.hpp>

namespace websocketpp {
namespace config {

/// Server config with iostream transport
struct core {
  using type = core;

  // Concurrency policy
  using concurrency_type = websocketpp::concurrency::basic;

  // HTTP Parser Policies
  using request_type = http::parser::request;
  using response_type = http::parser::response;

  // Message Policies
  using message_type = message_buffer::message<message_buffer::alloc::con_msg_manager>;
  using con_msg_manager_type = message_buffer::alloc::con_msg_manager<message_type>;
  using endpoint_msg_manager_type = message_buffer::alloc::endpoint_msg_manager<con_msg_manager_type>;

  /// Logging policies
  using elog_type = websocketpp::log::basic<concurrency_type, websocketpp::log::elevel>;
  using alog_type = websocketpp::log::basic<concurrency_type, websocketpp::log::alevel>;

  /// RNG policies
  using rng_type = websocketpp::random::none::int_generator<uint32_t>;

  /// Controls compile time enabling/disabling of thread syncronization
  /// code Disabling can provide a minor performance improvement to single
  /// threaded applications
  static constexpr bool const enable_multithreading = true;

  struct transport_config {
    using concurrency_type = core::concurrency_type;
    using elog_type = core::elog_type;
    using alog_type = core::alog_type;
    using request_type = core::request_type;
    using response_type = core::response_type;

    /// Controls compile time enabling/disabling of thread syncronization
    /// code Disabling can provide a minor performance improvement to single
    /// threaded applications
    static constexpr bool const enable_multithreading = true;

    /// Default timer values (in ms)

    /// Length of time to wait for socket pre-initialization
    /**
     * Exactly what this includes depends on the socket policy in use
     */
    static constexpr const long timeout_socket_pre_init = 5000;

    /// Length of time to wait before a proxy handshake is aborted
    static constexpr const long timeout_proxy = 5000;

    /// Length of time to wait for socket post-initialization
    /**
     * Exactly what this includes depends on the socket policy in use.
     * Often this means the TLS handshake
     */
    static constexpr const long timeout_socket_post_init = 5000;

    /// Length of time to wait for dns resolution
    static constexpr const long timeout_dns_resolve = 5000;

    /// Length of time to wait for TCP connect
    static constexpr const long timeout_connect = 5000;

    /// Length of time to wait for socket shutdown
    static constexpr const long timeout_socket_shutdown = 5000;
  };

  /// Transport Endpoint Component
  typedef websocketpp::transport::iostream::endpoint<transport_config>
      transport_type;

  /// User overridable Endpoint base class
  using endpoint_base = websocketpp::endpoint_base;
  /// User overridable Connection base class
  using connection_base = websocketpp::connection_base;

  /// Default timer values (in ms)

  /// Length of time before an opening handshake is aborted
  static constexpr const long timeout_open_handshake = 5000;
  /// Length of time before a closing handshake is aborted
  static constexpr const long timeout_close_handshake = 5000;
  /// Length of time to wait for a pong after a ping
  static constexpr const long timeout_pong = 5000;

  /// WebSocket Protocol version to use as a client
  /**
   * What version of the WebSocket Protocol to use for outgoing client
   * connections. Setting this to a value other than 13 (RFC6455) is not
   * recommended.
   */
  static constexpr const int client_version = 13; // RFC6455

  /// Default static error logging channels
  /**
   * Which error logging channels to enable at compile time. Channels not
   * enabled here will be unable to be selected by programs using the library.
   * This option gives an optimizing compiler the ability to remove entirely
   * code to test whether or not to print out log messages on a certain
   * channel
   *
   * Default is all except for development/debug level errors
   */
  static constexpr const websocketpp::log::level elog_level =
      websocketpp::log::elevel::all ^ websocketpp::log::elevel::devel;

  /// Default static access logging channels
  /**
   * Which access logging channels to enable at compile time. Channels not
   * enabled here will be unable to be selected by programs using the library.
   * This option gives an optimizing compiler the ability to remove entirely
   * code to test whether or not to print out log messages on a certain
   * channel
   *
   * Default is all except for development/debug level access messages
   */
  static constexpr const websocketpp::log::level alog_level =
      websocketpp::log::alevel::all ^ websocketpp::log::alevel::devel;

  /// Size of the per-connection read buffer
  /**
   * Each connection has an internal buffer of this size. A larger value will
   * result in fewer trips through the library and less CPU overhead at the
   * expense of increased memory usage per connection.
   *
   * If your application primarily deals in very large messages you may want
   * to try setting this value higher.
   *
   * If your application has a lot of connections or primarily deals in small
   * messages you may want to try setting this smaller.
   */
  static constexpr const size_t connection_read_buffer_size = 16384;

  /// Drop connections immediately on protocol error.
  /**
   * Drop connections on protocol error rather than sending a close frame.
   * Off by default. This may result in legit messages near the error being
   * dropped as well. It may free up resources otherwise spent dealing with
   * misbehaving clients.
   */
  static constexpr const bool drop_on_protocol_error = false;

  /// Suppresses the return of detailed connection close information
  /**
   * Silence close suppresses the return of detailed connection close
   * information during the closing handshake. This information is useful
   * for debugging and presenting useful errors to end users but may be
   * undesirable for security reasons in some production environments.
   * Close reasons could be used by an attacker to confirm that the endpoint
   * is out of resources or be used to identify the WebSocket implementation
   * in use.
   *
   * Note: this will suppress *all* close codes, including those explicitly
   * sent by local applications.
   */
  static constexpr const bool silent_close = false;

  /// Default maximum message size
  /**
   * Default value for the processor's maximum message size. Maximum message
   * size determines the point at which the library will fail a connection with
   * the message_too_big protocol error.
   *
   * The default is 32MB
   *
   * @since 0.3.0
   */
  static constexpr const size_t max_message_size = 32000000;

  /// Default maximum http body size
  /**
   * Default value for the http parser's maximum body size. Maximum body size
   * determines the point at which the library will abort reading an HTTP
   * connection with the 413/request entity too large error.
   *
   * The default is 32MB
   *
   * @since 0.5.0
   */
  static constexpr const size_t max_http_body_size = 32000000;

  /// Global flag for enabling/disabling extensions
  static constexpr const bool enable_extensions = true;

  /// Extension specific settings:

  /// permessage_compress extension
  struct permessage_deflate_config {
    using request_type = core::request_type;

    /// If the remote endpoint requests that we reset the compression
    /// context after each message should we honor the request?
    static constexpr const bool allow_disabling_context_takeover = true;

    /// If the remote endpoint requests that we reduce the size of the
    /// LZ77 sliding window size this is the lowest value that will be
    /// allowed. Values range from 8 to 15. A value of 8 means we will
    /// allow any possible window size. A value of 15 means do not allow
    /// negotiation of the window size (ie require the default).
    static constexpr const uint8_t minimum_outgoing_window_bits = 8;
  };

  typedef websocketpp::extensions::permessage_deflate::disabled<
      permessage_deflate_config>
      permessage_deflate_type;

  /// Autonegotiate permessage-deflate
  /**
   * Automatically enables the permessage-deflate extension.
   *
   * For clients this results in a permessage-deflate extension request being
   * sent with every request rather than requiring it to be requested manually
   *
   * For servers this results in accepting the first set of extension settings
   * requested by the client that we understand being used. The alternative is
   * requiring the extension to be manually negotiated in `validate`. With
   * auto-negotiate on, you may still override the auto-negotiate manually if
   * needed.
   */
  // static const bool autonegotiate_compression = false;
};

} // namespace config
} // namespace websocketpp

#endif // WEBSOCKETPP_CONFIG_CORE_HPP
