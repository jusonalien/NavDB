cmake_minimum_required(VERSION 2.8.1)

project(bplustree)

aux_source_directory(. DIR_SRCS)

add_executable(bplustree ${DIR_SRCS})