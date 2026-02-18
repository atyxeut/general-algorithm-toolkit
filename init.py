# Copyright (C) 2026 atyxeut
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import shutil
import subprocess
from pathlib import Path

# > python3 init.py llvm
#   specify `llvm` toolchain and build in debug mode (default)
# > python3 init.py llvm release
#   specify `llvm` toolchain and build in release mode
# > python3 init.py gcc
#   specify `gcc` toolchain and build in debug mode (default)
# > python3 init.py gcc release
#   specify `gcc` toolchain and build in release mode
parser = argparse.ArgumentParser(add_help=False)
parser.add_argument("toolchain", type=str)
parser.add_argument("build_mode", type=str, nargs="?", default="debug")
argv = parser.parse_args()


def remove(path: Path):
  if not path.exists():
    return
  if path.is_dir():
    shutil.rmtree(path)
  else:
    path.unlink()


def main():
  remove(Path(".cache"))
  remove(Path(".xmake"))
  remove(Path("build"))

  subprocess.run(["xmake", "f", "-v", "--toolchain=" + argv.toolchain, "-m", argv.build_mode])
  subprocess.run(["xmake", "project", "-k", "compile_commands", "--outputdir=build"], text=True, stdout=subprocess.PIPE)


if __name__ == "__main__":
  main()
