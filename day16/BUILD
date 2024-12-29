cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    visibility = ["//visibility:public"],
    deps = [":lib"],
)

cc_library(
    name = "lib",
    srcs = ["lib.cpp"],
    hdrs = ["lib.h"],
    visibility = ["//visibility:private"],
    deps = [
        "//common:point",
    ],
)

cc_test(
    name = "test",
    srcs = ["test.cpp"],
    deps = [
        ":lib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
