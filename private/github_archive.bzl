"""
get http_archives from github
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def github_archive(name = "", repo = "", commit = "", sha256 = "", build_file_content = None):
    http_archive(
        name = name,
        sha256 = sha256,
        strip_prefix = "{}-{}".format(repo.split("/")[1], commit),
        url = "https://github.com/{}/archive/{}.tar.gz".format(repo, commit),
        build_file_content = build_file_content,
    )
