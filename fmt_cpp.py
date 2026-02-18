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
import subprocess
import sys
from pathlib import Path

# > python3 fmt_cpp.py path
#                       ↑ this is required
#   1. if path is a folder, recursively format every subpath in it
#   2. if path is a .hpp/.cppm/.cpp/ file, format it using clang-format, styles are defined in .clang-format
parser = argparse.ArgumentParser(add_help=False)
parser.add_argument("path", type=str)
argv = parser.parse_args()


def fmt_cpp(path: Path):
  if path.is_dir():
    for item in path.iterdir():
      fmt_cpp(item)
  elif path.suffix == ".hpp" or path.suffix == ".cpp" or path.suffix == ".cppm":
    print(f"formatting {path}")
    subprocess.run(["clang-format", "-style=file:.clang-format", "-i", path])


def main():
  fmt_cpp(Path(argv.path).resolve())
  print("format done", file=sys.stderr)


if __name__ == "__main__":
  main()
