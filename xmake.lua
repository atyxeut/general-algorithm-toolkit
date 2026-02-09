set_xmakever("3.0.6")
set_project("aty-general-algorithm-toolbox")

add_rules("mode.debug", "mode.release")

set_config("builddir", "build")

if is_host("window") then
  -- recommended gcc binary: https://gcc-mcf.lhmouse.com/
  set_config("plat", "mingw")
end

set_languages("c++latest")

if get_config("toolchain") == "llvm" then
  if is_host("windows") then
    -- binary from https://github.com/mstorsjo/llvm-mingw
    local llvm_sdk_dir = "C:/llvm-mingw-ucrt-x86_64"
    -- suppress `warning: std and std.compat modules not found! maybe try to add --sdk=<PATH/TO/LLVM> or install libc++`
    set_policy("build.c++.modules.std", false)

    target("std_modules")
      set_kind("object")
      add_includedirs(llvm_sdk_dir .. "/include/c++/v1")
      add_files(llvm_sdk_dir .. "/share/libc++/v1/*.cppm", { public = true })
    target_end()
  elseif is_host("macosx") then
    set_policy("build.c++.modules", true)
    -- binary from Homebrew: brew install llvm
    add_ldflags("-L/usr/local/opt/llvm@21/lib/c++")
  end
end

if get_config("toolchain") == "gcc" and is_host("windows") then
  -- gcc 16 still requires this on Windows
  add_syslinks("stdc++exp")
end

target("gatk")
  set_kind("object")
  if get_config("toolchain") == "llvm" and is_host("windows") then
    add_deps("std_modules")
  end
  add_files("src/**.cppm", { public = true })
target_end()

target("test_main")
  set_kind("binary")
  add_deps("gatk")
  add_files("test/main.cpp")
  set_optimize("faster")
target_end()
