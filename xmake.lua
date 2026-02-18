-- Copyright (C) 2026 atyxeut
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

-- recommend llvm binary: https://github.com/mstorsjo/llvm-mingw
if get_config("toolchain") == "llvm" and is_host("macosx") then
  set_policy("build.c++.modules", true)
  -- binary from Homebrew: brew install llvm
  add_ldflags("-L/usr/local/opt/llvm@21/lib/c++")
end

-- recommend gcc binary: https://gcc-mcf.lhmouse.com/
if get_config("toolchain") == "gcc" and is_host("windows") then
  -- gcc 16 still requires this on Windows
  add_syslinks("stdc++exp")
end

includes("src")
includes("test")
