"""
Aspects with binary and config defaults.
"""

load("@bazel_clang_format//:defs.bzl", "make_clang_format_aspect")
load("@bazel_clang_tidy//:defs.bzl", "make_clang_tidy_aspect")

clang_format = make_clang_format_aspect(
    binary = "@llvm_toolchain//:clang-format",
    config = "@//:format_config",
)

clang_tidy = make_clang_tidy_aspect(
    binary = "@llvm_toolchain//:clang-tidy",
    config = "@//:tidy_config",
)
