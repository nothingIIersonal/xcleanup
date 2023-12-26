# xcleanup
I hope this helps ease your headache of getting and freeing heap pointers, file descriptors, etc, before exit.
<br />
Too bad there's no RAII, but we do have <b>atexit()</b>. Thanks for that at least...

# Usage
Include <b><xcleanup.h></b> in your project. An example of use can be found in the <b>main.c</b>. Build with <b>clang</b>/<b>gcc</b>.
<br />

To build an example (unix), type:
```bash
mkdir .build
cd .build
cmake ..
cmake --build .
```
<br />

On Windows, for example, you can use <b>nmake</b>:
```bash
mkdir .build
cd .build
cmake .. -G"NMake Makefiles" -DCMAKE_C_COMPILER=clang
nmake
```
<br />

To leak-check (unix), type:
```bash
cmake --build . --target leak-check
```