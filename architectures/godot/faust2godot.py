import os
import platform
import sys
import subprocess
import json
import shutil
from typing import Optional
from pathlib import Path

godot_project_name = "faust2godot/"


def get_vs_tool_path() -> Optional[Path]:
    # identify where is Visual Studio Install folder
    vs_where_path = os.path.join(os.environ['ProgramFiles(x86)'], 'Microsoft Visual Studio', 'Installer', 'vswhere.exe')

    if not os.path.exists(vs_where_path):
       return None

    output = subprocess.check_output([vs_where_path, "-utf8", "-prerelease", "-latest", "-property", "installationPath"]).decode("utf-8")

    # identify where dev command prompt is
    for vs_dir in output.splitlines():
        devcmd_path = vs_dir + "/VC/Auxiliary/Build/vcvars64.bat"
        if os.path.exists(devcmd_path):
            return Path(devcmd_path)

    return None

# get faust arch dsp directory
def get_arch_path():
    try:
        result = subprocess.run(
            ['faust', '--dspdir'],
            capture_output=True,
            text=True,
            check=True
        )
        path = result.stdout.strip()
        if os.path.isdir(path):
            return Path(path).resolve()
    except Exception as e:
        print("ERROR : Faust directory not found.\n",
          "Make sure that Faust is correctly installed.", e)
        exit()
    return Path()

def get_include_path() -> Path:
    try:
        result = subprocess.run(
            ['faust', '--includedir'],
            capture_output=True,
            text=True,
            check=True
        )
        path = result.stdout.strip()
        if os.path.isdir(path):
            return Path(path).resolve()
    except Exception as e:
        print("ERROR : Faust include directory not found.\n",
              "Make sure that Faust is correctly installed.", e)
        exit()
    return Path()


def get_platform_decorator() -> tuple[str, str]:
    if platform.system() == "Linux":
        return "lib", ".so"

    if platform.system() == "Windows":
        return "", ".dll"

    if platform.system() == "Darwin":
        return "lib", ".dylib"

    print("Error : Platform not supported.")
    return "",""

def print_help_message():
    # use the usage.sh script to stay coherent with other faust2xx tools
    print("Usage: python faust2godot.py [options] [Faust options] <file.dsp>")
    print("Compiles Faust programs to GDExtension usable in Godot game engine")
    print("   -as-bus-effect : force dsp to be compiled as Godot effect (even if it has no inputs)")
    print("   -nvoices [nbr] : compile DSP as polyphonic DSP with [nbr] voices)")
    print("   -effect        : add global effect for every voices (for polyphonic DSPs)")

# print success message in a nice box
def print_success_message():
    msg = "DONE. Open the " + godot_project_name + " directory inside Godot"
    row = len(msg)
    h = ''.join(['+'] + ['-' *row] + ['+'])
    result = '\n'+ h + '\n'"|"+msg+"|"'\n' + h
    print(result)
    print("Check https://github.com/MithanielVillard/Faust2Godot/blob/main/documentation/user-documentation.md for usage and documentation \n")

def get_faust_opt_flags() -> str:
    system = platform.system()
    machine = platform.machine()
    output_path = "faust2godot/bin/" + platform_decorator[0] + "faustdsp" + platform_decorator[1]

    if system == 'Darwin':
        if machine == 'arm64':
            # Silicon MX
            compile_flags_ = "-std=c++11 -Ofast"
        else :
            compile_flags_ = "-std=c++11 -Ofast -march=native"

        compile_flags_ += " -fPIC -shared -pthread"
        compile_flags_ += " -o " + output_path

    #MSVC Flags
    elif system == 'Windows':
        compile_flags_ = "& cl /EHsc /O2 /LD"
        compile_flags_ += " -o " + output_path
    else :
        compile_flags_ = "-std=c++11 -Ofast -march=native"
        compile_flags_ += " -o " + output_path

    if 'CXXFLAGS' in os.environ:
        compile_flags_ += " " + os.environ["CXXFLAGS"]

    # Set default values for CXX and CC
    if 'CXX' not in os.environ:
        if system == 'Windows':
            cmd_path = get_vs_tool_path()
            if cmd_path :
                print("Compiling using MSVC Compiler")
                os.environ['CXX'] = str(cmd_path)
        else :
            os.environ['CXX'] = "c++"

    if 'CC' not in os.environ:
        os.environ['CC'] = "cc"

    return compile_flags_

def compile_dsp(dsp_file: str, flags : list[str], ):
    try:
        subprocess.run(
            ["faust"] + flags + [dsp_file],
            capture_output=True,
            text=True,
            check=True
        )
    except subprocess.CalledProcessError as e:
        handle_error(e.stderr, dsp_file)

def compile_to_lib(dsp_file: str, flags : list[str]):
    cxx = os.environ["CXX"]
    try:
        subprocess.run(
            [cxx] + flags + [dsp_file + ".cpp"],
            # capture_output=True,
            text=True,
            check=True,
            shell=True
        )
    except subprocess.CalledProcessError as e:
        handle_error(e.stderr, dsp_file)

def compile_dsp_files(files: list[str], _compile_flags : str):
    for dsp in files:
        print(f"Compiling {dsp} to C++ using architecture file.")
        compile_dsp(dsp, ["-i", "-a", str(arch_path) + "/godot/godot.cpp", "-o", dsp + ".cpp"] + options)

        if not force_effect:
            print(f"Compiling '{dsp}' to JSON.")
            compile_dsp(dsp, ["-json", "-o", dsp + ".json"])

            with open(f'{dsp}.json', "r", encoding="utf-8") as f:
                as_string = f.read().replace('\\', '//')
                d = json.loads(as_string)

            if d['inputs'] <= 0:
                print(f"{dsp} detected as generator.")
                _compile_flags += " -DGENERATOR_DSP=1"

            os.remove(dsp+".json")

        print(f"Compiling '{dsp}' to GDExtension.")
        compile_to_lib(dsp, _compile_flags.split(" "))

        # Remove temporary files
        os.remove(dsp+".cpp")
        if os.path.exists(dsp+".h"):
            os.remove(dsp+".h")

        if os.path.exists(dsp+".obj"):
            os.remove(dsp+".obj")

        # collect binary file name for FaustWorks
        binaries.append(dsp)


def handle_error(error : Exception, failed_file : str):
    print("ERROR : failed to compile DSP : '" + failed_file + "'")
    print(error)

    if os.path.exists(godot_project_name):
        shutil.rmtree(godot_project_name)
    exit()


if __name__ == '__main__':
    print("\n[--------- Faust2Godot static compilation tool ----------]\n")

    arch_path = get_arch_path()
    platform_decorator = get_platform_decorator()
    compile_flags = get_faust_opt_flags()
    options = []
    force_effect = False

    dsp_files = []
    binaries = []

    if len(sys.argv) == 1:
        print("Please, provide a Faust file to process !")
        exit()

    params = sys.argv
    params.pop(0)

    if params[0] == '-help' or params[0] == '-h':
        print_help_message()
        exit()

    for i in range(len(params)):
        if params[i] == '-fx':
            include_path = get_include_path()
            compile_flags += " -I " + str(include_path) +"/ap_fixed -DFIXED_POINT"
            options += " -fx"
        elif params[i] == '-as-bus-effect':
            force_effect = True
        elif params[i] == '-nvoices':
            if i+1 > len(params) - 1:
                print("Please, provide a number of voices !")
                exit()
            nbr_voices = int(params[i+1])
            print("Compiling dsp as polyphonic")
            print(f"Number of voices : {nbr_voices}")
            compile_flags +=f" -DPOLY -DNVOICES={nbr_voices}"
            i+=1
        elif params[i] == '-effect':
            if i+1 > len(params) - 1:
                print("Please, provide a Faust file to process for global effect !")
                exit()
            compile_flags += " -DGLOBAL_EFFECT"
            compile_dsp(params[i + 1], ["-i", "-cn effect", "-a minimal-effect.cpp", "-o effect.h"])
            i+=1
        elif params[i].startswith('-'):
            options.append(params[i])
        elif len(params[i]) > 4 and params[i].endswith('.dsp'):
            dsp_files.append(params[i])

    if len(dsp_files) == 0:
        print("Please, provide a Faust file to process !")
        exit()

    # Copy godot project template from arch directory
    shutil.copytree(arch_path.joinpath("godot/template"), godot_project_name, dirs_exist_ok=True)

    compile_dsp_files(dsp_files, compile_flags)
    print_success_message()

    for binary in binaries:
        print(binary)