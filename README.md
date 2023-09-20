<img alt="librucksack logo" src="rucksack.png" width="250px" height="250px">

# librucksack

librucksack carries the files for another executable in memory.

If you want to have self-contained executables, you can [statically link](https://en.wikipedia.org/wiki/Static_build) them at compile time.
This will copy all the required libraries directly into your executable. This way, if you're on a different system, you don't need to install all required shared libraries.

However, this doesn't take files into consideration which might be opened by the executable at runtime.

Let's take [nmap](https://nmap.org) as an example: while you can statically link it so you can copy it onto other hosts for scanning purposes, most of nmap's powers lie in the scripting engine and its scripts. Those are installed into `/usr/share/nmap` when you `apt-get install nmap`; but in most cases, you need adminstrator rights for this installation command.

librucksack addresses this by hooking to [libc](https://gnu.org/software/libc/) commands to interact with files and answer them from memory if possible. 

## Features

- Support for **sys/stat.h** functions `stat`, `fstat`, `lstat`, `fstatat`
- Support for **stdio.h** functions `fopen`, `fdopen`, `freopen`, `fread`, `fwrite`
- Support for **unistd.h** functions `open`, `read`, `write`, `close`, `access`

## Usage

To get your rucksack ready for the journey, you need to pack it first. **Packing** means collecting all files which should be delivered from librucksack and having them in a rucksack-consumable format.

The steps are:

- **Pack** the files you want to deliver using `src/tools/pack.py`
- **Compile** librucksack with the new pack file
- **Use** librucksack!

Let's make an example by packing `/etc/passwd`, as it is opened by nearly every executable upon start:

- Make sure your project directory is tidy by running `make clean`
- Pack it by running `src/tools/pack.py --output src/lib/pack /etc/passwd`
- Compile it with `make dist`
- Enjoy your packed rucksack at `src/lib/librucksack.so`!

You can test if it works by using [strace]():
The command `strace --env=LD_PRELOAD=./src/lib/librucksack.so --trace=openat cat /etc/passwd` will print out all system calls executed by the binary (`cat` in this case), filtering for `openat` calls. Compare the output with the same `strace` call, but without the `--env=LD_PRELOAD...` part. See the difference?

## Contributions

Yes please! There is still alot of functionality missing.

As a start, why don't you have a look for *TODO* markers inside the source code?
