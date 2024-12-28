# BUILD file in src directory

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        ":lib",
    ],  # Add dependencies here
)

cc_library(
    name = "lib",
    srcs = ["lib.cpp"],
    hdrs = ["lib.h"],
    visibility = ["//visibility:private"],
    deps = [
        "@abseil-cpp//absl/strings"
    ],
)

cc_test(
    name = "test",
    srcs = ["test.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        ":lib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
