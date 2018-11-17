// ///////////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2018 Logan Barnes - All Rights Reserved
//  Geometry Visualisation Server
// ///////////////////////////////////////////////////////////////////////////////////////
#include "test-util/scene_server.hpp"
#include "test-util/test_service.hpp"
#include "scene_server.hpp"

namespace gvs {
namespace test {

TestServer::TestServer(std::string server_address)
    : server_(
          std::make_unique<gvs::net::GrpcServer>(std::make_shared<gvs::test::TestService>(), std::move(server_address)))
    , run_thread_([&] { server_->run(); }) {}

TestServer::~TestServer() {
    server_->shutdown();
    run_thread_.join();
}

std::shared_ptr<grpc::Channel> TestServer::inprocess_channel() {
    grpc::ChannelArguments channel_arguments;
    return server_->server()->InProcessChannel(channel_arguments);
}

} // namespace test
} // namespace gvs
