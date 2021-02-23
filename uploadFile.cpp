

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <azure/core/context.hpp>
#include <azure/core/http/body_stream.hpp>
#include <azure/core/http/curl/curl.hpp>
#include <azure/storage/blobs.hpp>

using namespace Azure::Storage::Blobs;
using namespace std::literals;

int main(int argc, char *argv[]) {
  std::string containerName = "testcontainer";
  std::string blobName = "sample-large-blob";

  std::vector<uint8_t> large_blob(1024 * 1024 * 1024);
  std::fill(large_blob.begin(), large_blob.end(), 5);
  Azure::Core::Http::MemoryBodyStream ms(large_blob);
  try {
    BlobClientOptions options;
    options.TransportPolicyOptions.Transport =
        std::make_shared<Azure::Core::Http::CurlTransport>();
    auto client = BlobContainerClient::CreateFromConnectionString(
        std::getenv("AZURE_STORAGE_CONNECTION_STRING"), containerName, options);
    client.CreateIfNotExists();
    auto block = client.GetBlobClient(blobName).AsBlockBlobClient();
    auto response = block.Upload(&ms);

    auto status = response.GetRawResponse().GetStatusCode();
    printf("Response status code: %d\n", status);

    auto model = response.ExtractValue();
    printf(
        "Last modified date of file: %s\n",
        model.LastModified.GetString(Azure::Core::DateTime::DateFormat::Rfc3339)
            .c_str());
    return 0;
  } catch (Azure::Storage::StorageException &e) {
    // Optional exception handling, in case something goes wrong.
    puts(e.Message.c_str());
    return 1;
  } catch (Azure::Core::Http::TransportException &te) {
    puts(te.what());
    return 1;
  }

  return 0;
}
