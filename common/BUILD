cc_library(
    name = "point",
    srcs = ["point.cpp"],
    hdrs = ["point.h"],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_test(
    name = "test",
    srcs = ["test.cpp"],
    deps = [
        ":point",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
