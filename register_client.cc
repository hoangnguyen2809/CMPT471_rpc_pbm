#include "register_client.h"

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using register_service::GetRequest;
using register_service::ReadItemRequest;
using register_service::RegisterArray;
using register_service::Response;

RegisterServiceClient::RegisterServiceClient(std::shared_ptr<Channel> channel, std::ostream& out)
    : stub_(RegisterService::NewStub(channel)), out_(out) {}

void RegisterServiceClient::OutputRegisterArray(::google::protobuf::internal::RepeatedPtrIterator<RegisterArray> it) {
    out_ << it->name() << " " << it->capacity() << " " << it->size() << std::endl;
    // Task: output the index and value of every item to `out_`
    // Use the following format
    // out_ << "Index: " << index_var << ", Value: " << value_var << std::endl;
    for (int i = 0; i < it->items_size(); ++i) {
        uint32_t index = i;
        uint32_t value = it->items(i);
        out_ << "Index: " << index << ", Value: " << value << std::endl;
    }
}

Result* RegisterServiceClient::GetAllRegisterArrays() {
    ::google::protobuf::Empty request;
    Response response;
    ClientContext context;

    // Task: call the GetAllRegisterArrays RPC
    Status status = stub_->GetAllRegisterArrays(&context, request, &response);
    if (!status.ok()) {
        out_ << "[Error:" << status.error_code() << "] " << status.error_message() << std::endl;
        return new Result(status, response);
    }

    if (response.response_case() == Response::ResponseCase::RESPONSE_NOT_SET) {
        out_ << "[GET] Empty response" << std::endl;
        return new Result(status, response);
    }

    if (response.response_case() != Response::ResponseCase::kArrays) {
        out_ << "[Error] [GET] Invalid response" << std::endl;
        return new Result(status, response);
    }

    // Task: Output the register arrays to `_out`
    // Iterate over all returned arrays
    // call OutputRegisterArray
    for (auto array : response.arrays().arrays()) {
        ::google::protobuf::RepeatedPtrField<RegisterArray> tempArray;
        tempArray.Add()->CopyFrom(array);
        OutputRegisterArray(tempArray.begin());
    }

    return new Result(status, response);
}

Result* RegisterServiceClient::GetRegisterArray(std::string name) {
    GetRequest request;
    // Task: Set the name of the register array
    request.set_name(name);
    Response response;
    ClientContext context;
    
    // Task: call the GetRegisterArray RPC
    Status status = stub_->GetRegisterArray(&context, request, &response);
    if (!status.ok()) {
        out_ << "[Error:" << status.error_code() << "] " << status.error_message() << std::endl;
        return new Result(status, response);
    }

    if (response.response_case() != Response::ResponseCase::kArrays) {
        out_ << "[Error] [GET] Invalid response" << std::endl;
        return new Result(status, response);
    }

    // Task: Output the returned register array to `_out`
    // call OutputRegisterArray
    const RegisterArray& array = response.arrays().arrays(0);
    ::google::protobuf::RepeatedPtrField<RegisterArray> tempArray;
    tempArray.Add()->CopyFrom(array);
    OutputRegisterArray(tempArray.begin());

    return new Result(status, response);
}

Result* RegisterServiceClient::CreateRegisterArray(std::string name, uint32_t capacity) {
    CreateRequest request;
    // Task: Set the name and capacity of the register array
    request.set_name(name);
    request.set_capacity(capacity);
    Response response;
    ClientContext context;
    
    // Task: call the CreateRegisterArray RPC
    Status status = stub_->CreateRegisterArray(&context, request, &response);
    if (!status.ok()) {
        out_ << "[Error:" << status.error_code() << "] " << status.error_message() << std::endl;
        return new Result(status, response);
    }

    out_ << "[CREATE] OK" << std::endl;
    return new Result(status, response);
}

Result* RegisterServiceClient::DeleteRegisterArray(std::string name) {
    DeleteRequest request;
    // Task: Set the name of the register array
    request.set_name(name);
    Response response;
    ClientContext context;

    // Task: call the DeleteRegisterArray RPC
    Status status = stub_->DeleteRegisterArray(&context, request, &response);
    if (!status.ok()) {
        out_ << "[Error:" << status.error_code() << "] " << status.error_message() << std::endl;
        return new Result(status, response);
    }

    out_ << "[DELETE] OK" << std::endl;
    return new Result(status, response);
}

Result* RegisterServiceClient::ReadValue(std::string name, uint32_t index) {
    ReadItemRequest request;
    // Task: Set the name of the register array, and the index of the item
    request.set_name(name);
    request.set_index(index);
    Response response;
    ClientContext context;

    // Task: call the ReadValue RPC
    Status status = stub_->ReadValue(&context, request, &response);
    if (!status.ok()) {
        out_ << "[Error:" << status.error_code() << "] " << status.error_message() << std::endl;
        return new Result(status, response);
    }

    if (response.response_case() != Response::ResponseCase::kValue) {
        return new Result(status, response);
    }

    out_ << "[READ] Register: " << name << ", Index: " << index << ", Value: " << response.value() << std::endl;
    return new Result(status, response);
}

Result* RegisterServiceClient::WriteValue(std::string name, uint32_t index, uint32_t value) {
    WriteItemRequest request;
    // Task: Set the name of the register array, and the index and value of the item
    request.set_name(name);
    request.set_index(index);
    request.set_value(value);
    Response response;
    ClientContext context;

    // Task: call the WriteValue RPC
    Status status = stub_->WriteValue(&context, request, &response);
    if (!status.ok()) {
        out_ << "[Error:" << status.error_code() << "] " << status.error_message() << std::endl;
        return new Result(status, response);
    }

    out_ << "[WRITE] Register: " << name << ", Index: " << index << ", Value: " << value << std::endl;
    return new Result(status, response);
}
