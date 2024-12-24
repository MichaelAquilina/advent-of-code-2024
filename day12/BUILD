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
    deps = [":map"],
)

cc_library(
    name = "map",
    srcs = ["map.cpp"],
    hdrs = ["map.h"],
    visibility = ["//visibility:private"],
    deps = [":point"],
)

cc_library(
    name = "point",
    srcs = ["point.cpp"],
    hdrs = ["point.h"],
    visibility = ["//visibility:private"],
    deps = [],
)
