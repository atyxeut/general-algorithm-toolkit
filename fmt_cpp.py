# Copyright 2026 atyxeut
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
