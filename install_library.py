import os
import subprocess
import shutil


def run_command(command, cwd=None):
    print(f"Running command: {command}")  # Print the command being run
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=cwd, shell=True)
    while True:
        output = process.stdout.readline()
        if output == b'' and process.poll() is not None:
            break
        if output:
            print(output.decode().strip())
    stderr = process.communicate()[1]
    if stderr:
        print(stderr.decode().strip())


def install_library():
    cmake_build_command = (
        f"cmake -B _builds "
        f"-DCMAKE_INSTALL_PREFIX=\"C:\\Users\\User\\Developments\\SharkCardGame\\vendors\\cbit2d\" -G \"MinGW Makefiles\" "
        f"-DCMAKE_CXX_STANDARD=17"
    )

    run_command(cmake_build_command)

    cmake_install_command = (
        f"cmake --build _builds --target install"
    )
    run_command(cmake_install_command)


def delete_build_folder():
    shutil.rmtree('_builds', ignore_errors=True)

def main():
    delete_build_folder()
    install_library()