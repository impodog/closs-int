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

src_const = os.path.join(src_path,"src/const.h")
try:
    print("Looking for src version in \"%s\"..." % src_const)
    with open(src_const,"r") as file:
        content = file.read()
        result = re.search(r"#define\s+VERSION\s+\"(v[\d\.]+)\"",content)
        if result is None:
            raise RuntimeError("Cannot find the source version info specified. Abort.")
        else:
            src_version = result.group(1)
            print("Source version found : " + src_version)
except FileNotFoundError:
    raise RuntimeError("Cannot find \"const.h\" as the source path inputed. Abort.")
dst_const = os.path.join(dst_path,"src/const.h")
try:
    print("Looking for dest version in \"%s\"..." % dst_const)
    with open(dst_const,"r") as file:
        content = file.read()
        result = re.search(r"#define\s+VERSION\s+\"(v[\d\.]+)\"",content)
        if result is None:
            raise RuntimeError("Cannot find the dest version info specified. Abort.")
        else:
            dst_version = result.group(1)
            print("Dest version found : " + dst_version)
except FileNotFoundError:
    raise RuntimeError("Cannot find \"const.h\" as the dest path inputed. Abort.")
if src_version == dst_version:
    print("Source and dest versions are the same. No need to sync.")
    exit(0)
print("File check is done.\n")
    
print("Input your commit message (hit ENTER to use default) : ")
message = ""
while True:
    i = input()
    if len(i) == 0:
        message = message[:-1]
        break
    message += i + "\n"
if len(message) == 0:
    message = "Syncing version " + src_version + " from gitee repo"
print("The commit message will be : \"%s\".\n" % message)


confirm = input("Confirm everything the ONE LAST TIME (p)roceed/(A)bort : ").lower()

if confirm == "p":
    shutil.copytree(src_path,dst_path,ignore=lambda src,name:".git",dirs_exist_ok=True)
    subprocess.run(["git","add","."])
    subprocess.run(["git","commit","-m",repr(message)])
    try:
        subprocess.run(["git","push"])
        print("Syncing Complete!")
    except:
        print("ERROR : Cannot push up to dest repo.")
elif confirm == "a":
    print("Aborted by user.")
else:
    print("Aborted by no action taken.")