@echo off

bjam stage --toolset=msvc-14.0 architecture=x86 address-model=64 --without-graph --without-graph_parallel --stagedir="C:\local\boost_1_67_0\vc14-x64" link=static runtime-link=shared runtime-link=shared threading=multi debug release