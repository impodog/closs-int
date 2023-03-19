import os,sys,shutil,re,subprocess
absdir = os.path.abspath(os.path.split(__file__)[0])

print()

src_path = input("Input the source path(default=../closs-int) : ")
if len(src_path) == 0:
    src_path = "../closs-int"
src_path = os.path.abspath(os.path.join(absdir,src_path))
print("Later we'll copy files from : \"%s\".\n" % src_path)

path_input = input("Input the path to copy to(default=.) : ")
if len(path_input) == 0:
    path_input = "."
dst_path = os.path.abspath(os.path.join(absdir,path_input))
print("The copy will be pasted into : \"%s\".\n" % dst_path)

try:
    os.chdir(src_path)
except:
    raise RuntimeError("Cannot find the directory to copy from. Abort.")

with open("src/const.h","r") as file:
    content = file.read()
    result = re.search(r"#define\s+VERSION\s+\"(v[\d\.]+)\"",content)
    if result is None:
        raise RuntimeError("Cannot find the version info specified. Abort.")
    else:
        version = result.group(1)
        print("Version Found : " + version)
os.chdir(dst_path)
print("Source file check is done.\n")
    
print("Input your commit message (hit ENTER to use default) : ")
message = ""
while True:
    i = input()
    if len(i) == 0:
        message = message[:-1]
        break
    message += i + "\n"
if len(message) == 0:
    message = "Syncing version " + version + " from gitee repo"
print("The commit message will be : \"%s\".\n" % message)


confirm = input("Confirm everything the ONE LAST TIME (p)roceed/(A)bort : ").lower()

if confirm == "p":
    shutil.copytree(src_path,dst_path,ignore=lambda src,name:".git",dirs_exist_ok=True)
    subprocess.run(["git","add","."])
    subprocess.run(["git","commit","-m",repr(message)])
    subprocess.run(["git","push"])
elif confirm == "a":
    print("Aborted by user.")
else:
    print("Aborted by no action taken.")