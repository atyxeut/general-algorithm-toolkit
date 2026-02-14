-- Copyright 2026 atyxeut
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

set_xmakever("3.0.6")
set_project("atyxeut-algorithm-toolkit")
set_languages("c++latest")

add_rules("mode.debug", "mode.release")

if is_mode("release") then
  add_defines("NDEBUG")
end

set_config("builddir", "build")
set_config("runtimes", "c++_shared")
if is_host("windows") then
  set_config("plat", "mingw")
end

-- recommended llvm binary: https://github.com/mstorsjo/llvm-mingw
if get_config("toolchain") == "llvm" and is_host("macosx") then
  set_policy("build.c++.modules", true)
  -- binary from Homebrew: brew install llvm
  add_ldflags("-L/usr/local/opt/llvm@21/lib/c++")
end

-- recommended gcc binary: https://gcc-mcf.lhmouse.com/
if get_config("toolchain") == "gcc" and is_host("windows") then
  -- gcc 16 still requires this on Windows
  add_syslinks("stdc++exp")
end

includes("src")
includes("test")
