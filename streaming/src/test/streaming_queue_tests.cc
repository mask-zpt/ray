#define BOOST_BIND_NO_PLACEHOLDERS
#include <unistd.h>
#include "gtest/gtest.h"
#include "queue/queue_client.h"
#include "ray/common/test_util.h"
#include "ray/core_worker/core_worker.h"

#include "data_reader.h"
#include "data_writer.h"
#include "message/message.h"
#include "message/message_bundle.h"
#include "ring_buffer.h"

#include "queue_tests_base.h"

using namespace std::placeholders;
namespace ray {
namespace streaming {

static int node_manager_port;

class StreamingWriterTest : public StreamingQueueTestBase {
 public:
  StreamingWriterTest() : StreamingQueueTestBase(1, node_manager_port) {}
};

class StreamingExactlySameTest : public StreamingQueueTestBase {
 public:
  StreamingExactlySameTest() : StreamingQueueTestBase(1, node_manager_port) {}
};

TEST_P(StreamingWriterTest, streaming_writer_exactly_once_test) {
  STREAMING_LOG(INFO) << "StreamingWriterTest.streaming_writer_exactly_once_test";

  uint32_t queue_num = 1;

  STREAMING_LOG(INFO) << "Streaming Strategy => EXACTLY ONCE";
  SubmitTest(queue_num, "StreamingWriterTest", "streaming_writer_exactly_once_test",
             60 * 1000);
}

INSTANTIATE_TEST_CASE_P(StreamingTest, StreamingWriterTest, testing::Values(0));

INSTANTIATE_TEST_CASE_P(StreamingTest, StreamingExactlySameTest,
                        testing::Values(0, 1, 5, 9));

}  // namespace streaming
}  // namespace ray

int main(int argc, char **argv) {
  // set_streaming_log_config("streaming_writer_test", StreamingLogLevel::INFO, 0);
  ::testing::InitGoogleTest(&argc, argv);
  RAY_CHECK(argc == 9);
  ray::TEST_STORE_EXEC_PATH = std::string(argv[1]);
  ray::TEST_RAYLET_EXEC_PATH = std::string(argv[2]);
  ray::streaming::node_manager_port = std::stoi(std::string(argv[3]));
  ray::TEST_MOCK_WORKER_EXEC_PATH = std::string(argv[4]);
  ray::TEST_GCS_SERVER_EXEC_PATH = std::string(argv[5]);
  ray::TEST_REDIS_SERVER_EXEC_PATH = std::string(argv[6]);
  ray::TEST_REDIS_MODULE_LIBRARY_PATH = std::string(argv[7]);
  ray::TEST_REDIS_CLIENT_EXEC_PATH = std::string(argv[8]);
  return RUN_ALL_TESTS();
}
