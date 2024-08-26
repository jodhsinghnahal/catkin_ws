set(cpp_redis_sources
  sources/builders/array_builder.cpp
  sources/builders/builders_factory.cpp
  sources/builders/bulk_string_builder.cpp
  sources/builders/error_builder.cpp
  sources/builders/integer_builder.cpp
  sources/builders/reply_builder.cpp
  sources/builders/simple_string_builder.cpp
  sources/core/client.cpp
  sources/core/consumer.cpp
  sources/core/reply.cpp
  sources/core/sentinel.cpp
  sources/core/subscriber.cpp
  sources/core/types.cpp
  sources/misc/dispatch_queue.cpp
  sources/misc/logger.cpp
  sources/network/redis_connection.cpp
  sources/network/tcp_client.cpp
  )

set(cpp_redis_includes
  includes/cpp_redis
  )
