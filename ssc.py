# Single Source C
# This just combines multiple C source files into one while taking care of imports and other things

# Things that won't be done
# - Statics won't be name mangled
# - __LINE__ and __FILE__ won't work well

import sys
import pathlib
import shutil

# note: all files have to be in same directory
def add_file(imports, file_name, out_file, header_guard):
  if file_name in imports: return

  if not file_name.endswith(".incl"): imports.append(file_name)

  if not pathlib.Path(file_name).is_file(): return

  with open(file_name) as f:
    for line in f:
      if line.strip().startswith("#include"):
        to_import = line.split("#include")[1].strip()
        if to_import.startswith('"'):
          # local incl
          fname = to_import.strip('"')
          if '/' in fname: fname = fname.split('/')[-1]
          fname = 'tmp_dir/' + fname
          add_file(imports, fname, out_file, header_guard)
        elif to_import not in imports:
          # system incl
          imports.append(to_import)
          out_file.write(line)
      elif not line.startswith("#") or header_guard not in line or "_H" not in line:
        out_file.write(line)

def filter_multiple_newlines(out_name, file_name):
  prev_empty = False
  with open(out_name, "r") as in_file:
    with open(file_name, "w") as out:
      for line in in_file:
        stripped_line = line.strip()
        if stripped_line:
          prev_empty = False
          out.write(line)
        elif not prev_empty:
          out.write(line)
          prev_empty = not stripped_line

def files_in_folder(folder):
  return (str(f) for f in pathlib.Path(folder).rglob("*") \
              if f.is_file() and f.suffix in ['.c', '.h', '.incl'])

def main():
  if len(sys.argv) < 5:
    print("Usage is: python3 ssc.py <out_name> "
          "<header_guard_start> <paths>")
    return
  shutil.rmtree('tmp_dir', ignore_errors=True)
  pathlib.Path('tmp_dir').mkdir(parents=False, exist_ok=False)
  out_name = sys.argv[1]
  out_is_header = out_name.endswith(".h")
  header_guard = sys.argv[2]
  paths = sys.argv[3:]
  imports = []

  with open('tmp_file', "w") as out:
    out.write("/* AUTO GENERATED */\n")
    if out_is_header: out.write("#ifndef " + header_guard + "_H\n#define " + header_guard + "_H\n\n")
    for f in [f for path in paths for f in files_in_folder(path)]:
      shutil.copy(f, "tmp_dir")

    all_files = sorted(files_in_folder('tmp_dir'), key = lambda x: x[-2:] != ".h")

    for f in all_files:
      if not f.endswith('.incl'): add_file(imports, f, out, header_guard)
    if out_is_header: out.write("\n#endif\n")

  shutil.rmtree('tmp_dir', ignore_errors=True)
  filter_multiple_newlines('tmp_file', out_name)
  pathlib.Path('tmp_file').unlink()

if __name__ == "__main__":
  main()
