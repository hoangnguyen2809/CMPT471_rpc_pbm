#include <grpcpp/grpcpp.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "result.h"
#include "register_client.h"

int main(int argc, char** argv) {
    std::string address("localhost:50051");
    RegisterServiceClient client(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()),
                                 std::cout);
    // This is a sample client
    client.CreateRegisterArray("test", 16);
    client.CreateRegisterArray("test", 16);
    client.CreateRegisterArray("test1", 32);
    client.ReadValue("test", 0);
    client.ReadValue("test", 1);
    client.ReadValue("test1", 1);
    client.ReadValue("test1", 55);

    Result* r1 = client.GetRegisterArray("test");

    client.WriteValue("test", 0, 100);
    client.WriteValue("test", 0, 200);
    client.WriteValue("test", 1, 2);
    client.WriteValue("test", 1, 3);
    client.WriteValue("test1", 0, 3);
    client.WriteValue("test1", 1, 333);
    client.WriteValue("test1", 2, 355);
    client.WriteValue("test1", 18, 366);
    client.WriteValue("test1", 32, 388);
    client.WriteValue("test", 100, 3);

    client.ReadValue("test", 0);
    client.ReadValue("test", 1);
    client.GetAllRegisterArrays();
    Result* r2 = client.GetRegisterArray("test1");
    client.DeleteRegisterArray("test1");
    client.GetRegisterArray("test1");
    client.GetAllRegisterArrays();
    client.DeleteRegisterArray("test");

    return 0;
}
