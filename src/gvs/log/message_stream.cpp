// ///////////////////////////////////////////////////////////////////////////////////////
// Geometry Visualization Server
// Copyright (c) 2018 Logan Barnes - All Rights Reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ///////////////////////////////////////////////////////////////////////////////////////
#include "message_stream.hpp"
#include <grpc++/client_context.h>

namespace gvs {
namespace log {

MessageStream::MessageStream(const std::string& server_address, const std::string& identifier)
    : MessageStream(server_address, std::chrono::seconds(4), identifier) {}

bool MessageStream::connected() const {
    return stub_ != nullptr;
}

void MessageStream::send() {
    if (stub_) {
        message_.set_contents(content_stream_.str());
        content_stream_.str("");

        grpc::ClientContext context;
        proto::Errors errors;
        grpc::Status status = stub_->SendMessage(&context, message_, &errors);

        if (not status.ok()) {
            std::cerr << "Error sending message: " << status.error_message() << std::endl;

        } else if (not errors.error_msg().empty()) {
            std::cerr << "Error sending message: " << errors.error_msg() << std::endl;
        }
    }
}

MessageStream& MessageStream::operator<<(MessageStream& (*func)(MessageStream&)) {
    if (stub_) {
        return func(*this);
    }
    return *this;
}

} // namespace log
} // namespace gvs
